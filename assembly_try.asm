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
		fmt_spec_int_arr_out: db "%d", 20, 0
		fmt_spec_real_in: db "%4f", 0
		fmt_spec_real_out: db "%4f", 10, 0
		fmt_spec_real_arr_out: db "%4f", 20, 0
		fmt_spec_string: db "%s", 10, 0
		fmt_spec_bool_in: db "%d", 0
		fmt_spec_bool_out: db "%d", 10, 0
		fmt_spec_bool_arr_out: db "%d", 20, 0
		integer_in: db "Input: Enter an integer value", 10, 0
		integer_in_len: equ $ - integer_in
		real_in: db "Input: Enter an real value", 10, 0
		real_in_len: equ $ - real_in
		boolean_in: db "Input: Enter an boolean value (1: true, 0: false)", 10, 0
		boolean_in_len: equ $ - boolean_in
		print_out: db "Output: ", 0
		print_out_len: equ $ - print_out
		array_in1: db "Input: Enter", 20, 0
		array_in_len1: equ $ - array_in1
		array_in2: db "elements of", 20, 0
		array_in_len2: equ $ - array_in2
		array_in3: db "type for range", 20, 0
		array_in_len3: equ $ - array_in3
		array_in4: db "to", 20, 0
		array_in_len4: equ $ - array_in4
		true: db "true", 10, 0
		true_len: equ $ - true
		false: db "false", 10, 0
		false_len: equ $ - false
		integer: db "integer", 20, 0
		integer_len: equ $ - integer
		real: db "real", 20, 0
		real_len: equ $ - real
		boolean: db "boolean", 20, 0
		boolean_len: equ $ - boolean
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


		; Code for getting user input
		push_regs                    ; save values
		; Display prompt for integer input
		mov      rax , 1
		mov      rdi , 1
		mov      rsi , integer_in
		mov      rdx , integer_in_len
		syscall

		; Code to get integer input
		mov      rdi , fmt_spec_int_in          ; get corresponding format specifier
		mov      rsi , rbp
		sub      rsi , 0
		xor      rax , rax
		rsp_align        ; align rsp to 16 byte offset boundary
		call     scanf
		rsp_realign      ; realign it to original position
		pop_regs        ; restore register values


		; Code for assign statement
		push_regs                    ; save values
		xor      rax , rax           ; flush out the rax register
		mov      rax , 1                    ; immediate to register
		mov      [RBP - 32] , rax            ; register to memory
		pop_regs                    ; restore register values


		; Display prompt for array input
		; Prompt part 1
		mov      rax , 1
		mov      rdi , 1
		mov      rsi , array_in1
		mov      rdx , array_in_len1
		syscall

		; Prompt part 2
		push_regs                    ; save values
		mov      rdi , fmt_spec_int_arr_out                ; get corresponding format specifier
		mov      rsi , 3                               ; move source index
		xor      rax , rax
		rsp_align                                     ; align stack pointer
		call     printf                               ; system call for output
		rsp_realign                                   ; restore previous alignment of stack

		; Prompt part 3
		mov      rax , 1
		mov      rdi , 1
		mov      rsi , array_in2
		mov      rdx , array_in_len2
		syscall

		; Prompt part 4
		mov      rax , 1
		mov      rdi , 1
		mov      rsi , integer
		mov      rdx , integer_len
		syscall

		; Prompt part 5
		mov      rax , 1
		mov      rdi , 1
		mov      rsi , array_in3
		mov      rdx , array_in_len3
		syscall

		; Prompt part 6
		push_regs                    ; save values
		mov      rdi , fmt_spec_int_arr_out                ; get corresponding format specifier
		mov      rsi , 1                               ; move source index
		xor      rax , rax
		rsp_align                                     ; align stack pointer
		call     printf                               ; system call for output
		rsp_realign                                   ; restore previous alignment of stack

		; Prompt part 7
		mov      rax , 1
		mov      rdi , 1
		mov      rsi , array_in4
		mov      rdx , array_in_len4
		syscall

		; Prompt part 8
		push_regs                    ; save values
		mov      rdi , fmt_spec_int_out                ; get corresponding format specifier
		mov      rsi , 3                               ; move source index
		xor      rax , rax
		rsp_align                                     ; align stack pointer
		call     printf                               ; system call for output
		rsp_realign                                   ; restore previous alignment of stack
		pop_regs

		; Code to get integer array input
		mov      rdi , fmt_spec_int_in          ; get corresponding format specifier
		mov      rdx , 3
		mov      rdi , rbp
		sub      rdi , 32
L1:
		sub      rdi , 16
		xor      rax , rax
		rsp_align        ; align rsp to 16 byte offset boundary
		call     scanf
		rsp_realign      ; realign it to original position
		sub		 rdx, 1
		cmp      rdx , 0
		jne      L1
		; Code for printing output
		mov      rax , 1
		mov      rdi , 1
		mov      rsi , print_out
		mov      rdx , print_out_len
		syscall

		push_regs                                         ; save values
		mov      rdi , fmt_spec_int_out                  ; get corresponding format specifier
		mov      rsi , [RBP - 0]                               ; move source index
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
		mov      rax , [RBP - 32]                               ; move source index
		cmp      rax , 0
		jne      L2
		mov      rax , 1
		mov      rdi , 1
		mov      rsi , false
		mov      rdx , false_len
		syscall
		pop_regs                                          ; restore register values
		jmp  L3


L2:
		mov      rax , 1
		mov      rdi , 1
		mov      rsi , true
		mov      rdx , true_len
		syscall
		pop_regs        ; restore register values
L3:


		; Code for printing output
		mov      rax , 1
		mov      rdi , 1
		mov      rsi , print_out
		mov      rdx , print_out_len
		syscall

		push_regs                                         ; save values
		mov      rcx , 3
		mov      rsi , [RBP - 32]                               ; move source index
L4:
		mov      rdi , fmt_spec_int_arr_out                  ; get corresponding format specifier
		sub      rsi , 16
		xor      rax , rax
		rsp_align                                         ; align stack pointer
		call     printf                                   ; system call for output
		rsp_realign                                       ; restore previous alignment of stack
		sub      rcx , 1
		cmp      rcx , 0
		jne      L4
		pop_regs                                          ; restore values
main_end:
		retq