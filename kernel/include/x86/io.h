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