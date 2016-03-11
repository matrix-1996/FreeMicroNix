extern idt
global idt_loader
idt_loader:
	lidt [idt]
	ret