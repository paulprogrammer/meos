	section .stage2

	[bits 16]

	mov bx, stage2_msg
	call print_string

	cli					; as from legacy, disable interrupts
	lgdt [gdt32_pseudo_descriptor]

	; from legacy, set the cr0.PE bit to enable protected mode
	mov eax, cr0
	or eax, 0x01
	mov cr0, eax

	; from legacy, do far jump to flush the cpu pipeline
	; updates the cs registers
	jmp CODE_SEG32:start_prot_mode


;;; this stuff is still 16 bit mode

end:
	hlt
	jmp $

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

;;; 32 bit mode stuff below -- yay lexical scoping

	[bits 32]
start_prot_mode:
	; now the old segments are meaningless, need to update to new values
	mov ax, DATA_SEG32
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	mov bx, protected_message
	call print_string32

    ;; Build 4 level page table and switch to long mode
    mov ebx, 0x1000
    call build_page_table
    mov cr3, ebx            ; MMU finds the PML4 table in cr3

    ;; Enable Physical Address Extension (PAE)
    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax

    ;; The EFER (Extended Feature Enable Register) MSR (Model-Specific Register) contains fields
    ;; related to IA-32e mode operation. Bit 8 if this MSR is the LME (long mode enable) flag
    ;; that enables IA-32e operation.
    mov ecx, 0xc0000080
    rdmsr
    or eax, 1 << 8
    wrmsr

	jmp $

%include "include/gdt.s"

; as we're in protected mode, we can no longer rely on bios
; so we'll have to write directly to the VGA buffer

; assume address of string in bx register
print_string32:
	pusha

	VGA_BUF equ 0xb8000
	WB_COLOR equ 0xf

	mov edx, VGA_BUF

print_string32_loop:
	cmp byte [ebx], 0
	je print_string32_return

	mov al, [ebx]
	mov ah, WB_COLOR
	mov [edx], ax

	add ebx, 1  			; next char
	add edx, 2 				; next vga buffer cell
	jmp print_string32_loop

print_string32_return:
	popa
	ret

   ;; Builds a 4 level page table starting at the address that's passed in ebx.
build_page_table:
    pusha

    PAGE64_PAGE_SIZE equ 0x1000
    PAGE64_TAB_SIZE equ 0x1000
    PAGE64_TAB_ENT_NUM equ 512

    ;; Initialize all four tables to 0. If the present flag is cleared, all other bits in any
    ;; entry are ignored. So by filling all entries with zeros, they are all "not present".
    ;; Each repetition zeros four bytes at once. That's why a number of repetitions equal to
    ;; the size of a single page table is enough to zero all four tables.
    mov ecx, PAGE64_TAB_SIZE ; ecx stores the number of repetitions
    mov edi, ebx             ; edi stores the base address
    xor eax, eax             ; eax stores the value
    rep stosd

    ;; Link first entry in PML4 table to the PDP table
    mov edi, ebx
    lea eax, [edi + (PAGE64_TAB_SIZE | 11b)] ; Set read/write and present flags
    mov dword [edi], eax

    ;; Link first entry in PDP table to the PD table
    add edi, PAGE64_TAB_SIZE
    add eax, PAGE64_TAB_SIZE
    mov dword [edi], eax

    ;; Link the first entry in the PD table to the page table
    add edi, PAGE64_TAB_SIZE
    add eax, PAGE64_TAB_SIZE
    mov dword [edi], eax

    ;; Identity map the first 2 MB of memory in the single page table
    add edi, PAGE64_TAB_SIZE
    mov ebx, 11b
    mov ecx, PAGE64_TAB_ENT_NUM
build_page_table_set_entry:
    mov dword [edi], ebx
    add ebx, PAGE64_PAGE_SIZE
    add edi, 8
    loop build_page_table_set_entry

    popa
    ret

stage2_msg: db "Hello from stage 2", 0x0d, 0x0a, 0x0d, 0x0a, 0
protected_message: db "Hello from protected mode", 0
