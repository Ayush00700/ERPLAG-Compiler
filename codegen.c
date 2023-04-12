#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "codegen.h"

FILE* assembly;
extern char* exitJumpLabel;
void macros_starter()
{
    /* This function defines the required macros in the assembly file
    */

    // Macro to store context
    fprintf(assembly, "\t\t;macro to store all registers\n");
    fprintf(assembly, "\t\t%%macro  push_regs    0\n");
    fprintf(assembly, "\t\tpush      rax\n");
    fprintf(assembly, "\t\tpush      rbx\n");
    fprintf(assembly, "\t\tpush      rcx\n");
    fprintf(assembly, "\t\tpush      rdx\n");
    fprintf(assembly, "\t\tpush      rsi\n");
    fprintf(assembly, "\t\tpush      rdi\n");
    // fprintf(assembly, "\t\tpush      rbp\n");
    // fprintf(assembly, "\t\tpush      rsp\n");
    fprintf(assembly, "\t\t%%endmacro\n\n\n");

    // Macro to restore context
    fprintf(assembly, "\t\t;macro to restore all registers\n");
    fprintf(assembly, "\t\t%%macro  pop_regs    0\n");
    // fprintf(assembly, "\t\tpop      rsp\n");
    // fprintf(assembly, "\t\tpop      rbp\n");
    fprintf(assembly, "\t\tpop      rdi\n");
    fprintf(assembly, "\t\tpop      rsi\n");
    fprintf(assembly, "\t\tpop      rdx\n");
    fprintf(assembly, "\t\tpop      rcx\n");
    fprintf(assembly, "\t\tpop      rbx\n");
    fprintf(assembly, "\t\tpop      rax\n");
    fprintf(assembly, "\t\t%%endmacro\n\n\n");

    // Macro to align stack to 16-byte offset
    fprintf(assembly, "\t\t;macro to align RSP\n");
    fprintf(assembly, "\t\t%%macro  rsp_align    0\n");
    fprintf(assembly, "\t\tpush      rbx\n");
    fprintf(assembly, "\t\tpush      rdx\n");
    fprintf(assembly, "\t\tpush      rax\n");
    fprintf(assembly, "\t\txor       rdx , rdx\n");
    fprintf(assembly, "\t\tmov       rax , rsp\n");
    fprintf(assembly, "\t\tmov       rbx , 16\n");
    fprintf(assembly, "\t\tidiv      rbx\n");
    fprintf(assembly, "\t\tmov       rbx , rdx\n");
    fprintf(assembly, "\t\tpop       rax\n");
    fprintf(assembly, "\t\tpop       rdx\n");
    fprintf(assembly, "\t\tsub       rsp , rbx\n");
    fprintf(assembly, "\t\t%%endmacro\n\n\n");

    // Macro to realign stack
    fprintf(assembly, "\t\t;macro to re-align RSP\n");
    fprintf(assembly, "\t\t%%macro  rsp_realign    0\n");
    fprintf(assembly, "\t\tadd      rsp , rbx\n");
    fprintf(assembly, "\t\tpop      rbx\n");
    fprintf(assembly, "\t\t%%endmacro\n\n\n");
}

void codegen_assgn_stmt(ir_code_node* ir, func_entry* local_ST)
{
    /* The entry will be of the following form
    +------------+----------+----------+-----------+
    |   ASSIGN   |    lhs   |    rhs   |    NULL   |
    +------------+----------+----------+-----------+
    Input: 
        - IR 3AC for operation and operands
        - Local symbol table for info regarding the local variables
    Output:
        - Assembly code for assignment statements
    */

    char* nameRHS = ir->left_op->name; 
    type_exp* rhsTypeExp = find_expr_codegen(nameRHS,local_ST);
    int offsetRHS;
    int indexRHS = 0;
    if(rhsTypeExp){
        offsetRHS =  rhsTypeExp->offset * 16;
    }
    else{
        indexRHS = -1;
    }

    char* nameLHS = ir->result->name; 
    type_exp* lhsTypeExp = find_expr_codegen(nameLHS,local_ST);
    int offsetLHS;
    int indexLHS = 0;
    if(lhsTypeExp){
        offsetLHS =  lhsTypeExp->offset * 16;
    }
    else{
        indexLHS = -1;
    }
    
    //TODO delete these comments after debug

    // char* nameRHS = ir->left_op->name;      // temporary variable name for rhs

    // char* nameLHS = ir->result->name;      // temporary variable name for rhs

    // Finding the symbol table entry for lhs variable
    // int indexLHS = sym_tab_entry_contains(nameLHS,local_ST->func_curr->entries);        // Checks if the symbol table contains - if yes we get the index
    // sym_tab_entry* temp = NULL;
    // if(indexLHS!=-1)
    // temp = local_ST->func_curr->entries[indexLHS];
    // while(temp!=NULL){  //Hashing collision
    //     if(!strcmp(temp->name,nameLHS)){
    //         break;
    //     }
    //     temp = temp->next;
    // }

    // int offsetLHS = 0;
    // if(indexLHS!=-1)offsetLHS = temp->offset*16;               // Get the memory offset for the lhs variable from the symbol

    // int indexRHS = sym_tab_entry_contains(nameRHS,local_ST->func_curr->entries);        // Check if RHS is a expression/ variable/ a constant

    fprintf(assembly, "\t\tpush_regs                    ; save values\n");
    fprintf(assembly, "\t\txor      rax , rax           ; flush out the rax register\n");

    // If RHS is a constant (immediate value in ASM jargon)
    if(indexRHS==-1){
        // CHECK ------> detect if immediate value is int or real
        int type_right_int = (strchr(nameRHS, '.'))? 0 : 1;
        if(type_right_int)
        {    
            if(!(strcmp(nameRHS,"true"))||!(strcmp(nameRHS,"false"))){
                int bool_temp = 0;
                if(!strcmp(nameRHS,"true")) bool_temp =1;
                fprintf(assembly, "\t\tmov      rax , %d                    ; immediate to register\n",bool_temp);
                
                fprintf(assembly, "\t\tmov      [RBP - %d] , rax            ; register to memory\n",offsetLHS);

            }
            else{
                fprintf(assembly, "\t\tmov      rax , %s                    ; immediate to register\n",nameRHS);
                
                fprintf(assembly, "\t\tmov      [RBP - %d] , rax            ; register to memory\n",offsetLHS);
            }

        }

        // RHS is REAL
        else
        {
            fprintf(assembly, "\t\t; Code for assigning immediate REAL\n");
            fprintf(assembly, "\t\tmovsd      xmm0 , %s                    ; immediate to register\n",nameRHS);
            
            fprintf(assembly, "\t\tmovsd      [RBP - %d] , xmm0            ; register to memory\n",offsetLHS);
            
        }
    }

    // If RHS not constant
    else
    {
        // temp = local_ST->func_curr->entries[indexRHS];
        // while(temp!=NULL){
        //     if(!strcmp(temp->name,nameRHS))
        //     {
        //         break;
        //     }
        //     temp = temp->next;
        // }

        // int offsetRHS = temp->offset*16;               // Get the memory offset for the rhs variable from the symbol

        if(!strcmp(rhsTypeExp->datatype, "integer")||!strcmp(rhsTypeExp->datatype, "boolean"))
        {
            fprintf(assembly, "\t\tmov      rax , [RBP - %d]                    ; memory to register\n",offsetRHS);
            
            fprintf(assembly, "\t\tmov      [RBP - %d] , rax            ; register to memory\n",offsetLHS);
        }

        // REAL
        else
        {
            fprintf(assembly, "\t\tmovsd      xmm0 , [RBP - %d]                    ; memory to register\n",offsetRHS);
            
            fprintf(assembly, "\t\tmovsd     [RBP - %d] , xmm0            ; register to memory\n",offsetLHS);
            
        }
    }
    
    fprintf(assembly, "\t\tpop_regs                    ; restore register values\n\n\n");
   
}

