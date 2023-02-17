%include "rw32-2022-mingw.inc"

section .data
	sMessage db "Hello World!",EOL,0

section .text
CMAIN:
	push ebp
	mov ebp,esp
	
	mov esi,sMessage	; ukazka volani funkce, ktera napise "Hello World!"
	call WriteString

	; zde muzete psat vas kod

	pop ebp
	ret
