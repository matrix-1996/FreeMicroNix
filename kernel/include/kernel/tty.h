#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H

#include <i386/global.h>

void terminal_initialize(void);
void terminal_clear(void);
void terminal_move_cursor(void);
void terminal_scroll(void);
void terminal_putchar(char c);
void terminal_write_hex(unsigned int n);
void terminal_write_decimal(int d);
void terminal_write(const char* data, size_t size);
void terminal_writestring(const char* data);

#endif
