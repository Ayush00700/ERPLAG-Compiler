#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "codegen.h"

FILE* assembly;

void macros_starter(){
    /* This function defines the required macros in the assembly file
    */

    fprintf(assembly, "\t\t;macro to store all registers\n");
    fprintf(assembly, "\t\t%%macro  push_regs    0\n");
    fprintf(assembly, "\t\tpush      rax\n");
    fprintf(assembly, "\t\tpush      rbx\n");
    fprintf(assembly, "\t\tpush      rcx\n");
    fprintf(assembly, "\t\tpush      rdx\n");
    // fprintf(assembly, "\t\tpush      rsp\n");
    // fprintf(assembly, "\t\tpush      rbp\n");
    fprintf(assembly, "\t\tpush      rsi\n");
    fprintf(assembly, "\t\tpush      rdi\n");
    fprintf(assembly, "\t\t%%endmacro\n\n\n");

    fprintf(assembly, "\t\t;macro to restore all registers\n");
    fprintf(assembly, "\t\t%%macro  pop_regs    0\n");
    fprintf(assembly, "\t\tpop      rdi\n");
    fprintf(assembly, "\t\tpop      rsi\n");
    // fprintf(assembly, "\t\tpop      rbp\n");
    // fprintf(assembly, "\t\tpop      rsp\n");
    fprintf(assembly, "\t\tpop      rdx\n");
    fprintf(assembly, "\t\tpop      rcx\n");
    fprintf(assembly, "\t\tpop      rbx\n");
    fprintf(assembly, "\t\tpop      rax\n");
    fprintf(assembly, "\t\t%%endmacro\n\n\n");

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

    fprintf(assembly, "\t\t;macro to re-align RSP\n");
    fprintf(assembly, "\t\t%%macro  rsp_realign    0\n");
    fprintf(assembly, "\t\tadd      rsp , rbx\n");
    fprintf(assembly, "\t\tpop      rbx\n");
    fprintf(assembly, "\t\t%%endmacro\n\n\n");


}

void codegen_assgn_stmt(ir_code_node* ir, func_entry* local_ST){
    /* The entry will be of the following form
    +------------+----------+----------+-----------+
    |   ASSIGN   |    lhs   |    rhs   |    NULL   |
    +------------+----------+----------+-----------+
    */

    
    char* nameRHS = ir->left_op->name;      // temporary variable name for rhs

    
    

    char* nameLHS = ir->result->name;      // temporary variable name for rhs

    // Finding the symbol table entry for lhs variable
    int indexLHS = sym_tab_entry_contains(nameLHS,local_ST->func_curr->entries);        // Checks if the symbol table contains - if yes we get the index
    sym_tab_entry* temp = NULL;
    if(indexLHS!=-1)
        temp = local_ST->func_curr->entries[indexLHS];
    while(temp!=NULL){  //Hashing collision
        if(!strcmp(temp->name,nameLHS)){
            break;
        }
        temp = temp->next;
    }
    int offsetLHS = 0;
    if(indexLHS!=-1)offsetLHS = temp->offset*16;               // Get the memory offset for the lhs variable from the symbol

    int indexRHS = sym_tab_entry_contains(nameRHS,local_ST->func_curr->entries);        // Check if RHS is a expression/ variable/ a constant
    fprintf(assembly, "\t\t; Code for getting assignment statement\n");
    
    fprintf(assembly, "\t\tpush_regs                    ; save values\n");
    
    fprintf(assembly, "\t\txor      rax , rax           ; flush out the rax register\n");

    // If RHS is a constant (immediate value in ASM jargon)
    if(indexRHS==-1){
        // CHECK ------> detect if immediate value is int or real
        int type_right_int = (strchr(nameLHS, '.'))? 0 : 1;
        if(type_right_int)
        {    
            fprintf(assembly, "\t\tmov      rax , %s                    ; immediate to register\n",nameRHS);
            
            fprintf(assembly, "\t\tmov      [%s] , rax            ; register to memory\n",ir->result->name);
            
        }

        else
        {
            fprintf(assembly, "\t\tmovsd      xmm0 , %s                    ; immediate to register\n",nameRHS);
            
            fprintf(assembly, "\t\tmovsd      [%s] , xmm0            ; register to memory\n",ir->result->name);
            
        }
    }

    // If RHS not constant
    else
    {
        temp = local_ST->func_curr->entries[indexRHS];
        while(temp!=NULL){
            if(!strcmp(temp->name,nameRHS)){
                break;
            }
            temp = temp->next;
        }

        int offsetRHS = temp->offset*16;               // Get the memory offset for the rhs variable from the symbol

        if(!strcmp(temp->type.datatype, "integer"))
        {
            fprintf(assembly, "\t\tmov      rax , [%s]                    ; memory to register\n",ir->left_op->name);
            
            fprintf(assembly, "\t\tmov      [%s] , rax            ; register to memory\n",ir->result->name);
            
        }

        else
        {
            fprintf(assembly, "\t\tmovsd      xmm0 , [%s]                    ; memory to register\n",ir->left_op->name);
            
            fprintf(assembly, "\t\tmovsd     [%s] , xmm0            ; register to memory\n",ir->result->name);
            
        }
    }
    
    fprintf(assembly, "\t\tpop_regs                    ; restore register values\n\n\n");
                                           
    
}

