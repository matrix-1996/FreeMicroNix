#include <x86/isa/8259.h>
#include <x86/isa/8253/h>
#include <x86/video/vga.h>

#include <stdint.h>
#include <stdbool.h>
#include <kernel/panic.h>


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


typedef struct interrupt_context
{
	uint32_t cr2;
	uint32_t gs;
	uint32_t fs;
	uint32_t ds;
	uint32_t es;
	uint32_t edi;
	uint32_t esi;
	uint32_t ebp;
	uint32_t ebx;
	uint32_t edx;
	uint32_t ecx;
	uint32_t eax;
	uint32_t int_no;
	uint32_t err_code;
	uint32_t cs;
	uint32_t eflags;
	uint32_t esp; /* If (cs & 0x3) != 0 */
	uint32_t ss;  /* If (cs & 0x3) != 0 */
} interrupt_context;

#define PIC_MASTER          0x20
#define PIC_SLAVE           0xA0
#define PIC_MASTER_COMMAND  0x20 
#define PIC_MASTER_DATA     0x21 
#define PIC_SLAVE_COMMAND   0xA0
#define PIC_SLAVE_DATA      0xA1
#define PIC_CMD_ENDINTR     0x20
#define PIC_READ_IRR        0x0A
#define PIC_READ_ISR        0x0B
#define ICW1_ICW4	        0x01		/* ICW4 (not) needed */
#define ICW1_SINGLE 	    0x02		/* Single (cascade) mode */
#define ICW1_INTERVAL4	    0x04		/* Call address interval 4 (8) */
#define ICW1_LEVEL	        0x08		/* Level triggered (edge) mode */
#define ICW1_INIT	        0x10		/* Initialization - required! */ 
#define ICW4_8086	        0x01		/* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO	        0x02		/* Auto (normal) EOI */
#define ICW4_BUF_SLAVE	    0x08		/* Buffered mode/slave */
#define ICW4_BUF_MASTER	    0x0C		/* Buffered mode/master */
#define ICW4_SFNM	        0x10		/* Special fully nested (not) */

#define IDTBASE 0x00000000
#define IDTSIZE 0xFF
#define IDT_ENTRIES 255
#define INTGATE  0x8E00
#define TRAPGATE 0xEF00

#define IRQ0  32
#define IRQ1  33
#define IRQ2  34
#define IRQ3  35
#define IRQ4  36
#define IRQ5  37
#define IRQ6  38
#define IRQ7  39
#define IRQ8  40
#define IRQ9  41
#define IRQ10 42
#define IRQ11 43
#define IRQ12 44
#define IRQ13 45
#define IRQ14 46
#define IRQ15 47

idt_entry idt[256];


void Initialize_8259_PIC()
{
	uint8_t MASTER_MASK = 0;
	uint8_t SLAVE_MASK = 0;

    outb(PIC_MASTER_COMMAND, ICW1_INIT + ICW1_ICW4);
    io_wait();
    outb(PIC_SLAVE_COMMAND, ICW1_INIT + ICW1_ICW4);
    io_wait();

    outb(PIC_MASTER_DATA, 0x20);
    io_wait();
    outb(PIC_SLAVE_DATA, 0x28);
    io_wait();

    outb(MASTER_PIC_DATA, 0x04); // Slave PIC at IRQ2
    io_wait();
    outb(PIC_SLAVE_DATA, 0x02);  // Cascade Identity
    io_wait();

    outb(PIC_MASTER_DATA, ICW4_8086);
    io_wait();   
    outb(PIC_SLAVE_DATA, ICW4_8086);
    io_wait();
    outb(PIC_MASTER_DATA, MASTER_MASK);
    outb(PIC_SLAVE_DATA, SLAVE_MASK);
}


