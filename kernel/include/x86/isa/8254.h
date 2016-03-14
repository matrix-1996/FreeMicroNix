#ifndef _X86_ISA_8254_H
#define _X86_ISA_8254_H

#include <stdbool.h>
#include <stdint.h>

#define PIT_CHANNEL_0           0x40
#define PIT_CHANNEL_1           0x41
#define PIT_CHANNEL_2           0x42
#define PIT_COMMAND_REGISTER    0x43

void PIT_8254_Phase(uint32_t hz);
void PIT_8254_Handler();
void PIT_8254_Wait(uint64_t ticks);
void Initialize_PIT_8254();

#endif