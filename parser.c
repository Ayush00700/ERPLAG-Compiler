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



