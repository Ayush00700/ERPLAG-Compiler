#include "parser.h"

extern ast_node* copier(ast_node* temp);
extern ast_node* create_ast(treeNodes* root);
extern void change_tree(treeNodes* root);
extern int create_abstract_tree();
extern int recursive_print_tree(ast_node* root,int listcount,int toreturn,FILE* fp);
extern ast_node* get_ast_root();