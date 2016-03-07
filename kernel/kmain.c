#include <io.h>
#include <video/vga.h>
#include <x86.h>
#include <cpuid.h>

void kmain()
{
	init_terminal();
	terminal_writestring("FreeMicroNix Version 0.01 Reborn\n");
	init_gdt(); // Initialize the Global Descriptor Table

	/*
	char * cputype = detect_cpu(); // Detect the type of CPU
	terminal_writestring("CPU is:\n")
	terminal_writestring(cputype);
	terminal_writestring("\n")
	*/

	init_idt();

}