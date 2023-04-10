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


		; Code for getting assignment statement
		push_regs                    ; save values
		xor      rax , rax           ; flush out the rax register
		mov      rax , 2                    ; immediate to register
		mov      [RBP - 0] , rax            ; register to memory
		pop_regs                    ; restore register values


		; Code for getting assignment statement
		push_regs                    ; save values
		xor      rax , rax           ; flush out the rax register
		mov      rax , 3                    ; immediate to register
		mov      [RBP - 32] , rax            ; register to memory
		pop_regs                    ; restore register values


		; Code for arithmetic
		push_regs                    ; save values
		mov     rax , [RBP - 32]
		mov     rbx , [RBP - 128]
		mul     rbx
		mov     [RBP - 480] , rax
		pop_regs        ; restore register values


		; Code for getting assignment statement
		push_regs                    ; save values
		xor      rax , rax           ; flush out the rax register
		mov      rax , [RBP - 480]                    ; memory to register
		mov      [RBP - 192] , rax            ; register to memory
		pop_regs                    ; restore register values


		; Code for arithmetic
		push_regs                    ; save values
		mov     rax , [RBP - 0]
		add     rax , [RBP - 192]
		mov     [RBP - 512] , rax
		pop_regs        ; restore register values


		; Code for getting assignment statement
		push_regs                    ; save values
		xor      rax , rax           ; flush out the rax register
		mov      rax , [RBP - 512]                    ; memory to register
		mov      [RBP - 224] , rax            ; register to memory
		pop_regs                    ; restore register values


		; Code for arithmetic
		push_regs                    ; save values
		mov     rax , [RBP - 96]
		sub     rax , [RBP - 128]
		mov     [RBP - 544] , rax
		pop_regs        ; restore register values


		; Code for getting assignment statement
		push_regs                    ; save values
		xor      rax , rax           ; flush out the rax register
		mov      rax , [RBP - 544]                    ; memory to register
		mov      [RBP - 256] , rax            ; register to memory
		pop_regs                    ; restore register values


		; Code for printing output
		push_regs                    ; save values
		mov      rdi , fmt_spec_int_out                  ; get corresponding format specifier
		mov      rdx , rbp                               ; take base pointer in rdx
		sub      rdx , 192                                ; move pointer to place from where we have to read
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
		sub      rdx , 224                                ; move pointer to place from where we have to read
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
		sub      rdx , 256                                ; move pointer to place from where we have to read
		mov      rsi , [rdx]                               ; move source index
		xor      rax , rax
		rsp_align                                     ; align stack pointer
		call     printf                                   ; system call for output
		rsp_realign                                      ; restore previous alignment of stack
		pop_regs        ; restore register values


		; Code for arithmetic
		push_regs                    ; save values
		mov     rax , [RBP - 256]
		mov     rbx , [RBP - 32]
		mul     rbx
		mov     [RBP - 576] , rax
		pop_regs        ; restore register values


		; Code for getting assignment statement
		push_regs                    ; save values
		xor      rax , rax           ; flush out the rax register
		mov      rax , [RBP - 576]                    ; memory to register
		mov      [RBP - 288] , rax            ; register to memory
		pop_regs                    ; restore register values


		; Code for arithmetic
		push_regs                    ; save values
		mov     rax , [RBP - 224]
		add     rax , [RBP - 288]
		mov     [RBP - 608] , rax
		pop_regs        ; restore register values


		; Code for getting assignment statement
		push_regs                    ; save values
		xor      rax , rax           ; flush out the rax register
		mov      rax , [RBP - 608]                    ; memory to register
		mov      [RBP - 320] , rax            ; register to memory
		pop_regs                    ; restore register values


		; Code for arithmetic
		push_regs                    ; save values
		mov     rax , [RBP - 96]
		mov     rbx , 2
		mul     rbx
		mov    [RBP - 640] , rax
		pop_regs        ; restore register values


		; Code for getting assignment statement
		push_regs                    ; save values
		xor      rax , rax           ; flush out the rax register
		mov      rax , [RBP - 640]                    ; memory to register
		mov      [RBP - 352] , rax            ; register to memory
		pop_regs                    ; restore register values


		; Code for arithmetic
		push_regs                    ; save values
		mov     rax , [RBP - 320]
		add     rax , [RBP - 352]
		mov     [RBP - 672] , rax
		pop_regs        ; restore register values


		; Code for getting assignment statement
		push_regs                    ; save values
		xor      rax , rax           ; flush out the rax register
		mov      rax , [RBP - 672]                    ; memory to register
		mov      [RBP - 384] , rax            ; register to memory
		pop_regs                    ; restore register values


		; Code for arithmetic
		push_regs                    ; save values
		mov     rax , [RBP - 128]
		mov     rbx , [RBP - 0]
		mul     rbx
		mov     [RBP - 704] , rax
		pop_regs        ; restore register values


		; Code for getting assignment statement
		push_regs                    ; save values
		xor      rax , rax           ; flush out the rax register
		mov      rax , [RBP - 704]                    ; memory to register
		mov      [RBP - 416] , rax            ; register to memory
		pop_regs                    ; restore register values


		; Code for arithmetic
		push_regs                    ; save values
		mov     rax , [RBP - 384]
		sub     rax , [RBP - 416]
		mov     [RBP - 736] , rax
		pop_regs        ; restore register values


		; Code for getting assignment statement
		push_regs                    ; save values
		xor      rax , rax           ; flush out the rax register
		mov      rax , [RBP - 736]                    ; memory to register
		mov      [RBP - 448] , rax            ; register to memory
		pop_regs                    ; restore register values


		; Code for getting assignment statement
		push_regs                    ; save values
		xor      rax , rax           ; flush out the rax register
		mov      rax , [RBP - 448]                    ; memory to register
		mov      [RBP - 64] , rax            ; register to memory
		pop_regs                    ; restore register values


		; Code for printing output
		push_regs                    ; save values
		mov      rdi , fmt_spec_int_out                  ; get corresponding format specifier
		mov      rdx , rbp                               ; take base pointer in rdx
		sub      rdx , 288                                ; move pointer to place from where we have to read
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
		sub      rdx , 320                                ; move pointer to place from where we have to read
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
		sub      rdx , 352                                ; move pointer to place from where we have to read
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
		sub      rdx , 384                                ; move pointer to place from where we have to read
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
		sub      rdx , 416                                ; move pointer to place from where we have to read
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
		sub      rdx , 448                                ; move pointer to place from where we have to read
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
		sub      rdx , 32                                ; move pointer to place from where we have to read
		mov      rsi , [rdx]                               ; move source index
		xor      rax , rax
		rsp_align                                     ; align stack pointer
		call     printf                                   ; system call for output
		rsp_realign                                      ; restore previous alignment of stack
		pop_regs        ; restore register values


main_end:
		retq