; Group-20
; --------------------
; 1. Rajan Sahu       2019B4A70572P
; 2. Yash Goyal       2019B4A70638P
; 3. Ayush Agarwal    2019B4A70652P
; 4. Vasu Swaroop     2019B4A70656P
; 5. A Sudarshan      2019B4A70744P
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
		global main
main:
