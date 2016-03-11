#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <x86/io.h>
#include <x86/multiboot.h>
#include <x86/video/vga.h>
#include <x86/isa/idt.h>
#include <x86/isa/8259.h>
#include <x86/isa/8254.h>
#include <x86/isa/interrupt.h>
#include <i386/gdt.h>
#include <i386/mm/memtype.h>
#include <i386/mm/paging.h>


void kmain(uint32_t magic, uint32_t mboot_addr, uint32_t kernel_physical_end, uint32_t kernel_physical_start)
{
	multiboot_info_t* mbi;
	Initialize_Terminal();
	
	// if Kernel Loaded Improperly
	if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
	{
		set_terminal_color(COLOR_LIGHT_RED);
		kprintf("INVALID_MULTIBOOT NUMBER!\n");
		set_terminal_color(COLOR_LIGHT_GREY);
		return; 
	}

	mbi = (multiboot_info_t*) mboot_addr;

	kprintf("FreeMicroNix Version 0.1\n");
    
	Initialize_8259_PIC(0x20, 0x28);      // Initialize the 8259 Programmable Interrupt Controller
	
		//Initialize_8254_PIT(100,true);  // Initialize the 8254 Programmable Interval Timer
	Initialize_GDT();                     // Initialize the Global Descriptor Table
	
	Interrupt_Handler_Installer();	      // Install Exception and IRQ Handlers
	Install_IDT();					      // Install the Interrupt Descriptor Table


	char *mem_suffix;
	uint32_t mem_divisor;
	if(mbi->mem_upper > MB)
	{
		mem_suffix = "MB";
		mem_divisor = MB;
	} 

	else 
	{
		mem_suffix = "KB";
		mem_divisor = KB;
	}

	kprintf("Available Memory: %d%s\n", mbi->mem_upper /mem_divisor, mem_suffix);							
    kprintf("Kernel loaded from 0x%x-0x%x (%dKB-%dKB)\n", kernel_physical_start, kernel_physical_end, kernel_physical_start / KB, kernel_physical_end / KB);

    //Initialize_Paging();

	kprintf("\nKernel Booted\n");
    for(;;){__asm__ __volatile__ ("hlt");}


}
