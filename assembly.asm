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
		fmt_spec_int_arr_out: db "%d ", 0
		fmt_spec_int_arr_out_last: db "%d", 10, 0
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
<<<<<<< Updated upstream
		printError: db "Out of Bound Exception in Array", 10, 0
		printError_len: db "Out of Bound Exception in Array", 10, 0
		array_in: db "Input: Enter %d elements of %s type for range %d to %d", 10, 0
		array_in_len: equ $ - array_in
=======
		printError: db "Error, Array Out of Dynamic Bound  ", 10, 0
		printError_len: equ $ - printError
		array_in1: db "Input: Enter", 20, 0
		array_in_len1: equ $ - array_in1
		array_in2: db "elements of", 20, 0
		array_in_len2: equ $ - array_in2
		array_in3: db "type for range", 20, 0
		array_in_len3: equ $ - array_in3
		array_in4: db "to", 20, 0
		array_in_len4: equ $ - array_in4
>>>>>>> Stashed changes
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
<<<<<<< Updated upstream
		ENTER    1232,0
=======
		ENTER    1856,0
>>>>>>> Stashed changes
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
		mov      rdx , rbp
		sub      rdx , 0
		mov      rax , 0x0000_0000_0000_0000
		mov      [rdx] , rax
		mov      rsi , rbp
		sub      rsi , 0
		mov      rax , 0
		rsp_align
		call     scanf
		rsp_realign
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
		mov      rdx , rbp
		sub      rdx , 32
		mov      rax , 0x0000_0000_0000_0000
		mov      [rdx] , rax
		mov      rsi , rbp
		sub      rsi , 32
		mov      rax , 0
		rsp_align
		call     scanf
		rsp_realign
		pop_regs        ; restore register values


		; Code for arithmetic
		push_regs                    ; save values
		mov     rax , [RBP - 32]
		mov     rbx , [RBP - 160]
		imul     rbx
		mov     [RBP - 560] , rax
		pop_regs        ; restore register values


		; Code for arithmetic
		push_regs                    ; save values
		mov     rax , [RBP - 0]
		add     rax , [RBP - 560]
		mov     [RBP - 592] , rax
		pop_regs        ; restore register values


		; Code for arithmetic
		push_regs                    ; save values
		mov     rax , [RBP - 128]
		sub     rax , [RBP - 160]
		mov     [RBP - 624] , rax
		pop_regs        ; restore register values


		; Code for arithmetic
		push_regs                    ; save values
		mov     rax , [RBP - 624]
		mov     rbx , [RBP - 32]
		imul     rbx
		mov     [RBP - 656] , rax
		pop_regs        ; restore register values


		; Code for arithmetic
		push_regs                    ; save values
		mov     rax , [RBP - 592]
		add     rax , [RBP - 656]
		mov     [RBP - 688] , rax
		pop_regs        ; restore register values


		; Code for arithmetic
		push_regs                    ; save values
		mov     rax , [RBP - 128]
		mov     rbx , 2
		imul     rbx
		mov    [RBP - 720] , rax
		pop_regs        ; restore register values


		; Code for arithmetic
		push_regs                    ; save values
		mov     rax , [RBP - 688]
		add     rax , [RBP - 720]
		mov     [RBP - 752] , rax
		pop_regs        ; restore register values


		; Code for arithmetic
		push_regs                    ; save values
		mov     rax , [RBP - 160]
		mov     rbx , [RBP - 0]
		imul     rbx
		mov     [RBP - 784] , rax
		pop_regs        ; restore register values


		; Code for arithmetic
		push_regs                    ; save values
		mov     rax , [RBP - 752]
		sub     rax , [RBP - 784]
		mov     [RBP - 816] , rax
		pop_regs        ; restore register values


		push_regs                    ; save values
		xor      rax , rax           ; flush out the rax register
		mov      rax , [RBP - 816]                    ; memory to register
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
		mov      rsi , [RBP - 64]                               ; move source index
		xor      rax , rax
		rsp_align                                         ; align stack pointer
		call     printf                                   ; system call for output
		rsp_realign                                       ; restore previous alignment of stack
		pop_regs                                          ; restore values
		push_regs                    ; save values
		xor      rax , rax           ; flush out the rax register
		mov      rax , 10                    ; immediate to register
		mov      [RBP - 464] , rax            ; register to memory
		pop_regs                    ; restore register values


