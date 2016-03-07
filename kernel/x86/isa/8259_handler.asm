%macro spawn_isr 1
	global int_isr_%1
	int_isr_%1:
		cli
		push byte 0
		push byte %1
		jmp IDT_ISR_Stub
%endmacro

%macro spawn_isr_error 1
	global int_isr_%1
	int_isr_%1:
		cli
		push byte %1
		jmp IDT_ISR_Stub
%endmacro

%macro spawn_isr_irq 2
	global int_irq_%1
	int_irq_%1:
		cli
		push byte 0
		push byte %2
		jmp IDT_ISR_Stub
%endmacro

extern Interrupt_Handler
IDT_ISR_Stub:
	
	pusha
	
	push ds
	push es
	push fs
	push gs
	
	mov ax, 0x18
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov eax, esp
	push eax
	
	mov eax, Interrupt_Handler
	call eax
	
	pop eax
	pop gs
	pop fs
	pop es
	pop ds
	
	popa
	add esp, 8
	iret


Interrupt_Exception_Set:
	spawn_isr 0
	spawn_isr 1
	spawn_isr 2
	spawn_isr 3
	spawn_isr 4
	spawn_isr 5
	spawn_isr 6
	spawn_isr 7
	spawn_isr_error 8
	spawn_isr 9
	spawn_isr_error 10
	spawn_isr_error 11
	spawn_isr_error 12
	spawn_isr_error 13
	spawn_isr_error 14
	spawn_isr 15
	spawn_isr 16
	spawn_isr 17
	spawn_isr 18
	spawn_isr 19
	spawn_isr 20
	spawn_isr 21
	spawn_isr 22
	spawn_isr 23
	spawn_isr 24
	spawn_isr 25
	spawn_isr 26
	spawn_isr 27
	spawn_isr 28
	spawn_isr 29
	spawn_isr 30
	spawn_isr 31

Interrupt_IRQ_Set:
	spawn_isr_irq 0, 32
	spawn_isr_irq 1, 33
	spawn_isr_irq 2, 34
	spawn_isr_irq 3, 35
	spawn_isr_irq 4, 36
	spawn_isr_irq 5, 37
	spawn_isr_irq 6, 38
	spawn_isr_irq 7, 39
	spawn_isr_irq 8, 40
	spawn_isr_irq 9, 41
	spawn_isr_irq 10, 42
	spawn_isr_irq 11, 43
	spawn_isr_irq 12, 44
	spawn_isr_irq 13, 45
	spawn_isr_irq 14, 46
	spawn_isr_irq 15, 47


extern ISR_General_Protection_Fault, ISR_Page_Fault 
global _asm_Syscalls, _asm_Page_Fault
_asm_Syscalls:
	SAVE_REGS
	push eax                 ; transmission du numero d'appel
	call do_syscalls
	pop eax
	cli
	sti
	RESTORE_REGS
	iret


_asm_Page_Fault:
	SAVE_REGS
	call ISR_Page_Fault
	RESTORE_REGS
	add esp,4
	iret

global _asm_Schedule
_asm_Schedule:
	SAVE_REGS
	call isr_schedule_int
	mov al,0x20
	out 0x20,al
	RESTORE_REGS
	iret