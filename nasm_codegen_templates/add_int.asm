; ; asm to compute x+y where x and y are command line args
; ; result in eax
; extern atoi
; extern printf
; extern puts

; ; rdi holds argc and rsi holds pointer argv
;         section     .text
;         global main
; main:
;         push    r12             ; save callee registers
;         push    r13

;         cmp     rdi, 3          ; we need 3 arguments including file name
;         jne     err

;         mov     r12, rsi        ; get the pointer argv into r12

;         mov     rdi, [r12+8]    ; get the x
;         call    atoi            ; x converted to integer and now in eax
;         mov     r13d, eax       ; move x into r13d

;         mov     rdi, [r12+16]   ; get the y
;         call    atoi            ; y converted to integer and now in eax

;         add     eax, r13d       ; eax = x + y

;         mov     rdi, answer     ; to print the answer
;         movsxd  rsi, eax
;         xor     rax, rax
;         call    printf
;         jmp     done

; err:
;         mov     edi, badArgC
;         call    puts
;         jmp     done

; done:
;         pop     r13             ; restore the registers
;         pop     r12
;         ret

; answer:
;         db      "%d", 10, 0
; badArgC:
;         db      "Requires exactly two arguments apart from file name", 10, 0


; asm to compute x+y where x and y are command line args
; result in eax
extern atoi
extern printf
extern puts

        section         .data
        a       dd      10
        b       dd      30
        c       dd      0
        fmt     db      "%d",10,0

        section     .text
        global main
main:
        mov     eax , [a]
        add     eax , [b]
        mov     [c] , eax

        lea     rdi , [rel fmt]
        mov     rsi , [c]
        xor     eax , eax
        call    printf
