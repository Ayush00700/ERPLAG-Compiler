#include "semanticAnalyzer.h"
#include <string.h>


#define INT_OFFSET 4 
#define REAL_OFFSET 8
#define BOOL_OFFSET 1
#define POINTER_OFFSET 4

func_entry* global_func_table[TABLE_SIZE];

// ----------------------------------------------HELPER FUNCTIONS FOR SET OPERATIONS ----------------------------------------------//
int sym_tab_entry_add(char* key,var_record* local_table,type_exp temp)
/*This function adds the key into the table along with the entry number
0 => need not be added since it already exists
1 => needs to be added*/
{
    // Hash the key to get the index
    unsigned int index = hash(key);
    sym_tab_entry* current = local_table->entries[index];

    // first checks whether it is already present or not 
    while(current!=NULL){
        if(strcmp(current->name,key)==0){
            return -1; //in case it doesn't need to add
        }
        current = current->next;
    }

    // else need to add the element
    sym_tab_entry* new_node = (sym_tab_entry*)malloc(sizeof(sym_tab_entry));
    new_node -> name = key;
    new_node -> next = local_table->entries[index];
    local_table->entries[index] = new_node;
    new_node->type = temp;
    new_node->offset = local_table->offset;
    if(!strcmp(temp.datatype,"integer")){
        local_table->offset += INT_OFFSET;
    }
    else if(!strcmp(temp.datatype,"real")){
        local_table->offset += REAL_OFFSET;
    }
    else if(!strcmp(temp.datatype,"boolean")){
        local_table->offset += BOOL_OFFSET;
    }
    else if(!strcmp(temp.datatype,"array")){
        if(temp.is_static){
            int x;
            if(!strcmp(temp.arr_data->arr_datatype,"real")){x=REAL_OFFSET;}
            else if(!strcmp(temp.arr_data->arr_datatype,"boolean")){x=BOOL_OFFSET;}
            else{x=INT_OFFSET;}
            int l = temp.arr_data->lower_bound;
            int u = temp.arr_data->upper_bound;
            local_table->offset += (u-l+1)*x;
        }
        else{
            local_table->offset += POINTER_OFFSET;
        }
    }
    return index;
}

int sym_tab_entry_contains(char* key,sym_tab_entry* table[])
/*Checks if the key is there in the table*/
{
    // Hash the key to get index
    unsigned int index = hash(key);
    sym_tab_entry* current = table[index];

    // If didn't match at first, go through the chain
    while(current!=NULL){
        if(strcmp(current->name,key)==0){
            // printf(" %d %d",current->entry_number,current->order);
            return 1;
        }
        current = current->next;
    }
    return 0;
}

// function table

func_entry* func_tab_entry_add(char* key,func_entry* table[],sym_tab_entry* input_list,sym_tab_entry* ouput_list,int* offset)
/*This function adds the key into the table along with the entry number
0 => need not be added since it already exists
1 => needs to be added*/
{
    // Hash the key to get the index
    unsigned int index = hash(key);
    func_entry* current = table[index];

    // first checks whether it is already present or not 
    while(current!=NULL){
        if(strcmp(current->name,key)==0){
            return NULL; //in case it doesn't need to add
        }
        current = current->next;
    }

    // else need to add the element
    func_entry* new_node = (func_entry*)malloc(sizeof(func_entry));
    new_node -> name = key;
    new_node -> next = table[index];
    table[index] = new_node;
    new_node->input_list = input_list;
    new_node->ouput_list = ouput_list;
    new_node->func_root = (var_record*) malloc(sizeof(var_record));
    new_node->func_root->construct_name = key;
    new_node->func_curr = new_node->func_root;
    new_node->offset = *offset;
    new_node->func_root->offset = new_node->offset;
    return new_node;
}

