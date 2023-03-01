/*Lexer Module*/
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
/*Lexer Module*/
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_LEN 20
#define TABLE_SIZE 101
#define TOTAL_KEYWORDS 30


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

typedef struct hash_table_contents{
    char* lexeme ;
    char* tk_type;
    struct hash_table_contents* next;
}hash_table_contents;

typedef union data{
    int num;
    double rnum;
}data;

typedef struct token_info{
    char type[20];
    int line_no;
    char* lexeme;
    data values;
}token_info;

typedef struct token_node{
    token_info* token;
    struct token_node* next_token; 
}token_node;

/* IMPORTANT GLOBAL VARIABLES FOR LEXER MODULE*/
int populate_count = 1;
int line_no = 1;

token_node* ll_node; //LINKED-LIST 
token_node* stack_head; //STACK FOR ERRORS 
token_node* current; //LAST NODE THAT WAS ADDED
token_node* get_next_curr;


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
/*This function adds an error entry into the stack*/
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
        temp->next_token = stack_head;
        stack_head = temp;
    }
}

void pop_error_tokens()
/*This function pops an element from stack to display the error*/
{
    token_node* temp;
    temp = stack_head;
    
    // Print error entries
    while(temp!=NULL){
        printf("Error in line number :%d, Couldn't tokenize the keyword %s\n", temp->token->line_no,temp->token->lexeme);
        temp = temp->next_token;
    }
}

