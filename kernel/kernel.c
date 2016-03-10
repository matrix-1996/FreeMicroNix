#include <x86/io.h>
#include <x86/video/vga.h>
#include <x86/isa/8259.h>
#include <x86/isa/8254.h>
#include <x86/isa/interrupt.h>
#include <stdbool.h>

#if defined(__i386__)
#include <i386/gdt.h>

#endif
void kmain()
{
	init_terminal();
	kprintf("FreeMicroNix Version 0.01 Reborn\n");
	asm ("sti");
	Initialize_GDT(); // Initialize the Global Descriptor Table
	Initialize_Interrupts(); // Initialize the Interrupt Descriptor Table and 8259 Programmable Interrupt Controller
	Initialize_8254_PIT(100,true);  // Initialize the 8254 Programmable Interval Timer

}
