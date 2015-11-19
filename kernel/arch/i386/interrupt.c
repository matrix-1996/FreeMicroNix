#include <stdint.h>
#include <stdio.h>

#include <i386/pic.h>
#include <i386/interrupt.h>
#include <kernel/portio.h>
#include <kernel/tty.h>
#include <i386/idt.h>
#include <stdbool.h>


struct interrupt_handler
{
	void (*handler)(struct interrupt_context*, void*);
	void* context;
	struct interrupt_handler* next;
	struct interrupt_handler* prev;
};

static struct interrupt_handler* interrupt_handlers[NUM_INTERRUPTS];


void RegisterHandler(unsigned int index, struct interrupt_handler* handler)
{
	assert(index < NUM_INTERRUPTS);
	bool was_enabled = SetEnabled(false);
	handler->prev = NULL;
	if ( (handler->next = interrupt_handlers[index]) )
		handler->next->prev = handler;
	interrupt_handlers[index] = handler;
	SetEnabled(was_enabled);
}


void UnregisterHandler(unsigned int index, struct interrupt_handler* handler)
{
	assert(index < NUM_INTERRUPTS);
	bool was_enabled = SetEnabled(false);
	if ( handler->prev )
		handler->prev->next = handler->next;
	else
		interrupt_handlers[index] = handler->next;
	if ( handler->next )
		handler->next->prev = handler->prev;
	SetEnabled(was_enabled);
}


static void RegisterRawHandler(unsigned int index,
                               void (*handler)(void),
                               bool userspace,
                               bool preemptive)
{
	assert(index < NUM_INTERRUPTS);
	addr_t handler_entry = (addr_t) handler;
	uint16_t selector = KCS;
	uint8_t rpl = userspace ? URPL : KRPL;
	uint8_t type = preemptive ? IDT::TYPE_TRAP : IDT::TYPE_INTERRUPT;
	uint8_t ist = 0;
	uint8_t flags = IDT::FLAG_PRESENT
	              | type << IDT::FLAG_TYPE_SHIFT
	              | rpl << IDT::FLAG_DPL_SHIFT;
	SetEntry(&interrupt_table[index], handler_entry, selector, flags, ist);
}



void isr_handler(struct interrupt_context* int_ctx)
{
	(void) int_ctx;
}

void irq_handler(struct interrupt_context* int_ctx)
{
	uint8_t irq = int_ctx->int_no - 32;

	terminal_writestring("Interrupt: ");
	terminal_write_decimal(irq);
	terminal_putchar('\n');

	// Handle the potentially spurious interrupts IRQ 7 and IRQ 15.
	if ( irq == 7 && !(pic_read_isr() & (1 << 7)) )
		return;
	if ( irq == 15 && !(pic_read_isr() & (1 << 15)) )
		return pic_eoi_master();

	(void) int_ctx;

	// Send the end of interrupt signal to the relevant PICs.
	if ( 8 <= irq )
		pic_eoi_slave();
	pic_eoi_master();
}

void interrupt_handler(struct interrupt_context* int_ctx)
{
	if ( int_ctx->int_no < 32 )
		isr_handler(int_ctx);
	else if ( 32 <= int_ctx->int_no && int_ctx->int_no < 32 + 16 )
		irq_handler(int_ctx);
}
