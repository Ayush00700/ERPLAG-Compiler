#include "codegendef.h"
extern void starter(/*ir_code* IR   symbol table, FILE* f*/);
extern char* codegen_arithmetic(ast_node* node,ir_code_node* ir, func_entry* local_ST,func_entry** global_ST);
extern char* codegen_relational(ast_node* node,ir_code_node* ir, func_entry* local_ST,func_entry** global_ST);
extern char* codegen_logical(ast_node* node,ir_code_node* ir, func_entry* local_ST,func_entry** global_ST);
extern char* codegen_input(ast_node* node,ir_code_node* ir, func_entry* local_ST,func_entry** global_ST);
extern char* codegen_output(ast_node* node,ir_code_node* ir, func_entry* local_ST,func_entry** global_ST);
