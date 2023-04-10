; ; asm to compute x+y where x and y are command line args
; ; result in rax
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
;         call    atoi            ; x converted to integer and now in rax
;         mov     r13d, rax       ; move x into r13d

;         mov     rdi, [r12+16]   ; get the y
;         call    atoi            ; y converted to integer and now in rax

;         add     rax, r13d       ; rax = x + y

;         mov     rdi, answer     ; to print the answer
;         movsxd  rsi, rax
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
; result in rax
extern atoi
extern printf
extern puts
extern scanf

section .bss
	INPT: resq 1

        section         .data
        a       dq      10
        b       dq      30
        c       dq      0
        d       dq      0
        fmt     db      "%d",0
        fmt_spec_int_in: db "%d", 0
        NEWLINE: db 0xa, 0xd
	LENGTH: equ $-NEWLINE

        section     .text
        %macro newline 0
        ;Print new line after the output 
	mov rax, 0x4
	mov rbx, 0x1
	mov rcx, NEWLINE
	mov rdx, LENGTH
	int 0x80
        %endmacro

        global main
main:
        
        mov rax, 0x3 ;read
	mov rbx, 0x1
	mov rcx, a
	mov rdx, 2
	int 80h

        mov rax, 0x3 ;read
	mov rbx, 0x1
	mov rcx, b
	mov rdx, 2
	int 80h

        ; mov     rax , [a] ; add
        ; sub     rax , '0'
        ; add     rax , [b]
        ; mov     [c] , rax

        mov     rax , [a] ; add
        sub     rax , '0'
        mov     rbx , [b]
        sub     rbx , '0'
        imul    rbx
        add     rax , '0'
        mov     [c] , rax

	; mov rax, 0x4 ; write
	; mov rbx, 0x1
	; mov rcx, c
	; mov rdx, 1
	; int 80h
        ; newline

        ; push_regs
                ; rsp_realign ;realign it to original position\n", offsetResult);

                ; pop_regs        ; restore register values\n\n\n");   
                
        lea     rdi , [fmt]
        mov     rsi , [c]
        xor     rax , rax
        call    printf