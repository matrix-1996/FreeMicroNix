/*
Copyright (C) <2016>  William Taylor Jones <freemicronix@gmail.com>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <x86/isa/8254.h>
#include <x86/isa/8259.h>
#include <x86/isa/idt.h>
#include <x86/isa/interrupt.h>
#include <x86/isa/interrupt_handler.h>
#include <x86/video/vga.h>
#include <x86/io.h>
#include <kernel/panic.h>

const char * exception_messages[] = {
	"Division By Zero",
	"Debug",
	"Non Maskable Interrupt",
	"Breakpoint",
	"Into Detected Overflow",
	"Out of Bounds",
	"Invalid Opcode",
	"No Coprocessor",
	"Double Fault",
	"Coprocessor Segment Overrun",
	"Bad TSS",
	"Segment Not Present",
	"Stack Fault",
	"General Protection Fault",
	"Page Fault",
	"Unknown Interrupt",
	"Coprocessor Fault",
	"Alignment Check",
	"Machine Check",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
};

void *Interrupt_Routines[256] = {
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,

};

void Install_Interrupt_Handler(uint32_t index, void(*handler) (interrupt_context_t* r))
{
	Interrupt_Routines[index] = handler;

}

void Uninstall_Interrupt_Handler(uint32_t index)
{
	Interrupt_Routines[index] = 0;
}

/******************Mode Switch Test*****************************/

void STU_Handler(interrupt_context_t* int_ctx){
	kprintf("Switching to user mode\n");
	if((int_ctx->int_no & 0x03) != 3){
		int_ctx->gs = int_ctx->es = int_ctx->fs = int_ctx->ds = int_ctx->ss = USER_DS;
		int_ctx->cs = USER_CS;
		int_ctx->eflags |= 0x00003000;
	}
}

void STK_Handler(interrupt_context_t* int_ctx){
	kprintf("Switching to kernel mode\n");
	if((int_ctx->int_no & 0x03) != 0){
		int_ctx->gs = int_ctx->es = int_ctx->fs = int_ctx->ds = int_ctx->ss = KERNEL_DS;
		int_ctx->cs = KERNEL_CS;
	}
}

/************************************************/

void Install_Syscall_Handler(void){
	Install_Interrupt_Handler(120, STU_Handler);
	Install_Interrupt_Handler(121, STK_Handler);
}


void Page_Fault_Handler(void)
{
	uint32_t vaddr;

	asm("mov %%cr2, %0" : "=r" (vaddr) );
	kprintf("interrupt: page fault at %x\n",vaddr);
	panic();

}

void Exception_Handler(interrupt_context_t* int_ctx)
{
	
	if (int_ctx->int_no < 32)
	{
		Terminal_Set_Color(COLOR_LIGHT_RED);
		kprintf("Exception %x: %s (code %x)\n", int_ctx->int_no, exception_messages[int_ctx->int_no],int_ctx->err_code);
		kprintf("\nDumping Segment Registers:\n cs: %x ss: %x ds: %x",int_ctx->cs, int_ctx->ss, int_ctx->ds);
		kprintf("\nDumping General Purpose Registers:\n edi: %x esi: %x ebp: %x ebx: %x edx: %x ecx: %x eax: %x", int_ctx->edi, int_ctx->esi, int_ctx->ebp, int_ctx->ebx, int_ctx->edx, int_ctx->ecx, int_ctx->eax);
		kprintf("\nDumping Other Registers:\n eip: %x eflags: %x useresp: %x");
		panic();
	}

}

void IRQ_Handler(interrupt_context_t* int_ctx)
{

	void(*handler)(interrupt_context_t* int_ctx);

	//kprintf("IRQ: %x\n",irq);

	handler = Interrupt_Routines[int_ctx->int_no];
	
	if ( int_ctx->int_no == IRQ7 && !(PIC_Read_ISR() & (1 << 7)) )
	{
		return;
	}


	if ( int_ctx->int_no == IRQ15 && !(PIC_Read_ISR() & (1 << 15)) )
	{
		return PIC_EOI_Master();
	}

	if (handler)
	{
		handler(int_ctx);
	}

	if (int_ctx->int_no >= IRQ8)
	{
		PIC_EOI_Slave();
	}
	PIC_EOI_Master();
}

void Interrupt_Handler(interrupt_context_t* int_ctx)
{
	if ( int_ctx->int_no < 32 )
		Exception_Handler(int_ctx);
	else if ( 32 <= int_ctx->int_no && int_ctx->int_no < 48 )
		IRQ_Handler(int_ctx);
}



