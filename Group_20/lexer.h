#include "lexerDef.h"

/* FUNCTION DECLARATIONS */
// Function to print token number, type and the corresponding lexeme
void printtokens();
// Function to initialize next pointer for get_next_token()
void initialize();
// Function returns the next valid token to the parser upon request
token_info* get_next_token();
// Function to populate the buffer with the data from file to be tokenized
void populate(char* buffer,FILE* fp,int bufsize);
// Function to add a lexical error entry into the stack
void add_error_token(token_info* tk);
// This function pops an element from stack to display the error
void pop_error_tokens();
// Function to create a hash table entry to map the token to the lexeme
hash_table_contents* create_hash_table_content(char* lexeme, char* tk);







