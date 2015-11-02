section .text
 
global _start
_start:
	mov ebp, 0
	push ebp
	push ebp
	mov ebp, esp
 
	push esi
	push edi
 
	call initialize_standard_library
 
	call _init
 
	pop edi
	pop esi
 
	call main
 
	mov edi, eax
	call exit