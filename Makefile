only_boot: boot.asm
	nasm -f bin boot.asm -o boot.bin
	qemu-system-i386 -drive format=raw,file=boot.bin

os: boot.asm kernel.c linker.ld
	nasm -f bin boot.asm -o boot.bin
	gcc -ffreestanding -c kernel.c -o kernel.o
	ld -T linker.ld kernel.o -o kernel.bin
	cat boot.bin kernel.bin > os.bin
	qemu-system-i386 -drive format=raw,file=os.bin

clear:
	rm -f *.o *.bin *.elf app