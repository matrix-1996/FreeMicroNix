#include <i386/kprintf.h>
#include <kernel/tty.h>
#include <i386/global.h>
#include <i386/gdt.h>
#include <i386/interrupt.h>
#include <i386/rtc.h>
#include <i386/mm.h>
#include <i386/timer.h>

void kernel_early(void)
{
	terminal_initialize();
}

void kernel_main(void)
{
	kprintf("FreeMicroNix\nVersion 0.02 Green Tiger\nCopyright 2015 William Taylor Jones\n");
	gdt_initialize();
	idt_initialize();
	timer_initialize(100, true);
	EnableInterrupts();
	rtc_initialize();
}
