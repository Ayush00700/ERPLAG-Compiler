#  Group-20
# --------------------
# 1. Rajan Sahu       2019B4A70572P
# 2. Yash Goyal       2019B4A70638P
# 3. Ayush Agarwal    2019B4A70652P
# 4. Vasu Swaroop     2019B4A70656P
# 5. A Sudarshan      2019B4A70744P

stage1exe: lexer.o parser.o driver.o ast.o
	gcc lexer.o parser.o driver.o ast.o -o stage2exe

driver.o: driver.c
	gcc -c driver.c

lexer.o: lexer.c lexer.h
	gcc -c lexer.c

parser.o: parser.c parser.h
	gcc -c parser.c

driver.o: driver.c
	gcc -c driver.c
	
ast.o: ast.c
	gcc -c ast.c	
clean:
	rm *.o stage2exe

