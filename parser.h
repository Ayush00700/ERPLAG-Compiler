#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef enum Boolean {False,True}Boolean;

#define NUMTERMINALS 57
// #define NON_TERMINALS 80
#define TABLE_SIZE 15 //might be changed
#define MAX_MULTI_RULES 10
#define NON_TERMINALS 74

//  parseTree PTree; 
//  ruleNode sentential;
//  Stack stack;
//  rule table[NON_TERMINALS][NUMTERMINALS]; 

char* arr_NonT[NON_TERMINALS] = {"<prog>","<modDecs>","<othermods>","<driver>",
"<modDec>","<mod>","<modDef>","<ip_list>","<ret>","<op_list>","<dataType>",
"<N1>","<type>","<N2>","<range_arrays>","<index_arr>","<stmts>","<stmt>",
"<iostmt>","<simplestmt>","<declarestmt>","<condstmt>","<iterstmt>",
"<var_print>","<boolconst>","<id_num_rnum>","<arr_ele_print>","<new_index>",
"<P1>","<assignstmt>","<modReusestmt>","<whichstmt>","<lvalueIDstmt>",
"<lvalueARRstmt>","<exp>","<element_index_with_expressions>","<sign>",
"<optional>","<idList>","<N3>","<arithOrboolExp>","<U>","<unary_op>",
"<new_NT>","<arithExp>","<AnyTerm>","<N4>","<logOp>","<N5>","<relOp>",
"<term>","<N6>","<op1>","<factor>","<N7>","<op2>","<N_11>","<arr_element>",
"<sign>","<N_11>","<arrExpr>","<N_10>","<arrTerm>","<arr_N4>","<arrFactor>","<arr_N5>","<caseStmts>","<default>","<N9>","<value>","<range_forloop>","<index_forloop>","<new_index_forloop>","<sign_forloop>"};

typedef enum NT{prog,modDecs,othermods,driver,modDec,mod,modDef,ip_list,ret,op_list,dataType,N1,type,N2,range_arrays,index_arr,stmts,stmt,
iostmt,simplestmt,declarestmt,condstmt,iterstmt,
var_print,boolconst,id_num_rnum,arr_ele_print,new_index,
P1,assignstmt,modReusestmt,whichstmt,lvalueIDstmt,
lvalueARRstmt,exp,element_index_with_expressions,sign,
optional,idList,N3,arithOrboolExp,U,unary_op,
new_NT,arithExp,AnyTerm,N4,logOp,N5,relOp,
term,N6,op1,factor,N7,op2,N_11,arr_element,
sign,N_11,arrExpr,N_10,arrTerm,arr_N4,arrFactor,arr_N5,caseStmts,default,N9,value,range_forloop,index_forloop,new_index_forloop,sign_forloop}NT;

typedef struct FirstSetNode{
    token_type type;
    struct FirstSet* next;
}FirstSetNode;

typedef struct FollowSetNode{
    token_type type;
    struct FollowSet* next;
}FollowSetNode;


typedef struct FirstSet{
    FirstSetNode head;
    FirstSetNode tail;
}FirstSet;

typedef struct FollowSet{
    FollowSetNode head;
    FollowSetNode tail;
}FollowSet;

typedef union FnF{
    FirstSet Fi;
    FollowSet Fo;
}FnF;


// typedef struct NonT{
//     char* label;
//     NT type;
//     Boolean derives_eps;
//     FirstSet* Fi;
//     FollowSet* Fo;
//     rule Rules;//What is the use of this?
// }NonT;

typedef struct NonT{ //STRUCTURE FOR A NON-TERMINAL 
    char* label; //--> the name of the non-terminal
    entry* first_set[TABLE_SIZE]; //first set of the non terminal
    entry* follow_set[TABLE_SIZE]; //follow set of the non terminal
    int last_added;
    int Rules[MAX_MULTI_RULES]; //the rules in which this particular non terminal occurs in RHS
}NonT;


typedef union node{
    NonT NonTerminal;//For memory issues, we might need to create a new DS which only stores relevant information
    token_info terminal;
}node;

typedef struct ruleNode{
    node Node;
    char* nodeInfo; 
    Boolean isTerminal;
    struct ruleNode* nextNode;
}ruleNode;

typedef struct rule{
    ruleNode *head;
    ruleNode *tail;
    int lineNo;
}rule;


typedef struct treeNodes{
    ruleNode Node; //alternatively add a check condition with a boolean isLeaf
    struct treeNodes* children;
    struct treeNodes* sibling; 
    struct treeNodes* parent; //used in case of non recursive traversals;
    Boolean isLeaf;
    Boolean isTerminal;
   
}treeNodes;

// typedef struct ruleTable{
//     rule ruleChain;
//     int num;
// }

/*
typedef struct parseTable{
    rule [NON_TERMINALS][NUMTERMINALS] table;
}parseTable;*/


typedef struct parseTree{
    treeNodes* root;
    treeNodes* curr;//might be removed
}parseTree;

typedef struct Stack{
    ruleNode* top;
    int num;
} Stack;

typedef struct entry{
    char* key;
    unsigned int entry_number;
    unsigned int order;
    struct entry* next;
}entry;


//Stack operations
void push(Stack* parseStack, ruleNode* element);
ruleNode* pop(Stack* parseStack);
ruleNode* top(Stack* parseStack);
int isEmptyStack(Stack* parseStack);

//Array indexing Hashing
int findRow(Stack stackTop);
int findCol(Stack stackTop);

//Sudarshan
void createParseTable(rule table[NON_TERMINALS][NUMTERMINALS]);
void populateGrammar(FILE* fptr,rule rules[]);
int noOfLines(FILE *fptr);
void printArray(rule rules[],int noOfRules);

treeNodes* convertToPTreenode(ruleNode Node);
void addNodesToStack(Stack* stack, rule* head);  



