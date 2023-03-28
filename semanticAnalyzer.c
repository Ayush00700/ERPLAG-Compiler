#include "semanticAnalyzer.h"
#include <string.h>


#define INT_OFFSET 4 
#define REAL_OFFSET 8
#define BOOL_OFFSET 1
#define POINTER_OFFSET 4

func_entry* global_func_table[TABLE_SIZE];
sym_tab_entry* global_symbol_table[TABLE_SIZE];
// ----------------------------------------------HELPER FUNCTIONS FOR SET OPERATIONS ----------------------------------------------//
int sym_tab_entry_add(char* key,sym_tab_entry* table[],type_exp temp)
/*This function adds the key into the table along with the entry number
0 => need not be added since it already exists
1 => needs to be added*/
{
    // Hash the key to get the index
    unsigned int index = hash(key);
    sym_tab_entry* current = table[index];

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
    new_node -> next = table[index];
    table[index] = new_node;
    new_node->type = temp;
    new_node->offset = offset;
    if(!strcmp(temp.datatype,"integer")){
        offset += INT_OFFSET;
    }
    else if(!strcmp(temp.datatype,"real")){
        offset += REAL_OFFSET;
    }
    else if(!strcmp(temp.datatype,"boolean")){
        offset += BOOL_OFFSET;
    }
    else if(!strcmp(temp.datatype,"array")){
        if(temp.is_static){
            int x;
            if(!strcmp(temp.arr_data->arr_datatype,"real")){x=REAL_OFFSET;}
            else if(!strcmp(temp.arr_data->arr_datatype,"boolean")){x=BOOL_OFFSET;}
            else{x=INT_OFFSET;}
            int l = temp.arr_data->lower_bound;
            int u = temp.arr_data->upper_bound;
            offset += (u-l+1)*x;
        }
        else{
            offset += POINTER_OFFSET;
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

func_entry* func_tab_entry_add(char* key,func_entry* table[],sym_tab_entry* input_list,sym_tab_entry* ouput_list)
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
    new_node->func_curr = new_node->func_root;
    new_node->offset = ouput_list->offset;
    new_node->func_root->offset = new_node->offset;
    return new_node;
}

void list_add(sym_tab_entry* list,ast_node* ast_root,int* offset){
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
    if(list == NULL){
        list = (sym_tab_entry*) malloc(sizeof(sym_tab_entry));
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
        list->name = ast_root->child_pointers[0]->token->lexeme;
        list->next = NULL;
        list->type = temp;
        list->offset = *offset;
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
            return 1;
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

void populate_symbol_table(ast_node* temp_node,type_exp temp){
    int a = sym_tab_entry_add(temp_node->token->lexeme,global_symbol_table,temp);
    if(a == -1){ // to change as the abstraction is increased
        printf("already present in the symbol table\n");
    }
}

void compute_expression(ast_node* ast_root){
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
        populate_symbol_table(temp_node,temp);
        temp_node = temp_node->next;
    }
    return;
}

sym_tab_entry* getlist(ast_node* ast_root,int* offset){
    sym_tab_entry* list_head; 
    ast_node* temp_node = ast_root;
    while(temp_node!=NULL){
        sym_tab_entry* temp = list_head; 
        list_add(temp,temp_node,offset);
        temp_node = temp_node->next;
    }
    return list_head;
}

void func_def(ast_node* ast_root){
    int offset = 0;
    sym_tab_entry* ip_list = getlist(ast_root->child_pointers[1],&offset);
    sym_tab_entry* op_list = getlist(ast_root->child_pointers[2],&offset);
    func_entry* local = func_tab_entry_add(ast_root->child_pointers[0]->token->lexeme,global_func_table,ip_list,op_list);
    local_populate(local->func_root,ast_root->child_pointers[3]);
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
    // else if(!strcmp(ast_root->name,"DECLARE")){
    //     compute_expression(ast_root);
    //     populate_(ast_root->next);
    // }
    // else if(ast_root->isTerminal){
    //     populate_(ast_root->next);
    //     return;
    // }
    else{
        int num = ast_root->no_of_children;
        for(int i=0;i<num;i++){
            populate_(ast_root->child_pointers[i]);
        }
        populate_(ast_root->next);
        return;
    }
}

void semantic(){
    ast_node* ast_root = get_ast_root();


    populate_(ast_root);

}