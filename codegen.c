#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "codegen.h"

FILE* assembly;

void macros_starter(){
    /* This function defines the required macros in the assembly file
    */

   fprintf(assembly, "\t\t; macro to store all registers\n");
   fprintf(assembly, "\t\t%% macro  push_regs    0\n");
   fprintf(assembly, "\t\tpush      rax\n");
   fprintf(assembly, "\t\tpush      rbx\n");
   fprintf(assembly, "\t\tpush      rcx\n");
   fprintf(assembly, "\t\tpush      rdx\n");
   fprintf(assembly, "\t\tpush      rsp\n");
   fprintf(assembly, "\t\tpush      rbp\n");
   fprintf(assembly, "\t\tpush      rsi\n");
   fprintf(assembly, "\t\tpush      rdi\n");
   fprintf(assembly, "\t\t%% endmacro\n\n\n");

   fprintf(assembly, "\t\t; macro to restore all registers\n");
   fprintf(assembly, "\t\t%% macro  pop_regs    0\n");
   fprintf(assembly, "\t\tpush      rdi\n");
   fprintf(assembly, "\t\tpush      rsi\n");
   fprintf(assembly, "\t\tpush      rbp\n");
   fprintf(assembly, "\t\tpush      rsp\n");
   fprintf(assembly, "\t\tpush      rdx\n");
   fprintf(assembly, "\t\tpush      rcx\n");
   fprintf(assembly, "\t\tpush      rbx\n");
   fprintf(assembly, "\t\tpush      rax\n");
   fprintf(assembly, "\t\t%% endmacro\n\n\n");

   fprintf(assembly, "\t\t; macro to align RSP\n");
   fprintf(assembly, "\t\t%% macro  rsp_align    0\n");
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
   fprintf(assembly, "\t\t%% endmacro\n\n\n");

   fprintf(assembly, "\t\t; macro to re-align RSP\n");
   fprintf(assembly, "\t\t%% macro  rsp_realign    0\n");
   fprintf(assembly, "\t\tadd      rsp , rbx\n");
   fprintf(assembly, "\t\tpush      rdx\n");
   fprintf(assembly, "\t\t%% endmacro\n\n\n");


}

