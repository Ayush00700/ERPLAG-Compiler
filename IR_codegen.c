#include <string.h>
#include "IR_codegen.h"
#include <ctype.h>
static int currentLabel = 1;
static int currentTempVar = 1;


// static int currentChildLabel = 0;

char* findReach(char* val, func_entry* local_ST){
    return "";
}


ir_code* initialize_ir_code()
/* This function initializes the Quadruple LISt to store the intermediate 3 address code*/
{
    ir_code* newCode = (ir_code*)malloc(sizeof(ir_code));
    newCode->head = NULL;
    newCode->tail = NULL;
    return newCode;
}

ir_code_node* getNew_ir_code_node(func_entry* local_ST)
/* This function initializes the Quadruple to store the intermediate 3 address code*/
{
    ir_code_node* newNode = (ir_code_node*)malloc(sizeof(ir_code_node));
    newNode->left_op = (char_offset*) malloc(sizeof(char_offset));
    newNode->right_op = (char_offset*) malloc(sizeof(char_offset));
    newNode->result = (char_offset*) malloc(sizeof(char_offset));
    newNode->left_op->name = NULL;
    newNode->result->name= NULL;
    newNode->right_op->name = NULL;
    newNode->next = NULL;
    newNode->operator = -1;
    newNode->result->name = NULL;
    newNode->left_op->reach = NULL;
    newNode->right_op->reach = NULL;
    newNode->reach = local_ST->func_curr->reach;
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
    ir_code_node* newNode = getNew_ir_code_node(local_ST);
    newNode->operator = GET_VALUE;
    newNode->result->name = node->token->lexeme;
    newNode->result->reach = findReach(newNode->result->name, local_ST);
    node->code = add_node_beg(newNode,NULL);
}

void IR_outputStmt(ast_node* node,func_entry* local_ST,func_entry** global_ST){
    generate_IR_for_module(node->child_pointers[0],local_ST,global_ST);//STMTS
    ir_code_node* newNode = getNew_ir_code_node(local_ST);
    newNode->operator = PRINT;
    if(node->child_pointers[0]->isTerminal){
        newNode->result->name = node->child_pointers[0]->token->lexeme;
        newNode->result->reach = findReach(newNode->result->name, local_ST);
    }else{
        newNode->result->name = node->child_pointers[0]->tempName;
        newNode->result->reach = findReach(newNode->result->name, local_ST);
        // sprintf(newNode->result->name,"%s",node->child_pointers[0]->tempName);
    }
    node->code = add_node_beg(newNode,node->child_pointers[0]->code);
}

void IR_arithmeticExpr(ast_node* node,func_entry* local_ST,func_entry** global_ST){
    generate_IR_for_module(node->child_pointers[0],local_ST,global_ST);//LEFTCHILD
    generate_IR_for_module(node->child_pointers[1],local_ST,global_ST);//RIGHTCHILD
    char* t = newTemp();
    node->tempName = t;
    ir_code_node* newNode = getNew_ir_code_node(local_ST);

    if(!(node->child_pointers[0]->isTerminal)){
            type_exp temp;
            temp.is_static = 1;
            temp.datatype = node->type;
            temp.reach_defined = local_ST->func_curr->reach;
        sym_tab_entry_add(t,local_ST->func_curr,temp); //TODO DEBUG

    }
    else{
        type_exp temp;
        // if(!strcmp(node->child_pointers[0]->token->type,"NUM")){
        temp.is_static = 1;
        temp.datatype = node->type;
        temp.reach_defined = local_ST->func_curr->reach;
        // }
        // else if(!strcmp(node->child_pointers[0]->token->type,"RNUM")){
        //     temp.is_static = 1;
        //     temp.datatype = "real";
        // }
        sym_tab_entry_add(t,local_ST->func_curr,temp); //TODO DEBUG

    }
    if(!strcmp(node->name,"PLUS")){
        newNode->operator = ADD;
        newNode->result->name = node->tempName;
        newNode->left_op->name = node->child_pointers[0]->tempName;
        newNode->right_op->name = node->child_pointers[1]->tempName;
        newNode->result->reach = findReach(newNode->result->name, local_ST);
        newNode->left_op->reach = findReach(newNode->left_op->name, local_ST);
        newNode->right_op->reach = findReach(newNode->right_op->name, local_ST);
    }
    else if(!strcmp(node->name,"MINUS")){
        newNode->operator = SUB;
        newNode->result->name = node->tempName;
        newNode->left_op->name = node->child_pointers[0]->tempName;
        newNode->right_op->name = node->child_pointers[1]->tempName;
        newNode->result->reach = findReach(newNode->result->name, local_ST);
        newNode->left_op->reach = findReach(newNode->left_op->name, local_ST);
        newNode->right_op->reach = findReach(newNode->right_op->name, local_ST);


    }
    else if(!strcmp(node->name,"MUL")){
        newNode->operator = MUL;
        newNode->result->name = node->tempName;
        newNode->left_op->name = node->child_pointers[0]->tempName;
        newNode->right_op->name = node->child_pointers[1]->tempName;
        newNode->result->reach = findReach(newNode->result->name, local_ST);
        newNode->left_op->reach = findReach(newNode->left_op->name, local_ST);
        newNode->right_op->reach = findReach(newNode->right_op->name, local_ST);

    }
    else if(!strcmp(node->name,"DIV")){
        newNode->operator = DIV;
        newNode->result->name = node->tempName;
        newNode->left_op->name = node->child_pointers[0]->tempName;
        newNode->right_op->name = node->child_pointers[1]->tempName;
        newNode->result->reach = findReach(newNode->result->name, local_ST);
        newNode->left_op->reach = findReach(newNode->left_op->name, local_ST);
        newNode->right_op->reach = findReach(newNode->right_op->name, local_ST);

    }
    node->code = add_node_end(newNode,node->child_pointers[1]->code);
    node->code = add_list_beg(node->child_pointers[0]->code,node->code);
}

