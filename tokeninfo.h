#include <stdlib.h>

typedef enum token_type{ID,NUM,RNUM,
AND,OR,TRUE,FALSE,
PLUS,MINUS,MUL,DIV,
LT,LE,GE,GT,EQ,NE,
DEF,ENDDEF,DRIVERDEF,DRIVERENDDEF,
COLON,RANGEOP,SEMICOL,COMMA,
ASSIGNOP,SQBO,SQBC,BO,BC,
COMMENTMARK,
INTEGER,REAL,BOOLEAN,OF,
ARRAY,START,END,END,DECLARE,
MODULE,DRIVER,PROGRAM,GET_VALUE,
PRINT,USE,WITH,PARAMETERS,TAKES,
INPUT,RETURNS,FOR,IN,SWITCH,
CASE,BREAK,DEFAULT,WHILE}token_type;

typedef union data{
    int num;
    float rnum;
}data;

typedef struct token_info{
    token_type type;
    int line_no;
    char* lexeme[20];
    data values;
}token_info;

