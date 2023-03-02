#include "lexerDef.h"
#include <stdlib.h>
#include <stdio.h>
/* Group-20
--------------------
1. Rajan Sahu       2019B4A70572P
2. Yash Goyal       2019B4A70638P
3. Ayush Agarwal    2019B4A70652P
4. Vasu Swaroop     2019B4A70656P
5. A Sudarshan      2019B4A70744P
*/
#ifndef LEXER_H
#define LEXER_H

// Rest of header file contents go here



/* FUNCTION DECLARATIONS 
----------------------------*/
// Function to print token number, type and the corresponding lexeme
extern void printtokens();
// Function to initialize next pointer for get_next_token()
extern void initialize();
// Function returns the next valid token to the parser upon request
extern token_info* get_next_token();
// Function to populate the buffer with the data from file to be tokenized
extern void populate(char* buffer,FILE* fp,int bufsize);
// Function to add a lexical error entry into the stack
extern void add_error_token(token_info* tk);
// Function to pop an element from stack to display the error
extern void pop_error_tokens();
// Function to create a hash table entry to map the token to the lexeme
extern hash_table_contents* create_hash_table_content(char* lexeme, char* tk);
// Function to hash the given lexeme
extern size_t _hash(char *lexeme);
// Function to initialize the hash table with NULL entries
extern void init_hash_table();
// Function to print the updated hash table
extern void print_hash_table();
// Function to find index in hash table at which insertion is to be done
extern int hash_table_insert(char* lexeme, char* tk);
// Function takes an input lexeme and returns the corresponding mapped value from the hash table
extern void populate_hash_table();
/*This function populates the hash table to map the keywords lower case keywords to the ones in upper case*/
extern char* lookup(char* lexeme);
// Function to traverse the buffer(s) and tokenize according to the message
extern void copy2lexeme(int f1,int f2,int b1,int b2,char* message,char* buf1,char* buf2,int bufsize);
// This function gives the cue for Lexical Analysis
extern void call_lexer(FILE* fp,int bufsize);

extern void removeComments(char *testcaseFile, char *cleanFile);

extern void postProcessing();

#endif
