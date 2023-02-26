#include <stdio.h>
#include "lexer.h"

/* IMPORTANT DEFINITIONS */

#define MAX_MULTI_RULES 10
#define NON_TERMINALS 73
#define TERMINALS 57
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

//Stack Operations
void push(Stack* parseStack, ruleNode* element);//Might need to handle epsilon rule issue
ruleNode* pop(Stack* parseStack);
ruleNode* top(Stack* parseStack);
int isEmptyStack(Stack* parseStack);
void printStack(Stack* parseStack);



//Tree Operations

treeNodes* convertToPTreenode(ruleNode* Node);
void addTokenInfo(treeNodes* Node, token_info* token );
// void addTerminalInfoToParser(token_info token);
void addRuleToTree(rule* rule);
void goToUncle();
void goToParent();
void goToChild();
void goToSibling();

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
//


//Tree Operations 
void createParseTree(){
    ptree.curr = (treeNodes*)malloc(sizeof(treeNodes));
}

void addTokenInfo(treeNodes* Node, token_info* token )
{
    Node->isTerminal=1;
    Node->token=token;
}

treeNodes* convertToPTreenode(ruleNode* Node)
{
    treeNodes* generated =(treeNodes*)malloc(sizeof(treeNodes));
    generated->symbol=Node;
    return generated;
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
    FILE* fp = fopen("grammar.txt","r");
    lines = noOfLines(fp);
    rule* rules = (rule*)malloc(sizeof(rule)*(lines+1));
    fclose(fp);
    fp = fopen("grammar.txt","r");
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
            if(current->isTerminal){
                //mmm
                int col_no = set_contains(current->nodeInfo,Terminals_table);//1 indexed
                if(arr[row_no-1][col_no-1]==-1)arr[row_no-1][col_no-1] = rule_no;//error check
                else {printf("\nGrammar not LL(1) compatible for non eps case %d %d %d %s %s", row_no,col_no, arr[row_no-1][col_no-1],current->nodeInfo,headnode->nodeInfo );}
                break;
            }
            else{
                //first set nikalo aur check if eps
                int non_term_index = set_contains(current->nodeInfo,non_Terminals_table);
                if(set_contains( "eps" ,non_terminals_set[non_term_index-1].first_set)){
                    for (int j=0;j<TABLE_SIZE;j++){
                        entry* check = non_terminals_set[non_term_index-1].first_set[j];
                        while(check!=NULL){
                        if(check==NULL || strcmp(check->key,"eps")==0 ){
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
                        if(check==NULL){
                            check = check->next;
                            continue;
                        }else {
                            int col_no = set_contains(check->key,Terminals_table);
                            if(arr[row_no-1][col_no-1]==-1||arr[row_no-1][col_no-1]==rule_no)arr[row_no-1][col_no-1] = rule_no;//error check
                            else {printf("\nGrammar not LL(1) compatible for follow set case rule_no: %d row_no : %d, col_no %d \n",rule_no, row_no, col_no);}                          
                        }   
                        check = check->next;
                        }
                //agar eps hai toh current ko next and repeat
                    }
                    break;
                }
            }
        }
        if(current == NULL){
                //add the follow of head
                    for (int j=0;j<TABLE_SIZE;j++){
                        if(non_terminals_set[row_no-1].follow_set[j]==NULL){
                            continue;
                        }else {
                            int col_no = set_contains(non_terminals_set[row_no-1].follow_set[j]->key,Terminals_table);
                            if(arr[row_no-1][col_no-1]==-1)arr[row_no-1][col_no-1] = rule_no;//error check
                            else {printf("\nGrammar not LL(1) compatible");}                          
                        }   
                    }
                    break;               

        }
    }
    return arr;
}
//Add error checks in all of them
void goToUncle()
{
    ptree.curr=ptree.curr->parent->r_sibling;
}
void goToParent()
{
    ptree.curr=ptree.curr->parent;
}
void goToChild()
{
    ptree.curr=ptree.curr->child;
}
void goToSibling();
{
    ptree.curr=ptree.curr->r_sibling;
}


void addRuleToTree(rule* rule)
{
    if(rule->head->nodeInfo==ptree.curr->symbol->nodeInfo)
    {
        ruleNode*Ru temp=rule->head->nextNode;
        treeNodes* node=convertToPTreenode(temp);
        node->parent=ptree.curr;
        temp=temp->nextNode;    
        ptree.curr->child=node;
        treeNodes* follow=node;
        while(temp)
        {
            treeNodes* node=convertToPTreenode(temp);
            node->parent=ptree.curr;
            follow->r_sibling=node;
            follow=node;
            temp=temp->nextNode;            
        }
        ptree.curr=ptree.curr->child;
    }
    else
    {
        //Error
    }
}


