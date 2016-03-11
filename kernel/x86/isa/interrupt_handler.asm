global isr0
global isr1
global isr2
global isr3
global isr4
global isr5
global isr6
global isr7
global isr8
global isr9
global isr10
global isr11
global isr12
global isr13
global isr14
global isr15
global isr16
global isr17
global isr18
global isr19
global isr20
global isr21
global isr22
global isr23
global isr24
global isr25
global isr26
global isr27
global isr28
global isr29
global isr30
global isr31
global irq0
global irq1
global irq2
global irq3
global irq4
global irq5
global irq6
global irq7
global irq8
global irq9
global irq10
global irq11
global irq12
global irq13
global irq14
global irq15

%macro isr 1
	global isr%1
	isr%1:
		cli
		push byte 0
		push byte %1
		jmp IDT_ISR_Stub
%endmacro

%macro isr_error 1
	global isr%1
	isr%1:
		cli
		push byte %1
		jmp IDT_ISR_Stub
%endmacro

%macro irq 1
	global irq%1
	irq%1:
		cli
		push byte 0
		push byte (%1 + 32)
		jmp IDT_ISR_Stub
%endmacro


Interrupt_Exception_Set:
	isr 0
	isr 1
	isr 2
	isr 3
	isr 4
	isr 5
	isr 6
	isr 7
	isr_error 8
	isr 9
	isr_error 10
	isr_error 11
	isr_error 12
	isr_error 13
	isr_error 14
	isr 15
	isr 16
	isr_error 17
	isr 18
	isr 19
	isr 20
	isr 21
    isr 22
	isr 23
	isr 24
	isr 25
	isr 26
	isr 27
	isr 28
	isr 29
	isr 30
	isr 31





Interrupt_IRQ_Set:
	irq 0
	irq 1
	irq 2
	irq 3
	irq 4
	irq 5
	irq 6
	irq 7
	irq 8
	irq 9
	irq 10
	irq 11
	irq 12
	irq 13
	irq 14
	irq 15


extern Interrupt_Handler
IDT_ISR_Stub:
	
	push eax
	push ebx
	push ecx
	push edx
	push ebp
	push esi
	push edi	
	push ds
	push es
	push fs
	push gs
	mov ax, 0x10
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
	pop edi
	pop esi
	pop ebp
	pop edx
	pop ecx 
	pop ebx
	pop eax
	add esp, 8
	iret
