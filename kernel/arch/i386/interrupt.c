#include <i386/global.h>
#include <i386/kprintf.h>
#include <i386/timer.h>
#include <i386/rtc.h>
#include <i386/interrupt.h>

typedef struct {
	unsigned short base_0_15;
	unsigned short selector;
	unsigned char zero;
	unsigned char flags;
	unsigned short base_16_31;
} __attribute__((packed)) idt_entry;

typedef struct {
    unsigned short limit;
    unsigned int base;
} __attribute__((packed)) idtr_entry;

#define IDT_ENTRIES 255
//static const uint8_t IDT_ENTRIES = 255;

static const uint8_t FLAG_PRESENT = 1 << 7;
static const uint8_t FLAG_DPL_SHIFT = 5;
static const uint8_t FLAG_DPL_BITS = 2;
static const uint8_t FLAG_TYPE_SHIFT = 0;
static const uint8_t FLAG_TYPE_BITS = 4;

static const uint8_t TYPE_INTERRUPT = 0xE;
static const uint8_t TYPE_TRAP = 0xF;

static const uint16_t PIC_MASTER = 0x20;
static const uint16_t PIC_SLAVE = 0xA0;
static const uint16_t PIC_COMMAND = 0x00;
static const uint16_t PIC_DATA = 0x01;
static const uint8_t PIC_CMD_ENDINTR = 0x20;
static const uint8_t PIC_ICW1_ICW4 = 0x01; // ICW4 (not) needed
static const uint8_t PIC_ICW1_SINGLE = 0x02; // Single (cascade) mode
static const uint8_t PIC_ICW1_INTERVAL4 = 0x04; // Call address interval 4 (8)
static const uint8_t PIC_ICW1_LEVEL = 0x08; // Level triggered (edge) mode
static const uint8_t PIC_CMD_INIT = 0x10;
static const uint8_t PIC_MODE_8086 = 0x01; // 8086/88 (MCS-80/85) mode
static const uint8_t PIC_MODE_AUTO = 0x02; // Auto (normal) EOI
static const uint8_t PIC_MODE_BUF_SLAVE = 0x08; // Buffered mode/slave
static const uint8_t PIC_MODE_BUF_MASTER = 0x0C; // Buffered mode/master
static const uint8_t PIC_MODE_SFNM = 0x10; // Special fully nested (not)
static const uint8_t PIC_READ_IRR = 0x0A;
static const uint8_t PIC_READ_ISR = 0x0B;

static const unsigned int IRQ0 = 32;
static const unsigned int IRQ1 = 33;
static const unsigned int IRQ2 = 34;
static const unsigned int IRQ3 = 35;
static const unsigned int IRQ4 = 36;
static const unsigned int IRQ5 = 37;
static const unsigned int IRQ6 = 38;
static const unsigned int IRQ7 = 39;
static const unsigned int IRQ8 = 40;
static const unsigned int IRQ9 = 41;
static const unsigned int IRQ10 = 42;
static const unsigned int IRQ11 = 43;
static const unsigned int IRQ12 = 44;
static const unsigned int IRQ13 = 45;
static const unsigned int IRQ14 = 46;
static const unsigned int IRQ15 = 47;

extern void idt_reload(void);	// from kernel/entry.asm

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


void isr_irq_generic_master()
{
	outb(PIC_MASTER, PIC_CMD_ENDINTR);
}

void isr_irq_generic_slave()
{
	outb(PIC_SLAVE, PIC_CMD_ENDINTR);
	outb(PIC_MASTER, PIC_CMD_ENDINTR);

}


extern uint16_t gdt_kernel_cs;
idt_entry idt[IDT_ENTRIES+1];
isr_t idt_isr_list[IDT_ENTRIES+1];
idtr_entry idtr;


void eoi_master()
{
	outb(PIC_MASTER, PIC_CMD_ENDINTR);
}

void eoi_slave()
{
	outb(PIC_SLAVE, PIC_CMD_ENDINTR);
}


void idt_generic_fault(struct regs* regs);
void idt_generic_interrupt(void);
void idt_exception_DE(struct regs* regs);
void idt_exception_GP(struct regs* regs);
void idt_exception_PF(struct regs* regs);


void idt_add_interrupt(int number, unsigned int base, unsigned short selector, unsigned char flags)
{
	idt[number].base_0_15 = (unsigned short)(base & 0xFFFF);
	idt[number].selector = (unsigned short)(selector);
	idt[number].zero = 0x00;
	idt[number].flags = flags;
	idt[number].base_16_31 = (unsigned short)(base >> 16);
}

void idt_isr_handler(struct regs* regs) {
	isr_t isr = idt_isr_list[regs->int_no];
	if (isr) {
		isr(regs);
	} else {
		idt_generic_fault(regs);
	}
}


