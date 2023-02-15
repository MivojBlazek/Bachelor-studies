%include "rw32-2018.inc"

; Cviceni c.2
; Vyzkousejte prelozit, spustit a zdebuggovat nasledujici kod.
; Kod muzete jakkoliv menit a hrat si s nim.
; U nekterych radku jsou otazky. Odpovezte na ne a potom je projdeme spolecne. 
; Muzete si radit, diskutovat problem a pouzivat materialy z prednasek a cviceni.

section .data
	sMessage db "Hello World!",EOL,0

section .text
CMAIN:
	push ebp
	mov ebp,esp

	mov esi,sMessage	; ukazka volani funkce, ktera napise "Hello World!"
	call WriteString

        mov eax, 0
        
        ; Pouzite debuger na nasledujici cast kodu
        mov al, 255
        mov ah, 255
        
        mov ax, 0 
        mov ah, 255
        
        add eax, 256
        ;Jak se bude menit obsah registru eax, al, ah, ax? Jaky je vztah mezi temito registry?
        
	pop ebp
	ret