void list_add(sym_tab_entry* list,ast_node* ast_root,int* offset,int initial){
    type_exp temp;
    if(strcmp(ast_root->child_pointers[1]->name,"ARRAY_DATATYPE")){
        temp.datatype = ast_root->child_pointers[1]->token->lexeme;
    }
    else{
        temp.datatype = "array";
        temp.arr_data = (arr_struct*) malloc(sizeof(arr_struct));
        temp.arr_data->arr_datatype =  ast_root->child_pointers[1]->child_pointers[1]->token->lexeme;
        int lower = check_static(ast_root->child_pointers[1]->child_pointers[0]->child_pointers[0]);
        int upper = check_static(ast_root->child_pointers[1]->child_pointers[0]->child_pointers[1]);
        if(lower&&upper){
            temp.arr_data->lower_bound = index_finder(ast_root->child_pointers[1]->child_pointers[0]->child_pointers[0]);
            temp.arr_data->upper_bound = index_finder(ast_root->child_pointers[1]->child_pointers[0]->child_pointers[1]);
            temp.is_static = 1;
        }
        else{
            temp.is_static = 0;
        }
    }
    if(!initial){
        list->name = ast_root->child_pointers[0]->token->lexeme;
        list->next = NULL;
        list->type = temp;
        list->offset = *offset;
    }
    else{
        while(list->next!=NULL){
            list = list->next;
        }
        list->next = (sym_tab_entry*) malloc(sizeof(sym_tab_entry));
        list->next->name = ast_root->child_pointers[0]->token->lexeme;
        list->next->next = NULL;
        list->next->type = temp;
        list->next->offset = *offset;
    }
    if(!strcmp(temp.datatype,"integer")){
        *offset += INT_OFFSET;
    }
    else if(!strcmp(temp.datatype,"real")){
        *offset += REAL_OFFSET;
    }
    else if(!strcmp(temp.datatype,"boolean")){
        *offset += BOOL_OFFSET;
    }
    else if(!strcmp(temp.datatype,"array")){
        if(temp.is_static){
            int x;
            if(!strcmp(temp.arr_data->arr_datatype,"real")){x=REAL_OFFSET;}
            else if(!strcmp(temp.arr_data->arr_datatype,"boolean")){x=BOOL_OFFSET;}
            else{x=INT_OFFSET;}
            int l = temp.arr_data->lower_bound;
            int u = temp.arr_data->upper_bound;
            *offset += (u-l+1)*x;
        }
        else{
            *offset += POINTER_OFFSET;
        }
    }
}

int func_tab_entry_contains(char* key,func_entry* table[])
/*Checks if the key is there in the table*/
{
    // Hash the key to get index
    unsigned int index = hash(key);
    func_entry* current = table[index];

    // If didn't match at first, go through the chain
    while(current!=NULL){
        if(strcmp(current->name,key)==0){
            // printf(" %d %d",current->entry_number,current->order);
            return index;
        }
        current = current->next;
    }
    return 0;
}



int check_static(ast_node* node){
    if(!strcmp(node->child_pointers[1]->name,"ID")){
        return 0;
    }
    else{
        return 1;
    }
}

int index_finder(ast_node* node){
    if(node->child_pointers[0]==NULL || !strcmp(node->child_pointers[0]->name,"PLUS")){
        return node->child_pointers[1]->token->values.num;
    }
    else{
        return -1*(node->child_pointers[1]->token->values.num);
    }
}

void populate_symbol_table(ast_node* temp_node,type_exp temp,var_record* local_table){
    int a = sym_tab_entry_add(temp_node->token->lexeme,local_table,temp);
    if(a == -1){ // to change as the abstraction is increased
        printf("already present in the symbol table\n");
    }
}

void compute_expression(ast_node* ast_root,var_record* local_table){
    type_exp temp;
    if(strcmp(ast_root->child_pointers[1]->name,"ARRAY_DATATYPE")){
        temp.datatype = ast_root->child_pointers[1]->token->lexeme;
    }
    else{
        temp.datatype = "array";
        temp.arr_data = (arr_struct*) malloc(sizeof(arr_struct));
        temp.arr_data->arr_datatype =  ast_root->child_pointers[1]->child_pointers[1]->token->lexeme;
        int lower = check_static(ast_root->child_pointers[1]->child_pointers[0]->child_pointers[0]);
        int upper = check_static(ast_root->child_pointers[1]->child_pointers[0]->child_pointers[1]);
        if(lower&&upper){
            temp.arr_data->lower_bound = index_finder(ast_root->child_pointers[1]->child_pointers[0]->child_pointers[0]);
            temp.arr_data->upper_bound = index_finder(ast_root->child_pointers[1]->child_pointers[0]->child_pointers[1]);
            temp.is_static = 1;
        }
        else{
            temp.is_static = 0;
        }
    }
    ast_node* temp_node = ast_root;
    temp_node = temp_node->child_pointers[0];
    while(temp_node!=NULL){
        populate_symbol_table(temp_node,temp,local_table);
        temp_node = temp_node->next;
    }
    return;
}

sym_tab_entry* getlist(ast_node* ast_root,int* offset){
    sym_tab_entry* list_head = (sym_tab_entry*) malloc(sizeof(sym_tab_entry));
    sym_tab_entry* temp = list_head;
    ast_node* temp_node = ast_root;
    int initial = 0;
    while(temp_node!=NULL){ 
        list_add(temp,temp_node,offset,initial);
        temp = list_head;
        temp_node = temp_node->next;
        initial++;
    }
    return list_head;
}

