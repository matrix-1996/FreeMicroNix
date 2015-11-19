#include <stdint.h>
#include <string.h>

#include <i386/idt.h>

#define IDT_TYPE_INTERRUPT 0xE /* Interrupts disabled in eflags. */
#define IDT_TYPE_TRAP 0xF      /* Interrupts unaffected in eflags. */

#define IDT_FLAG_PRESENT (1 << 7)
#define IDT_FLAG_DPL_SHIFT 5
#define IDT_FLAG_DPL_BITS 2
#define IDT_FLAG_TYPE_SHIFT 0
#define IDT_FLAG_TYPE_BITS 4



void isr0();
void isr1();
void isr2();
void isr3();
void isr4();
void isr5();
void isr6();
void isr7();
void isr8();
void isr9();
void isr10();
void isr11();
void isr12();
void isr13();
void isr14();
void isr15();
void isr16();
void isr17();
void isr18();
void isr19();
void isr20();
void isr21();
void isr22();
void isr23();
void isr24();
void isr25();
void isr26();
void isr27();
void isr28();
void isr29();
void isr30();
void isr31();
void irq0();
void irq1();
void irq2();
void irq3();
void irq4();
void irq5();
void irq6();
void irq7();
void irq8();
void irq9();
void irq10();
void irq11();
void irq12();
void irq13();
void irq14();
void irq15();


struct idt_entry idt[256];



void Set(struct idt_entry* table, size_t length)
{
	size_t limit = sizeof(idt_entry) * length - 1;
	asm volatile ("subl $6, %%esp\n\t"
	              "movw %w0, 0(%%esp)\n\t"
	              "movl %1, 2(%%esp)\n\t"
	              "lidt (%%esp)\n\t"
	              "addl $6, %%esp" : : "rN"(limit), "r"(table));
}

void SetEntry(struct idt_entry* entry, uintptr_t handler, uint16_t selector, uint8_t flags, uint8_t ist)
{
	entry->flags = flags;
	entry->ist = ist;
	entry->selector = selector;
	entry->handler_low = handler >> 0 & 0xFFFF;
	entry->handler_high = handler >> 16 & 0xFFFF;
}

void idt_initialize()
{
	memset(&idt, 0, sizeof(idt));
	SetEntry(&idt[0], isr0, IDT_TYPE_INTERRUPT, 0x0);
	SetEntry(&idt[1], isr1, IDT_TYPE_INTERRUPT, 0x0);
	SetEntry(&idt[2], isr2, IDT_TYPE_INTERRUPT, 0x0);
	SetEntry(&idt[3], isr3, IDT_TYPE_INTERRUPT, 0x0);
	SetEntry(&idt[4], isr4, IDT_TYPE_INTERRUPT, 0x0);
	SetEntry(&idt[5], isr5, IDT_TYPE_INTERRUPT, 0x0);
	SetEntry(&idt[6], isr6, IDT_TYPE_INTERRUPT, 0x0);
	SetEntry(&idt[7], isr7, IDT_TYPE_INTERRUPT, 0x0);
	SetEntry(&idt[8], isr8, IDT_TYPE_INTERRUPT, 0x0);
	SetEntry(&idt[9], isr9, IDT_TYPE_INTERRUPT, 0x0);
	SetEntry(&idt[10], isr10, IDT_TYPE_INTERRUPT, 0x0);
	SetEntry(&idt[11], isr11, IDT_TYPE_INTERRUPT, 0x0);
	SetEntry(&idt[12], isr12, IDT_TYPE_INTERRUPT, 0x0);
	SetEntry(&idt[13], isr13, IDT_TYPE_INTERRUPT, 0x0);
	SetEntry(&idt[14], isr14, IDT_TYPE_INTERRUPT, 0x0);
	SetEntry(&idt[15], isr15, IDT_TYPE_INTERRUPT, 0x0);
	SetEntry(&idt[16], isr16, IDT_TYPE_INTERRUPT, 0x0);
	SetEntry(&idt[17], isr17, IDT_TYPE_INTERRUPT, 0x0);
	SetEntry(&idt[18], isr18, IDT_TYPE_INTERRUPT, 0x0);
	SetEntry(&idt[19], isr19, IDT_TYPE_INTERRUPT, 0x0);
	SetEntry(&idt[20], isr20, IDT_TYPE_INTERRUPT, 0x0);
	SetEntry(&idt[21], isr21, IDT_TYPE_INTERRUPT, 0x0);
	SetEntry(&idt[22], isr22, IDT_TYPE_INTERRUPT, 0x0);
	SetEntry(&idt[23], isr23, IDT_TYPE_INTERRUPT, 0x0);
	SetEntry(&idt[24], isr24, IDT_TYPE_INTERRUPT, 0x0);
	SetEntry(&idt[25], isr25, IDT_TYPE_INTERRUPT, 0x0);
	SetEntry(&idt[26], isr26, IDT_TYPE_INTERRUPT, 0x0);
	SetEntry(&idt[27], isr27, IDT_TYPE_INTERRUPT, 0x0);
	SetEntry(&idt[28], isr28, IDT_TYPE_INTERRUPT, 0x0);
	SetEntry(&idt[29], isr29, IDT_TYPE_INTERRUPT, 0x0);
	SetEntry(&idt[30], isr30, IDT_TYPE_INTERRUPT, 0x0);
	SetEntry(&idt[31], isr31, IDT_TYPE_INTERRUPT, 0x0);
	SetEntry(&idt[32], irq0, IDT_TYPE_INTERRUPT, 0x0);
	SetEntry(&idt[33], irq1, IDT_TYPE_INTERRUPT, 0x0);
	SetEntry(&idt[34], irq2, IDT_TYPE_INTERRUPT, 0x0);
	SetEntry(&idt[35], irq3, IDT_TYPE_INTERRUPT, 0x0);
	SetEntry(&idt[36], irq4, IDT_TYPE_INTERRUPT, 0x0);
	SetEntry(&idt[37], irq5, IDT_TYPE_INTERRUPT, 0x0);
	SetEntry(&idt[38], irq6, IDT_TYPE_INTERRUPT, 0x0);
	SetEntry(&idt[39], irq7, IDT_TYPE_INTERRUPT, 0x0);
	SetEntry(&idt[40], irq8, IDT_TYPE_INTERRUPT, 0x0);
	SetEntry(&idt[41], irq9, IDT_TYPE_INTERRUPT, 0x0);
	SetEntry(&idt[42], irq10, IDT_TYPE_INTERRUPT, 0x0);
	SetEntry(&idt[43], irq11, IDT_TYPE_INTERRUPT, 0x0);
	SetEntry(&idt[44], irq12, IDT_TYPE_INTERRUPT, 0x0);
	SetEntry(&idt[45], irq13, IDT_TYPE_INTERRUPT, 0x0);
	SetEntry(&idt[46], irq14, IDT_TYPE_INTERRUPT, 0x0);
	SetEntry(&idt[47], irq15, IDT_TYPE_INTERRUPT, 0x0);
	Set(idt, sizeof(idt) - 1);
}
