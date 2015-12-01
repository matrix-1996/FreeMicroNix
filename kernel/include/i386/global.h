#ifndef _I386_GLOBAL_H
#define _I386_GLOBAL_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <i386/registers.h>
#include <string.h>
#include <stddef.h>
#include <stdarg.h>


struct regs {
	unsigned int gs, fs, es, ds;
	unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;
	unsigned int int_no, err_code;
	unsigned int eip, cs, eflags, useresp, ss;
};

typedef void (*isr_t) (struct regs *);

static inline void _crash(void) {
	asm volatile ("cli; hlt");
}

static inline void io_wait(void) {
	asm volatile ( "outb %%al, $0x80" : : "a"(0) );
}

typedef struct {
	int second, minute, hour, day, month, year, weekday;
} datetime_t;

int64_t time_timestamp(datetime_t dt);
int64_t time_set(int64_t newtime);
int64_t time_get(void);
int time_get_weekday(void);

__attribute__((unused))
static inline uint8_t outb(uint16_t port, uint8_t value)
{
	asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
	return value;
}

__attribute__((unused))
static inline uint16_t outw(uint16_t port, uint16_t value)
{
	asm volatile ("outw %1, %0" : : "dN" (port), "a" (value));
	return value;
}

__attribute__((unused))
static inline unsigned int outd(uint16_t port, unsigned int value)
{
	asm volatile ("outl %1, %0" : : "dN" (port), "a" (value));
	return value;
}

__attribute__((unused))
static inline uint8_t inb(uint16_t port)
{
	uint8_t result;
	asm volatile("inb %1, %0" : "=a" (result) : "dN" (port));
	return result;
}

__attribute__((unused))
static inline uint16_t inw(uint16_t port)
{
	uint16_t result;
	asm volatile("inw %1, %0" : "=a" (result) : "dN" (port));
	return result;
}

__attribute__((unused))
static inline unsigned int ind(uint16_t port)
{
	unsigned int result;
	asm volatile("inl %1, %0" : "=a" (result) : "dN" (port));
	return result;
}
uint64_t cpu_rdtsc();

#endif
