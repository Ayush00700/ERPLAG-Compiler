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
		t1:   dd   0
		t2:   dd   0
		x:   dd   0
		y:   dd   0
		z:   dd   0
		d1:   dd   0
		d2:   dd   0
		d3:   dd   0
		d4:   dd   0
		d5:   dd   0
		d6:   dd   0
		d7:   dd   0
		d8:   dd   0
		d9:   dd   0
		a:   dd   0
		b:   dd   0
		c:   dd   0


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
		mov      [RBP - 96] , rax            ; register to memory
		pop_regs                    ; restore register values


		; Code for getting assignment statement
		push_regs                    ; save values
		xor      rax , rax           ; flush out the rax register
		mov      rax , 9                    ; immediate to register
		mov      [RBP - 128] , rax            ; register to memory
		pop_regs                    ; restore register values


		; Code for getting user input
		push_regs                    ; save values
		mov      rdi , fmt_spec_int_in          ; get corresponding format specifier
				mov RDX, RBP
                sub RDX, 0     ; make RDX to point at location of variable on the stack
                ;So, we are firstly clearing upper 32 bits of memory so as to access data properly later
                mov RSI, RDX 
                mov RAX, 0 
                rsp_align ;align RSP to 16 byte boundary for scanf call
                call scanf 
                rsp_realign ;realign it to original position
		pop_regs        ; restore register values


		; Code for getting user input
		push_regs                    ; save values
		mov      rdi , fmt_spec_int_in          ; get corresponding format specifier
				mov RDX, RBP
                sub RDX, 32     ; make RDX to point at location of variable on the stack
                ;So, we are firstly clearing upper 32 bits of memory so as to access data properly later
                mov RSI, RDX 
                mov RAX, 0 
                rsp_align ;align RSP to 16 byte boundary for scanf call
                call scanf 
                rsp_realign ;realign it to original position
		pop_regs        ; restore register values


		; Code for arithmetic
		push_regs                    ; save values
		mov     rax , [RBP - 96]
		sub     rax , [RBP - 128]
		mov     [RBP - 480] , rax
		pop_regs        ; restore register values


		; Code for getting assignment statement
		push_regs                    ; save values
		xor      rax , rax           ; flush out the rax register
		mov      rax , [RBP - 480]                    ; memory to register
		mov      [RBP - 64] , rax            ; register to memory
		pop_regs                    ; restore register values


		; Code for arithmetic
		push_regs                    ; save values
		mov     rax , [RBP - 64]
		add     rax , [RBP - 96]
		mov     [RBP - 512] , rax
		pop_regs        ; restore register values


		; Code for getting assignment statement
		push_regs                    ; save values
		xor      rax , rax           ; flush out the rax register
		mov      rax , [RBP - 512]                    ; memory to register
		mov      [RBP - 32] , rax            ; register to memory
		pop_regs                    ; restore register values


		; Code for printing output
		push_regs                    ; save values
		mov      rdi , fmt_spec_int_out                  ; get corresponding format specifier
		mov      rdx , rbp                               ; take base pointer in rdx
		sub      rdx , 0                                ; move pointer to place from where we have to read
		mov      rsi , [rdx]                               ; move source index
		xor      rax , rax
		rsp_align                                     ; align stack pointer
		call     printf                                   ; system call for output
		rsp_realign                                      ; restore previous alignment of stack
		pop_regs        ; restore register values


		; Code for printing output
		push_regs                    ; save values
		mov      rdi , fmt_spec_int_out                  ; get corresponding format specifier
		mov      rdx , rbp                               ; take base pointer in rdx
		sub      rdx , 64                                ; move pointer to place from where we have to read
		mov      rsi , [rdx]                               ; move source index
		xor      rax , rax
		rsp_align                                     ; align stack pointer
		call     printf                                   ; system call for output
		rsp_realign                                      ; restore previous alignment of stack
		pop_regs        ; restore register values


		; Code for printing output
		push_regs                    ; save values
		mov      rdi , fmt_spec_int_out                  ; get corresponding format specifier
		mov      rdx , rbp                               ; take base pointer in rdx
		sub      rdx , 32                                ; move pointer to place from where we have to read
		mov      rsi , [rdx]                               ; move source index
		xor      rax , rax
		rsp_align                                     ; align stack pointer
		call     printf                                   ; system call for output
		rsp_realign                                      ; restore previous alignment of stack
		pop_regs        ; restore register values


		; Code for printing output
		push_regs                    ; save values
		mov      rdi , fmt_spec_int_out                  ; get corresponding format specifier
		mov      rdx , rbp                               ; take base pointer in rdx
		sub      rdx , 96                                ; move pointer to place from where we have to read
		mov      rsi , [rdx]                               ; move source index
		xor      rax , rax
		rsp_align                                     ; align stack pointer
		call     printf                                   ; system call for output
		rsp_realign                                      ; restore previous alignment of stack
		pop_regs        ; restore register values


		; Code for printing output
		push_regs                    ; save values
		mov      rdi , fmt_spec_int_out                  ; get corresponding format specifier
		mov      rdx , rbp                               ; take base pointer in rdx
		sub      rdx , 128                                ; move pointer to place from where we have to read
		mov      rsi , [rdx]                               ; move source index
		xor      rax , rax
		rsp_align                                     ; align stack pointer
		call     printf                                   ; system call for output
		rsp_realign                                      ; restore previous alignment of stack
		pop_regs        ; restore register values


main_end:
		retq