L2:
		; Code for relational
		push_regs                    ; save values
		mov     rax , [RBP - 464]
		mov     rbx , 15
		cmp     rax , rbx
<<<<<<< Updated upstream
		jle     L4
		mov     qword [RBP - 560], 0
		jmp  L5
L4:
		mov     qword [RBP - 560], 1
L5:
=======
		jle     L8
		mov     qword [RBP - 560], 0
		jmp  L9
L8:
		mov     qword [RBP - 560], 1
L9:
>>>>>>> Stashed changes
		pop_regs        ; restore register values


		mov      rax , [RBP - 560]
		cmp      rax , 1
		je       L3
		jmp      L4
L3:
		push_regs                    ; save values
		xor      rax , rax           ; flush out the rax register
		mov      rax , 7                    ; immediate to register
		mov      [RBP - 496] , rax            ; register to memory
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
		mov      rdx , rbp
		sub      rdx , 528
		mov      rax , 0x0000_0000_0000_0000
		mov      [rdx] , rax
		mov      rsi , rbp
		sub      rsi , 528
		mov      rax , 0
		rsp_align
		call     scanf
		rsp_realign
		pop_regs        ; restore register values


		; Code for arithmetic
		push_regs                    ; save values
		mov     rax , [RBP - 496]
		add     rax , [RBP - 528]
		mov     [RBP - 576] , rax
		pop_regs        ; restore register values


		push_regs                    ; save values
		xor      rax , rax           ; flush out the rax register
		mov      rax , [RBP - 576]                    ; memory to register
		mov      [RBP - 496] , rax            ; register to memory
		pop_regs                    ; restore register values


		; Code for printing output
		mov      rax , 1
		mov      rdi , 1
		mov      rsi , print_out
		mov      rdx , print_out_len
		syscall

		push_regs                                         ; save values
		mov      rdi , fmt_spec_int_out                  ; get corresponding format specifier
		mov      rsi , [RBP - 496]                               ; move source index
		xor      rax , rax
		rsp_align                                         ; align stack pointer
		call     printf                                   ; system call for output
		rsp_realign                                       ; restore previous alignment of stack
		pop_regs                                          ; restore values
		; Code for arithmetic
		push_regs                    ; save values
		mov     rax , [RBP - 64]
		add     rax , [RBP - 464]
		mov     [RBP - 608] , rax
		pop_regs        ; restore register values


		; Code for arithmetic
		push_regs                    ; save values
		mov     rax , [RBP - 496]
		mov     rbx , 2
		imul     rbx
		mov    [RBP - 640] , rax
		pop_regs        ; restore register values


		; Code for arithmetic
		push_regs                    ; save values
		mov     rax , [RBP - 608]
		add     rax , [RBP - 640]
		mov     [RBP - 672] , rax
		pop_regs        ; restore register values


		push_regs                    ; save values
		xor      rax , rax           ; flush out the rax register
		mov      rax , [RBP - 672]                    ; memory to register
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
		mov      rsi , [RBP - 64]                               ; move source index
		xor      rax , rax
		rsp_align                                         ; align stack pointer
		call     printf                                   ; system call for output
		rsp_realign                                       ; restore previous alignment of stack
		pop_regs                                          ; restore values
		; Code for arithmetic
		push_regs                    ; save values
		mov     rax , [RBP - 464]
		add     rax , 1
		mov     [RBP - 464] , rax
		pop_regs        ; restore register values


		jmp      L2
