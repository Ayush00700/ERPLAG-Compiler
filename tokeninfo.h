#include <stdlib.h>
#include <string.h>

#define MAX_LEN 20
#define TABLE_SIZE 101


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


typedef union data{
    int num;
    double rnum;
}data;

typedef struct token_info{
    char type[20];
    int line_no;
    char* lexeme;
    data values;
}token_info;

typedef struct token_node{
    token_info* token;
    struct token_node* next_token; 
}token_node;