void IR_switchStmt(ast_node* node,func_entry* local_ST,func_entry** global_ST){
    char* idName = node->child_pointers[0]->tempName;
    ast_node* curr = node->child_pointers[1];
    char* type = curr->type;
    ir_code* ifConds = NULL;
    ir_code* caseStmts = NULL;

    char* boolCheckTemp = newTemp();
    type_exp temp;
    temp.is_static = 1;
    temp.datatype = "boolean";
    temp.reach_defined = local_ST->func_curr->reach;
    sym_tab_entry_add(boolCheckTemp,local_ST->func_curr,temp);

    char* exitLabelString = newLabel();


    while(curr){
        char* currLabel = newLabel();
        char* caseId = curr->child_pointers[0]->tempName;
        ir_code_node* eqCheck = getNew_ir_code_node(local_ST);
        eqCheck->operator = EQ;
        eqCheck->result->name = boolCheckTemp;
        eqCheck->left_op->name = idName;
        eqCheck->right_op->name = caseId;
        eqCheck->result->reach = findReach(eqCheck->result->name, local_ST);
        eqCheck->left_op->reach = findReach(eqCheck->left_op->name, local_ST);
        eqCheck->right_op->reach = findReach(eqCheck->right_op->name, local_ST);


        ir_code_node* ifNode = getNew_ir_code_node(local_ST);
        ifNode->operator = IF;
        ifNode->result->name = boolCheckTemp;
        ifNode->left_op->name = currLabel;
        ifNode->result->reach = findReach(ifNode->result->name, local_ST);
        // ifNode->left_op->reach = findReach(ifNode->left_op->name, local_ST);
        

        ir_code_node* caseLabelNode = getNew_ir_code_node(local_ST);
        caseLabelNode->operator = LABEL;
        caseLabelNode->result->name = currLabel;


        ifConds = add_node_end(eqCheck,ifConds);
        ifConds = add_node_end(ifNode,ifConds);

        generate_IR_for_module(curr->child_pointers[1],local_ST,global_ST);//TODO fix the local_ST context change
        if(local_ST->func_curr->r_sibiling!=NULL){
            local_ST->func_curr = local_ST->func_curr->r_sibiling;     
        }else{
            local_ST->func_curr = local_ST->func_curr->parent;
            // currentChildLabel--;
        }

        caseStmts = add_node_end(caseLabelNode,caseStmts);
        caseStmts = add_list_end(curr->child_pointers[1]->code,caseStmts);

        ir_code_node* exitGoto = getNew_ir_code_node(local_ST);
        exitGoto->operator = GOTO;
        exitGoto->result->name = exitLabelString;

        caseStmts = add_node_end(exitGoto,caseStmts);
        
        
        // curr->code->head->nestingLevel[1] = currentChildLabel;
        // curr->code->head->nestingLevel[0] = currentSibilingLevel;
        // printf("%s Encountered sibiling[%d]:nesting[%d]\n",curr->name,curr->code->head->nestingLevel[0],curr->code->head->nestingLevel[1]);

        curr = curr->next;
        // printf("Case Loop Encountered sibiling[%d]:nesting[%d]\n",caseStmts->tail->nestingLevel[0],caseStmts->tail->nestingLevel[1]);
    }

    ir_code_node* exitGoto = getNew_ir_code_node(local_ST);
    exitGoto->operator = GOTO;
    exitGoto->result->name = exitLabelString;
    curr = node->child_pointers[2];
    if(node->child_pointers[2]&&!strcmp(node->child_pointers[2]->name,"DEFAULTCASE")){

        char* currLabel = newLabel();

        ir_code_node* exitGotoDefault = getNew_ir_code_node(local_ST);
        exitGotoDefault->operator = GOTO;
        exitGotoDefault->result->name = currLabel;

        
        ir_code_node* caseLabelNode = getNew_ir_code_node(local_ST);
        caseLabelNode->operator = LABEL;
        caseLabelNode->result->name = currLabel;

        generate_IR_for_module(curr->child_pointers[0],local_ST,global_ST);//TODO fix the local_ST context change

        if(local_ST->func_curr->r_sibiling!=NULL){
            local_ST->func_curr = local_ST->func_curr->r_sibiling;     
        }else{
            // currentChildLabel--;
            local_ST->func_curr = local_ST->func_curr->parent;
        }
        caseStmts = add_node_end(caseLabelNode,caseStmts);
        caseStmts = add_list_end(curr->child_pointers[0]->code,caseStmts);

        ir_code_node* exitGoto = getNew_ir_code_node(local_ST);
        exitGoto->operator = GOTO;
        exitGoto->result->name = exitLabelString;

        ifConds = add_node_end(exitGotoDefault,ifConds);
        caseStmts = add_node_end(exitGoto,caseStmts);
    // printf("Defaultcase Encountered sibiling[%d]:nesting[%d]\n",caseStmts->tail->nestingLevel[0],caseStmts->tail->nestingLevel[1]);
    }else{
        ifConds = add_node_end(exitGoto,ifConds);
    }

    ir_code_node* exitLabel = getNew_ir_code_node(local_ST);
    exitLabel->operator = LABEL;
    exitLabel->result->name = exitLabelString;

    node->code = add_list_end(ifConds,node->code);
    node->code = add_list_end(caseStmts,node->code);
    node->code = add_node_end(exitLabel,node->code);
    // printf("Switch Loop Encountered sibiling[%d]:nesting[%d]\n",node->code->head->nestingLevel[0],node->code->head->nestingLevel[1]);
}

