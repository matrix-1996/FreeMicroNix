#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include <kernel/tty.h>

void kernel_early(void)
{
	terminal_initialize();
}

void kernel_main(void)
{
	init_descriptor_tables();
	printf("FreeMicroNix\nVersion 0.01 Blue Panda\nCopyright 2015 William Taylor Jones\n");
}
