#include <x86/isa/8254.h>
#include <x86/sound/speaker.h>
#include <x86/io.h>

void Speaker_Beep(void)
{
	outb(0x43, 0xB6);					// Set timer 2 to speaker mode
	outb(0x42, 0xEE);					// Set frequency to 5 KHz
	outb(0x61, (inb(0x61) | 0x03 ) );	// Activate speaker and connect timer to it
	PIT_8254_Wait(2000);				// Wait for 2 seconds
	outb(0x61, (inb(0x61) & 0xFC ) );	// Turn off speaker
}
