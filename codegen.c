#include <string.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "IR_codegen.h"


char* codegen_logical(ast_node* node,ir_code_node* ir, func_entry* local_ST,func_entry** global_ST){
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
    sprintf(buff1, "; Code for logical op\n");
    strcpy(asmCode, buff1);
    sprintf(buff1, "push_regs                    ; save values\n");
    strcat(asmCode, buff1);

    // If left operand is a constant
    if(indexLeft == -1)
    {
        // TODO ----> identifying if immediate value is int or real
        if(!strcmp(resultType, "integer"))
        {
            sprintf(buff1, "mov     rax , %s            ; immediate to memory\n", nameLeft);
            strcat(asmCode, buff1);
        }

        else
        {
            sprintf(buff1, "movsd     xmm0 , %s            ; immediate to memory\n", nameLeft);
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
            sprintf(buff1, "mov     rax , [RBP - %d]\n", offsetLeft);
            strcat(asmCode, buff1);
        }
        else
        {
            sprintf(buff1, "movsd     xmm0 , [RBP - %d]\n", offsetLeft);
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
                    sprintf(buff2, "and     rax , %s\n", nameRight);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "mov     [RBP - %d] , rax\n", offsetResult);
                    strcat(asmCode, buff2);
                }

                else
                {
                    sprintf(buff2, "andpd     xmm0 , %s\n", nameRight);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "movsd     [RBP - %d] , xmm0\n", offsetResult);
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
                    sprintf(buff1, "and     rax , [RBP - %d]\n", offsetRight);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "mov     [RBP - %d] , rax\n", offsetResult);
                    strcat(asmCode, buff2);
                }
                else
                {
                    sprintf(buff1, "andpd     xmm0 , [RBP - %d]\n", offsetRight);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "movsd     [RBP - %d] , xmm0\n", offsetResult);
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
                    sprintf(buff2, "or     rax , %s\n", nameRight);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "mov     [RBP - %d] , rax\n", offsetResult);
                    strcat(asmCode, buff2);
                }

                else
                {
                    sprintf(buff2, "orpd     xmm0 , %s\n", nameRight);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "movsd     [RBP - %d] , xmm0\n", offsetResult);
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
                    sprintf(buff1, "or     rax , [RBP - %d]\n", offsetRight);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "mov     [RBP - %d] , rax\n", offsetResult);
                    strcat(asmCode, buff2);
                }
                else
                {
                    sprintf(buff1, "orpd     xmm0 , [RBP - %d]\n", offsetRight);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "movsd     [RBP - %d] , xmm0\n", offsetResult);
                    strcat(asmCode, buff2);
                }
            }
            break;
    }

    sprintf(buff2, "pop_regs        ; restore register values\n");
    strcat(asmCode, buff2);
    return asmCode;
}

char* codegen_input(ast_node* node,ir_code_node* ir, func_entry* local_ST,func_entry** global_ST){
    /* The entry will be of the following form
    +------------+----------+----------+-----------+
    |  GET_VALUE |    var   |   NULL   |    NULL   |
    +------------+----------+----------+-----------+
    */

    // mov  rax, 3 ; sys_read
    // mov  rbx, 0 ; stdin
    // mov  rcx, user_input ; user input
    // mov  rdx, user_input_length ; max length
    // int  80h

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

    sprintf(buff, "; Code for getting user input\n");
    strcpy(asmCode, buff);
    sprintf(buff, "push_regs                    ; save values\n");
    strcat(asmCode, buff);

    if(!strcmp(resultType, "integer"))
    {
        sprintf(buff, "mov      rdi , fmt_spec_int          ; get corresponding format specifier\n");
        strcat(asmCode, buff);
    }
    else if(!strcmp(resultType, "real"))
    {
        sprintf(buff, "mov      rdi , fmt_spec_real          ; get corresponding format specifier\n");
        strcat(asmCode, buff);
    }
    else if(!strcmp(resultType, "boolean"))
    {
        sprintf(buff, "mov      rdi , fmt_spec_bool          ; get corresponding format specifier\n");
        strcat(asmCode, buff);
    }
    // TODO ----> for arrays

    sprintf(buff, "mov      rdx , rbp                               ; take base pointer in rdx\n");
    strcat(asmCode, buff);
    sprintf(buff, "sub      rdx , %d                                ; move pointer to place where we have to store\n", offsetResult);
    strcat(asmCode, buff);
    sprintf(buff, "mov      rax , 0x0000_0000_ffff_ffff             ; set size\n");
    strcat(asmCode, buff);
    sprintf(buff, "mov      [rdx] , rax\n");
    strcat(asmCode, buff);
    sprintf(buff, "mov      rsi , rdx                               ; move source index\n");
    strcat(asmCode, buff);
    sprintf(buff, "mov      rax , zero\n");
    strcat(asmCode, buff);
    sprintf(buff, "mov      rsi , rdx\n");
    strcat(asmCode, buff);
    // sprintf(buff, "align_16_rsp                                     ; align stack pointer\n");
    // strcat(asmCode, buff);
    sprintf(buff, "call     scanf                                   ; system call for input\n");
    strcat(asmCode, buff);
    // sprintf(buff, "dealign_16_rsp                                      ; restore previos alignment of stack\n");
    // strcat(asmCode, buff);

    sprintf(buff, "pop_regs        ; restore register values\n");
    strcat(asmCode, buff);
    return asmCode;
}

