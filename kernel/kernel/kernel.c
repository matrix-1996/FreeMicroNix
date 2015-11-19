#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <kernel/tty.h>
#include <i386/idt.h>
#include <i386/pic.h>


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
	init_pit(100);
}