void codegen_assgn_stmt(ir_code_node* ir, func_entry* local_ST,func_entry** global_ST){
    /* The entry will be of the following form
    +------------+----------+----------+-----------+
    |   ASSIGN   |    lhs   |    rhs   |    NULL   |
    +------------+----------+----------+-----------+
    */

    char* asmCode = (char*) malloc(sizeof(char)*20);        // asm code attribute
    char* nameRHS = ir->left_op;      // temporary variable name for rhs

    char* buff = (char*) malloc(sizeof(char)*100);
    memset(buff,'\0',sizeof(buff));

    char* nameLHS = ir->left_op;      // temporary variable name for rhs

    // Finding the symbol table entry for lhs variable
    int indexLHS = sym_tab_entry_contains(nameLHS,local_ST->func_curr->entries);        // Checks if the symbol table contains - if yes we get the index
    sym_tab_entry* temp = NULL;
    temp = local_ST->func_curr->entries[indexLHS];
    while(temp!=NULL){  //Hashing collision
        if(!strcmp(temp->name,nameLHS)){
            break;
        }
        temp = temp->next;
    }
    int offsetLHS = temp->offset;               // Get the memory offset for the lhs variable from the symbol

    int indexRHS = sym_tab_entry_contains(nameRHS,local_ST->func_curr->entries);        // Check if RHS is a expression/ variable/ a constant
    sprintf(buff, "\t\t; Code for getting assignment statement\n");
    strcpy(asmCode, buff);
    sprintf(buff, "\t\tpush_regs                    ; save values\n");
    strcat(asmCode, buff);
    strcat(asmCode, "\t\txor    rax , rax           ; flush out the rax register");

    // If RHS is a constant (immediate value in ASM jargon)
    if(indexRHS==-1){
        // CHECK ------> detect if immediate value is int or real

        if(!strcmp(temp->type.datatype, "integer"))
        {    
            sprintf(buff, "\t\tmov      rax , %s                    ; immediate to register\n",nameRHS);
            strcat(asmCode, buff);
            sprintf(buff, "\t\tmov      [RBP - %d] , rax            ; register to memory\n",offsetLHS);
            strcat(asmCode, buff);
        }

        else
        {
            sprintf(buff, "\t\tmovsd      xmm0 , %s                    ; immediate to register\n",nameRHS);
            strcat(asmCode, buff);
            sprintf(buff, "\t\tmovsd      [RBP - %d] , xmm0            ; register to memory\n",offsetLHS);
            strcat(asmCode, buff);
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

        int offsetRHS = temp->offset;               // Get the memory offset for the rhs variable from the symbol

        if(!strcmp(temp->type.datatype, "integer"))
        {
            sprintf(buff, "\t\tmov      rax , [RBP - %d]                    ; memory to register\n",offsetRHS);
            strcat(asmCode, buff);
            sprintf(buff, "\t\tmov      [RBP - %d] , rax            ; register to memory\n",offsetLHS);
            strcat(asmCode, buff);
        }

        else
        {
            sprintf(buff, "\t\tmovsd      xmm0 , [RBP - %d]                    ; memory to register\n",offsetRHS);
            strcat(asmCode, buff);
            sprintf(buff, "\t\tmovsd     [RBP - %d] , xmm0            ; register to memory\n",offsetLHS);
            strcat(asmCode, buff);
        }
    }
    
    sprintf(buff, "\t\tpop_regs                    ; restore values");
    strcat(asmCode, buff);                                       
    fprintf(assembly, "%s", asmCode);
}

void codegen_logical(ir_code_node* ir, func_entry* local_ST,func_entry** global_ST){
    /* The entry will be of the following form
    +------------+----------+----------+-----------+
    |    label   |    temp  |    op1   |    op2    |
    +------------+----------+----------+-----------+
    label can be one of AND, OR
    */

    char* asmCode = (char*) malloc(sizeof(char)*20);        // asm code attribute
    char* buff1 = (char*) malloc(sizeof(char)*100);
    memset(buff1,'\0',sizeof(buff1));
    char* buff2 = (char*) malloc(sizeof(char)*100);
    memset(buff2,'\0',sizeof(buff2));

    // Get offset of result
    char* result = ir->result;
    int indexResult = sym_tab_entry_contains(result,local_ST->func_curr->entries);        // Checks if the symbol table contains - if yes we get the index
    sym_tab_entry* temp = NULL;
    temp = local_ST->func_curr->entries[indexResult];
    while(temp!=NULL){
        if(!strcmp(temp->name,result)){
            break;
        }
        temp = temp->next;
    }
    int offsetResult = temp->offset;               // Get the memory offset for the lhs variable from the symbol
    char* resultType = temp->type.datatype;

    // Get offset of left operand temp
    char* nameLeft = ir->left_op;
    int indexLeft = sym_tab_entry_contains(nameLeft,local_ST->func_curr->entries);        // Checks if the symbol table contains - if yes we get the index
    sprintf(buff1, "\t\t; Code for logical op\n");
    strcpy(asmCode, buff1);
    sprintf(buff1, "\t\tpush_regs                    ; save values\n");
    strcat(asmCode, buff1);

    // If left operand is a constant
    if(indexLeft == -1)
    {
        // TODO ----> identifying if immediate value is int or real
        if(!strcmp(resultType, "integer"))
        {
            sprintf(buff1, "\t\tmov     rax , %s            ; immediate to memory\n", nameLeft);
            strcat(asmCode, buff1);
        }

        else
        {
            sprintf(buff1, "\t\tmovsd     xmm0 , %s            ; immediate to memory\n", nameLeft);
            strcat(asmCode, buff1);
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
        int offsetLeft = temp->offset;               // Get the memory offset for the lhs variable from the symbol

        if(!strcmp(resultType, "integer"))
        {
            sprintf(buff1, "\t\tmov     rax , [RBP - %d]\n", offsetLeft);
            strcat(asmCode, buff1);
        }
        else
        {
            sprintf(buff1, "\t\tmovsd     xmm0 , [RBP - %d]\n", offsetLeft);
            strcat(asmCode, buff1);
        }
        
    }

    // For right operand
    switch(ir->operator)
    {
        char* nameRight;
        int indexRight;
        case AND:
            // Get offset of right operand temp
            nameRight = ir->right_op;
            indexRight = sym_tab_entry_contains(nameRight,local_ST->func_curr->entries);        // Checks if the symbol table contains - if yes we get the index
            // If right operand is a constant
            if(indexRight == -1)
            {
                // CHECK ----> identifying if immediate value is int or real
                if(!strcmp(resultType, "integer"))
                {
                    sprintf(buff2, "\t\tand     rax , %s\n", nameRight);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tmov     [RBP - %d] , rax\n", offsetResult);
                    strcat(asmCode, buff2);
                }

                else
                {
                    sprintf(buff2, "\t\tandpd     xmm0 , %s\n", nameRight);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tmovsd     [RBP - %d] , xmm0\n", offsetResult);
                    strcat(asmCode, buff2);
                }
            }

            else
            {
                temp = local_ST->func_curr->entries[indexLeft];
                while(temp!=NULL){
                    if(!strcmp(temp->name,nameRight)){
                        break;
                    }
                    temp = temp->next;
                }
                int offsetRight = temp->offset;               // Get the memory offset for the lhs variable from the symbol

                if(!strcmp(resultType, "integer"))
                {
                    sprintf(buff1, "\t\tand     rax , [RBP - %d]\n", offsetRight);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tmov     [RBP - %d] , rax\n", offsetResult);
                    strcat(asmCode, buff2);
                }
                else
                {
                    sprintf(buff1, "\t\tandpd     xmm0 , [RBP - %d]\n", offsetRight);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tmovsd     [RBP - %d] , xmm0\n", offsetResult);
                    strcat(asmCode, buff2);
                }
            }
            break;
        case OR:
            // Get offset of right operand temp
            nameRight = ir->right_op;
            indexRight = sym_tab_entry_contains(nameRight,local_ST->func_curr->entries);        // Checks if the symbol table contains - if yes we get the index
            // If right operand is a constant
            if(indexRight == -1)
            {
                // CHECK ----> identifying if immediate value is int or real
                if(!strcmp(resultType, "integer"))
                {
                    sprintf(buff2, "\t\tor     rax , %s\n", nameRight);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tmov     [RBP - %d] , rax\n", offsetResult);
                    strcat(asmCode, buff2);
                }

                else
                {
                    sprintf(buff2, "\t\torpd     xmm0 , %s\n", nameRight);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tmovsd     [RBP - %d] , xmm0\n", offsetResult);
                    strcat(asmCode, buff2);
                }
            }

            else
            {
                temp = local_ST->func_curr->entries[indexLeft];
                while(temp!=NULL){
                    if(!strcmp(temp->name,nameRight)){
                        break;
                    }
                    temp = temp->next;
                }
                int offsetRight = temp->offset;               // Get the memory offset for the lhs variable from the symbol

                if(!strcmp(resultType, "integer"))
                {
                    sprintf(buff1, "\t\tor     rax , [RBP - %d]\n", offsetRight);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tmov     [RBP - %d] , rax\n", offsetResult);
                    strcat(asmCode, buff2);
                }
                else
                {
                    sprintf(buff1, "\t\torpd     xmm0 , [RBP - %d]\n", offsetRight);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tmovsd     [RBP - %d] , xmm0\n", offsetResult);
                    strcat(asmCode, buff2);
                }
            }
            break;
    }

    sprintf(buff2, "\t\tpop_regs        ; restore register values\n");
    strcat(asmCode, buff2);
    fprintf(assembly, "%s", asmCode);
}

void codegen_input(ir_code_node* ir, func_entry* local_ST,func_entry** global_ST){
    /* The entry will be of the following form
    +------------+----------+----------+-----------+
    |  GET_VALUE |    var   |   NULL   |    NULL   |
    +------------+----------+----------+-----------+
    */
    char* asmCode = (char*) malloc(sizeof(char)*20);        // asm code attribute
    char* buff = (char*) malloc(sizeof(char)*100);
    memset(buff,'\0',sizeof(buff));

    // Get offset of result
    char* result = ir->result;
    int indexResult = sym_tab_entry_contains(result,local_ST->func_curr->entries);        // Checks if the symbol table contains - if yes we get the index
    sym_tab_entry* temp = NULL;
    temp = local_ST->func_curr->entries[indexResult];
    while(temp!=NULL){
        if(!strcmp(temp->name,result)){
            break;
        }
        temp = temp->next;
    }
    int offsetResult = temp->offset;               // Get the memory offset for the lhs variable from the symbol
    char* resultType = temp->type.datatype;

    sprintf(buff, "\t\t; Code for getting user input\n");
    strcpy(asmCode, buff);
    sprintf(buff, "\t\tpush_regs                    ; save values\n");
    strcat(asmCode, buff);

    if(!strcmp(resultType, "integer"))
    {
        sprintf(buff, "\t\tmov      rdi , fmt_spec_int          ; get corresponding format specifier\n");
        strcat(asmCode, buff);
    }
    else if(!strcmp(resultType, "real"))
    {
        sprintf(buff, "\t\tmov      rdi , fmt_spec_real          ; get corresponding format specifier\n");
        strcat(asmCode, buff);
    }
    else if(!strcmp(resultType, "boolean"))
    {
        sprintf(buff, "\t\tmov      rdi , fmt_spec_bool          ; get corresponding format specifier\n");
        strcat(asmCode, buff);
    }

    sprintf(buff, "\t\tmov      rdx , rbp                               ; take base pointer in rdx\n");
    strcat(asmCode, buff);
    sprintf(buff, "\t\tsub      rdx , %d                                ; move pointer to place where we have to store\n", offsetResult);
    strcat(asmCode, buff);
    sprintf(buff, "\t\tmov      rax , 0x0000_0000_ffff_ffff             ; set size\n");
    strcat(asmCode, buff);
    sprintf(buff, "\t\tmov      [rdx] , rax\n");
    strcat(asmCode, buff);
    sprintf(buff, "\t\tmov      rsi , rdx                               ; move source index\n");
    strcat(asmCode, buff);
    sprintf(buff, "\t\tmov      rax , zero\n");
    strcat(asmCode, buff);
    sprintf(buff, "\t\tmov      rsi , rdx\n");
    strcat(asmCode, buff);
    // sprintf(buff, "\t\talign_16_rsp                                     ; align stack pointer\n");
    // strcat(asmCode, buff);
    sprintf(buff, "\t\tcall     scanf                                   ; system call for input\n");
    strcat(asmCode, buff);
    // sprintf(buff, "\t\tdealign_16_rsp                                      ; restore previos alignment of stack\n");
    // strcat(asmCode, buff);

    sprintf(buff, "\t\tpop_regs        ; restore register values\n");
    strcat(asmCode, buff);
    fprintf(assembly, "%s", asmCode);
}

void codegen_output(ir_code_node* ir, func_entry* local_ST,func_entry** global_ST){
    /* The entry will be of the following form
    +------------+----------+----------+-----------+
    |    PRINT   |    var   |   NULL   |    NULL   |
    +------------+----------+----------+-----------+
    */
    char* asmCode = (char*) malloc(sizeof(char)*2000);        // asm code attribute
    char* buff = (char*) malloc(sizeof(char)*1000);
    memset(buff,'\0',sizeof(buff));

    // Get offset of result
    char* result = ir->result;
    int indexResult = sym_tab_entry_contains(result,local_ST->func_curr->entries);   // Checks if the symbol table contains - if yes we get the index
    if(indexResult==-1)
    {
        sprintf(buff, "\t\t; Code for printing output\n");
        strcpy(asmCode, buff);
        sprintf(buff, "\t\tpush_regs                    ; save values\n");
        strcat(asmCode, buff);

        sprintf(buff, "\t\tlea      rdi , [rel fmt_spec_int]                ; get corresponding format specifier\n");
        strcat(asmCode, buff);

        sprintf(buff, "\t\tmov      rsi , %s                               ; move source index\n", result);
        strcat(asmCode, buff);
        sprintf(buff, "\t\txor      rax , rax\n");
        strcat(asmCode, buff);
        // sprintf(buff, "\t\talign_16_rsp                                     ; align stack pointer\n");
        // strcat(asmCode, buff);
        sprintf(buff, "\t\tcall     printf                                   ; system call for output\n");
        strcat(asmCode, buff);
        // sprintf(buff, "\t\tdealign_16_rsp                                      ; restore previos alignment of stack\n");
        // strcat(asmCode, buff);
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
        int offsetResult = temp->offset;               // Get the memory offset for the lhs variable from the symbol
        char* resultType = temp->type.datatype;

        sprintf(buff, "\t\t; Code for printing output\n");
        strcpy(asmCode, buff);
        sprintf(buff, "\t\tpush_regs                    ; save values\n");
        strcat(asmCode, buff);

        if(!strcmp(resultType, "integer"))
        {
            sprintf(buff, "\t\tmov      rdi , fmt_spec_int          ; get corresponding format specifier\n");
            strcat(asmCode, buff);

            sprintf(buff, "\t\tmov      rdx , rbp                               ; take base pointer in rdx\n");
            strcat(asmCode, buff);
            sprintf(buff, "\t\tsub      rdx , %d                                ; move pointer to place from where we have to read\n", offsetResult);
            strcat(asmCode, buff);
            sprintf(buff, "\t\tmov      rsi , [rdx]                               ; move source index\n");
            strcat(asmCode, buff);
            sprintf(buff, "\t\txor      rax , rax\n");
            strcat(asmCode, buff);
            // sprintf(buff, "\t\talign_16_rsp                                     ; align stack pointer\n");
            // strcat(asmCode, buff);
            sprintf(buff, "\t\tcall     printf                                   ; system call for output\n");
            strcat(asmCode, buff);
            // sprintf(buff, "\t\tdealign_16_rsp                                      ; restore previos alignment of stack\n");
            // strcat(asmCode, buff);
        }
        else if(!strcmp(resultType, "real"))
        {
            sprintf(buff, "\t\tmov      rdi , fmt_spec_real          ; get corresponding format specifier\n");
            strcat(asmCode, buff);

            sprintf(buff, "\t\tmov      rdx , rbp                               ; take base pointer in rdx\n");
            strcat(asmCode, buff);
            sprintf(buff, "\t\tsub      rdx , %d                                ; move pointer to place from where we have to read\n", offsetResult);
            strcat(asmCode, buff);
            sprintf(buff, "\t\tmovq      xmm0 , [rdx]                               ; move source index\n");
            strcat(asmCode, buff);
            sprintf(buff, "\t\tmov      rax , 1\n");
            strcat(asmCode, buff);
            // sprintf(buff, "\t\talign_16_rsp                                     ; align stack pointer\n");
            // strcat(asmCode, buff);
            sprintf(buff, "\t\tcall     printf                                   ; system call for output\n");
            strcat(asmCode, buff);
            // sprintf(buff, "\t\tdealign_16_rsp                                      ; restore previos alignment of stack\n");
            // strcat(asmCode, buff);
        }
        else if(!strcmp(resultType, "boolean"))
        {

            sprintf(buff, "\t\tmov      rdi , fmt_spec_bool          ; get corresponding format specifier\n");
            strcat(asmCode, buff);

            char* true_label = newLabel();
            char* next_label = newLabel();

            sprintf(buff, "\t\tmov      rdx , rbp                               ; take base pointer in rdx\n");
            strcat(asmCode, buff);
            sprintf(buff, "\t\tsub      rdx , %d                                ; move pointer to place from where we have to read\n", offsetResult);
            strcat(asmCode, buff);
            sprintf(buff, "\t\tmov      rax , [rdx]                               ; move source index\n");
            strcat(asmCode, buff);
            sprintf(buff, "\t\tcmp      rax , 0\n");
            strcat(asmCode, buff);
            sprintf(buff, "\t\tjnz      %s\n", true_label);
            strcat(asmCode, buff);
            sprintf(buff, "\t\tcmp      rax , 0\n");
            strcat(asmCode, buff);
            sprintf(buff, "\t\tpush_regs                    ; save values\n");
            strcat(asmCode, buff);
            sprintf(buff, "\t\t; to print false\n");
            strcat(asmCode, buff);
            sprintf(buff, "\t\tmov      rax , 1\n");
            strcat(asmCode, buff);
            sprintf(buff, "\t\tmov      rdi , 1\n");
            strcat(asmCode, buff);
            sprintf(buff, "\t\tmov      rsi , false\n");
            strcat(asmCode, buff);
            sprintf(buff, "\t\tmov      rdx , false_len\n");
            strcat(asmCode, buff);
            sprintf(buff, "\t\tsyscall\n");
            strcat(asmCode, buff);
            sprintf(buff, "\t\tpop_regs        ; restore register values\n");
            strcat(asmCode, buff);

            sprintf(buff, "\t\tpush_regs                    ; save values\n");
            strcat(asmCode, buff);
            sprintf(buff, "%s:\n", true_label);
            strcat(asmCode, buff);
            sprintf(buff, "\t\t; to print true\n");
            strcat(asmCode, buff);
            sprintf(buff, "\t\tmov      rax , 1\n");
            strcat(asmCode, buff);
            sprintf(buff, "\t\tmov      rdi , 1\n");
            strcat(asmCode, buff);
            sprintf(buff, "\t\tmov      rsi , true\n");
            strcat(asmCode, buff);
            sprintf(buff, "\t\tmov      rdx , true_len\n");
            strcat(asmCode, buff);
            sprintf(buff, "\t\tsyscall\n");
            strcat(asmCode, buff);
            sprintf(buff, "\t\tpop_regs        ; restore register values\n");
            strcat(asmCode, buff);
            sprintf(buff, "%s:\n", next_label);
            strcat(asmCode, buff);
            // sprintf(buff, "\t\talign_16_rsp                                     ; align stack pointer\n");
            // strcat(asmCode, buff);
            // sprintf(buff, "\t\tcall     printf                                   ; system call for output\n");
            // strcat(asmCode, buff);
            // sprintf(buff, "\t\tdealign_16_rsp                                      ; restore previos alignment of stack\n");
            // strcat(asmCode, buff);
        }
    }
    sprintf(buff, "\t\tpop_regs        ; restore register values\n");
    strcat(asmCode, buff);
    fprintf(assembly, "%s", asmCode);
}

void codegen_arithmetic(ir_code_node* ir, func_entry* local_ST,func_entry** global_ST){
    /* The entry will be of the following form
    +------------+----------+----------+-----------+
    |    label   |    temp  |    op1   |    op2    |
    +------------+----------+----------+-----------+
    label can be one of ADD, SUB, MUL, DIv
    */

    char* asmCode = (char*) malloc(sizeof(char)*20);        // asm code attribute
    char* buff1 = (char*) malloc(sizeof(char)*100);
    memset(buff1,'\0',sizeof(buff1));
    char* buff2 = (char*) malloc(sizeof(char)*100);
    memset(buff2,'\0',sizeof(buff2));

    // Get offset of result
    char* result = ir->result;
    int indexResult = sym_tab_entry_contains(result,local_ST->func_curr->entries);        // Checks if the symbol table contains - if yes we get the index
    sym_tab_entry* temp = NULL;
    temp = local_ST->func_curr->entries[indexResult];
    while(temp!=NULL){
        if(!strcmp(temp->name,result)){
            break;
        }
        temp = temp->next;
    }
    int offsetResult = temp->offset;               // Get the memory offset for the lhs variable from the symbol
    char* resultType = temp->type.datatype;

    // Get offset of left operand temp
    char* nameLeft = ir->left_op;
    int indexLeft = sym_tab_entry_contains(nameLeft,local_ST->func_curr->entries);        // Checks if the symbol table contains - if yes we get the index
    sprintf(buff1, "\t\t; Code for arithmetic\n");
    strcpy(asmCode, buff1);
    sprintf(buff1, "\t\tpush_regs                    ; save values\n");
    strcat(asmCode, buff1);

    // If left operand is a constant
    if(indexLeft == -1)
    {
        // TODO ----> identifying if immediate value is int or real
        if(!strcmp(resultType, "integer"))
        {
            sprintf(buff1, "\t\tmov     rax , %s            ; immediate to memory\n", nameLeft);
            strcat(asmCode, buff1);
        }

        else
        {
            sprintf(buff1, "\t\tmovsd     xmm0 , %s            ; immediate to memory\n", nameLeft);
            strcat(asmCode, buff1);
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
        int offsetLeft = temp->offset;               // Get the memory offset for the lhs variable from the symbol

        if(!strcmp(resultType, "integer"))
        {
            sprintf(buff1, "\t\tmov     rax , [RBP - %d]\n", offsetLeft);
            strcat(asmCode, buff1);
        }
        else
        {
            sprintf(buff1, "\t\tmovsd     xmm0 , [RBP - %d]\n", offsetLeft);
            strcat(asmCode, buff1);
        }
        
    }

    // For right operand
    switch(ir->operator)
    {
        char* nameRight;
        int indexRight;
        case ADD:
            // Get offset of right operand temp
            nameRight = ir->right_op;
            indexRight = sym_tab_entry_contains(nameRight,local_ST->func_curr->entries);        // Checks if the symbol table contains - if yes we get the index
            // If right operand is a constant
            if(indexRight == -1)
            {
                // CHECK ----> identifying if immediate value is int or real
                if(!strcmp(resultType, "integer"))
                {
                    sprintf(buff2, "\t\tadd     rax , %s\n", nameRight);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tmov     [RBP - %d] , rax\n", offsetResult);
                    strcat(asmCode, buff2);
                }

                else
                {
                    sprintf(buff2, "\t\taddsd     xmm0 , %s\n", nameRight);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tmovsd     [RBP - %d] , xmm0\n", offsetResult);
                    strcat(asmCode, buff2);
                }
            }

            else
            {
                temp = local_ST->func_curr->entries[indexLeft];
                while(temp!=NULL){
                    if(!strcmp(temp->name,nameRight)){
                        break;
                    }
                    temp = temp->next;
                }
                int offsetRight = temp->offset;               // Get the memory offset for the lhs variable from the symbol

                if(!strcmp(resultType, "integer"))
                {
                    sprintf(buff1, "\t\tadd     rax , [RBP - %d]\n", offsetRight);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tmov     [RBP - %d] , rax\n", offsetResult);
                    strcat(asmCode, buff2);
                }
                else
                {
                    sprintf(buff1, "\t\taddsd     xmm0 , [RBP - %d]\n", offsetRight);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tmovsd     [RBP - %d] , xmm0\n", offsetResult);
                    strcat(asmCode, buff2);
                }
            }
            break;
        case SUB:
            // Get offset of right operand temp
            nameRight = ir->right_op;
            indexRight = sym_tab_entry_contains(nameRight,local_ST->func_curr->entries);        // Checks if the symbol table contains - if yes we get the index
            // If right operand is a constant
            if(indexRight == -1)
            {
                // CHECK ----> identifying if immediate value is int or real
                if(!strcmp(resultType, "integer"))
                {
                    sprintf(buff2, "\t\tsub     rax , %s\n", nameRight);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tmov     [RBP - %d] , rax\n", offsetResult);
                    strcat(asmCode, buff2);
                }

                else
                {
                    sprintf(buff2, "\t\tsubsd     xmm0 , %s\n", nameRight);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tmovsd     [RBP - %d] , xmm0\n", offsetResult);
                    strcat(asmCode, buff2);
                }
            }

            else
            {
                temp = local_ST->func_curr->entries[indexLeft];
                while(temp!=NULL){
                    if(!strcmp(temp->name,nameRight)){
                        break;
                    }
                    temp = temp->next;
                }
                int offsetRight = temp->offset;               // Get the memory offset for the lhs variable from the symbol

                if(!strcmp(resultType, "integer"))
                {
                    sprintf(buff1, "\t\tsub     rax , [RBP - %d]\n", offsetRight);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tmov     [RBP - %d] , rax\n", offsetResult);
                    strcat(asmCode, buff2);
                }
                else
                {
                    sprintf(buff1, "\t\tsubsd     xmm0 , [RBP - %d]\n", offsetRight);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tmovsd     [RBP - %d] , xmm0\n", offsetResult);
                    strcat(asmCode, buff2);
                }
            }
            break;
        case MUL:
            // Get offset of right operand temp
            nameRight = ir->right_op;
            indexRight = sym_tab_entry_contains(nameRight,local_ST->func_curr->entries);        // Checks if the symbol table contains - if yes we get the index
            // If right operand is a constant
            if(indexRight == -1)
            {
                // CHECK ----> identifying if immediate value is int or real
                if(!strcmp(resultType, "integer"))
                {
                    sprintf(buff2, "\t\tmul     rax , %s\n", nameRight);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tmov     [RBP - %d] , rax\n", offsetResult);
                    strcat(asmCode, buff2);
                }

                else
                {
                    sprintf(buff2, "\t\tmulsd     xmm0 , %s\n", nameRight);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tmovsd     [RBP - %d] , xmm0\n", offsetResult);
                    strcat(asmCode, buff2);
                }
            }

            else
            {
                temp = local_ST->func_curr->entries[indexLeft];
                while(temp!=NULL){
                    if(!strcmp(temp->name,nameRight)){
                        break;
                    }
                    temp = temp->next;
                }
                int offsetRight = temp->offset;               // Get the memory offset for the lhs variable from the symbol

                if(!strcmp(resultType, "integer"))
                {
                    sprintf(buff1, "\t\tmul     rax , [RBP - %d]\n", offsetRight);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tmov     [RBP - %d] , rax\n", offsetResult);
                    strcat(asmCode, buff2);
                }
                else
                {
                    sprintf(buff1, "\t\tmulsd     xmm0 , [RBP - %d]\n", offsetRight);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tmovsd     [RBP - %d] , xmm0\n", offsetResult);
                    strcat(asmCode, buff2);
                }
            }
            break;
        case DIV:
            // Get offset of right operand temp
            nameRight = ir->right_op;
            indexRight = sym_tab_entry_contains(nameRight,local_ST->func_curr->entries);        // Checks if the symbol table contains - if yes we get the index
            // If right operand is a constant
            if(indexRight == -1)
            {
                sprintf(buff2, "\t\tdivsd     xmm0 , %s\n", nameRight);
                strcat(asmCode, buff2);
                sprintf(buff2, "\t\tmovsd     [RBP - %d] , xmm0\n", offsetResult);
                strcat(asmCode, buff2);
            }

            else
            {
                temp = local_ST->func_curr->entries[indexLeft];
                while(temp!=NULL){
                    if(!strcmp(temp->name,nameRight)){
                        break;
                    }
                    temp = temp->next;
                }
                int offsetRight = temp->offset;               // Get the memory offset for the lhs variable from the symbol

                sprintf(buff1, "\t\tdivsd     xmm0 , [RBP - %d]\n", offsetRight);
                strcat(asmCode, buff2);
                sprintf(buff2, "\t\tmovsd     [RBP - %d] , xmm0\n", offsetResult);
                strcat(asmCode, buff2);
            }
            break;
    }

    sprintf(buff2, "\t\tpop_regs        ; restore register values\n");
    strcat(asmCode, buff2);
    fprintf(assembly, "%s", asmCode);
}

void codegen_relational(ir_code_node* ir, func_entry* local_ST,func_entry** global_ST){
    /* The entry will be of the following form
    +------------+----------+----------+-----------+
    |    label   |    temp  |    op1   |    op2    |
    +------------+----------+----------+-----------+
    label can be one of LT, LTE, GT, GTE, EQ, NEQ
    */

    char* asmCode = (char*) malloc(sizeof(char)*20);        // asm code attribute
    char* buff1 = (char*) malloc(sizeof(char)*100);
    memset(buff1,'\0',sizeof(buff1));
    char* buff2 = (char*) malloc(sizeof(char)*100);
    memset(buff2,'\0',sizeof(buff2));

    // Get offset of result
    char* result = ir->result;
    int indexResult = sym_tab_entry_contains(result,local_ST->func_curr->entries);        // Checks if the symbol table contains - if yes we get the index
    sym_tab_entry* temp = NULL;
    temp = local_ST->func_curr->entries[indexResult];
    while(temp!=NULL){
        if(!strcmp(temp->name,result)){
            break;
        }
        temp = temp->next;
    }
    int offsetResult = temp->offset;               // Get the memory offset for the lhs variable from the symbol
    char* resultType = temp->type.datatype;

    // Get offset of left operand temp
    char* nameLeft = ir->left_op;
    int indexLeft = sym_tab_entry_contains(nameLeft,local_ST->func_curr->entries);        // Checks if the symbol table contains - if yes we get the index
    sprintf(buff1, "\t\t; Code for relational\n");
    strcpy(asmCode, buff1);
    sprintf(buff1, "\t\tpush_regs                    ; save values\n");
    strcat(asmCode, buff1);

    // If left operand is a constant
    if(indexLeft == -1)
    {
        // If strchr(nameLeft, '.') returns null then it means left operand has no '.'
        // and hence it is an integer constant (immediate value)
        int type_left_int = (strchr(nameLeft, '.'))? 1 : 0;
        if(type_left_int)
        {
            sprintf(buff1, "\t\tmov     rax , %s            ; immediate to memory\n", nameLeft);
            strcat(asmCode, buff1);
        }

        else
        {
            sprintf(buff1, "\t\tmovsd     xmm0 , %s            ; immediate to memory\n", nameLeft);
            strcat(asmCode, buff1);
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
        int offsetLeft = temp->offset;               // Get the memory offset for the lhs variable from the symbol
        char* leftType = temp->type.datatype;

        if(!strcmp(leftType, "integer"))
        {
            sprintf(buff1, "\t\tmov     rax , [RBP - %d]\n", offsetLeft);
            strcat(asmCode, buff1);
        }
        else
        {
            sprintf(buff1, "\t\tmovsd     xmm0 , [RBP - %d]\n", offsetLeft);
            strcat(asmCode, buff1);
        }
        
    }

    // For right operand
    switch(ir->operator)
    {
        char* nameRight;
        int indexRight;
        char* true_label = newLabel();

        case LT:
            // Get offset of right operand temp
            nameRight = ir->right_op;
            indexRight = sym_tab_entry_contains(nameRight,local_ST->func_curr->entries);        // Checks if the symbol table contains - if yes we get the index
            // If right operand is a constant
            if(indexRight == -1)
            {
                // If strchr(nameRight, '.') returns null then it means right operand has no '.'
                // and hence it is an integer constant (immediate value)
                int type_right_int = (strchr(nameRight, '.'))? 1 : 0;
                if(type_right_int)
                {
                    sprintf(buff2, "\t\tmov     rbx , %s\n", nameRight);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tcmp     rax , rbx\n");
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tjlt     %s\n", true_label);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tmov     [RBP - %d], %d\n", offsetResult, 0);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "%s:\n", true_label);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tmov     [RBP - %d], %d\n", offsetResult, 1);
                    strcat(asmCode, buff2);
                }

                else
                {
                    sprintf(buff2, "\t\tmov     xmm1 , %s\n", nameRight);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tcomiss     xmm0 , xmm1\n");
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tjb     %s\n", true_label);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tmov     [RBP - %d], %d\n", offsetResult, 0);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "%s:\n", true_label);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tmov     [RBP - %d], %d\n", offsetResult, 1);
                    strcat(asmCode, buff2);
                }
            }

            else
            {
                temp = local_ST->func_curr->entries[indexLeft];
                while(temp!=NULL){
                    if(!strcmp(temp->name,nameRight)){
                        break;
                    }
                    temp = temp->next;
                }
                int offsetRight = temp->offset;               // Get the memory offset for the lhs variable from the symbol
                char* rightType = temp->type.datatype;

                if(!strcmp(rightType, "integer"))
                {
                    sprintf(buff2, "\t\tmov     rbx , [RBP - %d]\n", offsetRight);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tcmp     rax , rbx\n");
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tjlt     %s\n", true_label);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tmov     [RBP - %d], %d\n", offsetResult, 0);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "%s:\n", true_label);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tmov     [RBP - %d], %d\n", offsetResult, 1);
                    strcat(asmCode, buff2);
                }

                else
                {
                    sprintf(buff2, "\t\tmov     xmm1 , [RBP - %d]\n", offsetRight);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tcomiss     xmm0 , xmm1\n");
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tjb     %s\n", true_label);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tmov     [RBP - %d], %d\n", offsetResult, 0);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "%s:\n", true_label);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tmov     [RBP - %d], %d\n", offsetResult, 1);
                    strcat(asmCode, buff2);
                }
            }
            break;
        case GT:
            // Get offset of right operand temp
            nameRight = ir->right_op;
            indexRight = sym_tab_entry_contains(nameRight,local_ST->func_curr->entries);        // Checks if the symbol table contains - if yes we get the index
            // If right operand is a constant
            if(indexRight == -1)
            {
                // If strchr(nameRight, '.') returns null then it means right operand has no '.'
                // and hence it is an integer constant (immediate value)
                int type_right_int = (strchr(nameRight, '.'))? 1 : 0;
                if(type_right_int)
                {
                    sprintf(buff2, "\t\tmov     rbx , %s\n", nameRight);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tcmp     rax , rbx\n");
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tjgt     %s\n", true_label);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tmov     [RBP - %d], %d\n", offsetResult, 0);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "%s:\n", true_label);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tmov     [RBP - %d], %d\n", offsetResult, 1);
                    strcat(asmCode, buff2);
                }

                else
                {
                    sprintf(buff2, "\t\tmov     xmm1 , %s\n", nameRight);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tcomiss     xmm0 , xmm1\n");
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tja     %s\n", true_label);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tmov     [RBP - %d], %d\n", offsetResult, 0);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "%s:\n", true_label);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tmov     [RBP - %d], %d\n", offsetResult, 1);
                    strcat(asmCode, buff2);
                }
            }

            else
            {
                temp = local_ST->func_curr->entries[indexLeft];
                while(temp!=NULL){
                    if(!strcmp(temp->name,nameRight)){
                        break;
                    }
                    temp = temp->next;
                }
                int offsetRight = temp->offset;               // Get the memory offset for the lhs variable from the symbol
                char* rightType = temp->type.datatype;

                if(!strcmp(rightType, "integer"))
                {
                    sprintf(buff2, "\t\tmov     rbx , [RBP - %d]\n", offsetRight);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tcmp     rax , rbx\n");
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tjgt     %s\n", true_label);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tmov     [RBP - %d], %d\n", offsetResult, 0);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "%s:\n", true_label);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tmov     [RBP - %d], %d\n", offsetResult, 1);
                    strcat(asmCode, buff2);
                }
                
                else
                {
                    sprintf(buff2, "\t\tmov     xmm1 , [RBP - %d]\n", offsetRight);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tcomiss     xmm0 , xmm1\n");
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tjg     %s\n", true_label);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tmov     [RBP - %d], %d\n", offsetResult, 0);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "%s:\n", true_label);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tmov     [RBP - %d], %d\n", offsetResult, 1);
                    strcat(asmCode, buff2);
                }
            }
            break;
        case LE:
            // Get offset of right operand temp
            nameRight = ir->right_op;
            indexRight = sym_tab_entry_contains(nameRight,local_ST->func_curr->entries);        // Checks if the symbol table contains - if yes we get the index
            // If right operand is a constant
            if(indexRight == -1)
            {
                // If strchr(nameRight, '.') returns null then it means right operand has no '.'
                // and hence it is an integer constant (immediate value)
                int type_right_int = (strchr(nameRight, '.'))? 1 : 0;
                if(type_right_int)
                {
                    sprintf(buff2, "\t\tmov     rbx , %s\n", nameRight);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tcmp     rax , rbx\n");
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tjle     %s\n", true_label);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tmov     [RBP - %d], %d\n", offsetResult, 0);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "%s:\n", true_label);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tmov     [RBP - %d], %d\n", offsetResult, 1);
                    strcat(asmCode, buff2);
                }

                else
                {
                    sprintf(buff2, "\t\tmov     xmm1 , %s\n", nameRight);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tcomiss     xmm0 , xmm1\n");
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tjb     %s\n", true_label);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tjz     %s\n", true_label);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tmov     [RBP - %d], %d\n", offsetResult, 0);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "%s:\n", true_label);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tmov     [RBP - %d], %d\n", offsetResult, 1);
                    strcat(asmCode, buff2);
                }
            }

            else
            {
                temp = local_ST->func_curr->entries[indexLeft];
                while(temp!=NULL){
                    if(!strcmp(temp->name,nameRight)){
                        break;
                    }
                    temp = temp->next;
                }
                int offsetRight = temp->offset;               // Get the memory offset for the lhs variable from the symbol
                char* rightType = temp->type.datatype;

                if(!strcmp(rightType, "integer"))
                {
                    sprintf(buff2, "\t\tmov     rbx , [RBP - %d]\n", offsetRight);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tcmp     rax , rbx\n");
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tjle     %s\n", true_label);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tmov     [RBP - %d], %d\n", offsetResult, 0);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "%s:\n", true_label);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tmov     [RBP - %d], %d\n", offsetResult, 1);
                    strcat(asmCode, buff2);
                }
                
                else
                {
                    sprintf(buff2, "\t\tmov     xmm1 , [RBP - %d]\n", offsetRight);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tcomiss     xmm0 , xmm1\n");
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tjb     %s\n", true_label);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tjz     %s\n", true_label);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tmov     [RBP - %d], %d\n", offsetResult, 0);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "%s:\n", true_label);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tmov     [RBP - %d], %d\n", offsetResult, 1);
                    strcat(asmCode, buff2);
                }
            }
            break;
        case GE:
            // Get offset of right operand temp
            nameRight = ir->right_op;
            indexRight = sym_tab_entry_contains(nameRight,local_ST->func_curr->entries);        // Checks if the symbol table contains - if yes we get the index
            // If right operand is a constant
            if(indexRight == -1)
            {
                // If strchr(nameRight, '.') returns null then it means right operand has no '.'
                // and hence it is an integer constant (immediate value)
                int type_right_int = (strchr(nameRight, '.'))? 1 : 0;
                if(type_right_int)
                {
                    sprintf(buff2, "\t\tmov     rbx , %s\n", nameRight);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tcmp     rax , rbx\n");
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tjge     %s\n", true_label);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tmov     [RBP - %d], %d\n", offsetResult, 0);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "%s:\n", true_label);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tmov     [RBP - %d], %d\n", offsetResult, 1);
                    strcat(asmCode, buff2);
                }

                else
                {
                    sprintf(buff2, "\t\tmov     xmm1 , %s\n", nameRight);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tcomiss     xmm0 , xmm1\n");
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tja     %s\n", true_label);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tjz     %s\n", true_label);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tmov     [RBP - %d], %d\n", offsetResult, 0);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "%s:\n", true_label);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tmov     [RBP - %d], %d\n", offsetResult, 1);
                    strcat(asmCode, buff2);
                }
            }

            else
            {
                temp = local_ST->func_curr->entries[indexLeft];
                while(temp!=NULL){
                    if(!strcmp(temp->name,nameRight)){
                        break;
                    }
                    temp = temp->next;
                }
                int offsetRight = temp->offset;               // Get the memory offset for the lhs variable from the symbol
                char* rightType = temp->type.datatype;

                if(!strcmp(rightType, "integer"))
                {
                    sprintf(buff2, "\t\tmov     rbx , [RBP - %d]\n", offsetRight);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tcmp     rax , rbx\n");
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tjge     %s\n", true_label);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tmov     [RBP - %d], %d\n", offsetResult, 0);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "%s:\n", true_label);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tmov     [RBP - %d], %d\n", offsetResult, 1);
                    strcat(asmCode, buff2);
                }
                
                else
                {
                    sprintf(buff2, "\t\tmov     xmm1 , [RBP - %d]\n", offsetRight);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tcomiss     xmm0 , xmm1\n");
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tja     %s\n", true_label);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tjz     %s\n", true_label);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tmov     [RBP - %d], %d\n", offsetResult, 0);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "%s:\n", true_label);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tmov     [RBP - %d], %d\n", offsetResult, 1);
                    strcat(asmCode, buff2);
                }
            }
            break;
        case EQ:
            // Get offset of right operand temp
            nameRight = ir->right_op;
            indexRight = sym_tab_entry_contains(nameRight,local_ST->func_curr->entries);        // Checks if the symbol table contains - if yes we get the index
            // If right operand is a constant
            if(indexRight == -1)
            {
                // If strchr(nameRight, '.') returns null then it means right operand has no '.'
                // and hence it is an integer constant (immediate value)
                int type_right_int = (strchr(nameRight, '.'))? 1 : 0;
                if(type_right_int)
                {
                    sprintf(buff2, "\t\tmov     rbx , %s\n", nameRight);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tcmp     rax , rbx\n");
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tjz     %s\n", true_label);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tmov     [RBP - %d], %d\n", offsetResult, 0);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "%s:\n", true_label);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tmov     [RBP - %d], %d\n", offsetResult, 1);
                    strcat(asmCode, buff2);
                }

                else
                {
                    sprintf(buff2, "\t\tmov     xmm1 , %s\n", nameRight);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tcomiss     xmm0 , xmm1\n");
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tjz     %s\n", true_label);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tmov     [RBP - %d], %d\n", offsetResult, 0);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "%s:\n", true_label);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tmov     [RBP - %d], %d\n", offsetResult, 1);
                    strcat(asmCode, buff2);
                }
            }

            else
            {
                temp = local_ST->func_curr->entries[indexLeft];
                while(temp!=NULL){
                    if(!strcmp(temp->name,nameRight)){
                        break;
                    }
                    temp = temp->next;
                }
                int offsetRight = temp->offset;               // Get the memory offset for the lhs variable from the symbol
                char* rightType = temp->type.datatype;

                if(!strcmp(rightType, "integer"))
                {
                    sprintf(buff2, "\t\tmov     rbx , [RBP - %d]\n", offsetRight);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tcmp     rax , rbx\n");
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tjz     %s\n", true_label);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tmov     [RBP - %d], %d\n", offsetResult, 0);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "%s:\n", true_label);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tmov     [RBP - %d], %d\n", offsetResult, 1);
                    strcat(asmCode, buff2);
                }
                
                else
                {
                    sprintf(buff2, "\t\tmov     xmm1 , [RBP - %d]\n", offsetRight);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tcomiss     xmm0 , xmm1\n");
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tjz     %s\n", true_label);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tmov     [RBP - %d], %d\n", offsetResult, 0);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "%s:\n", true_label);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tmov     [RBP - %d], %d\n", offsetResult, 1);
                    strcat(asmCode, buff2);
                }
            }
            break;
        case NEQ:
            // Get offset of right operand temp
            nameRight = ir->right_op;
            indexRight = sym_tab_entry_contains(nameRight,local_ST->func_curr->entries);        // Checks if the symbol table contains - if yes we get the index
            // If right operand is a constant
            if(indexRight == -1)
            {
                // If strchr(nameRight, '.') returns null then it means right operand has no '.'
                // and hence it is an integer constant (immediate value)
                int type_right_int = (strchr(nameRight, '.'))? 1 : 0;
                if(type_right_int)
                {
                    sprintf(buff2, "\t\tmov     rbx , %s\n", nameRight);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tcmp     rax , rbx\n");
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tjnz     %s\n", true_label);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tmov     [RBP - %d], %d\n", offsetResult, 0);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "%s:\n", true_label);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tmov     [RBP - %d], %d\n", offsetResult, 1);
                    strcat(asmCode, buff2);
                }

                else
                {
                    sprintf(buff2, "\t\tmov     xmm1 , %s\n", nameRight);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tcomiss     xmm0 , xmm1\n");
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tjnz     %s\n", true_label);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tmov     [RBP - %d], %d\n", offsetResult, 0);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "%s:\n", true_label);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tmov     [RBP - %d], %d\n", offsetResult, 1);
                    strcat(asmCode, buff2);
                }
            }

            else
            {
                temp = local_ST->func_curr->entries[indexLeft];
                while(temp!=NULL){
                    if(!strcmp(temp->name,nameRight)){
                        break;
                    }
                    temp = temp->next;
                }
                int offsetRight = temp->offset;               // Get the memory offset for the lhs variable from the symbol
                char* rightType = temp->type.datatype;

                if(!strcmp(rightType, "integer"))
                {
                    sprintf(buff2, "\t\tmov     rbx , [RBP - %d]\n", offsetRight);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tcmp     rax , rbx\n");
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tjnz     %s\n", true_label);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tmov     [RBP - %d], %d\n", offsetResult, 0);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "%s:\n", true_label);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tmov     [RBP - %d], %d\n", offsetResult, 1);
                    strcat(asmCode, buff2);
                }

                else
                {
                    sprintf(buff2, "\t\tmov     xmm1 , [RBP - %d]\n", offsetRight);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tcomiss     xmm0 , xmm1\n");
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tjnz     %s\n", true_label);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tmov     [RBP - %d], %d\n", offsetResult, 0);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "%s:\n", true_label);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "\t\tmov     [RBP - %d], %d\n", offsetResult, 1);
                    strcat(asmCode, buff2);
                }
            }
            break;
        }

    sprintf(buff2, "\t\tpop_regs        ; restore register values\n");
    strcat(asmCode, buff2);

                                          
    fprintf(assembly, "%s", asmCode);
}

