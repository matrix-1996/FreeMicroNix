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

#ifndef _X86_ISA_IDT_H
#define _X86_ISA_IDT_H

#include <stdint.h>

typedef struct idt_entry {
    uint16_t base_lo;
    uint16_t sel;
    uint8_t always0;
    uint8_t flags;
    uint16_t base_hi;
}  idt_entry_t;


typedef struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__ ((packed)) idt_ptr_t;


void Create_IDT_Entry(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);
void Install_IDT(void);

#endif
