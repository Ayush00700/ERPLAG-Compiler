/* THE MATH_CS boiz
--------------------
1. Rajan Sahu       2019B4A70572P
2. Yash Goyal       2019B4A70638P
3. Ayush Agarwal    2019B4A70652P
4. Vasu Swaroop     2019B4A70656P
5. A Sudarshan      2019B4A70744P
*/

#define MAX_LEN 20
#define TABLE_SIZE 101
#define TOTAL_KEYWORDS 30

char* arr_keywords[30] = {
"integer","real","boolean","of",
"array","start","end","declare",
"module","driver","program","get_value",
"print","use","with","parameters","takes",
"input","returns","for","in","switch",
"case","break","default","while","true","false","AND","OR"
};

char* arr_keywords_upper[30] = {
"INTEGER","REAL","BOOLEAN","OF",
"ARRAY","START","END","DECLARE",
"MODULE","DRIVER","PROGRAM","GET_VALUE",
"PRINT","USE","WITH","PARAMETERS","TAKES",
"INPUT","RETURNS","FOR","IN","SWITCH",
"CASE","BREAK","DEFAULT","WHILE","TRUE","FALSE","AND","OR"
};

/* REQUIRED STRUCTURES FOR THE LEXICAL ANALYZER MODULE */
typedef struct hash_table_contents{
    char* lexeme ;
    char* tk_type;
    struct hash_table_contents* next;
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

/* IMPORTANT GLOBAL VARIABLES FOR LEXER MODULE */
int populate_count = 1;
int line_no = 1;
token_node* ll_node; //LINKED-LIST 
token_node* stack_head; //STACK FOR ERRORS 
token_node* current; //LAST NODE THAT WAS ADDED
token_node* get_next_curr;
hash_table_contents* hash_table[TABLE_SIZE];
