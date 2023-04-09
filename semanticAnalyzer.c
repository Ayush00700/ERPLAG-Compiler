#include "semanticAnalyzer.h"
#include <string.h>



func_entry* global_func_table[TABLE_SIZE];
func_entry* global_TABLE[TABLE_SIZE];

void initialize_global_func_table(){

    for(int i=0;i<TABLE_SIZE;i++){
        global_func_table[i] = NULL;
    }
}

void init_global(func_entry* global[]){
        for(int i=0;i<TABLE_SIZE;i++){
        global[i] = NULL;
    }
}

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
    new_node->offset = local_table->offset;
    temp.offset = new_node->offset;
    new_node->type = temp;

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
            return index;
        }
        current = current->next;
    }
    return -1;
}

// function table

void initialize_var_record(func_entry* new_node){
    new_node->func_root = (var_record*) malloc(sizeof(var_record));
    
    for(int i=0;i<TABLE_SIZE;i++){
        new_node->func_root->entries[i] = NULL;
    }

}

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
    initialize_var_record(new_node);
    new_node->func_root->construct_name = key;
    new_node->func_root->parent = NULL;
    new_node->func_root->child = NULL;
    new_node->func_root->r_sibiling = NULL;
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
    return -1;
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
        printf(" \"%s\" redeclared variable :: Error found at line no %d\n",temp_node->token->lexeme
        ,temp_node->token->line_no);
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
    if(initial==0)return NULL;
    return list_head;
}
void initialize_entries(var_record* local_table){

    for (int i=0;i<TABLE_SIZE;i++){
        local_table->entries[i] = NULL;
    }
}

void for_populate(var_record* local_table,ast_node* ast_root){
    type_exp temp;
    temp.datatype = "integer";
    ast_node* temp_node = ast_root;
    populate_symbol_table(temp_node,temp,local_table);
}


