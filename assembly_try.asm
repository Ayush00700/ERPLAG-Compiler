; Group-20
; ---------------------------------
; 1. Rajan Sahu       2019B4A70572P
; 2. Yash Goyal       2019B4A70638P
; 3. Ayush Agarwal    2019B4A70652P
; 4. Vasu Swaroop     2019B4A70656P
; 5. A Sudarshan      2019B4A70744P
; ---------------------------------
extern printf, scanf, exit
		; Data declaration such as zero and format specifiers for print/scan
		section      .data
		fmt_spec_int_in: db "%d", 0
		fmt_spec_int_out: db "%d", 10, 0
		fmt_spec_real_in: db "%4f", 0
		fmt_spec_real_out: db "%4f", 10, 0
		fmt_spec_string: db "%s", 10, 0
		fmt_spec_bool_in: db "%d", 0
		fmt_spec_bool_out: db "%d", 10, 0
		integer_in: db "Input: Enter an integer value", 10, 0
		integer_in_len: equ $ - integer_in
		real_in: db "Input: Enter an real value", 10, 0
		real_in_len: equ $ - real_in
		boolean_in: db "Input: Enter an boolean value", 10, 0
		boolean_in_len: equ $ - boolean_in
		print_out: db "Output: ", 0
		print_out_len: equ $ - print_out
		array_in: db "Input: Enter %d elements of %s type for range %d to %d", 10, 0
		array_in_len: equ $ - array_in
		true: db "true", 10, 0
		true_len: equ $ - true
		false: db "false", 10, 0
		false_len: equ $ - false
		zero: equ 0


		section      .text
		;macro to store all registers
		%macro  push_regs    0
		push      rax
		push      rbx
		push      rcx
		push      rdx
		push      rsi
		push      rdi
		%endmacro


		;macro to restore all registers
		%macro  pop_regs    0
		pop      rdi
		pop      rsi
		pop      rdx
		pop      rcx
		pop      rbx
		pop      rax
		%endmacro


		;macro to align RSP
		%macro  rsp_align    0
		push      rbx
		push      rdx
		push      rax
		xor       rdx , rdx
		mov       rax , rsp
		mov       rbx , 16
		idiv      rbx
		mov       rbx , rdx
		pop       rax
		pop       rdx
		sub       rsp , rbx
		%endmacro


		;macro to re-align RSP
		%macro  rsp_realign    0
		add      rsp , rbx
		pop      rbx
		%endmacro


		global main
main:
		push_regs                    ; save values
		mov      rbp , rsp           ; set base to current stack top
		pop_regs                     ; save values


		push_regs                    ; save values
		xor      rax , rax           ; flush out the rax register
		mov      rax , 1                    ; immediate to register
		mov      [RBP - 256] , rax            ; register to memory
		pop_regs                    ; restore register values


		push_regs                    ; save values
		xor      rax , rax           ; flush out the rax register
		mov      rax , 5                    ; immediate to register
		mov      [RBP - 160] , rax            ; register to memory
		pop_regs                    ; restore register values


		push_regs                    ; save values
		xor      rax , rax           ; flush out the rax register
		mov      rax , 9                    ; immediate to register
		mov      [RBP - 192] , rax            ; register to memory
		pop_regs                    ; restore register values


		push_regs                    ; save values
		xor      rax , rax           ; flush out the rax register
		mov      rax , 2                    ; immediate to register
		mov      [RBP - 0] , rax            ; register to memory
		pop_regs                    ; restore register values


		push_regs                    ; save values
		xor      rax , rax           ; flush out the rax register
		mov      rax , 3                    ; immediate to register
		mov      [RBP - 64] , rax            ; register to memory
		pop_regs                    ; restore register values


		; Code for arithmetic
		push_regs                    ; save values
		mov     rax , [RBP - 64]
		mov     rbx , [RBP - 192]
		imul     rbx
		mov     [RBP - 288] , rax
		pop_regs        ; restore register values


		; Code for arithmetic
		push_regs                    ; save values
		mov     rax , [RBP - 0]
		add     rax , [RBP - 288]
		mov     [RBP - 320] , rax
		pop_regs        ; restore register values


		; Code for arithmetic
		push_regs                    ; save values
		mov     rax , [RBP - 160]
		sub     rax , [RBP - 192]
		mov     [RBP - 352] , rax
		pop_regs        ; restore register values


		; Code for arithmetic
		push_regs                    ; save values
		mov     rax , [RBP - 352]
		mov     rbx , [RBP - 64]
		imul     rbx
		mov     [RBP - 384] , rax
		pop_regs        ; restore register values


		; Code for arithmetic
		push_regs                    ; save values
		mov     rax , [RBP - 320]
		add     rax , [RBP - 384]
		mov     [RBP - 416] , rax
		pop_regs        ; restore register values


		; Code for arithmetic
		push_regs                    ; save values
		mov     rax , [RBP - 160]
		mov     rbx , 2
		imul     rbx
		mov    [RBP - 448] , rax
		pop_regs        ; restore register values


		; Code for arithmetic
		push_regs                    ; save values
		mov     rax , [RBP - 416]
		add     rax , [RBP - 448]
		mov     [RBP - 480] , rax
		pop_regs        ; restore register values


		; Code for arithmetic
		push_regs                    ; save values
		mov     rax , [RBP - 192]
		mov     rbx , [RBP - 0]
		imul     rbx
		mov     [RBP - 512] , rax
		pop_regs        ; restore register values


		; Code for arithmetic
		push_regs                    ; save values
		mov     rax , [RBP - 480]
		sub     rax , [RBP - 512]
		mov     [RBP - 544] , rax
		pop_regs        ; restore register values


		push_regs                    ; save values
		xor      rax , rax           ; flush out the rax register
		mov      rax , [RBP - 544]                    ; memory to register
		mov      [RBP - 128] , rax            ; register to memory
		pop_regs                    ; restore register values


		; Code for relational
		push_regs                    ; save values
		mov     rax , [RBP - 128]
		mov     rbx , 10
		cmp     rax , rbx
		jg     L1
		mov     qword [RBP - 576], 0
		jmp  L2
