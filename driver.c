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
void initialize(){
    get_next_curr = ll_node;
}

token_info* get_next_token(){
    if(get_next_curr==NULL){
        return NULL;
    }
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


void populate(char* buffer,FILE* fp,int bufsize){
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
    if(stack_head==NULL){
        stack_head = (token_node*)malloc(sizeof(token_node));
        stack_head->token = tk;
        stack_head->next_token = NULL;
    }else{
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

hash_table_contents* create_hash_table_content(char* lexeme, char* tk){
    hash_table_contents* h = (hash_table_contents*)malloc(sizeof(hash_table_contents));
    // h->lexeme = lexeme;h->tk_type = (token_type)hash(lexeme);
    h->lexeme = lexeme;
    h->tk_type = tk;
    h->next = NULL;
    return h;
}


size_t _hash(char *lexeme){

    int length = strnlen(lexeme, MAX_LEN);
    size_t  hash_value = 0;
    for (int i =0;i<length;i++){
        hash_value += lexeme[i];
        hash_value = (hash_value*lexeme[i])%97;
    }

    return hash_value%TABLE_SIZE;
}

void init_hash_table(){
    for(int i=0;i<TABLE_SIZE;i++){
        hash_table[i] = NULL;
    }
}

void print_hash_table(){
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

int hash_table_insert(char* lexeme, char* tk){
    if(lexeme==NULL) return 0;
    int index = _hash(lexeme);
    hash_table_contents* entry = hash_table[index];

    while(entry!=NULL){
        if(strcmp(entry->lexeme,lexeme)==0){
            return 0;
        }
        entry = entry->next;
    }
    hash_table_contents* h = create_hash_table_content(lexeme, tk);
    h->next = hash_table[index];
    hash_table[index] = h;
    return index;
}

void populate_hash_table(){
    init_hash_table();
    for(int i=0;i<TOTAL_KEYWORDS;i++){
        hash_table_insert(arr_keywords[i],arr_keywords_upper[i]);
    }
}

char* lookup(char* lexeme){
    size_t hash_value = _hash(lexeme);
    hash_table_contents* entry = hash_table[hash_value];
    int found = 0;
    while(entry!=NULL){
        if(strcmp(entry->lexeme,lexeme)==0){
            found = 1;
            return entry->tk_type;
        }
        entry = entry->next;
    }
    if(found==0)return "ID";
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
        else if(!strcmp(message,"RNUM")){
            double value = (double)atof(lexeme);
            tk->values.rnum = (double)atof(tk->lexeme);
        }
        else if(!strcmp(message,"ID")) message = lookup(lexeme);

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
    token_info* token;
}treeNodes;

typedef struct parse_tree{
    treeNodes* root;
    treeNodes* curr;
}parse_tree;

typedef struct NonT{ //STRUCTURE FOR A NON-TERMINAL 
    char* label; //--> the name of the non-terminal
    // Boolean derives_eps;
    entry* first_set[TABLE_SIZE]; //first set of the non terminal
    entry* follow_set[TABLE_SIZE]; //follow set of the non terminal
    int last_added;
    int Rules[MAX_MULTI_RULES]; //the rules in which this particular non terminal occurs in RHS
}NonT;

/* IMPORTANT GLOBAL VARIABLES FOR PARSER MODULE*/

//Stack Operations
void push(Stack* parseStack, ruleNode* element);//Might need to handle epsilon rule issue
ruleNode* pop(Stack* parseStack);
ruleNode* top(Stack* parseStack);
int isEmptyStack(Stack* parseStack);
void printStack(Stack* parseStack);



//Tree Operations
void createParseTree(ruleNode* prog); 
treeNodes* convertToPTreenode(ruleNode* Node);

/* IMPORTANT GLOBAL VARIABLES*/
int non_terminals = 0; 
int terminals = 0;
int lines = 0;
int ischange = 1; 
int* arr[NON_TERMINALS];

entry* non_Terminals_table[TABLE_SIZE]; //set of non-terminals (hashed)
entry* Terminals_table[TABLE_SIZE]; //set of terminals (hashed)
Stack parse_stack;
parse_tree ptree;


//Stack Operations
void push(Stack* parseStack, ruleNode* element)
{
    if(!parseStack)
    {
        printf("The Stack is Empty");
    }
    else if(!element)
    {
        printf("The element passed is NULL or has some issues");
    }
    element->nextNode=parseStack->top;
    parseStack->top=element;
    parseStack->num++;
}

ruleNode* pop(Stack* parseStack)
{
    if(!parseStack)
    {
        printf("The Stack is Empty");
    }
    ruleNode* temp=parseStack->top;
    parseStack->top=parseStack->top->nextNode;
    parseStack->num--;
    return temp;
}

ruleNode* top(Stack* parseStack)
{
    if(!parseStack)
    {
        printf("The Stack is Empty");
    }
    ruleNode* temp=parseStack->top;

    return temp;
}


int isEmptyStack(Stack* parseStack)
{
    return parseStack->top?0:1;    
}
void addRecurse(Stack* stack, ruleNode* curr)
{
    if(!curr)
    return;
    else{
    ruleNode* temp = (ruleNode*)malloc(sizeof(ruleNode));
    temp->isTerminal= curr->isTerminal;
    temp->nodeInfo = curr->nodeInfo;
    addRecurse(stack, curr->nextNode);
    push(stack,temp);
    }
}
void addNodesToStack(Stack* stack, rule* Rule)
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

void printStack(Stack* parseStack){
    int i=0;
    while(parseStack->top)
    {   
        ruleNode* temp=parseStack->top;
        printf("line no- %d ,%s, %d \n",i++,temp->nodeInfo, temp->isTerminal);
        pop(parseStack);
    }

}

treeNodes* convertToPTreenode(ruleNode* Node)
{
    treeNodes* generated =(treeNodes*)malloc(sizeof(treeNodes));
    generated->symbol=Node;
    return generated;
}

void createParseTree(ruleNode* start){
    ptree.root = (treeNodes*)malloc(sizeof(treeNodes));
   
    ptree.curr = ptree.root;
    ptree.root->isTerminal = 0;
    ptree.root->symbol = start;
}

void addTokenInfo(treeNodes* Node, token_info* token )
{
    Node->isTerminal=1;
    Node->token=token;
}


unsigned int hash(char* key){ //hash function for hashing a string 
    unsigned int hashval = 0;
    for(;*key !='\0';key++){
        hashval = *key + 11*hashval;
    }
    return hashval%TABLE_SIZE;
}

void print_tables(entry* table[]){ 
 for (int i=0;i<TABLE_SIZE;i++){
        if(table[i]==NULL){
            printf("\t%d\t---\n",i);
        }else {
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

int set_add(char* key,entry* table[],int entry_number){
    unsigned int index = hash(key);
    entry* current = table[index];

    while(current!=NULL){// first checks whether it is already present or not 
        if(strcmp(current->key,key)==0){
            return 0; //in case it doesn't need to add
        }
        current = current->next;
    }

    entry* new_node = (entry*)malloc(sizeof(entry)); //else need to add the element
    new_node -> key = key;
    new_node -> next = table[index];
    new_node->entry_number = index;
    new_node->order = entry_number+1;
    table[index] = new_node;
    ischange = 1;
    return 1;

}

int set_contains(char* key,entry* table[]){
    unsigned int index = hash(key);
    entry* current = table[index];

    while(current!=NULL){
        if(strcmp(current->key,key)==0){
            // printf(" %d %d",current->entry_number,current->order);
            return current->order;
        }
        current = current->next;
    }
    return 0;
}




int noOfLines(FILE* fptr){
    char* strtok_result;
    int linecount = 0;
    fseek (fptr , 0 , SEEK_END);
    long fptr_size = ftell (fptr);
    rewind (fptr);
    char buffer[fptr_size];
    size_t result;
    result = fread (buffer,1,fptr_size,fptr);
    strtok_result = strtok(buffer, "\n");
    while(strtok_result!=NULL){
        linecount++;
        strtok_result = strtok(NULL,"\n");
    }
    return linecount;
}



void insert_in_rule(rule* rules, int i, Boolean isTerminal, char* buff){
        ruleNode* r2 = (ruleNode*)malloc(sizeof(ruleNode));
        r2->nodeInfo = buff;
        r2->isTerminal = isTerminal;
        r2->nextNode = NULL;
        rules[i-1].tail->nextNode = r2;
        rules[i-1].tail = r2;
}


void linepop(FILE* fp,int i,rule* rules){ //given a line of the grammar.txt file--parses it to form a linked list
    char* buff = (char*) malloc(sizeof(char)*50);
    memset(buff,'\0',50);
    fscanf(fp,"<%s> ",buff);
    buff[strlen(buff) - 1] = '\0';
    ruleNode* r1 = (ruleNode*)malloc(sizeof(ruleNode));

    r1->nodeInfo = buff;
    r1->isTerminal = False;
    r1->nextNode = NULL;
    (rules+i-1)->head = r1;
    rules[i-1].tail = r1;
    rules[i-1].lineNo = i;
    int r = set_add(buff,non_Terminals_table,non_terminals);
    if(r==1)non_terminals++;
    char c;
    c = fgetc(fp);// moving to RHS of production
    c = fgetc(fp);
    c = fgetc(fp);
    int break_point = 0;
    c = fgetc(fp);
    
    while(c!='\n'){
    if(c == '<'){
        char* buff = (char*) malloc(sizeof(char)*50);
        ungetc(c,fp);
        memset(buff,'\0',50);
        fscanf(fp,"<%s>",buff); //read a non terminal 
        buff[strlen(buff) - 1] = '\0';
        insert_in_rule(rules,i,False,buff);
        r = set_add(buff,non_Terminals_table,non_terminals);
        if(r==1) non_terminals++;
    }
    else{
        char* buff = (char*) malloc(sizeof(char)*20);
        ungetc(c,fp);
        memset(buff,'\0',20);
        fscanf(fp,"%s",buff); //read a terminal 
        insert_in_rule(rules,i,True,buff);
        r = set_add(buff,Terminals_table,terminals);
        if(r==1) terminals++;

    }

    if(break_point)break;
    c = fgetc(fp);
    if(c=='\n')break;
    c = fgetc(fp);

    if(c==EOF) break;
    }
}

void print_grammar(rule* rules){
      for(int i=0;i<lines;i++){
        printf("<%s> ==> ",rules[i].head->nodeInfo);
        ruleNode* currNode = rules[i].head->nextNode;
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

rule* populate_grammar(){ //makes an array of linked list
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


NonT* populate_non_terminals(entry* table[],rule* rules){ //start making an array of non-terminals along with important information
    NonT* non_terminals_rules = (NonT*) malloc(sizeof(NonT)*NON_TERMINALS);
    //int index,last_index = -1;

    /* FIrst added their corresponding names and hashed them into their respective places within the array */
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

void print_nont(NonT* nont){ //print non -terminals 
    for(int i=0;i<NON_TERMINALS;i++){
        printf("%s --> ",nont[i].label);
        for(int j=0;j<nont[i].last_added;j++){
            printf("%d,",nont[i].Rules[j]);
        }
        printf("\n");
    }
}

void print_tables_sets(entry* table[]){ //only print the elements which are there
     for (int i=0;i<TABLE_SIZE;i++){
        if(table[i]==NULL){
          continue;
        }else {
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


void print_first_Sets(NonT* nont){
     printf("FIRST SETS_---\n");
    for(int i=0;i<NON_TERMINALS;i++){
        printf("%s --> \n",nont[i].label);
        print_tables_sets(nont[i].first_set);
    }
}

void print_follow_sets(NonT* nont){
    printf("FOLLOW SETS_---\n");
        for(int i=0;i<NON_TERMINALS;i++){
        printf("%s --> \n",nont[i].label);
        print_tables_sets(nont[i].follow_set);
    }
}

/* Functionality to add a set of values into a set */
//provided the functionlaity of adding anything except the char* "except"
void set_add_sets(entry* set_to_add[], entry* final_set[],int entry_number,char* except){ 
    for(int i=0;i<TABLE_SIZE;i++){
        if(set_to_add[i]==NULL){
            continue;
        }else{
            entry* entry_node = set_to_add[i];
            while(entry_node!=NULL){
            if(except==NULL){ 
                set_add(entry_node->key, final_set,entry_number+1);
            }else if(strcmp(except,entry_node->key)==0){
                entry_node = entry_node->next;
                continue;
            }else{
                set_add(entry_node->key, final_set,entry_number+1);
            }
            entry_number++;
            entry_node = entry_node->next;
            }
        }
    }
}

/* Needs to be computed untill there is no more addition into their respective first sets*/
void compute_first_Set(rule* rules,NonT* non_terminals_set){
    
    for(int i=lines-1;i>=0;i--){
        int index = set_contains(rules[i].head->nodeInfo, non_Terminals_table);//find the index of non-terminal in the array of non-terminals
        ruleNode* rulenode = rules[i].head->nextNode;
       
        while(rulenode!=NULL){
            if(rulenode->isTerminal==True){ // encounters a terminal in the rule and break 
                int entry_number = set_contains(rulenode->nodeInfo,Terminals_table);
                set_add(rulenode->nodeInfo,non_terminals_set[index-1].first_set,entry_number-1);
                break;
            }else{ //encounters another non -terminal in the rule
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

void followadd(int ruleno,NonT* non_terminals_set,int i,rule* rules){
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


void compute_follow_Set(rule* rules,NonT* non_terminals_set){
    int index_start_symbol = set_contains("prog", non_Terminals_table);
    set_add("$",non_terminals_set[index_start_symbol-1].follow_set,0);

    for(int i=0;i<NON_TERMINALS;i++){
        char* lab = non_terminals_set[i].label;
        for(int j=0;j<non_terminals_set[i].last_added;j++){
            followadd(non_terminals_set[i].Rules[j],non_terminals_set,i,rules);
        }
    }
}
void print_parse_Table(int** arr,NonT* non_terminals_set){


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

int** create_parse_table(rule* rules,NonT* non_terminals_set){


    
    for (int i = 0; i < NON_TERMINALS; i++)
        arr[i] = (int*)malloc(TERMINALS* sizeof(int));
    

    for(int i=0; i<NON_TERMINALS; i++)
    {
        for(int j=0; j<TERMINALS; j++)
        {
            arr[i][j]=-1;
        }
    }

    for(int i =0;i<lines;i++){
        int rule_no = i+1;
        ruleNode* headnode = rules[i].head;
        ruleNode* current = headnode;
        current = current->nextNode;
        int row_no = set_contains(headnode->nodeInfo,non_Terminals_table);//1 indexed
        while(current!=NULL){
            if(current->isTerminal && strcmp(current->nodeInfo,"eps")!=0){
                //mmm
                int col_no = set_contains(current->nodeInfo,Terminals_table);//1 indexed
                if(arr[row_no-1][col_no-1]==-1)arr[row_no-1][col_no-1] = rule_no;//error check
                else {printf("\nGrammar not LL(1) compatible for non eps case %d %d %d %s %s", row_no,col_no, arr[row_no-1][col_no-1],current->nodeInfo,headnode->nodeInfo );}
                break;
            }
            else if(current->isTerminal==0){
                //first set nikalo aur check if eps
                int non_term_index = set_contains(current->nodeInfo,non_Terminals_table);
                if(set_contains( "eps" ,non_terminals_set[non_term_index-1].first_set)){
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


//Add error checks in all of them

int checkUncleExists(){

    if(ptree.curr==ptree.root|| ptree.curr->parent==ptree.root) return 0;

    if(ptree.curr->parent->r_sibling!=NULL){
        return 1;
    }else return 0;
}


void goToUncle(){
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
void goToParent(){
    // printf("\nprevious : %s\t",ptree.curr->symbol->nodeInfo);
    ptree.curr=ptree.curr->parent;
    // printf("\nnext : %s\t",ptree.curr->symbol->nodeInfo);
}
void goToChild(){
   // printf("\nprevious : %s\t",ptree.curr->symbol->nodeInfo);
    ptree.curr=ptree.curr->child;
   // printf("\nnext : %s\t",ptree.curr->symbol->nodeInfo);
}
void goToSibling(){
   // printf("\nprevious sibling : %s\t",ptree.curr->symbol->nodeInfo);
    ptree.curr=ptree.curr->r_sibling;
   // printf("\nnext sibling : %s\t",ptree.curr->symbol->nodeInfo);
}


void addRuleToTree(rule* rule)
{
    if(strcmp(rule->head->nodeInfo,ptree.curr->symbol->nodeInfo)==0)
    {
        ruleNode* temp=rule->head->nextNode;//Finds the first ruleNode to be added. The second element in the rule chain 
        treeNodes* node=convertToPTreenode(temp); //Converts it to Ptree node. No pointers attached yer
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
        ptree.curr=ptree.curr->child;//Once we push the new role. The first NonTerminal, i.e. the direct child of parent should be pointer by curr hence simulating the stack movement as well
    }
    else
    {
        //Error
    }
}


void call_parser(rule* rules){
    //Assume that you get the lookahead via this, filled some random values for now
    token_info* curr = get_next_token();


    while(curr) //till the time we keep on getting nextToken
    {

        ruleNode* stackTop=top(&parse_stack);//Find the top of stack
        
        while(!stackTop->isTerminal)
        {    
            
            int row_no= set_contains(stackTop->nodeInfo, non_Terminals_table);
            int col_no= set_contains(curr->type, Terminals_table);

            int indexToPush= arr[row_no-1][col_no-1];
            if(indexToPush==-1)
            {
                //Perform Error Recovery for no rule found to expansion 
                printf(" initial Error recovery performing\n");
                break;
            }
            else
            {
                rule ruleToPush=rules[indexToPush-1];
                if(strcmp(ruleToPush.head->nextNode->nodeInfo,"eps")!= 0){
                    addNodesToStack(&parse_stack, &ruleToPush);
                    addRuleToTree(&ruleToPush);
                   // printf("\n %d rule used",ruleToPush.lineNo);
                }
                else{
                    pop(&parse_stack);
                    addRuleToTree(&ruleToPush);
                   // printf("\n %d rule used and is epsilon rule",ruleToPush.lineNo);
                    goToUncle();
                } 
                
            }
            stackTop=top(&parse_stack);
        }
        //the case when it goes out of the above statement i.e. there is a terminal to match
        {
            if(strcmp(stackTop->nodeInfo,curr->type)==0)//If there is a match
            {
                pop(&parse_stack);//Pop the stack element
                addTokenInfo(ptree.curr, curr); //Since only a terminal is popped, we map the token info to the parse tree
                
                if(ptree.curr->r_sibling)//We need to take parse tree curr to the next step. Either there is a right sibling available, and we move there
                {
                    goToSibling();
                }//Else we go to the uncle, i.e. sibling of the parent. Since we are only coming to this stage when Parent has no use, we need to go the sibling which should ideally be present in the stack at the same time
                else
                {
                    goToUncle();
                }
            }
            else
            {
                //Perform Error Recovery due to non matchin
                printf("AA Error recovery performing\n");
            }
        }
    
        curr=get_next_token();
      
    }
    if(isEmptyStack(&parse_stack)){
        printf("\nParsing successfull");
    }else {
        printf("\nPerform error recovery");
        printf("\n%s",parse_stack.top->nodeInfo);
    }
}

int main(){
    /* Lexer module calls*/
    FILE* fp;
    char test_buff[50] = "code_test_case1.txt";

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


    call_parser(rules);


}
