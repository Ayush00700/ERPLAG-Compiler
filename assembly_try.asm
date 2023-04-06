extern printf, scanf, exit
section      .data
		fmt_spec_int: db "%d", 10, 0
		fmt_spec_real: db "%4f", 10, 0
		fmt_spec_string: db "%s", 10, 0
		zero: dw 0
section      .text
global main