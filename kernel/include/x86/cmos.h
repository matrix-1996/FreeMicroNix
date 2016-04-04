#ifndef _X86_CMOS_H
#define _X86_CMOS_H

#include <x86/cmos.h>
#include <x86/io.h>
#include <x86/nmi.h>
#include <stdint.h>


uint8_t CMOS_Read(uint8_t reg);

#endif