// char* codegen_assgn_stmt(ast_node* node,ir_code_node* ir, func_entry* local_ST,func_entry** global_ST){
//     // :=  lhs  rhs  -
//     char* asmCode = (char*) malloc(sizeof(char)*20);
//     char* nameRHS = node->child_pointers[1]->tempName;
//     char* buff = (char*) malloc(sizeof(char)*100);
//     memset(buff,'\0',sizeof(buff));
//     char* nameLHS = node->child_pointers[0]->tempName;
//     int indexLHS = sym_tab_entry_contains(nameLHS,local_ST->func_curr->entries);
//     sym_tab_entry* temp = NULL;
//     temp = local_ST->func_curr->entries[indexLHS];
//     while(temp!=NULL){
//         if(!strcmp(temp->name,nameLHS)){
//             break;
//         }
//         temp = temp->next;
//     }
//     int offsetLHS = temp->offset;

//     int indexRHS = sym_tab_entry_contains(nameRHS,local_ST->func_curr->entries);

//     strcpy(asmCode, "xor    eax , eax           ; flush out the eax register");
//     if(indexRHS==-1){
//         sprintf(buff, "mov [%d] , %s            ; immediate to memory\n",offsetLHS,nameRHS);
//         return buff;

//     }
//     temp = local_ST->func_curr->entries[indexRHS];
//     while(temp!=NULL){
//         if(!strcmp(temp->name,nameRHS)){
//             break;
//         }
//         temp = temp->next;
//     }

//     int offsetRHS = temp->offset;


//     // sprintf(lhs_name,"%d",offsetLHS);
//     // stcat();
//     sprintf(asmCode,"mov eax , [%d]\n",offsetRHS);
//     // char buff[100];
//     sprintf(buff, "mov [%d] , eax\n",offsetLHS);
//     strcat(asmCode, buff);
//     return asmCode;
// }

/*****************************************ASM CODE FNS START*****************************************/
char* codegen_assgn_stmt(ast_node* node,ir_code_node* ir, func_entry* local_ST,func_entry** global_ST){
    /* The entry will be of the following form
    +------------+----------+----------+-----------+
    |   ASSIGN   |    lhs   |    rhs   |    NULL   |
    +------------+----------+----------+-----------+
    */

    char* asmCode = (char*) malloc(sizeof(char)*20);        // asm code attribute
    char* nameRHS = node->child_pointers[1]->tempName;      // temporary variable name for rhs

    char* buff = (char*) malloc(sizeof(char)*100);
    memset(buff,'\0',sizeof(buff));

    char* nameLHS = node->child_pointers[0]->tempName;      // temporary variable name for rhs

    // Finding the symbol table entry for lhs variable
    int indexLHS = sym_tab_entry_contains(nameLHS,local_ST->func_curr->entries);        // Checks if the symbol table contains - if yes we get the index
    sym_tab_entry* temp = NULL;
    temp = local_ST->func_curr->entries[indexLHS];
    while(temp!=NULL){
        if(!strcmp(temp->name,nameLHS)){
            break;
        }
        temp = temp->next;
    }
    int offsetLHS = temp->offset;               // Get the memory offset for the lhs variable from the symbol

    int indexRHS = sym_tab_entry_contains(nameRHS,local_ST->func_curr->entries);        // Check if RHS is a expression/ variable/ a constant
    sprintf(buff, "; Code for getting assignment statement\n");
    strcpy(asmCode, buff);
    sprintf(buff, "push_regs                    ; save values\n");
    strcat(asmCode, buff);
    strcat(asmCode, "xor    rax , rax           ; flush out the rax register");

    // If RHS is a constant (immediate value in ASM jargon)
    if(indexRHS==-1){
        // CHECK ------> detect if immediate value is int or real

        if(!strcmp(temp->type.datatype, "integer"))
        {    
            sprintf(buff, "mov      rax , %s                    ; immediate to register\n",nameRHS);
            strcat(asmCode, buff);
            sprintf(buff, "mov      [RBP - %d] , rax            ; register to memory\n",offsetLHS);
            strcat(asmCode, buff);
        }

        else
        {
            sprintf(buff, "movsd      xmm0 , %s                    ; immediate to register\n",nameRHS);
            strcat(asmCode, buff);
            sprintf(buff, "movsd      [RBP - %d] , xmm0            ; register to memory\n",offsetLHS);
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
            sprintf(buff, "mov      rax , [RBP - %d]                    ; memory to register\n",offsetRHS);
            strcat(asmCode, buff);
            sprintf(buff, "mov      [RBP - %d] , rax            ; register to memory\n",offsetLHS);
            strcat(asmCode, buff);
        }

        else
        {
            sprintf(buff, "movsd      xmm0 , [RBP - %d]                    ; memory to register\n",offsetRHS);
            strcat(asmCode, buff);
            sprintf(buff, "movsd     [RBP - %d] , xmm0            ; register to memory\n",offsetLHS);
            strcat(asmCode, buff);
        }
    }
    
    sprintf(buff, "pop_regs                    ; restore values");
    strcat(asmCode, buff);                                       
    return asmCode;
}