void IR_iterative_while(ast_node* node,func_entry* local_ST,func_entry** global_ST){
    
    char* begin = newLabel();
    char* trueLabel = newLabel();
    char* falseLabel = newLabel();

    generate_IR_for_module(node->child_pointers[0],local_ST,global_ST);

    ir_code_node* labelNode0 = getNew_ir_code_node(local_ST);
    labelNode0->operator = LABEL;
    labelNode0->result->name = begin;

    ir_code_node* ifNode = getNew_ir_code_node(local_ST);
    ifNode->operator = IF;
    ifNode->left_op->name = trueLabel;
    ifNode->result->name = node->child_pointers[0]->tempName;
    ifNode->result->reach = findReach(ifNode->result->name, local_ST);
    

    ir_code_node* gotoNode = getNew_ir_code_node(local_ST);
    gotoNode->operator = GOTO;
    gotoNode->result->name = falseLabel;
    

    ir_code_node* labelNode1 = getNew_ir_code_node(local_ST);
    labelNode1->operator = LABEL;
    labelNode1->result->name = trueLabel;

    generate_IR_for_module(node->child_pointers[1],local_ST,global_ST);

    ir_code_node* gotoNode2 = getNew_ir_code_node(local_ST);
    gotoNode2->operator = GOTO;
    gotoNode2->result->name = begin;

    ir_code_node* labelNode2 = getNew_ir_code_node(local_ST);
    labelNode2->operator = LABEL;
    labelNode2->result->name = falseLabel;

    node->code = add_node_end(labelNode0,node->code);
    node->code = add_list_end(node->child_pointers[0]->code,node->code);
    node->code = add_node_end(ifNode,node->code);
    node->code = add_node_end(gotoNode,node->code);
    node->code = add_node_end(labelNode1,node->code);
    node->code = add_list_end(node->child_pointers[1]->code,node->code);
    node->code = add_node_end(gotoNode2,node->code);
    node->code = add_node_end(labelNode2,node->code);
    // printf("While Loop Encountered sibiling[%d]:nesting[%d]\n",node->code->head->nestingLevel[0],node->code->head->nestingLevel[1]);
}


void IR_iterative_for(ast_node* node,func_entry* local_ST,func_entry** global_ST){

    char* tempNameLeftRangeOP = node->child_pointers[1]->child_pointers[0]->tempName;
    char* tempNameRightRangeOP = node->child_pointers[1]->child_pointers[1]->tempName;
    char* indexName = node->child_pointers[0]->tempName;
    char* boolCheckTemp = newTemp();

    ir_code_node* assignNode = getNew_ir_code_node(local_ST);
    assignNode->operator = ASSIGN;
    assignNode->result->name = indexName;
    assignNode->left_op->name = tempNameLeftRangeOP;
    assignNode->result->reach = findReach(assignNode->result->name, local_ST);
    assignNode->left_op->reach = findReach(assignNode->left_op->name, local_ST);


    type_exp temp;
    temp.is_static = 1;
    temp.datatype = "boolean";
    temp.reach_defined = local_ST->func_curr->reach;
    sym_tab_entry_add(boolCheckTemp,local_ST->func_curr,temp);

    char* begin = newLabel();
    char* trueLabel = newLabel();
    char* falseLabel = newLabel();

    ir_code_node* labelNode0 = getNew_ir_code_node(local_ST);
    labelNode0->operator = LABEL;
    labelNode0->result->name = begin;

    ir_code_node* relOp1 = getNew_ir_code_node(local_ST);
    relOp1->operator = LE;
    relOp1->left_op->name = indexName;
    relOp1->right_op->name = tempNameRightRangeOP;
    relOp1->result->name = boolCheckTemp;
    relOp1->result->reach = findReach(relOp1->result->name, local_ST);
    relOp1->left_op->reach = findReach(relOp1->left_op->name, local_ST);
    relOp1->right_op->reach = findReach(relOp1->right_op->name, local_ST);


    ir_code_node* ifNode = getNew_ir_code_node(local_ST);
    ifNode->operator = IF;
    ifNode->result->name = boolCheckTemp;
    ifNode->left_op->name = trueLabel;
    ifNode->result->reach = findReach(ifNode->result->name, local_ST);


    ir_code_node* gotoNode = getNew_ir_code_node(local_ST);
    gotoNode->operator = GOTO;
    gotoNode->result->name = falseLabel;

    ir_code_node* labelNode1 = getNew_ir_code_node(local_ST);
    labelNode1->operator = LABEL;
    labelNode1->result->name = trueLabel;

    generate_IR_for_module(node->child_pointers[2],local_ST,global_ST);

    char* one = (char*) malloc(sizeof(char)*2);
    sprintf(one,"%d",1);
    ir_code_node* incNode = getNew_ir_code_node(local_ST);
    incNode->operator = ADD;
    incNode->left_op->name = indexName;
    incNode->right_op->name = one;
    incNode->result->name = indexName;
    incNode->result->reach = findReach(incNode->result->name, local_ST);
    incNode->left_op->reach = findReach(incNode->left_op->name, local_ST);
    incNode->right_op->reach = findReach(incNode->right_op->name, local_ST);


    ir_code_node* gotoNode2 = getNew_ir_code_node(local_ST);
    gotoNode2->operator = GOTO;
    gotoNode2->result->name = begin;

    ir_code_node* labelNode2 = getNew_ir_code_node(local_ST);
    labelNode2->operator = LABEL;
    labelNode2->result->name = falseLabel;

    node->code = add_node_end(assignNode,node->code);
    node->code = add_node_end(labelNode0,node->code);
    node->code = add_node_end(relOp1,node->code);
    node->code = add_node_end(ifNode,node->code);
    node->code = add_node_end(gotoNode,node->code);
    node->code = add_node_end(labelNode1,node->code);
    node->code = add_list_end(node->child_pointers[2]->code,node->code);
    node->code = add_node_end(incNode,node->code);
    node->code = add_node_end(gotoNode2,node->code);
    node->code = add_node_end(labelNode2,node->code);
    // printf("For Loop Encountered sibiling[%d]:nesting[%d]\n",node->code->head->nestingLevel[0],node->code->head->nestingLevel[1]);
}

void IR_booleanExpr(ast_node* node,func_entry* local_ST,func_entry** global_ST){
    generate_IR_for_module(node->child_pointers[0],local_ST,global_ST);//LEFT CHILD
    generate_IR_for_module(node->child_pointers[1],local_ST,global_ST);//Right child
    node->tempName = newTemp();
    
    type_exp temp;
    temp.is_static = 1;
    temp.datatype = "boolean";
    temp.reach_defined = local_ST->func_curr->reach;
    sym_tab_entry_add(node->tempName,local_ST->func_curr,temp);

    ir_code_node* newNode = getNew_ir_code_node(local_ST);
    if(!strcmp(node->name,"AND")){
        newNode->operator = AND;

    }else if(!strcmp(node->name,"OR")){
        newNode->operator = OR;
    }
    newNode->result->name = node->tempName;
    newNode->left_op->name = node->child_pointers[0]->tempName;
    newNode->right_op->name = node->child_pointers[1]->tempName;
    newNode->result->reach = findReach(newNode->result->name, local_ST);
    newNode->left_op->reach = findReach(newNode->left_op->name, local_ST);
    newNode->right_op->reach = findReach(newNode->right_op->name, local_ST);

    node->code = add_list_end(node->child_pointers[0]->code,node->code);
    node->code = add_list_end(node->child_pointers[1]->code,node->code);
    node->code = add_node_end(newNode,node->code);
}

