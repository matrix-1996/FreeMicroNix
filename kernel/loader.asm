global loader									; ELF entry point

FLAGS       equ  0x0							; multiboot flag
MAGIC       equ  0x1BADB002 					; magic number
CHECKSUM    equ  -MAGIC     					; the mulitboot checksum
KSTACK_SIZE equ 4096
 
section .bss
align 4											; align at 4 bytes
kstack:											; label points to beginning of memory
resb KSTACK_SIZE								; reserve stack for the kernel

section .text
align 4
	dd MAGIC
	dd FLAGS
	dd CHECKSUM

loader:
	mov esp, kstack + KSTACK_SIZE				; point esp to the start of the
												; kernel's stack
	extern kmain								; inform the loader of the kmain function
	call kmain									; call the kmain function

.hang:
	hlt
	jmp .hang