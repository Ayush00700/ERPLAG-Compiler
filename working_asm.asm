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
		x:   dq   0
		y:   dq   0
		z:   dq   0
		d1:   dq   0
		d2:   dq   0
		d3:   dq   0
		d4:   dq   0
		d5:   dq   0
		d6:   dq   0
		d7:   dq   0
		d8:   dq   0
		d9:   dq   0
		a:   dq   0
		b:   dq   0
		c:   dq   0


		section      .text
		;macro to store all registers
		; %macro  ;push_regs    0
		; push      rax
		; push      rbx
		; push      rcx
		; push      rdx
		; push      rsi
		; push      rdi
		; %endmacro


		; ;macro to restore all registers
		; %macro  ;pop_regs    0
		; pop      rdi
		; pop      rsi
		; pop      rdx
		; pop      rcx
		; pop      rbx
		; pop      rax
		; %endmacro


		;macro to align RSP
		; %macro  push rbp    0
		; push      rbx
		; push      rdx
		; push      rax
		; xor       rdx , rdx
		; mov       rax , rsp
		; mov       rbx , 8
		; idiv      rbx
		; mov       rbx , rdx
		; pop       rax
		; pop       rdx
		; sub       rsp , rbx
		; %endmacro


		;macro to re-align RSP
		; %macro  pop rbp    0
		; add      rsp , rbx
		; pop      rbx
		; %endmacro


		global main
main:
		;push_regs                    ; save values
		mov      rbp , rsp               ; set base to current stack top
		;pop_regs                     ; save values
		; Code for getting assignment statement
		;push_regs                    ; save values
		mov		 rax, 0           ; flush out the rax register
		mov      rax , 15                    ; immediate to register
		mov      [a] , rax            ; register to memory
		;pop_regs                    ; restore register values


		; Code for getting assignment statement
		;push_regs                    ; save values
		mov		 rax, 0           ; flush out the rax register
		mov      rax , 9                    ; immediate to register
		mov      [b] , rax            ; register to memory
		;pop_regs                    ; restore register values


		; Code for getting assignment statement
		;push_regs                    ; save values
		mov		 rax, 0           ; flush out the rax register
		mov      rax , 2                    ; immediate to register
		mov      [x] , rax            ; register to memory
		;pop_regs                    ; restore register values


		; Code for getting assignment statement
		;push_regs                    ; save values
		mov		 rax, 0           ; flush out the rax register
		mov      rax , 3                    ; immediate to register
		mov      [y] , rax            ; register to memory
		;pop_regs                    ; restore register values


		; Code for arithmetic
		;push_regs                    ; save values
		mov     rax , [a]
		add     rax , [b]
		mov     [t1] , rax
		;pop_regs        ; restore register values


		; Code for getting assignment statement
		;push_regs                    ; save values
		mov		 rax, 0           ; flush out the rax register
		mov      rax , [t1]                    ; memory to register
		mov      [z] , rax            ; register to memory
		;pop_regs                    ; restore register values


		; Code for arithmetic
		;push_regs                    ; save values
		mov     rax , [z]
		add     rax , [a]
		mov     [t2] , rax
		;pop_regs        ; restore register values


		; Code for getting assignment statement
		;push_regs                    ; save values
		mov		 rax, 0           ; flush out the rax register
		mov      rax ,  [t2]                    ; memory to register
		mov       [y] , rax            ; register to memory
		;pop_regs                    ; restore register values


		; Code for printing output
		;push_regs                    ; save values
		push rbp                                     ; align stack pointer
		mov      rdi , fmt_spec_int_out                  ; get corresponding format specifier
		mov      rsi ,  [x]                               ; move source index
		mov		 rax, 0
		call     printf                                   ; system call for output
		pop rbp            
		mov rax, 0                                 ; restore previous alignment of stack
		;pop_regs        ; restore register values


		; Code for printing output
		;push_regs                    ; save values
		push rbp                                     ; align stack pointer
		mov      rdi , fmt_spec_int_out                  ; get corresponding format specifier
		mov      rsi ,  [z]                               ; move source index
		mov		 rax, 0
		call     printf                                   ; system call for output
		pop rbp            
		mov rax, 0                          ; restore previous alignment of stack
		;pop_regs        ; restore register values


		; Code for printing output
		;push_regs                    ; save values
		push rbp                                     ; align stack pointer
		mov      rdi , fmt_spec_int_out                  ; get corresponding format specifier
		mov      rsi ,  [y]                               ; move source index
		mov		 rax, 0
		call     printf                                   ; system call for output
		pop rbp            
		mov rax, 0
		; mov rax, 0                                      ; restore previous alignment of stack
		;pop_regs        ; restore register values


		; Code for printing output
		;push_regs                    ; save values
		push rbp                                     ; align stack pointer
		mov      rdi , fmt_spec_int_out                  ; get corresponding format specifier
		mov      rsi ,  [a]                               ; move source index
		mov		 rax, 0
		call     printf                                   ; system call for output
		pop rbp            
		mov rax, 0                                   ; restore previous alignment of stack
		;pop_regs        ; restore register values


		; Code for printing output
		;push_regs                    ; save values
		push rbp                                     ; align stack pointer
		mov      rdi , fmt_spec_int_out                  ; get corresponding format specifier
		mov      rsi ,  [b]                               ; move source index
		mov		 rax, 0
		call     printf                                   ; system call for output
		pop rbp            
		mov rax, 0                                 ; restore previous alignment of stack
		;pop_regs        ; restore register values
