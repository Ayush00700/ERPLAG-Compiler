#include <stdlib.h>

int arr_keywords_size = 58;

char* arr_keywords[58] = {
"ERROR","ID","NUM","RNUM",
"AND","OR","TRUE","FALSE",
"PLUS","MINUS","MUL","DIV",
"LT","LE","GE","GT","EQ","NE",
"DEF","ENDDEF","DRIVERDEF","DRIVERENDDEF",
"COLON","RANGEOP","SEMICOL","COMMA",
"ASSIGNOP","SQBO","SQBC","BO","BC",
"COMMENTMARK",
"integer","real","boolean","of",
"array","start","end","declare",
"module","driver","program","get_value",
"print","use","with","parameters","takes",
"input","returns","for","in","switch",
"case","break","default","while"
};

typedef enum token_type{ //TODO Quotation marks
ERROR,ID,NUM,RNUM,
AND,OR,TRUE,FALSE,
PLUS,MINUS,MUL,DIV,
LT,LE,GE,GT,EQ,NE,
DEF,ENDDEF,DRIVERDEF,DRIVERENDDEF,
COLON,RANGEOP,SEMICOL,COMMA,
ASSIGNOP,SQBO,SQBC,BO,BC,
COMMENTMARK,
INTEGER,REAL,BOOLEAN,OF,
ARRAY,START,END,DECLARE,
MODULE,DRIVER,PROGRAM,GET_VALUE,
PRINT,USE,WITH,PARAMETERS,TAKES,
INPUT,RETURNS,FOR,IN,SWITCH,
CASE,BREAK,DEFAULT,WHILE
}token_type;

typedef union data{
    int num;
    double rnum;
}data;

typedef struct token_info{
    token_type type;
    int line_no;
    char* lexeme;
    data values;
}token_info;

typedef struct token_node{
    token_info* token;
    struct token_node* next_token; 
}token_node;

