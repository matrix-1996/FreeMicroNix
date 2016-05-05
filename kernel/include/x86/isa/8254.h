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