#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <x86/io.h>
#include <x86/multiboot.h>
#include <x86/video/vga.h>
#include <x86/sound/speaker.h>
#include <x86/isa/idt.h>
#include <x86/isa/8259.h>
#include <x86/isa/8254.h>
#include <x86/isa/interrupt.h>
#include <x86/isa/do_irq.h>
#include <i386/gdt.h>
#include <i386/mm/paging.h>


void kmain(uint32_t magic, uint32_t mboot_addr, uint32_t kernel_physical_end, uint32_t kernel_physical_start)
{
	multiboot_info_t* mbi;
	Initialize_Terminal();
	
	// if Kernel Loaded Improperly
	if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
	{
		Terminal_Set_Color(COLOR_LIGHT_RED);
		kprintf("INVALID_MULTIBOOT NUMBER!\n");
		Terminal_Set_Color(COLOR_LIGHT_GREY);
		return; 
	}

	mbi = (multiboot_info_t*) mboot_addr;

	Terminal_Set_Color(COLOR_LIGHT_BLUE);
	kprintf("FreeMicroNix Version 0.1\n");
    Terminal_Set_Color(COLOR_LIGHT_GREY);
	
	Initialize_PIT_8254();  			  // Initialize the 8254 Programmable Interval Timer
	
	Initialize_GDT();                     // Initialize the Global Descriptor Table

	Install_IDT();					      // Install the Interrupt Descriptor Table

	Interrupt_Handler_Installer();	      // Install Exception and IRQ Handlers


	char *mem_suffix;
	uint32_t mem_divisor;
	if(mbi->mem_upper > 100000)
	{
		mem_suffix = "MB";
		mem_divisor = 1000;
	} 

	else 
	{
		mem_suffix = "KB";
		mem_divisor = 1;
	}

	kprintf("Available Memory: %d%s\n", mbi->mem_upper /mem_divisor, mem_suffix);							
    kprintf("Kernel loaded from 0x%x-0x%x (%dKB-%dKB)\n", kernel_physical_start, kernel_physical_end, kernel_physical_start / 1024, kernel_physical_end / 1024);

	//Initialize_Paging();
    Enable_Interrupts();

    kprintf("Waiting 5 seconds\n");
    PIT_8254_Wait(5000);
    kprintf("Waited 5 seconds\n");
    
    kprintf("Testing Internal PC Speaker\nYou will hear a beep if present\n");
    Speaker_Beep();

    kprintf("System Loaded\n");
    for(;;){__asm__ __volatile__ ("hlt");}


}