void codegen_logical(ir_code_node* ir, func_entry* local_ST)
{
    /* The entry will be of the following form
    +------------+----------+----------+-----------+
    |    label   |    temp  |    op1   |    op2    |
    +------------+----------+----------+-----------+
    label can be one of AND, OR
    Input: 
        - IR 3AC for operation and operands
        - Local symbol table for info regarding the local variables
    Output:
        - Assembly code for logical statements
    */

    char* result = ir->result->name; 
    type_exp* resTypeExp = find_expr_codegen(result,local_ST);
    int offsetResult;
    char* resultType;
    int indexResult = 0;
    if(resTypeExp){
        offsetResult =  resTypeExp->offset * 16;
        resultType = resTypeExp->datatype;
    }
    else{
        indexResult = -1;
    }

    char* nameLeft = ir->left_op->name; 
    type_exp* lhsTypeExp = find_expr_codegen(nameLeft,local_ST);
    int offsetLeft;
    char* leftType;
    int indexLeft = 0;
    if(lhsTypeExp){
        offsetLeft =  lhsTypeExp->offset * 16;
        leftType = lhsTypeExp->datatype;
    }
    else{
        indexLeft = -1;
    }

    char* nameRight = ir->right_op->name; 
    type_exp* rhsTypeExp = find_expr_codegen(nameRight,local_ST);
    int offsetRight;
    char* rightType;
    int indexRight = 0;
    if(rhsTypeExp){
        offsetRight =  rhsTypeExp->offset * 16;
        rightType = rhsTypeExp->datatype;
    }
    else{
        indexRight = -1;
    }





    // char* result = ir->result->name;                // temporary variable name for result
    
    fprintf(assembly, "\t\t; Code for logical op\n");
    fprintf(assembly, "\t\tpush_regs                    ; save values\n");
    
    // If left operand is a constant
    if(indexLeft == -1)
    {
        // Since the operand will be 0 or 1 for logical
        fprintf(assembly, "\t\tmov     rax , %s            ; immediate to memory\n", nameLeft);
    }

    else
    {
        // temp = local_ST->func_curr->entries[indexLeft];
        // while(temp!=NULL){
        //     if(!strcmp(temp->name,nameLeft)){
        //         break;
        //     }
        //     temp = temp->next;
        // }
        // int offsetLeft = temp->offset*16;               // Get the memory offset for the lhs variable from the symbol

        // if(!strcmp(resultType, "integer"))
        // {
            fprintf(assembly, "\t\tmov     rax , [RBP - %d]\n", offsetLeft);
            
        // }
        // else
        // {
        //     fprintf(assembly, "\t\tmovsd     xmm0 , [RBP - %d]\n", offsetLeft);
        // }
        
    }

    // For right operand
    // char* nameRight;
    // int indexRight;
    
    switch(ir->operator)
    {
        case AND:
            // Get offset of right operand temp
            // nameRight = ir->right_op->name;
            // indexRight = sym_tab_entry_contains(nameRight,local_ST->func_curr->entries);        // Checks if the symbol table contains - if yes we get the index
            
            // If right operand is a constant
            if(indexRight == -1)
            {
                // int type_right_int = (strchr(nameRight, '.'))? 0 : 1;

                // CHECK ----> identifying if immediate value is int or real
                // if(type_right_int)
                // {
                    fprintf(assembly, "\t\tand     rax , %s\n", nameRight);
                    
                    fprintf(assembly, "\t\tmov     [RBP - %d] , rax\n", offsetResult);
                    
                // }

                // else
                // {
                //     fprintf(assembly, "\t\tandpd     xmm0 , %s\n", nameRight);
                    
                //     fprintf(assembly, "\t\tmovsd     [RBP - %d] , xmm0\n", offsetResult);
                    
                // }
            }

            else
            {
                // temp = local_ST->func_curr->entries[indexRight];
                // while(temp!=NULL){
                //     if(!strcmp(temp->name,nameRight)){
                //         break;
                //     }
                //     temp = temp->next;
                // }
                // int offsetRight = temp->offset*16;               // Get the memory offset for the lhs variable from the symbol

                // if(!strcmp(resultType, "integer"))
                // {
                    fprintf(assembly, "\t\tand     rax , [RBP - %d]\n", offsetRight);
                    
                    fprintf(assembly, "\t\tmov     [RBP - %d] , rax\n", offsetResult);
                    
                // }
                // else
                // {
                //     fprintf(assembly, "\t\tandpd     xmm0 , [RBP - %d]\n", offsetRight);
                    
                //     fprintf(assembly, "\t\tmovsd     [RBP - %d] , xmm0\n", offsetResult);
                    
                // }
            }
            break;
        
        case OR:
            // Get offset of right operand temp
            // nameRight = ir->right_op->name;
            // indexRight = sym_tab_entry_contains(nameRight,local_ST->func_curr->entries);        // Checks if the symbol table contains - if yes we get the index
            
            // If right operand is a constant
            if(indexRight == -1)
            {
                int type_right_int = (strchr(nameRight, '.'))? 0 : 1;
                // CHECK ----> identifying if immediate value is int or real
                // if(type_right_int)
                // {
                    fprintf(assembly, "\t\tor     rax , %s\n", nameRight);
                    
                    fprintf(assembly, "\t\tmov     [RBP - %d] , rax\n", offsetResult);
                    
                // }

                // else
                // {
                //     fprintf(assembly, "\t\torpd     xmm0 , %s\n", nameRight);
                    
                //     fprintf(assembly, "\t\tmovsd     [RBP - %d] , xmm0\n", offsetResult);
                    
                // }
            }

            else
            {
                // temp = local_ST->func_curr->entries[indexRight];
                // while(temp!=NULL){
                //     if(!strcmp(temp->name,nameRight)){
                //         break;
                //     }
                //     temp = temp->next;
                // }
                // int offsetRight = temp->offset*16;               // Get the memory offset for the lhs variable from the symbol

                // if(!strcmp(resultType, "integer"))
                // {
                    fprintf(assembly, "\t\tor     rax , [RBP - %d]\n", offsetRight);
                    
                    fprintf(assembly, "\t\tmov     [RBP - %d] , rax\n", offsetResult);
                    
                // }
                // else
                // {
                //     fprintf(assembly, "\t\torpd     xmm0 , [RBP - %d]\n", offsetRight);
                    
                //     fprintf(assembly, "\t\tmovsd     [RBP - %d] , xmm0\n", offsetResult);
                    
                // }
            }
            break;
    }

    fprintf(assembly, "\t\tpop_regs        ; restore register values\n\n\n");

}

void codegen_input(ir_code_node* ir, func_entry* local_ST)
{
    /* The entry will be of the following form
    +------------+----------+----------+-----------+
    |  GET_VALUE |    var   |   NULL   |    NULL   |
    +------------+----------+----------+-----------+
    Input: 
        - IR 3AC for operation and operands
        - Local symbol table for info regarding the local variables
    Output:
        - Assembly code for input statements
    */

    // Get offset of result

    char* result = ir->result->name; 
    type_exp* rhsTypeExp = find_expr_codegen(result,local_ST);
    int offsetResult;
    char* resultType;
    int indexRight = 0;
    if(rhsTypeExp){
        offsetResult =  rhsTypeExp->offset * 16;
        resultType = rhsTypeExp->datatype;
    }
    else{
        indexRight = -1;
    }

    fprintf(assembly, "\t\t; Code for getting user input\n");
    fprintf(assembly, "\t\tpush_regs                    ; save values\n");
    
    // Set up format specifier based on data type
    if(!strcmp(resultType, "integer"))
    {
        fprintf(assembly, "\t\t; Display prompt for integer input\n");
        fprintf(assembly, "\t\tmov      rax , 1\n");
        fprintf(assembly, "\t\tmov      rdi , 1\n");
        fprintf(assembly, "\t\tmov      rsi , integer_in\n");
        fprintf(assembly, "\t\tmov      rdx , integer_in_len\n");
        fprintf(assembly, "\t\tsyscall\n");

        fprintf(assembly, "\n\t\t; Code to get integer input\n");
        fprintf(assembly, "\t\tmov      rdi , fmt_spec_int_in          ; get corresponding format specifier\n");
        
    }
    else if(!strcmp(resultType, "real"))
    {
        fprintf(assembly, "\t\t; Display prompt for real input\n");
        fprintf(assembly, "\t\tmov      rax , 1\n");
        fprintf(assembly, "\t\tmov      rdi , 1\n");
        fprintf(assembly, "\t\tmov      rsi , real_in\n");
        fprintf(assembly, "\t\tmov      rdx , real_in_len\n");
        fprintf(assembly, "\t\tsyscall\n");

        fprintf(assembly, "\n\t\t; Code to get real input\n");
        fprintf(assembly, "\t\tmov      rdi , fmt_spec_real_in          ; get corresponding format specifier\n");
        
    }
    else if(!strcmp(resultType, "boolean"))
    {
        fprintf(assembly, "\t\t; Display prompt for boolean input\n");
        fprintf(assembly, "\t\tmov      rax , 1\n");
        fprintf(assembly, "\t\tmov      rdi , 1\n");
        fprintf(assembly, "\t\tmov      rsi , boolean_in\n");
        fprintf(assembly, "\t\tmov      rdx , boolean_in_len\n");
        fprintf(assembly, "\t\tsyscall\n");

        fprintf(assembly, "\n\t\t; Code to get boolean input\n");
        fprintf(assembly, "\t\tmov      rdi , fmt_spec_bool_in          ; get corresponding format specifier\n");
        
    }

    fprintf(assembly,"\t\t\t\tmov RDX, RBP\n\
                sub RDX, %d     ; make RDX to point at location of variable on the stack\n\
                mov RAX, 0x0000_0000_0000_0000 ;machine has sizeof(int) to be 2, for uniformity, We are taking\n\
                mov [RDX] , RAX ;our sizeof(int) to be 4, now scanf will just enter values in lower 32 bits\n\
                mov RSI, RBP\n\
                sub RSI, %d \n\
                mov RAX, 0 \n\
                rsp_align ;align RSP to 16 byte boundary for scanf call\n\
                call scanf \n\
                rsp_realign ;realign it to original position\n\
                ", offsetResult,offsetResult);

    fprintf(assembly, "\t\tpop_regs        ; restore register values\n\n\n");

}

