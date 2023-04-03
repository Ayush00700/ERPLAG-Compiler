#include <string.h>
#include "codegen.h"

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

typedef struct sym_tab_entry{
    char* name;
    int offset;
    type_exp type;
    struct sym_tab_entry* next; // this is just to deal with collisions
}sym_tab_entry;

typedef struct var_record{
    sym_tab_entry* entries[TABLE_SIZE];
    struct var_record* parent;
    struct var_record* child;
    struct var_record* r_sibiling;
    int offset;
    char* construct_name;
}var_record;

typedef struct func_entry{
    var_record* func_root; 
    char* name;
    // input and output
    sym_tab_entry* input_list;
    sym_tab_entry* ouput_list;
    var_record* func_curr; //initialized to func_root
    struct func_entry* next; // hash collision
    int offset;
    int visited;
}func_entry;

FILE* assembly;

void codegen(ir_code* IR ){
    
}
