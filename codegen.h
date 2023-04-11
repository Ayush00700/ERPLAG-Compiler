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
// This function generates assembly code for arithmetic statements
extern void codegen_arithmetic(ir_code_node* ir, func_entry* local_ST);
// This function generates assembly code for relational statements
extern void codegen_relational(ir_code_node* ir, func_entry* local_ST);
// This function generates assembly code for function definitions
extern void codegen_func(ir_code_node* ir, func_entry* local_ST);
// This function generates assembly code for conditional statements
extern void codegen_conditional(ir_code_node* ir, func_entry* local_ST);
// This function generates assembly code for labels for jump
extern void codegen_label(ir_code_node* ir, func_entry* local_ST);
// This function generates assembly code for static array read
extern void codegen_mem_read_ST(ir_code_node* ir, func_entry* local_ST);
// This function generates assembly code for static array write
extern void codegen_mem_write_ST(ir_code_node* ir, func_entry* local_ST);
// This function generates assembly code for dynamic array read
extern void codegen_mem_read(ir_code_node* ir, func_entry* local_ST);
// This function generates assembly code for dynamic array write
extern void codegen_mem_write(ir_code_node* ir, func_entry* local_ST);
// Code generation wrapper
extern void starter(FILE* assembly_file,ir_code* IR);

/*-----------------------------------------------------------HELPER FUNCTIONS----------------------------------------------------------------*/
extern type_exp* find_in_list_codegen(sym_tab_entry* output,char* key);
extern type_exp* find_in_func_table_codegen(char* key, func_entry* curr);
extern type_exp* find_in_table_codegen(char* key,var_record* table);
extern type_exp* find_expr_codegen(char* key, func_entry* curr);
extern void data_read(FILE* assembly);
