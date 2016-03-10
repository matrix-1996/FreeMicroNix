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

/*
char* i64toa(char* buf, long long val, int base, int min);
*/
char* itoa(int val,char *str, int base);
#endif
