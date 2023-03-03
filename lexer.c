/* Group-20
--------------------
1. Rajan Sahu       2019B4A70572P
2. Yash Goyal       2019B4A70638P
3. Ayush Agarwal    2019B4A70652P
4. Vasu Swaroop     2019B4A70656P
5. A Sudarshan      2019B4A70744P
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "lexer.h"

/* IMPORTANT GLOBAL VARIABLES FOR LEXER MODULE */
int populate_count = 1;
int line_no = 1;
token_node* ll_node; //LINKED-LIST 
token_node* stack_head; //STACK FOR ERRORS 
token_node* current; //LAST NODE THAT WAS ADDED
token_node* get_next_curr;
hash_table_contents* hash_table[TABLE_SIZE];
// #endif




char* arr_keywords[30] = {
"integer","real","boolean","of",
"array","start","end","declare",
"module","driver","program","get_value",
"print","use","with","parameters","takes",
"input","returns","for","in","switch",
"case","break","default","while","true","false","AND","OR"
};

char* arr_keywords_upper[30] = {
"INTEGER","REAL","BOOLEAN","OF",
"ARRAY","START","END","DECLARE",
"MODULE","DRIVER","PROGRAM","GET_VALUE",
"PRINT","USE","WITH","PARAMETERS","TAKES",
"INPUT","RETURNS","FOR","IN","SWITCH",
"CASE","BREAK","DEFAULT","WHILE","TRUE","FALSE","AND","OR"
};

void printtokens()
/*This function prints the token number, type and the corresponding lexeme*/
{
    token_node* temp = ll_node;
    int i=1; //token_number
    printf("Line_number          lexeme           Token_name\n"); 
    while(temp!=NULL){
        printf("%d\t\t\t",temp->token->line_no);
        printf("%s\t\t\t%s\n",temp->token->lexeme,temp->token->type);
        temp = temp->next_token;
        i++;
    }
}
void initialize()
/*Initialize next pointer for get_next_token()*/
{
    get_next_curr = ll_node;
}

token_info* get_next_token()
/*This function returns the next valid token to the parser upon request*/
{
    // When there are no more tokens
    if(get_next_curr==NULL){
        return NULL;
    }

    // Populate required fields and send the token to parser
    token_node* new_node =(token_node*) malloc(sizeof(token_node));
    new_node->token = (token_info*)malloc(sizeof(token_info));
    new_node->token->lexeme = (char*)malloc(sizeof(char)*20);
    strcpy(new_node->token->lexeme,get_next_curr->token->lexeme);
    new_node->token->line_no = get_next_curr->token->line_no;
    strcpy(new_node->token->type,get_next_curr->token->type);
    new_node->token->values = get_next_curr->token->values;

    get_next_curr =get_next_curr->next_token;
    return new_node->token;
}


void populate(char* buffer,FILE* fp,int bufsize)
/*This function populates the buffer with the data from file to be tokenized*/
{
    printf("We populated the buffer here %dth time\n",populate_count);
    populate_count++;
    if(fp!=NULL){
        // Flush buffer
        memset(buffer, 0, bufsize);
        // Read file to fill buffer
        fread(buffer,sizeof(char),bufsize,fp);
    }
}

void add_error_token(token_info* tk)
/*This function adds a lexical error entry into the stack*/
{
    // If stack is empty then initialize
    if(stack_head==NULL){
        stack_head = (token_node*)malloc(sizeof(token_node));
        stack_head->token = tk;
        stack_head->next_token = NULL;
    }
    
    // Push node
    else
    {
        token_node* temp = (token_node*)malloc(sizeof(token_node));
        temp->token = tk;
        token_node* temp1 = stack_head;
        while(temp1->next_token != NULL){
            temp1 = temp1->next_token;
        }
        temp1->next_token = temp;
    }
}

void pop_error_tokens()
/*This function pops an element from stack to display the error*/
{
    token_node* temp;
    temp = stack_head;
    int i=0;
    // Print error entries
     printf("Line_number          Keyword(that caused the error):\n"); 
    while(temp!=NULL){
        i++;
        printf("%d\t\t\t%s\n", temp->token->line_no,temp->token->lexeme);
        temp = temp->next_token;
    }
    if(i==0){
        printf("No Errors Found!\n");
    }   
}

