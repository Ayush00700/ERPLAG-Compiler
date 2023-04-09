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
		fmt_spec_int: db "%d", 10, 0
		fmt_spec_real: db "%4f", 10, 0
		fmt_spec_string: db "%s", 10, 0
		fmt_spec_bool: db "%d", 10, 0
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
		push      rsp
		push      rbp
		push      rsi
		push      rdi
		%endmacro


		;macro to restore all registers
		%macro  pop_regs    0
		push      rdi
		push      rsi
		push      rbp
		push      rsp
		push      rdx
		push      rcx
		push      rbx
		push      rax
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
		push      rdx
		%endmacro


		global main
main:
		; Code for getting assignment statement
		push_regs                    ; save values
		xor    rax , rax           ; flush out the rax register		mov      rax , 5                    ; immediate to register
		mov      [RBP - 12] , rax            ; register to memory
		pop_regs                    ; restore values		; Code for getting assignment statement
		push_regs                    ; save values
		xor    rax , rax           ; flush out the rax register		mov      rax , 9                    ; immediate to register
		mov      [RBP - 16] , rax            ; register to memory
		pop_regs                    ; restore values		; Code for getting user input
		push_regs                    ; save values
		mov      rdi , fmt_spec_int          ; get corresponding format specifier
		mov      rdx , rbp                               ; take base pointer in rdx
		sub      rdx , 0                                ; move pointer to place where we have to store
		mov      rax , 0x0000_0000_ffff_ffff             ; set size
		mov      [rdx] , rax
		mov      rsi , rdx                               ; move source index
		mov      rax , zero
		mov      rsi , rdx
		call     scanf                                   ; system call for input
		pop_regs        ; restore register values
		; Code for getting user input
		push_regs                    ; save values
		mov      rdi , fmt_spec_int          ; get corresponding format specifier
		mov      rdx , rbp                               ; take base pointer in rdx
		sub      rdx , 4                                ; move pointer to place where we have to store
		mov      rax , 0x0000_0000_ffff_ffff             ; set size
		mov      [rdx] , rax
		mov      rsi , rdx                               ; move source index
		mov      rax , zero
		mov      rsi , rdx
		call     scanf                                   ; system call for input
		pop_regs        ; restore register values
		; Code for arithmetic
		push_regs                    ; save values
		mov     rax , [RBP - 4]
		mov     rbx , [RBP - 16]
		mul     rbx
		mov     [RBP - 24] , rax
		pop_regs        ; restore register values
		; Code for arithmetic
		push_regs                    ; save values
		mov     rax , [RBP - 0]
		add     rax , [RBP - 24]
		mov     [RBP - 28] , rax
		pop_regs        ; restore register values
		; Code for arithmetic
		push_regs                    ; save values
		mov     rax , [RBP - 12]
		sub     rax , [RBP - 16]
		mov     [RBP - 32] , rax
		pop_regs        ; restore register values
		; Code for arithmetic
		push_regs                    ; save values
		mov     rax , [RBP - 32]
		mov     rbx , [RBP - 4]
		mul     rbx
		mov     [RBP - 36] , rax
		pop_regs        ; restore register values
		; Code for arithmetic
		push_regs                    ; save values
		mov     rax , [RBP - 28]
		add     rax , [RBP - 36]
		mov     [RBP - 40] , rax
		pop_regs        ; restore register values
		; Code for arithmetic
		push_regs                    ; save values
		mov     rax , [RBP - 12]
		mov     rbx , 2
		mul     rbx
		mov     [RBP - 44] , rax
		pop_regs        ; restore register values
		; Code for arithmetic
		push_regs                    ; save values
		mov     rax , [RBP - 40]
		add     rax , [RBP - 44]
		mov     [RBP - 48] , rax
		pop_regs        ; restore register values
		; Code for arithmetic
		push_regs                    ; save values
		mov     rax , [RBP - 16]
		mov     rbx , [RBP - 0]
		mul     rbx
		mov     [RBP - 52] , rax
		pop_regs        ; restore register values
		; Code for arithmetic
		push_regs                    ; save values
		mov     rax , [RBP - 48]
		sub     rax , [RBP - 52]
		mov     [RBP - 56] , rax
		pop_regs        ; restore register values
		; Code for getting assignment statement
		push_regs                    ; save values
		xor    rax , rax           ; flush out the rax register		mov      rax , [RBP - 56]                    ; memory to register
		mov      [RBP - 8] , rax            ; register to memory
		pop_regs                    ; restore values		; Code for printing output
		push_regs                    ; save values
		mov      rdi , fmt_spec_int          ; get corresponding format specifier
		mov      rdx , rbp                               ; take base pointer in rdx
		sub      rdx , 8                                ; move pointer to place from where we have to read
		mov      rsi , [rdx]                               ; move source index
		xor      rax , rax
		call     printf                                   ; system call for output
		pop_regs        ; restore register values
