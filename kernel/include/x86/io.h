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

#ifndef _X86_IO_H
#define _X86_IO_H

#include <stdint.h>

__attribute__((unused))
static inline void outb(uint16_t port, uint8_t value)
{
	asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}

__attribute__((unused))
static inline void outw(uint16_t port, uint16_t value)
{
	asm volatile ("outw %1, %0" : : "dN" (port), "a" (value));
}

__attribute__((unused))
static inline void outd(uint16_t port, uint32_t value)
{
	asm volatile ("outl %1, %0" : : "dN" (port), "a" (value));
}

__attribute__((unused))
static inline uint8_t inb(uint16_t port)
{
	unsigned char result;
	asm volatile("inb %1, %0" : "=a" (result) : "dN" (port));
	return result;
}

__attribute__((unused))
static inline uint16_t inw(uint16_t port)
{
	unsigned short result;
	asm volatile("inw %1, %0" : "=a" (result) : "dN" (port));
	return result;
}

__attribute__((unused))
static inline uint32_t ind(uint16_t port)
{
	unsigned int result;
	asm volatile("inl %1, %0" : "=a" (result) : "dN" (port));
	return result;
}

static inline void io_wait(void)
{
    outb(0x80,0);
}



#endif