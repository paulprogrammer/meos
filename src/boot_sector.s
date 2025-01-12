;; src/boot_sector.s

	section .boot_sector
	global __start

	[bits 16]

__start:
	mov bx, hello_msg
	call print_string

end:
	hlt
	jmp $

;; print a null terminated string using bios routines
print_string:
	pusha
	mov ah, 0x0e		; bios print char function

print_string_loop:
	cmp byte [bx], 0
	je print_string_return

	mov al, [bx]
	int 0x10

	inc bx
	jmp print_string_loop

print_string_return:
	popa
	ret

hello_msg: db "Hello world!", 0