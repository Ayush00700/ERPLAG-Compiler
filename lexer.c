#include "lexer.h"
#include <stdio.h>
#include <string.h>

int line_no = 1;

token_node* ll_node;
token_node* current;

void populate(char* buffer,FILE* fp,int bufsize){
    if(fp!=NULL){
        fread(buffer,sizeof(char),bufsize,fp);
    }
}

void copy2lexeme(int f1,int f2,int b1,int b2,char* message,char* buf1,char* buf2,int bufsize){
    int size;
    char* lexeme;
    if(b1==bufsize && f1==bufsize){
        size = f2-b2;
        lexeme = (char *)malloc(sizeof(char)*size);
        for(int i=b2;i<f2;i++){
            lexeme[i-b2] = buf2[i];
        }
    }
    else if(b2==bufsize && f2==bufsize){
        size = f1-b1;
        lexeme = (char *)malloc(sizeof(char)*size);
        for(int i=b1;i<f1;i++){
            lexeme[i-b1] = buf1[i];
        }
    }
    else if(b2==bufsize && f1==bufsize){
        size = 0;
        size += f2;
        size += bufsize-b1;
        lexeme = (char *)malloc(sizeof(char)*size);
        for(int i=b1;i<bufsize;i++){
            lexeme[i-b1] = buf1[i];
        }
        for(int i=0;i<f2;i++){
            lexeme[bufsize-b1+i] = buf2[i];
        }
    }
    else{
        size = 0;
        size += f1;
        size += bufsize-b2;
        lexeme = (char *)malloc(sizeof(char)*size);
        for(int i=b2;i<bufsize;i++){
            lexeme[i-b2] = buf2[i];
        }
        for(int i=0;i<f1;i++){
            lexeme[bufsize-b2+i] = buf1[i];
        }
    }
    token_info* tk ;
    if(strcmp(message,"error") == 0){
        tk = (token_info *)malloc(sizeof(token_info));
        strncpy(tk->lexeme,lexeme,size);
        tk->line_no = line_no;
        tk->type = "ERROR";
    }
    else{
        tk = (token_info *)malloc(sizeof(token_info));
        strncpy(tk->lexeme,lexeme,size);
        tk->line_no = line_no;
        if(strcmp(message,"NUM")){
            tk->values.num = atoi(lexeme);
        }
        if(strcmp(message,"RNUM")){
            tk->values.rnum = atof(lexeme);
        }
        if(strcmp(message,"ID")){
            //TODO implement lookup 
        }
        else{
            tk->type = message; //TODO Resolve
        }
    }
    if(ll_node == NULL){
        ll_node = (token_node*)malloc(sizeof(token_node));
        ll_node->token = tk;
        ll_node->next_token = NULL;
        current = ll_node;
    }
    else{
        token_node* temp = (token_node *)malloc(sizeof(token_node));
        current->next_token = temp;
        temp->token = tk;
        temp->next_token = NULL;
        current = temp;
    }
}
//TODO Linked List to Stack conversion for error tokens

