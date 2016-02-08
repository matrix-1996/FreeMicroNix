#ifndef INCLUDE_IO_H
#define INCLUDE_IO_H

__attribute__((unused))
static inline unsigned char outb(unsigned short port, unsigned char value)
{
	asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
	return value;
}

__attribute__((unused))
static inline unsigned short outw(unsigned short port, unsigned short value)
{
	asm volatile ("outw %1, %0" : : "dN" (port), "a" (value));
	return value;
}

__attribute__((unused))
static inline unsigned int outd(unsigned short port, unsigned int value)
{
	asm volatile ("outl %1, %0" : : "dN" (port), "a" (value));
	return value;
}

__attribute__((unused))
static inline unsigned char inb(unsigned short port)
{
	unsigned char result;
	asm volatile("inb %1, %0" : "=a" (result) : "dN" (port));
	return result;
}

__attribute__((unused))
static inline unsigned short inw(unsigned short port)
{
	unsigned short result;
	asm volatile("inw %1, %0" : "=a" (result) : "dN" (port));
	return result;
}

__attribute__((unused))
static inline unsigned int ind(unsigned short port)
{
	unsigned int result;
	asm volatile("inl %1, %0" : "=a" (result) : "dN" (port));
	return result;
}



#endif