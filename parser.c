#include "parser.h"

//Code by Vasu


void push(Stack* parseStack, nodeRepresentation* element)
{
    if(!parseStack)
    {
        printf("The Stack is Empty");
    }
    else if(!element)
    {
        printf("The element passed is NULL or has some issues");
    }
    element->next=parseStack->top;
    parseStack->top=element;
    parseStack->num++;
}

nodeRepresentation* pop(Stack* parseStack)
{
    if(!parseStack)
    {
        printf("The Stack is Empty")
        *flag=*flag; 
    }
    nodeRepresentation* temp=parseStack->top;
    parseStack->top=parseStack->top->next;
    parseStack->num--;
    return temp;
}

nodeRepresentation* top(Stack* parseStack)
{
    if(!parseStack)
    {
        printf("The Stack is Empty")
       
    }
    nodeRepresentation* temp=parseStack->top;

    return temp;
}


int isEmptyStack(Stack* parseStack)
{
    return parseStack->top?0:1;    
}

void addNodesToParseTree(parseTree PTree, rule Rule)
{
    treeNodes temp;
    while(nodeRepresentation temp=getNode(Rule))
    {
       t =convertToPTreenode(temp);

    }

}


int findRow(Stack stackTop)
{

    
}
int findCol(Stack stackTop)
{

    
}

void addNodesToStack(Stack* stack, rule* Rule)
{
    if(!Rule)
    {
        return;
    }
    else
    {
        addNodesToStack(stack, Rule->head->next);
        push(stack, Rule->head);
    }
}

// void addTerminal(parseTree PTree);
// {

// }

treeNodes* convertToPTreenode(nodeRepresentation Node)
{
    treeNodes* generated =(treenNodes*)malloc(sizeof(treeNodes));
    generated->Node= Node;
    if(Node.isTerminal)
    {
        generated->isTerminal=1;
        generated->isLeaf=0; //Some future/current way of checking if the bottom most layer
    }
    return generated;
}

