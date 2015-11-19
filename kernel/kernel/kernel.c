#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <kernel/tty.h>
#include <i386/idt.h>
#include <i386/pic.h>
#include <i386/pit_init.h>
#include <i386/interrupt.h>


void kernel_early(void)
{
	terminal_initialize();
}

void kernel_main(void)
{
	printf("FreeMicroNix\nVersion 0.01 Blue Panda\nCopyright 2015 William Taylor Jones\n");
	idt_initialize();
	pic_initialize();
	init_pit(100);
}
