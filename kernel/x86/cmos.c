#include <x86/cmos.h>
#include <x86/nmi.h>

uint8_t CMOS_Read(uint8_t reg)
{
	outb(0x70, ( NMI_Is_Enabled() << 7 ) | reg);
	return inb(0x71);
}

