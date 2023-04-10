#include "codegendef.h"
extern void starter(FILE* assembly_file,ir_code* IR);
extern void codegen_relational(ir_code_node* ir, func_entry* local_ST);
extern void codegen_arithmetic(ir_code_node* ir, func_entry* local_ST);
extern void codegen_logical(ir_code_node* ir, func_entry* local_ST);
extern void codegen_input(ir_code_node* ir, func_entry* local_ST);
extern void codegen_output(ir_code_node* ir, func_entry* local_ST);
extern void codegen_switch(ir_code_node* ir, func_entry* local_ST);
extern void codegen_iterative(ir_code_node* ir, func_entry* local_ST);
extern void codegen_procedure(ir_code_node* ir, func_entry* local_ST);
