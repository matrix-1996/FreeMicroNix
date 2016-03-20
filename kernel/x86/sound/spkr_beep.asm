global spkr_beep

spkr_beep:
	mov bx, 256								; on/off cycle time duration
	mov cx, bx								; temporary register
	in al, 0x61								; read port contents
	and al, 0x0fe							; timer output to speaker off
	mov dx, 80								; loop count

spkr_cycle:
	or al, 2
	out 0x61, al							; turn on speaker bit

spkr_on_delay:
	loop spkr_on_delay 						; delay
	and al, 0x0fd
	out 0x61, al							; turn off speaker bit
	mov cx, bx								; reset duration

spkr_off_delay:
	loop spkr_off_delay 					; delay
	dec dx
	jnz spkr_cycle							; loop 80 times
	ret