void codegen_output(ir_code_node* ir, func_entry* local_ST)
{
    /* The entry will be of the following form
    +------------+----------+----------+-----------+
    |    PRINT   |    var   |   NULL   |    NULL   |
    +------------+----------+----------+-----------+
    Input: 
        - IR 3AC for operation and operands
        - Local symbol table for info regarding the local variables
    Output:
        - Assembly code for output statements
    */
    
    // Get offset of result
    char* result = ir->result->name; 
    type_exp* rhsTypeExp = find_expr_codegen(result,local_ST);
    int offsetResult;
    char* resultType;
    int indexResult = 0;
    if(rhsTypeExp){
        offsetResult =  rhsTypeExp->offset * 16;
        resultType = rhsTypeExp->datatype;
    }
    else{
        indexResult = -1;
    }

    if(indexResult==-1)
    {   
        // BOOLEAN true
        if(!strcmp(result, "true"))
        {
            fprintf(assembly, "\t\t; Code for printing boolean\n");

            fprintf(assembly, "\t\tmov      rax , 1\n");
            fprintf(assembly, "\t\tmov      rdi , 1\n");
            fprintf(assembly, "\t\tmov      rsi , print_out\n");
            fprintf(assembly, "\t\tmov      rdx , print_out_len\n");
            fprintf(assembly, "\t\tsyscall\n");

            fprintf(assembly, "\n\t\tpush_regs                    ; save values\n");
            fprintf(assembly, "\t\tmov rax, 1                  ; system call for write\n");
            fprintf(assembly, "\t\tmov rdi, 1                  ; file handle 1 is stdout\n");
            fprintf(assembly, "\t\tmov rsi, true               ; address of string to output\n");
            fprintf(assembly, "\t\tmov rdx, true_len           ; number of bytes\n");
            fprintf(assembly, "\t\tsyscall                     ; invoke operating system to do the write\n");
        }
        
        // BOOLEAN false
        else if(!strcmp(result, "false"))
        {
            fprintf(assembly, "\t\t; Code for printing  boolean\n");

            fprintf(assembly, "\t\tmov      rax , 1\n");
            fprintf(assembly, "\t\tmov      rdi , 1\n");
            fprintf(assembly, "\t\tmov      rsi , print_out\n");
            fprintf(assembly, "\t\tmov      rdx , print_out_len\n");
            fprintf(assembly, "\t\tsyscall\n");

            fprintf(assembly, "\n\t\tpush_regs                    ; save values\n");
            fprintf(assembly, "\t\tmov rax, 1                  ; system call for write\n");
            fprintf(assembly, "\t\tmov rdi, 1                  ; file handle 1 is stdout\n");
            fprintf(assembly, "\t\tmov rsi, false               ; address of string to output\n");
            fprintf(assembly, "\t\tmov rdx, false_len           ; number of bytes\n");
            fprintf(assembly, "\t\tsyscall                     ; invoke operating system to do the write\n");
        }

        // INT or REAL
        else
        {
            // If strchr(nameRight, '.') returns null then it means right operand has no '.'
            // and hence it is an integer constant (immediate value)
            int type_left_int = (strchr(result, '.'))? -1 : 1;
            
            // INT
            if(type_left_int==1){
                fprintf(assembly, "\t\t; Code for printing output\n");
                
                fprintf(assembly, "\t\tpush_regs                    ; save values\n");
                

                fprintf(assembly, "\t\tmov      rdi , fmt_spec_int_out                ; get corresponding format specifier\n");
                

                fprintf(assembly, "\t\tmov      rsi , %s                               ; move source index\n", result);
                
                fprintf(assembly, "\t\txor      rax , rax\n");
                fprintf(assembly, "\t\trsp_align                                     ; align stack pointer\n");
                fprintf(assembly, "\t\tcall     printf                               ; system call for output\n");
                fprintf(assembly, "\t\trsp_realign                                   ; restore previous alignment of stack\n");

            }

            // FLOAT
            else if(type_left_int==-1){
                fprintf(assembly, "\t\t; Code for printing output\n");
                
                fprintf(assembly, "\t\tpush_regs                    ; save values\n");
                

                fprintf(assembly, "\t\tmov      rdi , fmt_spec_real_out                ; get corresponding format specifier\n");
                

                fprintf(assembly, "\t\tmov      rsi , %s                               ; move source index\n", result);
                
                fprintf(assembly, "\t\txor      rax , rax\n");
                fprintf(assembly, "\t\trsp_align                                     ; align stack pointer\n");
                fprintf(assembly, "\t\tcall     printf                               ; system call for output\n");
                fprintf(assembly, "\t\trsp_realign                                   ; restore previous alignment of stack\n");
            }
        }
    }

    else
    {

        fprintf(assembly, "\t\t; Code for printing output\n");

        fprintf(assembly, "\t\tmov      rax , 1\n");
        fprintf(assembly, "\t\tmov      rdi , 1\n");
        fprintf(assembly, "\t\tmov      rsi , print_out\n");
        fprintf(assembly, "\t\tmov      rdx , print_out_len\n");
        fprintf(assembly, "\t\tsyscall\n");

        fprintf(assembly, "\n\t\tpush_regs                                         ; save values\n");
        
        // INT
        if(!strcmp(resultType, "integer"))
        {
            fprintf(assembly, "\t\tmov      rdi , fmt_spec_int_out                  ; get corresponding format specifier\n");
            
            // fprintf(assembly, "\t\tmov RAX, 0x0000_0000_0000_ffff ;machine has sizeof(int) to be 2, for uniformity, We are taking\n");

            // fprintf(assembly, "\t\tmov [RBP-%d] , RAX ;our sizeof(int) to be 4, now scanf will just enter values in lower 32 bits\n                  ; get corresponding format specifier\n",offsetResult);

            // fprintf(assembly, "\t\tmov      rdx , rbp                               ; take base pointer in rdx\n");
            
            // fprintf(assembly, "\t\tsub      rdx , %d                                ; move pointer to place from where we have to read\n", offsetResult);
            
            fprintf(assembly, "\t\tmov      rsi , [RBP - %d]                               ; move source index\n",offsetResult);
            
            fprintf(assembly, "\t\txor      rax , rax\n");
            fprintf(assembly, "\t\trsp_align                                         ; align stack pointer\n");
            fprintf(assembly, "\t\tcall     printf                                   ; system call for output\n");
            fprintf(assembly, "\t\trsp_realign                                       ; restore previous alignment of stack\n");
            fprintf(assembly, "\t\tpop_regs                                          ; restore values\n");

        }

        // REAL
        else if(!strcmp(resultType, "real"))
        {
            fprintf(assembly, "\t\tmov      rdi , fmt_spec_real_out          ; get corresponding format specifier\n");
            
            // fprintf(assembly, "\t\tmov RAX, 0x0000_0000_ffff_ffff ;machine has sizeof(int) to be 2, for uniformity, We are taking\n");

            // fprintf(assembly, "\t\tmov [RBP-%d] , RAX ;our sizeof(int) to be 4, now scanf will just enter values in lower 32 bits\n                  ; get corresponding format specifier\n",offsetResult);

            // fprintf(assembly, "\t\tmov      rdx , rbp                               ; take base pointer in rdx\n");
            
            // fprintf(assembly, "\t\tsub      rdx , %d                                ; move pointer to place from where we have to read\n", offsetResult);
            
            fprintf(assembly, "\t\tmovq      xmm0 ,[RBP - %d]                               ; move source index\n",offsetResult);
            
            fprintf(assembly, "\t\tmov      rax , 1\n");
            fprintf(assembly, "\t\tcall     printf                                   ; system call for output\n");
            fprintf(assembly, "\t\tpop_regs                                          ; restore values\n");
            
        }

        // BOOLEAN
        else if(!strcmp(resultType, "boolean"))
        {
            // Set up the format specifier based on the data type
            fprintf(assembly, "\t\tmov      rdi , fmt_spec_bool_out                  ; get corresponding format specifier\n");
            
            // Labels to the jumps based on the value
            char* true_label = newLabel();
            char* next_label = newLabel();
            
            fprintf(assembly, "\t\tmov      rax , [RBP - %d]                               ; move source index\n",offsetResult);
            
            fprintf(assembly, "\t\tcmp      rax , 0\n");
            
            fprintf(assembly, "\t\tjne      %s\n", true_label);
            
            fprintf(assembly, "\t\tmov      rax , 1\n");
            fprintf(assembly, "\t\tmov      rdi , 1\n");
            fprintf(assembly, "\t\tmov      rsi , false\n");
            fprintf(assembly, "\t\tmov      rdx , false_len\n");
            fprintf(assembly, "\t\tsyscall\n");
            fprintf(assembly, "\t\tpop_regs                                          ; restore register values\n");
            fprintf(assembly, "\t\tjmp  %s\n\n\n",next_label);
            
            fprintf(assembly, "%s:\n", true_label);
            fprintf(assembly, "\t\tmov      rax , 1\n");
            fprintf(assembly, "\t\tmov      rdi , 1\n");
            fprintf(assembly, "\t\tmov      rsi , true\n");
            fprintf(assembly, "\t\tmov      rdx , true_len\n");
            fprintf(assembly, "\t\tsyscall\n");
            fprintf(assembly, "\t\tpop_regs        ; restore register values\n");
            fprintf(assembly, "%s:\n\n\n", next_label);
        }
    }    
    
}

