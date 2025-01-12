section .text
	global main

main:
mov al, 'H'
int 0x10
jmp $