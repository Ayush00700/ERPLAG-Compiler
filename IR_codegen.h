#include "IR_codegendef.h"
extern void generate_IR_for_module(ast_node* root,func_entry* local_ST,func_entry** global_ST);//STMTS
extern void print_ir_code(FILE* fptr,ir_code* intermediate_code);
extern ir_code* getIRList(ast_node* root, func_entry** global_ST);
extern char* codegen_assgn_stmt(ast_node* node,ir_code_node* ir, func_entry* local_ST,func_entry** global_ST);
