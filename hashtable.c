#include <stdlib.h>
#include "tokeninfo.h"
#include "string.h"
#include "stdio.h"

#define MAX_LEN 20
#define TABLE_SIZE 94


char* arr_keywords[26] = {
"integer","real","boolean","of",
"array","start","end","declare",
"module","driver","program","get_value",
"print","use","with","parameters","takes",
"input","returns","for","in","switch",
"case","break","default","while"
};

char* arr_keywords_upper[26] = {
"INTEGER","REAL","BOOLEAN","OF",
"ARRAY","START","END","DECLARE",
"MODULE","DRIVER","PROGRAM","GET_VALUE",
"PRINT","USE","WITH","PARAMETERS","TAKES",
"INPUT","RETURNS","FOR","IN","SWITCH",
"CASE","BREAK","DEFAULT","WHILE"
};


typedef struct hash_table_contents{

char* lexeme ;
char* tk_type;

}hash_table_contents;

hash_table_contents* hash_table[TABLE_SIZE];

hash_table_contents* create_hash_table_content(char* lexeme, char* tk){
    hash_table_contents* h = (hash_table_contents*)malloc(sizeof(hash_table_contents));
    // h->lexeme = lexeme;h->tk_type = (token_type)hash(lexeme);
    h->lexeme = lexeme;
    h->tk_type = tk;
    return h;
}


size_t hash(char *lexeme){

    int length = strnlen(lexeme, MAX_LEN);
    size_t  hash_value = 0;
    for (int i =0;i<length;i++){
        hash_value += lexeme[i];
        hash_value = (hash_value*lexeme[i])%97;
    }

    return hash_value;
}

void init_hash_table(){
    for(int i=0;i<TABLE_SIZE;i++){
        hash_table[i] = NULL;
    }
}

void print_hash_table(){
    for (int i=0;i<TABLE_SIZE;i++){
        if(hash_table[i]==NULL){
            printf("\t%d\t---\n",i);
        }else {
            printf("\t%i\t%s\t%s\n", i ,hash_table[i]->lexeme, hash_table[i]->tk_type);
        }
    }
}


int hash_table_insert(char* lexeme, char* tk){
    if(lexeme==NULL) return 0;
    int index = hash(lexeme);
    if(hash_table[index]!=NULL){
        printf("Element already existing at index %d\n",index);
        return 1;
    }else{
    hash_table_contents* h = create_hash_table_content(lexeme, tk);
    hash_table[index] = h;
    return index;
    }
}

void populate_hash_table(){
    init_hash_table();
    for(int i=0;i<26;i++){

        hash_table_insert(arr_keywords[i],arr_keywords_upper[i]);
    
    }
}

char* lookup(char* lexeme){
    size_t hash_value = hash(lexeme);

    if(hash_table[hash_value]!=NULL){
        printf("Found lexeme : %s\n", hash_table[hash_value]->tk_type);
        return hash_table[hash_value]->tk_type;
    }else {
        printf("not found\n");
        return "ID";
    }
}

int main(){

    populate_hash_table();
    print_hash_table();

    char* s = lookup("int");
    printf("%s\n",s);
}