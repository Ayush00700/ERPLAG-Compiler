#include <string.h>
#include "IR_codegen.h"

static int currentLabel = 1;
static int currentTempVar = 1;

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
        case ADD:
            // Get offset of right operand temp
            char* nameRight = ir->right_op;
            int indexRight = sym_tab_entry_contains(nameRight,local_ST->func_curr->entries);        // Checks if the symbol table contains - if yes we get the index
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
            char* nameRight = ir->right_op;
            int indexRight = sym_tab_entry_contains(nameRight,local_ST->func_curr->entries);        // Checks if the symbol table contains - if yes we get the index
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
            char* nameRight = ir->right_op;
            int indexRight = sym_tab_entry_contains(nameRight,local_ST->func_curr->entries);        // Checks if the symbol table contains - if yes we get the index
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
            char* nameRight = ir->right_op;
            int indexRight = sym_tab_entry_contains(nameRight,local_ST->func_curr->entries);        // Checks if the symbol table contains - if yes we get the index
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
        case AND:
            // Get offset of right operand temp
            char* nameRight = ir->right_op;
            int indexRight = sym_tab_entry_contains(nameRight,local_ST->func_curr->entries);        // Checks if the symbol table contains - if yes we get the index
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
            char* nameRight = ir->right_op;
            int indexRight = sym_tab_entry_contains(nameRight,local_ST->func_curr->entries);        // Checks if the symbol table contains - if yes we get the index
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
/******************************************ASM CODE FNS END******************************************/
char* concat(char* t1,char*t2){
}

ir_code* initialize_ir_code()
/* This function initializes the Quadruple LISt to store the intermediate 3 address code*/
{
    ir_code* newCode = (ir_code*)malloc(sizeof(ir_code));
    newCode->head = NULL;
    newCode->tail = NULL;
    return newCode;
}

ir_code_node* getNew_ir_code_node()
/* This function initializes the Quadruple to store the intermediate 3 address code*/
{
    ir_code_node* newNode = (ir_code_node*)malloc(sizeof(ir_code_node));
    newNode->left_op = NULL;
    newNode->result = NULL;
    newNode->right_op = NULL;
    newNode->next = NULL;
    newNode->operator = -1;
    return newNode;
}   

ir_code* add_node_beg(ir_code_node* entry, ir_code* list)
/* This function adds entry to the end of the list*/
{  
    if(!list){
        list = initialize_ir_code();
        list->head = entry;
        list->tail = entry;
    }
    else{
        ir_code_node* temp = list->head;
        list->head = entry;
        entry->next = temp;
    }
    return list;
}


ir_code* add_node_end(ir_code_node* entry, ir_code* list)
/* This function adds entry to the end of the list*/
{  
    if(!list){
        list = initialize_ir_code();
        list->head = entry;
        list->tail = entry;
    }
    else{
        list->tail->next = entry;
        list->tail = entry;
    }
    return list;
}

ir_code* add_list_beg(ir_code* entry, ir_code* list)
/* This function adds entry to the end of the list*/
{  
    if(!list && !entry){
        return NULL;
    }
    else if(!entry){
        return list;
    }
    else if(!list){
        list = initialize_ir_code();
        list->head = entry->head;
        list->tail = entry->tail;
    }
    else{
        ir_code_node* temp = list->head;
        list->head = entry->head;
        entry->tail->next = temp;
        return list;
    }
}

ir_code* add_list_end(ir_code* entry, ir_code* list)
/* This function adds entry to the end of the list*/
{  
    if(!list && !entry){
        return NULL;
    }
    else if(!entry){
        return list;
    }
    else if(!list){
        list = initialize_ir_code();
        list->head = entry->head;
        list->tail = entry->tail;
    }
    else{
        // if(list->tail==NULL){

        // }
        list->tail->next = entry->head;
        list->tail = entry->tail;
    }
    return list;
}



void add_entry(ir_code* list, ir_code_node* entry)
/* This function adds entry to the end of the list*/
{
    if(list->head = NULL)
        list->head = list->tail = entry;

    else if(list->head == list->tail)
        list->head->next = list->tail = entry;
    
    else
    {
        list->tail->next = entry;
        list->tail = entry;
    }  
}

char* newLabel(){
    char* name = (char*)malloc(sizeof(char)*20);
    sprintf(name,"L%d",currentLabel);
    currentLabel = currentLabel+1;
    return name;
}


