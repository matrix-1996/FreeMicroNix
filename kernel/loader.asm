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
	
	extern loadGDT					; load and init the gdt
	call loadGDT

	extern _init					; init the global constructors
	call _init

	extern kmain					; call the kmain function
	call kmain

	cli
.hang:
	hlt
	jmp .hang

