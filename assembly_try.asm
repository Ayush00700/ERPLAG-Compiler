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
		true: db "true", 10, 0
		true_len: equ $ - true
		false: db "false", 10, 0
		false_len: equ $ - false
		zero: equ 0
		t1:   dq   0
		t2:   dq   0
		t3:   dq   0
		t4:   dq   0
		t5:   dq   0
		t6:   dq   0
		t7:   dq   0
		t8:   dq   0
		x:   dq   0
		t9:   dq   0
		y:   dq   0
		z:   dq   0
		a:   dq   0
		b:   dq   0
		c:   dq   0


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
		mov      rbp , rsp               ; set base to current stack top
		pop_regs                     ; save values
		; Code for getting assignment statement
		push_regs                    ; save values
		xor      rax , rax           ; flush out the rax register
		mov      rax , 5                    ; immediate to register
		mov      [a] , rax            ; register to memory
		pop_regs                    ; restore register values


		; Code for getting assignment statement
		push_regs                    ; save values
		xor      rax , rax           ; flush out the rax register
		mov      rax , 9                    ; immediate to register
		mov      [b] , rax            ; register to memory
		pop_regs                    ; restore register values


		; Code for getting user input
		push_regs                    ; save values
		mov      rdi , fmt_spec_int_in          ; get corresponding format specifier
				mov RDX, RBP
                ;sub RDX, 0     ; make RDX to point at location of variable on the stack
                ;So, we are firstly clearing upper 32 bits of memory so as to access data properly later
                mov RSI, x 
                mov RAX, 0 
                rsp_align ;align RSP to 16 byte boundary for scanf call
                call scanf 
                rsp_realign ;realign it to original position
		pop_regs        ; restore register values


		; Code for getting user input
		push_regs                    ; save values
		mov      rdi , fmt_spec_int_in          ; get corresponding format specifier
				mov RDX, RBP
                ;sub RDX, 0     ; make RDX to point at location of variable on the stack
                ;So, we are firstly clearing upper 32 bits of memory so as to access data properly later
                mov RSI, y 
                mov RAX, 0 
                rsp_align ;align RSP to 16 byte boundary for scanf call
                call scanf 
                rsp_realign ;realign it to original position
		pop_regs        ; restore register values


		; Code for arithmetic
		push_regs                    ; save values
		mov     rax , [y]
		mov     rbx , [b]
		imul     rbx
		mov     [t1] , rax
		pop_regs        ; restore register values


		; Code for arithmetic
		push_regs                    ; save values
		mov     rax , [x]
		add     rax , [t1]
		mov     [t2] , rax
		pop_regs        ; restore register values


		; Code for arithmetic
		push_regs                    ; save values
		mov     rax , [a]
		sub     rax , [b]
		mov     [t3] , rax
		pop_regs        ; restore register values


		; Code for arithmetic
		push_regs                    ; save values
		mov     rax , [t3]
		mov     rbx , [y]
		imul     rbx
		mov     [t4] , rax
		pop_regs        ; restore register values


		; Code for arithmetic
		push_regs                    ; save values
		mov     rax , [t2]
		add     rax , [t4]
		mov     [t5] , rax
		pop_regs        ; restore register values


		; Code for arithmetic
		push_regs                    ; save values
		mov     rax , [a]
		mov     rbx , 2
		imul     rbx
		mov    [t6] , rax
		pop_regs        ; restore register values


		; Code for arithmetic
		push_regs                    ; save values
		mov     rax , [t5]
		add     rax , [t6]
		mov     [t7] , rax
		pop_regs        ; restore register values


		; Code for arithmetic
		push_regs                    ; save values
		mov     rax , [b]
		mov     rbx , [x]
		imul     rbx
		mov     [t8] , rax
		pop_regs        ; restore register values


		; Code for arithmetic
		push_regs                    ; save values
		mov     rax , [t7]
		sub     rax , [t8]
		mov     [t9] , rax
		pop_regs        ; restore register values


		; Code for getting assignment statement
		push_regs                    ; save values
		xor      rax , rax           ; flush out the rax register
		mov      rax , [t9]                    ; memory to register
		mov      [z] , rax            ; register to memory
		pop_regs                    ; restore register values


		; Code for printing output
		push_regs                    ; save values
		mov      rdi , fmt_spec_int_out                  ; get corresponding format specifier
		mov      rsi , [z]                               ; move source index
		xor      rax , rax
		rsp_align                                     ; align stack pointer
		call     printf                                   ; system call for output
		rsp_realign                                      ; restore previous alignment of stack
		pop_regs                    ; save values
main_end:
		retq