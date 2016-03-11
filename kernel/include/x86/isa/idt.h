#ifndef _X86_ISA_IDT_H
#define _X86_ISA_IDT_H

#include <stdint.h>

typedef struct idt_entry {
    uint16_t base_lo;
    uint16_t sel;
    uint8_t always0;
    uint8_t flags;
    uint16_t base_hi;
} __attribute__((packed)) idt_entry_t;


typedef struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__ ((packed)) idt_ptr_t;


void Create_IDT_Entry(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);
void Install_IDT(void);

#endif
