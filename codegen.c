// #include <string.h>
// #include "IR_codegen.h"
// #include "codegen.h"

// typedef struct ir_code_node
/* The entry will be of the following form
    +------------+----------+----------+-----------+
    |  operator  |  result  |  left_op |  right_op |
    +------------+----------+----------+-----------+
*/
// {
//     OPCODE operator;
//     char* result;
//     char* left_op;
//     char* right_op;
//     struct ir_code_node* next;
// }ir_code_node;

// typedef struct ir_code
// {
//     ir_code_node* head;
//     ir_code_node* tail;
// }ir_code;

// typedef struct arr_struct{
//     int lower_bound;
//     int upper_bound;
//     char* arr_datatype; //real boolean integer
// }arr_struct;

// typedef struct type_exp{
//     char* datatype; //real boolean integer ; array 
//     int is_static; //1->static and 0->dynamic
//     arr_struct* arr_data;
// }type_exp;

// typedef struct sym_tab_entry{
//     char* name;
//     int offset;
//     type_exp type;
//     struct sym_tab_entry* next; // this is just to deal with collisions
// }sym_tab_entry;

// typedef struct var_record{
//     sym_tab_entry* entries[TABLE_SIZE];
//     struct var_record* parent;
//     struct var_record* child;
//     struct var_record* r_sibiling;
//     int offset;
//     char* construct_name;
// }var_record;

// typedef struct func_entry{
//     var_record* func_root; 
//     char* name;
//     // input and output
//     sym_tab_entry* input_list;
//     sym_tab_entry* ouput_list;
//     var_record* func_curr; //initialized to func_root
//     struct func_entry* next; // hash collision
//     int offset;
//     int visited;
// }func_entry;

// FILE* assembly;

// Required functions

// void codegen_unary_op(ir_code_node* ir, /* symbol table parameter*/)
// {
//     /* Handling the symbol table ops*/

//     // Writing the assembly for both unary ops

//     /* Handling the symbol table ops*/
// }

// void codegen_input(ir_code_node* ir, /* symbol table parameter*/)
// {
//     /* Handling the symbol table ops*/

//     // Writing the assembly for input

//     /* Handling the symbol table ops*/
// }

// void codegen_output(ir_code_node* ir, /* symbol table parameter*/)
// {
//     /* Handling the symbol table ops*/

//     // Writing the assembly for output

//     /* Handling the symbol table ops*/
// }

// void codegen_arithmetic_nodiv(ir_code_node* ir, /* symbol table parameter*/)
// {
//     /* Handling the symbol table ops*/

//     // Writing the assembly for each arith op
//     switch(ir.operator)
//     {
//         case ADD:
//             break;
//         case SUB:
//             break;
//         case MUL:
//             break;
//     }

//     /* Handling the symbol table ops*/
// }

// void codegen_div(ir_code_node* ir, /* symbol table parameter*/)
// {
//     /* Handling the symbol table ops*/

//     // Writing the assembly for division

//     /* Handling the symbol table ops*/
// }

// void codegen_relational(ir_code_node* ir, /* symbol table parameter*/)
// {
//     /* Handling the symbol table ops*/

//     // Writing the assembly for each rel op

//     /* Handling the symbol table ops*/
// }

// void codegen_boolean(ir_code_node* ir, /* symbol table parameter*/)
// {
//     /* Handling the symbol table ops*/

//     // Writing the assembly for each bool op

//     /* Handling the symbol table ops*/
// }

// void codegen_func(ir_code_node* ir, /* symbol table parameter*/)
// {
//     /* Handling the symbol table ops*/

//     // Writing the assembly for function related ops

//     /* Handling the symbol table ops*/
// }

// void codegen_jump(ir_code_node* ir, /* symbol table parameter*/)
// {
//     /* Handling the symbol table ops*/

//     // Writing the assembly for jump

//     /* Handling the symbol table ops*/
// }

// void codegen_conditional(ir_code_node* ir, /* symbol table parameter*/)
// {
//     /* Handling the symbol table ops*/

//     // Writing the assembly for conditional

//     /* Handling the symbol table ops*/
// }

// // void codegen_label(ir_code_node* ir, /* symbol table parameter*/)
// // {
// //     /* Handling the symbol table ops*/

// //     // Writing the assembly for label

// //     /* Handling the symbol table ops*/
// // }


// void codegen_wrapper(ir_code* IR,  /* symbol table*/, FILE* f)
// {
//     assembly = f;

//     // Write the external functions and start data section for assembly
//     fprintf(assembly, "extern printf, scanf, exit\n");
//     fprintf(assembly, "section      .data\n");

//     // Write down all the format specifiers reqd
//     fprintf(assembly, "\t\tfmt_spec_int: db \"%%d\", 10, 0\n");
//     fprintf(assembly, "\t\tfmt_spec_real: db \"%%4f\", 10, 0\n");
//     fprintf(assembly, "\t\tzero: dw 0\n");

//     /* Data declaration of various types to be done by going thro each entry of the symbol table*/

//     fprintf(assembly, "section      .text\n");
//     fprintf(assembly, "global main");

//     ir_code_node *IR_head = IR->head;

//     // Go through each entry of the IR quadruple
//     while(IR_head)
//     {
//         switch (IR_head.operator)
//         {
//         case ASSIGN:
//             // codegen_assgn_stmt(IR_head, /* symbol table param*/);
//             break;
        
//         case UNARY_PLUS:
//         case UNARY_MINUS:
//             codegen_unary_op(IR_head, /* symbol table param*/);
//             break;

//         case GET_VALUE:
//             codegen_input(IR_head, /* symbol table param*/);
//             break;
        
//         case PRINT:
//             codegen_output(IR_head, /* symbol table param*/);
//             break;

//         case ADD:
//         case SUB:
//         case MUL:
//             codegen_arithmetic_nodiv(IR_head, /* symbol table param*/);
//             break;
//         case DIV:
//             codegen_div(IR_head, /* symbol table param*/);
//             break;
        
//         case LT:
//         case LE:
//         case GT:
//         case GE:
//         case EQ:
//         case NEQ:
//             codegen_relational(IR_head, /* symbol table param*/);
//             break;
        
//         case OR:
//         case AND:
//             codegen_boolean(IR_head, /* symbol table param*/);
//             break;
        
//         case FUNC:
//         case CALL:
//         case PARA_IN:
//         case PARA_OUT:
//         case RET:
//             codegen_func(IR_head, /* symbol table param*/);
        
//         case GOTO:
//             codegen_jump(IR_head, /* symbol table param*/);
//             break;
        
//         case IF:
//             codegen_conditional(IR_head, /* symbol table param*/);
//             break;



//         // case LABEL:
//         //     codegen_label(IR_head, /* symbol table param*/);
//         //     break;
//         }

//         IR_head = IR_head->next;
//     }

// }