void IR_assignmentStmt(ast_node* node,func_entry* local_ST,func_entry** global_ST){
    generate_IR_for_module(node->child_pointers[1],local_ST,global_ST);//RHS
    ir_code_node* newNode = getNew_ir_code_node(local_ST);
    newNode->operator = ASSIGN;
    if(node->child_pointers[0]->isTerminal){
        newNode->result->name = node->child_pointers[0]->token->lexeme;
        newNode->result->reach = findReach(newNode->result->name, local_ST);
    }
    else{
        IR_arrayAssign(node->child_pointers[0],local_ST,global_ST,node->child_pointers[1]);

        // newNode->result->name = node->child_pointers[0]->tempName;
        node->code = node->child_pointers[0]->code;
        return;
        //
        // newNode->result->name = newTemp();
        // node->child_pointers[0]->tempName = newNode->result->name; //MODIFY
        //add add_list_end()
    }
    newNode->left_op->name = node->child_pointers[1]->tempName;
    node->code = add_node_end(newNode,node->child_pointers[1]->code);
}

void IR_functionCall(ast_node* node,func_entry* local_ST,func_entry** global_ST){
    ir_code_node* callNode = getNew_ir_code_node(local_ST);
    ir_code* paraInList = NULL;
    ir_code* paraOutList = NULL;
    callNode->operator = CALL;
    callNode->result->name = node->child_pointers[0]->token->lexeme;
    ast_node* curr = node->child_pointers[2];
    int rightParaCount = 0;
    int leftParaCount = 0;
    while(curr!=NULL){
        rightParaCount++;
        if(!curr->child_pointers[1]->child_pointers[0]->isTerminal)
            generate_IR_for_module(curr->child_pointers[1]->child_pointers[0],local_ST,global_ST);
        ir_code_node* paraInNode = getNew_ir_code_node(local_ST);
        paraInNode->operator = PARA_IN;
        paraInNode->result->name = curr->child_pointers[1]->child_pointers[0]->tempName;
        paraInNode->result->reach = findReach(paraInNode->result->name, local_ST);
        paraInList = add_node_end(paraInNode,paraInList);
        curr = curr->next;
    }
    curr = node->child_pointers[1];
    while(curr!=NULL){
        leftParaCount++;
        ir_code_node* paraOutNode = getNew_ir_code_node(local_ST);
        paraOutNode->operator = PARA_OUT;
        paraOutNode->result->name = curr->tempName;
        paraOutNode->result->reach = findReach(paraOutNode->result->name, local_ST);

        paraOutList = add_node_end(paraOutNode,paraOutList);
        curr = curr->next;
    }
    char* rightChar = (char*) malloc(sizeof(char)*2);
    sprintf(rightChar,"%d",rightParaCount);
    char* leftChar = (char*) malloc(sizeof(char)*2);
    sprintf(leftChar,"%d",leftParaCount);
    callNode->left_op->name = leftChar;
    callNode->right_op->name = rightChar;
    node->code = add_list_end(paraInList,node->code);
    node->code = add_node_end(callNode,node->code);
    node->code = add_list_end(paraOutList,node->code);
}

