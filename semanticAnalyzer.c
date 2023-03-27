#include "semanticAnalyzer.h"
#include <string.h>

int offset = 0;
#define INT_OFFSET 4 
#define REAL_OFFSET 8
#define BOOL_OFFSET 1
#define POINTER_OFFSET 4


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
            if(!strcmp(temp.arr_datatype,"real")){x=REAL_OFFSET;}
            else if(!strcmp(temp.datatype,"boolean")){x=BOOL_OFFSET;}
            else{x=INT_OFFSET;}
            int l = temp.lower_bound;
            int u = temp.upper_bound;
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
        temp.arr_datatype =  ast_root->child_pointers[1]->child_pointers[1]->token->lexeme;
        int lower = check_static(ast_root->child_pointers[1]->child_pointers[0]->child_pointers[0]);
        int upper = check_static(ast_root->child_pointers[1]->child_pointers[0]->child_pointers[1]);
        if(lower&&upper){
            temp.lower_bound = index_finder(ast_root->child_pointers[1]->child_pointers[0]->child_pointers[0]);
            temp.upper_bound = index_finder(ast_root->child_pointers[1]->child_pointers[0]->child_pointers[1]);
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

void populate(ast_node* ast_root){
    if(!ast_root){
        return;
    }
    else if(!strcmp(ast_root->name,"DECLARE")){
        compute_expression(ast_root);
    }
    else if(ast_root->isTerminal){
        populate(ast_root->next);
        return;
    }
    else{
        int num = ast_root->no_of_children;
        for(int i=0;i<num;i++){
            populate(ast_root->child_pointers[i]);
        }
        return;
    }
}

void semantic(){
    ast_node* ast_root = get_ast_root();
    // iterate through the for loop : 

    populate(ast_root);

}