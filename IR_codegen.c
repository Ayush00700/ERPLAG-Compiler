#include <string.h>
#include "IR_codegen.h"

typedef struct ir_code
{
    char* result;
    char* left_op;
    char* operator;
    char* right_op;
    struct ir_code* next;
}ir_code;

ir_code* global_ir_code;

void initialize_ir_code(){
    
    global_ir_code = (ir_code*)malloc(sizeof(ir_code));
    global_ir_code->left_op = 
    global_ir_code->next = NULL;
}
int main(){
    initialize_ir_code();
    return 0;
}