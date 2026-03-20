BUILD = build

os: boot.asm arch/entry.asm kernel.c linker.ld
	mkdir -p $(BUILD)
	nasm -f bin boot.asm -o $(BUILD)/boot.bin
	nasm -f elf32 arch/entry.asm -o $(BUILD)/entry.o
	gcc -ffreestanding -m32 -fno-pic -I include -c kernel.c -o $(BUILD)/kernel.o
	ld -T linker.ld -m elf_i386 $(BUILD)/entry.o $(BUILD)/kernel.o -o $(BUILD)/kernel.bin
	cat $(BUILD)/boot.bin $(BUILD)/kernel.bin > $(BUILD)/os.bin
	qemu-system-i386 -drive format=raw,file=$(BUILD)/os.bin,if=floppy

clean:
	rm -rf $(BUILD)