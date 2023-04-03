#include <string.h>
#include "IR_codegen.h"

static int currentLabel = 1;
static int currentTempVar = 1;

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

ir_code* add_to_first(ir_code_node* entry, ir_code* list)
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

void IR_iterative(ast_node* node){
    
}

void IR_booleanExpr(ast_node* node){
    
}

void IR_arithmenticExpr(ast_node* node){
    
}

void IR_functionCall(ast_node* node){
    
}

void IR_arrayAccess(ast_node* node){
    
}


void IR_functionCreation(ast_node* node,func_entry* local_ST,func_entry* global_ST){
    generate_IR_for_module(node->child_pointers[3],local_ST,global_ST);//STMTS
    ir_code_node* newNode = getNew_ir_code_node();
    newNode->operator = FUNC;
    // newNode->result = local_ST->name;
    newNode->result = node->child_pointers[0]->token->lexeme;
    node->code = add_to_first(newNode,node->child_pointers[3]->code);
}
void IR_stmts(ast_node* node){
    return;
}

void print_ir_code(FILE* fptr,ir_code* intermediate_code){
    ir_code_node* curr = intermediate_code->head;
    while(curr){
        if(curr->operator==FUNC){
            fprintf(fptr,"%20s\t%20s\n",OPCODE_str[curr->operator],curr->result);
        }
        // fprintf(fptr,"%20s:=%20s\t%20s\t%20s\n",curr->result,curr->result,curr->left_op,(OPCODE)curr->operator,curr->result);
        curr = curr->next;      //TODO Debug for null
    }
}
void IR_codeGen(ast_node* root,func_entry** global_ST){
    if(root==NULL){
        return;
    }
    else if(!strcmp(root->name,"DRIVER")){
        func_entry* local_ST = find_module("DRIVER");
        generate_IR_for_module(root,local_ST,global_ST);
    }
    else if(!strcmp(root->name,"MODULE")){
        while(root!=NULL){
            char* func_lex = root->child_pointers[0]->token->lexeme;
            func_entry* local_ST = find_module(func_lex);
            generate_IR_for_module(root,local_ST,global_ST);
            root = root->next;
        }
    }
}

ir_code* getIRList(ast_node* root, func_entry** global_ST){
    IR_codeGen(root->child_pointers[1],global_ST); 
    IR_codeGen(root->child_pointers[2],global_ST); 
    IR_codeGen(root->child_pointers[3],global_ST); 
    return root->child_pointers[1]->code;
}
void generate_IR_for_module(ast_node* root,func_entry* local_ST,func_entry* global_ST){

    if(!strcmp(root->name,"INPUT_ID")){
        IR_inputStmt(root);
    }    
    else if(!strcmp(root->name,"ARRAY_ASSIGN")||!strcmp(root->name,"ARRAY")||!strcmp(root->name,"ARRAY_ACCESS")){
        IR_arrayAccess(root);
    }    

    else if(!strcmp(root->name,"OUTPUT")){
        IR_outputStmt(root);
    }    

    else if(!strcmp(root->name,"ASSIGN")){
        IR_assignmentStmt(root);
    }    

    else if(!strcmp(root->name,"SWITCH")){
        IR_switchStmt(root);
    }    

    else if(!strcmp(root->name,"FORLOOP")||!strcmp(root->name,"WHILELOOP")){
        IR_iterative(root);
    }    

    else if(!strcmp(root->name,"INPUT_ID")){
        IR_booleanExpr(root);
    }    

    else if(!strcmp(root->name,"INPUT_ID")){
        IR_arithmenticExpr(root);
    }    

    else if(!strcmp(root->name,"MODULEREUSE")){
        IR_functionCall(root);
    }    

    else if(!strcmp(root->name,"MODULE")){
        IR_functionCreation(root,local_ST,global_ST);
    }    
    else if(!strcmp(root->name,"STATEMENTS")){
        IR_stmts(root);
    } 
    else{
        root->code=NULL;
    }
}
