; Called from kmain.c in order to load the global descriptor table
;
;
;
global setGdt

section .data
gdtr dw 0 ; For limit storage
	 dd 0 ; For base storage

section .text
setGdt:
	mov eax, [ebx + 4]
	mov [gdtr + 2], eax
	mov ax, [esp + 8]
	mov [gdtr], ax
	lgdt [gdtr]

reloadSegments:
	jmp 0x08:reload_CS 				; set code segment to kernel code segment (0x08)



reload_CS:
	mov cx, 0x10					; set segment registers to kernel data segment (0x10)
	mov ds, cx
	mov es, cx
	mov fs, cx
	mov gs, ax
	mov ss, cx
	ret

setTss:
	extern tss						; set task state segment
	mov ecx, tss
	mov cx, [0x2b]
	ltr cx							; load the task register