char* newTemp(){
    char* name = (char*)malloc(sizeof(char)*20);
    sprintf(name,"t%d",currentTempVar);
    currentTempVar = currentTempVar+1;
    return name;
}

void IR_inputStmt(ast_node* node,func_entry* local_ST,func_entry** global_ST){
    // generate_IR_for_module(node->child_pointers[0],local_ST,global_ST);//STMTS
    ir_code_node* newNode = getNew_ir_code_node();
    newNode->operator = GET_VALUE;
    newNode->result = node->token->lexeme;
    node->code = add_node_beg(newNode,NULL);
}

void IR_outputStmt(ast_node* node,func_entry* local_ST,func_entry** global_ST){
    generate_IR_for_module(node->child_pointers[0],local_ST,global_ST);//STMTS
    ir_code_node* newNode = getNew_ir_code_node();
    newNode->operator = PRINT;
    if(node->child_pointers[0]->isTerminal){
        newNode->result = node->child_pointers[0]->token->lexeme;
    }else{
        newNode->result = node->child_pointers[0]->tempName;
        // sprintf(newNode->result,"%s",node->child_pointers[0]->tempName);
    }
    node->code = add_node_beg(newNode,node->child_pointers[0]->code);
}

void IR_arithmeticExpr(ast_node* node,func_entry* local_ST,func_entry** global_ST){
    generate_IR_for_module(node->child_pointers[0],local_ST,global_ST);//LEFTCHILD
    generate_IR_for_module(node->child_pointers[1],local_ST,global_ST);//RIGHTCHILD
    char* t = newTemp();
    node->tempName = t;
    ir_code_node* newNode = getNew_ir_code_node();

    if(!(node->child_pointers[0]->isTerminal)){
            type_exp temp;
            temp.is_static = 1;
            temp.datatype = node->type;
        sym_tab_entry_add(t,local_ST->func_root,temp); //TODO DEBUG
    }
    else{
        type_exp temp;
        // if(!strcmp(node->child_pointers[0]->token->type,"NUM")){
        temp.is_static = 1;
        temp.datatype = node->type;
        // }
        // else if(!strcmp(node->child_pointers[0]->token->type,"RNUM")){
        //     temp.is_static = 1;
        //     temp.datatype = "real";
        // }
        sym_tab_entry_add(t,local_ST->func_root,temp); //TODO DEBUG
    }
    if(!strcmp(node->name,"PLUS")){
        newNode->operator = ADD;
        newNode->result = node->tempName;
        newNode->left_op = node->child_pointers[0]->tempName;
        newNode->right_op = node->child_pointers[1]->tempName;
    }
    else if(!strcmp(node->name,"MINUS")){
        newNode->operator = SUB;
        newNode->result = node->tempName;
        newNode->left_op = node->child_pointers[0]->tempName;
        newNode->right_op = node->child_pointers[1]->tempName;

    }
    else if(!strcmp(node->name,"MUL")){
        newNode->operator = MUL;
        newNode->result = node->tempName;
        newNode->left_op = node->child_pointers[0]->tempName;
        newNode->right_op = node->child_pointers[1]->tempName;
    }
    else if(!strcmp(node->name,"DIV")){
        newNode->operator = DIV;
        newNode->result = node->tempName;
        newNode->left_op = node->child_pointers[0]->tempName;
        newNode->right_op = node->child_pointers[1]->tempName;
    }
    node->code = add_node_end(newNode,node->child_pointers[1]->code);
    node->code = add_list_beg(node->child_pointers[0]->code,node->code);
}

void IR_switchStmt(ast_node* node,func_entry* local_ST,func_entry** global_ST){
    
}

void IR_iterative(ast_node* node,func_entry* local_ST,func_entry** global_ST){
    
    // ir_code_node* ifNode = getNew_ir_code_node();
    // ifNode->operator = IF;
    // ifNode->result = node->tempName;
    // ifNode->left_op = node->true;
    // ifNode->right_op = node->false;
    // ir_code_node* gotoNode = getNew_ir_code_node();
    // gotoNode->operator = GOTO;
    // gotoNode->result = node->false;
}

