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
		fmt_spec_int_in: db "%hd", 0
		fmt_spec_int_out: db "%hd", 10, 0
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
		ENTER    1856,0
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


L1:
		; Code for relational
		push_regs                    ; save values
		mov     rax , [RBP - 464]
		mov     rbx , 15
		cmp     rax , rbx
		jle     L7
		mov     qword [RBP - 560], 0
		jmp  L8
L7:
		mov     qword [RBP - 560], 1
L8:
		pop_regs        ; restore register values


		mov      rax , [RBP - 560]
		cmp      rax , 1
		je       L2
		jmp      L3
L2:
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
				mov RDX, RBP
                sub RDX, 528     ; make RDX to point at location of variable on the stack
                mov RAX, 0x0000_0000_0000_0000 ;machine has sizeof(int) to be 2, for uniformity, We are taking
                mov [RDX] , RAX ;our sizeof(int) to be 4, now scanf will just enter values in lower 32 bits
                mov RSI, RBP
                sub RSI, 528 
                mov RAX, 0 
                rsp_align ;align RSP to 16 byte boundary for scanf call
                call scanf 
                rsp_realign ;realign it to original position
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


		jmp      L1
L3:
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
		jg     L9
		mov     qword [RBP - 1136], 0
		jmp  L10
L9:
		mov     qword [RBP - 1136], 1
L10:
		pop_regs        ; restore register values


		; Code for relational
		push_regs                    ; save values
		mov     rax , [RBP - 128]
		mov     rbx , [RBP - 160]
		cmp     rax , rbx
		jle     L11
		mov     qword [RBP - 1152], 0
		jmp  L12
L11:
		mov     qword [RBP - 1152], 1
L12:
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
		jl     L13
		mov     qword [RBP - 1184], 0
		jmp  L14
L13:
		mov     qword [RBP - 1184], 1
L14:
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
		jne      L15
		mov      rax , 1
		mov      rdi , 1
		mov      rsi , false
		mov      rdx , false_len
		syscall
		pop_regs                                          ; restore register values
		jmp  L16


L15:
		mov      rax , 1
		mov      rdi , 1
		mov      rsi , true
		mov      rdx , true_len
		syscall
		pop_regs        ; restore register values
L16:


		; Code for getting user input
		push_regs                    ; save values
				mov RDX, RBP
                sub RDX, 256     ; make RDX to point at location of variable on the stack
                mov RAX, 0x0000_0000_0000_0000 ;machine has sizeof(int) to be 2, for uniformity, We are taking
                mov [RDX] , RAX ;our sizeof(int) to be 4, now scanf will just enter values in lower 32 bits
                mov RSI, RBP
                sub RSI, 256 
                mov RAX, 0 
                rsp_align ;align RSP to 16 byte boundary for scanf call
                call scanf 
                rsp_realign ;realign it to original position
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
		jl     L17
		mov     qword [RBP - 1264], 0
		jmp  L18
L17:
		mov     qword [RBP - 1264], 1
L18:
		pop_regs        ; restore register values


		; Code for relational
		push_regs                    ; save values
		mov     rax , 13            ; immediate to memory
		mov     rbx , 15
		cmp     rax , rbx
		jg     L19
		mov     qword [RBP - 1280], 0
		jmp  L20
L19:
		mov     qword [RBP - 1280], 1
L20:
		pop_regs        ; restore register values


		; Code for logical op
		push_regs                    ; save values
		mov     rax , [RBP - 1264]
		or     rax , [RBP - 1280]
		mov     [RBP - 1296] , rax
		pop_regs        ; restore register values


		mov      rax , [RBP - 1296]
		cmp      rax , 1
		je       L4
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
		jl     L21
		mov     qword [RBP - 1472], 0
		jmp  L22
L21:
		mov     qword [RBP - 1472], 1
L22:
		pop_regs        ; restore register values


		; Code for relational
		push_regs                    ; save values
		mov     rax , 14            ; immediate to memory
		mov     rbx , 15
		cmp     rax , rbx
		jg     L23
		mov     qword [RBP - 1488], 0
		jmp  L24
L23:
		mov     qword [RBP - 1488], 1
L24:
		pop_regs        ; restore register values


		; Code for logical op
		push_regs                    ; save values
		mov     rax , [RBP - 1472]
		or     rax , [RBP - 1488]
		mov     [RBP - 1504] , rax
		pop_regs        ; restore register values


		mov      rax , [RBP - 1504]
		cmp      rax , 1
		je       L5
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
		jl     L25
		mov     qword [RBP - 1680], 0
		jmp  L26
L25:
		mov     qword [RBP - 1680], 1
L26:
		pop_regs        ; restore register values


		; Code for relational
		push_regs                    ; save values
		mov     rax , 15            ; immediate to memory
		mov     rbx , 15
		cmp     rax , rbx
		jg     L27
		mov     qword [RBP - 1696], 0
		jmp  L28
L27:
		mov     qword [RBP - 1696], 1
L28:
		pop_regs        ; restore register values


		; Code for logical op
		push_regs                    ; save values
		mov     rax , [RBP - 1680]
		or     rax , [RBP - 1696]
		mov     [RBP - 1712] , rax
		pop_regs        ; restore register values


		mov      rax , [RBP - 1712]
		cmp      rax , 1
		je       L6
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
main_end:
LEAVE
		retq