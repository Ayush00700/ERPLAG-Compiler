#  THE MATH_CS boiz
# --------------------
# 1. Rajan Sahu       2019B4A70572P
# 2. Yash Goyal       2019B4A70638P
# 3. Ayush Agarwal    2019B4A70652P
# 4. Vasu Swaroop     2019B4A70656P
# 5. A Sudarshan      2019B4A70744P

stage1exe: lexer.o parser.o driver.o
	gcc lexer.o parser.o driver.o -o stage1exe

driver.o: driver.c
	gcc -c driver.c

lexer.o: lexer.c lexer.h
	gcc -c lexer.c

parser.o: parser.c parser.h
	gcc -c parser.c

driver.o: driver.c
	gcc -c driver.c
	
clean:
	rm *.o stage1exe

