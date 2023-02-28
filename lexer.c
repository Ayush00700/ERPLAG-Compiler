#include "lexer.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*GLOBAL VARIABLES*/
int populate_count = 1;
int line_no = 1;

token_node* ll_node; //LINKED-LIST 
token_node* stack_head; //STACK FOR ERRORS 
token_node* current; //LAST NODE THAT WAS ADDED

void printtokens()
/*This function prints the token number, type and the corresponding lexeme*/
{
    token_node* temp = ll_node;
    int i=1; //token_number

    while(temp!=NULL){
        printf("%d line number ",temp->token->line_no);
        printf("token no. %d is %s .. lexeme is .. %s\n",i,temp->token->type,temp->token->lexeme);
        temp = temp->next_token;
        i++;
    }
}

void populate(char* buffer,FILE* fp,int bufsize)
/*This function populates the buffer with the data from file to be tokenized*/
{
    printf("We populated the buffer here %dth time\n",populate_count);
    populate_count++;
    if(fp!=NULL){
        memset(buffer, 0, bufsize);
        fread(buffer,sizeof(char),bufsize,fp);
    }
}

void add_error_token(token_info* tk)
/*This function adds an error entry into the stack*/
{
    // When stack is empty
    if(stack_head==NULL)
    {
        stack_head = (token_node*)malloc(sizeof(token_node));
        stack_head->token = tk;
        stack_head->next_token = NULL;
    }
    
    // Just add a node
    else{
        token_node* temp = (token_node*)malloc(sizeof(token_node));
        temp->token = tk;
        temp->next_token = stack_head;
        stack_head = temp;
    }
}

void pop_error_tokens()
/*This function pops an element from stack to display the error*/
{
    token_node* temp;
    temp = stack_head;
    while(temp!=NULL){
        printf("Error in line number :%d, Couldn't tokenize the keyword %s\n", temp->token->line_no,temp->token->lexeme);
        temp = temp->next_token;
    }
}

hash_table_contents* hash_table[TABLE_SIZE];

hash_table_contents* create_hash_table_content(char* lexeme, char* tk)
/*This function creates a hash table entry to map the token to the lexeme*/
{
    hash_table_contents* h = (hash_table_contents*)malloc(sizeof(hash_table_contents));
    // h->lexeme = lexeme;h->tk_type = (token_type)hash(lexeme);
    h->lexeme = lexeme;
    h->tk_type = tk;

    return h;
}


size_t hash(char *lexeme)
/*This function does the hashing for the given lexeme*/
{
    // strnlen takes 2 parameters: 1) string whose length is to be computed
    // 2) The max length to be checked
    int length = strnlen(lexeme, MAX_LEN);
    size_t  hash_value = 0;

    for (int i =0;i<length;i++)
    {
        hash_value += lexeme[i];
        hash_value = (hash_value*lexeme[i])%97;
    }

    return hash_value;
}

void init_hash_table()
/*This function initializes the hash table with NULL entries*/
{
    for(int i=0;i<TABLE_SIZE;i++){
        hash_table[i] = NULL;
    }
}

void print_hash_table()
/*This function prints the updated hash table*/
{
    for (int i=0;i<TABLE_SIZE;i++){
        // No entry found
        if(hash_table[i]==NULL)
        {
            printf("\t%d\t---\n",i);
        }
        // Entry found
        else
        {
            printf("\t%i\t%s\t%s\n", i ,hash_table[i]->lexeme, hash_table[i]->tk_type);
        }
    }
}


int hash_table_insert(char* lexeme, char* tk)
/*This function takes the lexeme and the token, hashes the lexeme and
returns a flag or index at which insertion is to be done*/
{
    if(lexeme==NULL)
        return 0;
    // Hash the lexeme
    int index = hash(lexeme);
    // Check for collision
    if(hash_table[index]!=NULL){
        printf("Element already existing at index %d\n",index);
        return 1;
    }
    // In case no collision, create entry and return index at which the entry is to be added
    else
    {
        hash_table_contents* h = create_hash_table_content(lexeme, tk);
        hash_table[index] = h;

        return index;
    }
}

void populate_hash_table()
/*This function populates the hash table to map the keywords lower case keywords to the ones in upper case*/
{
    // Initialize the hash table
    init_hash_table();
    for(int i=0;i<26;i++){

        hash_table_insert(arr_keywords[i],arr_keywords_upper[i]);
    
    }
}

char* lookup(char* lexeme){
    size_t hash_value = hash(lexeme);
    if (hash_value >= TABLE_SIZE || hash_table[hash_value]==NULL) return "ID";
    if(hash_table[hash_value]!=NULL){
        // printf("Found lexeme : %s\n", hash_table[hash_value]->tk_type);
        return hash_table[hash_value]->tk_type;
    }
}

