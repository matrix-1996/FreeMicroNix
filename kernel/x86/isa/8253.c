#include <stdint.h>
#include <stdbool.h>
#include <x86/isa/8259.h>
#include <x86/video/vga.h>

uint32_t timer_ticks;
uint32_t timer_frequency;
bool timer_initialized = false;
uint16_t timer_divisor;

void Initialize_8253_PIT(uint32_t frequency, bool reset_ticks)
{
	if (reset_ticks)
		timer_ticks = 0;

	if (frequency > 18) {
		timer_divisor = 1193180 / frequency;
		timer_frequency = frequency;
	} else {
		timer_divisor = 0;
		timer_frequency = 18;
	}

	outb(0x43, 0x36); // Command to reinitialize the PIT it repeating mode
	outb(0x40, (timer_divisor & 0xFF));
	outb(0x40, ((timer_divisor >> 8) & 0xFF));

	timer_initialized = true;
}

void ISR_IRQ_8253_PIT() 
{
	if (!timer_initialized) {
		outb(0x20, 0x20);
		return;
	}
	timer_ticks++;

	kprintf("Timer Ticks:%d\n", timer_ticks); // for debugging

	PIC_EOI_Master();
}