/* Group-20
--------------------
1. Rajan Sahu       2019B4A70572P
2. Yash Goyal       2019B4A70638P
3. Ayush Agarwal    2019B4A70652P
4. Vasu Swaroop     2019B4A70656P
5. A Sudarshan      2019B4A70744P
*/
#ifndef LEXERDEF_H
#define LEXERDEF_H

#define MAX_LEN 20
#define TABLE_SIZE 101
#define TOTAL_KEYWORDS 30


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

#endif