void codegen_switch(ir_code_node* ir, func_entry* local_ST,func_entry** global_ST)
{
    char* asmCode = (char*)malloc(sizeof(char)*20);
    char* buff = (char*)malloc(sizeof(char)*100);

    fprintf(assembly ,"%s", asmCode);
}

void codegen_iterative(ir_code_node* ir, func_entry* local_ST,func_entry** global_ST)
{
    char* asmCode = (char*)malloc(sizeof(char)*20);
    char* buff = (char*)malloc(sizeof(char)*100);

    fprintf(assembly ,"%s", asmCode);
}

void codegen_procedure(ir_code_node* ir, func_entry* local_ST,func_entry** global_ST)
{
    /* The entry will be of the following form
    +------------+----------+----------+-----------+
    |    label   |  fn_name |    NULL  |    NULL   |
    +------------+----------+----------+-----------+
    */
    char* asmCode = (char*)malloc(sizeof(char)*20);
    char* buff = (char*)malloc(sizeof(char)*100);

    char* funcName = ir->result;

    sprintf(buff, "_%s:\n",funcName);
    strcpy(asmCode, buff);
    

    fprintf(assembly ,"%s", asmCode);
}

// Required functions

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
        fprintf(assembly, "\t\tfmt_spec_int: db \"%%d\", 10, 0\n");
        fprintf(assembly, "\t\tfmt_spec_real: db \"%%4f\", 10, 0\n");
        fprintf(assembly, "\t\tfmt_spec_string: db \"%%s\", 10, 0\n");
        fprintf(assembly, "\t\tfmt_spec_bool: db \"%%d\", 10, 0\n");
        fprintf(assembly, "\t\ttrue: db \"true\", 10, 0\n");
        fprintf(assembly, "\t\ttrue_len: equ $ - true\n");
        fprintf(assembly, "\t\tfalse: db \"false\", 10, 0\n");
        fprintf(assembly, "\t\tfalse_len: equ $ - false\n");
        fprintf(assembly, "\t\tzero: equ 0\n");

        /* Data declaration of various types to be done by going thro each entry of the symbol table*/

        fprintf(assembly, "\n\n\t\tsection      .text\n");
        macros_starter();               // Define the macros
        fprintf(assembly, "\t\tglobal main\n");
        fprintf(assembly, "main:\n");

        printf("[+] ASM file updated!\n");

    }

    ir_code_node *IR_head = IR->head;
    var_record* func_curr;
    func_entry* function;

    // Go through each entry of the IR quadruple
    while(IR_head)
    {
        if(IR_head->operator==FUNC){ //if you reach FUNC --> then change the context
            
            //in case it is main --we have to search for DRIVER
            if (!strcmp(IR_head->result,"main")) function = find_module_global("DRIVER");
            else function = find_module_global(IR_head->result);
            func_curr = function->func_root; //once we get the function then just locate the root
        }


        char* reach = IR_head->reach; 

        if(strcmp(reach,"")){ //if reach is not "" then find the local construct
                              //in the current function
            func_curr = find_local_construct(function->name,reach);
        }else {
            func_curr = function->func_root;
        }

        function->func_curr = func_curr;
        
        // switch (IR_head->operator)
        // {
        // case ASSIGN:
        //     codegen_assgn_stmt(IR_head, /* symbol table param*/);
        //     break;

        // case GET_VALUE:
        //     codegen_input(IR_head,);
        //     break;
        
        // case PRINT:
        //     codegen_output(IR_head, /* symbol table param*/);
        //     break;

        // case ADD:
        // case SUB:
        // case MUL:
        //     codegen_arithmetic_nodiv(IR_head, /* symbol table param*/);
        //     break;
        // case DIV:
        //     codegen_div(IR_head, /* symbol table param*/);
        //     break;
        
        // case LT:
        // case LE:
        // case GT:
        // case GE:
        // case EQ:
        // case NEQ:
        //     codegen_relational(IR_head, /* symbol table param*/);
        //     break;
        
        // case OR:
        // case AND:
        //     codegen_boolean(IR_head, /* symbol table param*/);
        //     break;
        
        // case FUNC:
        // case CALL:
        // case PARA_IN:
        // case PARA_OUT:
        // case RET:
        //     codegen_func(IR_head, /* symbol table param*/);
        
        // case GOTO:
        //     codegen_jump(IR_head, /* symbol table param*/);
        //     break;
        
        // case IF:
        //     codegen_conditional(IR_head, /* symbol table param*/);
        //     break;



        // // case LABEL:
        // //     codegen_label(IR_head, /* symbol table param*/);
        // //     break;
        // }

        IR_head = IR_head->next;
    }

    fclose(assembly);
    
}
