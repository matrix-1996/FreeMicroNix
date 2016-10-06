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

BITS 16

jmp short _begin

bootmsg db "FreeMicroBoot",0xD,0xA,"Version 0.01",0xD,0xA,0


_begin:
	mov ax, 07C0h		; move 0x7c00 into ax
	mov ds, ax			; set data segment to where we're loaded
	
	call clear_screen
	
	mov si, bootmsg
	call print_string

	jmp $

clear_screen:
	mov ah, 0		; Set video mode BIOS call
	mov al, 3		; 80x25 char color video mode
	int 0x10		; BIOS video interrupt
	ret

print_string:
	
	lodsb 			; Load String
	or al, al		; check if 0
	jz done			
	mov ah, 0xE
	int 0x10		; BIOS video interrupt
	jmp print_string

done:
	ret

	times 510-($-$$) db 0	; Pad remainder of boot sector with 0s
	dw 0xAA55				; The standard PC boot signature

