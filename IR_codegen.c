#include <string.h>
#include "IR_codegen.h"

static int currentLabel = 1;
static int currentTempVar = 1;

ir_code* global_ir_code;

char* concat(char* t1,char*t2){

}

void initialize_ir_code()
/* This function initializes the Quadruple LISt to store the intermediate 3 address code*/
{
    
    global_ir_code = (ir_code*)malloc(sizeof(ir_code));
    global_ir_code->head = global_ir_code->tail = NULL;
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

}


char* newTemp(){

}

void IR_inputStmt(ast_node* node){

}

void IR_outputStmt(ast_node* node){

}

void IR_assignmentStmt(ast_node* node){
    
}

void IR_switchStmt(ast_node* node){
    
}

<<<<<<< HEAD
void IR_iterative(ast_node* node){
    
}

void IR_booleanExpr(ast_node* node){
    
=======
// void print_ir_code(FILE* fptr){
//     ir_code_node* curr = global_ir_code;
//     while(curr){
//         fprintf(fptr,"%20s:=%20s\t%20s\t%20s\n",curr->result,curr->result,curr->left_op,(OPCODE)curr->operator,curr->result);
//         curr = curr->next;      //TODO Debug for null
//     }
>>>>>>> 17e905aa19d7ff508561c066e41188031dc17101
}

void IR_arithmenticExpr(ast_node* node){
    
}

void IR_functionCall(ast_node* node){
    
}

void IR_functionCreation(ast_node* node){
    
}

void IR_stmts(ast_node* node){
    
}

// void print_ir_code(FILE* fptr,ir_code code){
//     // ir_code_node* curr = global_ir_code;
//     while(curr){
//         fprintf(fptr,"%20s:=%20s\t%20s\t%20s\n",curr->result,curr->result,curr->left_op,(OPCODE)curr->operator,curr->result);
//         curr = curr->next;      //TODO Debug for null
//     }
// }

ir_code* getIRList(ast_node* root){
    IR_codeGen(root); 
    return root->code;
}
void IR_codeGen(){

    
}
// int main(){
//     initialize_ir_code();
//     FILE* fptr = fopen("intermediate_code.txt","w+");
//     // print_ir_code(fptr);
//     fclose(fptr);
//     return 0;
// }