L4:
		; Code for arithmetic
		push_regs                    ; save values
		mov     rax , [RBP - 32]
		mov     rbx , [RBP - 160]
		imul     rbx
		mov     [RBP - 848] , rax
		pop_regs        ; restore register values


		; Code for arithmetic
		push_regs                    ; save values
		mov     rax , [RBP - 0]
		add     rax , [RBP - 848]
		mov     [RBP - 880] , rax
		pop_regs        ; restore register values


		; Code for arithmetic
		push_regs                    ; save values
		mov     rax , [RBP - 128]
		sub     rax , [RBP - 160]
		mov     [RBP - 912] , rax
		pop_regs        ; restore register values


		; Code for arithmetic
		push_regs                    ; save values
		mov     rax , [RBP - 912]
		mov     rbx , [RBP - 32]
		imul     rbx
		mov     [RBP - 944] , rax
		pop_regs        ; restore register values


		; Code for arithmetic
		push_regs                    ; save values
		mov     rax , [RBP - 880]
		add     rax , [RBP - 944]
		mov     [RBP - 976] , rax
		pop_regs        ; restore register values


		; Code for arithmetic
		push_regs                    ; save values
		mov     rax , [RBP - 128]
		mov     rbx , 2
		imul     rbx
		mov    [RBP - 1008] , rax
		pop_regs        ; restore register values


		; Code for arithmetic
		push_regs                    ; save values
		mov     rax , [RBP - 976]
		add     rax , [RBP - 1008]
		mov     [RBP - 1040] , rax
		pop_regs        ; restore register values


		; Code for arithmetic
		push_regs                    ; save values
		mov     rax , [RBP - 160]
		mov     rbx , [RBP - 0]
		imul     rbx
		mov     [RBP - 1072] , rax
		pop_regs        ; restore register values


		; Code for arithmetic
		push_regs                    ; save values
		mov     rax , [RBP - 1040]
		sub     rax , [RBP - 1072]
		mov     [RBP - 1104] , rax
		pop_regs        ; restore register values


		push_regs                    ; save values
		xor      rax , rax           ; flush out the rax register
		mov      rax , [RBP - 1104]                    ; memory to register
		mov      [RBP - 64] , rax            ; register to memory
		pop_regs                    ; restore register values


		; Code for relational
		push_regs                    ; save values
		mov     rax , [RBP - 64]
		mov     rbx , 10
		cmp     rax , rbx
<<<<<<< Updated upstream
		jg     L6
		mov     qword [RBP - 1136], 0
		jmp  L7
L6:
		mov     qword [RBP - 1136], 1
L7:
=======
		jg     L10
		mov     qword [RBP - 1136], 0
		jmp  L11
L10:
		mov     qword [RBP - 1136], 1
L11:
>>>>>>> Stashed changes
		pop_regs        ; restore register values


		; Code for relational
		push_regs                    ; save values
		mov     rax , [RBP - 128]
		mov     rbx , [RBP - 160]
		cmp     rax , rbx
<<<<<<< Updated upstream
		jle     L8
		mov     qword [RBP - 1152], 0
		jmp  L9
L8:
		mov     qword [RBP - 1152], 1
L9:
=======
		jle     L12
		mov     qword [RBP - 1152], 0
		jmp  L13
L12:
		mov     qword [RBP - 1152], 1
L13:
>>>>>>> Stashed changes
		pop_regs        ; restore register values


		; Code for logical op
		push_regs                    ; save values
		mov     rax , [RBP - 1136]
		or     rax , [RBP - 1152]
		mov     [RBP - 1168] , rax
		pop_regs        ; restore register values


		; Code for relational
		push_regs                    ; save values
		mov     rax , [RBP - 0]
		mov     rbx , [RBP - 32]
		cmp     rax , rbx
<<<<<<< Updated upstream
		jl     L10
		mov     qword [RBP - 1184], 0
		jmp  L11
L10:
		mov     qword [RBP - 1184], 1
L11:
=======
		jl     L14
		mov     qword [RBP - 1184], 0
		jmp  L15
L14:
		mov     qword [RBP - 1184], 1
L15:
>>>>>>> Stashed changes
		pop_regs        ; restore register values


		; Code for logical op
		push_regs                    ; save values
		mov     rax , [RBP - 1168]
		and     rax , [RBP - 1184]
		mov     [RBP - 1200] , rax
		pop_regs        ; restore register values


		; Code for logical op
		push_regs                    ; save values
		mov     rax , [RBP - 1200]
		and     rax , [RBP - 224]
		mov     [RBP - 1216] , rax
		pop_regs        ; restore register values


		push_regs                    ; save values
		xor      rax , rax           ; flush out the rax register
		mov      rax , [RBP - 1216]                    ; memory to register
		mov      [RBP - 240] , rax            ; register to memory
		pop_regs                    ; restore register values


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
		; Code for printing output
		mov      rax , 1
		mov      rdi , 1
		mov      rsi , print_out
		mov      rdx , print_out_len
		syscall

		push_regs                                         ; save values
		mov      rdi , fmt_spec_bool_out                  ; get corresponding format specifier
		mov      rax , [RBP - 224]                               ; move source index
		cmp      rax , 0
<<<<<<< Updated upstream
		jne      L12
=======
		jne      L16
