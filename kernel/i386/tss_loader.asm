global tss_loader
tss_loader:
	mov ax, 0x28
	ltr ax
	ret