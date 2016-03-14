#include <stdint.h>
#include <stdbool.h>
#include <x86/isa/8259.h>
#include <x86/isa/8254.h>
#include <x86/isa/interrupt.h>
#include <x86/video/vga.h>
#include <x86/io.h>

volatile uint64_t timer_ticks = 0;
uint64_t timer_seconds = 0;
uint64_t timer_milliseconds = 0;

void PIT_8254_Phase(uint32_t hz)
{
	uint32_t divisor = 1193182 / hz;
	outb(PIT_COMMAND_REGISTER, 0x36);
	outb(PIT_CHANNEL_0, divisor & 0xFF);
	outb(PIT_CHANNEL_0, divisor >> 8);

}

void PIT_8254_Handler() 
{
	timer_ticks++;
	if(timer_ticks % 1000 == 0)
	{
		timer_seconds++; // Pretty inaccurate
	}
}

void PIT_8254_Wait(uint64_t ticks)
{
	uint64_t eticks = timer_ticks + ticks;
	for(;eticks > timer_ticks;)
	{
		__asm__ __volatile__("hlt");
	}
}

void Initialize_PIT_8254()
{
	PIT_8254_Phase(1000);
	Install_Interrupt_Handler(0, PIT_8254_Handler);
	kprintf("8254 PIT Initialized on channel 0\n");
}