>>>>>>> Stashed changes
		mov      rax , 1
		mov      rdi , 1
		mov      rsi , false
		mov      rdx , false_len
		syscall
		pop_regs                                          ; restore register values
<<<<<<< Updated upstream
		jmp  L13


L12:
=======
		jmp  L17


L16:
>>>>>>> Stashed changes
		mov      rax , 1
		mov      rdi , 1
		mov      rsi , true
		mov      rdx , true_len
		syscall
		pop_regs        ; restore register values
<<<<<<< Updated upstream
L13:
=======
L17:


		; Code for getting user input
		push_regs                    ; save values
		; Display prompt for array input
		; Prompt part 1
		mov      rax , 1
		mov      rdi , 1
		mov      rsi , array_in1
		mov      rdx , array_in_len1
		syscall

		; Prompt part 2
		push_regs                    ; save values
		lea      rdi , [rel fmt_spec_int_arr_out]                ; get corresponding format specifier
		mov      rsi , 6                               ; move source index
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
		lea      rdi , [rel fmt_spec_int_arr_out]                ; get corresponding format specifier
		mov      rsi , 10                               ; move source index
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
		lea      rdi , [rel fmt_spec_int_out]                ; get corresponding format specifier
		mov      rsi , 15                               ; move source index
		xor      rax , rax
		rsp_align                                     ; align stack pointer
		call     printf                               ; system call for output
		rsp_realign                                   ; restore previous alignment of stack


		; Code to get integer array input
		mov      rdi , fmt_spec_int_in          ; get corresponding format specifier
		mov      rdx , rbp
		sub      rdx , 256
		mov      rax , 0x0000_0000_0000_0000
		mov      [rdx] , rax
		mov      rsi , rbp
		sub      rsi , 256
		mov      rax , 0
		rsp_align
		call     scanf
		rsp_realign
		mov      rdi , fmt_spec_int_in          ; get corresponding format specifier
		mov      rdx , rbp
		sub      rdx , 288
		mov      rax , 0x0000_0000_0000_0000
		mov      [rdx] , rax
		mov      rsi , rbp
		sub      rsi , 288
		mov      rax , 0
		rsp_align
		call     scanf
		rsp_realign
		mov      rdi , fmt_spec_int_in          ; get corresponding format specifier
		mov      rdx , rbp
		sub      rdx , 320
		mov      rax , 0x0000_0000_0000_0000
		mov      [rdx] , rax
		mov      rsi , rbp
		sub      rsi , 320
		mov      rax , 0
		rsp_align
		call     scanf
		rsp_realign
		mov      rdi , fmt_spec_int_in          ; get corresponding format specifier
		mov      rdx , rbp
		sub      rdx , 352
		mov      rax , 0x0000_0000_0000_0000
		mov      [rdx] , rax
		mov      rsi , rbp
		sub      rsi , 352
		mov      rax , 0
		rsp_align
		call     scanf
		rsp_realign
		mov      rdi , fmt_spec_int_in          ; get corresponding format specifier
		mov      rdx , rbp
		sub      rdx , 384
		mov      rax , 0x0000_0000_0000_0000
		mov      [rdx] , rax
		mov      rsi , rbp
		sub      rsi , 384
		mov      rax , 0
		rsp_align
		call     scanf
		rsp_realign
		mov      rdi , fmt_spec_int_in          ; get corresponding format specifier
		mov      rdx , rbp
		sub      rdx , 416
		mov      rax , 0x0000_0000_0000_0000
		mov      [rdx] , rax
		mov      rsi , rbp
		sub      rsi , 416
		mov      rax , 0
		rsp_align
		call     scanf
		rsp_realign
		pop_regs        ; restore register values


		; Code for arithmetic
		push_regs                    ; save values
		mov     rax , [RBP - 128]
		add     rax , [RBP - 0]
		mov     [RBP - 1232] , rax
		pop_regs        ; restore register values


		; Code for relational
		push_regs                    ; save values
		mov     rax , 13            ; immediate to memory
		mov     rbx , 10
		cmp     rax , rbx
		jl     L19
		mov     qword [RBP - 1264], 0
		jmp  L20
L19:
		mov     qword [RBP - 1264], 1
L20:
		pop_regs        ; restore register values


		; Code for relational
		push_regs                    ; save values
		mov     rax , 13            ; immediate to memory
		mov     rbx , 15
		cmp     rax , rbx
		jg     L21
		mov     qword [RBP - 1280], 0
		jmp  L22
