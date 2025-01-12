LD=ld
ASM=nasm
CC=gcc

.PHONY: all run clean build
all: run
build: os-image.bin

kernel.bin: kernel-entry.o kernel.o
	$(LD) -m elf_i386 -o $@ -Ttext 0x1000 $^ --oformat binary

kernel-entry.o: kernel-entry.asm
	$(ASM) $< -f elf -o $@

kernel.o: kernel.asm
	#$(CC) -m32 -fno-pic -ffreestanding -c $< -o $@
	$(ASM) $< -f elf -o $@

mbr.bin: mbr.asm switch-to-32bit.asm gdt.asm disk.asm 
	$(ASM) $< -f bin -o $@

os-image.bin: mbr.bin kernel.bin
	cat $^ > $@

run: os-image.bin
	qemu-system-i386 -vnc 0.0.0.0:0 -fda $<

clean:
	$(RM) *.bin *.o *.dis
