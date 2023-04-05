#include "ast.h"

typedef enum semErrors{
    TYPE_NOT_MATCHED,OUT_OF_SCOPE_VARIABLE,
    UNSUPPORTED_DTYPE,FUNC_NOT_DEFINED, OUT_OF_ORDER_INDEX
} semErrors;

typedef struct arr_struct{
    int lower_bound;
    int upper_bound;
    char* arr_datatype; //real boolean integer
}arr_struct;

typedef struct type_exp{
    char* datatype; //real boolean integer ; array 
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
    char* construct_name;
}var_record;

typedef struct func_entry{
    var_record* func_root; 
    char* name;
    // input and output
    sym_tab_entry* input_list; //use 
    sym_tab_entry* ouput_list;
    var_record* func_curr; //initialized to func_root
    struct func_entry* next; // hash collision
    int offset;
    int visited;
}func_entry;

