#include <x86/io.h>
#include <x86/video/vga.h>
#include <x86/isa/8254.h>
#include <x86/isa/interrupt.h>
#include <stdint.h>

#define KEYBOARD_INTERFACE 0x64
#define bit(n) (1<<(n))
#define check_flag(flags, n) ((flags) & bit (n))


void keyboard_reboot(void)
{
	uint8_t temp;
	
	disable_interrupts();

	do
	{
		temp = inb(KEYBOARD_INTERFACE);	// empty user data
		if (check_flag(temp, 0) != 0)
		{
			inb(0x60)					//  empty keyboard data
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
	kprintf("Ctrl+Alt+Del Pressed, restarting CPU\n");	// Warn the user
	PIT_8254_Wait(5000); 								// Wait approximately 5 seconds
	keyboard_reboot();	 								// Do keyboard_reboot()
}