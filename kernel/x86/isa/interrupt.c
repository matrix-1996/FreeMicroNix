#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <x86/isa/8254.h>
#include <x86/isa/8259.h>
#include <x86/isa/interrupt.h>
#include <x86/isa/interrupt_handler.h>
#include <x86/video/vga.h>
#include <kernel/panic.h>

extern void idt_loader(uint32_t);

static const char * exception_names[] = {
	"division by zero",
        "debug exception",
        "nonmaskable interrupt",
        "breakpoint",
        "overflow",
        "bounds check",
        "invalid instruction",
        "coprocessor error",
        "double fault",
        "copressor overrun",
        "invalid task",
        "segment not present",
        "stack exception",
        "general protection fault",
        "page fault",
        "unknown",
        "coprocessor error"
};

typedef struct idtr {
    uint16_t limite;
    uint32_t base;
} __attribute__ ((packed)) idtr;

typedef struct idt_entry {
    uint16_t offset0_15; // offset bits 0-15
    uint16_t selector;   // code segment selector in GDT
    uint8_t zero;        // unused, set to 0
    uint16_t type_attr;       // type and attributes
    uint16_t offset16_31;// offset bits 16-31
} __attribute__ ((packed)) idt_entry;


interrupt_context Interrupt_Handlers[256];

void Register_Interrupt_Handler(uint32_t irq, interrupt_context handler)
{
	Interrupt_Handlers[irq] = handler;
}

void Page_Fault_Handler(void)
{
	unsigned vaddr;

	asm("mov %%cr2, %0" : "=r" (vaddr) );
	kprintf("interrupt: page fault at %x\n",vaddr);
	panic();

}


static void Interrupt_Acknowledge( uint32_t i )
{
	if(i<32) {
		/* do nothing */
	} else {
		Acknowledge_8259_PIC(i-32);
	}
}


void Exception_Handler(interrupt_context* int_ctx)
{
	uint32_t exception = int_ctx->int_no;
	if (exception == 14)
	{
		Page_Fault_Handler();
	}
	else
	{
		kprintf("interrupt: exception %d: %s (code %x)\n", int_ctx->int_no, exception_names[int_ctx->int_no],int_ctx->err_code);
		panic();
	}
}

void IRQ_Handler(interrupt_context* int_ctx)
{
	uint8_t irq = int_ctx->int_no;
	kprintf("Recieved IRQ: %d\n",irq-32);
	// Handle the potentially spurious interrupts (uint32_t)IRQ 7 and (uint32_t)IRQ 15.
	if ( irq == IRQ7 && !(PIC_Read_ISR() & (1 << 7)) )
		return;
	if ( irq == IRQ15 && !(PIC_Read_ISR() & (1 << 15)) )
		return PIC_EOI_Master();

	(void) int_ctx;
	if (irq == IRQ0) IRQ_8254_PIT_Handler();
	// Send the end of interrupt signal to the relevant PICs.
	if ( IRQ8 <= irq ) PIC_EOI_Slave();
	PIC_EOI_Master();
}

void Interrupt_Handler(interrupt_context* int_ctx)
{
	if ( int_ctx->int_no < 32 )
		Exception_Handler(int_ctx);
	else if ( 32 <= int_ctx->int_no && int_ctx->int_no < 32 + 16 )
		IRQ_Handler(int_ctx);
}

void Initialize_IDT_Entry(idt_entry *entry, uint32_t handler, uint8_t type, uint8_t rpl)
{
    entry->offset0_15 = (uintptr_t) handler & 0xFFFF;
    entry->selector = 0x08;
    entry->zero = 0;
    entry->type_attr = IDT_FLAG_PRESENT |
	               rpl << IDT_FLAG_DPL_SHIFT |
	               type << IDT_FLAG_TYPE_SHIFT;
    entry->offset16_31 = (uintptr_t) (handler >> 16) & 0xFFFF;
}


idt_entry idt[256];
idtr kidtr;