void local_populate(var_record* local_table,ast_node* ast_root){
    if(!ast_root){
        return;
    }
    else if(!strcmp(ast_root->name,"CASE")){
        local_populate(local_table,ast_root->child_pointers[1]);
        var_record* local_case = (var_record*) malloc(sizeof(var_record));
        local_case->parent = local_table->parent;
        local_case->child = NULL;
        local_case->r_sibiling = NULL;
        local_case->offset = local_table->offset;
        local_case->construct_name = "CASE";
        local_populate(local_case,ast_root->next);
        local_table->offset = local_case->offset;
        local_table->r_sibiling = local_case;
    }
    else if(!strcmp(ast_root->name,"FORLOOP")){
        var_record* local_for = (var_record*) malloc(sizeof(var_record));
        local_for->parent = local_table;
        local_for->child = NULL;
        local_for->r_sibiling = NULL;
        local_for->offset = local_table->offset;
        local_for->construct_name = "FORLOOP";
        if(local_table->child == NULL){
            local_table->child = local_for;
        }
        else{
            var_record* temp = local_table->child;
            while(temp->r_sibiling!=NULL){
                temp = temp->r_sibiling;
            }
            temp->r_sibiling = local_for;
        }
        local_populate(local_for,ast_root->child_pointers[2]);
        local_table->offset = local_for->offset;
    }
    else if(!strcmp(ast_root->name,"WHILELOOP")){
        var_record* local_while = (var_record*) malloc(sizeof(var_record));
        local_while->parent = local_table;
        local_while->child = NULL;
        local_while->r_sibiling = NULL;
        local_while->offset = local_table->offset;
        local_while->construct_name = "WHILELOOP";
        if(local_table->child == NULL){
            local_table->child = local_while;
        }
        else{
            var_record* temp = local_table->child;
            while(temp->r_sibiling!=NULL){
                temp = temp->r_sibiling;
            }
            temp->r_sibiling = local_while;
        }
        local_populate(local_while,ast_root->child_pointers[1]);
        local_table->offset = local_while->offset; 
    }
    else if(!strcmp(ast_root->name,"SWITCH")){
        var_record* local_switch = (var_record*) malloc(sizeof(var_record));
        local_switch->parent = local_table;
        local_switch->child = NULL;
        local_switch->r_sibiling = NULL;
        local_switch->offset = local_table->offset;
        local_switch->construct_name = "CASE";
        if(local_table->child == NULL){
            local_table->child = local_switch;
        }
        else{
            var_record* temp = local_table->child;
            while(temp->r_sibiling!=NULL){
                temp = temp->r_sibiling;
            }
            temp->r_sibiling = local_switch;
        }
        local_populate(local_switch,ast_root->child_pointers[1]);
        local_table->offset = local_switch->offset;
        //naya for default
        if(ast_root->child_pointers[2] == NULL){
            return;
        }
        var_record* local_switch_default = (var_record*) malloc(sizeof(var_record));
        local_switch_default->parent = local_table;
        local_switch_default->child = NULL;
        local_switch_default->r_sibiling = NULL;
        local_switch_default->offset = local_table->offset;
        local_switch->construct_name = "DEFAULT";
         if(local_table->child == NULL){
            local_table->child = local_switch_default;
        }
        else{
            var_record* temp = local_table->child;
            while(temp->r_sibiling!=NULL){
                temp = temp->r_sibiling;
            }
            temp->r_sibiling = local_switch_default;
        }
        local_populate(local_switch_default,ast_root->child_pointers[2]);
        local_table->offset = local_switch_default->offset; 
    }
    else if(!strcmp(ast_root->name,"DECLARE")){
        compute_expression(ast_root,local_table);
        local_populate(local_table,ast_root->next);
    }
    else if(ast_root->isTerminal){
        local_populate(local_table,ast_root->next);
        return;
    }
    else{
        int num = ast_root->no_of_children;
        for(int i=0;i<num;i++){
            local_populate(local_table,ast_root->child_pointers[i]);
        }
        local_populate(local_table,ast_root->next);
        return;
    }
}

void func_def(ast_node* ast_root){
    int offset = 0;
    sym_tab_entry* ip_list = getlist(ast_root->child_pointers[1],&offset);
    sym_tab_entry* op_list = getlist(ast_root->child_pointers[2],&offset);
    func_entry* local;
    if(!strcmp(ast_root->name,"DRIVER")){
        local = func_tab_entry_add("DRIVER",global_func_table,ip_list,op_list,&offset);
        local_populate(local->func_root,ast_root->child_pointers[0]);
    }else{
        local = func_tab_entry_add(ast_root->child_pointers[0]->token->lexeme,global_func_table,ip_list,op_list,&offset);
        local_populate(local->func_root,ast_root->child_pointers[3]);
    }
}

