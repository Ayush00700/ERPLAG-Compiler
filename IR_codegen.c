#include <string.h>
#include "IR_codegen.h"

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
    FN_CALL
}OPCODE;

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
    struct ir_code* next;
}ir_code_node;

typedef struct ir_code
{
    ir_code_node* head;
    ir_code_node* tail;
}ir_code;

ir_code* global_ir_code;

void initialize_ir_code()
/* This function initializes the Quadruple to store the intermediate 3 address code*/
{
    
    global_ir_code = (ir_code*)malloc(sizeof(ir_code));
    global_ir_code->head = global_ir_code->tail = NULL;
}

void add_entry(ir_code* list, ir_code_node* entry)
/* This function adds entry to the end of the list*/
{
    if(list->head = NULL)
        list->head = list->tail = entry;

    else if(list->head == list->tail)
        list->head->next = list->tail = entry;
    
    else
    {
        list->tail->next = entry;
        list->tail = entry;
    }  
}
void IR_inputStmt(){

}


void IR_inputStmt(){
    
}


void print_ir_code(FILE* fptr){
    ir_code* curr = global_ir_code;
    while(curr){
        fprintf(fptr,"%20s:=%20s\t%20s\t%20s\n",curr->result,curr->result,curr->left_op,(OPCODE)curr->operator,curr->result);
        curr = curr->next;      //TODO Debug for null
    }
}

int main(){
    initialize_ir_code();
    FILE* fptr = fopen("intermediate_code.txt","w+");
    print_ir_code(fptr);
    fclose(fptr);
    return 0;
}