global page_directory_loader

page_directory_loader:
	push ebp
	mov ebp, esp
	mov eax, [esp + 8]
	mov cr3, eax
	mov esp, ebp
	pop ebp
	ret