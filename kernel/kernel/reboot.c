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

#include <x86/io.h>
#include <x86/video/vga.h>
#include <x86/isa/8254.h>
#include <x86/isa/interrupt/interrupt.h>
#include <stdint.h>

#define KEYBOARD_INTERFACE 0x64
#define bit(n) (1<<(n))
#define check_flag(flags, n) ((flags) & bit (n))


void keyboard_reboot(void)
{
	uint8_t temp;
	
	Disable_Interrupts();

	do
	{
		temp = inb(KEYBOARD_INTERFACE);	// empty user data
		if (check_flag(temp, 0) != 0)
		{
			inb(0x60);					//  empty keyboard data
		}
	} while (check_flag(temp, 1) != 0);

	outb(KEYBOARD_INTERFACE, 0xFE);		// Send the reboot signal to the 8042 keyboard controller


	for (;;)
	{
		__asm__ __volatile__("hlt");		// Halt the CPU if keyboard reboot didn't work	
	}
}

void reboot(void)
{
	kprintf("Rebooting CPU\n");	// Warn the user
	PIT_8254_Wait(5000); 								// Wait approximately 5 seconds
	keyboard_reboot();	 								// Do keyboard_reboot()
}