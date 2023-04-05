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
    newNode->left_op = (char*)malloc(sizeof(char)*20);
    newNode->result = (char*)malloc(sizeof(char)*20);
    newNode->right_op = (char*)malloc(sizeof(char)*20);
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
    sprintf(newNode->result,"%s",node->token->lexeme);
    node->code = add_node_beg(newNode,NULL);
}

void IR_outputStmt(ast_node* node,func_entry* local_ST,func_entry** global_ST){
    generate_IR_for_module(node->child_pointers[0],local_ST,global_ST);//STMTS
    ir_code_node* newNode = getNew_ir_code_node();
    newNode->operator = PRINT;
    if(node->child_pointers[0]->isTerminal){
        sprintf(newNode->result,"%s",node->child_pointers[0]->token->lexeme);
    }else{
        sprintf(newNode->result,"%s",node->child_pointers[0]->tempName);
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
        sym_tab_entry_add(t,local_ST->func_root,local_ST->func_root->entries[hash(node->child_pointers[0]->tempName)]->type); //TODO DEBUG
    }
    else{
        type_exp temp;
        if(!strcmp(node->child_pointers[0]->token->type,"NUM")){
            temp.is_static = 1;
            temp.datatype = "integer";
        }
        else if(!strcmp(node->child_pointers[0]->token->type,"RNUM")){
            temp.is_static = 1;
            temp.datatype = "real";
        }
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
    
}

void IR_booleanExpr(ast_node* node,func_entry* local_ST,func_entry** global_ST){
    
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
}

void IR_functionCall(ast_node* node,func_entry* local_ST,func_entry** global_ST){
    
}

void IR_arrayAccess(ast_node* node,func_entry* local_ST,func_entry** global_ST){
    
}

void IR_relational(ast_node* node,func_entry* local_ST,func_entry** global_ST){
    generate_IR_for_module(node->child_pointers[0],local_ST,global_ST);//LEFTCHILD
    generate_IR_for_module(node->child_pointers[1],local_ST,global_ST);//RIGHTCHILD
    ir_code_node* newNode = getNew_ir_code_node();

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
    node->code = add_node_end(newNode,node->child_pointers[1]->code);
    node->code = add_node_end(newNode,node->child_pointers[1]->code);
    ir_code_node* gotoNode = getNew_ir_code_node();
    newNode->operator = GE;
    newNode->result = node->tempName;
    newNode->left_op = node->child_pointers[0]->tempName;
    newNode->right_op = node->child_pointers[1]->tempName;

    node->code = add_list_beg(node->child_pointers[0]->code,node->code);  
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
    sprintf(newNode->result,"main");
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
    if(!strcmp(root->name,"INPUT_ID")){
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
        IR_iterative(root,local_ST,global_ST);
    }    

    else if(!strcmp(root->name,"AND")||!strcmp(root->name,"OR")){
        IR_booleanExpr(root,local_ST,global_ST);
    }    

    else if(!strcmp(root->name,"LT")||!strcmp(root->name,"LE")||!strcmp(root->name,"GT")||!strcmp(root->name,"GE")||!strcmp(root->name,"NE")||!strcmp(root->name,"EQ")){
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
