#ifndef _I386_INTERRUPT_H
#define _I386_INTERRUPT_H

#include <stdint.h>
#include <i386/registers.h>

const unsigned int IRQ0 = 32;
const unsigned int IRQ1 = 33;
const unsigned int IRQ2 = 34;
const unsigned int IRQ3 = 35;
const unsigned int IRQ4 = 36;
const unsigned int IRQ5 = 37;
const unsigned int IRQ6 = 38;
const unsigned int IRQ7 = 39;
const unsigned int IRQ8 = 40;
const unsigned int IRQ9 = 41;
const unsigned int IRQ10 = 42;
const unsigned int IRQ11 = 43;
const unsigned int IRQ12 = 44;
const unsigned int IRQ13 = 45;
const unsigned int IRQ14 = 46;
const unsigned int IRQ15 = 47;

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

void RegisterHandler(unsigned int index, struct interrupt_handler* handler);

void UnregisterHandler(unsigned int index, struct interrupt_handler* handler);

void isr_handler(struct interrupt_context* int_ctx);

void irq_handler(struct interrupt_context* int_ctx);

void interrupt_handler(struct interrupt_context* int_ctx);

#endif