;Copyright (C) <2016>  William Taylor Jones <freemicronix@gmail.com>
;
;This program is free software; you can redistribute it and/or modify
;it under the terms of the GNU General Public License as published by
;the Free Software Foundation; either version 2 of the License, or
;(at your option) any later version.
;
;This program is distributed in the hope that it will be useful,
;but WITHOUT ANY WARRANTY; without even the implied warranty of
;MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;GNU General Public License for more details.
;
;You should have received a copy of the GNU General Public License along
;with this program; if not, write to the Free Software Foundation, Inc.,
;51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.



[BITS 32]

GLOBAL _pageDirectory

STACKSIZE equ 0x8000

K_OFFSET_ADDR equ 0xC0000000
K_PAGE_NUMBER equ (K_OFFSET_ADDR >> 22)


; Setup the multiboot header

MBALIGN     equ  1<<0                  
MEMINFO     equ  1<<1
FLAGS       equ  MBALIGN | MEMINFO
MAGIC       equ  0x1BADB002
CHECKSUM    equ -(MAGIC + FLAGS)


section .kinit
align 0x1000
_pageDirectory:
    ; This page directory entry identity-maps the first 4MB of the 32-bit physical address space.
    ; All bits are clear except the following:
    ; bit 7: PS The kernel page is 4MB.
    ; bit 1: RW The kernel page is read/write.
    ; bit 0: P  The kernel page is present.
    ; This entry must be here -- otherwise the kernel will crash immediately after paging is
    ; enabled because it can't fetch the next instruction! It's ok to unmap this page later.
    dd 0x00000083
    times (K_PAGE_NUMBER - 1) dd 0	; Pages before kernel space
    ; This page directery entry defines a 4MB page containing the kernel
    dd 0x00000083
    times (1024 - KERNEL_PAGE_NUMBER - 1) dd 0  ; Pages after the kernel image.


section .kernel_early_bootstrap
align 4
MultiBootHeader:
	dd MAGIC
	dd FLAGS
	dd CHECKSUM

GLOBAL loader
loader:
	cli 							; disable interrupts

	mov ecx, (_pageDirectory - K_OFFSET_ADDR)
	mov cr3, ecx					; load Page Directory Base Address

    mov ecx, cr4
    or ecx, 0x00000010				; Set PSE bit in CR4 to enable 4MB pages.
    mov cr4, ecx

    mov ecx, cr0
    or ecx, 0x80000000				; Set PG bit in CR0 to enable paging.
    mov cr0, ecx

    lea ecx, [StartHigherHalf]
    jmp ecx

section .kinit

StartHigherHalf:

	mov DWORD [_pageDirectory], 0
	invlpg [0]

	; Paging is enabled at this point

	mov esp, _InitialKernelStack + STACKSIZE
		
	push ebx	; Push MultiBoot info structure - WARNING - physical addr may not be in first 4MB
	push eax	; Push MultiBoot magic number

	extern _init
	call _init						; call global constructors

	extern kmain
	call kmain						; transfer control to main kernel

	cli
.hang:
	hlt
	jmp .hang

section .bss
align 32

global _InitialKernelStack
	resb STACKSIZE