void idt_initialize(void) {
	memset(idt_isr_list, 0, sizeof(isr_t)*(IDT_ENTRIES+1));

	idtr.base = (unsigned int)idt;
	idtr.limit = sizeof(idt);

	idt_add_interrupt(0, (unsigned int)isr0, gdt_kernel_cs, 0x8E);
	idt_add_interrupt(1, (unsigned int)isr1, gdt_kernel_cs, 0x8E);
	idt_add_interrupt(2, (unsigned int)isr2, gdt_kernel_cs, 0x8E);
	idt_add_interrupt(3, (unsigned int)isr3, gdt_kernel_cs, 0x8E);
	idt_add_interrupt(4, (unsigned int)isr4, gdt_kernel_cs, 0x8E);
	idt_add_interrupt(5, (unsigned int)isr5, gdt_kernel_cs, 0x8E);
	idt_add_interrupt(6, (unsigned int)isr6, gdt_kernel_cs, 0x8E);
	idt_add_interrupt(7, (unsigned int)isr7, gdt_kernel_cs, 0x8E);
	idt_add_interrupt(8, (unsigned int)isr8, gdt_kernel_cs, 0x8E);
	idt_add_interrupt(9, (unsigned int)isr9, gdt_kernel_cs, 0x8E);
	idt_add_interrupt(10, (unsigned int)isr10, gdt_kernel_cs, 0x8E);
	idt_add_interrupt(11, (unsigned int)isr11, gdt_kernel_cs, 0x8E);
	idt_add_interrupt(12, (unsigned int)isr12, gdt_kernel_cs, 0x8E);
	idt_add_interrupt(13, (unsigned int)isr13, gdt_kernel_cs, 0x8E);
	idt_add_interrupt(14, (unsigned int)isr14, gdt_kernel_cs, 0x8E);
	idt_add_interrupt(15, (unsigned int)isr15, gdt_kernel_cs, 0x8E);
	idt_add_interrupt(16, (unsigned int)isr16, gdt_kernel_cs, 0x8E);
	idt_add_interrupt(17, (unsigned int)isr17, gdt_kernel_cs, 0x8E);
	idt_add_interrupt(18, (unsigned int)isr18, gdt_kernel_cs, 0x8E);
	idt_add_interrupt(19, (unsigned int)isr19, gdt_kernel_cs, 0x8E);
	idt_add_interrupt(20, (unsigned int)isr20, gdt_kernel_cs, 0x8E);
	idt_add_interrupt(21, (unsigned int)isr21, gdt_kernel_cs, 0x8E);
	idt_add_interrupt(22, (unsigned int)isr22, gdt_kernel_cs, 0x8E);
	idt_add_interrupt(23, (unsigned int)isr23, gdt_kernel_cs, 0x8E);
	idt_add_interrupt(24, (unsigned int)isr24, gdt_kernel_cs, 0x8E);
	idt_add_interrupt(25, (unsigned int)isr25, gdt_kernel_cs, 0x8E);
	idt_add_interrupt(26, (unsigned int)isr26, gdt_kernel_cs, 0x8E);
	idt_add_interrupt(27, (unsigned int)isr27, gdt_kernel_cs, 0x8E);
	idt_add_interrupt(28, (unsigned int)isr28, gdt_kernel_cs, 0x8E);
	idt_add_interrupt(29, (unsigned int)isr29, gdt_kernel_cs, 0x8E);
	idt_add_interrupt(30, (unsigned int)isr30, gdt_kernel_cs, 0x8E);
	idt_add_interrupt(31, (unsigned int)isr31, gdt_kernel_cs, 0x8E);
	idt_add_interrupt(32, (unsigned int)irq0, gdt_kernel_cs, 0x8E);
	idt_add_interrupt(33, (unsigned int)irq1, gdt_kernel_cs, 0x8E);
	idt_add_interrupt(34, (unsigned int)irq2, gdt_kernel_cs, 0x8E);
	idt_add_interrupt(35, (unsigned int)irq3, gdt_kernel_cs, 0x8E);
	idt_add_interrupt(36, (unsigned int)irq4, gdt_kernel_cs, 0x8E);
	idt_add_interrupt(37, (unsigned int)irq5, gdt_kernel_cs, 0x8E);
	idt_add_interrupt(38, (unsigned int)irq6, gdt_kernel_cs, 0x8E);
	idt_add_interrupt(39, (unsigned int)irq7, gdt_kernel_cs, 0x8E);
	idt_add_interrupt(40, (unsigned int)irq8, gdt_kernel_cs, 0x8E);
	idt_add_interrupt(41, (unsigned int)irq9, gdt_kernel_cs, 0x8E);
	idt_add_interrupt(42, (unsigned int)irq10, gdt_kernel_cs, 0x8E);
	idt_add_interrupt(43, (unsigned int)irq11, gdt_kernel_cs, 0x8E);
	idt_add_interrupt(44, (unsigned int)irq12, gdt_kernel_cs, 0x8E);
	idt_add_interrupt(45, (unsigned int)irq13, gdt_kernel_cs, 0x8E);
	idt_add_interrupt(46, (unsigned int)irq14, gdt_kernel_cs, 0x8E);
	idt_add_interrupt(47, (unsigned int)irq15, gdt_kernel_cs, 0x8E);

	idt_reload();
	
	outb(0x20, 0x11);							// Remap the IRQ table to interrupts 20 through 2F
	io_wait();
	outb(0xA0, 0x11);
	io_wait();
	outb(0x21, 0x20);
	io_wait();
	outb(0xA1, 0x28);
	io_wait();
	outb(0x21, 0x04);
	io_wait();
	outb(0xA1, 0x02);
	io_wait();
	outb(0x21, 0x01);
	io_wait();
	outb(0xA1, 0x01);
	io_wait();
	outb(0x21, 0x00);
	outb(0xA1, 0x00);
	
	idt_isr_list[0]  = idt_exception_DE;
	idt_isr_list[13] = idt_exception_GP;
	idt_isr_list[14] = idt_exception_PF;
	idt_isr_list[IRQ0] = isr_irq_timer;
	idt_isr_list[IRQ1] = isr_irq_generic_master;
	idt_isr_list[IRQ2] = isr_irq_generic_master;
	idt_isr_list[IRQ3] = isr_irq_generic_master;
	idt_isr_list[IRQ4] = isr_irq_generic_master;
	idt_isr_list[IRQ5] = isr_irq_generic_master;
	idt_isr_list[IRQ6] = isr_irq_generic_master;
	idt_isr_list[IRQ7] = isr_irq_generic_master;
	idt_isr_list[IRQ8] = isr_irq_rtc;
	idt_isr_list[IRQ9] = isr_irq_generic_slave;
	idt_isr_list[IRQ10] = isr_irq_generic_slave;
	idt_isr_list[IRQ11] = isr_irq_generic_slave;
	idt_isr_list[IRQ12] = isr_irq_generic_slave;
	idt_isr_list[IRQ13] = isr_irq_generic_slave;
	idt_isr_list[IRQ14] = isr_irq_generic_slave;
	idt_isr_list[IRQ15] = isr_irq_generic_slave;
}