void codegen_logical(ir_code_node* ir, func_entry* local_ST){
    /* The entry will be of the following form
    +------------+----------+----------+-----------+
    |    label   |    temp  |    op1   |    op2    |
    +------------+----------+----------+-----------+
    label can be one of AND, OR
    */

    
    
    
    
    

    // Get offset of result
    char* result = ir->result->name;
    int indexResult = sym_tab_entry_contains(result,local_ST->func_curr->entries);        // Checks if the symbol table contains - if yes we get the index
    sym_tab_entry* temp = NULL;
    temp = local_ST->func_curr->entries[indexResult];
    while(temp!=NULL){
        if(!strcmp(temp->name,result)){
            break;
        }
        temp = temp->next;
    }
    int offsetResult = temp->offset*16;               // Get the memory offset for the lhs variable from the symbol
    char* resultType = temp->type.datatype;

    // Get offset of left operand temp
    char* nameLeft = ir->left_op->name;
    int indexLeft = sym_tab_entry_contains(nameLeft,local_ST->func_curr->entries);        // Checks if the symbol table contains - if yes we get the index
    fprintf(assembly, "\t\t; Code for logical op\n");
    
    fprintf(assembly, "\t\tpush_regs                    ; save values\n");
    

    // If left operand is a constant
    if(indexLeft == -1)
    {
        // int type_left_int = (strchr(nameLeft, '.'))? 0 : 1;
        // TODO ----> identifying if immediate value is int or real
        // if( type_left_int)
        // {
            fprintf(assembly, "\t\tmov     rax , %s            ; immediate to memory\n", nameLeft);
            // 
        // }

        // else
        // {
            // fprintf(assembly, "\t\tmovsd     xmm0 , %s            ; immediate to memory\n", nameLeft);
            // 
        // }
    }

    else
    {
        temp = local_ST->func_curr->entries[indexLeft];
        while(temp!=NULL){
            if(!strcmp(temp->name,nameLeft)){
                break;
            }
            temp = temp->next;
        }
        int offsetLeft = temp->offset*16;               // Get the memory offset for the lhs variable from the symbol

        // if(!strcmp(resultType, "integer"))
        // {
            fprintf(assembly, "\t\tmov     rax , [%s]\n", ir->left_op->name);
            
        // }
        // else
        // {
        //     fprintf(assembly, "\t\tmovsd     xmm0 , [%s]\n", ir->left_op->name);
        // }
        
    }

    // For right operand
    switch(ir->operator)
    {
        char* nameRight;
        int indexRight;
        case AND:
            // Get offset of right operand temp
            nameRight = ir->right_op->name;
            indexRight = sym_tab_entry_contains(nameRight,local_ST->func_curr->entries);        // Checks if the symbol table contains - if yes we get the index
            // If right operand is a constant
            if(indexRight == -1)
            {
                // int type_right_int = (strchr(nameRight, '.'))? 0 : 1;

                // CHECK ----> identifying if immediate value is int or real
                // if(type_right_int)
                // {
                    fprintf(assembly, "\t\tand     rax , %s\n", nameRight);
                    
                    fprintf(assembly, "\t\tmov     [%s] , rax\n", ir->result->name);
                    
                // }

                // else
                // {
                //     fprintf(assembly, "\t\tandpd     xmm0 , %s\n", nameRight);
                    
                //     fprintf(assembly, "\t\tmovsd     [%s] , xmm0\n", ir->result->name);
                    
                // }
            }

            else
            {
                temp = local_ST->func_curr->entries[indexRight];
                while(temp!=NULL){
                    if(!strcmp(temp->name,nameRight)){
                        break;
                    }
                    temp = temp->next;
                }
                int offsetRight = temp->offset*16;               // Get the memory offset for the lhs variable from the symbol

                // if(!strcmp(resultType, "integer"))
                // {
                    fprintf(assembly, "\t\tand     rax , [%s]\n", ir->right_op->name);
                    
                    fprintf(assembly, "\t\tmov     [%s] , rax\n", ir->result->name);
                    
                // }
                // else
                // {
                //     fprintf(assembly, "\t\tandpd     xmm0 , [%s]\n", ir->right_op->name);
                    
                //     fprintf(assembly, "\t\tmovsd     [%s] , xmm0\n", ir->result->name);
                    
                // }
            }
            break;
        case OR:
            // Get offset of right operand temp
            nameRight = ir->right_op->name;
            indexRight = sym_tab_entry_contains(nameRight,local_ST->func_curr->entries);        // Checks if the symbol table contains - if yes we get the index
            // If right operand is a constant
            if(indexRight == -1)
            {
                int type_right_int = (strchr(nameRight, '.'))? 0 : 1;
                // CHECK ----> identifying if immediate value is int or real
                // if(type_right_int)
                // {
                    fprintf(assembly, "\t\tor     rax , %s\n", nameRight);
                    
                    fprintf(assembly, "\t\tmov     [%s] , rax\n", ir->result->name);
                    
                // }

                // else
                // {
                //     fprintf(assembly, "\t\torpd     xmm0 , %s\n", nameRight);
                    
                //     fprintf(assembly, "\t\tmovsd     [%s] , xmm0\n", ir->result->name);
                    
                // }
            }

            else
            {
                temp = local_ST->func_curr->entries[indexRight];
                while(temp!=NULL){
                    if(!strcmp(temp->name,nameRight)){
                        break;
                    }
                    temp = temp->next;
                }
                int offsetRight = temp->offset*16;               // Get the memory offset for the lhs variable from the symbol

                // if(!strcmp(resultType, "integer"))
                // {
                    fprintf(assembly, "\t\tor     rax , [%s]\n", ir->right_op->name);
                    
                    fprintf(assembly, "\t\tmov     [%s] , rax\n", ir->result->name);
                    
                // }
                // else
                // {
                //     fprintf(assembly, "\t\torpd     xmm0 , [%s]\n", ir->right_op->name);
                    
                //     fprintf(assembly, "\t\tmovsd     [%s] , xmm0\n", ir->result->name);
                    
                // }
            }
            break;
    }

    fprintf(assembly, "\t\tpop_regs        ; restore register values\n\n\n");
    
}

void codegen_input(ir_code_node* ir, func_entry* local_ST){
    /* The entry will be of the following form
    +------------+----------+----------+-----------+
    |  GET_VALUE |    var   |   NULL   |    NULL   |
    +------------+----------+----------+-----------+
    */
    
    
    

    // Get offset of result
    char* result = ir->result->name;
    int indexResult = sym_tab_entry_contains(result,local_ST->func_curr->entries);        // Checks if the symbol table contains - if yes we get the index
    sym_tab_entry* temp = NULL;
    temp = local_ST->func_curr->entries[indexResult];
    while(temp!=NULL){
        if(!strcmp(temp->name,result)){
            break;
        }
        temp = temp->next;
    }
    int offsetResult = temp->offset*16;               // Get the memory offset for the lhs variable from the symbol
    char* resultType = temp->type.datatype;

    fprintf(assembly, "\t\t; Code for getting user input\n");
    
    fprintf(assembly, "\t\tpush_regs                    ; save values\n");
    

    if(!strcmp(resultType, "integer"))
    {
        fprintf(assembly, "\t\tmov      rdi , fmt_spec_int_in          ; get corresponding format specifier\n");
        
    }
    else if(!strcmp(resultType, "real"))
    {
        fprintf(assembly, "\t\tmov      rdi , fmt_spec_real_in          ; get corresponding format specifier\n");
        
    }
    else if(!strcmp(resultType, "boolean"))
    {
        fprintf(assembly, "\t\tmov      rdi , fmt_spec_bool_in          ; get corresponding format specifier\n");
        
    }

    fprintf(assembly,\
    "\t\t\t\tmov RDX, RBP\n\
                ;sub RDX, 0     ; make RDX to point at location of variable on the stack\n\
                ;So, we are firstly clearing upper 32 bits of memory so as to access data properly later\n\
                mov RSI, %s \n\
                mov RAX, 0 \n\
                rsp_align ;align RSP to 16 byte boundary for scanf call\n\
                call scanf \n\
                rsp_realign ;realign it to original position\n", ir->result->name);

    fprintf(assembly, "\t\tpop_regs        ; restore register values\n\n\n");


}

