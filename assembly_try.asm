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
		mov      rbp , rsp               ; set base to current stack top
		; Code for getting assignment statement
		push_regs                    ; save values
		xor    rax , rax           ; flush out the rax register
		mov      rax , 5                    ; immediate to register
		mov      [RBP - 0] , rax            ; register to memory
		pop_regs                    ; restore values		; Code for getting user input
		push_regs                    ; save values
		mov      rdi , fmt_spec_int_in          ; get corresponding format specifier
		mov      rdx , rbp                               ; take base pointer in rdx
		sub      rdx , 0                                ; move pointer to place where we have to store
		mov      rax , 0x0000_0000_ffff_ffff             ; set size
		mov     [rdx] , rax
		mov      rsi , rdx                               ; move source index
		mov      rax , zero
		mov      rsi , rdx
		rsp_align                                        ; align stack pointer
		call     scanf                                   ; system call for input
		rsp_realign                                      ; restore previos alignment of stack
		pop_regs        ; restore register values
		; Code for printing output
		push_regs                    ; save values
		mov      rdi , fmt_spec_int_out                  ; get corresponding format specifier
		mov      rdx , rbp                               ; take base pointer in rdx
		sub      rdx , 0                                ; move pointer to place from where we have to read
		mov      rsi , [rdx]                               ; move source index
		xor      rax , rax
		rsp_align                                     ; align stack pointer
		call     printf                                   ; system call for output
		rsp_realign                                      ; restore previos alignment of stack
		pop_regs        ; restore register values
