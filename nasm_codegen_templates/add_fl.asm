; Floating point addition

        section     .text
        global sum
sum:
        xorpd       xmm0, xmm0              ; initialize sum as 0
        cmp         rsi, 0                  ; special case for length = 0
        je          done                    ; Addition done if lenght = 0

next:
        addsd       xmm0, [rdi]             ; add in current array element
        add         rdi, 8                  ; move to next element
        dec         rsi                     ; count down
        jnz         next                    ; addition not yet complete

done:
        ret                                 ; return the value that was in xmm0

; Floating point addition
; extern printf
;         section         .data
;         align   16
;         a       dd      30.0
;         b       dd      10.0
;         c       dd      0.0
;         fmt     db      "%.2f",10,0

;         section     .text
;         global main
; main:

;         push            rbp
;         movaps          xmm0 , [a]
;         movaps          xmm1 , [b]
;         paddsw           xmm0 , xmm1
;         movaps          [c] , xmm0

;         lea     rdi, [rel fmt]
;         mov     esi, dword [c]
;         xor     rax, rax
;         call    printf
;         pop     rbp
;         ret
