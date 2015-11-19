#include <i386/interrupt.h>
#include <kernel/tty.h>

extern void IRQ0_handler();
extern void init_PIT(uint32_t frequency);

void init_pit(uint32_t frequency)
{
	init_pit(frequency); // call the assembly function to initialize the pit
	terminal_writestring("Programmable Interval Timer Initialized\n");
	register_interrupt_handler(IRQ0, &IRQ0_handler); // register the IRQ0 handler
	terminal_writestring("Programmable Interval Timer set to IRQ 0\n");
}
