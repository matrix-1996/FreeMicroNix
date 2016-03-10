#ifndef _X86_ISA_8254_H
#define _X86_ISA_8254_H

#include <stdbool.h>

void Initialize_8254_PIT(uint32_t frequency, bool reset_ticks);
void IRQ_8254_PIT_Handler();
#endif