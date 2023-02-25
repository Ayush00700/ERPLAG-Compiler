#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define TABLE_SIZE 100

typedef struct entry{
    char* key;
    unsigned int entry_number;
    struct entry* next;
}entry;

entry* table[TABLE_SIZE];

unsigned int hash(char* key){
    unsigned int hashval = 0;
    for(;*key !='\0';key++){
        hashval = *key + 31*hashval;
    }
    return hashval%TABLE_SIZE;
}

int set_add(char* key){
    unsigned int index = hash(key);
    entry* current = table[index];

    while(current!=NULL){
        if(strcmp(current->key,key)==0){
            printf("Element already present!\n");
            return 0;
        }
        current = current->next;
    }

    entry* new_node = (entry*)malloc(sizeof(entry));
    new_node -> key = key;
    new_node -> next = table[index];
    new_node->entry_number = index;
    table[index] = new_node;
    return 1;

}

int set_contains(char* key){
    unsigned int index = hash(key);
    entry* current = table[index];

    while(current!=NULL){
        if(strcmp(current->key,key)==0){
            return current->entry_number;
        }
        current = current->next;
    }

    return 0;

}

int main(){
    set_add("non_terminal1");
    set_add("non_terminal2");
    set_add("apple");
    set_add("real_apple");
    set_add("non_terminal3");
    set_add("non_terminal3");

     for (int i=0;i<TABLE_SIZE;i++){
        if(table[i]==NULL){
            printf("\t%d\t---\n",i);
        }else {
            printf("\t%i\t%s\t%d\n", i ,table[i]->key,table[i]->entry_number);
        }
    }


    
}