/*THE HASHING IS PART OF THE PRE-PROCESSING STEP*/
hash_table_contents* hash_table[TABLE_SIZE];

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

    // If message is "ERROR", we have to map that and add entry to stack
    if(!strcmp(message,"ERROR")){
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


/*---------------------------------------------------------------------------------------------------------------------*/


/*Parser Module*/
#define MAX_MULTI_RULES 10
#define NON_TERMINALS 75
#define TERMINALS 58
typedef enum Boolean {False,True}Boolean;

typedef struct ruleNode{
    char* nodeInfo; 
    Boolean isTerminal;
    struct ruleNode* nextNode;
}ruleNode;

typedef struct rule{
    ruleNode *head;
    ruleNode *tail;
    int lineNo;
}rule;

typedef struct Stack{
    ruleNode* top;
    int num;
} Stack; //push element after creation

typedef struct entry{
    char* key;
    unsigned int entry_number;
    unsigned int order;
    struct entry* next;
}entry;

typedef struct treeNodes{
    ruleNode* symbol;
    struct treeNodes* parent;
    struct treeNodes* r_sibling;
    struct treeNodes* child;
    Boolean isTerminal;
    int error; //tetntatively error=0 is no error, error =-1 iss the error that the parse tree below is missing 
    //and going to nextRight, error==-3 is the case when we assume the existence of the terminal 
    //construct but was not recieved from the code, error=-2 gibberish in the source code following 
    //the node containing this flag
    token_info* token;
}treeNodes;

typedef struct parse_tree{
    treeNodes* root;
    treeNodes* curr;
}parse_tree;

// Structure for Non-terminal 
typedef struct NonT{
    char* label; //--> the name of the non-terminal
    // Boolean derives_eps;
    entry* first_set[TABLE_SIZE]; //first set of the non terminal
    entry* follow_set[TABLE_SIZE]; //follow set of the non terminal
    int last_added;
    int Rules[MAX_MULTI_RULES]; //the rules in which this particular non terminal occurs in RHS
}NonT;

//Stack Operations
void push(Stack* parseStack, ruleNode* element);//Might need to handle epsilon rule issue
ruleNode* pop(Stack* parseStack);
ruleNode* top(Stack* parseStack);
int isEmptyStack(Stack* parseStack);
void printStack(Stack* parseStack);

//Tree Operations
void createParseTree(ruleNode* prog); 
treeNodes* convertToPTreenode(ruleNode* Node);

/* IMPORTANT GLOBAL VARIABLES FOR PARSER MODULE*/
int non_terminals = 0; 
int terminals = 0;
int lines = 0;
int ischange = 1; 
int* arr[NON_TERMINALS];

entry* non_Terminals_table[TABLE_SIZE]; //set of non-terminals (hashed)
entry* Terminals_table[TABLE_SIZE]; //set of terminals (hashed)
Stack parse_stack;
parse_tree ptree;


// ----------------------------------------------HELPER FUNCTIONS FOR STACK OPERATIONS ----------------------------------------------//
void push(Stack* parseStack, ruleNode* element)
/*Push an element into the parseStack*/
{
    // Empty stack
    if(!parseStack)
    {
        printf("The Stack is Empty");
        return;
    }

    // Problem in element
    else if(!element)
    {
        printf("The element passed is NULL or has some issues");
        return;
    }

    // Else push element in stack
    element->nextNode=parseStack->top;
    parseStack->top=element;
    parseStack->num++;
}

ruleNode* pop(Stack* parseStack)
/*Pop the topmost element from parseStack*/
{
    // Can't pop from empty stack
    if(!parseStack)
    {
        printf("The Stack is Empty");
        return NULL;
    }

    // Else pop
    ruleNode* temp=parseStack->top;
    parseStack->top=parseStack->top->nextNode;
    parseStack->num--;
    return temp;
}

ruleNode* top(Stack* parseStack)
/*Give a peek of the topmost element of the stack*/
{
    if(!parseStack)
    {
        printf("The Stack is Empty");
        return NULL;
    }
    
    else{
        ruleNode* temp=parseStack->top;
        return temp;
    }

}


int isEmptyStack(Stack* parseStack)
/*Return 0 if stack is empty, else return 1*/
{
    return parseStack->top?0:1;    
}

void addRecurse(Stack* stack, ruleNode* curr)
/*To maintain the order so as to follow Left-most derivation*/
{
    if(!curr)
        return;

    else
    {
        ruleNode* temp = (ruleNode*)malloc(sizeof(ruleNode));
        temp->isTerminal= curr->isTerminal;
        temp->nodeInfo = curr->nodeInfo;
        addRecurse(stack, curr->nextNode);
        push(stack,temp);
    }
}

void addNodesToStack(Stack* stack, rule* Rule)
/*This function takes the stack and which rule is to be added into the stack. It calls the addRecurse fn*/
{
    if(!Rule)
    {
        return;
    }

    else{
        ruleNode* curr=Rule->head;
        curr=curr->nextNode;
        pop(stack);
        addRecurse(stack, curr);
    }
}

void printStack(Stack* parseStack)
/*This function prints the updated stack*/
{
    int i=0;
    while(parseStack->top)
    {   
        ruleNode* temp=parseStack->top;
        printf("line no- %d ,%s, %d \n",i++,temp->nodeInfo, temp->isTerminal);
        pop(parseStack);
    }

}

// ----------------------------------------------HELPER FUNCTIONS FOR PARSE TREE ----------------------------------------------//
treeNodes* convertToPTreenode(ruleNode* Node)
/*This function makes a Parse tree node from the passed rule node*/
{
    treeNodes* generated =(treeNodes*)malloc(sizeof(treeNodes));
    generated->symbol=Node;
    return generated;
}

void createParseTree(ruleNode* start)
/*Initializes the parse tree*/
{
    ptree.root = (treeNodes*)malloc(sizeof(treeNodes));
   
    ptree.curr = ptree.root;
    ptree.root->isTerminal = 0;
    ptree.root->symbol = start;
}

void addTokenInfo(treeNodes* Node, token_info* token )
/*Setter function for a tree node*/
{
    Node->isTerminal=1;
    Node->token=token;
}


unsigned int hash(char* key)
/*hash function to hash a string*/
{
    unsigned int hashval = 0;
    for(;*key !='\0';key++){
        hashval = *key + 11*hashval;
    }
    return hashval%TABLE_SIZE;
}

void print_tables(entry* table[])
/*Print the hash or parse table or any table in the required format*/
{ 
    for (int i=0;i<TABLE_SIZE;i++){
        // Empty entry
        if(table[i]==NULL){
            printf("\t%d\t---\n",i);
        }

        else
        {
            entry* current;
            current = table[i];
            printf("\t%d\t",i);

            // Check all entries if chained
            while(current!=NULL){
                printf("'%s':<%d>order<%d> \t",current->key,current->entry_number,current->order);
                current = current->next;
            }
            printf("\n");
        }
    }
}

// ----------------------------------------------HELPER FUNCTIONS FOR SET OPERATIONS ----------------------------------------------//
int set_add(char* key,entry* table[],int entry_number)
/*This function adds the key into the table along with the entry number
0 => need not be added since it already exists
1 => needs to be added*/
{
    // Hash the key to get the index
    unsigned int index = hash(key);
    entry* current = table[index];

    // first checks whether it is already present or not 
    while(current!=NULL){
        if(strcmp(current->key,key)==0){
            return 0; //in case it doesn't need to add
        }
        current = current->next;
    }

    // else need to add the element
    entry* new_node = (entry*)malloc(sizeof(entry));
    new_node -> key = key;
    new_node -> next = table[index];
    new_node->entry_number = index;
    new_node->order = entry_number+1;
    table[index] = new_node;
    ischange = 1;
    return 1;

}

int set_contains(char* key,entry* table[])
/*Checks if the key is there in the table*/
{
    // Hash the key to get index
    unsigned int index = hash(key);
    entry* current = table[index];

    // If didn't match at first, go through the chain
    while(current!=NULL){
        if(strcmp(current->key,key)==0){
            // printf(" %d %d",current->entry_number,current->order);
            return current->order;
        }
        current = current->next;
    }
    return 0;
}

// ----------------------------------------------HELPER FUNCTIONS FOR GRAMMAR ARRAY ----------------------------------------------//
int noOfLines(FILE* fptr)
/*This function counts the number of lines in the input file*/
{
    char* strtok_result;
    int linecount = 0;

    // Move the file pointer to end of the file
    fseek (fptr , 0 , SEEK_END);
    // Ask the position of file pointer
    long fptr_size = ftell (fptr);
    // Reset the pointer back to start
    rewind (fptr);

    char buffer[fptr_size];
    size_t result; // Indicates number of bytes successfully read
    result = fread(buffer,1,fptr_size,fptr);

    // Returns first token
    strtok_result = strtok(buffer, "\n");
    // Keep counting the tokens
    while(strtok_result!=NULL){
        linecount++;
        strtok_result = strtok(NULL,"\n");
    }
    return linecount;
}



void insert_in_rule(rule* rules, int i, Boolean isTerminal, char* buff)
/*This functions adds a symbol to a linked list of symbols in a rule
It adds the string in the buff in the linked list at the ith index of rules array*/
{
        ruleNode* r2 = (ruleNode*)malloc(sizeof(ruleNode));
        r2->nodeInfo = buff;
        r2->isTerminal = isTerminal;
        r2->nextNode = NULL;
        rules[i-1].tail->nextNode = r2;
        rules[i-1].tail = r2;
}


void linepop(FILE* fp,int i,rule* rules)
/*This function reads a line of grammar.txt file and parses it to form a linked list*/
{
    char* buff = (char*) malloc(sizeof(char)*50);
    // Initialize a buffer
    memset(buff,'\0',50);
    // Read the string enclosed by '<' and '>'
    fscanf(fp,"<%s> ",buff);
    // Set the final character a null character
    buff[strlen(buff) - 1] = '\0';

    // Set a rule node to be added to the set of rules
    ruleNode* r1 = (ruleNode*)malloc(sizeof(ruleNode));
    r1->nodeInfo = buff;
    r1->isTerminal = False;
    r1->nextNode = NULL;
    (rules+i-1)->head = r1;
    rules[i-1].tail = r1;
    rules[i-1].lineNo = i;

    // Check if the string in buffer is a new non-terminal or an existing one
    int r = set_add(buff,non_Terminals_table,non_terminals);
    if(r==1)
        non_terminals++;
    
    char c;
    // moving to RHS of production
    c = fgetc(fp); // for ' ' before the '-'
    c = fgetc(fp); // for '-'
    c = fgetc(fp); // for ' ' after '-'
    int break_point = 0;
    c = fgetc(fp);
    
    // Untile end of line
    while(c!='\n'){
        // Non-terminal
        if(c == '<'){
            char* buff = (char*) malloc(sizeof(char)*50);
            // Replace the character at fp with the character in c
            // Here we are just writing back '<'
            ungetc(c,fp);
            memset(buff,'\0',50);
            //read a non terminal
            fscanf(fp,"<%s>",buff); 
            buff[strlen(buff) - 1] = '\0';
            insert_in_rule(rules,i,False,buff);
            // Check if it a new non-terminal or not
            r = set_add(buff,non_Terminals_table,non_terminals);
            if(r==1)
                non_terminals++;
        }

        // Terminal
        else{
            char* buff = (char*) malloc(sizeof(char)*20);
            // Replace the character at fp with the character in c
            // Here we are just writing back the character we read
            ungetc(c,fp);
            memset(buff,'\0',20);
            //read a terminal
            fscanf(fp,"%s",buff); 
            insert_in_rule(rules,i,True,buff);
            // Check if it is a new terminal or not
            r = set_add(buff,Terminals_table,terminals);
            if(r==1)
                terminals++;

        }

        if(break_point)
            break;
        // Next 3 lines are to handle \r\n or just \n
        c = fgetc(fp);
        if(c=='\n')
            break;
        c = fgetc(fp);

        if(c==EOF)
            break;
        }
}

void print_grammar(rule* rules)
/*This function prints the grammar from the rules populated in the array of linked list*/
{
    for(int i=0;i<lines;i++){
        // LHS of rule
        printf("<%s> ==> ",rules[i].head->nodeInfo);
        ruleNode* currNode = rules[i].head->nextNode;

        // RHS of rule
        while(currNode!=NULL){
            if(currNode->isTerminal==True){
            printf("%s ",currNode->nodeInfo);
            }
            else{ printf("<%s> ",currNode->nodeInfo);
            }
            currNode = currNode->nextNode;
        
        }
        printf("\n");
    }
}

rule* populate_grammar()
/*This function makes an array of linked list and populates it with the rules read from grammar.txt*/
{
    FILE* fp = fopen("new_grammar.txt","r");
    lines = noOfLines(fp);
    rule* rules = (rule*)malloc(sizeof(rule)*(lines+1));
    fclose(fp);
    fp = fopen("new_grammar.txt","r");
    for(int i=1;i<=lines;i++){
        linepop(fp,i,rules);
    }
    return rules;
}


NonT* populate_non_terminals(entry* table[],rule* rules)
/*This function makes an array of non-terminals along with important information*/
{
    NonT* non_terminals_rules = (NonT*) malloc(sizeof(NonT)*NON_TERMINALS);
    //int index,last_index = -1;

    /* First added their corresponding names and hashed them into their respective places within the array */
    for(int i=0;i<lines;i++){
        char* buff = rules[i].head->nodeInfo;
        int index = set_contains(buff, table); // find the hashed value of non-terminal from the non-terminals table
        non_terminals_rules[index-1].label = buff;
        non_terminals_rules[index-1].last_added = 0;
    }

    /* Following loop completes the initialization of rules array present in every non terminal from the array of non-terminals*/
    for(int i=0;i<lines;i++){
        ruleNode* current = rules[i].head;
        current = current->nextNode;
        while(current!=NULL){
            if(!current->isTerminal){
                int index = set_contains(current->nodeInfo, table);
                int r =  non_terminals_rules[index-1].last_added;
                if(non_terminals_rules[index-1].last_added>0 && non_terminals_rules[index-1].Rules[non_terminals_rules[index-1].last_added-1]!=i+1){
                non_terminals_rules[index-1].Rules[non_terminals_rules[index-1].last_added] = i+1;
                non_terminals_rules[index-1].last_added++;
                }
                else if(non_terminals_rules[index-1].last_added==0){
                    non_terminals_rules[index-1].Rules[non_terminals_rules[index-1].last_added] = i+1;
                    non_terminals_rules[index-1].last_added++;
                }
            }
            current = current->nextNode;
        }

    }
    return non_terminals_rules;
}

void print_nont(NonT* nont)
/*This function prints non-terminals*/
{
    for(int i=0;i<NON_TERMINALS;i++){
        printf("%s --> ",nont[i].label);
        for(int j=0;j<nont[i].last_added;j++){
            printf("%d,",nont[i].Rules[j]);
        }
        printf("\n");
    }
}

void print_tables_sets(entry* table[])
/*This function prints only the elements which are present in table*/
{
     for (int i=0;i<TABLE_SIZE;i++){
        // NULL entry
        if(table[i]==NULL){
          continue;
        }
        // Non-NULL entry
        else {
           entry* current;
           current = table[i];
           printf("\t%d\t",i);
           while(current!=NULL){
           printf("'%s':<%d>order<%d> \t",current->key,current->entry_number,current->order);
           current = current->next;
           }
           printf("\n");
        }
    }

}


void print_first_Sets(NonT* nont)
/*This function prints the first sets of all the non-terminals*/
{
     printf("FIRST SETS_---\n");
    for(int i=0;i<NON_TERMINALS;i++){
        printf("%s --> \n",nont[i].label);
        print_tables_sets(nont[i].first_set);
    }
}

void print_follow_sets(NonT* nont)
/*This function prints the follow sets of all the non-terminals*/
{
    printf("FOLLOW SETS_---\n");
    for(int i=0;i<NON_TERMINALS;i++){
        printf("%s --> \n",nont[i].label);
        print_tables_sets(nont[i].follow_set);
    }
}

void set_add_sets(entry* set_to_add[], entry* final_set[],int entry_number,char* except)
/* Function to add a set of values into a set provided the functionlaity of adding
 anything except the string mentioned in the variable "except"*/
{ 
    for(int i=0;i<TABLE_SIZE;i++){

        if(set_to_add[i]==NULL){
            continue;
        }
        
        else{
            // Controlled addition
            entry* entry_node = set_to_add[i];

            while(entry_node!=NULL){
                if(except==NULL){ 
                    set_add(entry_node->key, final_set,entry_number+1);
                }
                else if(strcmp(except,entry_node->key)==0){
                    entry_node = entry_node->next;
                    continue;
                }
                else{
                    set_add(entry_node->key, final_set,entry_number+1);
                }
                entry_number++;
                entry_node = entry_node->next;
            }
        }
    }
}


void compute_first_Set(rule* rules,NonT* non_terminals_set)
/*Needs to be computed until there is no more addition into their respective first sets*/
{
    // Bottom-up computation
    for(int i=lines-1;i>=0;i--){
        //find the index of non-terminal in the array of non-terminals
        int index = set_contains(rules[i].head->nodeInfo, non_Terminals_table);
        // Get to first symbol in RHS of the rule
        ruleNode* rulenode = rules[i].head->nextNode;
       
        while(rulenode!=NULL){
            // encounters a terminal in the rule and break 
            if(rulenode->isTerminal==True){
                int entry_number = set_contains(rulenode->nodeInfo,Terminals_table);
                set_add(rulenode->nodeInfo,non_terminals_set[index-1].first_set,entry_number-1);
                break;
            }
            
            //encounters another non-terminal in the rule
            else{
                int entry_number = set_contains(rulenode->nodeInfo,non_Terminals_table);
                int add_index = set_contains(rulenode->nodeInfo,non_Terminals_table );
                if(rulenode->nextNode==NULL&&set_contains("eps",non_terminals_set[add_index-1].first_set)){ // A -> A1 B2 and B2 derives epsilon
                    set_add_sets(non_terminals_set[add_index-1].first_set,non_terminals_set[index-1].first_set,entry_number-1,NULL); // no exception needed
                }else if(set_contains("eps",non_terminals_set[add_index-1].first_set)){ // A -> A1 B2 and A1 derives epsilon
                    set_add_sets(non_terminals_set[add_index-1].first_set,non_terminals_set[index-1].first_set,entry_number-1,"eps"); //add everything except the epsilon
                }
                else if(!set_contains("eps",non_terminals_set[add_index-1].first_set)){ //remaining case
                    set_add_sets(non_terminals_set[add_index-1].first_set,non_terminals_set[index-1].first_set,entry_number-1,NULL);
                    break;
                }
                
            }
            rulenode = rulenode->nextNode; 
        }
    }
}

void followadd(int ruleno,NonT* non_terminals_set,int i,rule* rules)
/*Helper function to compute follow set*/
{
    ruleNode* rulenodehead = rules[ruleno-1].head;
    ruleNode* rulenode = rulenodehead;
    NonT* non_terminal = non_terminals_set+i;
    rulenode = rulenode->nextNode;
    while(rulenode!=NULL){
        if(strcmp(rulenode->nodeInfo,(non_terminals_set+i)->label)){
            rulenode = rulenode->nextNode;
            continue;
        }
        else{
            ruleNode* next = rulenode->nextNode;
            //case 1:
            if(next!=NULL && next->isTerminal){
                // add this next terminal to NT+i;
                set_add(next->nodeInfo,non_terminals_set[i].follow_set,0);
            }
            //case 2:
            else if(next == NULL){
                // add follow of rulenodehead to NT+i;
                int index = set_contains(rulenodehead->nodeInfo,non_Terminals_table);
                set_add_sets(non_terminals_set[index-1].follow_set,non_terminals_set[i].follow_set,0,NULL);
            }
            else{
                //find the first of node "next" and add it to the follow of NT+i
                // if the find is containing eps then next = next->nextnode and repeat untill next is NULL where just add the follow set or break; 
                while(next!=NULL){
                    if(next->isTerminal){
                        set_add(next->nodeInfo,non_terminals_set[i].follow_set,0);
                        break;
                    }
                    int index = set_contains(next->nodeInfo,non_Terminals_table);
                    int does_contain_eps = set_contains("eps",non_terminals_set[index-1].first_set);
                    if(does_contain_eps==0)//not containing
                    {
                        set_add_sets(non_terminals_set[index-1].first_set,non_terminals_set[i].follow_set,0,NULL);
                        break;
                    }else{
                        set_add_sets(non_terminals_set[index-1].first_set,non_terminals_set[i].follow_set,0,"eps");
                        next = next->nextNode;
                    }
                }
                if(next == NULL){
                    int index = set_contains(rulenodehead->nodeInfo,non_Terminals_table);
                    set_add_sets(non_terminals_set[index-1].follow_set,non_terminals_set[i].follow_set,0,NULL);
                }
            }
            rulenode = rulenode->nextNode;
        }
    }
}


void compute_follow_Set(rule* rules,NonT* non_terminals_set)
/*Needs to be computed until there is no more addition into their respective follow sets*/
{
    int index_start_symbol = set_contains("prog", non_Terminals_table);
    set_add("$",non_terminals_set[index_start_symbol-1].follow_set,0);

    for(int i=0;i<NON_TERMINALS;i++){
        char* lab = non_terminals_set[i].label;
        for(int j=0;j<non_terminals_set[i].last_added;j++){
            followadd(non_terminals_set[i].Rules[j],non_terminals_set,i,rules);
        }
    }
}
void print_parse_Table(int** arr,NonT* non_terminals_set)
/*This function prints the parse table*/
{
    printf("\n");
    for(int i=0;i<non_terminals;i++){
        char* non_term = non_terminals_set[i].label;
        printf("<%s:%d> ",non_term, i);
        for(int j=0;j<terminals;j++){
            if(arr[i][j]==-1){
                printf(" ");
            }else{
                printf("<rule:%d,col:%d>",arr[i][j],j);
            }
        }
        printf("\n");
    }

}

int** create_parse_table(rule* rules,NonT* non_terminals_set)
/*Creates a parse table using info from rules and non-terminals*/
{
    // Initialize each row
    for (int i = 0; i < NON_TERMINALS; i++)
        arr[i] = (int*)malloc(TERMINALS* sizeof(int));
    

    // To denote null entries
    for(int i=0; i<NON_TERMINALS; i++)
    {
        for(int j=0; j<TERMINALS; j++)
        {
            arr[i][j]=-1;
        }
    }

    // Fill in the table
    for(int i =0;i<lines;i++)
    {
        int rule_no = i+1;
        ruleNode* headnode = rules[i].head;
        ruleNode* current = headnode;
        current = current->nextNode;
        // Get row number (1-indexed)
        int row_no = set_contains(headnode->nodeInfo,non_Terminals_table);
        
        while(current!=NULL){
            if(current->isTerminal && strcmp(current->nodeInfo,"eps")!=0){
                // Get column number (1-indexed)
                int col_no = set_contains(current->nodeInfo,Terminals_table);
                // error check - collision
                if(arr[row_no-1][col_no-1]==-1)arr[row_no-1][col_no-1] = rule_no;
                else {printf("\nGrammar not LL(1) compatible for non eps case %d %d %d %s %s", row_no,col_no, arr[row_no-1][col_no-1],current->nodeInfo,headnode->nodeInfo );}
                break;
            }

            else if(current->isTerminal==0){
                // Get first set and check if eps exists in it
                int non_term_index = set_contains(current->nodeInfo,non_Terminals_table);
                // If it contains eps
                if(set_contains( "eps" ,non_terminals_set[non_term_index-1].first_set))
                {
                    for (int j=0;j<TABLE_SIZE;j++){
                        entry* check = non_terminals_set[non_term_index-1].first_set[j];
                        while(check!=NULL){
                        if(strcmp(check->key,"eps")==0 ){
                            check = check->next;
                            continue;
                        }else {
                            int col_no = set_contains(check->key,Terminals_table);
                            if(arr[row_no-1][col_no-1]==-1) arr[row_no-1][col_no-1] = rule_no;//error check
                            else {printf("\nGrammar not LL(1) compatible for eps case %d %d %d %s %s", row_no,col_no, arr[row_no-1][col_no-1],non_terminals_set[non_term_index-1].first_set[j]->key,headnode->nodeInfo );}                          
                        }   
                        check = check->next;
                        }
                    }
                    current = current->nextNode;
                }

                else{

                    for (int j=0;j<TABLE_SIZE;j++){
                        entry* check = non_terminals_set[non_term_index-1].first_set[j];
                        while(check!=NULL){
                    
                            int col_no = set_contains(check->key,Terminals_table);
                            if(arr[row_no-1][col_no-1]==-1||arr[row_no-1][col_no-1]==rule_no)arr[row_no-1][col_no-1] = rule_no;//error check
                            else {printf("\nGrammar not LL(1) compatible for follow set case rule_no: %d row_no : %d, col_no %d \n",rule_no, row_no, col_no);}                          
                        
                        check = check->next;
                        }
                //agar eps hai toh current ko next and repeat
                    }
                    break;
                }
            }
            else{
                //folow of head bharo
                current = current->nextNode;
                // printf("\nEpsilon deriving rule was found !");
            }
        }
        if(current == NULL){
                //add the follow of head
                    for (int j=0;j<TABLE_SIZE;j++){
                        entry* check = non_terminals_set[row_no-1].follow_set[j];
                        while(check!=NULL){
                 
                            int col_no = set_contains(check->key,Terminals_table);
                            if(arr[row_no-1][col_no-1]==-1)arr[row_no-1][col_no-1] = rule_no;//error check
                            else {printf("\nGrammar not LL(1) compatible %d %d %d",row_no,col_no,arr[row_no-1][col_no-1]);}                          
                        
                        check = check->next;
                        }
                    }             
        }
    }
    return arr;
}

// ----------------------------------------------HELPER FUNCTIONS FOR PARSE TREE TRAVERSAL ----------------------------------------------//
//Add error checks in all of them
int checkUncleExists()
/*Checks if the parent of the node has a sibling or not*/
{

    if(ptree.curr==ptree.root|| ptree.curr->parent==ptree.root)
        return 0;

    if(ptree.curr->parent->r_sibling!=NULL){
        return 1;
    }
    
    else
        return 0;
}

void goToSibling()
/*Go to right sibling node*/
{
   // printf("\nprevious sibling : %s\t",ptree.curr->symbol->nodeInfo);
    ptree.curr=ptree.curr->r_sibling;
   // printf("\nnext sibling : %s\t",ptree.curr->symbol->nodeInfo);
}


void goToNextRight()
/*Go to right sibling of the parent*/
{
     if(ptree.curr->r_sibling)//We need to take parse tree curr to the next step. Either there is a right sibling available, and we move there
    {
        goToSibling();
        return;
    }
    // printf("\nprevious_uncle : %s\t",ptree.curr->symbol->nodeInfo);
    if(checkUncleExists())
    {ptree.curr=ptree.curr->parent->r_sibling;}
    else{
        while(!checkUncleExists() && ptree.curr->parent!=ptree.root){
            ptree.curr = ptree.curr->parent;
        }
        if(ptree.curr->parent==ptree.root)return ;
        ptree.curr = ptree.curr->parent->r_sibling;
    }
    // printf("\nnext : %s\t",ptree.curr->symbol->nodeInfo);
}
void goToParent()
/*Go to the parent node*/
{
    // printf("\nprevious : %s\t",ptree.curr->symbol->nodeInfo);
    ptree.curr=ptree.curr->parent;
    // printf("\nnext : %s\t",ptree.curr->symbol->nodeInfo);
}
void goToChild()
/*Go to the child node*/
{
   // printf("\nprevious : %s\t",ptree.curr->symbol->nodeInfo);
    ptree.curr=ptree.curr->child;
   // printf("\nnext : %s\t",ptree.curr->symbol->nodeInfo);
}



void addRuleToTree(rule* rule)
/*Add the "rule" to the parse tree*/
{
    if(strcmp(rule->head->nodeInfo,ptree.curr->symbol->nodeInfo)==0)
    {
        ruleNode* temp=rule->head->nextNode;//Finds the first ruleNode to be added. The second element in the rule chain 
        treeNodes* node=convertToPTreenode(temp); //Converts it to Ptree node. No pointers attached yet
        node->parent=ptree.curr;//Add the parent pointer
        ptree.curr->child=node; //The parent will only have one child. Multiple children can have the pointer to the same parent 
        temp=temp->nextNode;    //Go to the next ruleNode
        treeNodes* follow=node; //follow is used to attach the sibling pointers
        while(temp)//While RuleNode end is not reached
        {
            treeNodes* node=convertToPTreenode(temp);//Comments same as before
            node->parent=ptree.curr;
            follow->r_sibling=node;
            follow=node;
            temp=temp->nextNode;   //we need not use the r_sibling because nextPointers of the rule         
        }
        ptree.curr=ptree.curr->child;//Once we push the new role. The first NonTerminal, i.e. the direct child of parent should be pointed by curr hence simulating the stack movement as well
    }
    else
    {
        // Error reporting
        printf("error in implementation\n"); // just to ensure that our parser is working fine
    }
}

void in_order_traversal(treeNodes* current)
/*Inorder traversal of the parse tree:
If it has left child recursively call that, else print the current node, if current is leftmost child print parent, then finally see the right siblings
Ex: Input:        A
                 /
                B -- C -- D
                    /
                   E -- F
    
    Output:  B,A,E,C,F,D*/
{
    if(!current)
        return;
    
    // Check if leftmost child exists
    if(current->child)
        in_order_traversal(current->child);

    else{
        // Print current node
        printf("%s, ", current->symbol->nodeInfo);
    }

    // Print parent node if current is leftmost child
    if(current->parent->child == current)
        printf("%s, ", current->parent->symbol->nodeInfo);

    // Right sibling
    treeNodes* temp = current->r_sibling;
    if(!temp){
        in_order_traversal(temp);
    }
}


void call_parser(rule* rules, NonT* nont)
/*This function gives cue to start the Syntax Analyzer*/
{
    //Assume that you get the lookahead via this, filled some random values for now
    int prevLineNo=-1;
    token_info* curr = get_next_token();
    prevLineNo=curr->line_no;

    // till the time we keep on getting nextToken
    while(curr)    
    {
        int flag = 0;
        // Find the top of stack
        ruleNode* stackTop=top(&parse_stack);
        // ParseStack operations
        while(!stackTop->isTerminal)
        {    
            int row_no= set_contains(stackTop->nodeInfo, non_Terminals_table);
            int col_no= set_contains(curr->type, Terminals_table);

            // Access the parse table
            int indexToPush= arr[row_no-1][col_no-1];

            // If null entry found
            if(indexToPush==-1)
            {
                //Error Recovery:
                // If lookahead symbol is in SYNC(A) then pop A from stack and continue 
                // If lookahead symbol is not in SYNC(A) then record error and move pointer to right
                // printf(" initial Error recovery performing\n");
                int index = set_contains(stackTop->nodeInfo,non_Terminals_table);
                int val = set_contains(curr->type,nont[index-1].follow_set);
                if(!val){ // if val ==0 that is not present in the follow set
                    printf("expected %s but got %s at the line no. %d [1] \n",stackTop->nodeInfo,curr->type, curr->line_no);
                    printf("we ignored %s\n",curr->type);
                    curr=get_next_token();
                    // skip changes in parse tree, gibberish added in the source code
                    //Add the check that if the curr->lineno changes greater than prev; then 
                    ptree.curr->error=-2;
                }
                else{
                    if(stackTop->nextNode->isTerminal){
                        if(!strcmp(stackTop->nextNode->nodeInfo,curr->type )){
                            printf("expected %s statement insted got %s at the line no. %d [2]\n",stackTop->nodeInfo,curr->type, curr->line_no);
                            printf("we popped stack top\n");
                            pop(&parse_stack);
                            //Add the flag node to indicate that the construct for that nonT is missing and recurssively go to uncle
                            ptree.curr->error=-1;
                            goToNextRight();
                            
                        }
                        else{
                            printf("expected %s but got %s at the line no. %d [3]\n",stackTop->nodeInfo,curr->type, curr->line_no);
                            printf("we ignored %s\n",curr->type);
                            curr=get_next_token();
                            // skip changes in parse tree 
                            ptree.curr->error=-2;
                        }
                    }
                    else{
                        int new_index = set_contains(stackTop->nextNode->nodeInfo,non_Terminals_table);
                        int temp = arr[new_index-1][col_no-1];
                        if(temp == -1){
                            printf("expected %s but got %s at the line no. %d [4]\n",stackTop->nodeInfo,curr->type, curr->line_no);
                            printf("we ignored %s\n",curr->type);
                            prevLineNo=curr->line_no;
                            curr=get_next_token();
                            // skip changes in parse tree ,ight need to error flags
                            ptree.curr->error=-2;
                        }
                        else{
                            printf("expected %s statement insted got %s at the line no. %d [5]\n",stackTop->nodeInfo,curr->type, curr->line_no);
                            printf("we popped stack top\n");
                            pop(&parse_stack);
                            //Add the flag node to indicate that the construct for that nonT is missing and recurssively go to uncle
                            ptree.curr->error=-1;
                            goToNextRight();
                        }
                    }
                }
    
            }
            // Safe to push (Rule exists)
            else
            {
                // Rule obtained from the parse table
                rule ruleToPush=rules[indexToPush-1];
                // If the rule is of the form A->eps
                if(strcmp(ruleToPush.head->nextNode->nodeInfo,"eps")!= 0){
                    // Push to stack
                    addNodesToStack(&parse_stack, &ruleToPush);
                    // Update parse tree
                    addRuleToTree(&ruleToPush);
                   // printf("\n %d rule used",ruleToPush.lineNo);
                }

                else{
                    pop(&parse_stack);
                    addRuleToTree(&ruleToPush);
                   // printf("\n %d rule used and is epsilon rule",ruleToPush.lineNo);
                    goToNextRight();
                } 
                
            }
            stackTop=top(&parse_stack);
        }
        // the case when it goes out of the above statement i.e. there is a terminal to match
        {
            // If there is a match
            if(strcmp(stackTop->nodeInfo,curr->type)==0)
            {
                pop(&parse_stack);//Pop the stack element
                addTokenInfo(ptree.curr, curr); //Since only a terminal is popped, we map the token info to the parse tree
                
               //Else we go to the uncle, i.e. sibling of the parent. Since we are only coming to this stage when Parent has no use, we need to go the sibling which should ideally be present in the stack at the same time
                
                    goToNextRight();
               
            }
            // Terminal mismatch
            else
            {
                // Error recovery - print message that missing terminal (lookahead symbol) was added (have to verify once)
                // and move to next lookahead
                int lineWithError=curr->line_no;
                if(prevLineNo!=curr->line_no)
                {
                    lineWithError=prevLineNo;
                }
                printf("Terminal mismatch %s was missing at the line no. %d [6]\n",stackTop->nodeInfo, lineWithError);
                pop(&parse_stack);
                ptree.curr->error=-3;
                goToNextRight();
                flag = 1;
                
            }
        }

        // Get next token from the lexer
        if(!flag) {prevLineNo=curr->line_no; curr=get_next_token();}
      
    }

    // No more elements in parse stack, then parsing is successful
    if(isEmptyStack(&parse_stack)){
        printf("\nParsing successfull");
    }

    // If stack is non-empty after parsing all input then perform error recovery
    else {
        // Error recovery - if A --> eps then apply that, else pop A and report (have to verify once)
        
        printf("\nError in implementation, the stack should not have been empty");

        printf("\n%s",parse_stack.top->nodeInfo);
    }
}

int main()
/*Main driver function*/
{
    /* Lexer module calls*/
    FILE* fp;
    char test_buff[50] = "code_error_case2.txt";

    fp = fopen(test_buff,"r");
    printf("Running file %s", test_buff);
    
    populate_hash_table();


    call_lexer(fp,4096);
    current->next_token = (token_node*) malloc(sizeof(token_node)); //need to add dollar in the tokens too at the end
    current->next_token->next_token = NULL;
    current->next_token->token = (token_info*)malloc(sizeof(token_info));
    current->next_token->token->lexeme = "$";
    strcpy(current->next_token->token->type,"$");

    
    initialize();
    // printf("TOKENS ARE .... (first to last) \n");
    // printtokens();
    // printf("ERRORS ARE .... (last to first) \n");
    // pop_error_tokens();    

    /* Parser module calls*/
    rule* rules = populate_grammar();
    set_add("$",Terminals_table,57);
    terminals++;
    NonT* nont = populate_non_terminals(non_Terminals_table,rules);

    int changes = 0;
    while(ischange){
        ischange = 0;
        compute_first_Set(rules,nont);
        changes ++;
    }    

    ischange = 1;
    changes = 0;
    while(ischange){
        ischange = 0;
        compute_follow_Set(rules, nont);
        changes++;
    }


/*




*/
    int ** arr = create_parse_table(rules,nont);
    // print_parse_Table(arr,nont);
    ruleNode* dollar= (ruleNode*)malloc(sizeof(ruleNode));
    char* dollar_char= "$";
    dollar->isTerminal=1;
    dollar->nodeInfo=dollar_char;
    push(&parse_stack, dollar);

    ruleNode* prog= (ruleNode*)malloc(sizeof(ruleNode));
    char* prog_char= "prog";
    prog->isTerminal=0;
    prog->nodeInfo=prog_char;
    push(&parse_stack, prog);

    createParseTree(prog);
    //start symbol add in the parse tree


    call_parser(rules,nont);

}