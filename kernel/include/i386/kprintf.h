#ifndef _I386_KPRINTF_H
#define _I386_KPRINTF_H

#include <kernel/tty.h>
#include <i386/global.h>

int kvsnprintf(char* str, size_t size, const char* fmt, va_list va);
int kprintf(const char *fmt, ...)
	__attribute__ ((format (printf, 1, 2)));
int snprintf(char* s, size_t size, const char *fmt, ...)
	__attribute__ ((format (printf, 3, 4)));
void crash(char* file, int line, const char* fmt, ...)
	__attribute__ ((format (printf, 3, 4)));
int _debug_printf(const char* fmt, ...);

#endif