extern void isr0(void);
extern void isr1(void);
extern void isr2(void);
extern void isr3(void);
extern void isr4(void);
extern void isr5(void);
extern void isr6(void);
extern void isr7(void);
extern void isr8(void);
extern void isr9(void);
extern void isr10(void);
extern void isr11(void);
extern void isr12(void);
extern void isr13(void);
extern void isr14(void);
extern void isr15(void);
extern void isr16(void);
extern void isr17(void);
extern void isr18(void);
extern void isr19(void);
extern void isr20(void);
extern void isr21(void);
extern void isr22(void);
extern void isr23(void);
extern void isr24(void);
extern void isr25(void);
extern void isr26(void);
extern void isr27(void);
extern void isr28(void);
extern void isr29(void);
extern void isr30(void);
extern void isr31(void);
extern void irq0(void);
extern void irq1(void);
extern void irq2(void);
extern void irq3(void);
extern void irq4(void);
extern void irq5(void);
extern void irq6(void);
extern void irq7(void);
extern void irq8(void);
extern void irq9(void);
extern void irq10(void);
extern void irq11(void);
extern void irq12(void);
extern void irq13(void);
extern void irq14(void);
extern void irq15(void);

void PIC_EOI_Master(void)
{
	outb(PIC_MASTER, PIC_CMD_ENDINTR);
}

void PIC_EOI_Slave(void)
{
	outb(PIC_SLAVE, PIC_CMD_ENDINTR);
}

uint16_t PIC_Read_IRR(void)
{
	outb(PIC_MASTER + PIC_COMMAND, PIC_READ_IRR);
	outb(PIC_SLAVE + PIC_COMMAND, PIC_READ_IRR);
	return inb(PIC_MASTER + PIC_COMMAND) << 0 |
	       inb(PIC_SLAVE  + PIC_COMMAND) << 8;
}

uint16_t PIC_Read_ISR(void)
{
	outb(PIC_MASTER + PIC_COMMAND, PIC_READ_ISR);
	outb(PIC_SLAVE + PIC_COMMAND, PIC_READ_ISR);
	return inb(PIC_MASTER + PIC_COMMAND) << 0 |
	       inb(PIC_SLAVE  + PIC_COMMAND) << 8;
}


void ISR_Handler(interrupt_context* int_ctx)
{
	(void) int_ctx;
}

void IRQ_Handler(interrupt_context* int_ctx)
{
	uint8_t irq = int_ctx;

	// Handle the potentially spurious interrupts IRQ 7 and IRQ 15.
	if ( irq == IRQ7 && !(PIC_Read_ISR() & (1 << 7)) )
		return;
	if ( irq == IRQ15 && !(PIC_Read_ISR() & (1 << 15)) )
		return PIC_EOI_Master();

	(void) int_ctx;
	if (irq == IRQ0) ISR_IRQ_8253_PIT();
	// Send the end of interrupt signal to the relevant PICs.
	if ( IRQ8 <= irq ) PIC_EOI_Slave();
	PIC_EOI_Master();
}

void Interrupt_Handler(interrupt_context* int_ctx)
{
	if ( int_ctx->int_no < 32 )
		ISR_Handler(int_ctx);
	else if ( 32 <= int_ctx->int_no && int_ctx->int_no < 32 + 16 )
		IRQ_Handler(int_ctx);
}


void ISR_General_Protection_Fault(void)
{
	kprintf("\nGeneral protection fault !\n");
	
	// handler to be implemented
	
	panic();
}

void ISR_Page_Fault(void)
{
	kprintf("\nPage Fault fault !\n");
	
	// handler to be implemented
	
	panic();
}


static inline bool Interrupts_Enabled()
{
    unsigned long flags;
    asm volatile ( "pushf\n\t"
                   "pop %0"
                   : "=g"(flags) );
    return flags & (1 << 9);
}

void Initialize_IDT_Entry(idt_entry *entry, uint16_t selector, uint32_t offset, uint16_t type)
{
    entry->offset0_15 = (offset & 0xffff);
    entry->selector = 0x08;
    entry->zero = 0;
    entry->type_attr = type;
    entry->offset16_31 = (offset & 0xffff0000) >> 16;
    return;
}


void Initialize_IDT(void)
{
    Initialize_IDT_Entry(0x08, (uint32_t) _asm_exc_GP, INTGATE, &kidt[0]);     /* #GP */
    init_idt_desc(0x08, (uint32_t) _asm_exc_PF, INTGATE, &kidt[14]);     /* #PF */
    init_idt_desc(0x08, (uint32_t) _asm_syscalls, TRAPGATE, &kidt[128]); //48
    
    kidtr.limite = IDTSIZE * 8;
    kidtr.base = IDTBASE;
    
    
    memcpy((int8_t *) kidtr.base, (int8_t *) kidt, kidtr.limite);

    asm("lidtl (kidtr)");  // Load the IDT
}


