#ifndef _CPUID_H
#define _CPUID_H

#include <stdint.h>
#include <x86.h>

regs_t cpu_cpuid(int code);
uint32_t cpu_vendor_name(char *name);
char* detect_cpu();

#endif