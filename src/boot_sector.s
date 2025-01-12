;; src/boot_sector.s

	section .boot_sector
	global __start

	[bits 16]

__start:
	mov bx, hello_msg
	call print_string

	mov si, disk_address_packet
	mov ah, 0x42		; BIOS extended read function
	mov dl, 0x80		; drive number
	int 0x13			; bios disk call
	jc error_reading_disk

ignore_disk_read_error:
	SND_STAGE_ADDR equ (BOOT_LOAD_ADDR + SECTOR_SIZE)
	jmp 0:SND_STAGE_ADDR

error_reading_disk:
	cmp word [dap_sectors_num], READ_SECTORS_NUM
	jle ignore_disk_read_error

	mov bx, error_reading_disk_msg
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

	align 4
disk_address_packet:
	db 0x10				; size of packet
	db 0 				; reserved; always 0
dap_sectors_num:
	dw READ_SECTORS_NUM ; number of sectors
	dd (BOOT_LOAD_ADDR + SECTOR_SIZE) ; destination address
	dq 1 				; sector start at

READ_SECTORS_NUM equ 64
BOOT_LOAD_ADDR equ 0x7c00
SECTOR_SIZE equ 512

hello_msg: db "Hello world!", 0x0d, 0x0a, 0
error_reading_disk_msg: db "Error: failed to read disk with 0x13/ah=0x42", 0x0d, 0x0a, 0