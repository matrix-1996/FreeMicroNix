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

MBALIGN     equ  1<<0                  
MEMINFO     equ  1<<1
FLAGS       equ  MBALIGN | MEMINFO
MAGIC       equ  0x1BADB002
CHECKSUM    equ -(MAGIC + FLAGS)

section .multiboot
align 4
	dd MAGIC
	dd FLAGS
	dd CHECKSUM

section .bootstrap_stack, nobits
align 4
stack_bottom:
resb 16384
stack_top:

section .text
global loader
loader:
	mov esp, stack_top				; point esp to the start of the kernel's stack
	push 0x0

	extern kernel_physical_start
	push kernel_physical_start

	extern kernel_physical_end
	push kernel_physical_end
	
	push ebx
	push eax

	extern _init
	call _init						; call global constructors


	extern kmain
	call kmain						; transfer control to main kernel

	cli
.hang:
	hlt
	jmp .hang
