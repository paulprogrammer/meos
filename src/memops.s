	[bits 64]

	section memops
	global bzero

bzero:
	push rcx					; save values from the 2 registers we will use
	push rbx
	xor rax, rax				; rax = 0 (return the number of cells zeroed)
	mov rcx, rsi				; the number of elements (u8byte) to zero out
	mov rbx, rdi				; the address to start zeroing

bzero_zero_mem_op:
	mov BYTE [rbx+rcx], 0		; zero out the memory cell

	loop bzero_zero_mem_op

	pop rbx						; return the registers to their initial state
	pop rcx
	ret 						; all done!