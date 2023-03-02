/* THE MATH_CS boiz
--------------------
1. Rajan Sahu       2019B4A70572P
2. Yash Goyal       2019B4A70638P
3. Ayush Agarwal    2019B4A70652P
4. Vasu Swaroop     2019B4A70656P
5. A Sudarshan      2019B4A70744P
*/

#include "parserDef.h"
#include <stdlib.h>
#include <stdio.h>

/* FUNCTION DECLARATIONS 
----------------------------*/
// Stack Operations
// Function to push element into the stack
void push(Stack* parseStack, ruleNode* element);
// Function to pop the top of the stack
ruleNode* pop(Stack* parseStack);
// Function to see what's at the top of the stack
ruleNode* top(Stack* parseStack);
// Function to check if the stack is empty or not
int isEmptyStack(Stack* parseStack);
// Function to push each node of rule following Left-most derivation
void addRecurse(Stack* stack, ruleNode* curr);
// Function to add Rule into the stack
void addNodesToStack(Stack* stack, rule* Rule);
// Function to print the updated stack
void printStack(Stack* parseStack);

// Tree Operations
// Function to create a parse tree node ready to be added
treeNodes* convertToPTreenode(ruleNode* Node);
// Function to create and initialize parse tree
void createParseTree(ruleNode* prog);
// Function to set fields of the tree node using token
void addTokenInfo(treeNodes* Node, token_info* token );
// Function to hash a string
unsigned int hash(char* key);
// Function to print any table in the required format
void print_tables(entry* table[]);
// Function to check if the parent of the node has a sibling or not
int checkUncleExists();
// Function to go to right sibling node
void goToSibling();
// Function to go to right node: either right sibling, if exists, else uncle
void goToNextRight();
// Function to go to the parent node
void goToParent();
// Function to go to the child node
void goToChild();
// Function to add the "rule" to the parse tree
void addRuleToTree(rule* rule);
// Function to perform the inorder traversal of the parse tree
void in_order_traversal(treeNodes* current);

// Set Operations
// Function to add key into the table along with the entry number
int set_add(char* key,entry* table[],int entry_number);
// Checks if the key is there in the table
int set_contains(char* key,entry* table[]);
// Function to add a set of values into a set except the string mentioned in the variable "except"
void set_add_sets(entry* set_to_add[], entry* final_set[],int entry_number,char* except);

// Grammar Operations
// Function to count the number of lines in the input file
int noOfLines(FILE* fptr);
// Function to add a symbol to a linked list of symbols in a rule
void insert_in_rule(rule* rules, int i, Boolean isTerminal, char* buff);
// Function to read a line of grammar.txt file and parse it to form a linked list
void linepop(FILE* fp,int i,rule* rules);
// Function to print the grammar rules from the array of linked list
void print_grammar(rule* rules);
// Function to make an array of linked list and populate it with the a grammar rules
rule* populate_grammar();
// Function to make an array of non-terminals along with important information
NonT* populate_non_terminals(entry* table[],rule* rules);
// Function to print non-terminals
void print_nont(NonT* nont);
// Function to prints only the non-null entries of the table
void print_tables_sets(entry* table[]);
// Function to print the first sets of all the non-terminals
void print_first_Sets(NonT* nont);
// This function prints the follow sets of all the non-terminals
void print_follow_sets(NonT* nont);
// Function to compute first set of all rules/non-terminals
void compute_first_Set(rule* rules,NonT* non_terminals_set);
// Function to help compute follow set
void followadd(int ruleno,NonT* non_terminals_set,int i,rule* rules);
// Function to compute follow sets for the non-terminals
void compute_follow_Set(rule* rules,NonT* non_terminals_set);
// Function to print the parse table
void print_parse_Table(int** arr,NonT* non_terminals_set);
// Function to create a parse table using info from rules and non-terminals
int** create_parse_table(rule* rules,NonT* non_terminals_set);

// Start parser
// Function to start the Syntax Analyzer
void call_parser(rule* rules, NonT* nont);