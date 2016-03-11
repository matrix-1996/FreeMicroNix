#ifndef _X86_ISA_8259_H
#define _X86_ISA_8259_H

#include <stdbool.h>
#include <stdint.h>

void IRQ_Remap(void);
void Initialize_8259_PIC(uint32_t PIC_MASTER_BASE ,uint32_t PIC_SLAVE_BASE);
void Enable_8259_PIC(uint8_t irq);
void Disable_8259_PIC(uint8_t irq);
void Acknowledge_8259_PIC( uint8_t irq );
uint16_t PIC_Read_IRR(void);
uint16_t PIC_Read_ISR(void);
void PIC_EOI_Master(void);
void PIC_EOI_Slave(void);

#endif