L1:
		mov     qword [RBP - 576], 1
L2:
		pop_regs        ; restore register values


		; Code for relational
		push_regs                    ; save values
		mov     rax , [RBP - 160]
		mov     rbx , [RBP - 192]
		cmp     rax , rbx
		jle     L3
		mov     qword [RBP - 592], 0
		jmp  L4
L3:
		mov     qword [RBP - 592], 1
L4:
		pop_regs        ; restore register values


		; Code for logical op
		push_regs                    ; save values
		mov     rax , [RBP - 576]
		or     rax , [RBP - 592]
		mov     [RBP - 608] , rax
		pop_regs        ; restore register values


		; Code for relational
		push_regs                    ; save values
		mov     rax , [RBP - 0]
		mov     rbx , [RBP - 64]
		cmp     rax , rbx
		jl     L5
		mov     qword [RBP - 624], 0
		jmp  L6
L5:
		mov     qword [RBP - 624], 1
L6:
		pop_regs        ; restore register values


		; Code for logical op
		push_regs                    ; save values
		mov     rax , [RBP - 608]
		and     rax , [RBP - 624]
		mov     [RBP - 640] , rax
		pop_regs        ; restore register values


		; Code for logical op
		push_regs                    ; save values
		mov     rax , [RBP - 640]
		and     rax , [RBP - 256]
		mov     [RBP - 656] , rax
		pop_regs        ; restore register values


		push_regs                    ; save values
		xor      rax , rax           ; flush out the rax register
		mov      rax , [RBP - 656]                    ; memory to register
		mov      [RBP - 272] , rax            ; register to memory
		pop_regs                    ; restore register values


		; Code for printing output
		mov      rax , 1
		mov      rdi , 1
		mov      rsi , print_out
		mov      rdx , print_out_len
		syscall

		push_regs                                         ; save values
		mov      rdi , fmt_spec_int_out                  ; get corresponding format specifier
		mov      rsi , [RBP - 128]                               ; move source index
		xor      rax , rax
		rsp_align                                         ; align stack pointer
		call     printf                                   ; system call for output
		rsp_realign                                       ; restore previous alignment of stack
		pop_regs                                          ; restore values
		; Code for printing output
		mov      rax , 1
		mov      rdi , 1
		mov      rsi , print_out
		mov      rdx , print_out_len
		syscall

		push_regs                                         ; save values
		mov      rdi , fmt_spec_bool_out                  ; get corresponding format specifier
		mov      rax , [RBP - 272]                               ; move source index
		cmp      rax , 0
		jne      L7
		mov      rax , 1
		mov      rdi , 1
		mov      rsi , false
		mov      rdx , false_len
		syscall
		pop_regs                                          ; restore register values
		jmp  L8


L7:
		mov      rax , 1
		mov      rdi , 1
		mov      rsi , true
		mov      rdx , true_len
		syscall
		pop_regs        ; restore register values
L8:


main_end:
		retq