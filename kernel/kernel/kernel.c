#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <kernel/tty.h>
#include <i386/idt.h>
#include <i386/pic.h>
#include <i386/timer.h>


void kernel_early(void)
{
	terminal_initialize();
}

void kernel_main(void)
{
	printf("FreeMicroNix\nVersion 0.01 Blue Panda\nCopyright 2015 William Taylor Jones\n");
	idt_initialize();
	pic_initialize();
	asm volatile("sti");
	asm volatile("int $0x3");
    asm volatile("int $0x4");
	init_timer(100);
}
