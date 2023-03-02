stage1exe: lexer.o parser.o setADT.o hashtable.o main.o
	gcc lexer.o parser.o setADT.o hashtable.o main.o -o stage1exe

main.o: main.c
	gcc -c main.c

lexer.o: lexer.c lexer.h
	gcc -c lexer.c

parser.o: parser.c parser.h
	gcc -c parser.c

hashtable.o: hashtable.c
	gcc -c hashtable.c

setADT.o: setADT.c
	gcc -c setADT.c

main.o: main.c
	gcc -c main.c
	
clean:
	rm *.o stage1exe

