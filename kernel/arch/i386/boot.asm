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
global _start
_start:
	mov esp, stack_top

	push ebp
	mov ebp, esp

	cli 								; disable interrupts
	call gdt_reload
	call idt_reload

	extern gdt
	mov eax, dword [gdt+0x34]			; Clear the TSS descriptor's busy flag.
	and ah, 0xFD
	mov dword [gdt+0x34], eax
	
	mov ax, 0x33						; Load the Task Register.
	ltr ax
	
	mov eax, cr0						; Re-enable paging.
	or eax, 0x80000000
	mov cr0, eax

	sti 								; Re-enable interrupts

	mov esp, ebp
	pop ebp
	ret
	

	mov edx, cr0						; We need to set up the FPU properly.
	and edx, 0xFFFFFFFB					; Ensure the FPU Emulation bit is disabled.
	or edx, 0x22						; Set the native exception and MP flags.
	mov cr0, edx
	
	fninit								; Initialize the FPU.
	fldcw [0x37A]				; Unmask invalid operand and division exceptions.


	; Initialize the core kernel before running the global constructors.
	extern kernel_early
	call kernel_early

	extern _init
	; Call the global constructors.
	call _init

	extern kernel_main
	; Transfer control to the main kernel.
	call kernel_main

	; Hang if kernel_main unexpectedly returns.
	cli
.hang:
	hlt
	jmp .hang


global gdt_reload
extern gdtr
gdt_reload:
	lgdt [gdtr]							; Load the GDT pointer.
	
	mov ax, 0x18						; Load the kernel data selectors.
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	
	jmp 0x10:.flush						; Load the kernel code selector.

.flush:
	ret


global idt_reload
extern idtr
idt_reload:
	lidt [idtr]							; Load the IDT pointer.
	ret
	
