#include <stdint.h>
#include <string.h>
#include <x86.h>
#include <io.h>
#include <architecture.h>

idtdesc kidt[IDTSIZE];
int_desc intt[IDTSIZE];
gdtdesc kgdt[GDTSIZE];
tss default_tss;
gdtr kgdtr;
idtr kidtr;

void Initialize_GDT_Descriptor(uint32_t base, uint32_t limite, uint8_t access, uint8_t other, gdtdesc *desc)
{
    desc->lim0_15 = (limite & 0xffff);
    desc->base0_15 = (base & 0xffff);
    desc->base16_23 = (base & 0xff0000) >> 16;
    desc->access = access;
    desc->lim16_19 = (limite & 0xf0000) >> 16;
    desc->other = (other & 0xf);
    desc->base24_31 = (base & 0xff000000) >> 24;
    return;
}

void Initialize_GDT(void)
{

    default_tss.debug_flag = 0x00;
    default_tss.io_map = 0x00;
    default_tss.esp0 = 0x1FFF0;
    default_tss.ss0 = 0x18;

    // initialize gdt segments
    Initialize_GDT_Descriptor(0x0, 0x0, 0x0, 0x0, &kgdt[0]);
    Initialize_GDT_Descriptor(0x0, 0xFFFFF, 0x9B, 0x0D, &kgdt[1]);  // code
    Initialize_GDT_Descriptor(0x0, 0xFFFFF, 0x93, 0x0D, &kgdt[2]);  // data
    Initialize_GDT_Descriptor(0x0, 0x0, 0x97, 0x0D, &kgdt[3]);      // stack

    Initialize_GDT_Descriptor(0x0, 0xFFFFF, 0xFF, 0x0D, &kgdt[4]);  // ucode 
    Initialize_GDT_Descriptor(0x0, 0xFFFFF, 0xF3, 0x0D, &kgdt[5]);  // udata 
    Initialize_GDT_Descriptor(0x0, 0x0, 0xF7, 0x0D, &kgdt[6]);      // ustack

    Initialize_GDT_Descriptor((uint32_t) & default_tss, 0x67, 0xE9, 0x00, &kgdt[7]); // tss descriptor

    // initialize the gdtr structure
    kgdtr.limite = GDTSIZE * 8;
    kgdtr.base = GDTBASE;

    //copy the gdtr to its memory area
    memcpy((char *) kgdtr.base, (char *) kgdt, kgdtr.limite);

    //load the gdtr register
    asm("lgdtl (kgdtr)");

    // initialize the segment registers
    asm("movw $0x10, %ax \n \
         movw %ax, %ds   \n \
         movw %ax, %es   \n \
         movw %ax, %fs   \n \
         movw %ax, %gs   \n \
         movw %ax, %ss   \n \
         ljmp $0x08, $next   \n \
         next:       \n");
            
}
