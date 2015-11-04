#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <kernel/timer.h>
#include <kernel/descriptor_tables.h>
#include <kernel/tty.h>

void kernel_early(void)
{
	init_descriptor_tables();
	terminal_initialize();
}

void kernel_main(void)
{
	printf("FreeMicroNix\nVersion 0.01 Blue Panda\nCopyright 2015 William Taylor Jones\n");
	asm volatile ("int $0x3");
	asm volatile ("int $0x4");

	asm volatile ("sti");
	init_timer(50);

}
