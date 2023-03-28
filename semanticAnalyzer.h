#include "semanticAnalyzerDef.h"

// Function to add key into the table along with the entry number
extern int sym_tab_entry_add(char* key,sym_tab_entry* table[],type_exp temp);
// Checks if the key is there in the table
extern int sym_tab_entry_contains(char* key,sym_tab_entry* table[]);


// semantic functions 
extern int check_static(ast_node* node);
extern int index_finder(ast_node* node);
extern void populate_symbol_table(ast_node* temp_node,type_exp temp);
extern void compute_expression(ast_node* ast_root);
extern void populate_(ast_node* ast_root);
extern void semantic();