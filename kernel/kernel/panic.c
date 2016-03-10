#include <x86/video/vga.h>

void panic(void)
{
	kprintf("\nKernel Panic:\n");
	asm volatile ("cli");
	asm volatile ("hlt");
}