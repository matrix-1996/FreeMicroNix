#include <stdint.h>
#include <stdio.h>

#include <i386/pic.h>
#include <i386/interrupt.h>
#include <kernel/portio.h>
#include <kernel/tty.h>

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

