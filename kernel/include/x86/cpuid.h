#ifndef _X86_CPUID_H
#define _X86_CPUID_H

#include <stdint.h>
#include <x86.h>

typedef struct regs_t
{
	uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
	uint32_t ds, es, fs, gs;
	uint32_t which_int, err_code;
	uint32_t eip, cs, eflags, user_esp, user_ss;
} __attribute__((packed)) regs_t;

regs_t cpu_cpuid(int code);
uint32_t cpu_vendor_name(char *name);
char* detect_cpu();

#endif