void IR_arrayAssign(ast_node* node,func_entry* local_ST,func_entry** global_ST,ast_node* nodeExp2){
    // generate_IR_for_module(nodeExp2,local_ST,global_ST);
    generate_IR_for_module(node->child_pointers[1],local_ST,global_ST);
    char* string = (char*) malloc(sizeof(char)*20);
    type_exp* tarr = find_expr(node->child_pointers[0],local_ST,node->child_pointers[0]->token->line_no);
    ir_code_node* relOp1 = getNew_ir_code_node(local_ST);
    ir_code_node* relOp2 = getNew_ir_code_node(local_ST);
    ir_code_node* orNode = getNew_ir_code_node(local_ST);
    ir_code_node* ifNode = getNew_ir_code_node(local_ST);
    ir_code_node* offsetNode1 = getNew_ir_code_node(local_ST);
    ir_code_node* offsetNode2 = getNew_ir_code_node(local_ST);
    ir_code_node* memWriteNode = getNew_ir_code_node(local_ST);

    char* string1 = (char*) malloc(sizeof(char)*20);
    sprintf(string1,"%d",tarr->arr_data->lower_bound);

    relOp1->operator = LT;
    relOp1->left_op->name = node->child_pointers[1]->tempName;
    relOp1->right_op->name = string1;
    relOp1->result->name = newTemp();
    relOp1->result->reach = findReach(relOp1->result->name, local_ST);
    relOp1->left_op->reach = findReach(relOp1->left_op->name, local_ST);
    relOp1->right_op->reach = findReach(relOp1->right_op->name, local_ST);

    
    type_exp temp;
    temp.is_static = 1;
    temp.datatype = "boolean";
    temp.reach_defined = local_ST->func_curr->reach;
    sym_tab_entry_add(relOp1->result->name,local_ST->func_curr,temp);

    char* string2 = (char*) malloc(sizeof(char)*20);
    sprintf(string2,"%d",tarr->arr_data->upper_bound);

    relOp2->operator = GT;
    relOp2->left_op->name = node->child_pointers[1]->tempName;
    relOp2->right_op->name = string2;
    relOp2->result->name = newTemp();
    relOp2->result->reach = findReach(relOp2->result->name, local_ST);
    relOp2->left_op->reach = findReach(relOp2->left_op->name, local_ST);
    relOp2->right_op->reach = findReach(relOp2->right_op->name, local_ST);


    
    temp.is_static = 1;
    temp.datatype = "boolean";
    temp.reach_defined = local_ST->func_curr->reach;
    sym_tab_entry_add(relOp2->result->name,local_ST->func_curr,temp);

    orNode->operator = OR;
    orNode->left_op->name = relOp1->result->name;
    orNode->right_op->name = relOp2->result->name;
    orNode->result->name = newTemp();
    orNode->result->reach = findReach(orNode->result->name, local_ST);
    orNode->left_op->reach = findReach(orNode->left_op->name, local_ST);
    orNode->right_op->reach = findReach(orNode->right_op->name, local_ST);


    
    temp.is_static = 1;
    temp.datatype = "boolean";
    temp.reach_defined = local_ST->func_curr->reach;
    sym_tab_entry_add(orNode->result->name,local_ST->func_curr,temp);

    ifNode->operator = IF;
    ifNode->left_op->name = newLabel();
    ifNode->result->name = orNode->result->name;
    ifNode->result->reach = findReach(ifNode->result->name, local_ST);

    
    temp.is_static = 1;
    temp.datatype = "boolean";
    temp.reach_defined = local_ST->func_curr->reach;
    sym_tab_entry_add(ifNode->result->name,local_ST->func_curr,temp);
    int width;
    if(!strcmp(tarr->arr_data->arr_datatype,"integer")){
        width = INT_OFFSET;
    }
    else if(!strcmp(tarr->arr_data->arr_datatype,"real")){
        width = REAL_OFFSET;
    }
    else if(!strcmp(tarr->arr_data->arr_datatype,"boolean")){
        width = BOOL_OFFSET;
    }
    int tempOffset = -1; //TODO fix for dynamic arrays
    if(tarr->is_static==1)
        tempOffset = tarr->offset - tarr->arr_data->lower_bound*width;
    else
        tempOffset = -1* tarr->arr_data->lower_bound*width;
    char* string3 = (char*) malloc(sizeof(char)*20);
    sprintf(string3,"%d",width);

    offsetNode1->operator = MUL;
    offsetNode1->left_op->name = node->child_pointers[1]->tempName;
    offsetNode1->right_op->name = string3;
    offsetNode1->result->name = newTemp();
    offsetNode1->result->reach = findReach(offsetNode1->result->name, local_ST);
    offsetNode1->left_op->reach = findReach(offsetNode1->left_op->name, local_ST);
    offsetNode1->right_op->reach = findReach(offsetNode1->right_op->name, local_ST);

    
    temp.is_static = 1;
    temp.datatype = "integer";
    temp.reach_defined = local_ST->func_curr->reach;
    sym_tab_entry_add(offsetNode1->result->name,local_ST->func_curr,temp);

    char* string4 = (char*) malloc(sizeof(char)*20);
    sprintf(string4,"%d",tempOffset);
    offsetNode2->operator = ADD;
    offsetNode2->left_op->name = string4;
    offsetNode2->right_op->name = offsetNode1->result->name;
    offsetNode2->result->name = newTemp();
    offsetNode2->result->reach = findReach(offsetNode2->result->name, local_ST);
    offsetNode2->left_op->reach = findReach(offsetNode2->left_op->name, local_ST);
    offsetNode2->right_op->reach = findReach(offsetNode2->right_op->name, local_ST);


    
    temp.is_static = 1;
    temp.datatype = "integer";
    temp.reach_defined = local_ST->func_curr->reach;
    sym_tab_entry_add(offsetNode2->result->name,local_ST->func_curr,temp);

    if(tarr->is_static==1){
        memWriteNode->operator = MEMWRITE_ST;
        memWriteNode->result->name = node->child_pointers[0]->token->lexeme;
        memWriteNode->left_op->name = offsetNode2->result->name;
        memWriteNode->right_op->name = nodeExp2->tempName;
        memWriteNode->result->reach = findReach(memWriteNode->result->name, local_ST);
        memWriteNode->left_op->reach = findReach(memWriteNode->left_op->name, local_ST);
        memWriteNode->right_op->reach = findReach(memWriteNode->right_op->name, local_ST);

    }
    else{
        memWriteNode->operator = MEMWRITE;
        memWriteNode->result->name = node->child_pointers[0]->token->lexeme;
        memWriteNode->left_op->name = offsetNode2->result->name;
        memWriteNode->right_op->name = nodeExp2->tempName;
        memWriteNode->result->reach = findReach(memWriteNode->result->name, local_ST);
        memWriteNode->left_op->reach = findReach(memWriteNode->left_op->name, local_ST);
        memWriteNode->right_op->reach = findReach(memWriteNode->right_op->name, local_ST);
    }

    node->code = add_list_end(nodeExp2->code,node->code);
    node->code = add_list_end(node->child_pointers[1]->code,node->code);
    node->code = add_node_end(relOp1,node->code);
    node->code = add_node_end(relOp2,node->code);
    node->code = add_node_end(orNode,node->code);
    node->code = add_node_end(ifNode,node->code);
    node->code = add_node_end(offsetNode1,node->code);
    node->code = add_node_end(offsetNode2,node->code);
    node->code = add_node_end(memWriteNode,node->code);

    // ifNode->operator = IF;
    // ifNode->


    // node->code = add_node_end(memWriteNode,node->code);


}

