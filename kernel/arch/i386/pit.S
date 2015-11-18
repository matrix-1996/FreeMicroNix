.intel_syntax noprefix


section .bss
system_timer_fractions:  resd 1          ; Fractions of 1 ms since timer initialized
system_timer_ms:         resd 1          ; Number of whole ms since timer initialized
IRQ0_fractions:          resd 1          ; Fractions of 1 ms between IRQs
IRQ0_ms:                 resd 1          ; Number of whole ms between IRQs
IRQ0_frequency:          resd 1          ; Actual frequency of PIT
PIT_reload_value:        resw 1          ; Current PIT reload value

section .text

IRQ0_handler:
	push eax
	push ebx
 
	mov eax, [IRQ0_fractions]
	mov ebx, [IRQ0_ms]                    ; eax.ebx = amount of time between IRQs
	add [system_timer_fractions], eax     ; Update system timer tick fractions
	adc [system_timer_ms], ebx            ; Update system timer tick milli-seconds
 
	mov al, 0x20
	out 0x20, al                          ; Send the EOI to the PIC
 
	pop ebx
	pop eax
	iretd