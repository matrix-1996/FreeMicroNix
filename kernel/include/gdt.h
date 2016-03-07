#ifndef _GDT_H
#define _GDT_H

#include <stdint.h>

#define GDTSIZE	0xFF
#define GDTBASE 0x00000800
#define IDTBASE 0x00000000
#define IDTSIZE 0xFF


#define INTGATE  0x8E00
#define TRAPGATE 0xEF00


#define SEG_DATA_RD        0x00 // Read-Only
#define SEG_DATA_RDA       0x01 // Read-Only, accessed
#define SEG_DATA_RDWR      0x02 // Read/Write
#define SEG_DATA_RDWRA     0x03 // Read/Write, accessed
#define SEG_DATA_RDEXPD    0x04 // Read-Only, expand-down
#define SEG_DATA_RDEXPDA   0x05 // Read-Only, expand-down, accessed
#define SEG_DATA_RDWREXPD  0x06 // Read/Write, expand-down
#define SEG_DATA_RDWREXPDA 0x07 // Read/Write, expand-down, accessed
#define SEG_CODE_EX        0x08 // Execute-Only
#define SEG_CODE_EXA       0x09 // Execute-Only, accessed
#define SEG_CODE_EXRD      0x0A // Execute/Read
#define SEG_CODE_EXRDA     0x0B // Execute/Read, accessed
#define SEG_CODE_EXC       0x0C // Execute-Only, conforming
#define SEG_CODE_EXCA      0x0D // Execute-Only, conforming, accessed
#define SEG_CODE_EXRDC     0x0E // Execute/Read, conforming
#define SEG_CODE_EXRDCA    0x0F // Execute/Read, conforming, accessed


#define MASTER_PIC         0x20
#define SLAVE_PIC          0xA0
#define MASTER_PIC_COMMAND 0x20 
#define MASTER_PIC_DATA    0x21 
#define SLAVE_PIC_COMMAND  0xA0
#define SLAVE_PIC_DATA     0xA1

#define ICW1_ICW4	    0x01		/* ICW4 (not) needed */
#define ICW1_SINGLE 	0x02		/* Single (cascade) mode */
#define ICW1_INTERVAL4	0x04		/* Call address interval 4 (8) */
#define ICW1_LEVEL	    0x08		/* Level triggered (edge) mode */
#define ICW1_INIT	    0x10		/* Initialization - required! */
 
#define ICW4_8086	    0x01		/* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO	    0x02		/* Auto (normal) EOI */
#define ICW4_BUF_SLAVE	0x08		/* Buffered mode/slave */
#define ICW4_BUF_MASTER	0x0C		/* Buffered mode/master */
#define ICW4_SFNM	    0x10		/* Special fully nested (not) */
 

typedef struct gdtr {
    uint32_t limite;
    uint32_t base;
} __attribute__ ((packed)) gdtr;

typedef struct gdtdesc {
    uint16_t lim0_15;
    uint16_t base0_15;
    uint8_t base16_23;
    uint8_t access;
    uint8_t lim16_19:4;
    uint8_t other:4;
    uint8_t base24_31;
} __attribute__ ((packed)) gdtdesc;

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

typedef struct idtr {
    uint16_t limite;
    uint32_t base;
} __attribute__ ((packed)) idtr;

typedef struct idtdesc {
    uint16_t offset0_15; // offset bits 0-15
    uint16_t selector;     // code segment selector in GDT
    uint16_t type;       // type and attributes
    uint16_t offset16_31; // offset bits 16-31
} __attribute__ ((packed)) idtdesc;

typedef struct
{
	uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
	uint32_t ds, es, fs, gs;
	uint32_t which_int, err_code;
	uint32_t eip, cs, eflags, user_esp, user_ss;
} __attribute__((packed)) regs_t;

typedef void (*int_desc)(void);

void init_gdt_desc(uint32_t base, uint32_t limite, uint8_t acces, uint8_t other, gdtdesc *desc);
void init_gdt(void);
void init_idt_desc(uint16_t selector, uint32_t offset, uint16_t type, idtdesc *desc);
void init_idt(void);
void init_pic(void);

#endif