void createParseTable(rule table[][])
{
    //push the dollar symbol and/or equivalent begining symbols
    while(isNextAvailable())//this function should check if there is a next token incoming. 
    //the above function can also be replaced by checking null pointer in the DS of lexemme
    {
        /// Implement the parsing algorithm 
        token_info inputToken=getNextToken();
        if(stack.num<0)
        {
            printf("error, stack empty")
            break;
        }
        nodeRepresentation * stackTop= top(Stack);
        while(stack.num>0)
        {
            if(stackTop->isTerminal)
            {
                if(inputToken.type==stackTop->top.Node.type)
                {
                    pop(Stack);
                    //Perform some parsetree computation addNodesToParseTree(parseTree,head);         
                    
                }
                else
                {
                    //Perform Error Recovery
                }
            }
            else
            {
                rule Rule=table[findRow(stackTop)][findCol(inputToken)]
                if(isRuleEmpty(Rule)==0)//Can make do with only the implicit null comparison 
                {
                    pop(Stack);
                    addNodesToParseTree(parseTree,Rule);          
                    addNodesToStack(stack, Rule)     
                }
                else
                {
                    //Perform Error Recovery
                }
                    
            }
        }  
    }
#include <stdio.h>
#include "lexer.h"

/* IMPORTANT DEFINITIONS */

#define MAX_MULTI_RULES 10
#define NON_TERMINALS 72
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


typedef struct entry{
    char* key;
    unsigned int entry_number;
    unsigned int order;
    struct entry* next;
}entry;

typedef struct NonT{ //STRUCTURE FOR A NON-TERMINAL 
    char* label; //--> the name of the non-terminal
    // Boolean derives_eps;
    entry* first_set[TABLE_SIZE]; //first set of the non terminal
    entry* follow_set[TABLE_SIZE]; //follow set of the non terminal
    int last_added;
    int Rules[MAX_MULTI_RULES]; //the rules in which this particular non terminal occurs in RHS
}NonT;

/* IMPORTANT GLOBAL VARIABLES*/
int non_terminals = 0; 
int terminals = 0;
int lines = 0;
int ischange = 1; 


entry* non_Terminals_table[TABLE_SIZE]; //set of non-terminals (hashed)
entry* Terminals_table[TABLE_SIZE]; //set of terminals (hashed)

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




/*----------------------------------------HELPER FUNCTIONS----------------------------------------*/
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

void populateGrammar(FILE* fptr,rule rules[]){
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
}

void printArray(rule rules[],int noOfRules){
    for(int i=0;i<noOfRules;i++){
        
    }
}

FirstSet join(FirstSet f1, FirstSet f2)
{
    f1.tail.next = &f2;
    f1.tail = f2.tail;

    return f1;
}

int belong(char a, char* arr)
/*Returns 1 if the element a belongs to arr else 0*/
{
    int flag=-1;
    for(int i=0; i<sizeof(arr); i++)
    {
        if(arr[i] == a)
        {
            flag=1;
            return i;
        }
    }

    return -1;
}

/*----------------------------------------FIRST SET COMPUTATION----------------------------------------*/
FirstSet first_of_rule(rule r, NonT nt[])
{
    // If first symbol in RHS of rule r is a terminal
    // Just return the symbol
        //if(r->head->next)
    if(r.head.next->isTerminal)
    {
        FirstSet f;
        // Single node
        f.head.type = f.tail.type = r.head.nextNode->nodeInfo->type;
        f.head.next = f.tail.next = NULL;

        return f;
    }

    // If first symbol in RHS of rule r is a nonterminal
    else
    {       
        
        // Find the first set of this non-terminal
        FirstSet f =  first_of_nt(r.head.nextNode, nt);

        // Checking if the first symbol in the RHS of the rule derives epsilon
        if(nt[r.head.nextNode->nodeInfo->type].derives_eps)
        {
            // Add symbols in the follow set of this symbol
            FollowSet f1 = follow(r.head.nextNode, nt);
            FirstSet f2;

            FollowSetNode* F = &f1.head;
            f2.head.type = f2.tail.type = F->type;
            f2.head.next = f2.tail.next = NULL;

            while(F->next != NULL)
            {
                F = F->next;
                FirstSetNode F1;
                F1.type = F->type;
                F1.next = NULL;
                f2.tail.next = &F1;
                f2.tail = F1;
            }

            
            f = join(f,f2);
        }

        return f;

    }
}
//this function might need to be changed
FirstSet first_of_nt(nodeRepresentation* N, NonT nt[])
{
    if(nt[N->Node->type].derives_eps)
        {
        
            // Add epsilon to first set and return
            FirstSetNode f1;
            f1.type = (token_type)EPS;
            f1.next = NULL;
            // f.tail.next = &f1;
            // f.tail = f1;
        }
}

/*----------------------------------------FOLLOW SET COMPUTATION----------------------------------------*/
FollowSet follow(nodeRepresentation* N, NonT nt[])
{



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
    for(int i=0;i<NON_TERMINALS;i++){
        printf("%s --> \n",nont[i].label);
        print_tables_sets(nont[i].first_set);
    }
}

void print_follow_sets(NonT* nont){
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
            if(except==NULL){
                set_add(set_to_add[i]->key, final_set,entry_number+1);
            }else if(strcmp(except,set_to_add[i]->key)==0){
                continue;
            }else{
                set_add(set_to_add[i]->key, final_set,entry_number+1);
            }
            entry_number++;
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
                    set_add_sets(non_terminals_set[add_index-1].first_set,non_terminals_set[index-1].first_set,entry_number-1,"eps");
                    break;
                }
                
            }
            rulenode = rulenode->nextNode; 
        }
    }
}

void followadd(int ruleno,NonT* non_terminals_set,int i,rule* rules){
    // ruleNode* rulenode = 
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


int main(){
    FILE* fptr = fopen("grammar.txt","r+");
    if(fptr==NULL){
        perror("Error opening file");
        exit(1);
    }
    int lines = noOfLines(fptr);
    printf("No of Lines %d",lines);
    rule rules[lines];
    populateGrammar(fptr,rules);
    fclose(fptr);
    printArray(rules,lines);
}



    rule* rules = populate_grammar();
    // print_grammar(rules);
    // print_tables(non_Terminals_table);
    // printf("\n\n");
    // print_tables(Terminals_table);
    // printf("%d %d\n",non_terminals,terminals);

    NonT* nont = populate_non_terminals(non_Terminals_table,rules);
    // int index = hash("modDecs");
    // printf("%d\n",index);
    // int r = set_contains("modDecs",non_Terminals_table);
    // printf("%d",set_contains("modDecs",non_Terminals_table));
    print_nont(nont);
    int changes = 0;
    while(ischange){
        ischange = 0;
        compute_first_Set(rules,nont);
        changes ++;
    }
    // print_first_Sets(nont);
    // printf("\n%d changes",changes);
    compute_follow_Set(rules, nont);
    print_follow_sets(nont);


}
