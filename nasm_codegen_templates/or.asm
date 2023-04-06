extern printf
        section     .data
        a       dd      10
        b       dd      5
        c       dd      0
        fmt     db      "%d",10,0

        section     .text
        global      main
main:
        mov     eax , [a]
        or      eax , [b]
        mov     [c] , eax

        lea     rdi , [fmt]
        mov     rsi , [c]
        xor     eax , eax
        call printf