void Interrupt_Handler_Installer(void)
{

    Initialize_8259_PIC(0x20, 0x28); // Initialize the 8259 Programmable Interrupt Controller

	Create_IDT_Entry(0, (uint32_t) isr0, 0x08, 0x8E);
    Create_IDT_Entry(1, (uint32_t) isr1, 0x08, 0x8E);
    Create_IDT_Entry(2, (uint32_t) isr2, 0x08, 0x8E);
    Create_IDT_Entry(3, (uint32_t) isr3, 0x08, 0x8E);
    Create_IDT_Entry(4, (uint32_t) isr4, 0x08, 0x8E);
    Create_IDT_Entry(5, (uint32_t) isr5, 0x08, 0x8E);
    Create_IDT_Entry(6, (uint32_t) isr6, 0x08, 0x8E);
    Create_IDT_Entry(7, (uint32_t) isr7, 0x08, 0x8E);
    Create_IDT_Entry(8, (uint32_t) isr8, 0x08, 0x8E);
    Create_IDT_Entry(9, (uint32_t) isr9, 0x08, 0x8E);
    Create_IDT_Entry(10, (uint32_t) isr10, 0x08, 0x8E);
    Create_IDT_Entry(11, (uint32_t) isr11, 0x08, 0x8E);
    Create_IDT_Entry(12, (uint32_t) isr12, 0x08, 0x8E);
    Create_IDT_Entry(13, (uint32_t) isr13, 0x08, 0x8E);
    Create_IDT_Entry(14, (uint32_t) isr14, 0x08, 0x8E);
    Create_IDT_Entry(15, (uint32_t) isr15, 0x08, 0x8E);
    Create_IDT_Entry(16, (uint32_t) isr16, 0x08, 0x8E);
    Create_IDT_Entry(17, (uint32_t) isr17, 0x08, 0x8E);
    Create_IDT_Entry(18, (uint32_t) isr18, 0x08, 0x8E);
    Create_IDT_Entry(19, (uint32_t) isr19, 0x08, 0x8E);
    Create_IDT_Entry(20, (uint32_t) isr20, 0x08, 0x8E);
    Create_IDT_Entry(21, (uint32_t) isr21, 0x08, 0x8E);
    Create_IDT_Entry(22, (uint32_t) isr22, 0x08, 0x8E);
    Create_IDT_Entry(23, (uint32_t) isr23, 0x08, 0x8E);
    Create_IDT_Entry(24, (uint32_t) isr24, 0x08, 0x8E);
    Create_IDT_Entry(25, (uint32_t) isr25, 0x08, 0x8E);
    Create_IDT_Entry(26, (uint32_t) isr26, 0x08, 0x8E);
    Create_IDT_Entry(27, (uint32_t) isr27, 0x08, 0x8E);
    Create_IDT_Entry(28, (uint32_t) isr28, 0x08, 0x8E);
    Create_IDT_Entry(29, (uint32_t) isr29, 0x08, 0x8E);
    Create_IDT_Entry(30, (uint32_t) isr30, 0x08, 0x8E);
    Create_IDT_Entry(31, (uint32_t) isr31, 0x08, 0x8E);    
    Create_IDT_Entry(32, (uint32_t) irq0, 0x08, 0x8E);
	Create_IDT_Entry(33, (uint32_t) irq1, 0x08, 0x8E);
	Create_IDT_Entry(34, (uint32_t) irq2, 0x08, 0x8E);
	Create_IDT_Entry(35, (uint32_t) irq3, 0x08, 0x8E);
	Create_IDT_Entry(36, (uint32_t) irq4, 0x08, 0x8E);
	Create_IDT_Entry(37, (uint32_t) irq5, 0x08, 0x8E);
	Create_IDT_Entry(38, (uint32_t) irq6, 0x08, 0x8E);
	Create_IDT_Entry(39, (uint32_t) irq7, 0x08, 0x8E);
	Create_IDT_Entry(40, (uint32_t) irq8, 0x08, 0x8E);
	Create_IDT_Entry(41, (uint32_t) irq9, 0x08, 0x8E);
	Create_IDT_Entry(42, (uint32_t) irq10, 0x08, 0x8E);
	Create_IDT_Entry(43, (uint32_t) irq11, 0x08, 0x8E);
	Create_IDT_Entry(44, (uint32_t) irq12, 0x08, 0x8E);
	Create_IDT_Entry(45, (uint32_t) irq13, 0x08, 0x8E);
	Create_IDT_Entry(46, (uint32_t) irq14, 0x08, 0x8E);
	Create_IDT_Entry(47, (uint32_t) irq15, 0x08, 0x8E);

	// add system calls
	// 2 most significant bits are DPL
	// if ring 3 wants to get ring 0 handler its not option
	// Kenerl stuff = 0x1000 1110 = 0x8E
	// User   stuff = 0x1110 1110 = 0xBE
	Create_IDT_Entry(120, (uint32_t)isr120,0x08,0x8E);
	Create_IDT_Entry(121, (uint32_t)isr120,0x08,0xBE);

	kprintf("Interrupt Handlers Installed\n");
}

void Enable_Interrupt(uint32_t irq)
{
	if (irq < 32 )
	{
		/* do nothing */
	}
	else
	{
	Enable_8259_PIC((uint32_t)irq - 32);
	}
}

void Disable_Interrupt(uint32_t irq)
{
	if (irq < 32 )
	{
		/* do nothing */
	}
	else
	{
	Disable_8259_PIC(irq - 32);
	}
}

void Disable_Interrupts(void)
{
	asm("cli");
}

void Enable_Interrupts(void)
{
	asm("sti");
}

void Pause_Interrupts(void)
{
	asm("sti");
	asm("hlt");
}