void populate_(ast_node* ast_root){
    if(!ast_root){
        return;
    }
    else if(!strcmp(ast_root->name,"DRIVER")){
        func_def(ast_root);
    }
    else if(!strcmp(ast_root->name,"MODULE")){
        func_def(ast_root);
        populate_(ast_root->next);
    }
    else{
        int num = ast_root->no_of_children;
        for(int i=0;i<num;i++){
            populate_(ast_root->child_pointers[i]);
        }
        populate_(ast_root->next);
        return;
    }
}


//create enum
type_exp* throw_error(semErrors error)
{
    //check with enum 
    type_exp* gen_error=(type_exp*) malloc(sizeof(type_exp));
    switch(error)
    {
        case TYPE_NOT_MATCHED: {
        gen_error->datatype="TypeNotMatched";
        return gen_error;}
        case OUT_OF_SCOPE_VARIABLE: {
        gen_error->datatype="OutOfScope";
        return gen_error;}
        case UNSUPPORTED_DTYPE: {
        gen_error->datatype="OutOfScope";
        return gen_error;
        }

        
    }
}

type_exp* compare_dTypes(type_exp* left, type_exp* right)
{
        if(!strcmp(left->datatype,right->datatype))
        {
            if(!strcmp(left->datatype,"array"))
            {
                if(!strcmp(left->arr_data->arr_datatype,right->arr_data->arr_datatype))
                {
                    if(left->arr_data->lower_bound==right->arr_data->lower_bound
                    &&left->arr_data->upper_bound==right->arr_data->upper_bound)
                    {
                        return left;
                    }
                  else return throw_error(TYPE_NOT_MATCHED);
                }
                else return throw_error(TYPE_NOT_MATCHED); 
            }
            else return left;
        }
        else return throw_error(TYPE_NOT_MATCHED);
}

type_exp* find_in_table(char* key,var_record* table){
    int index = sym_tab_entry_contains(key,table->entries);

    sym_tab_entry* temp = table->entries[index];

    while(temp!=NULL){
        if(!strcmp(temp->name,key)){
            return &temp->type;
        }
        temp = temp->next;
    }
    return throw_error(OUT_OF_SCOPE_VARIABLE);

}

type_exp* find_expr(ast_node* node, func_entry* curr)
{
    if(!curr)
    {
        return throw_error(OUT_OF_SCOPE_VARIABLE);
    }
    var_record* current_rec= curr->func_curr;
    var_record* temp=current_rec;
    char * key=node->token->lexeme;
    type_exp* type=find_in_table(key, current_rec);
    if(type)
    {
        return type;
    }    
    else
    {
        curr->func_curr=curr->func_curr->parent; //also need to add for checking variable in the op list and ip list of the function record
        type= find_expr(node,curr);
        curr->func_curr=temp;
        return type;
    }

}