void codegen_arithmetic(ir_code_node* ir, func_entry* local_ST)
{
    /* The entry will be of the following form
    +------------+----------+----------+-----------+
    |    label   |    temp  |    op1   |    op2    |
    +------------+----------+----------+-----------+
    label can be one of ADD, SUB, MUL, DIv
    Input: 
        - IR 3AC for operation and operands
        - Local symbol table for info regarding the local variables
    Output:
        - Assembly code for arithmetic statements
    */

    // Get offset of result

    char* result = ir->result->name; 
    type_exp* resTypeExp = find_expr_codegen(result,local_ST);
    int offsetResult;
    char* resultType;
    int indexResult = 0;
    if(resTypeExp){
        offsetResult =  resTypeExp->offset * 16;
        resultType = resTypeExp->datatype;
    }
    else{
        indexResult = -1;
    }

    // char* result = ir->result->name;
    // int indexResult = sym_tab_entry_contains(result,local_ST->func_curr->entries);        // Checks if the symbol table contains - if yes we get the index
    // sym_tab_entry* temp = NULL;
    // temp = local_ST->func_curr->entries[indexResult];
    
    // // Hashing collision
    // while(temp!=NULL)
    // {
    //     if(!strcmp(temp->name,result))
    //     {
    //         break;
    //     }
    //     temp = temp->next;
    // }
    // int offsetResult = temp->offset*16;               // Get the memory offset for the lhs variable from the symbol
    // char* resultType = temp->type.datatype;
    
    char* nameLeft = ir->left_op->name; 
    type_exp* lhsTypeExp = find_expr_codegen(nameLeft,local_ST);
    int offsetLeft;
    char* leftType;
    int indexLeft = 0;
    if(lhsTypeExp){
        offsetLeft =  lhsTypeExp->offset * 16;
        leftType = lhsTypeExp->datatype;
    }
    else{
        indexLeft = -1;
    }

    char* nameRight = ir->right_op->name; 
    type_exp* rhsTypeExp = find_expr_codegen(nameRight,local_ST);
    int offsetRight;
    char* rightType;
    int indexRight = 0;
    if(rhsTypeExp){
        offsetRight =  rhsTypeExp->offset * 16;
        rightType = rhsTypeExp->datatype;
    }
    else{
        indexRight = -1;
    }


    // LEFT operand
    // char* nameLeft = ir->left_op->name;
    // int indexLeft = sym_tab_entry_contains(nameLeft,local_ST->func_curr->entries);        // Checks if the symbol table contains - if yes we get the index

    // Get offset of left operand temp
    fprintf(assembly, "\t\t; Code for arithmetic\n");
    fprintf(assembly, "\t\tpush_regs                    ; save values\n");
    

    // If left operand is an immediate operand
    if(indexLeft == -1)
    {
        // If strchr(nameRight, '.') returns null then it means right operand has no '.'
        // and hence it is an integer constant (immediate value)
        int type_left_int = (strchr(nameLeft, '.'))? 0 : 1;
        // INT
        if(type_left_int)
        {
            fprintf(assembly, "\t\tmov     rax , %s            ; immediate to memory\n", nameLeft);
        }

        // REAL
        else
        {
            fprintf(assembly, "\t\tmovsd     xmm0 , %s            ; immediate to memory\n", nameLeft);
        }
    }

    else
    {
        // temp = local_ST->func_curr->entries[indexLeft];
        // Hashing collision
        // while(temp!=NULL)
        // {
        //     if(!strcmp(temp->name,nameLeft))
        //     {
        //         break;
        //     }
        //     temp = temp->next;
        // }
        // int offsetLeft = temp->offset*16;               // Get the memory offset for the lhs variable from the symbol

        // INT
        if(!strcmp(resultType, "integer"))
        {
            fprintf(assembly, "\t\tmov     rax , [RBP - %d]\n", offsetLeft);
            
        }

        // REAL
        else
        {
            fprintf(assembly, "\t\tmovsd     xmm0 , [RBP - %d]\n", offsetLeft);
            
        }
        
    }

    // RIGHT operand
    // char* nameRight;
    // int indexRight;

    // For right operand
    switch(ir->operator)
    {
        case ADD:
            // Get offset of right operand temp
            // nameRight = ir->right_op->name;
            // indexRight = sym_tab_entry_contains(nameRight,local_ST->func_curr->entries);        // Checks if the symbol table contains - if yes we get the index
            
            // If right operand is a constant
            if(indexRight == -1)
            {
                // If strchr(nameRight, '.') returns null then it means right operand has no '.'
                // and hence it is an integer constant (immediate value)
                int type_right_int = (strchr(nameRight, '.'))? 0 : 1;
                
                // INT
                if(type_right_int)
                {
                    fprintf(assembly, "\t\tadd     rax , %s\n", nameRight);
                    
                    fprintf(assembly, "\t\tmov     [RBP - %d] , rax\n", offsetResult);
                    
                }

                // REAL
                else
                {
                    fprintf(assembly, "\t\taddsd     xmm0 , %s\n", nameRight);
                    
                    fprintf(assembly, "\t\tmovsd     [RBP - %d] , xmm0\n", offsetResult);
                    
                }
            }

            else
            {
                // temp = local_ST->func_curr->entries[indexRight];
                
                // // Hashing collision
                // while(temp!=NULL)
                // {
                //     if(!strcmp(temp->name,nameRight))
                //     {
                //         break;
                //     }
                //     temp = temp->next;
                // }
                // int offsetRight = temp->offset*16;               // Get the memory offset for the lhs variable from the symbol

                // INT
                if(!strcmp(resultType, "integer"))
                {
                    fprintf(assembly, "\t\tadd     rax , [RBP - %d]\n", offsetRight);
                    
                    fprintf(assembly, "\t\tmov     [RBP - %d] , rax\n", offsetResult);
                    
                }
                
                // REAL
                else
                {
                    fprintf(assembly, "\t\taddsd     xmm0 , [RBP - %d]\n", offsetRight);
                    
                    fprintf(assembly, "\t\tmovsd     [RBP - %d] , xmm0\n", offsetResult);
                    
                }
            }
            break;

        case SUB:
            // Get offset of right operand temp
            // nameRight = ir->right_op->name;
            // indexRight = sym_tab_entry_contains(nameRight,local_ST->func_curr->entries);        // Checks if the symbol table contains - if yes we get the index
            
            // If right operand is an immediate value
            if(indexRight == -1)
            {
                // If strchr(nameRight, '.') returns null then it means right operand has no '.'
                // and hence it is an integer constant (immediate value)
                int type_right_int = (strchr(nameRight, '.'))? 0 : 1;
                
                // INT
                if(type_right_int)
                {
                    fprintf(assembly, "\t\tsub     rax , %s\n", nameRight);
                    
                    fprintf(assembly, "\t\tmov     [RBP - %d] , rax\n", offsetResult);
                    
                }

                // REAL
                else
                {
                    fprintf(assembly, "\t\tsubsd     xmm0 , %s\n", nameRight);
                    
                    fprintf(assembly, "\t\tmovsd     [RBP - %d] , xmm0\n", offsetResult);
                    
                }
            }

            else
            {
                // temp = local_ST->func_curr->entries[indexRight];
                // while(temp!=NULL)
                // {
                //     if(!strcmp(temp->name,nameRight))
                //     {
                //         break;
                //     }
                //     temp = temp->next;
                // }
                // int offsetRight = temp->offset*16;               // Get the memory offset for the lhs variable from the symbol

                // INT
                if(!strcmp(resultType, "integer"))
                {
                    fprintf(assembly, "\t\tsub     rax , [RBP - %d]\n", offsetRight);
                    
                    fprintf(assembly, "\t\tmov     [RBP - %d] , rax\n", offsetResult);
                    
                }

                // REAL
                else
                {
                    fprintf(assembly, "\t\tsubsd     xmm0 , [RBP - %d]\n", offsetRight);
                    
                    fprintf(assembly, "\t\tmovsd     [RBP - %d] , xmm0\n", offsetResult);
                    
                }
            }
            break;

        case MUL:
            // Get offset of right operand temp
            // nameRight = ir->right_op->name;
            // indexRight = sym_tab_entry_contains(nameRight,local_ST->func_curr->entries);        // Checks if the symbol table contains - if yes we get the index
            
            // If right operand is an immediate value
            if(indexRight == -1)
            {
                // If strchr(nameRight, '.') returns null then it means right operand has no '.'
                // and hence it is an integer constant (immediate value)
                int type_right_int = (strchr(nameRight, '.') != NULL)? 0 : 1;

                // INT
                if(type_right_int)
                {
                    
                    fprintf(assembly, "\t\tmov     rbx , %s\n", nameRight);
                    fprintf(assembly, "\t\timul     rbx\n");
                    
                    fprintf(assembly, "\t\tmov    [RBP - %d] , rax\n", offsetResult);
                    
                }

                // REAL
                else
                {
                    fprintf(assembly, "\t\tmovsd      xmm1 , %s\n", nameRight);
                    fprintf(assembly, "\t\tmulsd     xmm1\n");
                    
                    fprintf(assembly, "\t\tmovsd     [RBP - %d] , xmm0\n", offsetResult);
                    
                }
            }

            else
            {
                // temp = local_ST->func_curr->entries[indexRight];
                // while(temp!=NULL)
                // {
                //     if(!strcmp(temp->name,nameRight))
                //     {
                //         break;
                //     }
                //     temp = temp->next;
                // }
                // int offsetRight = temp->offset*16;               // Get the memory offset for the lhs variable from the symbol

                if(!strcmp(resultType, "integer"))
                {
                    fprintf(assembly, "\t\tmov     rbx , [RBP - %d]\n", offsetRight);

                    fprintf(assembly, "\t\timul     rbx\n");
                    
                    fprintf(assembly, "\t\tmov     [RBP - %d] , rax\n", offsetResult);
                    
                }
                else
                {
                    fprintf(assembly, "\t\tmovsd     xmm1 , [RBP - %d]\n", offsetRight);
                  
                    fprintf(assembly, "\t\tmulsd     xmm1\n");
                    
                    fprintf(assembly, "\t\tmovsd     [RBP - %d] , xmm0\n", offsetResult);
                    
                }
            }
            break;

        case DIV:
            // Get offset of right operand temp
            // nameRight = ir->right_op->name;
            // indexRight = sym_tab_entry_contains(nameRight,local_ST->func_curr->entries);        // Checks if the symbol table contains - if yes we get the index
            
            // If right operand is an immediate value
            if(indexRight == -1)
            {
                fprintf(assembly, "\t\tdivsd     xmm0 , %s\n", nameRight);
                
                fprintf(assembly, "\t\tmovsd     [RBP - %d] , xmm0\n", offsetResult);
                
            }

            else
            {
                // temp = local_ST->func_curr->entries[indexRight];
                // while(temp!=NULL)
                // {
                //     if(!strcmp(temp->name,nameRight))
                //     {
                //         break;
                //     }
                //     temp = temp->next;
                // }
                // int offsetRight = temp->offset*16;               // Get the memory offset for the lhs variable from the symbol

                fprintf(assembly, "\t\tdivsd     xmm0 , [RBP - %d]\n", offsetRight);
                
                fprintf(assembly, "\t\tmovsd     [RBP - %d] , xmm0\n", offsetResult);
                
            }
            break;
    }

    fprintf(assembly, "\t\tpop_regs        ; restore register values\n\n\n");

}

