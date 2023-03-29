/* Group-20
--------------------
1. Rajan Sahu       2019B4A70572P
2. Yash Goyal       2019B4A70638P
3. Ayush Agarwal    2019B4A70652P
4. Vasu Swaroop     2019B4A70656P
5. A Sudarshan      2019B4A70744P
*/

#ifndef PARSER_H
#define PARSER_H

#include "parserDef.h"
#include <stdlib.h>
#include <stdio.h>
#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"
#define RESET "\x1B[0m"

/* FUNCTION DECLARATIONS 
----------------------------*/
// Stack Operations
// Function to push element into the stack
extern void push(Stack* parseStack, ruleNode* element);
// Function to pop the top of the stack
extern ruleNode* pop(Stack* parseStack);
// Function to see what's at the top of the stack
extern ruleNode* top(Stack* parseStack);
// Function to check if the stack is empty or not
extern int isEmptyStack(Stack* parseStack);
// Function to push each node of rule following Left-most derivation
extern void addRecurse(Stack* stack, ruleNode* curr);
// Function to add Rule into the stack
extern void addNodesToStack(Stack* stack, rule* Rule);
// Function to print the updated stack
extern void printStack(Stack* parseStack);

// Tree Operations
// Function to create a parse tree node ready to be added
extern treeNodes* convertToPTreenode(ruleNode* Node);
// Function to create and initialize parse tree
extern void createParseTree(ruleNode* prog);
// Function to set fields of the tree node using token
extern void addTokenInfo( token_info* temp );
// Function to hash a string
extern unsigned int hash(char* key);
// Function to print any table in the required format
extern void print_tables(entry* table[]);
// Function to check if the parent of the node has a sibling or not
extern int checkUncleExists();
// Function to go to right sibling node
extern void goToSibling();
// Function to go to right node: either right sibling, if exists, else uncle
extern void goToNextRight();
// Function to go to the parent node
extern void goToParent();
// Function to go to the child node
extern void goToChild();
// Function to add the "rule" to the parse tree
extern void addRuleToTree(rule* rule);
// Function to perform the inorder traversal of the parse tree
extern void in_order_traversal(FILE* fp, treeNodes* current);
// Function to print the parse tree using inorder traversal
extern void print_parse_tree(char* out_file);
// Function to return parse tree (ptree)
extern parse_tree* get_ptree();

// Set Operations
// Function to add key into the table along with the entry number
extern int set_add(char* key,entry* table[],int entry_number);
// Checks if the key is there in the table
extern int set_contains(char* key,entry* table[]);
// Function to add a set of values into a set except the string mentioned in the variable "except"
extern void set_add_sets(entry* set_to_add[], entry* final_set[],int entry_number,char* except);

// Grammar Operations
// Function to count the number of lines in the input file
extern int noOfLines(FILE* fptr);
// Function to add a symbol to a linked list of symbols in a rule
extern void insert_in_rule(rule* rules, int i, Boolean isTerminal, char* buff);
// Function to read a line of grammar.txt file and parse it to form a linked list
extern void linepop(FILE* fp,int i,rule* rules);
// Function to print the grammar rules from the array of linked list
extern void print_grammar(rule* rules);
// Function to make an array of linked list and populate it with the a grammar rules
extern rule* populate_grammar();
// Function to make an array of non-terminals along with important information
extern NonT* populate_non_terminals(entry* table[],rule* rules);
// Function to print non-terminals
extern void print_nont(NonT* nont);
// Function to prints only the non-null entries of the table
extern void print_tables_sets(entry* table[]);
// Function to print the first sets of all the non-terminals
extern void print_first_Sets(NonT* nont);
// This function prints the follow sets of all the non-terminals
extern void print_follow_sets(NonT* nont);
// Function to compute first set of all rules/non-terminals
extern void compute_first_Set(rule* rules,NonT* non_terminals_set);
// Function to help compute follow set
extern void followadd(int ruleno,NonT* non_terminals_set,int i,rule* rules);
// Function to compute follow sets for the non-terminals
extern void compute_follow_Set(rule* rules,NonT* non_terminals_set);
// Function to print the parse table
extern void print_parse_Table(int** arr,NonT* non_terminals_set);
// Function to create a parse table using info from rules and non-terminals
extern void create_parse_table(rule* rules,NonT* non_terminals_set);

extern void popOnErrors(token_info* curr, int prevLineNo);


extern void free_recursive_rulenodes(ruleNode* rule_head);
extern void free_rules(rule* rules);
extern void free_recursive_entries(entry* entry_head);
extern void free_sets(entry* table[]);
extern void free_non_terminals_table(NonT* non_terminals_table);
extern void free_parser_data(NonT* non_terminals, rule* rules);


// Start parser
// Function to start the Syntax Analyzer
extern void call_parser(rule* rules, NonT* nont);

extern void parseCompletely(int lflag);

#endif