type_exp* type_checking(ast_node* node, func_entry* curr)
{
    if(!strcmp(node->name,"ID"))
    {
        type_exp* var_exp= find_expr(node, curr);
        if(!strcmp(var_exp->datatype,"OutOfScope"))
        printf("Error found at line no %d : Out of scope \n", node->token->line_no);
        return var_exp;
    }
    else if(!strcmp(node->name,"ASSIGN"))
    {
        type_exp* left =type_checking(node->child_pointers[0],curr);
        type_exp* right=type_checking(node->child_pointers[1],curr);
        type_exp* ret=compare_dTypes(left, right);
        
        if(!strcmp(ret->datatype,"TypeNotMatched"))
        {
            printf("Error found at line no %d : Type Mismatch \n", node->child_pointers[0]->token->line_no);
        }
        return ret;
    }
    else if(!strcmp(node->name,"NUM"))
    {
        type_exp* temp =(type_exp*) malloc(sizeof(type_exp));
        temp->datatype="integer";
        return temp;
    }
    else if(!strcmp(node->name,"RNUM"))
    {
        type_exp* temp =(type_exp*) malloc(sizeof(type_exp));
        temp->datatype="real";
        return temp;
    }
    else if(!strcmp(node->name,"TRUE")||!strcmp(node->name,"FALSE"))
    {
        type_exp* temp =(type_exp*) malloc(sizeof(type_exp));
        temp->datatype="boolean";
        return temp;
    }else if(!strcmp(node->name,"UNARY")){
        
        //CHILD_POINTER[0] just has a plus or minus sign no need to call anything
        if(!node->child_pointers[0]) return throw_error(UNSUPPORTED_DTYPE);
        //CHILD_POINTER[1] overall type can be a num, rnum, integer, real or boolean
        type_exp* temp = type_checking(node->child_pointers[1],curr);
        //(only primitives)
        if(!strcmp(temp->datatype,"array")){printf("\nError found at line no %d : Unary operation not supported for array dataype", node->child_pointers[0]->token->line_no);}
        else if(!strcmp(temp->datatype,"integer")||
                !strcmp(temp->datatype,"real")/* ||
                do we need to add other datatypes ?*/){return temp;}
        else{printf("\nError found at line no %d : Unary operation not supported for the dataype", node->token->line_no);}

        return throw_error(UNSUPPORTED_DTYPE);
    }
    else if(!strcmp(node->name, "ARRAY_ACCESS")){
        //CHILD_POINTER[0] will always be ID of the array (need to check it's type)
        type_exp* var_id = type_checking(node->child_pointers[0],curr);
        //CHILD_POINTER[1] will always give an expression, overall type could be 
        type_exp* arr_expr = type_checking(node->child_pointers[1],curr);

        if(!strcmp(arr_expr->datatype,"integer")){
            if(!strcmp(var_id->datatype,"array")){
                type_exp* temp =(type_exp*) malloc(sizeof(type_exp));
                temp->datatype = var_id->arr_data->arr_datatype;
                return temp;
            }else {printf("Error found at line no %d : ID is not of array datatype",node->child_pointers[0]->token->line_no);}
        }else {printf("Error found at line no %d : array index expression only supports integer type",node->child_pointers[0]->token->line_no);}

        //num or integer
        return throw_error(UNSUPPORTED_DTYPE);
        //to return only the type of ID
    }
    else if(!strcmp(node->name, "PLUS")||!strcmp(node->name, "MINUS")||!strcmp(node->name, "MULT"))
    {
        if(node->no_of_children!=0){    //to handle non unary expression 
        type_exp* left =type_checking(node->child_pointers[0],curr);
        type_exp* right=type_checking(node->child_pointers[1],curr);
        type_exp* ret=compare_dTypes(left, right);
    //check that they are both real||int
        if(!strcmp(ret->datatype,"TypeNotMatched"))
        {
            printf("\nError found at line no %d : Type Mismatch", node->token->line_no);
        }
        return ret;
        }
     }
    else if(!strcmp(node->name, "DIV"))
    {
        //divide by zero check
    } 
    else if (!strcmp(node->name, "AND")||!strcmp(node->name, "OR"))
    {
        //Check that they are both boolean
    }
    else if(!strcmp(node->name, "FORINDEX"))
    {
        // check index types
    }
    else if(!strcmp(node->name, "FORRANGE"))
    {
        //check increasing order
    }
    else if(!strcmp(node->name, "FORLOOP"))
    {
        //change the current variable record
        curr->func_curr=curr->func_curr->child;

    }
    else if(!strcmp(node->name, "WHILELOOP"))
    {
        curr->func_curr=curr->func_curr->child;

    }
    //might need to handle cases and default and switch
    else return NULL;
}

/*Functions to add
    traverse parse tree and change the func_entry
    searche the variable for variabe symbol_entry in the current func_entry
    
*/
func_entry* find_module(char* key){
    int index = func_tab_entry_contains(key, global_func_table);
    func_entry* function = global_func_table[index];

    while(function!=NULL){
        if(!strcmp(function->name,key)){
            return function;
        }
        function = function->next;
    }
}


void perform_type_checking(ast_node* ast_root,func_entry* func){

    if(ast_root==NULL) {return ;}
    
    else if(!strcmp(ast_root->name,"DRIVER")){
        func = find_module("DRIVER");
    }
    else if(!strcmp(ast_root->name,"MODULE")){
        func = find_module(ast_root->child_pointers[0]->token->lexeme);
    }
    else if(!strcmp(ast_root->name,"STATEMENTS")){
        while(ast_root->next!=NULL){
        type_checking(ast_root->next,func);
        ast_root = ast_root->next;
        }
        type_checking(ast_root,func);
    }
    int num_of_children = ast_root->no_of_children;
    for(int i=0;i<num_of_children;i++){
        perform_type_checking(ast_root->child_pointers[i],func);
    }
    perform_type_checking(ast_root->next,func);
    return ;
}


void semantic(){
    ast_node* ast_root = get_ast_root();

    populate_(ast_root);
    perform_type_checking( ast_root, NULL);

}