/*
Copyright (C) <2016>  William Taylor Jones <freemicronix@gmail.com>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/


K_PHYS_ADDR = 0x100000; /* The kernel begins at 1MB in the physical address space */

K_OFFSET_ADDR = 0xC0000000; /* 3GB offset */



OUTPUT_FORMAT(elf32-i386) /* Sets the type of ELF */
ENTRY(loader)

SECTIONS
{


    . = K_PHYS_ADDR; /* Physical addresses are used for the
                        code that is before paging starts */

    .kernel_early_bootstrap :
    {
        *(.kernel_early_bootstrap)
    }
    

    /* The rest of the kernel uses virtual memory addresses */
    
    . += K_OFFSET_ADDR; 

    .kinit ALIGN(0x1000) : AT(ADDR(.kinit) - K_OFFSET_ADDR)
    {
        start_kinit_section = .;

        *(.kinit)

        end_kinit_section = .;


        /* Make this part 4K aligned in the future so that it's
           page can be freed for usage

           . = ALIGN(0x4000)
        */
    }


    .text ALIGN(0x1000) : AT(ADDR(.text) - K_OFFSET_ADDR)
    {
        *(.text)
    }


    .data ALIGN(0x1000) : AT(ADDR(.data) - K_OFFSET_ADDR)
    {
        *(.data)
    }

    .rodata ALIGN(0x1000) : AT(ADDR(.rodata) - K_OFFSET_ADDR)
    {
        *(.rodata*)
    }

    .bss ALIGN(0x1000) : AT(ADDR(.bss) - K_OFFSET_ADDR)
    {
        _sbss = .;
        *(COMMON)
        *(.bss)
        _ebss = .;
    }

    .kernel_end : AT(ADDR(.kernel_end) - K_OFFSET_ADDR)
    {
        kernel_end = .;
    }

}