void idt_generic_fault(struct regs* regs) {
	DisableInterrupts();
	char eflags[33];
	uitoa(eflags, regs->eflags, 2, 32);
	
	kprintf("Exception handler called, dumping registers:                                    ");
	kprintf("EAX: %8X  EBX: %8X  ECX: %8X  EDX: %8X  ESI: %8X       EDI: %8X  CS: %4X  DS: %4X  ES: %4X  FS: %4X  GS: %4X  EIP: %8X  SS: %4X  ESP: %8X  EBP: %8X  EFLAGS: %s", regs->eax, regs->ebx, regs->ecx, regs->edx, regs->esi, regs->edi, regs->cs, regs->ds, regs->es, regs->fs, regs->gs, regs->eip, regs->ss, regs->esp, regs->ebp, eflags);
	kprintf("Unhandled exception %2X occurred, error %8X.                                ", regs->int_no, regs->err_code);
	_crash();
}

void idt_exception_DE(struct regs* regs) {
	DisableInterrupts();
	char eflags[33];
	uitoa(eflags, regs->eflags, 2, 32);
	
	kprintf("Exception handler called, dumping registers:                                    ");
	kprintf("EAX: %8X  EBX: %8X  ECX: %8X  EDX: %8X  ESI: %8X       EDI: %8X  CS: %4X  DS: %4X  ES: %4X  FS: %4X  GS: %4X  EIP: %8X  SS: %4X  ESP: %8X  EBP: %8X  EFLAGS: %s", regs->eax, regs->ebx, regs->ecx, regs->edx, regs->esi, regs->edi, regs->cs, regs->ds, regs->es, regs->fs, regs->gs, regs->eip, regs->ss, regs->esp, regs->ebp, eflags);
	kprintf("Divide error occurred in kernel, error %8X. System halted.                 ", regs->err_code);
	_crash();
}

void idt_exception_GP(struct regs* regs) {
	DisableInterrupts();
	char eflags[33];
	uitoa(eflags, regs->eflags, 2, 32);
	
	kprintf("Exception handler called, dumping registers:                                    ");
	kprintf("EAX: %8X  EBX: %8X  ECX: %8X  EDX: %8X  ESI: %8X       EDI: %8X  CS: %4X  DS: %4X  ES: %4X  FS: %4X  GS: %4X  EIP: %8X  SS: %4X  ESP: %8X  EBP: %8X  EFLAGS: %s", regs->eax, regs->ebx, regs->ecx, regs->edx, regs->esi, regs->edi, regs->cs, regs->ds, regs->es, regs->fs, regs->gs, regs->eip, regs->ss, regs->esp, regs->ebp, eflags);
	kprintf("General protection fault occurred in kernel, error %8X. System halted.     ", regs->err_code);
	_crash();
}

void idt_exception_PF(struct regs* regs) {
	unsigned int cr2;
	DisableInterrupts();
	asm volatile ("mov %%cr2, %0" : "=r"(cr2));
	char eflags[33];
	uitoa(eflags, regs->eflags, 2, 32);
	
}