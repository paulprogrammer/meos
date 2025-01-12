[bits 16]
switch_to_32bit:
	push ax
	mov al, '3'
	call printit
	pop ax

	cli											; 1. disable interrupts
	lgdt [gdt_descriptor]		; 2. load gdt descriptor
	call OpenA20Gate

	push ax
	mov al, '5'
	call printit
	pop ax

	mov eax, cr0

	push ax
	mov al, '5'
	call printit
	pop ax

	or eax, 0x1							; 3. enable protected mode
	jmp $
	mov cr0, eax

	push ax
	mov al, '6'
	call printit
	pop ax

	jmp CODE_SEG:init_32bit ; 4. far jump to flush pipeline

OpenA20Gate:
    in al, 0x93         ; switch A20 gate via fast A20 port 92

    or al, 2            ; set A20 Gate bit 1
    and al, ~1          ; clear INIT_NOW bit
    out 0x92, al

    ret

[bits 32]
init_32bit:
	push ax
	mov al, '7'
	call printit
	pop ax
	mov ax, DATA_SEG				; 5. update segment registers
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	mov ebp, 0x90000				; 6. set up stack
	mov esp, ebp

	call BEGIN_32BIT				; 7. move back to mbr.asm