void local_populate(var_record* local_table,ast_node* ast_root){
    if(!ast_root){
        return;
    }
    else if(!strcmp(ast_root->name,"CASE_HEAD")){
        local_populate(local_table,ast_root->child_pointers[1]);
        var_record* local_case = (var_record*) malloc(sizeof(var_record));
        local_case->parent = local_table->parent;
        local_case->child = NULL;
        local_case->r_sibiling = NULL;
        local_case->offset = local_table->parent->offset;
        local_case->construct_name = "CASE";
        initialize_entries(local_case);
        //local_table->offset = local_case->offset;
        if(ast_root->next!=NULL)local_populate(local_case,ast_root->next);
        if(ast_root->next!=NULL)local_table->r_sibiling = local_case;
    }
    else if(!strcmp(ast_root->name,"CASE")){
        local_populate(local_table,ast_root->child_pointers[1]);
        var_record* local_case = (var_record*) malloc(sizeof(var_record));
        local_case->parent = local_table->parent;
        local_case->child = NULL;
        local_case->r_sibiling = NULL;
        local_case->offset = local_table->parent->offset;
        local_case->construct_name = "CASE";
        initialize_entries(local_case);
        //local_table->offset = local_case->offset;
        if(ast_root->next!=NULL)local_populate(local_case,ast_root->next);
        // local_table->offset = local_case->offset;
        if(ast_root->next!=NULL)local_table->r_sibiling = local_case;
    }
    else if(!strcmp(ast_root->name,"FORLOOP")){
        var_record* local_for = (var_record*) malloc(sizeof(var_record));
        local_for->parent = local_table;
        local_for->child = NULL;
        local_for->r_sibiling = NULL;
        local_for->offset = local_table->offset;
        local_for->construct_name = "FORLOOP";
        initialize_entries(local_for);
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
        for_populate(local_for,ast_root->child_pointers[0]);
        local_populate(local_for,ast_root->child_pointers[2]);
        //local_table->offset = local_for->offset;
        local_populate(local_table,ast_root->next);
    }
    else if(!strcmp(ast_root->name,"WHILELOOP")){
        var_record* local_while = (var_record*) malloc(sizeof(var_record));
        local_while->parent = local_table;
        local_while->child = NULL;
        local_while->r_sibiling = NULL;
        local_while->offset = local_table->offset;
        local_while->construct_name = "WHILELOOP";
        initialize_entries(local_while);
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
        //local_table->offset = local_while->offset; 
        local_populate(local_table,ast_root->next);
    }
    else if(!strcmp(ast_root->name,"SWITCH")){
        var_record* local_switch = (var_record*) malloc(sizeof(var_record));
        local_switch->parent = local_table;
        local_switch->child = NULL;
        local_switch->r_sibiling = NULL;
        int temp = local_table->offset;
        local_switch->offset = local_table->offset;
        local_switch->construct_name = "CASE_HEAD";
        initialize_entries(local_switch);
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
        //local_table->offset = temp;
        //naya for default
        if(ast_root->child_pointers[2] == NULL){
            local_populate(local_table,ast_root->next);
            return;
        }
        var_record* local_switch_default = (var_record*) malloc(sizeof(var_record));
        local_switch_default->parent = local_table;
        local_switch_default->child = NULL;
        local_switch_default->r_sibiling = NULL;
        local_switch_default->offset = local_table->offset;
        local_switch_default->construct_name = "DEFAULT";
        initialize_entries(local_switch_default);
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
        //local_table->offset = local_switch_default->offset; 
        local_populate(local_table,ast_root->next);
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
void func_def_(ast_node* ast_root,func_entry* global[]){
     int offset = 0;
    sym_tab_entry* ip_list = NULL; sym_tab_entry* op_list = NULL;
    if(!strcmp(ast_root->name,"MODULE")){ip_list = getlist(ast_root->child_pointers[1],&offset);}
    if(!strcmp(ast_root->name,"MODULE")){op_list = getlist(ast_root->child_pointers[2],&offset);}
    func_entry* local = NULL;
    if(!strcmp(ast_root->name,"DRIVER")){
        local = func_tab_entry_add("DRIVER",global,ip_list,op_list,&offset);
        local_populate(local->func_root,ast_root->child_pointers[0]);
    }else{
        local = func_tab_entry_add(ast_root->child_pointers[0]->token->lexeme,global,ip_list,op_list,&offset);
        local_populate(local->func_root,ast_root->child_pointers[3]);
    }
}

void func_def(ast_node* ast_root){
    int offset = 0;
    sym_tab_entry* ip_list = NULL; sym_tab_entry* op_list = NULL;
    if(!strcmp(ast_root->name,"MODULE")){ip_list = getlist(ast_root->child_pointers[1],&offset);}
    if(!strcmp(ast_root->name,"MODULE")){op_list = getlist(ast_root->child_pointers[2],&offset);}
    func_entry* local = NULL;
    if(!strcmp(ast_root->name,"DRIVER")){
        local = func_tab_entry_add("DRIVER",global_func_table,ip_list,op_list,&offset);
        local_populate(local->func_root,ast_root->child_pointers[0]);
    }else{
        //Might need to add an overloading check?
        local = func_tab_entry_add(ast_root->child_pointers[0]->token->lexeme,global_func_table,ip_list,op_list,&offset);
        local_populate(local->func_root,ast_root->child_pointers[3]);
    }
}

void populate_copy(ast_node* ast_root,func_entry* global[]){
    if(!ast_root){
        return;
    }
    else if(!strcmp(ast_root->name,"DRIVER")){
        func_def_(ast_root,global);
    }
    else if(!strcmp(ast_root->name,"MODULE")){
        func_def_(ast_root,global);
        populate_copy(ast_root->next,global); 
    }
    else{
        int num = ast_root->no_of_children;
        for(int i=0;i<num;i++){
            populate_copy(ast_root->child_pointers[i],global);
        }
        populate_copy(ast_root->next,global);
        return;
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


type_exp* throw_error(semErrors error, int line)
{
    //check with enum 
    type_exp* gen_error=(type_exp*) malloc(sizeof(type_exp));
    switch(error)
    {
        case TYPE_NOT_MATCHED: {
        printf("Error found at line no %d : Type Mismatch \n", line);break;}
        case OUT_OF_SCOPE_VARIABLE: {
        printf("Error found at line no %d : Out of scope \n", line);break;}
        case UNSUPPORTED_DTYPE: {
        printf("Error found at line no %d : Unsupported Datatype \n", line);break;}
        case FUNC_NOT_DEFINED:{
        printf("Error found at line no %d : Function Not Defined \n", line);break;}
        case OUT_OF_ORDER_INDEX:
        {printf("Error found at line no %d : Index Out of Order \n", line);break;}
        case RECURSION_NOT_ALLOWED:
        {printf("Error found at line no %d : Recursion not allowed \n", line);break;}
        case PARAMETER_LIST_MISMATCH:
        {printf("Error found at line no %d : Parameter list mismatch \n", line);break;}
        case DEFAULT_NOT_FOUND:
        {printf("Error found at line no %d : Default not found \n", line);break;}
        case DEFAULT_FOUND:
        {printf("Error found at line no %d : Unexpected default found \n", line);break;}
         case INDEX_OUT_OF_BOUNDS:
        {printf("Error found at line no %d : Index out of bounds\n", line);break;}
        
    }
    return NULL;
}

type_exp* compare_dTypes(type_exp* left, type_exp* right, int line)
{   if(left!=NULL && right !=NULL){
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
                  else return throw_error(TYPE_NOT_MATCHED,line);
                }
                else return throw_error(TYPE_NOT_MATCHED,line); 
            }
            else return left;
        }
        else return throw_error(TYPE_NOT_MATCHED,line);

    }else return NULL;
}

type_exp* find_in_func_table(ast_node* ast_root, func_entry* curr,int line){
    //extract the lexeme out of ast_root
    char* key = ast_root->token->lexeme;
    //check if the module has any parameters or not :
  
    //check in the input list 
    sym_tab_entry* temp = curr->input_list;
    while(temp!=NULL){
        if(!strcmp(temp->name,key)){
            return &temp->type;
        }
        temp = temp->next;
    }   

    //check in the output list 
    temp = curr->ouput_list;
    while(temp!=NULL){
        if(!strcmp(temp->name,key)){
            return &temp->type;
        }
        temp = temp->next;
    }


    printf(" \"%s\" could not be found ::",key);

    return throw_error(OUT_OF_SCOPE_VARIABLE,line);
}


type_exp* find_in_table(char* key,var_record* table,int line){
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

type_exp* find_expr(ast_node* node, func_entry* curr,int line)
{
    if(!curr)
    {
        return throw_error(OUT_OF_SCOPE_VARIABLE,line);
    }
    var_record* current_rec = curr->func_curr;
    var_record* temp = current_rec;
    char * key=node->token->lexeme;
    type_exp* type=find_in_table(key, current_rec,line);
    if(type!=NULL)
        return type;
    else
    {
        curr->func_curr=curr->func_curr->parent; //also need to add for checking variable in the op list and ip list of the function record
        if(curr->func_curr==NULL){
            // just check in the function parameter list
            type = find_in_func_table(node,curr,line);
            curr->func_curr = temp;
            //return from here only
        }else{
        type = find_expr(node,curr,line);
        curr->func_curr = temp;
        }
        return type;
    }
}

int line_number_finder(ast_node* ast_root){

    if(ast_root&& ast_root->isTerminal){
        return ast_root->token->line_no;
    }else return line_number_finder(ast_root->child_pointers[0]);
}

// void check_cases_boolean(ast_node* node, func_entry* curr, type_exp* switch_dtype)
// {
    
// }

void check_cases(ast_node* node, func_entry* curr, type_exp* switch_dtype){

    if(!node){
        return;
    }

    type_exp* case_id = type_checking(node->child_pointers[0],curr);
    int line = node->child_pointers[0]->token->line_no;

    compare_dTypes(case_id,switch_dtype,line);
    // printf("--Case type mismatch with Switch type\n");
    
    perform_type_checking(node->child_pointers[1],curr);

    if(curr->func_curr->r_sibiling&&(!strcmp(curr->func_curr->r_sibiling->construct_name,"CASE")))
    {
    curr->func_curr = curr->func_curr->r_sibiling;
    check_cases(node->next,curr,switch_dtype);
    }
    else if(curr->func_curr->r_sibiling)
    {
    curr->func_curr->parent->child = curr->func_curr->r_sibiling;
    curr->func_curr = curr->func_curr->r_sibiling;
    }
    else
    {
        curr->func_curr = curr->func_curr->parent;
    }
}

void perform_type_matching_out(ast_node* actual, sym_tab_entry* formal, func_entry* curr, int line)
{
    while(actual&&formal)
    {
        //printf("%s \n",actual->child_pointers[0]->child_pointers[0]->token->lexeme);
       type_exp* act= find_expr(actual, curr, line);
       type_exp* form=&formal->type;
       compare_dTypes(act, form, line);
       actual=actual->next;
       formal=formal->next;
       //check if value is assigned throughout
    }
    if(actual||formal)
    {
        throw_error(PARAMETER_LIST_MISMATCH,line);
    }
}
void perform_type_matching_in(ast_node* actual, sym_tab_entry* formal, func_entry* curr, int line)
{
    while(actual&&formal)
    {
        type_exp* act;
        ast_node* temp;
        char* arr_access;
        //printf("%s \n",actual->child_pointers[0]->child_pointers[0]->token->lexeme);
       //ast_node* temp= actual->child_pointers[1]->child_pointers[0]?actual->child_pointers[1]->child_pointers[0]:actual->child_pointers[1]->child_pointers[1];
       if(actual->child_pointers[1]->isTerminal)
       {
            act=type_checking(actual->child_pointers[1], curr);
            arr_access=act->datatype;
            arr_access=act->datatype;
       }
       else
       {
            temp= actual->child_pointers[1]->child_pointers[0];
            act= find_expr(temp, curr, line);
            if(act)
            {
                arr_access=act->datatype;
                arr_access=act->datatype;
                if(actual->child_pointers[1]->child_pointers[1])
                {
                    act->datatype=act->arr_data->arr_datatype;
                }
            }
            else
            {
                throw_error(OUT_OF_SCOPE_VARIABLE, line);
                return;
            }
       }
       type_exp* form=&formal->type;
       compare_dTypes(act, form, line);
       act->datatype=arr_access;
       actual=actual->next;
       formal=formal->next;
    }
    if(actual||formal)
    {
        throw_error(PARAMETER_LIST_MISMATCH,line);
    }
}


type_exp* type_checking(ast_node* node, func_entry* curr)
{

    if(!node)
    return NULL;
    else if(!strcmp(node->name,"INPUT_ID"))
    {
        find_expr(node,curr, node->token->line_no);
    }
    else if(!strcmp(node->name,"ID"))
    {
        int line=node->token->line_no;
        type_exp* var_exp= find_expr(node, curr,line);
        return var_exp;
    }
    else if(!strcmp(node->name,"MODULEREUSE"))
    {
        // what to return
        int line=node->child_pointers[0]->token->line_no;
        func_entry* temp=find_module(node->child_pointers[0]->token->lexeme);
        if(!temp)
        {
            throw_error(FUNC_NOT_DEFINED,line);
            return NULL;
        }
        else if(temp==curr)
        {
            throw_error(RECURSION_NOT_ALLOWED,line);
            return NULL;
        }
        else
        {
            //handle the edge 4th case
            if(node->child_pointers[1])
            {
                if(temp->ouput_list)
                {
                 perform_type_matching_out(node->child_pointers[1],temp->ouput_list,curr,line);
                }
                else
                {
                 throw_error(PARAMETER_LIST_MISMATCH, line);
                }
            }
            else
            {

                //check proper use such that it is not present in an expression
            }

            if(node->child_pointers[2])
            {
                if(temp->input_list)
                {
                    perform_type_matching_in(node->child_pointers[2], temp->input_list,curr,line);
                }
                else
                {
                    throw_error(PARAMETER_LIST_MISMATCH, line);
                }
            }
        }
    }
    else if(!strcmp(node->name, "ARRAY")){
        type_exp* var_id = type_checking(node->child_pointers[0],curr);
        type_exp* index = type_checking(node->child_pointers[1],curr);
        int line = node->child_pointers[0]->token->line_no;

        if(var_id&& index){
            if(strcmp(var_id->datatype,"array")||strcmp(index->datatype,"integer")){
                printf("Array expression needs array variable\
                to be declared and index expression to consist of integer type\n");
                return throw_error(UNSUPPORTED_DTYPE,line);
            }
        }else if(var_id){
            //Convert this into throw_error()
            printf("Error line no %d Array index expression couldn't be found under scope\n",line);
        }
        return NULL;

    }
    else if(!strcmp(node->name, "OUTPUT")){
        type_exp* var = type_checking(node->child_pointers[0],curr);
        int line = line_number_finder(node);
        if(var){
            if(!strcmp(var->datatype,"array")){
                printf("Array ID variable can't be printed ::");
                return throw_error(UNSUPPORTED_DTYPE,line);
            }
        }

    }
    else if(!strcmp(node->name,"ARRAY_ASSIGN")){
        type_exp* arr_data = type_checking(node->child_pointers[0],curr);
        type_exp* arr_index = type_checking(node->child_pointers[1],curr);
        if( arr_data&&arr_index 
            && !strcmp(arr_data->datatype,"array")
            && !strcmp(arr_index->datatype,"integer")){
               //I think error here, because we should return the inside element type rather than array
                type_exp* temp=(type_exp*)malloc(sizeof(type_exp));
                temp->datatype=arr_data->arr_data->arr_datatype;
                return arr_data;
        }
    }
    else if(!strcmp(node->name,"LT_result")||!strcmp(node->name,"LE_result")){
        type_exp* op1 = type_checking(node->child_pointers[0],curr);
        type_exp* op2 = type_checking(node->child_pointers[1],curr);
        int line=line_number_finder(node);
        // int line=node->child_pointers[0]->token->line_no;//Might need to check child's line number
        type_exp* compare = compare_dTypes(op1,op2,line);
            if(!compare)
            return NULL;
            if(op1&&op2&&strcmp(op1->datatype,"array")&& strcmp(op2->datatype,"array")
                &&compare){
                type_exp* temp = (type_exp*) malloc(sizeof(type_exp));
                temp->datatype = "boolean";
                return temp;
            }else{
                printf("\nError found at line number %d: expected boolean operation",
                node->child_pointers[0]->token->line_no);
                return throw_error(UNSUPPORTED_DTYPE,line);
            }
    }
        else if(!strcmp(node->name,"GT_result")||!strcmp(node->name,"GE_result")){
        type_exp* op1 = type_checking(node->child_pointers[0],curr);
        type_exp* op2 = type_checking(node->child_pointers[1],curr);
        int line=line_number_finder(node);
        // int line=node->child_pointers[0]->token->line_no;//Might need to check child's line number
        type_exp* compare = compare_dTypes(op1,op2,line);

        if(op1&&op2&&strcmp(op1->datatype,"array")&& strcmp(op2->datatype,"array")
          &&compare){
            type_exp* temp = (type_exp*) malloc(sizeof(type_exp));
            temp->datatype = "boolean";
            return temp;
        }else{
            return compare;
        }
    }
        else if(!strcmp(node->name,"EQ_result")||!strcmp(node->name,"NE_result")){
        type_exp* op1 = type_checking(node->child_pointers[0],curr);
        type_exp* op2 = type_checking(node->child_pointers[1],curr);
        int line=line_number_finder(node);//Might need to check child's line number
        type_exp* compare = compare_dTypes(op1,op2,line);

        if(op1&&op2&&strcmp(op1->datatype,"array")&& strcmp(op2->datatype,"array")
          &&compare){
            type_exp* temp = (type_exp*) malloc(sizeof(type_exp));
            temp->datatype = "boolean";
            return temp;
        }else{
           return compare;
        }
    }
        else if(!strcmp(node->name,"AND")||!(strcmp(node->name,"OR"))){
        type_exp* op1 = type_checking(node->child_pointers[0],curr);
        type_exp* op2 = type_checking(node->child_pointers[1],curr);
        int line=line_number_finder(node);

        type_exp* compare = compare_dTypes(op1,op2,line);

        if(compare&&op1&&op2&&strcmp(op1->datatype,"array")&& strcmp(op2->datatype,"array")        ){
            type_exp* temp = (type_exp*) malloc(sizeof(type_exp));
            temp->datatype = "boolean";
            node->type = temp->datatype;
            return temp;
        }else{
            return compare;
        }
    }
    else if(!strcmp(node->name,"ASSIGN"))
    {
        type_exp* left =type_checking(node->child_pointers[0],curr);
        type_exp* right=type_checking(node->child_pointers[1],curr);
        int line;
        type_exp* ret;
        if(left&&strcmp(left->datatype,"array")){
        line=line_number_finder(node);
        ret=compare_dTypes(left, right,line);}//Might need to check child's line number
        else{line=line_number_finder(node);
        if(left && right && !strcmp(left->arr_data->arr_datatype,right->datatype));
        return NULL;
        }
        return ret;
    }
    else if(!strcmp(node->name,"NUM"))
    {
        type_exp* temp =(type_exp*) malloc(sizeof(type_exp));
        temp->datatype="integer";
        temp->is_static=1;
        return temp;
    }
    else if(!strcmp(node->name,"RNUM"))
    {
        type_exp* temp =(type_exp*) malloc(sizeof(type_exp));
        temp->datatype="real";
        temp->is_static=1;
        return temp;
    }
    else if(!strcmp(node->name,"TRUE")||!strcmp(node->name,"FALSE"))
    {
        type_exp* temp =(type_exp*) malloc(sizeof(type_exp));
        temp->datatype="boolean";
        return temp;
    }else if(!strcmp(node->name,"UNARY")){
        int line=node->child_pointers[0]->token->line_no;
        
        //CHILD_POINTER[0] just has a plus or minus sign no need to call anything
        if(!node->child_pointers[0]) return throw_error(UNSUPPORTED_DTYPE,node->token->line_no);
        //CHILD_POINTER[1] overall type can be a num, rnum, integer, real or boolean
        type_exp* temp = type_checking(node->child_pointers[1],curr);
        //(only primitives)
        if(temp&&!strcmp(temp->datatype,"array")){
             int line=node->child_pointers[0]->token->line_no;
             return throw_error(UNSUPPORTED_DTYPE, line);}
        else if(temp&&(!strcmp(temp->datatype,"integer")||
                !strcmp(temp->datatype,"real"))/* ||
                do we need to add other datatypes ?*/){return temp;}
        return throw_error(UNSUPPORTED_DTYPE,line);
    }
    else if(!strcmp(node->name, "ARRAY_ACCESS")){
        //CHILD_POINTER[0] will always be ID of the array (need to check it's type)
        type_exp* var_id = type_checking(node->child_pointers[0],curr);
        //CHILD_POINTER[1] will always give an expression, overall type could be 
        type_exp* arr_expr = type_checking(node->child_pointers[1],curr);
        int line=node->child_pointers[0]->token->line_no;
        if(var_id&&arr_expr&&!strcmp(arr_expr->datatype,"integer")){
            if(!strcmp(var_id->datatype,"array")){
                type_exp* temp =(type_exp*) malloc(sizeof(type_exp));
                temp->datatype = var_id->arr_data->arr_datatype;

                if(arr_expr->is_static)
                {
                    ast_node* index=node->child_pointers[1];
                    if(index)
                    {
                        int val=index->token?index->token->values.num:index->child_pointers[1]->token->values.num;
                        //write a case for handling negative numbers
                        if(!(val<var_id->arr_data->upper_bound&&val>var_id->arr_data->lower_bound))
                        return throw_error(INDEX_OUT_OF_BOUNDS, line);
                    }
                }
                return temp;
            }else {printf("ID is not of array datatype"); throw_error(UNSUPPORTED_DTYPE, line);}
        }else if(!var_id)
        {
            return NULL;
        }
        else
        {printf("Array index expression only supports integer type"); throw_error(UNSUPPORTED_DTYPE, line);}
        //num or integer
        // int line=node->child_pointers[0]->token->line_no;//Might need to check child's line number

        return throw_error(UNSUPPORTED_DTYPE,line);
        //to return only the type of ID
    }
    else if(!strcmp(node->name, "SIGNED_NUMBER"))
    {
        type_exp* num=type_checking(node->child_pointers[1],curr);
        // type_exp* sign=type_checking(node->child_pointers[1],line);
        int line=node->child_pointers[1]->token->line_no;
        if(num&&strcmp(num->datatype,"integer"))
        return throw_error(UNSUPPORTED_DTYPE,line);
        else return num;
    }
    else if(!strcmp(node->name, "PLUS")||!strcmp(node->name, "MINUS")||!strcmp(node->name, "MUL")||!strcmp(node->name,"DIV"))
    {
        if(node->no_of_children!=0){  
            //to handle non unary expression 
            type_exp* left =type_checking(node->child_pointers[0],curr);
            type_exp* right=type_checking(node->child_pointers[1],curr);
            int line=line_number_finder(node);//Might need to check child's line number
            //for PLUS operator need not be always line number tractable
            type_exp* ret=compare_dTypes(left, right,line);
            if(ret&&(!strcmp(ret->datatype,"boolean")||!strcmp(ret->datatype,"array")))
            {
                throw_error(UNSUPPORTED_DTYPE, line);
                return NULL;
            }
            if(!strcmp(node->name, "DIV"))//Check this case. Added later
            {
                type_exp* real=(type_exp*)malloc(sizeof(type_exp));
                real->datatype="real";
                return real;
            }
        if(ret)node->type = ret->datatype;
        return ret;
        }
     }
    else if(!strcmp(node->name, "FORINDEX"))
    {
        // check index types
        int line=node->child_pointers[1]->token->line_no;
        if(node->child_pointers[0]!=NULL&&(
        !strcmp(node->child_pointers[0]->name, "MINUS")||
        !strcmp(node->child_pointers[0]->name,"PLUS")))
        return type_checking(node->child_pointers[1],curr);
        else if(!strcmp(node->child_pointers[1]->name,"NUM"))//Is this right fosho??
        return type_checking(node->child_pointers[1],curr);
        return throw_error(UNSUPPORTED_DTYPE,line);
    } 
    else if(!strcmp(node->name, "FORRANGE"))
    {
        //check increasing order
        int line = node->child_pointers[0]->child_pointers[1]->token->line_no;
        type_exp* index1 = type_checking(node->child_pointers[0],curr);
        type_exp* index2 = type_checking(node->child_pointers[1],curr);
        //check for error
        if(!index1||!index2)
        return NULL;
        //TODO static array check and (check bas)
        int val1 = node->child_pointers[0]->child_pointers[1]->token->values.num;
        int val2 = node->child_pointers[1]->child_pointers[1]->token->values.num;
        if(val1>val2)
        return throw_error(OUT_OF_ORDER_INDEX,line);
        else return NULL;
    }
    else if(!strcmp(node->name, "FORLOOP"))
    {
        //change the current variable record
        var_record* temp= curr->func_curr;
        curr->func_curr=curr->func_curr->child;
        //TO CALL PERFORM TYPE CHECKING ON THEIR STATEMENTS CHILD
        type_exp* id_type = type_checking(node->child_pointers[0],curr);
        //special case to be added when we dont want to change the for variable
        type_exp* range_type = type_checking(node->child_pointers[1],curr);
        perform_type_checking(node->child_pointers[2],curr);
        curr->func_curr=temp;
        curr->func_curr->child = curr->func_curr->child->r_sibiling;
    }
    else if(!strcmp(node->name, "WHILELOOP"))
    {
        var_record* temp= curr->func_curr;
        curr->func_curr=curr->func_curr->child;
        //TO CALL PERFORM TYPE CHECKING ON THEIR STATEMENTS CHILD
        type_exp* condition = type_checking(node->child_pointers[0],curr);
        if(!strcmp(node->child_pointers[0]->name,"ID")
           && strcmp(condition->datatype,"boolean")){
            printf("Error found at line no: %d expected boolean expression",
            node->child_pointers[0]->token->line_no);
           }
        perform_type_checking(node->child_pointers[1],curr);
        curr->func_curr=temp;
        curr->func_curr->child = curr->func_curr->child->r_sibiling;
    }
    else if(!strcmp(node->name, "SWITCH")){
        var_record* temp= curr->func_curr;
        int line = node->child_pointers[0]->token->line_no;
        type_exp* switch_id = type_checking(node->child_pointers[0],curr);

        curr->func_curr=curr->func_curr->child;
        if(switch_id)check_cases(node->child_pointers[1],curr,switch_id);
        
        if(switch_id&&!strcmp(switch_id->datatype,"integer"))
        {
            if(!node->child_pointers[2])
            {
                //need to add check for children's line number
                printf("Switch variable of type integer");
                throw_error(DEFAULT_NOT_FOUND,line);
            }
            //if default doesnt exist then error
        }
        else if(switch_id&&!strcmp(switch_id->datatype,"boolean"))
        {
            if(node->child_pointers[2])
            {
                 printf("Switch variable of type boolean. ");
                throw_error(DEFAULT_FOUND,line);
                
            }
        }
        else if(switch_id&&!strcmp(switch_id->datatype,"real"))
        {
            throw_error(UNSUPPORTED_DTYPE, line);
            return NULL;
        }
        type_checking(node->child_pointers[2],curr);

        if(node->child_pointers[2]&&curr->func_curr->r_sibiling){
        curr->func_curr->parent->child = curr->func_curr->r_sibiling;
        curr->func_curr = temp;
        }else if(curr->func_curr->r_sibiling){
        curr->func_curr = temp;
        }
        else {curr->func_curr = temp;}
        // type_exp* compare;
        // if(case_ids)compare = compare_dTypes(switch_id,case_ids,line);
        // return compare;
    }
    else if(!strcmp(node->name,"DEFAULTCASE")){
        perform_type_checking(node->child_pointers[0],curr);
    }
    else return NULL;
}

func_entry* find_module_global(char* key){
    int index = func_tab_entry_contains(key, global_TABLE);
    if(index==-1)return NULL;
    func_entry* function = global_TABLE[index];

    while(function!=NULL){      //HASH COLLISION
        if(!strcmp(function->name,key)){
            return function;
        }
        function = function->next;
    }
    return NULL; 
}


func_entry* find_module(char* key){
    int index = func_tab_entry_contains(key, global_func_table);
    if(index==-1)return NULL;
    func_entry* function = global_func_table[index];

    while(function!=NULL){      //HASH COLLISION
        if(!strcmp(function->name,key)){
            return function;
        }
        function = function->next;
    }
    return NULL; 
}


void perform_type_checking(ast_node* ast_root,func_entry* func){

    if(ast_root==NULL) {return ;}
    
    else if(!strcmp(ast_root->name,"DRIVER")){
        func = find_module("DRIVER");
    }
    else if(!strcmp(ast_root->name,"MODULE")){
        func = find_module(ast_root->child_pointers[0]->token->lexeme);
        if(func==NULL){
        printf("\nError found at line no %d : Function Not Defined",ast_root->child_pointers[0]->token->line_no);
        }else func->visited = 1;
    }
    else if(!strcmp(ast_root->name,"STATEMENTS")){
        while(ast_root->next!=NULL){
        type_checking(ast_root->next,func);
        ast_root = ast_root->next;
        }
        // type_checking(ast_root,func);
        return;
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
    initialize_global_func_table();
    //get_global_symbol_table(ast_root);
    populate_(ast_root);
    perform_type_checking(ast_root,NULL);
    
    //perform bound checking
}


void number(var_record* node, char* reach){

if(node&&node->parent==NULL){
    char* temp = (char*) malloc(sizeof(char)*3);
    memset(temp,'\0',sizeof(temp));
    strcpy(temp,reach);
    node->reach = temp;
    number( node->child,strcat(reach,"d")); //direction : 0 go down
}else if(node){
    char* temp = (char*) malloc(sizeof(char)*100);
    memset(temp,'\0',sizeof(temp));
    strcpy(temp,reach);
    node->reach = temp;
    number(node->child,strcat(reach,"d"));
    int n = strlen(reach);
    reach[n-1] = '\0';
    number(node->r_sibiling,strcat(reach,"r"));
    n = strlen(reach);
    reach[n-1] = '\0';
}else return;
}

void get_global_symbol_table(ast_node* ast_root){

    init_global(global_TABLE);

    populate_copy(ast_root,global_TABLE); 
    
    for(int i=0;i<TABLE_SIZE;i++){
        if(global_TABLE[i]){
            char* reach = (char*)malloc(sizeof(char)*100);
            memset(reach,'\0',sizeof(reach));
            number(global_TABLE[i]->func_root,reach);
        }
    }

}

var_record* find_local_construct(char* func_name, char* arr){

    func_entry* function = find_module_global(func_name);

    var_record* temp = function->func_root;

    int n = strlen(arr);

    for(int i=0;i<n;i++){
        if(arr[i]=='d'){
            temp = temp->child;
        }else if(arr[i]=='r'){
            temp = temp->r_sibiling;
        }
    }
    
    return temp;
}

// function call (after defiinition or declaration only) check ??
// offset computation needs to be corrected (KIYA KYA PEHLE SE) ??