void codegen_output(ir_code_node* ir, func_entry* local_ST){
    /* The entry will be of the following form
    +------------+----------+----------+-----------+
    |    PRINT   |    var   |   NULL   |    NULL   |
    +------------+----------+----------+-----------+
    */
    
    // Get offset of result
    char* result = ir->result->name;
    int indexResult = sym_tab_entry_contains(result,local_ST->func_curr->entries);   // Checks if the symbol table contains - if yes we get the index
    if(indexResult==-1)
    {   
        if(!strcmp(result, "true"))
        {
            fprintf(assembly, "\t\t; Code for printing output\n");
            fprintf(assembly, "\t\tpush_regs                    ; save values\n");
            fprintf(assembly, "\t\tmov rax, 1                  ; system call for write\n");
            fprintf(assembly, "\t\tmov rdi, 1                  ; file handle 1 is stdout\n");
            fprintf(assembly, "\t\tmov rsi, true               ; address of string to output\n");
            fprintf(assembly, "\t\tmov rdx, true_len           ; number of bytes\n");
            fprintf(assembly, "\t\tsyscall                     ; invoke operating system to do the write\n");
        }
        
        else if(!strcmp(result, "false"))
        {
            fprintf(assembly, "\t\t; Code for printing output\n");
            fprintf(assembly, "\t\tpush_regs                    ; save values\n");
            fprintf(assembly, "\t\tmov rax, 1                  ; system call for write\n");
            fprintf(assembly, "\t\tmov rdi, 1                  ; file handle 1 is stdout\n");
            fprintf(assembly, "\t\tmov rsi, false               ; address of string to output\n");
            fprintf(assembly, "\t\tmov rdx, false_len           ; number of bytes\n");
            fprintf(assembly, "\t\tsyscall                     ; invoke operating system to do the write\n");
        }

        // INT or REAL
        else
        { 
            int type_left_int = (strchr(result, '.'))? -1 : 1;
            
            // INT
            if(type_left_int==1){
                fprintf(assembly, "\t\t; Code for printing output\n");
                
                fprintf(assembly, "\t\tpush_regs                    ; save values\n");
                

                fprintf(assembly, "\t\tmov      rdi , rel fmt_spec_int_out                ; get corresponding format specifier\n");
                

                fprintf(assembly, "\t\tmov      rsi , %s                               ; move source index\n", ir->result->name);
                
                fprintf(assembly, "\t\txor      rax , rax\n");
                
                fprintf(assembly, "\t\trsp_align                                     ; align stack pointer\n");

                fprintf(assembly, "\t\tcall     printf                                   ; system call for output\n");
                
                fprintf(assembly, "\t\trsp_realign                                      ; restore previous alignment of stack\n");

            }

            // FLOAT
            else if(type_left_int==-1){
                fprintf(assembly, "\t\t; Code for printing output\n");
                
                fprintf(assembly, "\t\tpush_regs                    ; save values\n");
                

                fprintf(assembly, "\t\tmov      rdi , rel fmt_spec_real_out                ; get corresponding format specifier\n");
                

                fprintf(assembly, "\t\tmov      rsi , %s                               ; move source index\n", ir->result->name);
                
                fprintf(assembly, "\t\txor      rax , rax\n");
                
                fprintf(assembly, "\t\trsp_align                                     ; align stack pointer\n");

                fprintf(assembly, "\t\tcall     printf                                   ; system call for output\n");
                
                fprintf(assembly, "\t\trsp_realign                                      ; restore previous alignment of stack\n");
            }
        }
    }

    else
    {
        sym_tab_entry* temp = NULL;
        temp = local_ST->func_curr->entries[indexResult];
        while(temp!=NULL){
            if(!strcmp(temp->name,result)){
                break;
            }
            temp = temp->next;
        }
        int offsetResult = temp->offset*16;               // Get the memory offset for the lhs variable from the symbol
        char* resultType = temp->type.datatype;

        fprintf(assembly, "\t\t; Code for printing output\n");
        
        fprintf(assembly, "\t\tpush_regs                    ; save values\n");
        

        if(!strcmp(resultType, "integer"))
        {
            fprintf(assembly, "\t\tmov      rdi , fmt_spec_int_out                  ; get corresponding format specifier\n");
            

            // fprintf(assembly, "\t\tmov      rdx , rbp                               ; take base pointer in rdx\n");
            
            // fprintf(assembly, "\t\tsub      rdx , %d                                ; move pointer to place from where we have to read\n", ir->result->name);
            
            fprintf(assembly, "\t\tmov      rsi , [%s]                               ; move source index\n",ir->result->name);
            
            fprintf(assembly, "\t\txor      rax , rax\n");
            
            fprintf(assembly, "\t\trsp_align                                     ; align stack pointer\n");
            // 
            fprintf(assembly, "\t\tcall     printf                                   ; system call for output\n");
            
            fprintf(assembly, "\t\trsp_realign                                      ; restore previous alignment of stack\n");
            // 
            fprintf(assembly, "\t\tpop_regs                    ; save values\n");

        }
        else if(!strcmp(resultType, "real"))
        {
            fprintf(assembly, "\t\tmov      rdi , fmt_spec_real_out          ; get corresponding format specifier\n");
            

            // fprintf(assembly, "\t\tmov      rdx , rbp                               ; take base pointer in rdx\n");
            
            // fprintf(assembly, "\t\tsub      rdx , %d                                ; move pointer to place from where we have to read\n", offsetResult);
            
            fprintf(assembly, "\t\tmovq      xmm0 ,[%s]                               ; move source index\n",ir->result->name);
            
            fprintf(assembly, "\t\tmov      rax , 1\n");
            
            // fprintf(assembly, "\t\trsp_align                                     ; align stack pointer\n");
            // 
            fprintf(assembly, "\t\tcall     printf                                   ; system call for output\n");
            
            // fprintf(assembly, "\t\trsp_realign                                      ; restore previous alignment of stack\n");
            fprintf(assembly, "\t\tpop_regs                    ; save values\n");
            
        }
        else if(!strcmp(resultType, "boolean"))
        {

            fprintf(assembly, "\t\tmov      rdi , fmt_spec_bool_out          ; get corresponding format specifier\n");
            

            char* true_label = newLabel();
            char* next_label = newLabel();

            // fprintf(assembly, "\t\tmov      rdx , rbp                               ; take base pointer in rdx\n");
            
            // fprintf(assembly, "\t\tsub      rdx , %d                                ; move pointer to place from where we have to read\n", offsetResult);
            
            fprintf(assembly, "\t\tmov      rax , [%s]                               ; move source index\n",ir->result->name);
            
            fprintf(assembly, "\t\tcmp      rax , 0\n");
            
            fprintf(assembly, "\t\tjnz      %s\n", true_label);
            
            fprintf(assembly, "\t\tcmp      rax , 0\n");
            
            // fprintf(assembly, "\t\tpush_regs                    ; save values\n");
            
            fprintf(assembly, "\t\t; to print false\n");
            
            fprintf(assembly, "\t\tmov      rax , 1\n");
            
            fprintf(assembly, "\t\tmov      rdi , 1\n");
            
            fprintf(assembly, "\t\tmov      rsi , false\n");
            
            fprintf(assembly, "\t\tmov      rdx , false_len\n");
            
            fprintf(assembly, "\t\tsyscall\n");
            
            fprintf(assembly, "\t\tpop_regs        ; restore register values\n");
            
            fprintf(assembly, "\t\tjmp  %s      ; restore register values\n\n\n",next_label);
            

            // fprintf(assembly, "\t\tpush_regs                    ; save values\n");
            
            fprintf(assembly, "%s:\n", true_label);
            
            fprintf(assembly, "\t\t; to print true\n");
            
            fprintf(assembly, "\t\tmov      rax , 1\n");
            
            fprintf(assembly, "\t\tmov      rdi , 1\n");
            
            fprintf(assembly, "\t\tmov      rsi , true\n");
            
            fprintf(assembly, "\t\tmov      rdx , true_len\n");
            
            fprintf(assembly, "\t\tsyscall\n");
            
            fprintf(assembly, "\t\tpop_regs        ; restore register values\n");
            
            fprintf(assembly, "%s:\n\n\n", next_label);
            
            // fprintf(assembly, "\t\trsp_align                                     ; align stack pointer\n");
            // 
            // fprintf(assembly, "\t\tcall     printf                                   ; system call for output\n");
            // 
            // fprintf(assembly, "\t\trsp_realign                                      ; restore previous alignment of stack\n");
            // 
        }
    }
    // fprintf(assembly, "\t\tpop_regs        ; restore register values\n\n\n");
    
    
}

