#ifndef _I386_GDT_H
#define _I386_GDT_H

#include <stdint.h>

typedef struct gdtr {
    uint32_t limite;
    uint32_t base;
} __attribute__ ((packed)) gdtr;

typedef struct gdt_entry {
    uint16_t lim0_15;
    uint16_t base0_15;
    uint8_t base16_23;
    uint8_t access;
    uint8_t lim16_19:4;
    uint8_t other:4;
    uint8_t base24_31;
} __attribute__ ((packed)) gdt_entry;

typedef struct tss {
	uint16_t previous_task, __previous_task_unused;
	uint32_t esp0;
	uint16_t ss0, __ss0_unused;
	uint32_t esp1;
	uint16_t ss1, __ss1_unused;
	uint32_t esp2;
	uint16_t ss2, __ss2_unused;
	uint32_t cr3;
	uint32_t eip, eflags, eax, ecx, edx, ebx, esp, ebp, esi, edi;
	uint16_t es, __es_unused;
	uint16_t cs, __cs_unused;
	uint16_t ss, __ss_unused;
	uint16_t ds, __ds_unused;
	uint16_t fs, __fs_unused;
	uint16_t gs, __gs_unused;
	uint16_t ldt_selector, __ldt_sel_unused;
	uint16_t debug_flag, io_map;
} __attribute__ ((packed)) tss;


void Initialize_GDT(void);

#endif