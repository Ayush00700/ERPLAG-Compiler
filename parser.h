#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef enum Boolean {False,True}Boolean;

#define NUMTERMINALS 57
#define NUMNONTERMINALS 80

extern parseTree PTree; 
extern nodeRepresentation sentential;
extern nodeRepresentation ruleNode;
extern Stack stack;
extern rule_chain [NUMNONTERMINALS][NUMTERMINALS] table;


char* arr_NonT[65] = {"<prog>","<modDecs>","<othermods>","<driver>",
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

typedef struct rule{
    // nodeRepresentation* head;
    // nodeRepresentation* tail;
    nodeRepresentation* head;
    int lineNo;
    FnF Fifo;
    {
        /* data */
    };
    
}rule;

typedef struct rule_chain{
    rule* chain_head;
    rule* chain_tail;
    Boolean isNull;
}rule_chain;

typedef struct NonT{
    char* label;
    NT type;
    Boolean derives_eps;
    FirstSet* Fi;
    FollowSet* Fo;
    rule_chain Rules;
}NonT;



union node{
    NonT NonTerminal;//For memory issues, we might need to create a new DS which only stores relevant information
    token_info terminal;
}

typedef struct nodeRepresentation{
    node Node; 
    Boolean isTerminal;
    nodeRepresentation* next;
}nodeRepresentation; 

typedef struct NonTerminalTreeNodes{
    node Node; //alternatively add a check condition with a boolean isLeaf
    NonTerminalTreeNodes* children;
    NonTerminalTreeNodes* sibling; 
    NonTerminalTreeNodes* parent; //used in case of non recursive traversals;
    Boolean isleaf;
    Boolean isTerminal;
    // NTTN* 

    /*
    NonT NonTerminal;//For memory issues, we might need to create a new DS which only stores relevant information
    token_info terminal;

    */
}NonTerminalTreeNodes;

typedef struct ruleTable{
    rule_chain ruleChain;
    int num;
}

/*
typedef struct parseTable{
    rule_chain [NUMNONTERMINALS][NUMTERMINALS] table;

  
}parseTable;*/


typedef struct parseTree{
    NonTerminalTreeNodes* root;
    NonTerminalTreeNodes* curr;
}parseTree;

typedef struct Stack{
    nodeRepresentation* top;
    int num;
} Stack;

void push(Stack* parseStack, nodeRepresentation* element)
{
    if(!parseStack)
    {
        printf("The Stack is Empty")
     
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
        *flag=*flag; 
    }
    nodeRepresentation* temp=parseStack->top;

    return temp;
}
void addTerminalToTree(parseTree PTree, inputToken)
{

}

int isEmptyStack(Stack* parseStack)
{
    return parseStack->top?0:1;    
}

int findRow(Stack stackTop)
{

    
}
int findRow(Stack stackTop)
{

    
}
void createParseTable(parseTable* parseTable);

void populateGrammar(FILE* fptr,rule rules[]);
int noOfLines(FILE *fptr);
