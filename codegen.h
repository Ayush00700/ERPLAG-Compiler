#include "codegendef.h"
// This function defines the required macros in the assembly file
extern void macros_starter();
// This function generates assembly code for assignment statements
extern void codegen_assgn_stmt(ir_code_node* ir, func_entry* local_ST);
// This function generates assembly code for logical statements
extern void codegen_logical(ir_code_node* ir, func_entry* local_ST);
// This function generates assembly code for input statements
extern void codegen_input(ir_code_node* ir, func_entry* local_ST);
// This function generates assembly code for output statements
extern void codegen_output(ir_code_node* ir, func_entry* local_ST);
// extern void starter(FILE* assembly_file,ir_code* IR);
// extern void codegen_relational(ir_code_node* ir, func_entry* local_ST);
// extern void codegen_arithmetic(ir_code_node* ir, func_entry* local_ST);
// extern void codegen_logical(ir_code_node* ir, func_entry* local_ST);
// extern void codegen_input(ir_code_node* ir, func_entry* local_ST);
// extern void codegen_output(ir_code_node* ir, func_entry* local_ST);
// extern void codegen_switch(ir_code_node* ir, func_entry* local_ST);
// extern void codegen_iterative(ir_code_node* ir, func_entry* local_ST);
// extern void codegen_procedure(ir_code_node* ir, func_entry* local_ST);
// extern type_exp* find_expr_codegen(char* key, func_entry* curr);
// extern void data_read(FILE* assembly);
