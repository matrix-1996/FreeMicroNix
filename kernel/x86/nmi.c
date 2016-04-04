#include <x86/nmi.h>

bool nmi_enabled = false;

void NMI_Enable(void)
{
	outb(0x70, inb(0x70) & 0x7F);
	nmi_enabled = true;
}

void NMI_Disable(void)
{
	outb(0x70, inb(0x70) | 0x80);
}

bool NMI_Is_Enabled(void)
{
	return nmi_enabled;
}