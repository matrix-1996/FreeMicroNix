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


section .bootstrap_stack, nobits											; align at 4 bytes
align 4
kstack:											; label points to beginning of memory
resb KSTACK_SIZE								; reserve stack for the kernel


section .text
align 4
global loader
loader:
	mov esp, kstack + KSTACK_SIZE				; point esp to the start of the
												; kernel's stack
	extern _init
	call _init
	extern kmain								; inform the loader of the kmain function
	call kmain									; call the kmain function
	cli
.hang:
	hlt
	jmp .hang