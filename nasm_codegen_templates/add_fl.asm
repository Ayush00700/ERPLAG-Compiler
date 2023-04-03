; Floating point addition
global sum

        section     .text
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