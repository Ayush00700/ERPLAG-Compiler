#include "IR_codegendef.h"
extern void generate_IR_for_module(ast_node* root,func_entry* local_ST,func_entry** global_ST);//STMTS
extern void print_ir_code(FILE* fptr,ir_code* intermediate_code);
extern ir_code* getIRList(ast_node* root, func_entry** global_ST);
// extern void codegen_wrapper(ir_code* IR  /* symbol table, FILE* f*/);
extern void IR_arrayAssign(ast_node* node,func_entry* local_ST,func_entry** global_ST,ast_node* nodeExp2);
extern char* newTemp();
extern char* newLabel();
