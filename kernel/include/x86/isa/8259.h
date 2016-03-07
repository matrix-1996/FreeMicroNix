#ifndef _X86_ISA_8259_H
#define _X86_ISA_8259_H

void PIC_EOI_Master(void);
void PIC_EOI_Slave(void);
void Initialize_IDT(void);
void Initialize_8259_PIC(void);

#endif