void codegen_arithmetic(ir_code_node* ir, func_entry* local_ST){
    /* The entry will be of the following form
    +------------+----------+----------+-----------+
    |    label   |    temp  |    op1   |    op2    |
    +------------+----------+----------+-----------+
    label can be one of ADD, SUB, MUL, DIv
    */

    // Get offset of result
    char* result = ir->result->name;
    int indexResult = sym_tab_entry_contains(result,local_ST->func_curr->entries);        // Checks if the symbol table contains - if yes we get the index
    sym_tab_entry* temp = NULL;
    temp = local_ST->func_curr->entries[indexResult];
    while(temp!=NULL){
        if(!strcmp(temp->name,result)){
            break;
        }
        temp = temp->next;
    }
    int offsetResult = temp->offset*16;               // Get the memory offset for the lhs variable from the symbol
    char* resultType = temp->type.datatype;
    char* nameLeft = ir->left_op->name;
    int indexLeft = sym_tab_entry_contains(nameLeft,local_ST->func_curr->entries);        // Checks if the symbol table contains - if yes we get the index

    // Get offset of left operand temp
    fprintf(assembly, "\t\t; Code for arithmetic\n");
    
    fprintf(assembly, "\t\tpush_regs                    ; save values\n");
    

    // If left operand is a constant
    if(indexLeft == -1)
    {
        int type_left_int = (strchr(nameLeft, '.'))? 0 : 1;
        // TODO ----> identifying if immediate value is int or real
        if(type_left_int)
        {
            fprintf(assembly, "\t\tmov     rax , %s            ; immediate to memory\n", nameLeft);
            
        }

        else
        {
            fprintf(assembly, "\t\tmovsd     xmm0 , %s            ; immediate to memory\n", nameLeft);
            
        }
    }

    else
    {
        temp = local_ST->func_curr->entries[indexLeft];
        while(temp!=NULL){
            if(!strcmp(temp->name,nameLeft)){
                break;
            }
            temp = temp->next;
        }
        int offsetLeft = temp->offset*16;               // Get the memory offset for the lhs variable from the symbol

        if(!strcmp(resultType, "integer"))
        {
            fprintf(assembly, "\t\tmov     rax , [%s]\n", ir->left_op->name);
            
        }
        else
        {
            fprintf(assembly, "\t\tmovsd     xmm0 , [%s]\n", ir->left_op->name);
            
        }
        
    }

    // For right operand
    switch(ir->operator)
    {
        char* nameRight;
        int indexRight;
        case ADD:
            // Get offset of right operand temp
            nameRight = ir->right_op->name;
            indexRight = sym_tab_entry_contains(nameRight,local_ST->func_curr->entries);        // Checks if the symbol table contains - if yes we get the index
            // If right operand is a constant
            if(indexRight == -1)
            {
                int type_right_int = (strchr(nameRight, '.'))? 0 : 1;
                // CHECK ----> identifying if immediate value is int or real
                if(type_right_int)
                {
                    fprintf(assembly, "\t\tadd     rax , [%s]\n", ir->right_op->name);
                    
                    fprintf(assembly, "\t\tmov     [%s] , rax\n", ir->result->name);
                    
                }

                else
                {
                    fprintf(assembly, "\t\taddsd     xmm0 , [%s]\n", ir->right_op->name);
                    
                    fprintf(assembly, "\t\tmovsd     [%s] , xmm0\n", ir->result->name);
                    
                }
            }

            else
            {
                temp = local_ST->func_curr->entries[indexRight];
                while(temp!=NULL){
                    if(!strcmp(temp->name,nameRight)){
                        break;
                    }
                    temp = temp->next;
                }
                int offsetRight = temp->offset*16;               // Get the memory offset for the lhs variable from the symbol

                if(!strcmp(resultType, "integer"))
                {
                    fprintf(assembly, "\t\tadd     rax , [%s]\n", ir->right_op->name);
                    
                    fprintf(assembly, "\t\tmov     [%s] , rax\n", ir->result->name);
                    
                }
                else
                {
                    fprintf(assembly, "\t\taddsd     xmm0 , [%s]\n", ir->right_op->name);
                    
                    fprintf(assembly, "\t\tmovsd     [%s] , xmm0\n", ir->result->name);
                    
                }
            }
            break;
        case SUB:
            // Get offset of right operand temp
            nameRight = ir->right_op->name;
            indexRight = sym_tab_entry_contains(nameRight,local_ST->func_curr->entries);        // Checks if the symbol table contains - if yes we get the index
            // If right operand is a constant
            if(indexRight == -1)
            {
                int type_right_int = (strchr(nameRight, '.'))? 0 : 1;
                // CHECK ----> identifying if immediate value is int or real
                if(type_right_int)
                {
                    fprintf(assembly, "\t\tsub     rax , %s\n", nameRight);
                    
                    fprintf(assembly, "\t\tmov     [%s] , rax\n", ir->result->name);
                    
                }

                else
                {
                    fprintf(assembly, "\t\tsubsd     xmm0 , %s\n", nameRight);
                    
                    fprintf(assembly, "\t\tmovsd     [%s] , xmm0\n", ir->result->name);
                    
                }
            }

            else
            {
                temp = local_ST->func_curr->entries[indexRight];
                while(temp!=NULL){
                    if(!strcmp(temp->name,nameRight)){
                        break;
                    }
                    temp = temp->next;
                }
                int offsetRight = temp->offset*16;               // Get the memory offset for the lhs variable from the symbol

                if(!strcmp(resultType, "integer"))
                {
                    fprintf(assembly, "\t\tsub     rax , [%s]\n", ir->right_op->name);
                    
                    fprintf(assembly, "\t\tmov     [%s] , rax\n", ir->result->name);
                    
                }
                else
                {
                    fprintf(assembly, "\t\tsubsd     xmm0 , [%s]\n", ir->right_op->name);
                    
                    fprintf(assembly, "\t\tmovsd     [%s] , xmm0\n", ir->result->name);
                    
                }
            }
            break;
        case MUL:
            // Get offset of right operand temp
            nameRight = ir->right_op->name;
            indexRight = sym_tab_entry_contains(nameRight,local_ST->func_curr->entries);        // Checks if the symbol table contains - if yes we get the index
            // If right operand is a constant
            if(indexRight == -1)
            {
                int type_right_int = (strchr(nameRight, '.') != NULL)? 0 : 1;

                // CHECK ----> identifying if immediate value is int or real
                if(type_right_int)
                {
                    
                    fprintf(assembly, "\t\tmov     rbx , %s\n", nameRight);
                    
                    fprintf(assembly, "\t\timul     rbx\n");
                    
                    fprintf(assembly, "\t\tmov    [%s] , rax\n", ir->result->name);
                    
                }

                else
                {
                    fprintf(assembly, "\t\tmovsd      xmm1 , %s\n", nameRight);

                    fprintf(assembly, "\t\tmulsd     xmm1\n");
                    
                    fprintf(assembly, "\t\tmovsd     [%s] , xmm0\n", ir->result->name);
                    
                }
            }

            else
            {
                temp = local_ST->func_curr->entries[indexRight];
                while(temp!=NULL){
                    if(!strcmp(temp->name,nameRight)){
                        break;
                    }
                    temp = temp->next;
                }
                int offsetRight = temp->offset*16;               // Get the memory offset for the lhs variable from the symbol

                if(!strcmp(resultType, "integer"))
                {
                    fprintf(assembly, "\t\tmov     rbx , [%s]\n", ir->right_op->name);

                    fprintf(assembly, "\t\timul     rbx\n");
                    
                    fprintf(assembly, "\t\tmov     [%s] , rax\n", ir->result->name);
                    
                }
                else
                {
                    fprintf(assembly, "\t\tmovsd     xmm1 , [%s]\n", ir->right_op->name);
                  
                    fprintf(assembly, "\t\tmulsd     xmm1\n");
                    
                    fprintf(assembly, "\t\tmovsd     [%s] , xmm0\n", ir->result->name);
                    
                }
            }
            break;
        case DIV:
            // Get offset of right operand temp
            nameRight = ir->right_op->name;
            indexRight = sym_tab_entry_contains(nameRight,local_ST->func_curr->entries);        // Checks if the symbol table contains - if yes we get the index
            // If right operand is a constant
            if(indexRight == -1)
            {
                fprintf(assembly, "\t\tdivsd     xmm0 , %s\n", nameRight);
                
                fprintf(assembly, "\t\tmovsd     [%s] , xmm0\n", ir->result->name);
                
            }

            else
            {
                temp = local_ST->func_curr->entries[indexRight];
                while(temp!=NULL){
                    if(!strcmp(temp->name,nameRight)){
                        break;
                    }
                    temp = temp->next;
                }
                int offsetRight = temp->offset*16;               // Get the memory offset for the lhs variable from the symbol

                fprintf(assembly, "\t\tdivsd     xmm0 , [%s]\n", ir->right_op->name);
                
                fprintf(assembly, "\t\tmovsd     [%s] , xmm0\n", ir->result->name);
                
            }
            break;
    }

    fprintf(assembly, "\t\tpop_regs        ; restore register values\n\n\n");
    
    
}

