#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum Boolean {False,True}Boolean;

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

typedef struct ruleNode{
    token_info* nodeInfo; 
    Boolean isTerminal;
    struct ruleNode* nextNode;
}ruleNode;

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
    ruleNode head;
    ruleNode tail;
    int lineNo;
    FnF Fifo;
    struct rule* next;
}rule;

typedef struct rule_chain{
    rule* chain_head;
    rule* chain_tail;
}rule_chain;

typedef struct NonT{
    char* label;
    NT type;
    Boolean derives_eps;
    FirstSet* Fi;
    FollowSet* Fo;
    rule_chain Rules;
}NonT;

void populateGrammar(FILE* fptr,rule rules[]);
int noOfLines(FILE *fptr);
