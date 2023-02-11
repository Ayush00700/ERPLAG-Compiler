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
    int line_no = 1;
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
                if(c == '\n'){
                    line_no++;
                    state=1;
                }
                else if(c == '+') state=2;
                else if(c == '-') state=3;
                else if(c == '/') state=4;
                else if(c == '<') state=5;
                else if(c == '>') state=9;
                else if(c == '=') state=13;
                else if(c == '!') state=15;
                else if(c == ':') state=17;
                else if(c == ';') state=19;
                else if(c == ',') state=20;
                else if(c == '[') state=21;
                else if(c == ']') state=22;
                else if(c == '(') state=23;
                else if(c == ')') state=24;
                else if(c == '.') state=25;
                else if(c == ' ' || c == '\t') state=27;
                else if((c>=65 && c<=90) || (c>=61 && c<=122) || c==95) state=28;
                else if(c>='0' && c<='9') state=29;
                else if(c == '*') state=36;
                else{
                    copy2lexeme(f1,f2,b1,b2,"error",buf1,buf2);
                }
            case 2:
            default:
                if(f1 == bufsize){
                    f2++;
                    if(f2 == bufsize){
                        f1=0;
                        populate(buf1);
                    }
                }
                else{
                    f1++;
                    if(f1 == bufsize){
                        f2 = 0;
                        populate(buf2);
                    }
                }
                
        }
    }
}