void IR_booleanExpr(ast_node* node,func_entry* local_ST,func_entry** global_ST){
    generate_IR_for_module(node->child_pointers[0],local_ST,global_ST);//LEFT CHILD
    generate_IR_for_module(node->child_pointers[1],local_ST,global_ST);//Right child
    node->tempName = newTemp();
    
    type_exp temp;
    temp.is_static = 1;
    temp.datatype = "boolean";
    sym_tab_entry_add(node->tempName,local_ST->func_root,temp);

    ir_code_node* newNode = getNew_ir_code_node();
    if(!strcmp(node->name,"AND")){
        newNode->operator = AND;

    }else if(!strcmp(node->name,"OR")){
        newNode->operator = OR;
    }
    newNode->result = node->tempName;
    newNode->left_op = node->child_pointers[0]->tempName;
    newNode->right_op = node->child_pointers[1]->tempName;
    node->code = add_list_end(node->child_pointers[0]->code,node->code);
    node->code = add_list_end(node->child_pointers[1]->code,node->code);
    node->code = add_node_end(newNode,node->code);
}

void IR_assignmentStmt(ast_node* node,func_entry* local_ST,func_entry** global_ST){
    generate_IR_for_module(node->child_pointers[1],local_ST,global_ST);//RHS
    ir_code_node* newNode = getNew_ir_code_node();
    newNode->operator = ASSIGN;
    if(node->child_pointers[0]->isTerminal)
        newNode->result = node->child_pointers[0]->token->lexeme;
    else{
        generate_IR_for_module(node->child_pointers[0],local_ST,global_ST);
        //TODO
        newNode->result = newTemp();
        node->child_pointers[0]->tempName = newNode->result; //MODIFY
        //add add_list_end()
    }
    newNode->left_op = node->child_pointers[1]->tempName;
    node->code = add_node_end(newNode,node->child_pointers[1]->code);
    // node->asm_code = codegen_assgn_stmt(node,newNode,local_ST,global_ST);
    // printf("\n%s\n",node->asm_code);
}

void IR_functionCall(ast_node* node,func_entry* local_ST,func_entry** global_ST){
    
}

void IR_arrayAccess(ast_node* node,func_entry* local_ST,func_entry** global_ST){
    
}

void IR_relational(ast_node* node,func_entry* local_ST,func_entry** global_ST){
    generate_IR_for_module(node->child_pointers[0],local_ST,global_ST);//LEFTCHILD
    generate_IR_for_module(node->child_pointers[1],local_ST,global_ST);//RIGHTCHILD
    ir_code_node* newNode = getNew_ir_code_node();
    node->tempName = newTemp();
    type_exp temp;
    temp.is_static = 1;
    temp.datatype = "boolean";
    sym_tab_entry_add(node->tempName,local_ST->func_root,temp); //TODO DEBUG

    if(!strcmp(node->name,"LT_result")){
        newNode->operator = LT;
        newNode->result = node->tempName;
        newNode->left_op = node->child_pointers[0]->tempName;
        newNode->right_op = node->child_pointers[1]->tempName;
    }
    else if(!strcmp(node->name,"GT_result")){
        newNode->operator = GT;
        newNode->result = node->tempName;
        newNode->left_op = node->child_pointers[0]->tempName;
        newNode->right_op = node->child_pointers[1]->tempName;

    }
    else if(!strcmp(node->name,"NE_result")){
        newNode->operator = NEQ;
        newNode->result = node->tempName;
        newNode->left_op = node->child_pointers[0]->tempName;
        newNode->right_op = node->child_pointers[1]->tempName;
    }
    else if(!strcmp(node->name,"EQ_result")){
        newNode->operator = EQ;
        newNode->result = node->tempName;
        newNode->left_op = node->child_pointers[0]->tempName;
        newNode->right_op = node->child_pointers[1]->tempName;
    }
    
    else if(!strcmp(node->name,"LE_result")){
        newNode->operator = LE;
        newNode->result = node->tempName;
        newNode->left_op = node->child_pointers[0]->tempName;
        newNode->right_op = node->child_pointers[1]->tempName;
    }
    else if(!strcmp(node->name,"GE_result")){
        newNode->operator = GE;
        newNode->result = node->tempName;
        newNode->left_op = node->child_pointers[0]->tempName;
        newNode->right_op = node->child_pointers[1]->tempName;
    }

    node->code = add_list_end(node->child_pointers[0]->code,node->code);
    node->code = add_list_end(node->child_pointers[1]->code,node->code);
    node->code = add_node_end(newNode,node->code);
}

