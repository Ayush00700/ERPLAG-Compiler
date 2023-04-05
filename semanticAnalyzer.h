#include "semanticAnalyzerDef.h"

// Function to add key into the table along with the entry number
extern int sym_tab_entry_add(char* key,var_record* local_table,type_exp temp);
// Checks if the key is there in the table
extern int sym_tab_entry_contains(char* key,sym_tab_entry* table[]);


//hash functions
extern func_entry* func_tab_entry_add(char* key,func_entry* table[],sym_tab_entry* input_list,sym_tab_entry* ouput_list,int* offset);
extern void list_add(sym_tab_entry* list,ast_node* ast_root,int* offset,int initial);
extern int func_tab_entry_contains(char* key,func_entry* table[]);
extern func_entry* find_module(char* key);


// semantic functions 
extern int check_static(ast_node* node);
extern int index_finder(ast_node* node);
extern void populate_symbol_table(ast_node* temp_node,type_exp temp,var_record* local_table);
extern void compute_expression(ast_node* ast_root,var_record* local_table);
extern void local_populate(var_record* local_table,ast_node* ast_root);
extern void func_def(ast_node* ast_root);
extern sym_tab_entry* getlist(ast_node* ast_root,int* offset);
extern void populate_(ast_node* ast_root);
extern type_exp* type_checking(ast_node* node, func_entry* curr);
extern void perform_type_checking(ast_node* ast_root,func_entry* func);
extern void semantic();
extern void get_global_symbol_table(ast_node* ast_root);
extern int sym_tab_entry_add(char* key,var_record* local_table,type_exp temp);
