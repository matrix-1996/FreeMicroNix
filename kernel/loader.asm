MBALIGN     equ  1<<0                  
MEMINFO     equ  1<<1
FLAGS       equ  MBALIGN | MEMINFO
MAGIC       equ  0x1BADB002
CHECKSUM    equ -(MAGIC + FLAGS)
KSTACK_SIZE equ 4096


section .multiboot
align 4
	dd MAGIC
	dd FLAGS
	dd CHECKSUM


section .bootstrap_stack, nobits
align 4
stack_bottom:
resb 16384
stack_top:

section .text
align 4
global loader
loader:
	mov esp, stack_top				; point esp to the start of the kernel's stack
	extern tss
	mov ecx, tss
	extern gdt
	mov [gdt + 0x28 + 2], cx
	shr ecx, 16
	mov [gdt + 0x28 + 4], cl
	shr ecx, 8
	mov [gdt + 0x28 + 7], cl

	sub esp, 6
	extern gdt_size_minus_one
	mov cx, [gdt_size_minus_one]
	mov [esp], cx
	mov ecx, gdt
	mov [esp+2], ecx
	lgdt [esp]
	add esp, 6

	push 0x08
	push 0x1f

	mov cx, 0x10
	mov ds, cx
	mov es, cx
	mov fs, cx
	mov gs, ax
	mov ss, cx
	
	mov cx, [0x2b]
	ltr cx
	extern _init
	call _init
	extern kmain
	call kmain									; call the kmain function
	cli
.hang:
	hlt
	jmp .hang

