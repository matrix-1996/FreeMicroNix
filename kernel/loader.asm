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
	push 0x0

	extern kernel_physical_start
	push kernel_physical_start

	extern kernel_physical_end
	push kernel_physical_end
	
	push ebx
	push eax

	;extern _init
	;call _init						; call global constructors


	extern kmain
	call kmain						; transfer control to main kernel

	cli
.hang:
	hlt
	jmp .hang