void Initialize_Interrupts(void)
{
	Initialize_8259_PIC(0x20, 0x28);

	memset(&idt, 0, sizeof(idt)*256);

	Initialize_IDT_Entry(&idt[0], (uint32_t)isr0, IDT_TYPE_INTERRUPT, 0x0);
	Initialize_IDT_Entry(&idt[1], (uint32_t)isr1, IDT_TYPE_INTERRUPT, 0x0);
	Initialize_IDT_Entry(&idt[2], (uint32_t)isr2, IDT_TYPE_INTERRUPT, 0x0);
	Initialize_IDT_Entry(&idt[3], (uint32_t)isr3, IDT_TYPE_INTERRUPT, 0x0);
	Initialize_IDT_Entry(&idt[4], (uint32_t)isr4, IDT_TYPE_INTERRUPT, 0x0);
	Initialize_IDT_Entry(&idt[5], (uint32_t)isr5, IDT_TYPE_INTERRUPT, 0x0);
	Initialize_IDT_Entry(&idt[6], (uint32_t)isr6, IDT_TYPE_INTERRUPT, 0x0);
	Initialize_IDT_Entry(&idt[7], (uint32_t)isr7, IDT_TYPE_INTERRUPT, 0x0);
	Initialize_IDT_Entry(&idt[8], (uint32_t)isr8, IDT_TYPE_INTERRUPT, 0x0);
	Initialize_IDT_Entry(&idt[9], (uint32_t)isr9, IDT_TYPE_INTERRUPT, 0x0);
	Initialize_IDT_Entry(&idt[10], (uint32_t)isr10, IDT_TYPE_INTERRUPT, 0x0);
	Initialize_IDT_Entry(&idt[11], (uint32_t)isr11, IDT_TYPE_INTERRUPT, 0x0);
	Initialize_IDT_Entry(&idt[12], (uint32_t)isr12, IDT_TYPE_INTERRUPT, 0x0);
	Initialize_IDT_Entry(&idt[12], (uint32_t)isr13, IDT_TYPE_INTERRUPT, 0x0);
	Initialize_IDT_Entry(&idt[14], (uint32_t)isr14, IDT_TYPE_INTERRUPT, 0x0);
	Initialize_IDT_Entry(&idt[15], (uint32_t)isr15, IDT_TYPE_INTERRUPT, 0x0);
	Initialize_IDT_Entry(&idt[16], (uint32_t)isr16, IDT_TYPE_INTERRUPT, 0x0);
	Initialize_IDT_Entry(&idt[17], (uint32_t)isr17, IDT_TYPE_INTERRUPT, 0x0);
	Initialize_IDT_Entry(&idt[18], (uint32_t)isr18, IDT_TYPE_INTERRUPT, 0x0);
	Initialize_IDT_Entry(&idt[19], (uint32_t)isr19, IDT_TYPE_INTERRUPT, 0x0);
	Initialize_IDT_Entry(&idt[20], (uint32_t)isr20, IDT_TYPE_INTERRUPT, 0x0);
	Initialize_IDT_Entry(&idt[21], (uint32_t)isr21, IDT_TYPE_INTERRUPT, 0x0);
	Initialize_IDT_Entry(&idt[22], (uint32_t)isr22, IDT_TYPE_INTERRUPT, 0x0);
	Initialize_IDT_Entry(&idt[23], (uint32_t)isr23, IDT_TYPE_INTERRUPT, 0x0);
	Initialize_IDT_Entry(&idt[24], (uint32_t)isr24, IDT_TYPE_INTERRUPT, 0x0);
	Initialize_IDT_Entry(&idt[25], (uint32_t)isr25, IDT_TYPE_INTERRUPT, 0x0);
	Initialize_IDT_Entry(&idt[26], (uint32_t)isr26, IDT_TYPE_INTERRUPT, 0x0);
	Initialize_IDT_Entry(&idt[27], (uint32_t)isr27, IDT_TYPE_INTERRUPT, 0x0);
	Initialize_IDT_Entry(&idt[28], (uint32_t)isr28, IDT_TYPE_INTERRUPT, 0x0);
	Initialize_IDT_Entry(&idt[29], (uint32_t)isr29, IDT_TYPE_INTERRUPT, 0x0);
	Initialize_IDT_Entry(&idt[30], (uint32_t)isr30, IDT_TYPE_INTERRUPT, 0x0);
	Initialize_IDT_Entry(&idt[31], (uint32_t)isr31, IDT_TYPE_INTERRUPT, 0x0);
	Initialize_IDT_Entry(&idt[32], (uint32_t)irq0, IDT_TYPE_INTERRUPT, 0x0);
	Initialize_IDT_Entry(&idt[33], (uint32_t)irq1, IDT_TYPE_INTERRUPT, 0x0);
	Initialize_IDT_Entry(&idt[34], (uint32_t)irq2, IDT_TYPE_INTERRUPT, 0x0);
	Initialize_IDT_Entry(&idt[35], (uint32_t)irq3, IDT_TYPE_INTERRUPT, 0x0);
	Initialize_IDT_Entry(&idt[36], (uint32_t)irq4, IDT_TYPE_INTERRUPT, 0x0);
	Initialize_IDT_Entry(&idt[37], (uint32_t)irq5, IDT_TYPE_INTERRUPT, 0x0);
	Initialize_IDT_Entry(&idt[38], (uint32_t)irq6, IDT_TYPE_INTERRUPT, 0x0);
	Initialize_IDT_Entry(&idt[39], (uint32_t)irq7, IDT_TYPE_INTERRUPT, 0x0);
	Initialize_IDT_Entry(&idt[40], (uint32_t)irq8, IDT_TYPE_INTERRUPT, 0x0);
	Initialize_IDT_Entry(&idt[41], (uint32_t)irq9, IDT_TYPE_INTERRUPT, 0x0);
	Initialize_IDT_Entry(&idt[42], (uint32_t)irq10, IDT_TYPE_INTERRUPT, 0x0);
	Initialize_IDT_Entry(&idt[43], (uint32_t)irq11, IDT_TYPE_INTERRUPT, 0x0);
	Initialize_IDT_Entry(&idt[44], (uint32_t)irq12, IDT_TYPE_INTERRUPT, 0x0);
	Initialize_IDT_Entry(&idt[45], (uint32_t)irq13, IDT_TYPE_INTERRUPT, 0x0);
	Initialize_IDT_Entry(&idt[46], (uint32_t)irq14, IDT_TYPE_INTERRUPT, 0x0);
	Initialize_IDT_Entry(&idt[47], (uint32_t)irq15, IDT_TYPE_INTERRUPT, 0x0);
	
	kidtr.limite = sizeof(idt_entry) * 256 - 1;
	kidtr.base =  (uint32_t) &idt;
	idt_loader((uint32_t) &idt);
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