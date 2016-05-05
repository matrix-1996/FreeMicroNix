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


#ifndef _STRING_H
#define _STRING_H 1

#include <sys/cdefs.h>
#include <stddef.h>

size_t lfind(const char * str, const char accept);
int memcmp(const void * s1, const void * s2,size_t n);
void* memcpy(void *dest, const void *src, size_t n);
void* memset(void *s, int c, size_t n);
void* memmove(void* dstptr, const void* srcptr, size_t size);
char* strcat(char *dest, const char *src);
char* strchr(const char *s, int c);
int strcmp(const char * s1, const char * s2);
char* strcpy(char *dest, const char *src);
size_t strcspn(const char *s1, const char *s2);
char* strdup(const char* str);
size_t strlen(const char *s);
char* strncpy(char *dest, const char *src, size_t n);
char* strpbrk(const char * str, const char * accept);
size_t strspn(const char *s1, const char *s2);
char* strstr(char *s1, const char *s2);
char* strtok(char *str, const char *delim);
char* strtok_r(char * str, const char * delim, char ** saveptr);


#endif
