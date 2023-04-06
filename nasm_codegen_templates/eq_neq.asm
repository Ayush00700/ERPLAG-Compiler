extern printf
        section     .data
        a       dd      10
        b       dd      10
        c       db      "a is equal to b",10,0
        lenc    equ     $ - c
        d       db      "a is not equal b",10,0
        lend    equ     $ - d

        section     .text
        global      main
main:
        mov     eax , [a]
        cmp     eax , [b]
        je      L1

        mov     rax , 1         ; system call for print
        mov     rdi , 1         ; fd = 1 for stdout
        mov     rsi , d         ; start of string
        mov     rdx , lend      ; number of bytes
        syscall
        mov rax, 60             ; system call for exit
        xor rdi, rdi            ; exit code 0
        syscall                 ; invoke operating system to exit

L1:
        mov     rax , 1         ; system call for print
        mov     rdi , 1         ; fd = 1 for stdout
        mov     rsi , c         ; start of string
        mov     rdx , lend      ; number of bytes
        syscall
        mov rax, 60             ; system call for exit
        xor rdi, rdi            ; exit code 0
        syscall                 ; invoke operating system to exit