char* codegen_arithmetic(ast_node* node,ir_code_node* ir, func_entry* local_ST,func_entry** global_ST){
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
    sprintf(buff1, "; Code for arithmetic\n");
    strcpy(asmCode, buff1);
    sprintf(buff1, "push_regs                    ; save values\n");
    strcat(asmCode, buff1);

    // If left operand is a constant
    if(indexLeft == -1)
    {
        // TODO ----> identifying if immediate value is int or real
        if(!strcmp(resultType, "integer"))
        {
            sprintf(buff1, "mov     rax , %s            ; immediate to memory\n", nameLeft);
            strcat(asmCode, buff1);
        }

        else
        {
            sprintf(buff1, "movsd     xmm0 , %s            ; immediate to memory\n", nameLeft);
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
            sprintf(buff1, "mov     rax , [RBP - %d]\n", offsetLeft);
            strcat(asmCode, buff1);
        }
        else
        {
            sprintf(buff1, "movsd     xmm0 , [RBP - %d]\n", offsetLeft);
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
                    sprintf(buff2, "add     rax , %s\n", nameRight);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "mov     [RBP - %d] , rax\n", offsetResult);
                    strcat(asmCode, buff2);
                }

                else
                {
                    sprintf(buff2, "addsd     xmm0 , %s\n", nameRight);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "movsd     [RBP - %d] , xmm0\n", offsetResult);
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
                    sprintf(buff1, "add     rax , [RBP - %d]\n", offsetRight);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "mov     [RBP - %d] , rax\n", offsetResult);
                    strcat(asmCode, buff2);
                }
                else
                {
                    sprintf(buff1, "addsd     xmm0 , [RBP - %d]\n", offsetRight);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "movsd     [RBP - %d] , xmm0\n", offsetResult);
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
                    sprintf(buff2, "sub     rax , %s\n", nameRight);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "mov     [RBP - %d] , rax\n", offsetResult);
                    strcat(asmCode, buff2);
                }

                else
                {
                    sprintf(buff2, "subsd     xmm0 , %s\n", nameRight);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "movsd     [RBP - %d] , xmm0\n", offsetResult);
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
                    sprintf(buff1, "sub     rax , [RBP - %d]\n", offsetRight);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "mov     [RBP - %d] , rax\n", offsetResult);
                    strcat(asmCode, buff2);
                }
                else
                {
                    sprintf(buff1, "subsd     xmm0 , [RBP - %d]\n", offsetRight);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "movsd     [RBP - %d] , xmm0\n", offsetResult);
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
                    sprintf(buff2, "mul     rax , %s\n", nameRight);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "mov     [RBP - %d] , rax\n", offsetResult);
                    strcat(asmCode, buff2);
                }

                else
                {
                    sprintf(buff2, "mulsd     xmm0 , %s\n", nameRight);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "movsd     [RBP - %d] , xmm0\n", offsetResult);
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
                    sprintf(buff1, "mul     rax , [RBP - %d]\n", offsetRight);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "mov     [RBP - %d] , rax\n", offsetResult);
                    strcat(asmCode, buff2);
                }
                else
                {
                    sprintf(buff1, "mulsd     xmm0 , [RBP - %d]\n", offsetRight);
                    strcat(asmCode, buff2);
                    sprintf(buff2, "movsd     [RBP - %d] , xmm0\n", offsetResult);
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
                sprintf(buff2, "divsd     xmm0 , %s\n", nameRight);
                strcat(asmCode, buff2);
                sprintf(buff2, "movsd     [RBP - %d] , xmm0\n", offsetResult);
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

                sprintf(buff1, "divsd     xmm0 , [RBP - %d]\n", offsetRight);
                strcat(asmCode, buff2);
                sprintf(buff2, "movsd     [RBP - %d] , xmm0\n", offsetResult);
                strcat(asmCode, buff2);
            }
            break;
    }

    sprintf(buff2, "pop_regs        ; restore register values\n");
    strcat(asmCode, buff2);
    return asmCode;
}

