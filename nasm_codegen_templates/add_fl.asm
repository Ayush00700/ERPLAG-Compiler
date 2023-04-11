; Floating point addition
extern printf
        section         .data
        a:      dq      10.0
        b:      dq      30.0
        c:      dq      0.0
        fmt:    db      "%4f",10,0

        section         .text
        global main
main:
        movsd           xmm0 , [a] 
        addsd           xmm0 , [b]
        movsd           [c] , xmm0

        mov             rdi , rel fmt
        mov             rsi , [c]
        xor             rax , rax
        call            printf

;         lea     rdi, [rel fmt]
;         mov     esi, dword [c]
;         xor     rax, rax
;         call    printf
;         pop     rbp
;         ret