int lexer(FILE* fp,int bufsize){
    //TWIN BUFFER SYSTEM 
    char* buf1 = (char*)malloc(bufsize*sizeof(char));
    char* buf2 = (char*)malloc(bufsize*sizeof(char));
    fread(&buf1,sizeof(char),bufsize,fp);// first populate the first buffer

    int b1,b2,f1,f2; // begin and forward pointers of the twin buffers

    b1 = 0;
    f1 = 0;
    b2 = bufsize; //not reading buffer2
    f2 = bufsize;

    int state = 1; //start state = 1
    line_no = 1; 
    char c; //current character to be read from the buffer  
    int hold = 0; //hold if 1 then hold the current forward pointer
    while(1){ //TODO Ends when?
        if(hold == 0){
            if(f1 == bufsize){
                f2++;
                if(f2 == bufsize){
                    f1=0;
                    populate(buf1,fp,bufsize);
                }
            }
            else{
                f1++;
                if(f1 == bufsize){
                    f2 = 0;
                    populate(buf2,fp,bufsize);
                }
            }
        }
        hold = 0;
        if(f1 == bufsize){ //character to  be read from a particular buffer
            c = buf2[f2]; // if f1 == buffsize then you arent reading from the first buffer
        }
        else{
            c = buf1[f1]; 
        }
        switch(state){ //switch case- on the current state that drives the DFA
            case 1:
                if(f1==bufsize){ // first buffer is inactive
                    b2 = f2; //since we are in the start state --> move the begin pointer to the forward
                }
                else{
                    b1 = f1;
                }
                if(c == '\n'){
                    line_no++; //adding line no
                    state=1; //continuing in the same state owing to newline encounter
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
                    copy2lexeme(f1,f2,b1,b2,"error",buf1,buf2,bufsize); 
                }
                break;
            case 2:
                copy2lexeme(f1,f2,b1,b2,"PLUS",buf1,buf2,bufsize);
                state = 1; //going back to the start state
                hold = 1; 
                /*     '+'   
                            ^
                            |
                */
               break;
            case 3:
                copy2lexeme(f1,f2,b1,b2,"MINUS",buf1,buf2,bufsize);
                state = 1;
                hold = 1;
                break;
            case 4:
                copy2lexeme(f1,f2,b1,b2,"DIV",buf1,buf2,bufsize);
                state = 1;
                hold = 1;
                break;
            case 5:
                if(c == '=') state=6;
                else if(c == '<') state=7;
                else{
                    copy2lexeme(f1,f2,b1,b2,"LT",buf1,buf2,bufsize);
                    state = 1;
                    hold = 1;
                }
                break;
            case 6:
                copy2lexeme(f1,f2,b1,b2,"LE",buf1,buf2,bufsize);
                state = 1;
                hold = 1;
                break;
            case 7:
                if(c == '<') state = 8;
                else{
                    copy2lexeme(f1,f2,b1,b2,"DEF",buf1,buf2,bufsize);
                    state = 1;
                    hold = 1;
                }
                break;
            case 8:
                copy2lexeme(f1,f2,b1,b2,"DRIVERDEF",buf1,buf2,bufsize);
                state = 1;
                hold = 1;
                break;
            case 9:
                if(c=='>') state=11;
                else if(c=='=') state=10;
                else{
                    copy2lexeme(f1,f2,b1,b2,"GT",buf1,buf2,bufsize);
                    state = 1;
                    hold = 1;
                }
                break;
            case 10:
                copy2lexeme(f1,f2,b1,b2,"GE",buf1,buf2,bufsize);
                state = 1;
                hold = 1;
                break;
            case 11:
                if(c=='>') state=12;
                else{
                    copy2lexeme(f1,f2,b1,b2,"ENDDEF",buf1,buf2,bufsize);
                    state = 1;
                    hold = 1; 
                }
                break;
            case 12:
                copy2lexeme(f1,f2,b1,b2,"DRIVERENDDEF",buf1,buf2,bufsize);
                state = 1;
                hold = 1;
                break;
            case 13:
                if(c == '=') state = 14;
                else{
                    copy2lexeme(f1,f2,b1,b2,"error",buf1,buf2,bufsize);
                    state = 1;
                    hold = 1;
                }
                break;
            case 14:
                copy2lexeme(f1,f2,b1,b2,"EQ",buf1,buf2,bufsize);
                state = 1;
                hold = 1;
                break;
            case 15:
                if(c == '=') state = 16;
                else{
                    copy2lexeme(f1,f2,b1,b2,"error",buf1,buf2,bufsize);
                    state = 1;
                    hold = 1;
                }
                break;
            case 16:
                copy2lexeme(f1,f2,b1,b2,"NE",buf1,buf2,bufsize);
                state = 1;
                hold = 1;
                break;
            case 17:
                if(c=='=') state = 18;
                else{
                    copy2lexeme(f1,f2,b1,b2,"COLON",buf1,buf2,bufsize);
                    state = 1;
                    hold = 1;
                }
                break;
            case 18:
                copy2lexeme(f1,f2,b1,b2,"ASSIGNOP",buf1,buf2,bufsize);
                state = 1;
                hold = 1;
                break;
            case 19:
                copy2lexeme(f1,f2,b1,b2,"SEMICOL",buf1,buf2,bufsize);
                state = 1;
                hold = 1;
                break;
            case 20:
                copy2lexeme(f1,f2,b1,b2,"COMMA",buf1,buf2,bufsize);
                state = 1;
                hold = 1;
                break;
            case 21:
                copy2lexeme(f1,f2,b1,b2,"SQBO",buf1,buf2,bufsize);
                state = 1;
                hold = 1;
                break;
            case 22:
                copy2lexeme(f1,f2,b1,b2,"SQBC",buf1,buf2,bufsize);
                state = 1;
                hold = 1;
                break;
            case 23:
                copy2lexeme(f1,f2,b1,b2,"BO",buf1,buf2,bufsize);
                state = 1;
                hold = 1;
                break;
            case 24:
                copy2lexeme(f1,f2,b1,b2,"BC",buf1,buf2,bufsize);
                state = 1;
                hold = 1;
                break;
            case 25:
                if(c == '.') state = 26;
                else{
                    copy2lexeme(f1,f2,b1,b2,"error",buf1,buf2,bufsize);
                    state = 1;
                    hold = 1;
                }
                break;
            case 26:
                copy2lexeme(f1,f2,b1,b2,"RANGEOP",buf1,buf2,bufsize);
                state = 1;
                hold = 1;
                break;
            case 27:
                if(f1 == bufsize){
                    b1 = bufsize;
                    b2 = f2;
                }
                else{
                    b2 = bufsize;
                    b1 = f1;
                }
                if(c==' '||c=='\t') state = 27;
                else{
                    state = 1;
                    hold = 1;
                }
                break;
            case 28:
                if((c>=65 && c<=90) || (c>=61 && c<=122) || c==95 || (c>='0'&&c<='9')) state=28;
                else{
                    copy2lexeme(f1,f2,b1,b2,"ID",buf1,buf2,bufsize);
                    state = 1;
                    hold=1;
                }
                break;
            case 29:
                if(c>='0'&&c<='9') state = 29;
                else if(c == '.') state = 30;
                else{
                    copy2lexeme(f1,f2,b1,b2,"NUM",buf1,buf2,bufsize);
                    state = 1;
                    hold = 1;
                }
                break;
            case 30:
                if(c=='.') {
                    state = 31;
                    hold = 1;
                }
                else if(c>='0'&&c<='9') state = 32;
                else{
                    copy2lexeme(f1,f2,b1,b2,"error",buf1,buf2,bufsize);
                    state = 1;
                    hold = 1;
                }
                break;
            case 31:
                if(f1 == bufsize){
                    if(f2==0){
                        f2 = bufsize;
                        f1 = bufsize-1;
                        copy2lexeme(f1,f2,b1,b2,"NUM",buf1,buf2,bufsize);
                        state = 1;
                        hold = 1;
                    }
                    else{
                        f2--;
                        copy2lexeme(f1,f2,b1,b2,"NUM",buf1,buf2,bufsize);
                        state = 1;
                        hold = 1;
                    }
                }
                else{
                    if(f1==0){
                        f1 = bufsize;
                        f2 = bufsize-1;
                        copy2lexeme(f1,f2,b1,b2,"NUM",buf1,buf2,bufsize);
                        state = 1;
                        hold = 1;
                    }
                    else{
                        f1--;
                        copy2lexeme(f1,f2,b1,b2,"NUM",buf1,buf2,bufsize);
                        state = 1;
                        hold = 1;
                    }
                }
                break;
            case 32:
                if(c>='0'&&c<='9') state = 32;
                else if(c =='E' || c=='e') state = 33;
                else{
                    copy2lexeme(f1,f2,b1,b2,"RNUM",buf1,buf2,bufsize);
                    state = 1;
                    hold = 1;
                }
                break;
            case 33:
                if(c>='0'&&c<='9') state = 35;
                else if(c=='+'||c=='-') state = 34;
                else{
                    copy2lexeme(f1,f2,b1,b2,"error",buf1,buf2,bufsize);
                    state = 1;
                    hold = 1;
                }
                break;
            case 34:
                if(c>='0'&&c<='9') state = 35;
                else{
                    copy2lexeme(f1,f2,b1,b2,"error",buf1,buf2,bufsize);
                    state = 1;
                    hold = 1;
                }
                break;
            case 35:
                if(c>='0'&&c<='9') state = 35;
                else{
                    copy2lexeme(f1,f2,b1,b2,"RNUM",buf1,buf2,bufsize);
                    state = 1;
                    hold = 1;
                }
                break;
            case 36:
                if(c=='*') state = 37;
                else{
                    copy2lexeme(f1,f2,b1,b2,"MUL",buf1,buf2,bufsize);
                    state = 1;
                    hold = 1;
                }
                break;
            case 37:
                if(c=='*') state = 38;
                else{
                    if(c=='\n') line_no++;
                    state = 37;
                }
                break;
            case 38:
                if(c=='*') state = 39;
                else{
                    if(c=='\n') line_no++;
                    state = 37;
                }
                break;
            case 39:
                state = 1;
                hold = 1;
                break;
        }
    }
}