void IR_arrayAccess(ast_node* node,func_entry* local_ST,func_entry** global_ST){
    // generate_IR_for_module(node->child_pointers[0],local_ST,global_ST);
    generate_IR_for_module(node->child_pointers[1],local_ST,global_ST);
    type_exp temp;
    type_exp* tarr = find_expr(node->child_pointers[0],local_ST,node->child_pointers[0]->token->line_no);

    char* string = (char*) malloc(sizeof(char)*20);
    ir_code_node* relOp1 = getNew_ir_code_node(local_ST);
    ir_code_node* relOp2 = getNew_ir_code_node(local_ST);
    ir_code_node* orNode = getNew_ir_code_node(local_ST);
    ir_code_node* ifNode = getNew_ir_code_node(local_ST);
    ir_code_node* offsetNode1 = getNew_ir_code_node(local_ST);
    ir_code_node* offsetNode2 = getNew_ir_code_node(local_ST);
    ir_code_node* memWriteNode = getNew_ir_code_node(local_ST);


    char* string1 = (char*) malloc(sizeof(char)*20);
    sprintf(string1,"%d",tarr->arr_data->lower_bound);

    relOp1->operator = LT;
    relOp1->left_op->name = node->child_pointers[1]->tempName;
    relOp1->right_op->name = string1;
    relOp1->result->name = newTemp();
    relOp1->result->reach = findReach(relOp1->result->name, local_ST);
    relOp1->left_op->reach = findReach(relOp1->left_op->name, local_ST);
    relOp1->right_op->reach = findReach(relOp1->right_op->name, local_ST);

    
    temp.is_static = 1;
    temp.datatype = "boolean";
    temp.reach_defined = local_ST->func_curr->reach;
    sym_tab_entry_add(relOp1->result->name,local_ST->func_curr,temp);

    char* string2 = (char*) malloc(sizeof(char)*20);
    sprintf(string2,"%d",tarr->arr_data->upper_bound);

    relOp2->operator = GT;
    relOp2->left_op->name = node->child_pointers[1]->tempName;
    relOp2->right_op->name = string2;
    relOp2->result->name = newTemp();
    relOp2->result->reach = findReach(relOp2->result->name, local_ST);
    relOp2->left_op->reach = findReach(relOp2->left_op->name, local_ST);
    relOp2->right_op->reach = findReach(relOp2->right_op->name, local_ST);

    
    temp.is_static = 1;
    temp.datatype = "boolean";
    temp.reach_defined = local_ST->func_curr->reach;
    sym_tab_entry_add(relOp2->result->name,local_ST->func_curr,temp);

    orNode->operator = OR;
    orNode->left_op->name = relOp1->result->name;
    orNode->right_op->name = relOp2->result->name;
    orNode->result->name = newTemp();
    orNode->result->reach = findReach(orNode->result->name, local_ST);
    orNode->left_op->reach = findReach(orNode->left_op->name, local_ST);
    orNode->right_op->reach = findReach(orNode->right_op->name, local_ST);

    
    temp.is_static = 1;
    temp.datatype = "boolean";
    temp.reach_defined = local_ST->func_curr->reach;
    sym_tab_entry_add(orNode->result->name,local_ST->func_curr,temp);

    ifNode->operator = IF;
    ifNode->left_op->name = newLabel();
    ifNode->result->name = orNode->result->name;
    ifNode->result->reach = findReach(ifNode->result->name, local_ST);

    
    temp.is_static = 1;
    temp.datatype = "boolean";
    temp.reach_defined = local_ST->func_curr->reach;
    sym_tab_entry_add(ifNode->result->name,local_ST->func_curr,temp);
    int width;
    if(!strcmp(tarr->arr_data->arr_datatype,"integer")){
        width = INT_OFFSET;
    }
    else if(!strcmp(tarr->arr_data->arr_datatype,"real")){
        width = REAL_OFFSET;
    }
    else if(!strcmp(tarr->arr_data->arr_datatype,"boolean")){
        width = BOOL_OFFSET;
    }
    int tempOffset = -1; //TODO fix for dynamic arrays
    if(tarr->is_static==1)
        tempOffset = tarr->offset - tarr->arr_data->lower_bound*width;
    else
        tempOffset = -1* tarr->arr_data->lower_bound*width;
    char* string3 = (char*) malloc(sizeof(char)*20);
    sprintf(string3,"%d",width);

    offsetNode1->operator = MUL;
    offsetNode1->left_op->name = node->child_pointers[1]->tempName;
    offsetNode1->right_op->name = string3;
    offsetNode1->result->name = newTemp();
    offsetNode1->result->reach = findReach(offsetNode1->result->name, local_ST);
    offsetNode1->left_op->reach = findReach(offsetNode1->left_op->name, local_ST);
    offsetNode1->right_op->reach = findReach(offsetNode1->right_op->name, local_ST);
    
    temp.is_static = 1;
    temp.datatype = "integer";
    temp.reach_defined = local_ST->func_curr->reach;
    sym_tab_entry_add(offsetNode1->result->name,local_ST->func_curr,temp);

    char* string4 = (char*) malloc(sizeof(char)*20);
    sprintf(string4,"%d",tempOffset);
    offsetNode2->operator = ADD;
    offsetNode2->left_op->name = string4;
    offsetNode2->right_op->name = offsetNode1->result->name;
    offsetNode2->result->name = newTemp();
    offsetNode2->result->reach = findReach(offsetNode2->result->name, local_ST);
    offsetNode2->left_op->reach = findReach(offsetNode2->left_op->name, local_ST);
    offsetNode2->right_op->reach = findReach(offsetNode2->right_op->name, local_ST);


    
    temp.is_static = 1;
    temp.datatype = "integer";
    temp.reach_defined = local_ST->func_curr->reach;
    sym_tab_entry_add(offsetNode2->result->name,local_ST->func_curr,temp);

    node->tempName = newTemp();
    temp.is_static = 1;
    temp.datatype = tarr->arr_data->arr_datatype;
    temp.reach_defined = local_ST->func_curr->reach;
    sym_tab_entry_add(node->tempName,local_ST->func_curr,temp);

    if(tarr->is_static==1){
        memWriteNode->operator = MEMREAD_ST;
        memWriteNode->result->name =node->tempName; 
        memWriteNode->left_op->name = node->child_pointers[0]->token->lexeme;
        memWriteNode->right_op->name = offsetNode2->result->name;
        memWriteNode->result->reach = findReach(memWriteNode->result->name, local_ST);
        memWriteNode->left_op->reach = findReach(memWriteNode->left_op->name, local_ST);
        memWriteNode->right_op->reach = findReach(memWriteNode->right_op->name, local_ST);
    }
    else{
        memWriteNode->operator = MEMREAD;
        memWriteNode->result->name =node->tempName; 
        memWriteNode->left_op->name = node->child_pointers[0]->token->lexeme;
        memWriteNode->right_op->name = offsetNode2->result->name;
        memWriteNode->result->reach = findReach(memWriteNode->result->name, local_ST);
        memWriteNode->left_op->reach = findReach(memWriteNode->left_op->name, local_ST);
        memWriteNode->right_op->reach = findReach(memWriteNode->right_op->name, local_ST);
    }
    


    node->code = add_list_end(node->child_pointers[1]->code,node->code);
    node->code = add_node_end(relOp1,node->code);
    node->code = add_node_end(relOp2,node->code);
    node->code = add_node_end(orNode,node->code);
    node->code = add_node_end(ifNode,node->code);
    node->code = add_node_end(offsetNode1,node->code);
    node->code = add_node_end(offsetNode2,node->code);
    node->code = add_node_end(memWriteNode,node->code);

    // ifNode->operator = IF;
    // ifNode->


    // node->code = add_node_end(memWriteNode,node->code);


}