void IR_codeGen(ast_node* root,func_entry** global_ST){
    ast_node* curr = root;
    if(root==NULL){
        return;
    }
    else if(!strcmp(root->name,"DRIVER")){
        func_entry* local_ST = find_module("DRIVER");
        generate_IR_for_module(root,local_ST,global_ST);
    }
    else if(!strcmp(curr->name,"MODULE")){
        while(curr!=NULL){
            char* func_lex = curr->child_pointers[0]->token->lexeme;
            func_entry* local_ST = find_module(func_lex);
            generate_IR_for_module(curr,local_ST,global_ST);
            curr = curr->next;
        }
    }
}

ir_code* IR_prog(ast_node* root,func_entry** global_ST){
    IR_codeGen(root->child_pointers[1],global_ST); 
    IR_codeGen(root->child_pointers[2],global_ST); 
    IR_codeGen(root->child_pointers[3],global_ST); 
    for(int i=3;i>=1;i--){
        if(root->child_pointers[i]==NULL)continue;
        root->code = add_list_beg(root->child_pointers[i]->code,root->code);
    }
    return root->code;
}
void IR_driverCreation(ast_node* node,func_entry* local_ST,func_entry** global_ST){
    generate_IR_for_module(node->child_pointers[0],local_ST,global_ST);//STMTS
    ir_code_node* newNode = getNew_ir_code_node();
    newNode->operator = FUNC;
    // sprintf(newNode->result,"main");
    
    newNode->result = "main";
    node->code = add_node_beg(newNode,node->child_pointers[0]->code);
    ir_code_node* endNode = getNew_ir_code_node();
    endNode->operator = RET;
    node->code = add_node_end(endNode,node->code);
}

void IR_functionCreation(ast_node* node,func_entry* local_ST,func_entry** global_ST,Boolean listStop){
    ast_node* curr = node;
    generate_IR_for_module(node->child_pointers[3],local_ST,global_ST);//STMTS
    ir_code_node* newNode = getNew_ir_code_node();
    ir_code_node* endNode = getNew_ir_code_node();
    newNode->operator = FUNC;
    // newNode->result = local_ST->name;
    newNode->result = node->child_pointers[0]->token->lexeme;
    node->code = add_node_beg(newNode,node->child_pointers[3]->code);
    endNode->operator = RET;
    node->code = add_node_end(endNode,node->code);
    while(curr->next!=NULL&&listStop==False){    //To handle the list of module that this module has.
        curr= curr->next;
        char* func_lex = curr->child_pointers[0]->token->lexeme;
        func_entry* new_local_ST = find_module(func_lex);

        IR_functionCreation(curr,new_local_ST,global_ST,True);
        node->code = add_list_end(curr->code,node->code);
    }
}
void IR_stmts(ast_node* node,func_entry* local_ST,func_entry** global_ST){
    ast_node* curr = node->next;
    while(curr!=NULL){

        generate_IR_for_module(curr,local_ST,global_ST);//STMTS

        node->code = add_list_end(curr->code,node->code);
        curr = curr->next;
    }
}
void IR_unaryStmts(ast_node* node,func_entry* local_ST,func_entry** global_ST){
    generate_IR_for_module(node->child_pointers[1],local_ST,global_ST);//STMTS
    ir_code_node* minusNode = getNew_ir_code_node();
    if(!strcmp(node->child_pointers[0]->name,"PLUS")){
        node->code = node->child_pointers[1]->code;
        node->tempName = node->child_pointers[1]->tempName;
    }else if(!strcmp(node->child_pointers[0]->name,"MINUS")){
        minusNode->operator = MUL;
        char* str = (char *) malloc(sizeof(char)*10);
        sprintf(str,"%d",-1);
        minusNode->result = newTemp();
        minusNode->left_op = str;
        minusNode->right_op = node->child_pointers[1]->tempName;
        node->code = add_node_end(minusNode,node->child_pointers[1]->code);
        node->tempName = minusNode->result;
    }
}

