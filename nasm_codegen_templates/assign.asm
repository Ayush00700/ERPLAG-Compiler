; ASM to check if memory to reg are working fine
extern printf
        section     .data
            lhs     dd      0
            rhs     dd      10
            fmt     db      "%d", 10, 0    

        section     .text
        global      main

main:
        xor     eax, eax        ; flush the value in eax
        mov     eax, [rhs]      ; get the value at the location labelled rhs
        mov    [lhs], eax       ; store it in the location labelled lhs

        ; For testing if the above operation worked
        lea     rdi, [rel fmt]
        mov     rsi, [lhs]
        xor     eax, eax
        call    printf