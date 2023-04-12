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
<<<<<<< HEAD
		fmt_spec_int_in: db "%d", 0
		fmt_spec_int_out: db "%d", 10, 0
=======
		fmt_spec_int_in: db "%hd", 0
		fmt_spec_int_out: db "%hd", 10, 0
>>>>>>> codegen_exp
		fmt_spec_real_in: db "%4f", 0
		fmt_spec_real_out: db "%4f", 10, 0
		fmt_spec_string: db "%s", 10, 0
		fmt_spec_bool_in: db "%d", 0
		fmt_spec_bool_out: db "%d", 10, 0
<<<<<<< HEAD
=======
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
>>>>>>> codegen_exp
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
<<<<<<< HEAD
		pop      
=======
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
		ENTER 400,0
		push_regs                    ; save values
		mov      rbp , rsp           ; set base to current stack top
		pop_regs                     ; save values


		push_regs                    ; save values
		xor      rax , rax           ; flush out the rax register
		mov      rax , 5                    ; immediate to register
		mov      [RBP - 96] , rax            ; register to memory
		pop_regs                    ; restore register values


		push_regs                    ; save values
		xor      rax , rax           ; flush out the rax register
		mov      rax , 9                    ; immediate to register
		mov      [RBP - 128] , rax            ; register to memory
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
                sub RDX, 0     ; make RDX to point at location of variable on the stack
                mov RAX, 0x0000_0000_0000_0000 ;machine has sizeof(int) to be 2, for uniformity, We are taking
                mov [RDX] , RAX ;our sizeof(int) to be 4, now scanf will just enter values in lower 32 bits
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
		mov      rdi , fmt_spec_int_in          ; get corresponding format specifier
				mov RDX, RBP
                sub RDX, 32     ; make RDX to point at location of variable on the stack
                mov RAX, 0x0000_0000_0000_0000 ;machine has sizeof(int) to be 2, for uniformity, We are taking
                mov [RDX] , RAX ;our sizeof(int) to be 4, now scanf will just enter values in lower 32 bits
                mov RSI, RBP
                sub RSI, 32 
                mov RAX, 0 
                rsp_align ;align RSP to 16 byte boundary for scanf call
                call scanf 
                rsp_realign ;realign it to original position
                		pop_regs        ; restore register values


		; Code for arithmetic
		push_regs                    ; save values
		mov     rax , [RBP - 32]
		mov     rbx , [RBP - 128]
		imul     rbx
		mov     [RBP - 192] , rax
		pop_regs        ; restore register values


		; Code for arithmetic
		push_regs                    ; save values
		mov     rax , [RBP - 0]
		add     rax , [RBP - 192]
		mov     [RBP - 224] , rax
		pop_regs        ; restore register values


		; Code for arithmetic
		push_regs                    ; save values
		mov     rax , [RBP - 96]
		sub     rax , [RBP - 128]
		mov     [RBP - 256] , rax
		pop_regs        ; restore register values


		; Code for arithmetic
		push_regs                    ; save values
		mov     rax , [RBP - 256]
		mov     rbx , [RBP - 32]
		imul     rbx
		mov     [RBP - 288] , rax
		pop_regs        ; restore register values


		; Code for arithmetic
		push_regs                    ; save values
		mov     rax , [RBP - 224]
		add     rax , [RBP - 288]
		mov     [RBP - 320] , rax
		pop_regs        ; restore register values


		; Code for arithmetic
		push_regs                    ; save values
		mov     rax , [RBP - 96]
		mov     rbx , 2
		imul     rbx
		mov    [RBP - 352] , rax
		pop_regs        ; restore register values


		; Code for arithmetic
		push_regs                    ; save values
		mov     rax , [RBP - 320]
		add     rax , [RBP - 352]
		mov     [RBP - 384] , rax
		pop_regs        ; restore register values


		; Code for arithmetic
		push_regs                    ; save values
		mov     rax , [RBP - 128]
		mov     rbx , [RBP - 0]
		imul     rbx
		mov     [RBP - 416] , rax
		pop_regs        ; restore register values


		; Code for arithmetic
		push_regs                    ; save values
		mov     rax , [RBP - 384]
		sub     rax , [RBP - 416]
		mov     [RBP - 448] , rax
		pop_regs        ; restore register values


		push_regs                    ; save values
		xor      rax , rax           ; flush out the rax register
		mov      rax , [RBP - 448]                    ; memory to register
		mov      [RBP - 64] , rax            ; register to memory
		pop_regs                    ; restore register values


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
		mov      rdi , fmt_spec_int_out                  ; get corresponding format specifier
		mov      rsi , [RBP - 32]                               ; move source index
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
		mov      rdi , fmt_spec_int_out                  ; get corresponding format specifier
		mov      rsi , [RBP - 64]                               ; move source index
		xor      rax , rax
		rsp_align                                         ; align stack pointer
		call     printf                                   ; system call for output
		rsp_realign                                       ; restore previous alignment of stack
		pop_regs                                          ; restore values
main_end:
		LEAVE
		retq
>>>>>>> codegen_exp
