#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};
 
static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) 
{
	return fg | bg << 4;
}
 
static inline uint16_t vga_entry(unsigned char uc, uint8_t color) 
{
	return (uint16_t) uc | (uint16_t) color << 8;
}
 
size_t strlen(const char* str) 
{
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}
 
static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 60;
 
size_t row;
size_t column;
uint8_t color;
 
/* Note the use of the volatile keyword to prevent the compiler from eliminating dead stores. */
volatile uint16_t* buffer;
extern void entering_v86(uint32_t ss, uint32_t esp, uint32_t cs, uint32_t eip);

/* get functions */
register uint32_t esp asm("esp");
register uint32_t eip asm("eip");
register uint32_t es asm("es");
register uint32_t cs asm("cs");
register uint32_t ss asm("ss");
/* sorry for this terrible copy-pasted code ? */
void initialize(void) 
{
	row = 0;
	column = 0;
	color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	buffer = (uint16_t*) 0xB8000;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			buffer[index] = vga_entry(' ', color);
		}
	}
}
 
void *memset(void *dest, char val, size_t count) {
    char *temp = (char *)dest;
    for( ; count != 0; count--) *temp++ = val;
    return dest;
}
void setcolor(uint8_t color2) 
{
	color = color2;
}
 
void putentryat(char c, uint8_t color, size_t x, size_t y) 
{
	const size_t index = y * VGA_WIDTH + x;
	buffer[index] = vga_entry(c, color);
}
 
void tputchar(char c) 
{
	putentryat(c, color, column, row);
	if (++column == VGA_WIDTH) {
		column = 0;
		if (++row == VGA_HEIGHT)
			row = 0;
	}
}
 
void write(const char* data, size_t size) 
{
	for (size_t i = 0; i < size; i++)
		tputchar(data[i]);
}
 
void writestring(const char* data) 
{
	write(data, strlen(data));
}
void initialize13h(void) 
{
	row = 0;
	column = 0;
	color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	buffer = (uint16_t*) 0xA0000;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			buffer[index] = vga_entry(' ', color);
		}
	}
}
 
static void putpixel(int x,int y, int color) {
    unsigned where = x*4 + y*3200;
    unsigned char* screen = (unsigned char*) 0xA0000;
    screen[where] = color & 255;              // BLUE
    screen[where + 1] = (color >> 8) & 255;   // GREEN
    screen[where + 2] = (color >> 16) & 255;  // RED
}

int main(void) 
{
	entering_v86(ss, esp, cs, eip); //bad
	return 1;
}