/*THE HASHING IS PART OF THE PRE-PROCESSING STEP*/
hash_table_contents* create_hash_table_content(char* lexeme, char* tk)
/*This function creates a hash table entry to map the token to the lexeme*/
{
    hash_table_contents* h = (hash_table_contents*)malloc(sizeof(hash_table_contents));
    // h->lexeme = lexeme;h->tk_type = (token_type)hash(lexeme);
    h->lexeme = lexeme;
    h->tk_type = tk;
    h->next = NULL;
    return h;
}


size_t _hash(char *lexeme)
/*This function does the hashing for the given lexeme*/
{

    int length = strnlen(lexeme, MAX_LEN);
    size_t  hash_value = 0;
    for (int i =0;i<length;i++){
        hash_value += lexeme[i];
        hash_value = (hash_value*lexeme[i])%97;
    }

    return hash_value%TABLE_SIZE;
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
        if(hash_table[i]==NULL){
            printf("\t%d\t---\n",i);
        }else {
            hash_table_contents* entry = hash_table[i];
            while(entry!=NULL){
                printf("\t%i\t%s\t%s", i ,entry->lexeme, entry->tk_type);
                entry = entry->next;
            }
        }
    }
}

int hash_table_insert(char* lexeme, char* tk)
/*This function takes the lexeme and the token, hashes the lexeme and
returns a flag or index at which insertion is to be done*/
{
    if(lexeme==NULL)
        return 0;

    int index = _hash(lexeme);
    hash_table_contents* entry = hash_table[index];

    // Check until you get an empty cell to add (part of linear probing)
    while(entry!=NULL){
        if(strcmp(entry->lexeme,lexeme)==0){
            return 0;
        }
        entry = entry->next;
    }
    // Now that we have the index, we create hash table entry to be added
    hash_table_contents* h = create_hash_table_content(lexeme, tk);
    h->next = hash_table[index];
    hash_table[index] = h;
    return index;
}

void populate_hash_table()
/*This function populates the hash table to map the keywords lower case keywords to the ones in upper case*/
{
    init_hash_table();
    for(int i=0;i<TOTAL_KEYWORDS;i++){
        hash_table_insert(arr_keywords[i],arr_keywords_upper[i]);
    }
}

char* lookup(char* lexeme)
/*This function takes an input lexeme and returns the corresponding mapped value from the hash table*/
{
    size_t hash_value = _hash(lexeme);
    hash_table_contents* entry = hash_table[hash_value];
    int found = 0;

    // Find a match :(
    while(entry!=NULL){
        if(strcmp(entry->lexeme,lexeme)==0){
            found = 1;
            return entry->tk_type;
        }
        entry = entry->next;
    }

    // If not found, return ID
    return "ID";
}

