#include "ast.h"

ast_node* copier(ast_node* temp){
    ast_node* new_node = (ast_node*)malloc(sizeof(ast_node));
    new_node->child_pointers = temp->child_pointers;
    new_node->isTerminal = temp->isTerminal;
    new_node->name = temp->name;
    new_node->next = temp->next;
    new_node->no_of_children = temp->no_of_children;
    new_node->token = temp->token;
    return new_node;
}

ast_node* create_ast(treeNodes* root/*,treeNodes* root_parent*/){
    int rule_no = root->rule_no;
    switch(rule_no){
        case 1:
            ast_node* child1 = create_ast(root->child_pointers[0]);
            ast_node* child2 = create_ast(root->child_pointers[1]);
            ast_node* child3 = create_ast(root->child_pointers[2]);
            ast_node* child4 = create_ast(root->child_pointers[3]);
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "PROGRAM";
            parent->isTerminal = False;
            parent->no_of_children = 4;
            parent->child_pointers = (ast_node**)malloc(parent->no_of_children*sizeof(ast_node*));
            parent->next = NULL;
            parent->child_pointers[0] = child1;
            parent->child_pointers[1] = child2;
            parent->child_pointers[2] = child3;
            parent->child_pointers[3] = child4;
            free(root->child_pointers[0]);
            free(root->child_pointers[1]);
            free(root->child_pointers[2]);
            free(root->child_pointers[3]);
            return parent;
        case 7:
            ast_node* toreturn = create_ast(root->child_pointers[4]);
            free(root->child_pointers[0]);
            free(root->child_pointers[1]);
            free(root->child_pointers[2]);
            free(root->child_pointers[3]);
            free(root->child_pointers[4]);
            return toreturn;
        case 25:
            ast_node* child1 = create_ast(root->child_pointers[1]);
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "STATEMENTS";
            parent->isTerminal = False;
            parent->no_of_children = 1;
            parent->child_pointers = (ast_node**)malloc(parent->no_of_children*sizeof(ast_node*));
            parent->child_pointers[0] = child1;
            parent->next = NULL;
            free(root->child_pointers[0]);
            free(root->child_pointers[1]);
            free(root->child_pointers[2]);
            return parent;
        case 26:
            ast_node* child1 = create_ast(root->child_pointers[0]);
            ast_node* child2 = create_ast(root->child_pointers[1]);
            child1->next = child2;
            free(root->child_pointers[0]);
            free(root->child_pointers[1]);
            return child1;
        case 27:
            free(root->child_pointers[0]);
            return NULL;
        case 28:
            ast_node* child1 = create_ast(root->child_pointers[0]);
            free(root->child_pointers[0]);
            return child1;
        case 29:
            ast_node* child1 = create_ast(root->child_pointers[0]);
            free(root->child_pointers[0]);
            return child1;
        case 30:
            ast_node* child1 = create_ast(root->child_pointers[0]);
            free(root->child_pointers[0]);
            return child1;
        case 31:
            ast_node* child1 = create_ast(root->child_pointers[0]);
            free(root->child_pointers[0]);
            return child1;
        case 32:
            ast_node* child1 = create_ast(root->child_pointers[0]);
            free(root->child_pointers[0]);
            return child1;
        case 33:
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "INPUT";
            parent->isTerminal = True;
            parent->token = root->child_pointers[2]->token;
            free(root->child_pointers[0]);
            free(root->child_pointers[1]);
            free(root->child_pointers[3]);
            free(root->child_pointers[4]);
            return parent;
        case 34:
            ast_node* child1 = create_ast(root->child_pointers[2]);
            free(root->child_pointers[0]);
            free(root->child_pointers[1]);
            free(root->child_pointers[3]);
            free(root->child_pointers[4]);
            return child1;
        case 35:
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "TRUE";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;
        case 36:
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "FALSE";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;
        // case 40: (REDUNDANT) 
        case 37:
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "ID";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;
        case 38:
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "NUM";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;
        case 39:
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "RNUM";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;
        case 41:
            ast_node* temp = (ast_node*)malloc(sizeof(ast_node));
            temp->name = "ID";
            temp->isTerminal = True;
            temp->token = root->child_pointers[0]->token;
            root->child_pointers[1]->inh = temp;
            ast_node* child1 = create_ast(root->child_pointers[1]);
            free(root->child_pointers[1]);
            return child1;
        case 42:
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "NUM";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;
        case 43:
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "RNUM";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;
        case 44:
            ast_node* child1 = create_ast(root->child_pointers[0]);
            free(root->child_pointers[0]);
            return child1;
        case 45:
            ast_node* child1 = create_ast(root->child_pointers[1]);
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "ARRAY";
            parent->isTerminal = False;
            parent->no_of_children = 2;
            parent->child_pointers = (ast_node**)malloc(parent->no_of_children*sizeof(ast_node*));
            parent->child_pointers[0] = copier(root->inh);
            parent->child_pointers[1] = child1;
            free(root->child_pointers[0]);
            free(root->child_pointers[1]);
            free(root->child_pointers[2]);
            return parent;
        case 46:
            free(root->child_pointers[0]);
            return copier(root->inh);
        case 47:
            ast_node* child1 = create_ast(root->child_pointers[0]);
            free(root->child_pointers[0]);
            return child1;
        case 48:
            ast_node* child1 = create_ast(root->child_pointers[0]);
            free(root->child_pointers[0]);
            return child1;
        case 49:
            ast_node* temp = (ast_node*)malloc(sizeof(ast_node));
            temp->name = "ID";
            temp->isTerminal = True;
            temp->token = root->child_pointers[0]->token;
            root->child_pointers[1]->inh = temp;
            ast_node* child1 = create_ast(root->child_pointers[1]);
            free(root->child_pointers[1]);
            return child1;
        case 50:
            root->child_pointers[0]->inh = root->inh;
            ast_node* temp = create_ast(root->child_pointers[0]);
            free(root->child_pointers[0]);
            return temp;
        case 51:
            root->child_pointers[0]->inh = root->inh;
            ast_node* temp = create_ast(root->child_pointers[0]);
            free(root->child_pointers[0]);
            return temp;
        case 52:
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "ASSIGN";
            parent->isTerminal = False;
            parent->no_of_children = 2;
            parent->child_pointers = (ast_node**)malloc(parent->no_of_children*sizeof(ast_node*));
            parent->next = NULL;
            parent->child_pointers[0] = root->inh;
            parent->child_pointers[1] = create_ast(root->child_pointers[1]);
            free(root->child_pointers[0]);
            free(root->child_pointers[1]);
            free(root->child_pointers[2]);
            return parent;
        case 53:
            ast_node* left = (ast_node*)malloc(sizeof(ast_node));
            left->name = "ARRAY_ASSIGN";
            left->isTerminal = False;
            left->no_of_children = 2;
            left->child_pointers = (ast_node**)malloc(left->no_of_children*sizeof(ast_node*));
            left->next = NULL;
            left->child_pointers[0] = root->inh;
            left->child_pointers[1] = create_ast(root->child_pointers[1]);
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "ASSIGN";
            parent->isTerminal = False;
            parent->no_of_children = 2;
            parent->child_pointers = (ast_node**)malloc(parent->no_of_children*sizeof(ast_node*));
            parent->next = NULL;
            parent->child_pointers[0] = left;
            parent->child_pointers[1] = create_ast(root->child_pointers[4]);
            free(root->child_pointers[0]);
            free(root->child_pointers[1]);
            free(root->child_pointers[2]);
            free(root->child_pointers[3]);
            free(root->child_pointers[4]);
            free(root->child_pointers[5]);
            return parent;
        case 54: 
            parent->name = "SIGNED_NUMBER";
            parent->isTerminal = False;
            parent->no_of_children = 2;
            parent->child_pointers = (ast_node**)malloc(parent->no_of_children*sizeof(ast_node*));
            parent->next = NULL;
            parent->child_pointers[0] = create_ast(root->child_pointers[0]);
            parent->child_pointers[1] = create_ast(root->child_pointers[1]);
            free(root->child_pointers[0]);
            free(root->child_pointers[1]);
            return parent;
        case 55:
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "NUM";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;
        case 56:
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "ID";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;
        case 57:
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "PLUS";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;
        case 58:
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "MINUS";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;
        case 59:
            free(root->child_pointers[0]);
            return NULL;
        case 73: 
            ast_node* parent = create_ast(root->child_pointers[0]);
            free(root->child_pointers[0]);
            return parent;
        case 74:
            ast_node* parent = create_ast(root->child_pointers[0]);
            free(root->child_pointers[0]);
            return parent;
        case 75:
            ast_node* temp = create_ast(root->child_pointers[0]);
            root->child_pointers[1]->inh = temp;
            ast_node* parent = create_ast(root->child_pointers[1]);
            free(root->child_pointers[0]);
            free(root->child_pointers[1]);
            return parent;
        case 76:
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "PLUS";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;
        case 77:
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "MINUS";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;
        case 78:
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "UNARY";
            parent->isTerminal = False;
            parent->no_of_children = 2;
            parent->child_pointers = (ast_node**)malloc(parent->no_of_children*sizeof(ast_node*));
            parent->next = NULL;
            parent->child_pointers[0] = root->inh;
            parent->child_pointers[1] = create_ast(root->child_pointers[1]);
            free(root->child_pointers[0]);
            free(root->child_pointers[1]);
            free(root->child_pointers[2]);
            return parent;
        case 79:
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "UNARY";
            parent->isTerminal = False;
            parent->no_of_children = 2;
            parent->child_pointers = (ast_node**)malloc(parent->no_of_children*sizeof(ast_node*));
            parent->next = NULL;
            parent->child_pointers[0] = root->inh;
            parent->child_pointers[1] = create_ast(root->child_pointers[0]);
            free(root->child_pointers[0]);
            return parent;
        case 80:
            ast_node* temp = create_ast(root->child_pointers[0]);
            root->child_pointers[1]->inh = temp;
            free(root->child_pointers[0]);
            ast_node* parent = create_ast(root->child_pointers[1]);
            free(root->child_pointers[1]);
            return parent;
        case 81:
            ast_node* left = (ast_node*)malloc(sizeof(ast_node));
            ast_node* oper = create_ast(root->child_pointers[0]);
            left->name = oper->name;
            left->isTerminal = False;
            left->no_of_children = 2;
            left->child_pointers = (ast_node**)malloc(left->no_of_children*sizeof(ast_node*));
            left->next = NULL;
            left->child_pointers[0] = root->inh;
            left->child_pointers[1] = create_ast(root->child_pointers[1]);
            root->child_pointers[2]->inh = left;
            ast_node* parent = create_ast(root->child_pointers[2]);
            free(root->child_pointers[0]);
            free(root->child_pointers[1]);
            free(root->child_pointers[2]);
            return parent;
        case 82:
            free(root->child_pointers[0]);
            return root->inh;
        case 83:
            ast_node* temp = create_ast(root->child_pointers[0]);
            root->child_pointers[1]->inh = temp;
            ast_node* parent = create_ast(root->child_pointers[1]);
            free(root->child_pointers[0]);
            free(root->child_pointers[1]);
            return parent;
        case 84:
            ast_node* parent = create_ast(root->child_pointers[0]);
            free(root->child_pointers[0]);
            return parent;
        case 85:
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            ast_node* temp = create_ast(root->child_pointers[0]);
            parent->name = temp->name;
            parent->isTerminal = False;
            parent->no_of_children = 2;
            parent->child_pointers = (ast_node**)malloc(parent->no_of_children*sizeof(ast_node*));
            parent->next = NULL;
            parent->child_pointers[0] = root->inh;
            parent->child_pointers[1] = create_ast(root->child_pointers[1]);
            free(root->child_pointers[0]);
            free(root->child_pointers[1]);
            return parent;
        case 86:
            free(root->child_pointers[0]);
            return root->inh;
        case 87:
            ast_node* temp = create_ast(root->child_pointers[0]);
            root->child_pointers[1]->inh = temp;
            free(root->child_pointers[0]);
            ast_node* parent = create_ast(root->child_pointers[1]);
            free(root->child_pointers[1]);
            return parent;
        case 88:
            ast_node* left = (ast_node*)malloc(sizeof(ast_node));
            ast_node* oper = create_ast(root->child_pointers[0]);
            left->name = oper->name;
            left->isTerminal = False;
            left->no_of_children = 2;
            left->child_pointers = (ast_node**)malloc(left->no_of_children*sizeof(ast_node*));
            left->next = NULL;
            left->child_pointers[0] = root->inh;
            left->child_pointers[1] = create_ast(root->child_pointers[1]);
            root->child_pointers[2]->inh = left;
            ast_node* parent = create_ast(root->child_pointers[2]);
            free(root->child_pointers[0]);
            free(root->child_pointers[1]);
            free(root->child_pointers[2]);
            return parent;
        case 89:
            free(root->child_pointers[0]);
            return root->inh;
        case 90:
            ast_node* temp = create_ast(root->child_pointers[0]);
            root->child_pointers[1]->inh = temp;
            free(root->child_pointers[0]);
            ast_node* parent = create_ast(root->child_pointers[1]);
            free(root->child_pointers[1]);
            return parent;
        case 91:
            ast_node* left = (ast_node*)malloc(sizeof(ast_node));
            ast_node* oper = create_ast(root->child_pointers[0]);
            left->name = oper->name;
            left->isTerminal = False;
            left->no_of_children = 2;
            left->child_pointers = (ast_node**)malloc(left->no_of_children*sizeof(ast_node*));
            left->next = NULL;
            left->child_pointers[0] = root->inh;
            left->child_pointers[1] = create_ast(root->child_pointers[1]);
            root->child_pointers[2]->inh = left;
            ast_node* parent = create_ast(root->child_pointers[2]);
            free(root->child_pointers[0]);
            free(root->child_pointers[1]);
            free(root->child_pointers[2]);
            return parent;
        case 92:
            free(root->child_pointers[0]);
            return root->inh;
        case 93:
            ast_node* temp = create_ast(root->child_pointers[1]);
            free(root->child_pointers[0]);
            free(root->child_pointers[1]);
            free(root->child_pointers[2]);
            return temp;
        case 94:
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "NUM";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;
        case 95:
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "RNUM";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;
        case 96:
            ast_node* temp = (ast_node*)malloc(sizeof(ast_node));
            temp->name = "ID";
            temp->isTerminal = True;
            temp->token = root->child_pointers[0]->token;
            root->child_pointers[1]->inh = temp;
            ast_node* child1 = create_ast(root->child_pointers[1]);
            free(root->child_pointers[1]);
            return child1;
        case 97:
            ast_node*   parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "ARRAY_ACCESS";
            parent->isTerminal = False;
            parent->no_of_children = 2;
            parent->child_pointers = (ast_node**)malloc(parent->no_of_children*sizeof(ast_node*));
            parent->next = NULL;
            parent->child_pointers[0] = root->inh;
            parent->child_pointers[1] = create_ast(root->child_pointers[1]);
            free(root->child_pointers[0]);
            free(root->child_pointers[1]);
            free(root->child_pointers[2]);
            return parent;
        case 98:
            free(root->child_pointers[0]);
            return root->inh;
        // case 99: (REDUNDANT)
        case 100:
            ast_node* temp = create_ast(root->child_pointers[0]);
            root->child_pointers[1]->inh = temp;
            ast_node* parent = create_ast(root->child_pointers[1]);
            free(root->child_pointers[0]);
            free(root->child_pointers[1]);
            return parent;
        case 101:
            ast_node* parent = create_ast(root->child_pointers[0]);
            free(root->child_pointers[0]);
            return parent;
        case 102:
            parent->name = "SIGNED_NUMBER";
            parent->isTerminal = False;
            parent->no_of_children = 2;
            parent->child_pointers = (ast_node**)malloc(parent->no_of_children*sizeof(ast_node*));
            parent->next = NULL;
            parent->child_pointers[0] = root->inh;
            parent->child_pointers[1] = create_ast(root->child_pointers[0]);
            free(root->child_pointers[0]);
            return parent;
        case 103:
            parent->name = "SIGNED_EXPRESSION";
            parent->isTerminal = False;
            parent->no_of_children = 2;
            parent->child_pointers = (ast_node**)malloc(parent->no_of_children*sizeof(ast_node*));
            parent->next = NULL;
            parent->child_pointers[0] = root->inh;
            parent->child_pointers[1] = create_ast(root->child_pointers[1]);
            free(root->child_pointers[0]);
            free(root->child_pointers[1]);
            free(root->child_pointers[2]);
            return parent;
        case 104:
            ast_node* temp = create_ast(root->child_pointers[0]);
            root->child_pointers[1]->inh = temp;
            free(root->child_pointers[0]);
            ast_node* parent = create_ast(root->child_pointers[1]);
            free(root->child_pointers[1]);
            return parent;
        case 105:
            ast_node* left = (ast_node*)malloc(sizeof(ast_node));
            ast_node* oper = create_ast(root->child_pointers[0]);
            left->name = oper->name;
            left->isTerminal = False;
            left->no_of_children = 2;
            left->child_pointers = (ast_node**)malloc(left->no_of_children*sizeof(ast_node*));
            left->next = NULL;
            left->child_pointers[0] = root->inh;
            left->child_pointers[1] = create_ast(root->child_pointers[1]);
            root->child_pointers[2]->inh = left;
            ast_node* parent = create_ast(root->child_pointers[2]);
            free(root->child_pointers[0]);
            free(root->child_pointers[1]);
            free(root->child_pointers[2]);
            return parent;
        case 106:
            free(root->child_pointers[0]);
            return root->inh;
        case 107:
            ast_node* temp = create_ast(root->child_pointers[0]);
            root->child_pointers[1]->inh = temp;
            free(root->child_pointers[0]);
            ast_node* parent = create_ast(root->child_pointers[1]);
            free(root->child_pointers[1]);
            return parent;
        case 108:
            ast_node* left = (ast_node*)malloc(sizeof(ast_node));
            ast_node* oper = create_ast(root->child_pointers[0]);
            left->name = oper->name;
            left->isTerminal = False;
            left->no_of_children = 2;
            left->child_pointers = (ast_node**)malloc(left->no_of_children*sizeof(ast_node*));
            left->next = NULL;
            left->child_pointers[0] = root->inh;
            left->child_pointers[1] = create_ast(root->child_pointers[1]);
            root->child_pointers[2]->inh = left;
            ast_node* parent = create_ast(root->child_pointers[2]);
            free(root->child_pointers[0]);
            free(root->child_pointers[1]);
            free(root->child_pointers[2]);
            return parent;
        case 109:
            free(root->child_pointers[0]);
            return root->inh;
        case 110:
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "ID";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;
        case 111:
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "NUM";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;
        case 112:
            ast_node* parent = create_ast(root->child_pointers[0]);
            free(root->child_pointers[0]);
            return parent;
        case 113:
            ast_node* parent = create_ast(root->child_pointers[1]);
            free(root->child_pointers[0]);
            free(root->child_pointers[1]);
            free(root->child_pointers[2]);
            return parent;
        case 114:
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "PLUS";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;
        case 115:
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "MINUS";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;
        case 116:
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "MUL";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;
        case 117:
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "DIV";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;
        case 118:
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "AND";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;
        case 119:
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "OR";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;
        case 120:
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "LT";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;
        case 121:
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "LE";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;
        case 122:
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "GT";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;
        case 123:
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "GE";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;
        case 124:
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "EQ";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;
        case 125:
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "NE";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;
        


        
    }
}

void change_tree(treeNodes* root){
    if(root->isTerminal){
        return;
    }
    treeNodes* temp = root->child;
    int num=0;
    while(temp!=NULL){
        change_tree(temp);
        temp = temp->r_sibling;
        num++;
    }
    root->no_of_children = num;
    root->child_pointers = (treeNodes**)malloc(num*sizeof(treeNodes*));
    temp = root->child;
    for(int i=0;i<num;i++){
        root->child_pointers[i] = temp;
        temp = temp->r_sibling;
    }
}

int main(){
    parse_tree* ptree = get_ptree();
    change_tree(ptree->root);
    ast_node* new_root = create_ast(ptree->root);
}