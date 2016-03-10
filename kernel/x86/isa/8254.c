#include <stdint.h>
#include <stdbool.h>
#include <x86/isa/8259.h>
#include <x86/video/vga.h>
#include <x86/io.h>

uint32_t timer_ticks;
uint32_t timer_frequency;
bool timer_initialized = false;
uint16_t timer_divisor;

void Initialize_8254_PIT(uint32_t frequency, bool reset_ticks)
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

void IRQ_8254_PIT_Handler() 
{
	if (!timer_initialized) {
		PIC_EOI_Master();
		return;
	}
	timer_ticks++;

	kprintf("Timer Ticks:%d\n", timer_ticks); // for debugging

	PIC_EOI_Master();
}