#include "tokeninfo.h"
#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
enum Boolean {True,False};
void populateGrammar(FILE* fptr);
typedef struct ruleNode{
    token_info* nodeInfo; 
    Boolean isTerminal;
    struct ruleNode* nextNode;
}ruleNode;

typedef struct ruleLL{
    ruleNode head;
    
}ruleLL;