void codegen_relational(ir_code_node* ir, func_entry* local_ST){
    /* The entry will be of the following form
    +------------+----------+----------+-----------+
    |    label   |    temp  |    op1   |    op2    |
    +------------+----------+----------+-----------+
    label can be one of LT, LTE, GT, GTE, EQ, NEQ
    */

    
    
    
    
    

    // Get offset of result
    char* result = ir->result->name;
    int indexResult = sym_tab_entry_contains(result,local_ST->func_curr->entries);        // Checks if the symbol table contains - if yes we get the index
    sym_tab_entry* temp = NULL;
    temp = local_ST->func_curr->entries[indexResult];
    while(temp!=NULL){
        if(!strcmp(temp->name,result)){
            break;
        }
        temp = temp->next;
    }
    int offsetResult = temp->offset*16;               // Get the memory offset for the lhs variable from the symbol
    char* resultType = temp->type.datatype;

    // Get offset of left operand temp
    char* nameLeft = ir->left_op->name;
    int indexLeft = sym_tab_entry_contains(nameLeft,local_ST->func_curr->entries);        // Checks if the symbol table contains - if yes we get the index
    fprintf(assembly, "\t\t; Code for relational\n");
    
    fprintf(assembly, "\t\tpush_regs                    ; save values\n");
    

    // If left operand is a constant
    if(indexLeft == -1)
    {
        // If strchr(nameLeft, '.') returns null then it means left operand has no '.'
        // and hence it is an integer constant (immediate value)
        int type_left_int = (strchr(nameLeft, '.'))? 0 : 1;
        if(type_left_int)
        {
            fprintf(assembly, "\t\tmov     rax , %s            ; immediate to memory\n", nameLeft);
            
        }

        else
        {
            fprintf(assembly, "\t\tmovsd     xmm0 , %s            ; immediate to memory\n", nameLeft);
            
        }
    }

    else
    {
        temp = local_ST->func_curr->entries[indexLeft];
        while(temp!=NULL){
            if(!strcmp(temp->name,nameLeft)){
                break;
            }
            temp = temp->next;
        }
        int offsetLeft = temp->offset*16;               // Get the memory offset for the lhs variable from the symbol
        char* leftType = temp->type.datatype;

        if(!strcmp(leftType, "integer"))
        {
            fprintf(assembly, "\t\tmov     rax , [%s]\n", ir->left_op->name);
            
        }
        else
        {
            fprintf(assembly, "\t\tmovsd     xmm0 , [%s]\n", ir->left_op->name);
            
        }
        
    }
    char* nameRight;
    int indexRight;
    char* true_label = newLabel();
    char* next_label = newLabel();

    // For right operand
    switch(ir->operator)
    {
        case LT:
            // Get offset of right operand temp
            nameRight = ir->right_op->name;
            indexRight = sym_tab_entry_contains(nameRight,local_ST->func_curr->entries);        // Checks if the symbol table contains - if yes we get the index
            // If right operand is a constant
            if(indexRight == -1)
            {
                // If strchr(nameRight, '.') returns null then it means right operand has no '.'
                // and hence it is an integer constant (immediate value)
                int type_right_int = (strchr(nameRight, '.'))? 0 : 1;
                if(type_right_int)
                {
                    fprintf(assembly, "\t\tmov     rbx , %s\n", nameRight);
                    
                    fprintf(assembly, "\t\tcmp     rax , rbx\n");
                    
                    fprintf(assembly, "\t\tjl     %s\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [%s], %d\n", ir->result->name, 0);

                    fprintf(assembly, "\t\tjmp  %s\n", next_label);
                    
                    fprintf(assembly, "%s:\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [%s], %d\n", ir->result->name, 1);

                    fprintf(assembly, "%s:\n", next_label);
                    
                }

                else
                {
                    fprintf(assembly, "\t\tmov     xmm1 , %s\n", nameRight);
                    
                    fprintf(assembly, "\t\tcomiss     xmm0 , xmm1\n");
                    
                    fprintf(assembly, "\t\tjb     %s\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [%s], %d\n", ir->result->name, 0);
                    
                    fprintf(assembly, "\t\tjmp  %s\n", next_label);
                    
                    fprintf(assembly, "%s:\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [%s], %d\n", ir->result->name, 1);

                    fprintf(assembly, "%s:\n", next_label);
                    
                }
            }

            else
            {
                temp = local_ST->func_curr->entries[indexRight];
                while(temp!=NULL){
                    if(!strcmp(temp->name,nameRight)){
                        break;
                    }
                    temp = temp->next;
                }
                int offsetRight = temp->offset*16;               // Get the memory offset for the lhs variable from the symbol
                char* rightType = temp->type.datatype;

                if(!strcmp(rightType, "integer"))
                {
                    fprintf(assembly, "\t\tmov     rbx , [%s]\n", ir->right_op->name);
                    
                    fprintf(assembly, "\t\tcmp     rax , rbx\n");
                    
                    fprintf(assembly, "\t\tjl     %s\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [%s], %d\n", ir->result->name, 0);
                    
                    fprintf(assembly, "\t\tjmp  %s\n", next_label);
                    
                    fprintf(assembly, "%s:\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [%s], %d\n", ir->result->name, 1);

                    fprintf(assembly, "%s:\n", next_label);
                    
                }

                else
                {
                    fprintf(assembly, "\t\tmov     xmm1 , [%s]\n", ir->right_op->name);
                    
                    fprintf(assembly, "\t\tcomiss     xmm0 , xmm1\n");
                    
                    fprintf(assembly, "\t\tjb     %s\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [%s], %d\n", ir->result->name, 0);
                    
                    fprintf(assembly, "\t\tjmp  %s\n", next_label);
                    
                    fprintf(assembly, "%s:\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [%s], %d\n", ir->result->name, 1);

                    fprintf(assembly, "%s:\n", next_label);
                    
                }
            }
            break;
        case GT:
            // Get offset of right operand temp
            nameRight = ir->right_op->name;
            indexRight = sym_tab_entry_contains(nameRight,local_ST->func_curr->entries);        // Checks if the symbol table contains - if yes we get the index
            // If right operand is a constant
            if(indexRight == -1)
            {
                // If strchr(nameRight, '.') returns null then it means right operand has no '.'
                // and hence it is an integer constant (immediate value)
                int type_right_int = (strchr(nameRight, '.'))? 0 : 1;
                if(type_right_int)
                {
                    fprintf(assembly, "\t\tmov     rbx , %s\n", nameRight);
                    
                    fprintf(assembly, "\t\tcmp     rax , rbx\n");
                    
                    fprintf(assembly, "\t\tjg     %s\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [%s], %d\n", ir->result->name, 0);
                    
                    fprintf(assembly, "\t\tjmp  %s\n", next_label);
                    
                    fprintf(assembly, "%s:\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [%s], %d\n", ir->result->name, 1);

                    fprintf(assembly, "%s:\n", next_label);
                    
                }

                else
                {
                    fprintf(assembly, "\t\tmov     xmm1 , %s\n", nameRight);
                    
                    fprintf(assembly, "\t\tcomiss     xmm0 , xmm1\n");
                    
                    fprintf(assembly, "\t\tja     %s\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [%s], %d\n", ir->result->name, 0);
                    
                    fprintf(assembly, "\t\tjmp  %s\n", next_label);
                    
                    fprintf(assembly, "%s:\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [%s], %d\n", ir->result->name, 1);

                    fprintf(assembly, "%s:\n", next_label);
                    
                }
            }

            else
            {
                temp = local_ST->func_curr->entries[indexRight];
                while(temp!=NULL){
                    if(!strcmp(temp->name,nameRight)){
                        break;
                    }
                    temp = temp->next;
                }
                int offsetRight = temp->offset*16;               // Get the memory offset for the lhs variable from the symbol
                char* rightType = temp->type.datatype;

                if(!strcmp(rightType, "integer"))
                {
                    fprintf(assembly, "\t\tmov     rbx , [%s]\n", ir->right_op->name);
                    
                    fprintf(assembly, "\t\tcmp     rax , rbx\n");
                    
                    fprintf(assembly, "\t\tjg     %s\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [%s], %d\n", ir->result->name, 0);
                    
                    fprintf(assembly, "\t\tjmp  %s\n", next_label);
                    
                    fprintf(assembly, "%s:\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [%s], %d\n", ir->result->name, 1);

                    fprintf(assembly, "%s:\n", next_label);
                    
                }
                
                else
                {
                    fprintf(assembly, "\t\tmov     xmm1 , [%s]\n", ir->right_op->name);
                    
                    fprintf(assembly, "\t\tcomiss     xmm0 , xmm1\n");
                    
                    fprintf(assembly, "\t\tjg     %s\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [%s], %d\n", ir->result->name, 0);
                    
                    fprintf(assembly, "\t\tjmp  %s\n", next_label);
                    
                    fprintf(assembly, "%s:\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [%s], %d\n", ir->result->name, 1);

                    fprintf(assembly, "%s:\n", next_label);
                    
                }
            }
            break;
        case LE:
            // Get offset of right operand temp
            nameRight = ir->right_op->name;
            indexRight = sym_tab_entry_contains(nameRight,local_ST->func_curr->entries);        // Checks if the symbol table contains - if yes we get the index
            // If right operand is a constant
            if(indexRight == -1)
            {
                // If strchr(nameRight, '.') returns null then it means right operand has no '.'
                // and hence it is an integer constant (immediate value)
                int type_right_int = (strchr(nameRight, '.'))? 0 : 1;
                if(type_right_int)
                {
                    fprintf(assembly, "\t\tmov     rbx , %s\n", nameRight);
                    
                    fprintf(assembly, "\t\tcmp     rax , rbx\n");
                    
                    fprintf(assembly, "\t\tjle     %s\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [%s], %d\n", ir->result->name, 0);
                    
                    fprintf(assembly, "\t\tjmp  %s\n", next_label);
                    
                    fprintf(assembly, "%s:\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [%s], %d\n", ir->result->name, 1);

                    fprintf(assembly, "%s:\n", next_label);
                    
                }

                else
                {
                    fprintf(assembly, "\t\tmov     xmm1 , %s\n", nameRight);
                    
                    fprintf(assembly, "\t\tcomiss     xmm0 , xmm1\n");
                    
                    fprintf(assembly, "\t\tjb     %s\n", true_label);
                    
                    fprintf(assembly, "\t\tjz     %s\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [%s], %d\n", ir->result->name, 0);
                    
                    fprintf(assembly, "\t\tjmp  %s\n", next_label);
                    
                    fprintf(assembly, "%s:\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [%s], %d\n", ir->result->name, 1);

                    fprintf(assembly, "%s:\n", next_label);
                    
                }
            }

            else
            {
                temp = local_ST->func_curr->entries[indexRight];
                while(temp!=NULL){
                    if(!strcmp(temp->name,nameRight)){
                        break;
                    }
                    temp = temp->next;
                }
                int offsetRight = temp->offset*16;               // Get the memory offset for the lhs variable from the symbol
                char* rightType = temp->type.datatype;

                if(!strcmp(rightType, "integer"))
                {
                    fprintf(assembly, "\t\tmov     rbx , [%s]\n", ir->right_op->name);
                    
                    fprintf(assembly, "\t\tcmp     rax , rbx\n");
                    
                    fprintf(assembly, "\t\tjle     %s\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [%s], %d\n", ir->result->name, 0);
                    
                    fprintf(assembly, "\t\tjmp  %s\n", next_label);
                    
                    fprintf(assembly, "%s:\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [%s], %d\n", ir->result->name, 1);

                    fprintf(assembly, "%s:\n", next_label);
                    
                }
                
                else
                {
                    fprintf(assembly, "\t\tmov     xmm1 , [%s]\n", ir->right_op->name);
                    
                    fprintf(assembly, "\t\tcomiss     xmm0 , xmm1\n");
                    
                    fprintf(assembly, "\t\tjb     %s\n", true_label);
                    
                    fprintf(assembly, "\t\tjz     %s\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [%s], %d\n", ir->result->name, 0);
                    
                    fprintf(assembly, "\t\tjmp  %s\n", next_label);
                    
                    fprintf(assembly, "%s:\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [%s], %d\n", ir->result->name, 1);

                    fprintf(assembly, "%s:\n", next_label);
                    
                }
            }
            break;
        case GE:
            // Get offset of right operand temp
            nameRight = ir->right_op->name;
            indexRight = sym_tab_entry_contains(nameRight,local_ST->func_curr->entries);        // Checks if the symbol table contains - if yes we get the index
            // If right operand is a constant
            if(indexRight == -1)
            {
                // If strchr(nameRight, '.') returns null then it means right operand has no '.'
                // and hence it is an integer constant (immediate value)
                int type_right_int = (strchr(nameRight, '.'))? 0 : 1;
                if(type_right_int)
                {
                    fprintf(assembly, "\t\tmov     rbx , %s\n", nameRight);
                    
                    fprintf(assembly, "\t\tcmp     rax , rbx\n");
                    
                    fprintf(assembly, "\t\tjge     %s\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [%s], %d\n", ir->result->name, 0);
                    
                    fprintf(assembly, "\t\tjmp  %s\n", next_label);
                    
                    fprintf(assembly, "%s:\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [%s], %d\n", ir->result->name, 1);

                    fprintf(assembly, "%s:\n", next_label);
                    
                }

                else
                {
                    fprintf(assembly, "\t\tmov     xmm1 , %s\n", nameRight);
                    
                    fprintf(assembly, "\t\tcomiss     xmm0 , xmm1\n");
                    
                    fprintf(assembly, "\t\tja     %s\n", true_label);
                    
                    fprintf(assembly, "\t\tjz     %s\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [%s], %d\n", ir->result->name, 0);
                    
                    fprintf(assembly, "\t\tjmp  %s\n", next_label);
                    
                    fprintf(assembly, "%s:\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [%s], %d\n", ir->result->name, 1);

                    fprintf(assembly, "%s:\n", next_label);
                    
                }
            }

            else
            {
                temp = local_ST->func_curr->entries[indexRight];
                while(temp!=NULL){
                    if(!strcmp(temp->name,nameRight)){
                        break;
                    }
                    temp = temp->next;
                }
                int offsetRight = temp->offset*16;               // Get the memory offset for the lhs variable from the symbol
                char* rightType = temp->type.datatype;

                if(!strcmp(rightType, "integer"))
                {
                    fprintf(assembly, "\t\tmov     rbx , [%s]\n", ir->right_op->name);
                    
                    fprintf(assembly, "\t\tcmp     rax , rbx\n");
                    
                    fprintf(assembly, "\t\tjge     %s\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [%s], %d\n", ir->result->name, 0);
                    
                    fprintf(assembly, "\t\tjmp  %s\n", next_label);
                    
                    fprintf(assembly, "%s:\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [%s], %d\n", ir->result->name, 1);

                    fprintf(assembly, "%s:\n", next_label);
                    
                }
                
                else
                {
                    fprintf(assembly, "\t\tmov     xmm1 , [%s]\n", ir->right_op->name);
                    
                    fprintf(assembly, "\t\tcomiss     xmm0 , xmm1\n");
                    
                    fprintf(assembly, "\t\tja     %s\n", true_label);
                    
                    fprintf(assembly, "\t\tjz     %s\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [%s], %d\n", ir->result->name, 0);
                    
                    fprintf(assembly, "\t\tjmp  %s\n", next_label);
                    
                    fprintf(assembly, "%s:\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [%s], %d\n", ir->result->name, 1);

                    fprintf(assembly, "%s:\n", next_label);
                    
                }
            }
            break;
        case EQ:
            // Get offset of right operand temp
            nameRight = ir->right_op->name;
            indexRight = sym_tab_entry_contains(nameRight,local_ST->func_curr->entries);        // Checks if the symbol table contains - if yes we get the index
            // If right operand is a constant
            if(indexRight == -1)
            {
                // If strchr(nameRight, '.') returns null then it means right operand has no '.'
                // and hence it is an integer constant (immediate value)
                int type_right_int = (strchr(nameRight, '.'))? 0 : 1;
                if(type_right_int)
                {
                    fprintf(assembly, "\t\tmov     rbx , %s\n", nameRight);
                    
                    fprintf(assembly, "\t\tcmp     rax , rbx\n");
                    
                    fprintf(assembly, "\t\tjz     %s\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [%s], %d\n", ir->result->name, 0);
                    
                    fprintf(assembly, "\t\tjmp  %s\n", next_label);
                    
                    fprintf(assembly, "%s:\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [%s], %d\n", ir->result->name, 1);

                    fprintf(assembly, "%s:\n", next_label);
                    
                }

                else
                {
                    fprintf(assembly, "\t\tmov     xmm1 , %s\n", nameRight);
                    
                    fprintf(assembly, "\t\tcomiss     xmm0 , xmm1\n");
                    
                    fprintf(assembly, "\t\tjz     %s\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [%s], %d\n", ir->result->name, 0);
                    
                    fprintf(assembly, "\t\tjmp  %s\n", next_label);
                    
                    fprintf(assembly, "%s:\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [%s], %d\n", ir->result->name, 1);

                    fprintf(assembly, "%s:\n", next_label);
                    
                }
            }

            else
            {
                temp = local_ST->func_curr->entries[indexRight];
                while(temp!=NULL){
                    if(!strcmp(temp->name,nameRight)){
                        break;
                    }
                    temp = temp->next;
                }
                int offsetRight = temp->offset*16;               // Get the memory offset for the lhs variable from the symbol
                char* rightType = temp->type.datatype;

                if(!strcmp(rightType, "integer"))
                {
                    fprintf(assembly, "\t\tmov     rbx , [%s]\n", ir->right_op->name);
                    
                    fprintf(assembly, "\t\tcmp     rax , rbx\n");
                    
                    fprintf(assembly, "\t\tjz     %s\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [%s], %d\n", ir->result->name, 0);
                    
                    fprintf(assembly, "\t\tjmp  %s\n", next_label);
                    
                    fprintf(assembly, "%s:\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [%s], %d\n", ir->result->name, 1);

                    fprintf(assembly, "%s:\n", next_label);
                    
                }
                
                else
                {
                    fprintf(assembly, "\t\tmov     xmm1 , [%s]\n", ir->right_op->name);
                    
                    fprintf(assembly, "\t\tcomiss     xmm0 , xmm1\n");
                    
                    fprintf(assembly, "\t\tjz     %s\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [%s], %d\n", ir->result->name, 0);
                    
                    fprintf(assembly, "\t\tjmp  %s\n", next_label);
                    
                    fprintf(assembly, "%s:\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [%s], %d\n", ir->result->name, 1);

                    fprintf(assembly, "%s:\n", next_label);
                    
                }
            }
            break;
        case NEQ:
            // Get offset of right operand temp
            nameRight = ir->right_op->name;
            indexRight = sym_tab_entry_contains(nameRight,local_ST->func_curr->entries);        // Checks if the symbol table contains - if yes we get the index
            // If right operand is a constant
            if(indexRight == -1)
            {
                // If strchr(nameRight, '.') returns null then it means right operand has no '.'
                // and hence it is an integer constant (immediate value)
                int type_right_int = (strchr(nameRight, '.'))? 0 : 1;
                if(type_right_int)
                {
                    fprintf(assembly, "\t\tmov     rbx , %s\n", nameRight);
                    
                    fprintf(assembly, "\t\tcmp     rax , rbx\n");
                    
                    fprintf(assembly, "\t\tjnz     %s\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [%s], %d\n", ir->result->name, 0);
                    
                    fprintf(assembly, "\t\tjmp  %s\n", next_label);
                    
                    fprintf(assembly, "%s:\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [%s], %d\n", ir->result->name, 1);

                    fprintf(assembly, "%s:\n", next_label);
                    
                }

                else
                {
                    fprintf(assembly, "\t\tmov     xmm1 , %s\n", nameRight);
                    
                    fprintf(assembly, "\t\tcomiss     xmm0 , xmm1\n");
                    
                    fprintf(assembly, "\t\tjnz     %s\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [%s], %d\n", ir->result->name, 0);
                    
                    fprintf(assembly, "\t\tjmp  %s\n", next_label);
                    
                    fprintf(assembly, "%s:\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [%s], %d\n", ir->result->name, 1);

                    fprintf(assembly, "%s:\n", next_label);
                    
                }
            }

            else
            {
                temp = local_ST->func_curr->entries[indexRight];
                while(temp!=NULL){
                    if(!strcmp(temp->name,nameRight)){
                        break;
                    }
                    temp = temp->next;
                }
                int offsetRight = temp->offset*16;               // Get the memory offset for the lhs variable from the symbol
                char* rightType = temp->type.datatype;

                if(!strcmp(rightType, "integer"))
                {
                    fprintf(assembly, "\t\tmov     rbx , [%s]\n", ir->right_op->name);
                    
                    fprintf(assembly, "\t\tcmp     rax , rbx\n");
                    
                    fprintf(assembly, "\t\tjnz     %s\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [%s], %d\n", ir->result->name, 0);
                    
                    fprintf(assembly, "\t\tjmp  %s\n", next_label);
                    
                    fprintf(assembly, "%s:\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [%s], %d\n", ir->result->name, 1);

                    fprintf(assembly, "%s:\n", next_label);
                    
                }

                else
                {
                    fprintf(assembly, "\t\tmov     xmm1 , [%s]\n", ir->right_op->name);
                    
                    fprintf(assembly, "\t\tcomiss     xmm0 , xmm1\n");
                    
                    fprintf(assembly, "\t\tjnz     %s\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [%s], %d\n", ir->result->name, 0);
                    
                    fprintf(assembly, "\t\tjmp  %s\n", next_label);
                    
                    fprintf(assembly, "%s:\n", true_label);
                    
                    fprintf(assembly, "\t\tmov     qword [%s], %d\n", ir->result->name, 1);

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
    */
    
    
   
    char* funcName = ir->result->name;

    fprintf(assembly, "%s:\n",funcName);

    fprintf(assembly, "\t\tpush_regs                    ; save values\n");

    fprintf(assembly, "\t\tmov      rbp , rsp               ; set base to current stack top\n");
    fprintf(assembly, "\t\tpop_regs                     ; save values\n");
    
    // fprintf(assembly, "\t\tpop_regs        ; restore register values\n");


    
}

void codegen_conditional(ir_code_node* ir, func_entry* local_ST)
{
    /* Handling the symbol table ops*/

    // Writing the assembly for conditional

    /* Handling the symbol table ops*/
}

void codegen_label(ir_code_node* ir, func_entry* local_ST)
{
    /* Handling the symbol table ops*/

    // Writing the assembly for label

    /* Handling the symbol table ops*/
}

void codegen_mem_read_ST(ir_code_node* ir, func_entry* local_ST)
{
    /* Handling the symbol table ops*/

    // Writing the assembly for label

    /* Handling the symbol table ops*/
}

void codegen_mem_write_ST(ir_code_node* ir, func_entry* local_ST)
{
    /* Handling the symbol table ops*/

    // Writing the assembly for label

    /* Handling the symbol table ops*/
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

    assembly = assembly_file;
    // assembly = fopen("assembly_try.asm","w");

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
        fprintf(assembly, "\t\tfmt_spec_int_in: db \"%%d\", 0\n");
        fprintf(assembly, "\t\tfmt_spec_int_out: db \"%%d\", 10, 0\n");
        fprintf(assembly, "\t\tfmt_spec_real_in: db \"%%4f\", 0\n");
        fprintf(assembly, "\t\tfmt_spec_real_out: db \"%%4f\", 10, 0\n");
        fprintf(assembly, "\t\tfmt_spec_string: db \"%%s\", 10, 0\n");
        fprintf(assembly, "\t\tfmt_spec_bool_in: db \"%%d\", 0\n");
        fprintf(assembly, "\t\tfmt_spec_bool_out: db \"%%d\", 10, 0\n");
        fprintf(assembly, "\t\ttrue: db \"true\", 10, 0\n");
        fprintf(assembly, "\t\ttrue_len: equ $ - true\n");
        fprintf(assembly, "\t\tfalse: db \"false\", 10, 0\n");
        fprintf(assembly, "\t\tfalse_len: equ $ - false\n");
        fprintf(assembly, "\t\tzero: equ 0\n");

        data_read(assembly);

        /* Data declaration of various types to be done by going thro each entry of the symbol table*/

        fprintf(assembly, "\n\n\t\tsection      .text\n");
        macros_starter();               // Define the macros
        fprintf(assembly, "\t\tglobal main\n");
        // fprintf(assembly, "main:\n");

        printf("[+] ASM file updated!\n");

    }
    ir_code_node *IR_head = IR->head;
    var_record* func_curr;
    func_entry* local_ST;



    // Go through each entry of the IR quadruple
    while(IR_head)
    {
        if(IR_head->operator==FUNC){ //if you reach FUNC --> then change the context
            
            //in case it is main --we have to search for DRIVER
            if (!strcmp(IR_head->result->name,"main")) local_ST = find_module_global("DRIVER");
            else local_ST = find_module_global(IR_head->result->name);
            func_curr = local_ST->func_root; //once we get the local_ST then just locate the root
        }
        type_exp* left_exp;
        type_exp* right_exp;
        type_exp* res_exp;


        // int index = ;        // Checks if the symbol table contains - if yes we get the index
        
        if(IR_head->left_op->name&& (sym_tab_entry_contains(IR_head->left_op->name,local_ST->func_curr->entries)!=-1)){
            left_exp = find_expr_codegen(IR_head->left_op->name,local_ST);
            char* temp = (char*)malloc(sizeof(char)*30);
            memset(temp,'\0',sizeof(temp));
            strcpy(temp, IR_head->left_op->name);
            strcat(temp, left_exp->reach_defined);
            IR_head->left_op->name = temp;
            // strcat(IR_head->left_op->name, left_exp->reach_defined);
            // fprintf(assembly, "\t\t%s   dd   0\n",strcat(IR_head->left_op->name,IR_head->left_op->reach));
        }
        if(IR_head->right_op->name&& (sym_tab_entry_contains(IR_head->right_op->name,local_ST->func_curr->entries)!=-1)){
            right_exp = find_expr_codegen(IR_head->right_op->name,local_ST);
            char* temp = (char*)malloc(sizeof(char)*30);
            memset(temp,'\0',sizeof(temp));
            strcpy(temp, IR_head->right_op->name);
            strcat(temp, right_exp->reach_defined);
            IR_head->right_op->name = temp;
            // strcat(IR_head->right_op->name, right_exp->reach_defined);
            // fprintf(assembly, "\t\t%s   dd   0\n",strcat(IR_head->right_op->name,IR_head->right_op->reach));
        }
        if(IR_head->result->name&&strcmp(IR_head->result->name,"main")&& (sym_tab_entry_contains(IR_head->result->name,local_ST->func_curr->entries)!=-1)){
            res_exp = find_expr_codegen(IR_head->result->name,local_ST);
            char* temp = (char*)malloc(sizeof(char)*30);
            memset(temp,'\0',sizeof(temp));
            strcpy(temp, IR_head->result->name);
            strcat(temp, res_exp->reach_defined);
            IR_head->result->name = temp;
        }
        
        char* reach = "";   
        func_curr = local_ST->func_root;

        // char* reach = IR_head->result->reach; 
        // if(reachstrcmp(reach,"")){ //if reach is not "" then find the local construct
        //                       //in the current local_ST
        //     func_curr = find_local_construct(local_ST->name,reach);
        // }else {
        //     func_curr = local_ST->func_root;
        // }

        local_ST->func_curr = func_curr;

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
            // codegen_jump(IR_head, local_ST);
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
    fprintf(assembly, "main_end:\n");
    fprintf(assembly, "\t\tretq");
    fclose(assembly);
}


type_exp* find_in_list_codegen(sym_tab_entry* output,char* key)
{
    while(output){
        if(!strcmp(output->name,key)){
            return &output->type;
        }
        output = output->next;
    }  
    return NULL;
}

type_exp* find_in_func_table_codegen(char* key, func_entry* curr){
    //extract the lexeme out of ast_root
     
    //check if the module has any parameters or not :
  
    //check in the input list 
    // sym_tab_entry* temp = curr->input_list;
    // while(temp!=NULL){
    //     if(!strcmp(temp->name,key)){
    //         return &temp->type;
    //     }
    //     temp = temp->next;
    // }   
    type_exp* input=find_in_list_codegen(curr->input_list, key);
    if(input)
    return input;
    type_exp* output=find_in_list_codegen(curr->ouput_list, key);
    if(output)
    return output;
    printf("%s: ",key);
}

type_exp* find_in_table_codegen(char* key,var_record* table){
    int index = sym_tab_entry_contains(key,table->entries);
    if(index==-1) return NULL;

    sym_tab_entry* temp = table->entries[index];

    while(temp!=NULL){
        if(!strcmp(temp->name,key)){
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
    type_exp* type=find_in_table_codegen(key, current_rec);
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
void data_read(FILE* assembly){
    code_gen(assembly);
}