void call_parser(rule* rules){
    //Assume that you get the lookahead via this
    token_info* curr = get_next_token();
    curr->lexeme="ID";
    curr->line_no=2;
    // curr->values=3 ;
    //curr->

    while(curr)
    {

        ruleNode* stackTop=top(&parse_stack);
        if(stackTop->isTerminal)
        {
            if(stackTop->nodeInfo==curr->lexeme)
            {
                pop(&parse_stack);
                addTokenInfo(ptree.curr, curr);
                if(ptree.curr->r_sibling)
                {
                    goToSibling();
                }
                else
                {
                    goToUncle();
                }
                //perform tree operation
            }
            else
            {
                //Perform Error Recovery
            }
        }
        else
        {
            int row_no= set_contains(stackTop->nodeInfo, non_Terminals_table);
            int col_no= set_contains(curr->lexeme, Terminals_table);
            int indexToPush= arr[row_no-1][col_no-1];
            if(indexToPush==-1)
            {
                //Perform Error Recovery
            }
            else
            {
                rule ruleToPush=rules[indexToPush];
                addNodesToStack(&parse_stack, &ruleToPush);
                addRuleToTree(&ruleToPush);
            }

        }
        curr=get_next_token();
      
    }
    
}
 //first - check from parse table given top of stack and curr(token), which rule you need to use
    
    

    //second - push that rule into the stack using the function addNodesToStack(&parse_stack, &rules[rule_no]);
    //third - note down the rule_no
    //if top of stack is a terminal and matches with the curr(token:lexeme)then pop the stack and get_next_token and iterate

    /* BASELINE IMPLEMENTATION

    while(curr){
        
        curr = get_next_token();

        int pop = 0;
        while(!pop){

            do stack operations(check the top of the stack and curr(lookahead)-->)
            if stack top is terminal 
                then pop = 1 (pop from the stack)
            else if TOP_OF_STACK IS replaced (some rule no from the parsetable)
                    THEN ADD THE RULE INTO THE PARSE TREE AS WELL (initially the parse tree consists of <prog>TreeNode)

            if something pops (because the non-terminal and the rule associated derives epsilon)
                then pop = 1;(pop from the stack)
        }


    }
    
    */


int main(){
    rule* rules = populate_grammar();
    // print_grammar(rules);
    // print_tables(non_Terminals_table);
    // printf("\n\n");
    // print_tables(Terminals_table);
    printf("%d %d\n",non_terminals,terminals);

    NonT* nont = populate_non_terminals(non_Terminals_table,rules);
    // int index = hash("modDecs");
    // printf("%d\n",index);
    // int r = set_contains("modDecs",non_Terminals_table);
    // printf("%d",set_contains("modDecs",non_Terminals_table));
    // print_nont(nont);
    int changes = 0;
    while(ischange){
        ischange = 0;
        compute_first_Set(rules,nont);
        changes ++;
    }

    // print_first_Sets(nont);
    // printf("\n%d changes",changes);
    ischange = 1;
    changes = 0;
    while(ischange){
        ischange = 0;
        compute_follow_Set(rules, nont);
        changes++;
    }

    // print_follow_sets(nont);
    // printf("\n%d changes",changes);
    print_tables_sets(non_Terminals_table);
    // printf("\n%d", terminals);


    //for parsing operations uncomment below :


    int ** arr = create_parse_table(rules,nont); //Confusion with the global name arr?
    print_parse_Table(arr,nont);
    ruleNode* dollar= (ruleNode*)malloc(sizeof(ruleNode));
    char* dollar_char= "$";
    dollar->isTerminal=0;
    dollar->nodeInfo=dollar_char;
    push(&parse_stack, dollar);

    ruleNode* prog= (ruleNode*)malloc(sizeof(ruleNode));
    char* prog_char= "prog";
    prog->isTerminal=0;
    prog->nodeInfo=prog_char;
    push(&parse_stack, prog);

    addNodesToStack(&parse_stack, &rules[0]);
    // printStack(&parse_stack);

    // print_first_Sets(nont);

    // print_follow_sets(nont);
}