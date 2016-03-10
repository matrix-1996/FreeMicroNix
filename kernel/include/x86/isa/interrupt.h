#ifndef _X86_ISA_INTERRUPT_H
#define _X86_ISA_INTERRUPT_H

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

#define IDTBASE 0x00000000
#define IDTSIZE 0xFF
#define IDT_ENTRIES 255
#define INTGATE  0x8E00
#define TRAPGATE 0xEF00
#define IDT_TYPE_INTERRUPT 0xE /* Interrupts disabled in eflags. */
#define IDT_TYPE_TRAP 0xF      /* Interrupts unaffected in eflags. */

#define IDT_FLAG_PRESENT (1 << 7)
#define IDT_FLAG_DPL_SHIFT 5
#define IDT_FLAG_DPL_BITS 2
#define IDT_FLAG_TYPE_SHIFT 0
#define IDT_FLAG_TYPE_BITS 4

typedef struct interrupt_context
{
	uint32_t cr2;
	uint32_t gs;
	uint32_t fs;
	uint32_t ds;
	uint32_t es;
	uint32_t edi;
	uint32_t esi;
	uint32_t ebp;
	uint32_t ebx;
	uint32_t edx;
	uint32_t ecx;
	uint32_t eax;
	uint32_t int_no;
	uint32_t err_code;
	uint32_t cs;
	uint32_t eflags;
	uint32_t esp; /* If (cs & 0x3) != 0 */
	uint32_t ss;  /* If (cs & 0x3) != 0 */
} interrupt_context;

void Register_Interrupt_Handler(uint32_t irq, interrupt_context handler);
void Initialize_Interrupts(void);
void Enable_Interrupt(uint32_t irq);
void Disable_Interrupt(uint32_t irq);
void Disable_Interrupts(void);
void Enable_Interrupts(void);
void Pause_Interrupts(void);

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

#endif