L21:
		mov     qword [RBP - 1280], 1
L22:
		pop_regs        ; restore register values


		; Code for logical op
		push_regs                    ; save values
		mov     rax , [RBP - 1264]
		or     rax , [RBP - 1280]
		mov     [RBP - 1296] , rax
		pop_regs        ; restore register values


		mov      rax , [RBP - 1296]
		cmp      rax , 1
		je       L5
		; Code for arithmetic
		push_regs                    ; save values
		mov     rax , 13            ; immediate to memory
		mov     rbx , 2
		imul     rbx
		mov    [RBP - 1312] , rax
		pop_regs        ; restore register values


		; Code for arithmetic
		push_regs                    ; save values
		mov     rax , -4            ; immediate to memory
		add     rax , [RBP - 1312]
		mov     [RBP - 1344] , rax
		pop_regs        ; restore register values


		push_regs                    ; save values
		xor      rax , rax           ; flush out the rax register
		mov      rax , [RBP - 1344]                    ; memory to register
		mov      [RBP - 1376] , rax            ; register to memory
		pop_regs                    ; restore register values


		; Code for arithmetic
		push_regs                    ; save values
		mov     rax , [RBP - 1376]
		mov     rbx , 2
		imul     rbx
		mov    [RBP - 1408] , rax
		pop_regs        ; restore register values


		; Code for arithmetic
		push_regs                    ; save values
		mov     rax , [RBP - 1232]
		add     rax , [RBP - 1408]
		mov     [RBP - 1440] , rax
		pop_regs        ; restore register values


		; Code for relational
		push_regs                    ; save values
		mov     rax , 14            ; immediate to memory
		mov     rbx , 10
		cmp     rax , rbx
		jl     L23
		mov     qword [RBP - 1472], 0
		jmp  L24
L23:
		mov     qword [RBP - 1472], 1
L24:
		pop_regs        ; restore register values


		; Code for relational
		push_regs                    ; save values
		mov     rax , 14            ; immediate to memory
		mov     rbx , 15
		cmp     rax , rbx
		jg     L25
		mov     qword [RBP - 1488], 0
		jmp  L26
L25:
		mov     qword [RBP - 1488], 1
L26:
		pop_regs        ; restore register values


		; Code for logical op
		push_regs                    ; save values
		mov     rax , [RBP - 1472]
		or     rax , [RBP - 1488]
		mov     [RBP - 1504] , rax
		pop_regs        ; restore register values


		mov      rax , [RBP - 1504]
		cmp      rax , 1
		je       L6
		; Code for arithmetic
		push_regs                    ; save values
		mov     rax , 14            ; immediate to memory
		mov     rbx , 2
		imul     rbx
		mov    [RBP - 1520] , rax
		pop_regs        ; restore register values


		; Code for arithmetic
		push_regs                    ; save values
		mov     rax , -4            ; immediate to memory
		add     rax , [RBP - 1520]
		mov     [RBP - 1552] , rax
		pop_regs        ; restore register values


		push_regs                    ; save values
		xor      rax , rax           ; flush out the rax register
		mov      rax , [RBP - 1552]                    ; memory to register
		mov      [RBP - 1584] , rax            ; register to memory
		pop_regs                    ; restore register values


		; Code for arithmetic
		push_regs                    ; save values
		mov     rax , [RBP - 1584]
		mov     rbx , 3
		imul     rbx
		mov    [RBP - 1616] , rax
		pop_regs        ; restore register values


		; Code for arithmetic
		push_regs                    ; save values
		mov     rax , [RBP - 1440]
		add     rax , [RBP - 1616]
		mov     [RBP - 1648] , rax
		pop_regs        ; restore register values


		; Code for relational
		push_regs                    ; save values
		mov     rax , 15            ; immediate to memory
		mov     rbx , 10
		cmp     rax , rbx
		jl     L27
		mov     qword [RBP - 1680], 0
		jmp  L28
L27:
		mov     qword [RBP - 1680], 1
L28:
		pop_regs        ; restore register values


		; Code for relational
		push_regs                    ; save values
		mov     rax , 15            ; immediate to memory
		mov     rbx , 15
		cmp     rax , rbx
		jg     L29
		mov     qword [RBP - 1696], 0
		jmp  L30
