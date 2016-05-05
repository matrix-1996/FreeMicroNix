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

#include <x86/isa/idt.h>
#include <x86/isa/interrupt.h>
#include <x86/video/vga.h>
#include <string.h>
#include <stdint.h>

extern void idt_loader(void);

idt_entry_t idt[NUM_INTERRUPTS];
idt_ptr_t idtp;


void Create_IDT_Entry(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt[num].base_hi = (base >> 16) & 0xFFFF;
    idt[num].base_lo = (base & 0xFFFF);
    idt[num].sel = sel;
    idt[num].always0 = 0;
    idt[num].flags = flags;
}

void Install_IDT(void)
{
	idtp.limit = (sizeof(idt_entry_t) * NUM_INTERRUPTS) -1;
	idtp.base  = (uint32_t) &idt;
	memset(&idt, 0, sizeof(idt_entry_t) * NUM_INTERRUPTS);
	
	idt_loader();
	kprintf("Interrupt Descriptor Table Initialized\n");
}