char* codegen_relational(ast_node* node,ir_code_node* ir, func_entry* local_ST,func_entry** global_ST){
    /* The entry will be of the following form
    +------------+----------+----------+-----------+
    |    label   |    temp  |    op1   |    op2    |
    +------------+----------+----------+-----------+
    label can be one of LT, LTE, GT, GTE, EQ, NEQ
    */

    char* asmCode = (char*) malloc(sizeof(char)*20);        // asm code attribute
                                          
    return asmCode;
}

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

FILE* assembly;

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


void main2(/*ir_code* IR   symbol table, FILE* f*/)
{
    assembly = fopen("assembly_try.asm", "w");

    if(!assembly)
        printf("[-] Error opening assembly_try.asm!\n");
    
    else
    {
        printf("[+] assembly_try.asm opened!\n");

        // Write the external functions and start data section for assembly
        fprintf(assembly, "extern printf, scanf, exit\n");
        fprintf(assembly, "\t\t; Data declaration such as zero and format specifiers for print/scan\n");
        fprintf(assembly, "\t\tsection      .data\n");

        // Write down all the format specifiers reqd
        fprintf(assembly, "\t\tfmt_spec_int: db \"%%d\", 10, 0\n");
        fprintf(assembly, "\t\tfmt_spec_real: db \"%%4f\", 10, 0\n");
        fprintf(assembly, "\t\tfmt_spec_string: db \"%%s\", 10, 0\n");
        fprintf(assembly, "\t\tfmt_spec_bool: db \"%%d\", 10, 0\n");
        fprintf(assembly, "\t\tzero: equ 0\n");

        /* Data declaration of various types to be done by going thro each entry of the symbol table*/

        fprintf(assembly, "\n\n\t\tsection      .text\n");
        fprintf(assembly, "\t\tglobal main\n");
        fprintf(assembly, "main:\n");

        printf("[+] ASM file updated!\n");
        fclose(assembly);
    }    
}

    // // Write the external functions and start data section for assembly
    // fprintf(assembly, "extern printf, scanf, exit\n");
    // fprintf(assembly, "section      .data\n");

    // // Write down all the format specifiers reqd
    // fprintf(assembly, "\t\tfmt_spec_int: db \"%%d\", 10, 0\n");
    // fprintf(assembly, "\t\tfmt_spec_real: db \"%%4f\", 10, 0\n");
    // fprintf(assembly, "\t\tfmt_spec_string: db \"%%s\", 10, 0\n");
    // fprintf(assembly, "\t\tzero: dw 0\n");

    // /* Data declaration of various types to be done by going thro each entry of the symbol table*/

    // fprintf(assembly, "section      .text\n");
    // fprintf(assembly, "global main");

    // printf("[+] ASM file updated!\n");

    // ir_code_node *IR_head = IR->head;

    // // Go through each entry of the IR quadruple
    // while(IR_head)
    // {
    //     switch (IR_head.operator)
    //     {
    //     case ASSIGN:
    //         // codegen_assgn_stmt(IR_head, /* symbol table param*/);
    //         break;
        
    //     case UNARY_PLUS:
    //     case UNARY_MINUS:
    //         codegen_unary_op(IR_head, /* symbol table param*/);
    //         break;

    //     case GET_VALUE:
    //         codegen_input(IR_head, /* symbol table param*/);
    //         break;
        
    //     case PRINT:
    //         codegen_output(IR_head, /* symbol table param*/);
    //         break;

    //     case ADD:
    //     case SUB:
    //     case MUL:
    //         codegen_arithmetic_nodiv(IR_head, /* symbol table param*/);
    //         break;
    //     case DIV:
    //         codegen_div(IR_head, /* symbol table param*/);
    //         break;
        
    //     case LT:
    //     case LE:
    //     case GT:
    //     case GE:
    //     case EQ:
    //     case NEQ:
    //         codegen_relational(IR_head, /* symbol table param*/);
    //         break;
        
    //     case OR:
    //     case AND:
    //         codegen_boolean(IR_head, /* symbol table param*/);
    //         break;
        
    //     case FUNC:
    //     case CALL:
    //     case PARA_IN:
    //     case PARA_OUT:
    //     case RET:
    //         codegen_func(IR_head, /* symbol table param*/);
        
    //     case GOTO:
    //         codegen_jump(IR_head, /* symbol table param*/);
    //         break;
        
    //     case IF:
    //         codegen_conditional(IR_head, /* symbol table param*/);
    //         break;



    //     // case LABEL:
    //     //     codegen_label(IR_head, /* symbol table param*/);
    //     //     break;
    //     }

    //     IR_head = IR_head->next;
    // }

// }
