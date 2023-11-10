; Autor reseni: Michal Blazek xblaze38
; Pocet cyklu k serazeni puvodniho retezce: 4115
; Pocet cyklu razeni sestupne serazeneho retezce: 4307
; Pocet cyklu razeni vzestupne serazeneho retezce: 3727
; Pocet cyklu razeni retezce s vasim loginem: 867
; Implementovany radici algoritmus: Bubble sort
; ------------------------------------------------

; DATA SEGMENT
                .data
; login:          .asciiz "vitejte-v-inp-2023"    ; puvodni uvitaci retezec
; login:          .asciiz "vvttpnjiiee3220---"  ; sestupne serazeny retezec
; login:          .asciiz "---0223eeiijnpttvv"  ; vzestupne serazeny retezec
login:          .asciiz "xblaze38"            ; SEM DOPLNTE VLASTNI LOGIN
                                                ; A POUZE S TIMTO ODEVZDEJTE

params_sys5:    .space  8   ; misto pro ulozeni adresy pocatku
                            ; retezce pro vypis pomoci syscall 5
                            ; (viz nize - "funkce" print_string)

; CODE SEGMENT
                .text
main:
        ; SEM DOPLNTE VASE RESENI

        ; Spocitme pocet znaku v login:
        xor r3, r3, r3
        xor r1, r1, r1
        for:
                xor r2, r2, r2
                lb r2, login(r1)
                beq r2, r3, end_for
                daddi r1, r1, 1
                b for
        end_for:
        ; r1 = pocet znaku
        daddi r1, r1, -1 ; algoritmus prochazi pouze do r1 - 1

        ; Provedeme radici algoritmus
        xor r3, r3, r3
        forI:                                   ; i = r3
                beq r3, r1, end_forI
                xor r2, r2, r2
                forJ:                           ; j = r2
                        xor r5, r5, r5
                        sub r5, r1, r3
                        beq r2, r5, end_forJ
                        ; IF
                        xor r6, r6, r6
                        lb r6, login(r2)
                        xor r7, r7, r7
                        daddi r7, r2, 1         ; j + 1 = r7
                        lb r5, login(r7)
                        sub r5, r5, r6
                        bgez r5, end_if ; if (r6 > r5)
                                ; SWAP_START
                                ; lb r6, login(r2)
                                lb r5, login(r7)
                                sb r5, login(r2)
                                sb r6, login(r7)
                                ; SWAP_END
                        end_if:
                        ; IF_END
                        daddi r2, r2, 1
                        b forJ
                end_forJ:
                daddi r3, r3, 1
                b forI
        end_forI:

        daddi   r4, r0, login   ; vozrovy vypis: adresa login: do r4
        jal     print_string    ; vypis pomoci print_string - viz nize

        syscall 0   ; halt


print_string:   ; adresa retezce se ocekava v r4
                sw      r4, params_sys5(r0)
                daddi   r14, r0, params_sys5    ; adr pro syscall 5 musi do r14
                syscall 5   ; systemova procedura - vypis retezce na terminal
                jr      r31 ; return - r31 je urcen na return address