void codegen_relational(ir_code_node* ir, func_entry* local_ST)
{
    /* The entry will be of the following form
    +------------+----------+----------+-----------+
    |    label   |    temp  |    op1   |    op2    |
    +------------+----------+----------+-----------+
    label can be one of LT, LTE, GT, GTE, EQ, NEQ
    Input: 
        - IR 3AC for operation and operands
        - Local symbol table for info regarding the local variables
    Output:
        - Assembly code for relational statements
    */

    // Get offset of result
    char* result = ir->result->name; 
    type_exp* resTypeExp = find_expr_codegen(result,local_ST);
    int offsetResult;
    char* resultType;
    int indexResult = 0;
    if(resTypeExp){
        offsetResult =  resTypeExp->offset * 16;
        resultType = resTypeExp->datatype;
    }
    else{
        indexResult = -1;
    }

    char* nameLeft = ir->left_op->name; 
    type_exp* lhsTypeExp = find_expr_codegen(nameLeft,local_ST);
    int offsetLeft;
    char* leftType;
    int indexLeft = 0;
    if(lhsTypeExp){
        offsetLeft =  lhsTypeExp->offset * 16;
        leftType = lhsTypeExp->datatype;
    }
    else{
        indexLeft = -1;
    }

    char* nameRight = ir->right_op->name; 
    type_exp* rhsTypeExp = find_expr_codegen(nameRight,local_ST);
    int offsetRight;
    char* rightType;
    int indexRight = 0;
    if(rhsTypeExp){
        offsetRight =  rhsTypeExp->offset * 16;
        rightType = rhsTypeExp->datatype;
    }
    else{
        indexRight = -1;
    }
    
    // char* result = ir->result->name;
    // int indexResult = sym_tab_entry_contains(result,local_ST->func_curr->entries);        // Checks if the symbol table contains - if yes we get the index
    // sym_tab_entry* temp = NULL;
    // temp = local_ST->func_curr->entries[indexResult];
    
    // // Hashing collision
    // while(temp!=NULL)
    // {
    //     if(!strcmp(temp->name,result))
    //     {
    //         break;
    //     }
    //     temp = temp->next;
    // }
    // int offsetResult = temp->offset*16;               // Get the memory offset for the lhs variable from the symbol
    // char* resultType = temp->type.datatype;

    // // Get offset of left operand temp
    // char* nameLeft = ir->left_op->name;
    // int indexLeft = sym_tab_entry_contains(nameLeft,local_ST->func_curr->entries);        // Checks if the symbol table contains - if yes we get the index
    fprintf(assembly, "\t\t; Code for relational\n");
    fprintf(assembly, "\t\tpush_regs                    ; save values\n");
    

    // If left operand is a constant
    if(indexLeft == -1)
    {
        // If strchr(nameRight, '.') returns null then it means right operand has no '.'
        // and hence it is an integer constant (immediate value)
        int type_left_int = (strchr(nameLeft, '.'))? 0 : 1;

        // INT
        if(type_left_int)
        {
            fprintf(assembly, "\t\tmov     rax , %s            ; immediate to memory\n", nameLeft);
        }

        // REAL
        else
        {
            fprintf(assembly, "\t\tmovsd     xmm0 , %s            ; immediate to memory\n", nameLeft);
        }
    }

    else
    {
        // temp = local_ST->func_curr->entries[indexLeft];
        
        // Hashing collision
        // while(temp!=NULL)
        // {
        //     if(!strcmp(temp->name,nameLeft))
        //     {
        //         break;
        //     }
        //     temp = temp->next;
        // }
        // int offsetLeft = temp->offset*16;               // Get the memory offset for the lhs variable from the symbol
        // char* leftType = temp->type.datatype;

        // INT
        if(!strcmp(leftType, "integer"))
        {
            fprintf(assembly, "\t\tmov     rax , [RBP - %d]\n", offsetLeft);
            
        }
        
        // REAL
        else
        {
            fprintf(assembly, "\t\tmovsd     xmm0 , [RBP - %d]\n", offsetLeft);
            
        }
        
    }
    // char* nameRight;
    // int indexRight;
    char* true_label = newLabel();
    char* next_label = newLabel();

    // For right operand
    switch(ir->operator)
    {
        case LT:
            // Get offset of right operand temp
            // nameRight = ir->right_op->name;
            // indexRight = sym_tab_entry_contains(nameRight,local_ST->func_curr->entries);        // Checks if the symbol table contains - if yes we get the index
            
            // If right operand is an immediate value
            if(indexRight == -1)
            {
                // If strchr(nameRight, '.') returns null then it means right operand has no '.'
                // and hence it is an integer constant (immediate value)
                int type_right_int = (strchr(nameRight, '.'))? 0 : 1;
                
                // INT
                if(type_right_int)
                {
                    fprintf(assembly, "\t\tmov     rbx , %s\n", nameRight);
                    fprintf(assembly, "\t\tcmp     rax , rbx\n");
                    fprintf(assembly, "\t\tjl     %s\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [RBP - %d], %d\n", offsetResult, 0);

                    fprintf(assembly, "\t\tjmp  %s\n", next_label);
                    fprintf(assembly, "%s:\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [RBP - %d], %d\n", offsetResult, 1);

                    fprintf(assembly, "%s:\n", next_label);
                    
                }

                // REAL
                else
                {
                    fprintf(assembly, "\t\tmov     xmm1 , %s\n", nameRight);
                    fprintf(assembly, "\t\tcomiss     xmm0 , xmm1\n");
                    fprintf(assembly, "\t\tjb     %s\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [RBP - %d], %d\n", offsetResult, 0);
                    
                    fprintf(assembly, "\t\tjmp  %s\n", next_label);
                    fprintf(assembly, "%s:\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [RBP - %d], %d\n", offsetResult, 1);

                    fprintf(assembly, "%s:\n", next_label);
                    
                }
            }

            else
            {
                // temp = local_ST->func_curr->entries[indexRight];

                // // Hashing collision
                // while(temp!=NULL)
                // {
                //     if(!strcmp(temp->name,nameRight))
                //     {
                //         break;
                //     }
                //     temp = temp->next;
                // }
                // int offsetRight = temp->offset*16;               // Get the memory offset for the lhs variable from the symbol
                // char* rightType = temp->type.datatype;

                // INT
                if(!strcmp(rightType, "integer"))
                {
                    fprintf(assembly, "\t\tmov     rbx , [RBP - %d]\n", offsetRight);
                    
                    fprintf(assembly, "\t\tcmp     rax , rbx\n");
                    fprintf(assembly, "\t\tjl     %s\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [RBP - %d], %d\n", offsetResult, 0);
                    
                    fprintf(assembly, "\t\tjmp  %s\n", next_label);
                    fprintf(assembly, "%s:\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [RBP - %d], %d\n", offsetResult, 1);

                    fprintf(assembly, "%s:\n", next_label);
                    
                }

                // REAL
                else
                {
                    fprintf(assembly, "\t\tmov     xmm1 , [RBP - %d]\n", offsetRight);
                    
                    fprintf(assembly, "\t\tcomiss     xmm0 , xmm1\n");
                    fprintf(assembly, "\t\tjb     %s\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [RBP - %d], %d\n", offsetResult, 0);
                    
                    fprintf(assembly, "\t\tjmp  %s\n", next_label);
                    fprintf(assembly, "%s:\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [RBP - %d], %d\n", offsetResult, 1);

                    fprintf(assembly, "%s:\n", next_label);
                    
                }
            }
            break;

        case GT:
            // Get offset of right operand temp
            // nameRight = ir->right_op->name;
            // indexRight = sym_tab_entry_contains(nameRight,local_ST->func_curr->entries);        // Checks if the symbol table contains - if yes we get the index
            
            // If right operand is an immediate value
            if(indexRight == -1)
            {
                // If strchr(nameRight, '.') returns null then it means right operand has no '.'
                // and hence it is an integer constant (immediate value)
                int type_right_int = (strchr(nameRight, '.'))? 0 : 1;
                
                // INT
                if(type_right_int)
                {
                    fprintf(assembly, "\t\tmov     rbx , %s\n", nameRight);
                    fprintf(assembly, "\t\tcmp     rax , rbx\n");
                    fprintf(assembly, "\t\tjg     %s\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [RBP - %d], %d\n", offsetResult, 0);
                    
                    fprintf(assembly, "\t\tjmp  %s\n", next_label);
                    fprintf(assembly, "%s:\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [RBP - %d], %d\n", offsetResult, 1);

                    fprintf(assembly, "%s:\n", next_label);
                }

                // REAL
                else
                {
                    fprintf(assembly, "\t\tmov     xmm1 , %s\n", nameRight);
                    fprintf(assembly, "\t\tcomiss     xmm0 , xmm1\n");
                    fprintf(assembly, "\t\tja     %s\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [RBP - %d], %d\n", offsetResult, 0);
                    
                    fprintf(assembly, "\t\tjmp  %s\n", next_label);
                    fprintf(assembly, "%s:\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [RBP - %d], %d\n", offsetResult, 1);

                    fprintf(assembly, "%s:\n", next_label);
                }
            }

            else
            {
                // temp = local_ST->func_curr->entries[indexRight];
                
                // // Hashing collision
                // while(temp!=NULL)
                // {
                //     if(!strcmp(temp->name,nameRight))
                //     {
                //         break;
                //     }
                //     temp = temp->next;
                // }
                // int offsetRight = temp->offset*16;               // Get the memory offset for the lhs variable from the symbol
                // char* rightType = temp->type.datatype;

                // INT
                if(!strcmp(rightType, "integer"))
                {
                    fprintf(assembly, "\t\tmov     rbx , [RBP - %d]\n", offsetRight);
                    
                    fprintf(assembly, "\t\tcmp     rax , rbx\n");
                    fprintf(assembly, "\t\tjg     %s\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [RBP - %d], %d\n", offsetResult, 0);
                    
                    fprintf(assembly, "\t\tjmp  %s\n", next_label);
                    fprintf(assembly, "%s:\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [RBP - %d], %d\n", offsetResult, 1);

                    fprintf(assembly, "%s:\n", next_label);
                    
                }
                
                // REAL
                else
                {
                    fprintf(assembly, "\t\tmov     xmm1 , [RBP - %d]\n", offsetRight);
                    
                    fprintf(assembly, "\t\tcomiss     xmm0 , xmm1\n");
                    fprintf(assembly, "\t\tjg     %s\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [RBP - %d], %d\n", offsetResult, 0);
                    
                    fprintf(assembly, "\t\tjmp  %s\n", next_label);
                    fprintf(assembly, "%s:\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [RBP - %d], %d\n", offsetResult, 1);

                    fprintf(assembly, "%s:\n", next_label);
                    
                }
            }
            break;

        case LE:
            // Get offset of right operand temp
            // nameRight = ir->right_op->name;
            // indexRight = sym_tab_entry_contains(nameRight,local_ST->func_curr->entries);        // Checks if the symbol table contains - if yes we get the index
            
            // If right operand is an immediate value
            if(indexRight == -1)
            {
                // If strchr(nameRight, '.') returns null then it means right operand has no '.'
                // and hence it is an integer constant (immediate value)
                int type_right_int = (strchr(nameRight, '.'))? 0 : 1;
                
                // INT
                if(type_right_int)
                {
                    fprintf(assembly, "\t\tmov     rbx , %s\n", nameRight);
                    fprintf(assembly, "\t\tcmp     rax , rbx\n");
                    fprintf(assembly, "\t\tjle     %s\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [RBP - %d], %d\n", offsetResult, 0);
                    
                    fprintf(assembly, "\t\tjmp  %s\n", next_label);
                    fprintf(assembly, "%s:\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [RBP - %d], %d\n", offsetResult, 1);

                    fprintf(assembly, "%s:\n", next_label);
                    
                }

                // REAL
                else
                {
                    fprintf(assembly, "\t\tmov     xmm1 , %s\n", nameRight);
                    fprintf(assembly, "\t\tcomiss     xmm0 , xmm1\n");
                    fprintf(assembly, "\t\tjb     %s\n", true_label);
                    fprintf(assembly, "\t\tjz     %s\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [RBP - %d], %d\n", offsetResult, 0);
                    
                    fprintf(assembly, "\t\tjmp  %s\n", next_label);
                    fprintf(assembly, "%s:\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [RBP - %d], %d\n", offsetResult, 1);

                    fprintf(assembly, "%s:\n", next_label);
                    
                }
            }

            else
            {
                // temp = local_ST->func_curr->entries[indexRight];
                
                // // Hashing collision
                // while(temp!=NULL)
                // {
                //     if(!strcmp(temp->name,nameRight))
                //     {
                //         break;
                //     }
                //     temp = temp->next;
                // }
                // int offsetRight = temp->offset*16;               // Get the memory offset for the lhs variable from the symbol
                // char* rightType = temp->type.datatype;

                // INT
                if(!strcmp(rightType, "integer"))
                {
                    fprintf(assembly, "\t\tmov     rbx , [RBP - %d]\n", offsetRight);
                    
                    fprintf(assembly, "\t\tcmp     rax , rbx\n");
                    fprintf(assembly, "\t\tjle     %s\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [RBP - %d], %d\n", offsetResult, 0);
                    
                    fprintf(assembly, "\t\tjmp  %s\n", next_label);
                    fprintf(assembly, "%s:\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [RBP - %d], %d\n", offsetResult, 1);

                    fprintf(assembly, "%s:\n", next_label);
                    
                }
                
                // REAL
                else
                {
                    fprintf(assembly, "\t\tmov     xmm1 , [RBP - %d]\n", offsetRight);
                    
                    fprintf(assembly, "\t\tcomiss     xmm0 , xmm1\n");
                    fprintf(assembly, "\t\tjb     %s\n", true_label);
                    fprintf(assembly, "\t\tjz     %s\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [RBP - %d], %d\n", offsetResult, 0);
                    
                    fprintf(assembly, "\t\tjmp  %s\n", next_label);
                    fprintf(assembly, "%s:\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [RBP - %d], %d\n", offsetResult, 1);

                    fprintf(assembly, "%s:\n", next_label);
                    
                }
            }
            break;

        case GE:
            // Get offset of right operand temp
            // nameRight = ir->right_op->name;
            // indexRight = sym_tab_entry_contains(nameRight,local_ST->func_curr->entries);        // Checks if the symbol table contains - if yes we get the index
            
            // If right operand is an immediate value
            if(indexRight == -1)
            {
                // If strchr(nameRight, '.') returns null then it means right operand has no '.'
                // and hence it is an integer constant (immediate value)
                int type_right_int = (strchr(nameRight, '.'))? 0 : 1;
                
                // INT
                if(type_right_int)
                {
                    fprintf(assembly, "\t\tmov     rbx , %s\n", nameRight);
                    fprintf(assembly, "\t\tcmp     rax , rbx\n");
                    fprintf(assembly, "\t\tjge     %s\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [RBP - %d], %d\n", offsetResult, 0);
                    
                    fprintf(assembly, "\t\tjmp  %s\n", next_label);
                    fprintf(assembly, "%s:\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [RBP - %d], %d\n", offsetResult, 1);

                    fprintf(assembly, "%s:\n", next_label);
                    
                }

                // REAL
                else
                {
                    fprintf(assembly, "\t\tmov     xmm1 , %s\n", nameRight);
                    fprintf(assembly, "\t\tcomiss     xmm0 , xmm1\n");
                    fprintf(assembly, "\t\tja     %s\n", true_label);
                    fprintf(assembly, "\t\tjz     %s\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [RBP - %d], %d\n", offsetResult, 0);
                    
                    fprintf(assembly, "\t\tjmp  %s\n", next_label);
                    fprintf(assembly, "%s:\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [RBP - %d], %d\n", offsetResult, 1);

                    fprintf(assembly, "%s:\n", next_label);
                    
                }
            }

            else
            {
                // temp = local_ST->func_curr->entries[indexRight];
                
                // // Hashing collision
                // while(temp!=NULL)
                // {
                //     if(!strcmp(temp->name,nameRight))
                //     {
                //         break;
                //     }
                //     temp = temp->next;
                // }
                // int offsetRight = temp->offset*16;               // Get the memory offset for the lhs variable from the symbol
                // char* rightType = temp->type.datatype;

                // INT
                if(!strcmp(rightType, "integer"))
                {
                    fprintf(assembly, "\t\tmov     rbx , [RBP - %d]\n", offsetRight);
                    
                    fprintf(assembly, "\t\tcmp     rax , rbx\n");
                    fprintf(assembly, "\t\tjge     %s\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [RBP - %d], %d\n", offsetResult, 0);
                    
                    fprintf(assembly, "\t\tjmp  %s\n", next_label);
                    fprintf(assembly, "%s:\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [RBP - %d], %d\n", offsetResult, 1);

                    fprintf(assembly, "%s:\n", next_label);
                    
                }
                
                // REAL
                else
                {
                    fprintf(assembly, "\t\tmov     xmm1 , [RBP - %d]\n", offsetRight);
                    
                    fprintf(assembly, "\t\tcomiss     xmm0 , xmm1\n");
                    fprintf(assembly, "\t\tja     %s\n", true_label);
                    fprintf(assembly, "\t\tjz     %s\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [RBP - %d], %d\n", offsetResult, 0);
                    
                    fprintf(assembly, "\t\tjmp  %s\n", next_label);
                    fprintf(assembly, "%s:\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [RBP - %d], %d\n", offsetResult, 1);

                    fprintf(assembly, "%s:\n", next_label);
                    
                }
            }
            break;

        case EQ:
            // Get offset of right operand temp
            // nameRight = ir->right_op->name;
            // indexRight = sym_tab_entry_contains(nameRight,local_ST->func_curr->entries);        // Checks if the symbol table contains - if yes we get the index
            
            // If right operand is an immediate value
            if(indexRight == -1)
            {
                // If strchr(nameRight, '.') returns null then it means right operand has no '.'
                // and hence it is an integer constant (immediate value)
                int type_right_int = (strchr(nameRight, '.'))? 0 : 1;
                
                // INT
                if(type_right_int)
                {
                    fprintf(assembly, "\t\tmov     rbx , %s\n", nameRight);
                    fprintf(assembly, "\t\tcmp     rax , rbx\n");
                    fprintf(assembly, "\t\tjz     %s\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [RBP - %d], %d\n", offsetResult, 0);
                    
                    fprintf(assembly, "\t\tjmp  %s\n", next_label);
                    fprintf(assembly, "%s:\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [RBP - %d], %d\n", offsetResult, 1);

                    fprintf(assembly, "%s:\n", next_label);
                    
                }

                // REAL
                else
                {
                    fprintf(assembly, "\t\tmov     xmm1 , %s\n", nameRight);
                    fprintf(assembly, "\t\tcomiss     xmm0 , xmm1\n");
                    fprintf(assembly, "\t\tjz     %s\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [RBP - %d], %d\n", offsetResult, 0);
                    
                    fprintf(assembly, "\t\tjmp  %s\n", next_label);
                    fprintf(assembly, "%s:\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [RBP - %d], %d\n", offsetResult, 1);

                    fprintf(assembly, "%s:\n", next_label);
                    
                }
            }

            else
            {
                // temp = local_ST->func_curr->entries[indexRight];
                
                // // Hashing collision
                // while(temp!=NULL)
                // {
                //     if(!strcmp(temp->name,nameRight))
                //     {
                //         break;
                //     }
                //     temp = temp->next;
                // }
                // int offsetRight = temp->offset*16;               // Get the memory offset for the lhs variable from the symbol
                // char* rightType = temp->type.datatype;

                // INT
                if(!strcmp(rightType, "integer"))
                {
                    fprintf(assembly, "\t\tmov     rbx , [RBP - %d]\n", offsetRight);
                    
                    fprintf(assembly, "\t\tcmp     rax , rbx\n");
                    fprintf(assembly, "\t\tjz     %s\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [RBP - %d], %d\n", offsetResult, 0);
                    
                    fprintf(assembly, "\t\tjmp  %s\n", next_label);
                    fprintf(assembly, "%s:\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [RBP - %d], %d\n", offsetResult, 1);

                    fprintf(assembly, "%s:\n", next_label);
                    
                }
                
                // REAL
                else
                {
                    fprintf(assembly, "\t\tmov     xmm1 , [RBP - %d]\n", offsetRight);
                    
                    fprintf(assembly, "\t\tcomiss     xmm0 , xmm1\n");
                    fprintf(assembly, "\t\tjz     %s\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [RBP - %d], %d\n", offsetResult, 0);
                    
                    fprintf(assembly, "\t\tjmp  %s\n", next_label);
                    fprintf(assembly, "%s:\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [RBP - %d], %d\n", offsetResult, 1);

                    fprintf(assembly, "%s:\n", next_label);
                    
                }
            }
            break;

        case NEQ:
            // Get offset of right operand temp
            // nameRight = ir->right_op->name;
            // indexRight = sym_tab_entry_contains(nameRight,local_ST->func_curr->entries);        // Checks if the symbol table contains - if yes we get the index
            
            // If right operand is an immediate value
            if(indexRight == -1)
            {
                // If strchr(nameRight, '.') returns null then it means right operand has no '.'
                // and hence it is an integer constant (immediate value)
                int type_right_int = (strchr(nameRight, '.'))? 0 : 1;
                
                // INT
                if(type_right_int)
                {
                    fprintf(assembly, "\t\tmov     rbx , %s\n", nameRight);
                    fprintf(assembly, "\t\tcmp     rax , rbx\n");
                    fprintf(assembly, "\t\tjnz     %s\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [RBP - %d], %d\n", offsetResult, 0);
                    
                    fprintf(assembly, "\t\tjmp  %s\n", next_label);
                    fprintf(assembly, "%s:\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [RBP - %d], %d\n", offsetResult, 1);

                    fprintf(assembly, "%s:\n", next_label);
                    
                }


                // REAL
                else
                {
                    fprintf(assembly, "\t\tmov     xmm1 , %s\n", nameRight);
                    fprintf(assembly, "\t\tcomiss     xmm0 , xmm1\n");
                    fprintf(assembly, "\t\tjnz     %s\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [RBP - %d], %d\n", offsetResult, 0);
                    
                    fprintf(assembly, "\t\tjmp  %s\n", next_label);
                    fprintf(assembly, "%s:\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [RBP - %d], %d\n", offsetResult, 1);

                    fprintf(assembly, "%s:\n", next_label);
                    
                }
            }

            else
            {
                // temp = local_ST->func_curr->entries[indexRight];
                
                // // Hashing collision
                // while(temp!=NULL)
                // {
                //     if(!strcmp(temp->name,nameRight))
                //     {
                //         break;
                //     }
                //     temp = temp->next;
                // }
                // int offsetRight = temp->offset*16;               // Get the memory offset for the lhs variable from the symbol
                // char* rightType = temp->type.datatype;

                // INT
                if(!strcmp(rightType, "integer"))
                {
                    fprintf(assembly, "\t\tmov     rbx , [RBP - %d]\n", offsetRight);
                    
                    fprintf(assembly, "\t\tcmp     rax , rbx\n");
                    fprintf(assembly, "\t\tjnz     %s\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [RBP - %d], %d\n", offsetResult, 0);
                    
                    fprintf(assembly, "\t\tjmp  %s\n", next_label);
                    fprintf(assembly, "%s:\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [RBP - %d], %d\n", offsetResult, 1);

                    fprintf(assembly, "%s:\n", next_label);
                    
                }

                // REAL
                else
                {
                    fprintf(assembly, "\t\tmov     xmm1 , [RBP - %d]\n", offsetRight);
                    
                    fprintf(assembly, "\t\tcomiss     xmm0 , xmm1\n");
                    fprintf(assembly, "\t\tjnz     %s\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [RBP - %d], %d\n", offsetResult, 0);
                    
                    fprintf(assembly, "\t\tjmp  %s\n", next_label);
                    fprintf(assembly, "%s:\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [RBP - %d], %d\n", offsetResult, 1);

                    fprintf(assembly, "%s:\n", next_label);
                    
                }
            }
            break;
        }

    fprintf(assembly, "\t\tpop_regs        ; restore register values\n\n\n");
  
}

