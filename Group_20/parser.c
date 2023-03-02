/* THE MATH_CS boiz
--------------------
1. Rajan Sahu       2019B4A70572P
2. Yash Goyal       2019B4A70638P
3. Ayush Agarwal    2019B4A70652P
4. Vasu Swaroop     2019B4A70656P
5. A Sudarshan      2019B4A70744P
*/

#include "lexer.h"
#include <string.h>
#include "parserDef.h"
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
/*This function adds a symbol to a linked list of symbols in a rule
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
/* Function to add a set of values into a set except the string mentioned in the variable "except"*/
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
/*Go to right node: either right sibling, if exists, else uncle*/
{
    // We need to take parse tree curr to the next step. Either there is a right sibling available, and we move there
    if(ptree.curr->r_sibling){
        goToSibling();
        return;
    }

    // printf("\nprevious_uncle : %s\t",ptree.curr->symbol->nodeInfo);
    if(checkUncleExists()){
        ptree.curr=ptree.curr->parent->r_sibling;
    }

    else{
        while(!checkUncleExists() && ptree.curr->parent!=ptree.root){
            ptree.curr = ptree.curr->parent;
        }
        if(ptree.curr->parent==ptree.root)
            return ;
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

void print_parse_tree(char* out_file)
/*This function uses the inorder traversal and */
{
    // Open file in append mode
    FILE* f = fopen(out_file,"a+");

    // If file opened successfully
    if(!f){

        // Create a pointer to traverse the parse tree

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

            // If no rule found
            if(indexToPush==-1)
            {
                /* Error Recovery required */
                int index = set_contains(stackTop->nodeInfo,non_Terminals_table);
                int val = set_contains(curr->type,nont[index-1].follow_set);

                // If lookahead symbol is NOT in FOLLOW(A)
                if(!val){
                    // Enters here because user added something extra in the source code
                    printf("expected %s but got %s at the line no. %d [1] \n",stackTop->nodeInfo,curr->type, curr->line_no);
                    // Printing what was the extra
                    printf("we ignored %s\n",curr->type);
                    // Move lookahead
                    curr=get_next_token();

                    // skip changes in parse tree, gibberish added in the source code
                    //Add the check that if the curr->lineno changes greater than prev; then 
                    ptree.curr->error=-2;
                }

                // If lookahead symbol is in FOLLOW(A)
                else{
                    // If next symbol in stack is a terminal
                    if(stackTop->nextNode->isTerminal){
                        // If that terminal matches the lookahead
                        if(!strcmp(stackTop->nextNode->nodeInfo,curr->type )){
                            // Enters here because construct generated by the non-terminal on the stack top is missing
                            printf("expected %s statement insted got %s at the line no. %d [2]\n",stackTop->nodeInfo,curr->type, curr->line_no);
                            printf("we popped stack top\n");
                            // Pop the non-terminal currently on top of stack
                            pop(&parse_stack);

                            //Add the flag node to indicate that the construct for that nonT is missing and recurssively go to uncle
                            ptree.curr->error=-1;
                            goToNextRight();
                            
                        }

                        // If terminal mismatch
                        else{
                            // Enters here because user added something extra in the source code
                            printf("expected %s but got %s at the line no. %d [3]\n",stackTop->nodeInfo,curr->type, curr->line_no);
                            // Printing the extra
                            printf("we ignored %s\n",curr->type);
                            // Move lookahead
                            curr=get_next_token();

                            // skip changes in parse tree 
                            ptree.curr->error=-2;
                        }
                    }

                    // If next symbol on stack is a non-terminal B
                    else{
                        // Check if there is a rule in the cell in the parse table corresponding to row B and column of lookahead symbol
                        int new_index = set_contains(stackTop->nextNode->nodeInfo,non_Terminals_table);
                        int temp = arr[new_index-1][col_no-1];

                        // If no rule found
                        if(temp == -1){
                            // Enters here because user added something extra in the source code
                            printf("expected %s but got %s at the line no. %d [4]\n",stackTop->nodeInfo,curr->type, curr->line_no);                    
                            // Printing the extra
                            printf("we ignored %s\n",curr->type);
                            prevLineNo=curr->line_no;
                            // Move lookahead
                            curr=get_next_token();

                            // skip changes in parse tree ,ight need to error flags
                            ptree.curr->error=-2;
                        }

                        // If rule found then top of the stack is useless now
                        else{
                            // Enters here because construct generated by the non-terminal on the stack top is missing
                            printf("expected %s statement insted got %s at the line no. %d [5]\n",stackTop->nodeInfo,curr->type, curr->line_no);
                            printf("we popped stack top\n");
                            // Pop the non-terminal currently on top of stack
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
                
                // Else we go to the uncle, i.e. sibling of the parent. Since we are only coming to this stage when Parent has no use,
                // we need to go the sibling which should ideally be present in the stack at the same time
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

                // Report that the terminal on top of stack was missing in the source code
                printf("Terminal mismatch %s was missing at the line no. %d [6]\n",stackTop->nodeInfo, lineWithError);
                pop(&parse_stack);

                // Parse tree changes corresponding to this error case
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

        // With our current implementation, it shouldn't reach here, but if it does then there is an issue in implementation
        printf("\nError in implementation, the stack should not have been empty");
        // What was the top of the stack when error occured - just for debugging
        printf("\n%s",parse_stack.top->nodeInfo);
    }
}