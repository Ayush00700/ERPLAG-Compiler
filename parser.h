#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum Boolean {False,True}Boolean;


typedef struct ruleNode{
    token_info* nodeInfo; 
    Boolean isTerminal;
    struct ruleNode* nextNode;
}ruleNode;

typedef struct FirstSet{
    token_type type;
    struct FirstSet* next;
}FirstSet;

typedef struct FollowSet{
    token_type type;
    struct FollowSet* next;
}FollowSet;

typedef struct ruleLL{
    ruleNode head;
    int lineNo;
    FirstSet firstSets;
    FollowSet followSets;
}ruleLL;

void populateGrammar(FILE* fptr,ruleLL rules[]);
int noOfLines(FILE *fptr);
