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
