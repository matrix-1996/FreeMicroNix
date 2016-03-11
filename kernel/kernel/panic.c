#include <x86/video/vga.h>

void panic(void)
{
	kprintf("\nKernel Panic!\n");
	for (;;)
	{
		asm volatile ("hlt");		
	}
}