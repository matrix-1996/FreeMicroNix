extern _init, kmain

MBALIGN     equ  1<<0                  
MEMINFO     equ  1<<1
FLAGS       equ  MBALIGN | MEMINFO
MAGIC       equ  0x1BADB002
CHECKSUM    equ -(MAGIC + FLAGS)

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
global loader
loader:
	mov esp, stack_top				; point esp to the start of the kernel's stack

	call _init						; call global constructors
	
	call kmain						; transfer control to main kernel

	cli
.hang:
	hlt
	jmp .hang
