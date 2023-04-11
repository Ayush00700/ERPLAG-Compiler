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
		mov      [RBP - 224] , rax            ; register to memory
		pop_regs                    ; restore register values


		push_regs                    ; save values
		xor      rax , rax           ; flush out the rax register
		mov      rax , 5                    ; immediate to register
		mov      [RBP - 128] , rax            ; register to memory
		pop_regs                    ; restore register values


		push_regs                    ; save values
		xor      rax , rax           ; flush out the rax register
		mov      rax , 9                    ; immediate to register
		mov      [RBP - 160] , rax            ; register to memory
		pop_regs                    ; restore register values


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
				mov RDX, RBP
                ;sub RDX, 0     ; make RDX to point at location of variable on the stack
                ;So, we are firstly clearing upper 32 bits of memory so as to access data properly later
                mov RSI, RBP
                sub RSI, 0 
                mov RAX, 0 
                rsp_align ;align RSP to 16 byte boundary for scanf call
                call scanf 
                rsp_realign ;realign it to original position
		pop_regs        ; restore register values


		; Code for getting user input
		push_regs                    ; save values
		; Display prompt for integer input
		mov      rax , 1
		mov      rdi , 1
		mov      rsi , integer_in
		mov      rdx , integer_in_len
		syscall

		; Code to get integer input
		mov      rdi , fmt_spec_int_in          ; get corresponding format specifi