void IR_relational(ast_node* node,func_entry* local_ST,func_entry** global_ST){
    generate_IR_for_module(node->child_pointers[0],local_ST,global_ST);//LEFTCHILD
    generate_IR_for_module(node->child_pointers[1],local_ST,global_ST);//RIGHTCHILD
    ir_code_node* newNode = getNew_ir_code_node(local_ST);
    node->tempName = newTemp();
    type_exp temp;
    temp.is_static = 1;
    temp.datatype = "boolean";
    temp.reach_defined = local_ST->func_curr->reach;
    sym_tab_entry_add(node->tempName,local_ST->func_curr,temp); //TODO DEBUG

    if(!strcmp(node->name,"LT_result")){
        newNode->operator = LT;
        newNode->result->name = node->tempName;
        newNode->left_op->name = node->child_pointers[0]->tempName;
        newNode->right_op->name = node->child_pointers[1]->tempName;
        newNode->result->reach = findReach(newNode->result->name, local_ST);
        newNode->left_op->reach = findReach(newNode->left_op->name, local_ST);
        newNode->right_op->reach = findReach(newNode->right_op->name, local_ST);

    }
    else if(!strcmp(node->name,"GT_result")){
        newNode->operator = GT;
        newNode->result->name = node->tempName;
        newNode->left_op->name = node->child_pointers[0]->tempName;
        newNode->right_op->name = node->child_pointers[1]->tempName;
        newNode->result->reach = findReach(newNode->result->name, local_ST);
        newNode->left_op->reach = findReach(newNode->left_op->name, local_ST);
        newNode->right_op->reach = findReach(newNode->right_op->name, local_ST);


    }
    else if(!strcmp(node->name,"NE_result")){
        newNode->operator = NEQ;
        newNode->result->name = node->tempName;
        newNode->left_op->name = node->child_pointers[0]->tempName;
        newNode->right_op->name = node->child_pointers[1]->tempName;
        newNode->result->reach = findReach(newNode->result->name, local_ST);
        newNode->left_op->reach = findReach(newNode->left_op->name, local_ST);
        newNode->right_op->reach = findReach(newNode->right_op->name, local_ST);

    }
    else if(!strcmp(node->name,"EQ_result")){
        newNode->operator = EQ;
        newNode->result->name = node->tempName;
        newNode->left_op->name = node->child_pointers[0]->tempName;
        newNode->right_op->name = node->child_pointers[1]->tempName;
        newNode->result->reach = findReach(newNode->result->name, local_ST);
        newNode->left_op->reach = findReach(newNode->left_op->name, local_ST);
        newNode->right_op->reach = findReach(newNode->right_op->name, local_ST);
    }
    
    else if(!strcmp(node->name,"LE_result")){
        newNode->operator = LE;
        newNode->result->name = node->tempName;
        newNode->left_op->name = node->child_pointers[0]->tempName;
        newNode->right_op->name = node->child_pointers[1]->tempName;
        newNode->result->reach = findReach(newNode->result->name, local_ST);
        newNode->left_op->reach = findReach(newNode->left_op->name, local_ST);
        newNode->right_op->reach = findReach(newNode->right_op->name, local_ST);

    }
    else if(!strcmp(node->name,"GE_result")){
        newNode->operator = GE;
        newNode->result->name = node->tempName;
        newNode->left_op->name = node->child_pointers[0]->tempName;
        newNode->right_op->name = node->child_pointers[1]->tempName;
        newNode->result->reach = findReach(newNode->result->name, local_ST);
        newNode->left_op->reach = findReach(newNode->left_op->name, local_ST);
        newNode->right_op->reach = findReach(newNode->right_op->name, local_ST);

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
        func_entry* local_ST = find_module_global("DRIVER");
        generate_IR_for_module(root,local_ST,global_ST);
    }
    else if(!strcmp(curr->name,"MODULE")){
        while(curr!=NULL){
            char* func_lex = curr->child_pointers[0]->token->lexeme;
            func_entry* local_ST = find_module_global(func_lex);
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

    // currentSibilingLevel = 0;
    // currentChildLabel = 0;
    ir_code_node* newNode = getNew_ir_code_node(local_ST);
    newNode->operator = FUNC;
    // sprintf(newNode->result->name,"main");
    
    newNode->result->name = "main";
    node->code = add_node_beg(newNode,node->child_pointers[0]->code);
    //TODO AYUSUDU
    // currentSibilingLevel = 0;
    // currentChildLabel = 0;

    ir_code_node* endNode = getNew_ir_code_node(local_ST);
    endNode->operator = RET;
    node->code = add_node_end(endNode,node->code);
}

void IR_functionCreation(ast_node* node,func_entry* local_ST,func_entry** global_ST,Boolean listStop){
    ast_node* curr = node;
    generate_IR_for_module(node->child_pointers[3],local_ST,global_ST);//STMTS
    
    // currentSibilingLevel = 0;
    // currentChildLabel = 0;
    
    ir_code_node* newNode = getNew_ir_code_node(local_ST);
    ir_code_node* endNode = getNew_ir_code_node(local_ST);
    newNode->operator = FUNC;
    // newNode->result->name = local_ST->name;
    newNode->result->name = node->child_pointers[0]->token->lexeme;
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
    // int currentSibilingLevel = 0;
    int flag = 0;
    int flag2 =0;
    while(curr!=NULL){    
        if(flag==0&&(!strcmp(curr->name,"FORLOOP")||!strcmp(curr->name,"WHILELOOP")||!strcmp(curr->name,"SWITCH"))){
            flag=1;
            flag2 = 1;
            local_ST->func_curr = local_ST->func_curr->child;
            // currentChildLabel++;  
        }
        generate_IR_for_module(curr,local_ST,global_ST);
        if(flag2){
            // currentSibilingLevel++;
        }
        node->code = add_list_end(curr->code,node->code);
        if(flag2){
            // curr->code->head->nestingLevel[1] = currentChildLabel;
            // curr->code->head->nestingLevel[0] = currentSibilingLevel;
            // printf("%s Encountered sibiling[%d]:nesting[%d]\n",curr->name,curr->code->head->nestingLevel[0],curr->code->head->nestingLevel[1]);
        }
        curr = curr->next;
        flag2=0;
    }
}
void IR_unaryStmts(ast_node* node,func_entry* local_ST,func_entry** global_ST){
    generate_IR_for_module(node->child_pointers[1],local_ST,global_ST);//STMTS
    ir_code_node* minusNode = getNew_ir_code_node(local_ST);
    if(!strcmp(node->child_pointers[0]->name,"PLUS")){
        node->code = node->child_pointers[1]->code;
        node->tempName = node->child_pointers[1]->tempName;
    }else if(!strcmp(node->child_pointers[0]->name,"MINUS")){
        minusNode->operator = MUL;
        char* str = (char *) malloc(sizeof(char)*10);
        sprintf(str,"%d",-1);
        minusNode->result->name = newTemp();
        minusNode->left_op->name = str;
        minusNode->right_op->name = node->child_pointers[1]->tempName;
        minusNode->result->reach = findReach(minusNode->result->name, local_ST);
        minusNode->left_op->reach = findReach(minusNode->left_op->name, local_ST);
        minusNode->right_op->reach = findReach(minusNode->right_op->name, local_ST);

        
        type_exp temp;
        temp.is_static = 1;
        if(isdigit(minusNode->right_op->name[0])){
            int type_left_int = (strchr(minusNode->right_op->name, '.'))? 0 : 1;
            if(type_left_int){
                temp.datatype  = "integer";
            }
            else{
                temp.datatype  = "real";
            }
        }
        else{
            temp.datatype = node->child_pointers[1]->type;
        }
        temp.reach_defined = local_ST->func_curr->reach;
        sym_tab_entry_add(minusNode->result->name,local_ST->func_curr,temp);

        node->code = add_node_end(minusNode,node->child_pointers[1]->code);
        node->tempName = minusNode->result->name;
    }
}

void print_ir_code(FILE* fptr,ir_code* intermediate_code){
    ir_code_node* curr = intermediate_code->head;
    while(curr){
        if(curr->operator==FUNC||curr->operator==GET_VALUE||curr->operator==PRINT||curr->operator==PARA_IN||curr->operator==PARA_OUT||curr->operator==CALL||curr->operator==GOTO){
            fprintf(fptr,"%s\t%s\t\t\n",OPCODE_str[curr->operator],curr->result->name);
        }
        else if(curr->operator==LABEL){
            fprintf(fptr,"%s: ",curr->result->name);
        }
        else if(curr->operator==RET){
            fprintf(fptr,"%s\t\t\n",OPCODE_str[curr->operator]);
        }
        else if(curr->operator==ADD){
            fprintf(fptr,"%s = %s %s %s\t\t\n",curr->result->name,curr->left_op->name,"+",curr->right_op->name);
        }
        else if(curr->operator==SUB){
            fprintf(fptr,"%s = %s %s %s\t\t\n",curr->result->name,curr->left_op->name,"-",curr->right_op->name);
        }
        else if(curr->operator==MUL){
            fprintf(fptr,"%s = %s %s %s\t\t\n",curr->result->name,curr->left_op->name,"*",curr->right_op->name);
        }
        else if(curr->operator==DIV){
            fprintf(fptr,"%s = %s %s %s\t\t\n",curr->result->name,curr->left_op->name,"/",curr->right_op->name);   
        }
        else if(curr->operator==IF){
            fprintf(fptr,"%s %s GOTO %s\t\t\n",OPCODE_str[curr->operator],curr->result->name,curr->left_op->name);   
        }
        else if(curr->operator==ASSIGN){
            fprintf(fptr,"%s = %s\t\t\n",curr->result->name,curr->left_op->name);   
        }
        else if(curr->operator==MEMWRITE_ST||curr->operator==MEMWRITE){
            fprintf(fptr,"%s[%s] = %s\t\t\n",curr->result->name,curr->left_op->name,curr->right_op->name);   
        }
        else if(curr->operator==MEMREAD_ST||curr->operator==MEMREAD){
            fprintf(fptr,"%s = %s[%s]\t\t\n",curr->result->name,curr->left_op->name,curr->right_op->name);   
        }
        
        else if(curr->operator==AND || curr->operator==OR){
            fprintf(fptr,"%s = %s %s %s\t\t\n",curr->result->name,curr->left_op->name,OPCODE_str[curr->operator],curr->right_op->name);        }
        else if(curr->operator==LT || curr->operator==LE|| curr->operator==GE|| curr->operator==GT|| curr->operator==NEQ|| curr->operator==EQ){
            fprintf(fptr,"%s = %s %s %s\t\t\n",curr->result->name,curr->left_op->name,OPCODE_str[curr->operator],curr->right_op->name);        }

        // fprintf(fptr,"%20s:=%20s\t%20s\t%20s\n",curr->result->name,curr->result->name,curr->left_op->name,(OPCODE)curr->operator,curr->result->name);
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
    else if(!strcmp(root->name,"ARRAY_ACCESS")){
        IR_arrayAccess(root,local_ST,global_ST);
    }   

    else if(!strcmp(root->name,"OUTPUT")){
        IR_outputStmt(root,local_ST,global_ST);
    }    

    else if(!strcmp(root->name,"ASSIGN")){
        IR_assignmentStmt(root,local_ST,global_ST);
    }    

    else if(!strcmp(root->name,"SWITCH")){
        IR_switchStmt(root,local_ST,global_ST);  //local_ST
    }    
    else if(!strcmp(root->name,"FORLOOP")||!strcmp(root->name,"WHILELOOP")){ 
        if(!strcmp(root->name,"FORLOOP"))
            IR_iterative_for(root,local_ST,global_ST);
        else
            IR_iterative_while(root,local_ST,global_ST);
        if(local_ST->func_curr->r_sibiling!=NULL){
            local_ST->func_curr = local_ST->func_curr->r_sibiling;     
        }else{
            local_ST->func_curr = local_ST->func_curr->parent;  
            // currentChildLabel--;
        }
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