void codegen_func(ir_code_node* ir, func_entry* local_ST)
{
    /* The entry will be of the following form
    +------------+----------+----------+-----------+
    |    label   |  fn_name |    NULL  |    NULL   |
    +------------+----------+----------+-----------+
    Input: 
        - IR 3AC for operation and operands
        - Local symbol table for info regarding the local variables
    Output:
        - Assembly code for function definitions
    */

    char* funcName = ir->result->name;

    fprintf(assembly, "%s:\n",funcName);
    fprintf(assembly, "\t\tENTER    %d,0\n",local_ST->func_root->offset*16);
    // fprintf(assembly, "\t\tpush_regs                    ; save values\n");
    // fprintf(assembly, "\t\tmov      rbp , rsp           ; set base to current stack top\n");
    // fprintf(assembly, "\t\tpop_regs                     ; save values\n\n\n");

}

void codegen_conditional(ir_code_node* ir, func_entry* local_ST)
{
    char* condCheckVar = ir->result->name; 
    char* condCheckVarReach = ir->result->reach;
    type_exp* dataOfCondVar = find_expr_codegen(condCheckVar,local_ST);
    int offsetResult =  dataOfCondVar->offset * 16;

    fprintf(assembly, "\t\tmov      rax , [RBP - %d]\n", offsetResult);
    // fprintf(assembly, "\t\tand      rax , 0x0000_0000_0000_0001\n", offsetResult);
    fprintf(assembly, "\t\tcmp      rax , 1\n");
    fprintf(assembly, "\t\tje       %s\n",ir->left_op->name);

}

