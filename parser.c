#include "parser.h"

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

void populateGrammar(FILE* fptr,ruleLL rules[]){
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
FirstSet first_of_rule(rule r, rule rules[], NonT nt[])
{
    // If first symbol in RHS of rule r is a terminal
    // Just return the symbol
    if(r.head.nextNode->isTerminal)
    {
        FirstSet f;
        // Single node
        f.head.type = f.tail.type = r.head.nodeInfo->type;
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
            
        }

        return f;

    }
}
FirstSet first_of_nt(ruleNode* N, NonT nt[])
{
    if(nt[N->nodeInfo->type].derives_eps)
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
FollowSet follow(ruleNode* N, NonT nt[])
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



