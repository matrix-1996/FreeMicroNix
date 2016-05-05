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

#ifndef _X86_ISA_8259_H
#define _X86_ISA_8259_H

#include <stdbool.h>
#include <stdint.h>

void Initialize_8259_PIC(uint32_t PIC_MASTER_BASE ,uint32_t PIC_SLAVE_BASE);
void Enable_8259_PIC(uint8_t irq);
void Disable_8259_PIC(uint8_t irq);
void Acknowledge_8259_PIC( uint8_t irq );
uint16_t PIC_Read_IRR(void);
uint16_t PIC_Read_ISR(void);
void PIC_EOI_Master(void);
void PIC_EOI_Slave(void);

#endif