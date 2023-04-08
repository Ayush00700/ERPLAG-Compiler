#include "codegendef.h"
extern void starter(FILE* assembly_file);
extern void codegen_arithmetic(ir_code_node* ir, func_entry* local_ST,func_entry** global_ST);
extern void codegen_relational(ir_code_node* ir, func_entry* local_ST,func_entry** global_ST);
extern void codegen_logical(ir_code_node* ir, func_entry* local_ST,func_entry** global_ST);
extern void codegen_input(ir_code_node* ir, func_entry* local_ST,func_entry** global_ST);
extern void codegen_output(ir_code_node* ir, func_entry* local_ST,func_entry** global_ST);
