#include "codegendef.h"
extern void starter(FILE* assembly_file);
extern void codegen_arithmetic(ast_node* node,ir_code_node* ir, func_entry* local_ST,func_entry** global_ST);
extern void codegen_relational(ast_node* node,ir_code_node* ir, func_entry* local_ST,func_entry** global_ST);
extern void codegen_logical(ast_node* node,ir_code_node* ir, func_entry* local_ST,func_entry** global_ST);
extern void codegen_input(ast_node* node,ir_code_node* ir, func_entry* local_ST,func_entry** global_ST);
extern void codegen_output(ast_node* node,ir_code_node* ir, func_entry* local_ST,func_entry** global_ST);
