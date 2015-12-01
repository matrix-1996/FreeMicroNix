#ifndef _I386_INTERRUPT_H
#define _I386_INTERRUPT_H

#include <i386/global.h>

inline bool InterruptsAreEnabled()
{
	unsigned long are_enabled;
	asm("pushf\t\n"
	    "pop %0\t\n"
	    "and $0x000200, %0" : "=r"(are_enabled));
	return are_enabled != 0;
}


inline void EnableInterrupts()
{
	asm volatile("sti");
}

inline void DisableInterrupts()
{
	asm volatile("cli");
}

void idt_add_interrupt(int number, unsigned int base, unsigned short selector, unsigned char flags);

void idt_initialize(void);

#endif
