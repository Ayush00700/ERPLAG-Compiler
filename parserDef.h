/* Group-20
--------------------
1. Rajan Sahu       2019B4A70572P
2. Yash Goyal       2019B4A70638P
3. Ayush Agarwal    2019B4A70652P
4. Vasu Swaroop     2019B4A70656P
5. A Sudarshan      2019B4A70744P
*/
#ifndef PARSERDEF_H
#define PARSERDEF_H
#include "lexer.h"

#define MAX_MULTI_RULES 10
#define NON_TERMINALS 75
#define TERMINALS 58
typedef enum Boolean {False,True}Boolean;

//FOR IR CODE


typedef enum OPCODE
{
    ASSIGN,
    UNARY_PLUS,
    UNARY_MINUS,
    GET_VALUE,
    PRINT,
    ADD,
    SUB,
    MUL,
    DIV,
    LT,
    LE,
    GT,
    GE,
    EQ,
    NEQ,
    OR,
    AND,
    CALL,
    FUNC,
    GOTO,
    IF,
    LABEL,
    RET,
    PARA_IN,
    PARA_OUT
}OPCODE;


static const char * const OPCODE_str[] = {
    [ASSIGN] = "ASSIGN",
    [UNARY_PLUS] = "UNARY_PLUS",
    [UNARY_MINUS] = "UNARY_MINUS",
    [GET_VALUE] = "GET_VALUE",
    [PRINT] = "PRINT",
    [ADD] = "ADD",
    [SUB] = "SUB",
    [MUL] = "MUL",
    [DIV] = "DIV",
    [LT] = "LT",
    [LE] = "LE",
    [GT] = "GT",
    [GE] = "GE",
    [EQ] = "EQ",
    [NEQ] = "NEQ",
    [OR] = "OR",
    [AND] = "AND",
    [CALL] = "CALL",
    [FUNC] = "FUNC",
    [GOTO] = "GOTO",
    [IF] = "IF",
    [LABEL] = "LABEL",
    [RET] = "RET",
    [PARA_IN] = "PARA_IN",
    [PARA_OUT] = "PARA_OUT"
};


typedef struct ir_code_node
/* The entry will be of the following form
    +------------+----------+----------+-----------+
    |  operator  |  result  |  left_op |  right_op |
    +------------+----------+----------+-----------+
*/
{
    OPCODE operator;
    char* result;
    char* left_op;
    char* right_op;
    struct ir_code_node* next;
}ir_code_node;

typedef struct ir_code
{
    ir_code_node* head;
    ir_code_node* tail;
}ir_code;

//FOR PARSER

typedef struct ruleNode{
    char* nodeInfo; 
    Boolean isTerminal;
    int rule_no;//add
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

typedef struct ast_node{
    char* name;
    int no_of_children;
    struct ast_node** child_pointers;
    struct ast_node* next;
    ir_code* code; //FOR INTERMEDIATE CODE GEN
    char* tempName; //FOR INTERMEDIATE CODE GEN
    char* true;
    char* false;
    char* nextJump;
    char* type;
    char* asm_code;
    Boolean isTerminal;
    token_info* token;
}ast_node;

typedef struct treeNodes{
    ruleNode* symbol;
    struct treeNodes* parent;
    struct treeNodes* r_sibling;
    struct treeNodes* child;
    Boolean isTerminal;
    int rule_no;//add
    int no_of_children;
    struct treeNodes** child_pointers;
    int error; //tetntatively error=0 is no error, error =-1 iss the error that the parse tree below is missing 
    //and going to nextRight, error==-3 is the case when we assume the existence of the terminal 
    //construct but was not recieved from the code, error=-2 gibberish in the source code following 
    //the node containing this flag
    token_info* token;
    ast_node* inh;
}treeNodes;


typedef struct parse_tree{
    treeNodes* root;
    treeNodes* curr;
}parse_tree;

// Structure for Non-terminal 
typedef struct NonT{
    char* label; //--> the name of the non-terminal
    // Boolean derives_eps;
    entry* first_set[TABLE_SIZE]; //first set of the non terminal
    entry* follow_set[TABLE_SIZE]; //follow set of the non terminal
    int last_added;
    int Rules[MAX_MULTI_RULES]; //the rules in which this particular non terminal occurs in RHS
}NonT;

#endif