void codegen_label(ir_code_node* ir, func_entry* local_ST)
{
    char* condCheckVar = ir->result->name;
    fprintf(assembly, "%s:\n", condCheckVar);
}

void codegen_goto(ir_code_node* ir, func_entry* local_ST)
{
    char* condCheck = ir->result->name;
    fprintf(assembly, "\t\tjmp      %s\n", condCheck);
}


void codegen_mem_read_ST(ir_code_node* ir, func_entry* local_ST)
{

    
    /* The entry will be of the following form
    +------------+----------+----------+-----------+
    |   ASSIGN   |    lhs   |    rhs   |    NULL   |
    +------------+----------+----------+-----------+
    Input: 
        - IR 3AC for operation and operands
        - Local symbol table for info regarding the local variables
    Output:
        - Assembly code for assignment statements
    */
    char* lhsArrayRead = ir->result->name; 
    char* lhsArrayReadReach = ir->result->reach;
    type_exp* lhsTypeExp = find_expr_codegen(lhsArrayRead,local_ST);
    int offsetResultLhs =  lhsTypeExp->offset * 16;

    char* rhsArrayIndex = ir->right_op->name; 
    char* rhsArrayIndexReach = ir->right_op->reach;
    type_exp* rhsTypeExp = find_expr_codegen(rhsArrayIndex,local_ST);
    int offsetResultRhs =  rhsTypeExp->offset * 16;

    fprintf(assembly, "\t\tpush_regs                    ; save values\n");
    fprintf(assembly, "\t\txor      rax , rax           ; flush out the rax register\n");

    if(!strcmp(lhsTypeExp->datatype, "integer")||!strcmp(lhsTypeExp->datatype, "boolean"))
    {
        fprintf(assembly, "\t\tmov      rax , [RBP - %d]                    ; memory to register\n",offsetResultRhs);
        
        fprintf(assembly, "\t\tmov      [RBP - %d] , rax            ; register to memory\n",offsetResultLhs);
        
    }

    // REAL
    else
    {
        fprintf(assembly, "\t\tmovsd      xmm0 , [RBP - %d]                    ; memory to register\n",offsetResultRhs);
        
        fprintf(assembly, "\t\tmovsd     [RBP - %d] , xmm0            ; register to memory\n",offsetResultLhs);   
    }
    
    fprintf(assembly, "\t\tpop_regs                    ; restore register values\n\n\n");
   
}

void codegen_mem_write_ST(ir_code_node* ir, func_entry* local_ST)
{
    char* lhsArrayRead = ir->left_op->name; 
    char* lhsArrayReadReach = ir->left_op->reach;
    type_exp* lhsTypeExp = find_expr_codegen(lhsArrayRead,local_ST);
    int offsetResultLhs =  lhsTypeExp->offset * 16;

    char* rhsArrayIndex = ir->right_op->name; 
    char* rhsArrayIndexReach = ir->right_op->reach;
    type_exp* rhsTypeExp = find_expr_codegen(rhsArrayIndex,local_ST);
    int offsetResultRhs =  rhsTypeExp->offset * 16;

    fprintf(assembly, "\t\tpush_regs                    ; save values\n");
    fprintf(assembly, "\t\txor      rax , rax           ; flush out the rax register\n");

    if(!strcmp(lhsTypeExp->datatype, "integer")||!strcmp(lhsTypeExp->datatype, "boolean"))
    {
        fprintf(assembly, "\t\tmov      rax , [RBP - %d]                    ; memory to register\n",offsetResultRhs);
        
        fprintf(assembly, "\t\tmov      [RBP - %d] , rax            ; register to memory\n",offsetResultLhs);
    }
    // REAL
    else
    {
        fprintf(assembly, "\t\tmovsd      xmm0 , [RBP - %d]                    ; memory to register\n",offsetResultRhs);
        
        fprintf(assembly, "\t\tmovsd     [RBP - %d] , xmm0            ; register to memory\n",offsetResultLhs);   
    }
    
    fprintf(assembly, "\t\tpop_regs                    ; restore register values\n\n\n");
}

void codegen_mem_read(ir_code_node* ir, func_entry* local_ST)
{
    /* Handling the symbol table ops*/

    // Writing the assembly for label

    /* Handling the symbol table ops*/
}

void codegen_mem_write(ir_code_node* ir, func_entry* local_ST)
{
    /* Handling the symbol table ops*/

    // Writing the assembly for label

    /* Handling the symbol table ops*/
}