/*HERE WE FOLLOW THE BUFFER AND SIMULTE IT ON THE DFA*/
void copy2lexeme(int f1,int f2,int b1,int b2,char* message,char* buf1,char* buf2,int bufsize)
/*fi, bi: Forward and begin pointer of buffer i respectively
messgae: The lexeme to which we have to map the string read in buffer as instructed by the DFA*/
{
    int size; //size of the string should be one more than the required because of '\0' character
    char* lexeme;

    // Tokenizing from buffer 2
    if(b1==bufsize && f1==bufsize){
        size = f2-b2;
        size++; //accounting for endline character 
        lexeme = (char *)malloc(sizeof(char)*size);
        for(int i=b2;i<f2;i++){
            lexeme[i-b2] = buf2[i];
        }
        lexeme[f2-b2] = '\0';
    }

    // Tokenizing from buffer 1
    else if(b2==bufsize && f2==bufsize){
        size = f1-b1;
        size++;
        lexeme = (char *)malloc(sizeof(char)*size);
        for(int i=b1;i<f1;i++){
            lexeme[i-b1] = buf1[i];
        }
        lexeme[f1-b1] = '\0';
    }

    // Tokenizing from buffer 1 and then buffer 2
    else if(b2==bufsize && f1==bufsize){
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

    // Tokenizing from buffer 2 then buffer 1
    else{
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
    token_info* tk;
    /*IN THE FOLLOWING STEP, WE POPULATE THE token_info OBJECT BASED ON THE MESSAGE*/
    int flag = 0;
    if(size>20){
        flag = 1;
    }

    // If message is "ERROR", we have to map that and add entry to stack
    if(!strcmp(message,"ERROR") || flag){
        tk = (token_info *)malloc(sizeof(token_info));
        tk->lexeme = (char*)malloc(sizeof(char)*size);
        strncpy(tk->lexeme,lexeme,size);
        tk->line_no = line_no;
        memset(tk->type,'\0',20*sizeof(char));
        strcpy(tk->type,"ERROR");
    }

    // If not an ERROR
    else{

        /*TOKENIZATION*/
        tk = (token_info *)malloc(sizeof(token_info));
        tk->lexeme = (char*)malloc(sizeof(char)*size);
        strncpy(tk->lexeme,lexeme,size);
        tk->line_no = line_no;
        if(!strcmp(message,"NUM")){
            tk->values.num = atoi(tk->lexeme);
        }
        else if(!strcmp(message,"RNUM")){
            double value = (double)atof(lexeme);
            tk->values.rnum = (double)atof(tk->lexeme);
        }
        else if(!strcmp(message,"ID")) message = lookup(lexeme);

        memset(tk->type,'\0',20*sizeof(char));
        strcpy(tk->type,message);
    }

    // Although error, we tokenize but push in error stack
    if(!strcmp(tk->type,"ERROR")){
        add_error_token(tk);
    }

    // When linked list of tokens is empty, create a node (doesn't involve error tokens)
    else if(ll_node == NULL){
        ll_node = (token_node*)malloc(sizeof(token_node));
        ll_node->token = tk;
        ll_node->next_token = NULL;
        current = ll_node;
    }

    // Else, just extend the list
    else{
        token_node* temp = (token_node *)malloc(sizeof(token_node));
        current->next_token = temp;
        temp->token = tk;
        temp->next_token = NULL;
        current = temp;
    }
}
//TODO Linked List to Stack conversion for error tokens --> DONE 

void call_lexer(FILE* fp,int bufsize)
/*This function gives the cue for Lexical Analysis*/
{
    //TWIN BUFFER SYSTEM 
    char* buf1 = (char*)malloc(bufsize*sizeof(char));
    char* buf2 = (char*)malloc(bufsize*sizeof(char));
    fread(buf1,sizeof(char),bufsize,fp);// first populate the first buffer
    ll_node = NULL; //LINKED-LIST 
    stack_head = NULL; //STACK FOR ERRORS 
    current = NULL; //LAST NODE THAT WAS ADDED
    get_next_curr = NULL;

    int b1,b2,f1,f2; // begin and forward pointers of the twin buffers

    b1 = 0;
    f1 = 0;
    b2 = bufsize; //not reading buffer2
    f2 = bufsize;

    /*DFA Encoding*/
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

void removeComments(char *testcaseFile, char *cleanFile)
{
    FILE* f1 = fopen(testcaseFile,"r");
    if(f1 == NULL)
    {
        printf("Error opening file:%s!\n",testcaseFile);   
        exit(1);             
    }

    FILE* f2 = fopen(cleanFile,"w+");
    if(f2 == NULL)
    {
        printf("Error opening file:%s!\n",cleanFile);   
        exit(1);             
    }

    char ch = fgetc(f1);
    int commentFlag =0;
    while(ch!=EOF){
        char nextChar = fgetc(f1);
        if(ch=='*' && nextChar=='*'){
            commentFlag = commentFlag==0?1:0;
            ch = fgetc(f1);
            continue;
        }
        if(commentFlag==0){
            fputc(ch,f2);
        }
        else if(ch=='\n'){
            fputc(ch,f2);
        }
        ch = nextChar;
    }
    fclose(f1);
    fclose(f2);
    printf("Successfully removed comments and added to file the original file %s to %s\n",testcaseFile,cleanFile);

    // populate();   
}

int postProcessing(){
    if(current==NULL){
        printf("No tokens generated\n");
        return 0;
    }
    current->next_token = (token_node*) malloc(sizeof(token_node)); //need to add dollar in the tokens too at the end
    current->next_token->next_token = NULL;
    current->next_token->token = (token_info*)malloc(sizeof(token_info));
    current->next_token->token->lexeme = "$";
    strcpy(current->next_token->token->type,"$");
    initialize();
    return 1;
}
// int main(){
//     char originalFile[100];
//     printf("Enter name of new clean file where you want the code without comments\n");
//     scanf("%s",&originalFile);

//     char cleanFile[100];
//     printf("Enter name of new clean file where you want the code without comments\n");
//     scanf("%s",&cleanFile);
//     removeComments(originalFile,cleanFile);
    // int 0;
// }