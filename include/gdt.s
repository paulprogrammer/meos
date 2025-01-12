; https://wiki.osdev.org/Global_Descriptor_Table

	align 8

gdt32_start:
	; 8-byte null description
	; used to catch translations with null selectors
	dd 0x0
	dd 0x0

gdt32_code_segment:
	; 8 byte segment descriptor index 1
	; first 16 bits of segment limit
	dw 0xffff
	; first 24 bits of segment base addr
	dw 0x0000
	db 0x00
	; 0-3: segment that specifies execute/read code seg
	;   4: descriptor type flag indicating code/data
	; 5-6: descriptor priv level 0
	;   7: segment present flag (must be 1)
	db 10011010b
	; 0-3: last 4 bits of seg limit
	;   4: unused
	;   5: 64-bit code segment flag (not yet!)
	;   6: op limit size 32 bits
	;   7: 4kb granularity
	db 11001111b
	; last 8 bits of seg base addr
	db 0x00

gdt32_data_segment:
    ;; Only differences are explained ...
    dw 0xffff
    dw 0x0000
    db 0x00
    ;; 0-3: segment type that specifies a read/write data segment
    db 10010010b
    db 11001111b
    db 0x00

gdt32_end:

; build the address descriptor
gdt32_pseudo_descriptor:
	; limit value of 0 is 1 byte, so subtract one from the calculated size
	dw gdt32_end - gdt32_start - 1
	; start of the gdt
	dd gdt32_start

CODE_SEG32 equ gdt32_code_segment - gdt32_start
DATA_SEG32 equ gdt32_data_segment - gdt32_start