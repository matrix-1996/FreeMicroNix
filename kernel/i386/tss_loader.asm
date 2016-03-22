global tss_loader
tss_loader:
	mov ax, 0x2B
	ltr ax
	ret