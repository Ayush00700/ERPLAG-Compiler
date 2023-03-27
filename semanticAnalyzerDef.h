#include "ast.h"

typedef union type_exp{
    char* datatype;
    char* arr_datatype;
    int lower_bound;
    int upper_bound;
    int is_static; //1->static and 0->dynamic
}type_exp;

typedef struct sym_tab_entry{
    char* name;
    int offset;
    type_exp type;
    struct sym_tab_entry* next; // this is just to deal with collisions
}sym_tab_entry;

typedef struct sym_table{
    sym_tab_entry* entries[TABLE_SIZE];
    struct sym_table* parent;
    struct sym_table* child;
    struct sym_table* r_sibiling;
}sym_table;

typedef struct func_entry{
    sym_table* func_info; 
    // input and output
    sym_tab_entry* input_list;
    sym_tab_entry* ouput_list;
}func_entry;

