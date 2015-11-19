#ifndef _I386_IDT_H
#define _I386_IDT_H


struct idt_entry
{
	uint16_t handler_low;
	uint16_t selector;
	uint8_t ist;
	uint8_t flags;
	uint16_t handler_high;
};


static const uint8_t FLAG_PRESENT = 1 << 7;
static const uint8_t FLAG_DPL_SHIFT = 5;
static const uint8_t FLAG_DPL_BITS = 2;
static const uint8_t FLAG_TYPE_SHIFT = 0;
static const uint8_t FLAG_TYPE_BITS = 4;

static const uint8_t TYPE_INTERRUPT = 0xE;
static const uint8_t TYPE_TRAP = 0xF;

void Set(struct idt_entry* table, size_t length);
void SetEntry(struct idt_entry* entry, uintptr_t handler, uint16_t selector,
              uint8_t flags, uint8_t ist);


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

void idt_initialize();

#endif
