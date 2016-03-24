#ifndef _X86_ISA_INTERRUPT_H
#define _X86_ISA_INTERRUPT_H

#include <stdint.h>

#define IRQ0  32
#define IRQ1  33
#define IRQ2  34
#define IRQ3  35
#define IRQ4  36
#define IRQ5  37
#define IRQ6  38
#define IRQ7  39
#define IRQ8  40
#define IRQ9  41
#define IRQ10 42
#define IRQ11 43
#define IRQ12 44
#define IRQ13 45
#define IRQ14 46
#define IRQ15 47

#define NUM_INTERRUPTS 256


/*
PC Interrupts:
IRQ	Interrupt
0	32	System Timer
1	33	Keyboard
2	34	Cascade
3	35	Serial Port 2
4	36	Serial Port 1
5	37	Parallel Port 2 or Sound Card
6	38	Floppy
7	39	Parallel Port 1
8	40	Real Time Clock
9	41	Network (or ATA 3)
10	42	Network/Sound/SCSI
11	43	SCSI Other (or ATA 2)
12	44	PS/2 Mouse
13	45	FPU
14	46	ATA 0
15	47	ATA 1
*/

typedef struct interrupt_context
{

	uint32_t gs, fs, es, ds;
	uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
	uint32_t int_no, err_code;
	uint32_t eip, cs, eflags, useresp, ss;

} interrupt_context_t;


void Install_Interrupt_Handler(uint32_t index, void(*handler) (interrupt_context_t* r));
void Uninstall_Interrupt_Handler(uint32_t index);
void Interrupt_Handler_Installer(void);
void Enable_Interrupt(uint32_t irq);
void Disable_Interrupt(uint32_t irq);
void Disable_Interrupts(void);
void Enable_Interrupts(void);
void Pause_Interrupts(void);

#endif
