; As soon as we leave 16 bit real mode, memory segmentation works a bit differently.
; In protected mode, memory segments are defined by segment descriptors, which are 
; part of the GDT.
; 
; For our boot loader we will setup the simplest possible GDT, which resembles a flat 
; memory model. The code and the data segment are fully overlapping and spanning the 
; complete 4 GB of addressable memory. 

; null segment descriptor
gdt_start:
	dq 0x0

; code segment descriptor
gdt_code:
	dw 0xffff			; segment length, bits 0-15
	dw 0x0				; segment base, bits 0-15
	db 10011010b		; flags (8 bits)
	db 11001111b  		; flags (4 bits) + segment length bits 16-19
	db 0x0				; segment base, bits 24-31

; data segment descriptor
gdt_data:
	dw 0xffff			; segment length, bits 0-15
	dw 0x0				; segment base, bits 0-15
	db 0x0				; segment base, bits 16-23
	db 10010010b		; flags (8 bits)
	db 11001111b  		; flags (4 bits) + segment length, bits 16-19
	db 0x0				; segment base, bits 24-31

gdt_end:

; GDT descriptor
gdt_descriptor:
	dw gdt_end - gdt_start - 1		; size (16 bit)
	dd gdt_start ; address (32 bit)

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start
