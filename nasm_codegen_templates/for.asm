extern printf
        section     .data
        count       dd      10
        iter        dd      0
        fmt         db      "%d",10,0

        section     .text
        global      main
main:
        ; just print the iteration variable value
        lea     rdi , [fmt]
        mov     rsi , [iter]
        xor     rax , rax
        call    printf
        ; update iteration variable
        mov     ecx , [iter]
        inc     ecx
        mov     [iter] , ecx
        ; loop condition
        cmp     ecx , [count]
        jne     main

done:
        mov rax, 60             ; system call for exit
        xor rdi, rdi            ; exit code 0
        syscall                 ; invoke operating system to exit