L29:
		mov     qword [RBP - 1696], 1
L30:
		pop_regs        ; restore register values


		; Code for logical op
		push_regs                    ; save values
		mov     rax , [RBP - 1680]
		or     rax , [RBP - 1696]
		mov     [RBP - 1712] , rax
		pop_regs        ; restore register values


		mov      rax , [RBP - 1712]
		cmp      rax , 1
		je       L7
		; Code for arithmetic
		push_regs                    ; save values
		mov     rax , 15            ; immediate to memory
		mov     rbx , 2
		imul     rbx
		mov    [RBP - 1728] , rax
		pop_regs        ; restore register values


		; Code for arithmetic
		push_regs                    ; save values
		mov     rax , -4            ; immediate to memory
		add     rax , [RBP - 1728]
		mov     [RBP - 1760] , rax
		pop_regs        ; restore register values


		push_regs                    ; save values
		xor      rax , rax           ; flush out the rax register
		mov      rax , [RBP - 1760]                    ; memory to register
		mov      [RBP - 1792] , rax            ; register to memory
		pop_regs                    ; restore register values


		; Code for arithmetic
		push_regs                    ; save values
		mov     rax , [RBP - 1648]
		add     rax , [RBP - 1792]
		mov     [RBP - 1824] , rax
		pop_regs        ; restore register values


		push_regs                    ; save values
		xor      rax , rax           ; flush out the rax register
		mov      rax , [RBP - 1824]                    ; memory to register
		mov      [RBP - 32] , rax            ; register to memory
		pop_regs                    ; restore register values
>>>>>>> Stashed changes


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
<<<<<<< Updated upstream
=======
		; Code for printing output
		mov      rax , 1
		mov      rdi , 1
		mov      rsi , print_out
		mov      rdx , print_out_len
		syscall

		push_regs                                         ; save values

		push_regs                                         ; save values
		mov      rdi , fmt_spec_int_arr_out                  ; get corresponding format specifier
		mov      rsi , [RBP - 256]                        ; move source index
		xor      rax , rax
		rsp_align                                         ; align stack pointer
		call     printf                                   ; system call for output
		rsp_realign                                       ; restore previous alignment of stack
		pop_regs                                          ; restore values

		push_regs                                         ; save values
		mov      rdi , fmt_spec_int_arr_out                  ; get corresponding format specifier
		mov      rsi , [RBP - 288]                        ; move source index
		xor      rax , rax
		rsp_align                                         ; align stack pointer
		call     printf                                   ; system call for output
		rsp_realign                                       ; restore previous alignment of stack
		pop_regs                                          ; restore values

		push_regs                                         ; save values
		mov      rdi , fmt_spec_int_arr_out                  ; get corresponding format specifier
		mov      rsi , [RBP - 320]                        ; move source index
		xor      rax , rax
		rsp_align                                         ; align stack pointer
		call     printf                                   ; system call for output
		rsp_realign                                       ; restore previous alignment of stack
		pop_regs                                          ; restore values

		push_regs                                         ; save values
		mov      rdi , fmt_spec_int_arr_out                  ; get corresponding format specifier
		mov      rsi , [RBP - 352]                        ; move source index
		xor      rax , rax
		rsp_align                                         ; align stack pointer
		call     printf                                   ; system call for output
		rsp_realign                                       ; restore previous alignment of stack
		pop_regs                                          ; restore values

		push_regs                                         ; save values
		mov      rdi , fmt_spec_int_arr_out                  ; get corresponding format specifier
		mov      rsi , [RBP - 384]                        ; move source index
		xor      rax , rax
		rsp_align                                         ; align stack pointer
		call     printf                                   ; system call for output
		rsp_realign                                       ; restore previous alignment of stack
		pop_regs                                          ; restore values

		push_regs                                         ; save values
		mov      rdi , fmt_spec_int_arr_out_last                  ; get corresponding format specifier
		mov      rsi , [RBP - 416]                        ; move source index
		xor      rax , rax
		rsp_align                                         ; align stack pointer
		call     printf                                   ; system call for output
		rsp_realign                                       ; restore previous alignment of stack
		pop_regs                                          ; restore values
		; Display prompt for integer input
		mov      rax , 1
		mov      rdi , 1
		mov      rsi , printError
		mov      rdx , printError_len
		syscall
>>>>>>> Stashed changes
main_end:
		LEAVE
		retq