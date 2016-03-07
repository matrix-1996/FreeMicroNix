#include <x86/io.h>
#include <x86/video/vga.h>
#include <cpuid.h>

#if defined(__i386__)
#include <i386/gdt.h>

#endif
void kmain()
{
	init_terminal();
	kprintf("FreeMicroNix Version 0.01 Reborn\n");
	Initialize_GDT();// Initialize the Global Descriptor Table
	Initialize_IDT();
	Initialize_8259_PIC();
}