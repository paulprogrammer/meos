[bits 16]
org 0x7c00

; where to load the kernel
KERNEL_OFFSET equ 0x1000

BOOT_DRIVE db 0

; BIOS sets the boot drive in 'dl' register; store for later
mov [BOOT_DRIVE], dl

; initialize stack
mov bp, 0x9000
mov sp, bp

push ax
mov al, '1'
call printit
pop ax

call load_kernel

push ax
mov al, '2'
call printit
pop ax

call switch_to_32bit

jmp $

%include "disk.asm"
%include "gdt.asm"
%include "switch-to-32bit.asm"

[bits 16]
load_kernel:
	mov bx, KERNEL_OFFSET
	mov dh, 2
	mov dl, [BOOT_DRIVE]
	push ax
	mov al, 'a'
	call printit
	pop ax
	call disk_load
	ret

[bits 32]
BEGIN_32BIT:
	;call KERNEL_OFFSET
	call printit
	jmp $

[bits 16]
printit:
pusha
; set BIOS print screen settings
mov ah, 0x0e  	; teletype
xor bh, bh 		; page number
mov bl, 4 		; red on black
mov cx, 1 		; write one character

; print on screen
; mov al, 'X'
int 0x10 		; bios print interrupt

popa
ret
;jmp $			; loop forever

; padding
times 510 - ($-$$) db 0

; magic number
dw 0xaa55
