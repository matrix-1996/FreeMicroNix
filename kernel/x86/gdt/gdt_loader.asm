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

extern gp

global gdt_loader
gdt_loader:
    lgdt [gp]        ; Load the new GDT pointer
    mov ax, 0x10      ; 0x10 is the offset in the GDT to our data segment
    mov ds, ax        ; Load all data segment selectors
    mov es, ax
    mov ss, ax
    ;mov ax, 0x33
    ;mov fs, ax
    ;mov ax, 0x3B
    ;mov gs, ax
    jmp 0x08:.flush   ; 0x08 is the offset to our code segment: Far jump!
.flush:
    ret
