%define BOOT_SEGMENT 0x07c0

BITS 16

jmp BOOT_SEGMENT:_boot


_boot:

	mov ax, cs
	mov ds, ax
	mov es, ax

	add ax, 200
	mov ss, ax
	mov sp, ax


	call clear_screen
	mov si, BootMsg
	call print
	call get_keypress
	call clear_screen

	jmp $


clear_screen:
	mov ah, 0x0 ; Set Video Mode
	mov al, 0x2 ; Mode: 640x200 pixels, 80x25 chars  
	int 0x10	; BIOS Video Service Interrupt
	ret

print:
	lodsb 			; load the string byte
	or al,al		; test for end of string
	jz print_done 	; if end of string, goto print_done
	mov ah, 0x0E	; Write Teletype to Active Page
	int 0x10		; BIOS Video Service Interrupt
	jmp print 		; Loop

print_done:
	ret

get_keypress:
	mov ah, 0x0	; read keyboard input
	int 0x16	; BIOS Keyboard Service Interrupt
	ret

BootMsg db "FreeMicroNix Bootloader Version 0.01",0xA,0xA,0xD,"Press Any Key To Boot",0x7,0


times 510 - ($-$$) db 0	; Fill rest of boot sector with zeros
dw 0xAA55				; Boot Signature