void print_ir_code(FILE* fptr,ir_code* intermediate_code){
    ir_code_node* curr = intermediate_code->head;
    while(curr){
        if(curr->operator==FUNC||curr->operator==GET_VALUE||curr->operator==PRINT){
            fprintf(fptr,"%s\t%s\n",OPCODE_str[curr->operator],curr->result);
        }
        else if(curr->operator==RET){
            fprintf(fptr,"%s\n",OPCODE_str[curr->operator]);
        }
        else if(curr->operator==ADD){
            fprintf(fptr,"%s = %s %s %s\n",curr->result,curr->left_op,"+",curr->right_op);
        }
        else if(curr->operator==SUB){
            fprintf(fptr,"%s = %s %s %s\n",curr->result,curr->left_op,"-",curr->right_op);
        }
        else if(curr->operator==MUL){
            fprintf(fptr,"%s = %s %s %s\n",curr->result,curr->left_op,"*",curr->right_op);
        }
        else if(curr->operator==DIV){
            fprintf(fptr,"%s = %s %s %s\n",curr->result,curr->left_op,"/",curr->right_op);   
        }
        else if(curr->operator==ASSIGN){
            fprintf(fptr,"%s = %s\n",curr->result,curr->left_op);   
        }
        else if(curr->operator==AND || curr->operator==OR){
            fprintf(fptr,"%s = %s %s %s\n",curr->result,curr->left_op,OPCODE_str[curr->operator],curr->right_op);        }
        else if(curr->operator==LT || curr->operator==LE|| curr->operator==GE|| curr->operator==GT|| curr->operator==NEQ|| curr->operator==EQ){
            fprintf(fptr,"%s = %s %s %s\n",curr->result,curr->left_op,OPCODE_str[curr->operator],curr->right_op);        }

        // fprintf(fptr,"%20s:=%20s\t%20s\t%20s\n",curr->result,curr->result,curr->left_op,(OPCODE)curr->operator,curr->result);
        curr = curr->next;      //TODO Debug for null
    }
}


ir_code* getIRList(ast_node* root, func_entry** global_ST){
    return IR_prog(root,global_ST);
}
void generate_IR_for_module(ast_node* root,func_entry* local_ST,func_entry** global_ST){

    if(!strcmp(root->name,"UNARY")){
        IR_unaryStmts(root,local_ST,global_ST);
    }    
    else if(!strcmp(root->name,"INPUT_ID")){
        IR_inputStmt(root,local_ST,global_ST);
    }    
    else if(!strcmp(root->name,"ARRAY_ASSIGN")||!strcmp(root->name,"ARRAY")||!strcmp(root->name,"ARRAY_ACCESS")){
        // IR_arrayAccess(root,local_ST,global_ST);
    }    

    else if(!strcmp(root->name,"OUTPUT")){
        IR_outputStmt(root,local_ST,global_ST);
    }    

    else if(!strcmp(root->name,"ASSIGN")){
        IR_assignmentStmt(root,local_ST,global_ST);
    }    

    else if(!strcmp(root->name,"SWITCH")){
        IR_switchStmt(root,local_ST,global_ST);
    }    

    else if(!strcmp(root->name,"FORLOOP")||!strcmp(root->name,"WHILELOOP")){
        local_ST->func_curr = local_ST->func_curr->child;  
        IR_iterative(root,local_ST,global_ST);
        if(local_ST->func_curr->r_sibiling!=NULL){
            local_ST->func_curr = local_ST->func_curr->parent;     
        }else
        local_ST->func_curr = local_ST->func_curr->parent;  
    }    

    else if(!strcmp(root->name,"AND")||!strcmp(root->name,"OR")){
        IR_booleanExpr(root,local_ST,global_ST);
    }    

    else if(!strcmp(root->name,"LT_result")||!strcmp(root->name,"LE_result")||!strcmp(root->name,"GT_result")||!strcmp(root->name,"GE_result")||!strcmp(root->name,"NE_result")||!strcmp(root->name,"EQ_result")){
        IR_relational(root,local_ST,global_ST);
    }    

    else if(!strcmp(root->name,"PLUS")||!strcmp(root->name,"MINUS")||!strcmp(root->name,"MUL")||!strcmp(root->name,"DIV")){
        IR_arithmeticExpr(root,local_ST,global_ST);
    }    

    else if(!strcmp(root->name,"MODULEREUSE")){
        IR_functionCall(root,local_ST,global_ST);
    }    

    else if(!strcmp(root->name,"MODULE")){
        IR_functionCreation(root,local_ST,global_ST,False);
    }    
    else if(!strcmp(root->name,"DRIVER")){
        IR_driverCreation(root,local_ST,global_ST);
    }
    else if(!strcmp(root->name,"STATEMENTS")){
        IR_stmts(root,local_ST,global_ST);
    } 
    else{
        root->code=NULL;
    }
}
