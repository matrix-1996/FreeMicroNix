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


#endif
