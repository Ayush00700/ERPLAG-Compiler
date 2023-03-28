#include "ast.h"

typedef struct arr_struct{
    int lower_bound;
    int upper_bound;
    char* arr_datatype;
}arr_struct;

typedef struct type_exp{
    char* datatype;
    int is_static; //1->static and 0->dynamic
    arr_struct* arr_data;
}type_exp;

typedef struct sym_tab_entry{
    char* name;
    int offset;
    type_exp type;
    struct sym_tab_entry* next; // this is just to deal with collisions
}sym_tab_entry;

typedef struct var_record{
    sym_tab_entry* entries[TABLE_SIZE];
    struct var_record* parent;
    struct var_record* child;
    struct var_record* r_sibiling;
    int offset;
}var_record;

typedef struct func_entry{
    var_record* func_root; 
    char* name;
    // input and output
    sym_tab_entry* input_list;
    sym_tab_entry* ouput_list;
    var_record* func_curr;
    struct func_entry* next;
    int offset;
}func_entry;

