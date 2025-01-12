; Take advantage of the fact we're still in 16-bit mode,
; so we can use BIOS calls to manage disk operations.

disk_load:
	pusha
	push dx

	mov ah, 0x02 ; read mode
	mov al, dh   ; read dh number of sectors
	mov cl, 0x02 ; start from sector 2 (sector 1 is boot sector)
	mov ch, 0x00 ; cylinder 0
	mov dh, 0x00 ; head 0

	; dl => drive number is set as input to disk_load
	; es:bx => buffer pointer is set as input

	int 0x13       ; BIOS interrupt
	jc disk_error  ; check carry bit for error

	pop dx       ; get back original number of sectors to read
	cmp al, dh   ; BIOS sets 'al' to the # of sectors read
	             ; compare to 'dh' and error if not equal
	jne sectors_error
	popa
	ret

disk_error:
	jmp disk_loop

sectors_error:
	jmp disk_loop

disk_loop:
	jmp $
