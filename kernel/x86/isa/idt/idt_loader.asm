extern idtp
global idt_loader
idt_loader:
	lidt [idtp]
	ret