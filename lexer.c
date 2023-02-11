#include "lexer.h"
#include <stdio.h>

int lexer(FILE* fp,int bufsize){
    char* buf1 = (char*)malloc(bufsize*sizeof(char));
    char* buf2 = (char*)malloc(bufsize*sizeof(char));
    fread(&buf1,sizeof(char),bufsize,fp);
    int b1,b2,f1,f2;
    b1 = 0;
    f1 = 0;
    b2 = bufsize;
    f2 = bufsize;
    int state = 1;
    while(1){
        char c;
        if(f1 == bufsize){
            c = buf2[f2];
        }
        else{
            c = buf1[f1]; 
        }
        switch(state){
            case 1:
                if(c == '\n') state=1;
                else if(c == '+') state=2;
                else if(c == '-') state=3;
                
                
        }
    }
}




