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


#ifndef _STDLIB_H
#define _STDLIB_H 1

#include <sys/cdefs.h>
#include <string.h>

void exit(int status);
void abort(void);
char* strreverse(char * str);
int abs(int val);
int atoi(const char * str);
char* u64toa(char* buf, unsigned long long val, int base, int min);
char* uitoa(char* buf, unsigned int val, int base, int min);
char* itoa(int val,char *str, int base);
#endif
