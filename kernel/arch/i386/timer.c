#include <kernel/tty.h>
#include <kernel/portio.h>
#include <i386/interrupt.h>

uint32_t tick = 0;

static void timer_callback(registers_t regs)
{
    tick++;
    terminal_writestring("Tick: ");
    terminal_write_decimal(tick);
    terminal_putchar('\n');
}


void init_timer(uint32_t frequency)
{	
	register_interrupt_handler(IRQ0, &timer_callback);
	uint32_t divisor = 1193180 / frequency;
	outport8(0x43,0x36);
	uint8_t l = (uint8_t)(divisor & 0xFF);
    uint8_t h = (uint8_t)( (divisor>>8) & 0xFF );
    outport8(0x40, l);
    outport8(0x40, h);
}