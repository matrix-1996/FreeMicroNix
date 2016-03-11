#ifndef _I386_GDT_H
#define _I386_GDT_H


struct gdt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));
typedef struct gdt_ptr gdt_ptr_t;

typedef struct gdt_entry {
    uint16_t limit0_15;
    uint16_t base0_15;
    uint8_t base16_23;
    uint8_t access;
    uint8_t granularity;
    uint8_t base24_31;
} __attribute__ ((packed)) gdt_entry_t;

typedef struct tss_entry {
    uint32_t prev_tss;
    uint32_t esp0;
    uint32_t ss0;
    uint32_t esp1;
    uint32_t ss1;
    uint32_t esp2;
    uint32_t ss2;
    uint32_t cr3;
    uint32_t eip;
    uint32_t eflags;
    uint32_t eax;
    uint32_t ecx;
    uint32_t edx;
    uint32_t ebx;
    uint32_t esp;
    uint32_t ebp;
    uint32_t esi;
    uint32_t edi;
    uint32_t es;
    uint32_t cs;
    uint32_t ss;
    uint32_t ds;
    uint32_t fs;
    uint32_t gs;
    uint32_t ldt;
    uint16_t trap;
    uint16_t iomap_base;
} __attribute__((packed)) tss_entry_t;


extern void gdt_loader(void);
extern void tss_loader(void);
void Initialize_GDT(void);

#endif