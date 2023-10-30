bits 64

section .text

global HalLoadGdtr

HalLoadGdtr:
	lgdt [rcx]

	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax

	push qword 0x08
	lea rax, [rel .Flush]
	push rax
	retfq
.Flush:
	ret