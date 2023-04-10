section .text
	default rel
	extern printf
	extern scanf
	global main


main:
    finit;
    lea rsi, [rbp - 4]
	mov rdi, int_in
	xor rax, rax
	call scanf

    mov ax, word [rbp - 4]
	movsx rsi, ax
	mov rdi, int_out
	xor rax, rax
	call printf
	mov rsi, 0
	mov rdi, newline
	xor rax, rax
	call printf




.exit:
	add rsp, 16
	pop rbp
	ret

section .data
	int_out:  db  "%hd", 09, 0
	int_in:  db  "%hd", 0
	real_out:  db  "%f", 09, 0
	real_in:  db  "%f", 0
	newline:  db  10, 0
	real_val:  db  0,0,0,0
