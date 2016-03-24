#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <x86/gdt.h>
#include <x86/io.h>
#include <x86/video/vga.h>

#define GDT_SIZE 6

gdt_entry_t gdt[GDT_SIZE];
gdt_ptr_t gp;
tss_entry_t tss_entry;


 void tss_setup(void)
 {
    tss_entry.prev_tss = 0;
    tss_entry.esp0 = 0;    
    tss_entry.ss0 = 0x10;
    tss_entry.esp1 = 0;
    tss_entry.ss1 = 0;
    tss_entry.esp2 = 0;
    tss_entry.ss2 = 0;
    tss_entry.cr3 = 0;
    tss_entry.eip = 0;
    tss_entry.eflags = 0;
    tss_entry.eax = 0;
    tss_entry.ecx = 0;
    tss_entry.edx = 0;
    tss_entry.ebx = 3;
    tss_entry.esp = 0;
    tss_entry.ebp = 0;
    tss_entry.esi = 0;
    tss_entry.edi = 0;
    tss_entry.es = 0x13;
    tss_entry.cs = 0x0B;
    tss_entry.ss = 0;
    tss_entry.ds = 0x13;
    tss_entry.fs = 0x13;
    tss_entry.gs = 0x13;
    tss_entry.ldt = 0;
    tss_entry.trap = 0;
    tss_entry.iomap_base = 0;
 }

 void tss_set_kernel_stack(uint32_t stack)
 {
    tss_entry.esp0 = stack;
 }

void Add_GDT_Descriptor(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity)
{
    gdt[num].base0_15 = (base & 0xFFFF);
    gdt[num].base16_23 = (base >> 16) & 0xFF;
    gdt[num].base24_31 = (base >> 24 ) & 0xFF;

    gdt[num].limit0_15 = (limit & 0xFFFF);

    gdt[num].access = access;

    gdt[num].granularity = ((limit >> 16) & 0x0F);
    gdt[num].granularity |= (granularity & 0xF0);
}

void Initialize_GDT(void)
{
    gp.base = (uint32_t)&gdt;
    gp.limit = (sizeof(gdt_entry_t)*GDT_SIZE) - 1;

    // initialize gdt segments
    Add_GDT_Descriptor(0, 0x0, 0x0, 0x0, 0x0);                           // Null Segment
    Add_GDT_Descriptor(1, 0x0, 0xFFFFFFFF, 0x9A, 0xCF);                  // Kernel Code Segment
    Add_GDT_Descriptor(2, 0x0, 0xFFFFFFFF, 0x92, 0xCF);                  // Kernel Data Segment
    Add_GDT_Descriptor(3, 0x0, 0xFFFFFFFF, 0xFA, 0xCF);                  // User Code Segment 
    Add_GDT_Descriptor(4, 0x0, 0xFFFFFFFF, 0xF2, 0xCF);                  // User Data Segment
    Add_GDT_Descriptor(5, 0x0, sizeof(tss_entry) - 1, 0xE9, 0x0);        // Task Switch Segment
    //Add_GDT_Descriptor(6, 0x0, 0xFFFFFFFF, 0xF2, 0xCF);                  // F Segment
    //Add_GDT_Descriptor(7, 0x0, 0xFFFFFFFF, 0xF2, 0xCF);                  // G Segment

    tss_setup();

    gdt_loader();
    kprintf("Global Descriptor Table Initialized\n");
    tss_loader();
    kprintf("Task State Segment Initialized\n");
}
