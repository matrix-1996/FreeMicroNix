/*
Copyright (C) <2016>  William Taylor Jones <freemicronix@gmail.com>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <x86/io.h>
#include <x86/multiboot.h>
#include <x86/video/vga.h>
#include <x86/sound/speaker.h>
#include <x86/isa/interrupt/idt.h>
#include <x86/isa/interrupt/interrupt.h>
#include <x86/isa/8259.h>
#include <x86/isa/8254.h>
#include <x86/gdt.h>
#include <x86/rtc.h>
#include <kernel/reboot.h>
#include <kernel/panic.h>

void kmain(uint32_t magic, uint32_t mboot_addr)
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

	kprintf("Available Memory: %d%s (%dKB)\n", mbi->mem_upper /mem_divisor, mem_suffix, mbi->mem_upper);


    kprintf("Interrupts Enabled\n");
    Enable_Interrupts();

    //Install_Syscall_Handler();

    /*
    Initialize_RTC();					  // Initialize the Real Time Clock Handler

    RTC_Update();
    rtc_time_t* rtctime = RTC_Get_Current_Time();

    kprintf("RTC Reported Time: %d:%d:%d %d/%d/%d UTC\n",rtctime->hour, rtctime->minute, rtctime->second, rtctime->month, rtctime->day, rtctime->year);

	*/
	
    kprintf("Testing Programmable Interval Timer: Waiting 2 seconds\n");
    PIT_8254_Wait(2000);
    kprintf("Waited 2 seconds\n");
    
    kprintf("RTC Ticks: %d\n",Get_RTC_Ticks());

    kprintf("Testing Internal PC Speaker\nYou will hear a beep if present\n");
    Speaker_Beep();

    kprintf("System Loaded\n");
    




    for(;;){__asm__ __volatile__ ("hlt");}

}