void starter(FILE* assembly_file,ir_code* IR)
{
    /* This function is the overall wrapper function and calling other functions based on the operator in the
    ir code node received as parameter. The called functions are responsible to generate the corresponding
    assembly code by directly writing the code into the assembly file passed as file pointer
    */

    assembly = assembly_file;

    if(!assembly)
        printf("[-] Error opening assembly_try.asm!\n");
    
    else
    {
        printf("[+] assembly_try.asm opened!\n");

        // Write the external functions and start data section for assembly
        fprintf(assembly, "; Group-20\n");
        fprintf(assembly, "; ---------------------------------\n");
        fprintf(assembly, "; 1. Rajan Sahu       2019B4A70572P\n");
        fprintf(assembly, "; 2. Yash Goyal       2019B4A70638P\n");
        fprintf(assembly, "; 3. Ayush Agarwal    2019B4A70652P\n");
        fprintf(assembly, "; 4. Vasu Swaroop     2019B4A70656P\n");
        fprintf(assembly, "; 5. A Sudarshan      2019B4A70744P\n");
        fprintf(assembly, "; ---------------------------------\n");
        fprintf(assembly, "extern printf, scanf, exit\n");
        fprintf(assembly, "\t\t; Data declaration such as zero and format specifiers for print/scan\n");
        fprintf(assembly, "\t\tsection      .data\n");

        // Write down all the format specifiers reqd
        fprintf(assembly, "\t\tfmt_spec_int_in: db \"%%hd\", 0\n");
        fprintf(assembly, "\t\tfmt_spec_int_out: db \"%%hd\", 10, 0\n");
        fprintf(assembly, "\t\tfmt_spec_real_in: db \"%%4f\", 0\n");
        fprintf(assembly, "\t\tfmt_spec_real_out: db \"%%4f\", 10, 0\n");
        fprintf(assembly, "\t\tfmt_spec_string: db \"%%s\", 10, 0\n");
        fprintf(assembly, "\t\tfmt_spec_bool_in: db \"%%d\", 0\n");
        fprintf(assembly, "\t\tfmt_spec_bool_out: db \"%%d\", 10, 0\n");
        fprintf(assembly, "\t\tinteger_in: db \"Input: Enter an integer value\", 10, 0\n");
        fprintf(assembly, "\t\tinteger_in_len: equ $ - integer_in\n");
        fprintf(assembly, "\t\treal_in: db \"Input: Enter an real value\", 10, 0\n");
        fprintf(assembly, "\t\treal_in_len: equ $ - real_in\n");
        fprintf(assembly, "\t\tboolean_in: db \"Input: Enter an boolean value\", 10, 0\n");
        fprintf(assembly, "\t\tboolean_in_len: equ $ - boolean_in\n");
        fprintf(assembly, "\t\tprint_out: db \"Output: \", 0\n");
        fprintf(assembly, "\t\tprint_out_len: equ $ - print_out\n");
        fprintf(assembly, "\t\tprintError: db \"Out of Bound Exception in Array\", 10, 0\n");
        fprintf(assembly, "\t\tprintError_len: db \"Out of Bound Exception in Array\", 10, 0\n");
        fprintf(assembly, "\t\tarray_in: db \"Input: Enter %%d elements of %%s type for range %%d to %%d\", 10, 0\n");
        fprintf(assembly, "\t\tarray_in_len: equ $ - array_in\n");
        fprintf(assembly, "\t\ttrue: db \"true\", 10, 0\n");
        fprintf(assembly, "\t\ttrue_len: equ $ - true\n");
        fprintf(assembly, "\t\tfalse: db \"false\", 10, 0\n");
        fprintf(assembly, "\t\tfalse_len: equ $ - false\n");
        fprintf(assembly, "\t\tzero: equ 0\n");

        // data_read(assembly); //TODO Toggle if want data of variables

        // Start section .text of assembly
        fprintf(assembly, "\n\n\t\tsection      .text\n");
        // Define the macros
        macros_starter();
        fprintf(assembly, "\t\tglobal main\n");
    }

    ir_code_node *IR_head = IR->head;
    var_record* func_curr;
    func_entry* local_ST;

    // Go through each entry of the IR quadruple
    while(IR_head)
    {
        //if you reach FUNC --> then change the context
        if(IR_head->operator==FUNC)
        {
            
            //in case it is main --we have to search for DRIVER
            if (!strcmp(IR_head->result->name,"main")) local_ST = find_module_global("DRIVER");
            else local_ST = find_module_global(IR_head->result->name);
            func_curr = local_ST->func_root; //once we get the local_ST then just locate the root
        }

        type_exp* left_exp;
        type_exp* right_exp;
        type_exp* res_exp;
        
        // Check if symbol table contains left operand label
        // if(IR_head->left_op->name&& (sym_tab_entry_contains(IR_head->left_op->name,local_ST->func_curr->entries)!=-1)){
        //     left_exp = find_expr_codegen(IR_head->left_op->name,local_ST);
        //     char* temp = (char*)malloc(sizeof(char)*30);
        //     memset(temp,'\0',sizeof(temp));
        //     strcpy(temp, IR_head->left_op->name);
        //     strcat(temp, left_exp->reach_defined);
        //     IR_head->left_op->name = temp;
        // }

        // // Check if symbol table contains right operand label
        // if(IR_head->right_op->name&& (sym_tab_entry_contains(IR_head->right_op->name,local_ST->func_curr->entries)!=-1)){
        //     right_exp = find_expr_codegen(IR_head->right_op->name,local_ST);
        //     char* temp = (char*)malloc(sizeof(char)*30);
        //     memset(temp,'\0',sizeof(temp));
        //     strcpy(temp, IR_head->right_op->name);
        //     strcat(temp, right_exp->reach_defined);
        //     IR_head->right_op->name = temp;
        // }

        // // Check if symbol table contains result label
        // if(IR_head->result->name&&strcmp(IR_head->result->name,"main")&& (sym_tab_entry_contains(IR_head->result->name,local_ST->func_curr->entries)!=-1)){
        //     res_exp = find_expr_codegen(IR_head->result->name,local_ST);
        //     char* temp = (char*)malloc(sizeof(char)*30);
        //     memset(temp,'\0',sizeof(temp));
        //     strcpy(temp, IR_head->result->name);
        //     strcat(temp, res_exp->reach_defined);
        //     IR_head->result->name = temp;
        // }
        
        /* REACH is the path to take from function root symbol table to the corresponding construct's
        local symbol table (d = down, r = right). Moving down increases nesting level unlike moving right */
        char* reach = "";   
        func_curr = local_ST->func_root;
        local_ST->func_curr = func_curr;


        local_ST->func_curr = find_local_construct(local_ST->func_root->construct_name,IR_head->reach);

        // For calling respective code gen fns
        switch (IR_head->operator)
        {
        case ASSIGN:
            codegen_assgn_stmt(IR_head, local_ST);
            break;

        case GET_VALUE:
            codegen_input(IR_head, local_ST);
            break;
        
        case PRINT:
            codegen_output(IR_head, local_ST);
            break;

        case ADD:
        case SUB:
        case MUL:
        case DIV:
            codegen_arithmetic(IR_head, local_ST);
            break;
        
        case LT:
        case LE:
        case GT:
        case GE:
        case EQ:
        case NEQ:
            codegen_relational(IR_head, local_ST);
            break;
        
        case OR:
        case AND:
            codegen_logical(IR_head, local_ST);
            break;
        
        case FUNC:
            codegen_func(IR_head, local_ST);
        case CALL:
        case PARA_IN:
        case PARA_OUT:
        case RET:
            break;
        
        case GOTO:
            codegen_goto(IR_head, local_ST);
            break;
        
        case IF:
            codegen_conditional(IR_head, local_ST);
            break;
        
        case LABEL:
            codegen_label(IR_head, local_ST);
            break;
        
        case MEMREAD_ST:
            codegen_mem_read_ST(IR_head, local_ST);
            break;

        case MEMWRITE_ST:
            codegen_mem_write_ST(IR_head, local_ST);
            break;

        case MEMREAD:
            codegen_mem_read(IR_head, local_ST);
            break;

        case MEMWRITE:
            codegen_mem_write(IR_head, local_ST);
            break;
        }
        IR_head = IR_head->next;
    }
    if(exitJumpLabel!=NULL){
        // fprintf(assembly, "\t\tpush_regs                    ; save values\n");

        fprintf(assembly, "\t\t; Display prompt for integer input\n");
        fprintf(assembly, "\t\tmov      rax , 1\n");
        fprintf(assembly, "\t\tmov      rdi , 1\n");
        fprintf(assembly, "\t\tmov      rsi , printError\n");
        fprintf(assembly, "\t\tmov      rdx , printError_len\n");
        fprintf(assembly, "\t\tsyscall\n");

        // fprintf(assembly, "\t\tpop_regs                    ; save values\n");

    }

    fprintf(assembly, "main_end:\n");
    fprintf(assembly, "LEAVE\n");
    fprintf(assembly, "\t\tretq");
    fclose(assembly);
}

/*-----------------------------------------------------------HELPER FUNCTIONS----------------------------------------------------------------*/
type_exp* find_in_list_codegen(sym_tab_entry* output,char* key)
{
    while(output)
    {
        if(!strcmp(output->name,key))
        {
            return &output->type;
        }
        output = output->next;
    }  
    return NULL;
}

type_exp* find_in_func_table_codegen(char* key, func_entry* curr)
{
 
    type_exp* input=find_in_list_codegen(curr->input_list, key);
    if(input)
        return input;
    
    type_exp* output=find_in_list_codegen(curr->ouput_list, key);
    if(output)
        return output;
    
    return NULL;
}

type_exp* find_in_table_codegen(char* key,var_record* table,func_entry* func)
{
    int index = sym_tab_entry_contains(key,table->entries);
    if(index==-1) return NULL;

    sym_tab_entry* temp = table->entries[index];

    while(temp!=NULL)
    {
        if(!strcmp(temp->name,key))
        {
            temp->offset = func->func_root->offset;
            if(!strcmp(temp->type.datatype,"integer")){
                func->func_root->offset +=  INT_OFFSET;
            }
            else if(!strcmp(temp->type.datatype,"boolean")){
                func->func_root->offset +=  BOOL_OFFSET;
                
            }else if(!strcmp(temp->type.datatype,"real")){
                func->func_root->offset +=  REAL_OFFSET;
            }
            return &temp->type;
        }
        temp = temp->next;
    }
    return NULL;//think should return NULL
}

type_exp* find_expr_codegen(char* key, func_entry* curr)
{
    if(!curr)
    {
        return NULL;
    }
    var_record* current_rec = curr->func_curr;
    var_record* temp = current_rec;
    type_exp* type=find_in_table_codegen(key, current_rec,curr);
    
    if(type)
        return type;
    else
    {
        curr->func_curr=curr->func_curr->parent; //also need to add for checking variable in the op list and ip list of the function record
        if(curr->func_curr==NULL)
        {
            // just check in the function parameter list
            type = find_in_func_table_codegen(key,curr);
            curr->func_curr = temp;
            //return from here only
        }
        else
        {
            type = find_expr_codegen(key,curr);
            curr->func_curr = temp;
        }
        return type;
    }
}

void data_read(FILE* assembly)
{
    code_gen(assembly);
}
/*-------------------------------------------------------END OF  HELPER FUNCTIONS------------------------------------------------------------*/