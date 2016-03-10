global idt_loader
idt_loader:
	mov eax, [esp+4]
	lidt [eax]
	ret