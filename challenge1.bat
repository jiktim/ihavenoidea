i686-elf-gcc -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
i686-elf-as unrealMode.s -o unrealMode.o
i686-elf-as boot.s -o boot.o
i686-elf-gcc -T linker.ld -o ihavenoidea.bin -ffreestanding -O2 -nostdlib boot.o unrealMode.o kernel.o -lgcc
qemu-system-i386 -kernel ihavenoidea.bin
