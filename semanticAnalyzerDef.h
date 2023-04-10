#include "ast.h"

typedef enum semErrors{
    TYPE_NOT_MATCHED,OUT_OF_SCOPE_VARIABLE,
    UNSUPPORTED_DTYPE,FUNC_NOT_DEFINED, OUT_OF_ORDER_INDEX,
    RECURSION_NOT_ALLOWED ,PARAMETER_LIST_MISMATCH, DEFAULT_NOT_FOUND,
     DEFAULT_FOUND, INDEX_OUT_OF_BOUNDS, VAR_REDECLARED,VALUE_MODIFIED, VALUE_NOT_MODIFIED,FUNCTION_NOT_DEFINED
} semErrors;

typedef struct arr_struct{
    int lower_bound;
    int upper_bound;
    char* arr_datatype; //real boolean integer
}arr_struct;

// typedef union var_change{
//     var_record* loop;
//     sym_tab_entry* output;
// }var_change;

// typedef struct con_change{
//     var_change construct;
//     int conType;//1 is for, 2 is while, 3 is output value
//     char* construct_name;
//     con_change* next;
//     con_change* prev;
// }con_change;

// typedef struct List{
//     con_change* head;
//     int count; 
// }List;

typedef struct type_exp{
    char* datatype; //real boolean integer ; array 
    int is_static; //1->static and 0->dynamic
    int offset;
    arr_struct* arr_data;
    int line_defined;
    int isChanged;
}type_exp;

typedef struct sym_tab_entry{
    char* name;
    int offset;
    type_exp type;
    struct sym_tab_entry* next; // this is just to deal with collisions
    // int isChanged;
}sym_tab_entry;

typedef struct var_record{
    sym_tab_entry* entries[TABLE_SIZE];
    struct var_record* parent;
    struct var_record* child;
    struct var_record* r_sibiling;
    int offset;
    // int isChanged;
    char* construct_name;
    // int nesting;
    // int sibiling;
    char* reach;
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
    int defined;
}func_entry;

