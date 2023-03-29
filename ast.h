#include "parser.h"

extern ast_node* copier(ast_node* temp);
extern ast_node* create_ast(treeNodes* root);
extern void change_tree(treeNodes* root);
extern void create_abstract_tree();
extern void recursive_print_tree(ast_node* root,int listcount,FILE* fp);
extern ast_node* get_ast_root();