void copy2lexeme(int f1,int f2,int b1,int b2,char* message,char* buf1,char* buf2,int bufsize){
    int size; //size of the string should be one more than the required because of '\0' character
    char* lexeme;
    if(b1==bufsize && f1==bufsize){ //reading from buffer 2 
        size = f2-b2;
        size++; //accounting for endline character 
        lexeme = (char *)malloc(sizeof(char)*size);
        for(int i=b2;i<f2;i++){
            lexeme[i-b2] = buf2[i];
        }
        lexeme[f2-b2] = '\0';
    }
    else if(b2==bufsize && f2==bufsize){ //reading from buffer 1
        size = f1-b1;
        size++;
        lexeme = (char *)malloc(sizeof(char)*size);
        for(int i=b1;i<f1;i++){
            lexeme[i-b1] = buf1[i];
        }
        lexeme[f1-b1] = '\0';
    }
    else if(b2==bufsize && f1==bufsize){ //first buffer 1 and then buffer 2
        size = 1;
        size += f2 + bufsize-b1;
        lexeme = (char *)malloc(sizeof(char)*size);
        for(int i=b1;i<bufsize;i++){
            lexeme[i-b1] = buf1[i];
        }
        for(int i=0;i<f2;i++){
            lexeme[bufsize-b1+i] = buf2[i];
        }
        lexeme[bufsize-b1+f2] = '\0';
    }
    else{ //first buffer 2 then buffer 1
        size = 1;
        size += f1+bufsize-b2;
        lexeme = (char *)malloc(sizeof(char)*size);
        for(int i=b2;i<bufsize;i++){
            lexeme[i-b2] = buf2[i];
        }
        for(int i=0;i<f1;i++){
            lexeme[bufsize-b2+i] = buf1[i];
        }
        lexeme[bufsize-b2+f1] = '\0';
    }
    token_info* tk ;
    if(!strcmp(message,"ERROR")){
        tk = (token_info *)malloc(sizeof(token_info));
        tk->lexeme = (char*)malloc(sizeof(char)*size);
        strncpy(tk->lexeme,lexeme,size);
        tk->line_no = line_no;
        memset(tk->type,'\0',20*sizeof(char));
        strcpy(tk->type,"ERROR");
    }
    else{
        tk = (token_info *)malloc(sizeof(token_info));
        tk->lexeme = (char*)malloc(sizeof(char)*size);
        strncpy(tk->lexeme,lexeme,size);
        tk->line_no = line_no;
        if(!strcmp(message,"NUM")){
            tk->values.num = atoi(tk->lexeme);
        }
        if(!strcmp(message,"RNUM")){
            double value = (double)atof(lexeme);
            tk->values.rnum = (double)atof(tk->lexeme);
        }
        if(!strcmp(message,"ID")){
            //TODO implement lookup --> DONE
            message = lookup(lexeme);
        }
        memset(tk->type,'\0',20*sizeof(char));
        strcpy(tk->type,message);
    }
    if(!strcmp(tk->type,"ERROR")){
        add_error_token(tk);
    }
    else if(ll_node == NULL){
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
//TODO Linked List to Stack conversion for error tokens --> DONE 

void call_lexer(FILE* fp,int bufsize){
    //TWIN BUFFER SYSTEM 
    char* buf1 = (char*)malloc(bufsize*sizeof(char));
    char* buf2 = (char*)malloc(bufsize*sizeof(char));
    fread(buf1,sizeof(char),bufsize,fp);// first populate the first buffer

    int b1,b2,f1,f2; // begin and forward pointers of the twin buffers

    b1 = 0;
    f1 = 0;
    b2 = bufsize; //not reading buffer2
    f2 = bufsize;

    int state = 1; //start state = 1
    line_no = 1; 
    char c; //current character to be read from the buffer  
    int hold = 0; //hold if 1 then hold the current forward pointer
    int enter = 0;
    int ender = 0;
    while(!ender){ //TODO Ends when? --> DONE
        if(hold == 0 && enter==1){
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
        enter = 1;
        hold = 0;
        if(f1 == bufsize){ //character to  be read from a particular buffer
            c = buf2[f2]; // if f1 == buffsize then you arent reading from the first buffer
        }
        else{
            c = buf1[f1]; 
        }
        if(c=='\000'){
            c = '\n';
            ender = 1;
        }
        switch(state){ //switch case- on the current state that drives the DFA
            case 1:
                if(f1==bufsize){ // first buffer is inactive
                    b2 = f2; //since we are in the start state --> move the begin pointer to the forward
                    b1 = bufsize;
                }
                else{
                    b1 = f1;
                    b2 = bufsize;
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
                else if(c == ' ' || c == '\t' || c == '\r') state=27;
                else if((c>=65 && c<=90) || (c>=97 && c<=122) || c==95) state=28;
                else if(c>='0' && c<='9') state=29;
                else if(c == '*') state=36;
                else{
                    state = 40; 
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
                    copy2lexeme(f1,f2,b1,b2,"ERROR",buf1,buf2,bufsize);
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
                    copy2lexeme(f1,f2,b1,b2,"ERROR",buf1,buf2,bufsize);
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
                    copy2lexeme(f1,f2,b1,b2,"ERROR",buf1,buf2,bufsize);
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
                if(c==' '||c=='\t' || c=='\r') state = 27;
                else{
                    state = 1;
                    hold = 1;
                }
                break;
            case 28:
                if((c>=65 && c<=90) || (c>=97 && c<=122) || c==95 || (c>='0'&&c<='9')) state=28;
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
                    copy2lexeme(f1,f2,b1,b2,"ERROR",buf1,buf2,bufsize);
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
                    copy2lexeme(f1,f2,b1,b2,"ERROR",buf1,buf2,bufsize);
                    state = 1;
                    hold = 1;
                }
                break;
            case 34:
                if(c>='0'&&c<='9') state = 35;
                else{
                    copy2lexeme(f1,f2,b1,b2,"ERROR",buf1,buf2,bufsize);
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
            case 40:
                copy2lexeme(f1,f2,b1,b2,"ERROR",buf1,buf2,bufsize);
                state = 1;
                hold = 1;

        }
    }
}

/*This is the driver function to test tokenization*/
int main(){
    FILE* fp;
    fp = fopen("code_test_case3.txt","r");
    populate_hash_table();

    // char* source;
    // source = "   abc:=b/5+c;";
    // if(source[3]==' ') printf("s%cs s",source[0]);

    call_lexer(fp,4096);
    printf("TOKENS ARE .... (first to last) \n");
    printtokens();
    printf("ERRORS ARE .... (last to first) \n");
    pop_error_tokens();
}
