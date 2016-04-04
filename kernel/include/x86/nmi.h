#ifndef _X86_NMI_H
#define _X86_NMI_H

#include <x86/io.h>
#include <stdbool.h>

void NMI_Enable(void);
void NMI_Disable(void);
bool NMI_Is_Enabled(void);

#endif