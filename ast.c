#include "ast.h"
#include <stdlib.h>
#include <string.h>

int astNodeCount=1;
ast_node* new_root;

ast_node* initializeNewNode(){
    ast_node* new_node = (ast_node*)malloc(sizeof(ast_node));
    new_node->child_pointers = NULL;
    new_node->isTerminal = False;
    new_node->name = NULL;
    new_node->next = NULL;
    new_node->no_of_children=0;
    new_node->code=NULL;
    new_node->tempName=NULL;
    new_node->token=NULL;
    new_node->type=NULL;
    // new_node->asm_code=(char*) malloc(sizeof(char)*1000);
    return new_node; 
}

ast_node* copier(ast_node* temp)
/*This function returns a deep copy of the passed node*/
{
    ast_node* new_node = initializeNewNode();
    new_node->child_pointers = temp->child_pointers;
    new_node->isTerminal = temp->isTerminal;
    new_node->name = temp->name;
    new_node->next = temp->next;
    new_node->no_of_children = temp->no_of_children;
    new_node->token = temp->token;
    return new_node;
}

ast_node* create_ast(treeNodes* root /*,treeNodes* root_parent*/)
/*This function creates the AST based on the semantic rules for each grammar rule*/
{
    int rule_no = root->rule_no;
    switch(rule_no){
        case 1:
        {
            // Creating AST node's children subtrees
            ast_node* child1 = create_ast(root->child_pointers[0]);
            ast_node* child2 = create_ast(root->child_pointers[1]);
            ast_node* child3 = create_ast(root->child_pointers[2]);
            ast_node* child4 = create_ast(root->child_pointers[3]);
            ast_node* parent = initializeNewNode();
            // Assigning node members to group the subtrees
            parent->name = "PROGRAM";
            parent->isTerminal = False;
            parent->no_of_children = 4;
            parent->child_pointers = (ast_node**)malloc(parent->no_of_children*sizeof(ast_node*));
            parent->next = NULL;
            parent->child_pointers[0] = child1;
            parent->child_pointers[1] = child2;
            parent->child_pointers[2] = child3;
            parent->child_pointers[3] = child4;
            // Collapsing unnecessary non-terminals
            free(root->child_pointers[0]);
            free(root->child_pointers[1]);
            free(root->child_pointers[2]);
            free(root->child_pointers[3]);
            return parent;
        }
        case 2:
        {
            // Creating AST node's children subtrees
            ast_node* child1 = create_ast(root->child_pointers[0]);
            ast_node* child2 = create_ast(root->child_pointers[1]);
            // Inserting child1 at beginning of child2 list
            child1->next = child2;
            // Collapsing unnecessary non-terminals
            free(root->child_pointers[0]);
            free(root->child_pointers[1]);
            return child1;
        }
        case 3:
        // Since epsilon, just free the parse tree node "eps"
        {free(root->child_pointers[0]);
        return NULL;}
        case 4:
            // Create node for terminal token
            {ast_node* temp = initializeNewNode();
            temp->name = "MOD_DEC_ID";
            temp->isTerminal = True;
            temp->token = root->child_pointers[2]->token;
            temp->next= NULL;
            // Collapsing unnecessary non-terminals
            free(root->child_pointers[0]);
            free(root->child_pointers[1]);
            free(root->child_pointers[3]);
            return temp;}
        case 5:
            // Creating AST node's children subtrees
            {ast_node* child1 = create_ast(root->child_pointers[0]);
            ast_node* child2 = create_ast(root->child_pointers[1]);
            // Inserting child1 at the beginning of child2 list
            child1->next = child2;
            // Collapsing unnecessary non-terminals
            free(root->child_pointers[0]);
            free(root->child_pointers[1]);
            return child1;}
        case 6:
            // Since epsilon, just free the parse tree node "eps"
            {free(root->child_pointers[0]);
            return NULL;}
        case 7:
            // Creating AST node's children subtree
           {
            ast_node* child1 = create_ast(root->child_pointers[4]);
            ast_node* parent = initializeNewNode();
            parent->name = "DRIVER";
            parent->isTerminal = False;
            parent->no_of_children = 1;
            parent->next = NULL;
            parent->child_pointers = (ast_node**)malloc(parent->no_of_children*sizeof(ast_node*));
            parent->child_pointers[0] = child1;
            
            // Collapsing unnecessary non-terminals
            free(root->child_pointers[0]);
            free(root->child_pointers[1]);
            free(root->child_pointers[2]);
            free(root->child_pointers[3]);
            free(root->child_pointers[4]);
            return parent;}
        case 8:
            {// Create node for terminal token
            ast_node* temp = initializeNewNode();
            temp->name = "FUNC_ID";
            temp->isTerminal = True;
            temp->token = root->child_pointers[2]->token;
            // root->child_pointers[1]->inh = temp; //check 
            // Creating AST node's children subtrees
            ast_node* child1 = create_ast(root->child_pointers[7]);
            ast_node* child2 = create_ast(root->child_pointers[10]);
            ast_node* child3 = create_ast(root->child_pointers[11]);
            ast_node* parent = initializeNewNode();
            // Assigning node members to group the subtrees
            parent->name = "MODULE";
            parent->isTerminal = False;
            parent->next= NULL;
            parent->no_of_children = 4;
            parent->child_pointers = (ast_node**)malloc(parent->no_of_children*sizeof(ast_node*));
            parent->child_pointers[0] = temp;
            parent->child_pointers[1] = child1;
            parent->child_pointers[2] = child2;
            parent->child_pointers[3] = child3;
            // Collapsing unnecessary non-terminals
            free(root->child_pointers[0]);
            free(root->child_pointers[1]);
            free(root->child_pointers[3]);
            free(root->child_pointers[4]);
            free(root->child_pointers[5]);
            free(root->child_pointers[6]);
            free(root->child_pointers[7]);
            free(root->child_pointers[8]);
            free(root->child_pointers[9]);
            free(root->child_pointers[10]);
            free(root->child_pointers[11]);
            return parent;}
        case 9:
            // Creating AST node's child subtree
            {ast_node* child1 = create_ast(root->child_pointers[2]);
            // Collapsing unnecessary non-terminals
            free(root->child_pointers[0]);
            free(root->child_pointers[1]);
            free(root->child_pointers[2]);
            free(root->child_pointers[3]);
            free(root->child_pointers[4]);
            return child1;}
        case 10:
            // Since epsilon, just free the parse tree node "eps"
            {free(root->child_pointers[0]);
            return NULL;}
        case 11:
            // Create node for terminal token
            {ast_node* temp = initializeNewNode();
            temp->name = "ID";
            temp->isTerminal = True;
            temp->token = root->child_pointers[0]->token;
            temp->tempName = temp->token->lexeme;
            // Creating AST node's children subtrees
            ast_node* child1 = create_ast(root->child_pointers[2]);
            ast_node* child2 = create_ast(root->child_pointers[3]);
            ast_node* parent = initializeNewNode();
            // Assigning node members to group the subtrees
            parent->name = "IPLIST_HEAD";
            parent->isTerminal = False;
            parent->no_of_children = 2;
            parent->child_pointers = (ast_node**)malloc(parent->no_of_children*sizeof(ast_node*));
            parent->child_pointers[0] = temp;
            parent->child_pointers[1] = child1;
            parent->next= child2;
            // Collapsing unnecessary non-terminals
            free(root->child_pointers[1]);
            free(root->child_pointers[2]);
            free(root->child_pointers[3]);
            return parent;}
        case 12:
            // Create node for terminal token
            {ast_node* temp = initializeNewNode();
            temp->name = "ID";
            temp->isTerminal = True;
            temp->token = root->child_pointers[1]->token;
            temp->tempName = temp->token->lexeme;
            // Creating AST node's children subtrees
            ast_node* child1 = create_ast(root->child_pointers[3]);
            ast_node* child2 = create_ast(root->child_pointers[4]);
            ast_node* parent = initializeNewNode();
            // Assigning node members to group subtrees
            parent->name = "IPLIST";
            parent->isTerminal = False;
            parent->no_of_children = 2;
            parent->child_pointers = (ast_node**)malloc(parent->no_of_children*sizeof(ast_node*));
            parent->child_pointers[0] = temp;
            parent->child_pointers[1] = child1;
            parent->next= child2;
            // Collapsing unnecessary non-terminals
            free(root->child_pointers[0]);
            free(root->child_pointers[2]);
            free(root->child_pointers[3]);
            free(root->child_pointers[4]);
            return parent;}
        case 13:
            // Since epsilon, just free the parse tree node "eps"
            {free(root->child_pointers[0]);
            return NULL;}
        case 14:
            // Create node for terminal token
           { ast_node* temp = initializeNewNode();
            temp->name = "ID";
            temp->isTerminal = True;
            temp->token = root->child_pointers[0]->token;
            temp->tempName = temp->token->lexeme;
            // Creating AST node's children subtrees
            ast_node* child1 = create_ast(root->child_pointers[2]);
            ast_node* child2 = create_ast(root->child_pointers[3]);
            ast_node* parent = initializeNewNode();
            // Assigning node members to group the subtrees
            parent->name = "OPLIST_HEAD";
            parent->isTerminal = False;
            parent->no_of_children = 2;
            parent->child_pointers = (ast_node**)malloc(parent->no_of_children*sizeof(ast_node*));
            parent->child_pointers[0] = temp;
            parent->child_pointers[1] = child1;
            parent->next= child2;
            // Collapsing unnecessary non-terminals
            free(root->child_pointers[1]);
            free(root->child_pointers[2]);
            free(root->child_pointers[3]);
            return parent;}
        case 15:
            // Create node for terminal token
            {ast_node* temp = initializeNewNode();
            temp->name = "ID";
            temp->isTerminal = True;
            temp->token = root->child_pointers[1]->token;
            temp->tempName = temp->token->lexeme;
            // Creating AST node's children subtrees
            ast_node* child1 = create_ast(root->child_pointers[3]);
            ast_node* child2 = create_ast(root->child_pointers[4]);
            ast_node* parent = initializeNewNode();
            // Assigning node members to group the subtrees
            parent->name = "OPLIST";
            parent->isTerminal = False;
            parent->no_of_children = 2;
            parent->child_pointers = (ast_node**)malloc(parent->no_of_children*sizeof(ast_node*));
            parent->child_pointers[0] = temp;
            parent->child_pointers[1] = child1;
            parent->next= child2;
            // Collapsing unnecessary non-terminals
            free(root->child_pointers[0]);
            free(root->child_pointers[2]);
            free(root->child_pointers[3]);
            free(root->child_pointers[4]);
            return parent;}
        case 16:
            // Since epsilon, just free the parse tree node "eps"
            {free(root->child_pointers[0]);
            return NULL;}
        case 17:
            // Create node for terminal token
            {ast_node* parent = initializeNewNode();
            parent->name = "INTEGER";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;}
        case 18:    
            // Create node for terminal token
            {ast_node* parent = initializeNewNode();
            parent->name = "REAL";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;}
        case 19:
            // Create node for terminal token
            {ast_node* parent = initializeNewNode();
            parent->name = "BOOLEAN";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;}
        case 20:
            // Creating AST node's children subtrees
            {ast_node* child1 = create_ast(root->child_pointers[2]);
            ast_node* child2 = create_ast(root->child_pointers[5]);
            ast_node* parent = initializeNewNode();
            // Assigning node members to group the subtrees
            parent->name = "ARRAY_DATATYPE";
            parent->isTerminal = False;
            parent->next= NULL;
            parent->no_of_children = 2;
            parent->child_pointers = (ast_node**)malloc(parent->no_of_children*sizeof(ast_node*));
            parent->child_pointers[0] = child1;
            parent->child_pointers[1] = child2;
            // Collapsing unnecessary non-terminals
            free(root->child_pointers[0]);
            free(root->child_pointers[1]);
            free(root->child_pointers[2]);
            free(root->child_pointers[3]);
            free(root->child_pointers[4]);
            free(root->child_pointers[5]);
            return parent;}
        case 21:
            // Creating AST node's children subtrees
            {ast_node* child1 = create_ast(root->child_pointers[0]);
            ast_node* child2 = create_ast(root->child_pointers[2]);
            ast_node* parent = initializeNewNode();
            // Assigning node members to group the subtrees
            parent->name = "ARRAY_RANGE";
            parent->isTerminal = False;
            parent->next= NULL;
            parent->no_of_children = 2;
            parent->child_pointers = (ast_node**)malloc(parent->no_of_children*sizeof(ast_node*));
            parent->child_pointers[0] = child1;
            parent->child_pointers[1] = child2;
            // Collapsing unnecessary non-terminals
            free(root->child_pointers[0]);
            free(root->child_pointers[1]);
            free(root->child_pointers[2]);
            return parent;}
        case 22:
            // Create node for terminal token
            {ast_node* parent = initializeNewNode();
            parent->name = "INTEGER";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;}
        case 23:
            // Create node for terminal token
            {ast_node* parent = initializeNewNode();
            parent->name = "REAL";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;}
        case 24:
            // Create node for terminal token
            {ast_node* parent = initializeNewNode();
            parent->name = "BOOLEAN";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;}
        case 25:
            // Creating AST child subtree
            {ast_node* parent = initializeNewNode();
            ast_node* child1 = create_ast(root->child_pointers[1]);
            parent->name = "STATEMENTS";
            parent->isTerminal = False;
            parent->next= child1;
            parent->no_of_children = 0;
            // Collapsing unnecessary non-terminals
            free(root->child_pointers[0]);
            free(root->child_pointers[1]);
            free(root->child_pointers[2]);
            return parent;}
        case 26:
            // Creating AST node's children subtree
            {ast_node* child1 = create_ast(root->child_pointers[0]);
            ast_node* child2 = create_ast(root->child_pointers[1]);
            // Inserting child1 to the beginning of child2 list
            child1->next = child2;
            // Collapsing unnecessary non-terminals
            free(root->child_pointers[0]);
            free(root->child_pointers[1]);
            return child1;}
        case 27:
            // Since epsilon, just free the parse tree node "eps"
           { free(root->child_pointers[0]);
            return NULL;}
        case 28:
            // Create AST subtree
            {ast_node* child1 = create_ast(root->child_pointers[0]);
            // Collapsing unnecessary non-terminals
            free(root->child_pointers[0]);
            return child1;}
        case 29:
            // Create AST subtree
            {ast_node* child1 = create_ast(root->child_pointers[0]);
            // Collapsing unnecessary non-terminals
            free(root->child_pointers[0]);
            return child1;}
        case 30:
            // Create AST subtree
            {ast_node* child1 = create_ast(root->child_pointers[0]);
            // Collapsing unnecessary non-terminals
            free(root->child_pointers[0]);
            return child1;}
        case 31:
            // Create AST subtree
            {ast_node* child1 = create_ast(root->child_pointers[0]);
            // Collapsing unnecessary non-terminals
            free(root->child_pointers[0]);
            return child1;}
        case 32:
            // Create AST subtree
            {ast_node* child1 = create_ast(root->child_pointers[0]);
            // Collapsing unnecessary non-terminals
            free(root->child_pointers[0]);
            return child1;}
        case 33:
            // Create node for terminal token
            {ast_node* parent = initializeNewNode();
            parent->name = "INPUT_ID";
            parent->isTerminal = True;
            parent->token = root->child_pointers[2]->token;
            
            parent->tempName = parent->token->lexeme;
            // Collapsing unnecessary non-terminals
            free(root->child_pointers[0]);
            free(root->child_pointers[1]);
            free(root->child_pointers[3]);
            free(root->child_pointers[4]);
            return parent;}
        case 34:
            // Create AST subtree
            {ast_node* child1 = create_ast(root->child_pointers[2]);
            // Discuss if we require the node
            // Maybe required for ignoring this type of io in
            // case of type checking - CHECK TODO
            ast_node* parent = initializeNewNode();
            parent->name = "OUTPUT";
            parent->isTerminal = False;
            parent->next= NULL;
            parent->no_of_children = 1;
            parent->child_pointers = (ast_node**)malloc(parent->no_of_children*sizeof(ast_node*));
            parent->child_pointers[0] = child1;
            // Collapsing unnecessary non-terminals
            free(root->child_pointers[0]);
            free(root->child_pointers[1]);
            free(root->child_pointers[2]);
            free(root->child_pointers[3]);
            free(root->child_pointers[4]);
            return parent;}
        case 35:
            // Create node for terminal token
            {ast_node* parent = initializeNewNode();
            parent->name = "TRUE";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            parent->tempName = "true";
            return parent;}
        case 36:
            // Create node for terminal token
            {ast_node* parent = initializeNewNode();
            parent->name = "FALSE";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            parent->tempName = "false";
            return parent;}
        case 37:
        // Create node for terminal token
            {ast_node* parent = initializeNewNode();
            parent->name = "ID";
            parent->isTerminal = True;
            parent->tempName = parent->token->lexeme;
            parent->token = root->child_pointers[0]->token;
            return parent;}
        case 38:
            // Create node for terminal token
           { ast_node* parent = initializeNewNode();
            parent->name = "NUM";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            parent->tempName = parent->token->lexeme;
            return parent;}
        case 39:
            // Create node for terminal token
            {ast_node* parent = initializeNewNode();
            parent->name = "RNUM";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            parent->tempName = parent->token->lexeme;
            return parent;
            }
        case 40:{
            break;
        }
        case 41:
            // Create node for terminal token
           { ast_node* temp = initializeNewNode();
            temp->name = "ID";
            temp->isTerminal = True;
            temp->token = root->child_pointers[0]->token;
            temp->tempName = temp->token->lexeme;
            // This node is required in the sibling subtree
            // So we pass as inherited attribute
            root->child_pointers[1]->inh = temp;
            // Creating AST node child subtree
            ast_node* child1 = create_ast(root->child_pointers[1]);
            // Collapsing unnecessary non-terminals
            free(root->child_pointers[1]);
            return child1;}
        case 42:
            // Create node for terminal token
            {ast_node* parent = initializeNewNode();
            parent->name = "NUM";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            parent->tempName = parent->token->lexeme;

            return parent;}
        case 43:
            // Create node for terminal token
           { ast_node* parent = initializeNewNode();
            parent->name = "RNUM";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            parent->tempName = parent->token->lexeme;
            return parent;}
        case 44:
            // Create AST subtree
            {ast_node* child1 = create_ast(root->child_pointers[0]);
            // Collapsing unnecessary non-terminals
            free(root->child_pointers[0]);
            return child1;}
        case 45:
            // Creating AST node's child subtree
           { ast_node* child1 = create_ast(root->child_pointers[1]);
            ast_node* parent = initializeNewNode();
            // Assigning node members
            parent->name = "ARRAY";
            parent->isTerminal = False;
            parent->no_of_children = 2;
            parent->child_pointers = (ast_node**)malloc(parent->no_of_children*sizeof(ast_node*));
            // Get a copy of the current parse tree node's inherited attribute
            parent->child_pointers[0] = copier(root->inh);
            parent->child_pointers[1] = child1;
            // Collapsing unnecessary non-terminals
            free(root->child_pointers[0]);
            free(root->child_pointers[1]);
            free(root->child_pointers[2]);
            return parent;}
        case 46:
            // Since epsilon, just free the parse tree node "eps"
            {free(root->child_pointers[0]);
            // Return inherited attribute as the synthesized attribute
            return copier(root->inh);}
        case 47:
            // Maybe we need a node to indicate the type of simplestatement
            // Create AST subtree
            {ast_node* child1 = create_ast(root->child_pointers[0]);
            // Collapsing unnecessary non-terminals
            free(root->child_pointers[0]);
            return child1;}
        case 48:
            // Maybe we need a node to indicate the type of simplestatement
            // Create AST subtree
            {ast_node* child1 = create_ast(root->child_pointers[0]);
            // Collapsing unnecessary non-terminals
            free(root->child_pointers[0]);
            return child1;}
        case 49:
            // Create node for terminal token
            {ast_node* temp = initializeNewNode();
            temp->name = "ID";
            temp->isTerminal = True;
            temp->token = root->child_pointers[0]->token;
            temp->tempName = temp->token->lexeme;
            // This node is required in the sibling subtree
            // So we pass as inherited attribute
            root->child_pointers[1]->inh = temp;
            // Create AST subtree
            ast_node* child1 = create_ast(root->child_pointers[1]);
            // Collapsing unnecessary non-terminals
            free(root->child_pointers[1]);
            return child1;}
        case 50:
            // Assign inherited attribute of current node as that of the parent
            {root->child_pointers[0]->inh = root->inh;
            // Create AST subtree
            ast_node* temp = create_ast(root->child_pointers[0]);
            // Collapsing unnecessary non-terminals
            free(root->child_pointers[0]);
            return temp;}
        case 51:
            // Assign inherited attribute of current node as that of the parent
            {root->child_pointers[0]->inh = root->inh;
            // Create AST subtree
            ast_node* temp = create_ast(root->child_pointers[0]);
            // Collapsing unnecessary non-terminals
            free(root->child_pointers[0]);
            return temp;}
        case 52:
            // Create AST node
            {ast_node* parent = initializeNewNode();
            // Assign node members
            parent->name = "ASSIGN";
            parent->isTerminal = False;
            parent->no_of_children = 2;
            parent->child_pointers = (ast_node**)malloc(parent->no_of_children*sizeof(ast_node*));
            parent->next = NULL;
            // Previously the below format was followed but here its different
            // ast_node* child1 = create_ast(root->child_pointers[1]);
            // parent->child_pointers[0] = copier(root->inh);
            // parent->child_pointers[1] = child1;
            // This node uses the inherited attribute assigned in annotated parse tree
            parent->child_pointers[0] = root->inh;
            parent->child_pointers[1] = create_ast(root->child_pointers[1]);
            // Collapsing unnecessary non-terminals
            free(root->child_pointers[0]);
            free(root->child_pointers[1]);
            free(root->child_pointers[2]);
            return parent;}
        case 53:
            // Create node for left child of AST node
            {ast_node* left = initializeNewNode();
            left->name = "ARRAY_ASSIGN";
            left->isTerminal = False;
            left->no_of_children = 2;
            left->child_pointers = (ast_node**)malloc(left->no_of_children*sizeof(ast_node*));
            left->next = NULL;
            // This node uses the inherited attribute assigned in annotated parse tree
            left->child_pointers[0] = root->inh;
            // Create the AST subtree
            left->child_pointers[1] = create_ast(root->child_pointers[1]);
            // Create AST node to group the children
            ast_node* parent = initializeNewNode();
            parent->name = "ASSIGN";
            parent->isTerminal = False;
            parent->no_of_children = 2;
            parent->child_pointers = (ast_node**)malloc(parent->no_of_children*sizeof(ast_node*));
            parent->next = NULL;
            parent->child_pointers[0] = left;
            parent->child_pointers[1] = create_ast(root->child_pointers[4]);
            // Collapsing unnecessary non-terminals
            free(root->child_pointers[0]);
            free(root->child_pointers[1]);
            free(root->child_pointers[2]);
            free(root->child_pointers[3]);
            free(root->child_pointers[4]);
            free(root->child_pointers[5]);
            return parent;}
        case 54:
            // Create AST node
            {ast_node* parent = initializeNewNode();
            // Assigning node members
            parent->name = "SIGNED_NUMBER";
            parent->isTerminal = False;
            parent->no_of_children = 2;
            parent->child_pointers = (ast_node**)malloc(parent->no_of_children*sizeof(ast_node*));
            parent->next = NULL;
            parent->child_pointers[0] = create_ast(root->child_pointers[0]);
            parent->child_pointers[1] = create_ast(root->child_pointers[1]);
            // Collapsing unnecessary non-terminals
            free(root->child_pointers[0]);
            free(root->child_pointers[1]);
            return parent;}
        case 55:
            // Create node for terminal token
            {ast_node* parent = initializeNewNode();
            parent->name = "NUM";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            parent->tempName = parent->token->lexeme;
            return parent;}
        case 56:
            // Create node for terminal token
            {ast_node* parent = initializeNewNode();
            parent->name = "ID";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            parent->tempName = parent->token->lexeme;
            return parent;}
        case 57:
            // Create node for terminal token
            {ast_node* parent = initializeNewNode();
            parent->name = "PLUS";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;}
        case 58:
            // Create node for terminal token
           { ast_node* parent = initializeNewNode();
            parent->name = "MINUS";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;}
        case 59:
            // Since epsilon, just free the parse tree node "eps"
            {free(root->child_pointers[0]);
            return NULL;}
        case 60:
            // Create node for terminal token
            {ast_node* temp = initializeNewNode();
            temp->name = "ID";
            temp->isTerminal = True;
            temp->token = root->child_pointers[3]->token;
            temp->tempName = temp->token->lexeme;
            // Creating AST node's children subtrees
            ast_node* child1 = create_ast(root->child_pointers[0]);
            ast_node* child2 = create_ast(root->child_pointers[6]);
            ast_node* parent = initializeNewNode();
            // Assigning node members
            parent->name = "MODULEREUSE";
            parent->isTerminal = False;
            parent->next= NULL;
            parent->no_of_children = 3;
            parent->child_pointers = (ast_node**)malloc(parent->no_of_children*sizeof(ast_node*));
            parent->child_pointers[0] = temp;
            parent->child_pointers[1] = child1;
            parent->child_pointers[2] = child2;
            // Collapsing unnecessary non-terminals
            free(root->child_pointers[0]);
            free(root->child_pointers[1]);
            free(root->child_pointers[2]);
            free(root->child_pointers[4]);
            free(root->child_pointers[5]);
            free(root->child_pointers[6]);
            free(root->child_pointers[7]);
            return parent;}
        case 61:
            // Creating AST node's children subtrees
            {ast_node* child1 = create_ast(root->child_pointers[0]);
            ast_node* child2 = create_ast(root->child_pointers[1]);
            ast_node* parent = initializeNewNode();
            // Assigning node members
            parent->name = "PARALIST_HEAD";
            parent->isTerminal = False;
            parent->no_of_children = 2;
            parent->child_pointers = (ast_node**)malloc(parent->no_of_children*sizeof(ast_node*));
            parent->child_pointers[0] = child1;
            parent->child_pointers[1] = child2;
            ast_node* child3 = create_ast(root->child_pointers[2]);
            parent->next= child3;
            // Collapsing unnecessary non-terminals
            free(root->child_pointers[0]);
            free(root->child_pointers[1]);
            free(root->child_pointers[2]);
            return parent;}
        case 62:
            // Creating AST node's children subtrees
            {ast_node* child1 = create_ast(root->child_pointers[1]);
            ast_node* child2 = create_ast(root->child_pointers[2]);
            ast_node* parent = initializeNewNode();
            // Assigning node members
            parent->name = "PARALIST";
            parent->isTerminal = False;
            parent->no_of_children = 2;
            parent->child_pointers = (ast_node**)malloc(parent->no_of_children*sizeof(ast_node*));
            parent->child_pointers[0] = child1;
            parent->child_pointers[1] = child2;
            ast_node* child3 = create_ast(root->child_pointers[3]);
            parent->next= child3;
            // Collapsing unnecessary non-terminals
            free(root->child_pointers[0]);
            free(root->child_pointers[1]);
            free(root->child_pointers[2]);
            free(root->child_pointers[3]);
            return parent;}
        case 63:
            // Since epsilon, just free the parse tree node "eps"
            {free(root->child_pointers[0]);
            return NULL;}
        case 64:
            // Create node for terminal token
            {ast_node* parent = initializeNewNode();
            parent->name = "NUM";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            parent->tempName = parent->token->lexeme;
            return parent;}
        case 65:
            // Create node for terminal token
            {ast_node* parent = initializeNewNode();
            parent->name = "RNUM";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            parent->tempName = parent->token->lexeme;
            return parent;}
        case 66:
            // Create AST subtree
            {ast_node* child1 = create_ast(root->child_pointers[0]);
            // Collapsing unnecessary non-terminals
            free(root->child_pointers[0]);
            return child1;}
        case 67:
            // Create node for terminal token
            {ast_node* temp = initializeNewNode();
            temp->name = "ID";
            temp->isTerminal = True;
            temp->token = root->child_pointers[0]->token;
            temp->tempName = temp->token->lexeme;
            // Creating AST node's child subtree
            ast_node* child1 = create_ast(root->child_pointers[1]);
            ast_node* parent = initializeNewNode();
            // Assigning node members
            parent->name = "NON_CONST_PARAMETER";
            parent->isTerminal = False;
            parent->next= NULL;
            parent->no_of_children = 2;
            parent->child_pointers = (ast_node**)malloc(parent->no_of_children*sizeof(ast_node*));
            parent->child_pointers[0] = temp;
            parent->child_pointers[1] = child1;
            // Collapsing unnecessary non-terminals
            free(root->child_pointers[1]);
            return parent;}
        case 68:
            // Create AST subtree
            {ast_node* child1 = create_ast(root->child_pointers[1]);
            // Collapsing unnecessary non-terminals
            free(root->child_pointers[0]);
            free(root->child_pointers[1]);
            free(root->child_pointers[2]);
            free(root->child_pointers[3]);
            return child1;}

        case 69:
            // Since epsilon, just free the parse tree node "eps"
            {free(root->child_pointers[0]);
            return NULL;}
        case 70:
            // Create node for terminal token
            {
            ast_node* child1 = create_ast(root->child_pointers[1]);
            ast_node* temp = initializeNewNode();
            temp->name = "ID_LIST_HEAD";
            temp->isTerminal = True;
            temp->next = child1;
            temp->token = root->child_pointers[0]->token;
            temp->tempName = temp->token->lexeme;
            // ast_node* parent = initializeNewNode();
            // parent->name = "ID_LIST";
            // parent->isTerminal = False;
            // parent->no_of_children = 1;
            // parent->child_pointers = (ast_node**)malloc(parent->no_of_children*sizeof(ast_node*));
            // parent->child_pointers[0] = temp;
            // Create AST subtree
            // ast_node* child1 = create_ast(root->child_pointers[1]);
            // parent->next= child1;
            // Collapsing unnecessary non-terminals
            free(root->child_pointers[1]);
            // return parent;}
            return temp;}
        case 71:
            {ast_node* temp = initializeNewNode();
            temp->name = "ID";
            temp->isTerminal = True;
            temp->token = root->child_pointers[1]->token;
            temp->tempName = temp->token->lexeme;
            ast_node* child1 = create_ast(root->child_pointers[2]);
            temp->next = child1;
            free(root->child_pointers[0]);
            free(root->child_pointers[1]);
            // free(root->child_pointers[2]); /--> missed this?
            return temp;}
        case 72:
            // Since epsilon, just free the parse tree node "eps"
           { free(root->child_pointers[0]);
            return NULL;}
        case 73: 
            {ast_node* parent = create_ast(root->child_pointers[0]);
            free(root->child_pointers[0]);
            return parent;}
        case 74:
            {ast_node* parent = create_ast(root->child_pointers[0]);
            free(root->child_pointers[0]);
            return parent;}
        case 75:
            {ast_node* temp = create_ast(root->child_pointers[0]);
            root->child_pointers[1]->inh = temp;
            ast_node* parent = create_ast(root->child_pointers[1]);
            free(root->child_pointers[0]);
            free(root->child_pointers[1]);
            return parent;}
        case 76:
            // Create node for terminal token
            {ast_node* parent = initializeNewNode();
            parent->name = "PLUS";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;}
        case 77:
            // Create node for terminal token
            {ast_node* parent = initializeNewNode();
            parent->name = "MINUS";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;}
        case 78:
       {    ast_node* parent = initializeNewNode();
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
            return parent;}
        case 79:
        {    ast_node* parent = initializeNewNode();
            parent->name = "UNARY";
            parent->isTerminal = False;
            parent->no_of_children = 2;
            parent->child_pointers = (ast_node**)malloc(parent->no_of_children*sizeof(ast_node*));
            parent->next = NULL;
            parent->child_pointers[0] = root->inh;
            parent->child_pointers[1] = create_ast(root->child_pointers[0]);
            free(root->child_pointers[0]);
            return parent;}
        case 80:
      {      ast_node* temp = create_ast(root->child_pointers[0]);
            root->child_pointers[1]->inh = temp;
            free(root->child_pointers[0]);
            ast_node* parent = create_ast(root->child_pointers[1]);
            free(root->child_pointers[1]);
            return parent;}
        case 81:
        {    ast_node* left = initializeNewNode();
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
            return parent;}
        case 82:
            // Since epsilon, just free the parse tree node "eps"
            free(root->child_pointers[0]);
            // Return inherited attribute as the synthesized attribute
            return root->inh;
        case 83:
           { ast_node* temp = create_ast(root->child_pointers[0]);
            root->child_pointers[1]->inh = temp;
            ast_node* parent = create_ast(root->child_pointers[1]);
            free(root->child_pointers[0]);
            free(root->child_pointers[1]);
            return parent;}
        case 84:
        {    ast_node* parent = create_ast(root->child_pointers[0]);
            free(root->child_pointers[0]);
            return parent;}
        case 85:
           { ast_node* parent = initializeNewNode();
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
            return parent;}
        case 86:
            // Since epsilon, just free the parse tree node "eps"
            free(root->child_pointers[0]);
            // Return inherited attribute as the synthesized attribute
            return root->inh;
        case 87:
        {    ast_node* temp = create_ast(root->child_pointers[0]);
            root->child_pointers[1]->inh = temp;
            free(root->child_pointers[0]);
            ast_node* parent = create_ast(root->child_pointers[1]);
            free(root->child_pointers[1]);
            return parent;}
        case 88:
{            ast_node* left = initializeNewNode();
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
            return parent;}
        case 89:
            // Since epsilon, just free the parse tree node "eps"
            free(root->child_pointers[0]);
            // Return inherited attribute as the synthesized attribute
            return root->inh;
        case 90:
     {       ast_node* temp = create_ast(root->child_pointers[0]);
            root->child_pointers[1]->inh = temp;
            free(root->child_pointers[0]);
            ast_node* parent = create_ast(root->child_pointers[1]);
            free(root->child_pointers[1]);
            return parent;}
        case 91:
{            ast_node* left = initializeNewNode();
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
            return parent;}
        case 92:
            {// Since epsilon, just free the parse tree node "eps"
            free(root->child_pointers[0]);
            // Return inherited attribute as the synthesized attribute
            return root->inh;}
        case 93:
       {     ast_node* temp = create_ast(root->child_pointers[1]);
            free(root->child_pointers[0]);
            free(root->child_pointers[1]);
            free(root->child_pointers[2]);
            return temp;}
        case 94:
            // Create node for terminal token
            {ast_node* parent = initializeNewNode();
            parent->name = "NUM";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            parent->tempName = parent->token->lexeme;
            return parent;}
        case 95:
            // Create node for terminal token
            {ast_node* parent = initializeNewNode();
            parent->name = "RNUM";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            parent->tempName = parent->token->lexeme;
            return parent;}
        case 96:
 {           ast_node* temp = initializeNewNode();
            temp->name = "ID";
            temp->isTerminal = True;
            temp->token = root->child_pointers[0]->token;
            temp->tempName = temp->token->lexeme;
            root->child_pointers[1]->inh = temp;
            ast_node* child1 = create_ast(root->child_pointers[1]);
            free(root->child_pointers[1]);
            return child1;}
        case 97:
 {           ast_node*   parent = initializeNewNode();
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
            return parent;}
        case 98:
            {// Since epsilon, just free the parse tree node "eps"
            free(root->child_pointers[0]);
            // Return inherited attribute as the synthesized attribute
            return root->inh;}
        case 99:
        {
            break;
        }
        case 100:
        {    ast_node* temp = create_ast(root->child_pointers[0]);
            root->child_pointers[1]->inh = temp;
            ast_node* parent = create_ast(root->child_pointers[1]);
            free(root->child_pointers[0]);
            free(root->child_pointers[1]);
            return parent;}
        case 101:
         {   ast_node* parent = create_ast(root->child_pointers[0]);
            free(root->child_pointers[0]);
            return parent;}
        case 102:
        {        
            ast_node*   parent = initializeNewNode();
            parent->name = "SIGNED_NUMBER";
            parent->isTerminal = False;
            parent->no_of_children = 2;
            parent->child_pointers = (ast_node**)malloc(parent->no_of_children*sizeof(ast_node*));
            parent->next = NULL;
            parent->child_pointers[0] = root->inh;
            parent->child_pointers[1] = create_ast(root->child_pointers[0]);
            free(root->child_pointers[0]);
            return parent;}
        case 103:
        {   ast_node*   parent = initializeNewNode();
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
            return parent;}
        case 104:
        {    ast_node* temp = create_ast(root->child_pointers[0]);
            root->child_pointers[1]->inh = temp;
            free(root->child_pointers[0]);
            ast_node* parent = create_ast(root->child_pointers[1]);
            free(root->child_pointers[1]);
            return parent;}
        case 105:
{            ast_node* left = initializeNewNode();
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
            return parent;}
        case 106:
            // Since epsilon, just free the parse tree node "eps"
            {free(root->child_pointers[0]);
            // Return inherited attribute as the synthesized attribute
            return root->inh;}
        case 107:
{            ast_node* temp = create_ast(root->child_pointers[0]);
            root->child_pointers[1]->inh = temp;
            free(root->child_pointers[0]);
            ast_node* parent = create_ast(root->child_pointers[1]);
            free(root->child_pointers[1]);
            return parent;}
        case 108:
{            ast_node* left = initializeNewNode();
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
            return parent;}
        case 109:
            // Since epsilon, just free the parse tree node "eps"
            {free(root->child_pointers[0]);
            // Return inherited attribute as the synthesized attribute
            return root->inh;}
        case 110:
            // Create node for terminal token
            {ast_node* parent = initializeNewNode();
            parent->name = "ID";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            parent->tempName = parent->token->lexeme;
            return parent;}
        case 111:
            // Create node for terminal token
            {ast_node* parent = initializeNewNode();
            parent->name = "NUM";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            parent->tempName = parent->token->lexeme;
            return parent;}
        case 112:
    {       ast_node* parent = create_ast(root->child_pointers[0]);
            free(root->child_pointers[0]);
            return parent;}
        case 113:
          {  ast_node* parent = create_ast(root->child_pointers[1]);
            free(root->child_pointers[0]);
            free(root->child_pointers[1]);
            free(root->child_pointers[2]);
            return parent;}
        case 114:
            // Create node for terminal token
            {ast_node* parent = initializeNewNode();
            parent->name = "PLUS";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;}
        case 115:
            // Create node for terminal token
            {ast_node* parent = initializeNewNode();
            parent->name = "MINUS";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;}
        case 116:
            // Create node for terminal token
            {ast_node* parent = initializeNewNode();
            parent->name = "MUL";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;}
        case 117:
            // Create node for terminal token
            {ast_node* parent = initializeNewNode();
            parent->name = "DIV";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;}
        case 118:
            // Create node for terminal token
            {ast_node* parent = initializeNewNode();
            parent->name = "AND";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;}
        case 119:
            // Create node for terminal token
            {ast_node* parent = initializeNewNode();
            parent->name = "OR";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;}
        case 120:
            // Create node for terminal token
            {ast_node* parent = initializeNewNode();
            parent->name = "LT_result";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;}
        case 121:
            // Create node for terminal token
            {ast_node* parent = initializeNewNode();
            parent->name = "LE_result";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;}
        case 122:
            // Create node for terminal token
            {ast_node* parent = initializeNewNode();
            parent->name = "GT_result";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;}
        case 123:
            // Create node for terminal token
            {ast_node* parent = initializeNewNode();
            parent->name = "GE_result";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;}
        case 124:
            // Create node for terminal token
            {ast_node* parent = initializeNewNode();
            parent->name = "EQ_result";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;}
        case 125:
            {// Create node for terminal token
            ast_node* parent = initializeNewNode();
            parent->name = "NE_result";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;}

        case 126:
  {          ast_node* child1 = create_ast(root->child_pointers[1]);
            ast_node* child2 = create_ast(root->child_pointers[3]);
            ast_node* parent = initializeNewNode();
            parent->name = "DECLARE";
            parent->isTerminal = False;
            parent->no_of_children = 2;
            parent->next= NULL;
            parent->child_pointers = (ast_node**)malloc(parent->no_of_children*sizeof(ast_node*));
            parent->child_pointers[0] = child1;
            parent->child_pointers[1] = child2;
            free(root->child_pointers[0]);
            free(root->child_pointers[1]);
            free(root->child_pointers[2]);
            free(root->child_pointers[3]);
            free(root->child_pointers[4]);
            return parent;}
        case 127:
{           ast_node* temp = initializeNewNode();
            temp->name = "ID";
            temp->isTerminal = True;
            temp->token = root->child_pointers[2]->token;
            temp->tempName = temp->token->lexeme;
            ast_node* child1 = create_ast(root->child_pointers[5]);
            ast_node* child2 = create_ast(root->child_pointers[6]);
            ast_node* parent = initializeNewNode();
            parent->name = "SWITCH";
            parent->isTerminal = False;
            parent->next= NULL;
            parent->no_of_children = 3;
            parent->child_pointers = (ast_node**)malloc(parent->no_of_children*sizeof(ast_node*));
            parent->child_pointers[0] = temp;
            parent->child_pointers[1] = child1;
            parent->child_pointers[2] = child2;
            free(root->child_pointers[0]);
            free(root->child_pointers[1]);
            free(root->child_pointers[3]);
            free(root->child_pointers[4]);
            free(root->child_pointers[5]);
            free(root->child_pointers[6]);
            free(root->child_pointers[7]);
            return parent;}
        case 128:
{           ast_node* child1 = create_ast(root->child_pointers[1]);
            ast_node* statement = initializeNewNode();
            ast_node* child2 = create_ast(root->child_pointers[3]);
            statement->name = "STATEMENTS";
            statement->isTerminal = False;
            statement->next= child2;
            statement->no_of_children = 0;
            ast_node* parent = initializeNewNode();
            parent->name = "CASE_HEAD";
            parent->isTerminal = False;
            parent->no_of_children = 2;
            parent->child_pointers = (ast_node**)malloc(parent->no_of_children*sizeof(ast_node*));
            parent->child_pointers[0] = child1;
            parent->child_pointers[1] = statement;
            ast_node* child3 = create_ast(root->child_pointers[6]);
            parent->next= child3;
            free(root->child_pointers[0]);
            free(root->child_pointers[1]);
            free(root->child_pointers[2]);
            free(root->child_pointers[3]);
            free(root->child_pointers[4]);
            free(root->child_pointers[5]);
            free(root->child_pointers[6]);
            return parent;}
        case 129:
{           ast_node* child1 = create_ast(root->child_pointers[1]);
            ast_node* child2 = create_ast(root->child_pointers[3]);
            ast_node* statement = initializeNewNode();
            statement->name = "STATEMENTS";
            statement->isTerminal = False;
            statement->next= child2;
            statement->no_of_children = 0;

            ast_node* parent = initializeNewNode();
            parent->name = "CASE";
            parent->isTerminal = False;
            parent->no_of_children = 2;
            parent->child_pointers = (ast_node**)malloc(parent->no_of_children*sizeof(ast_node*));
            parent->child_pointers[0] = child1;
            parent->child_pointers[1] = statement;
            ast_node* child3 = create_ast(root->child_pointers[6]);
            parent->next= child3;
            free(root->child_pointers[0]);
            free(root->child_pointers[1]);
            free(root->child_pointers[2]);
            free(root->child_pointers[3]);
            free(root->child_pointers[4]);
            free(root->child_pointers[5]);
            free(root->child_pointers[6]);
            return parent;}
        case 130:
            // Since epsilon, just free the parse tree node "eps"
            {free(root->child_pointers[0]);
            return NULL;}
        case 131:
            // Create node for terminal token
            {ast_node* parent = initializeNewNode();
            parent->name = "NUM";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            parent->tempName = parent->token->lexeme;
            return parent;}
        case 132:
            // Create node for terminal token
            {ast_node* parent = initializeNewNode();
            parent->name = "TRUE";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            parent->tempName = "true";
            return parent;}
        case 133:
            // Create node for terminal token
            {ast_node* parent = initializeNewNode();
            parent->name = "FALSE";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            parent->tempName = "false";
            return parent;}
        case 134:
{            ast_node* child1 = create_ast(root->child_pointers[2]);
            ast_node* statement = initializeNewNode();
            statement->name = "STATEMENTS";
            statement->isTerminal = False;
            statement->next= child1;
            statement->no_of_children = 0;
            ast_node* parent = initializeNewNode();
            parent->name = "DEFAULTCASE";
            parent->isTerminal = False;
            parent->next= NULL;
            parent->no_of_children = 1;
            parent->child_pointers = (ast_node**)malloc(parent->no_of_children*sizeof(ast_node*));
            parent->child_pointers[0] = statement;
            free(root->child_pointers[0]);
            free(root->child_pointers[1]);
            free(root->child_pointers[2]);
            free(root->child_pointers[3]);
            free(root->child_pointers[4]);
            return parent;}
        case 135:
            // Since epsilon, just free the parse tree node "eps"
            {free(root->child_pointers[0]);
            return NULL;}
        case 136:
{            ast_node* temp = initializeNewNode();
            temp->name = "ID";
            temp->isTerminal = True;
            temp->token = root->child_pointers[2]->token;
            temp->tempName = temp->token->lexeme;
            // root->child_pointers[1]->inh = temp; //check 
            ast_node* child1 = create_ast(root->child_pointers[4]);
            ast_node* child2 = create_ast(root->child_pointers[7]);
            ast_node* statement = initializeNewNode();
            statement->name = "STATEMENTS";
            statement->isTerminal = False;
            statement->next= child2;
            statement->no_of_children = 0;
            ast_node* parent = initializeNewNode();
            parent->name = "FORLOOP";
            parent->isTerminal = False;
            parent->next= NULL;
            parent->no_of_children = 3;
            parent->child_pointers = (ast_node**)malloc(parent->no_of_children*sizeof(ast_node*));
            parent->child_pointers[0] = temp;
            parent->child_pointers[1] = child1;
            parent->child_pointers[2] = statement;
            free(root->child_pointers[0]);
            free(root->child_pointers[1]);
            free(root->child_pointers[3]);
            free(root->child_pointers[4]);
            free(root->child_pointers[5]);
            free(root->child_pointers[6]);
            free(root->child_pointers[7]);
            free(root->child_pointers[8]);
            return parent;}
        case 137:
  {         ast_node* child1 = create_ast(root->child_pointers[2]);
            ast_node* child2 = create_ast(root->child_pointers[5]);
            ast_node* statement = initializeNewNode();
            statement->name = "STATEMENTS";
            statement->isTerminal = False;
            statement->next= child2;
            statement->no_of_children = 0;
            ast_node* parent = initializeNewNode();
            parent->name = "WHILELOOP";
            parent->isTerminal = False;
            parent->next= NULL;
            parent->no_of_children = 2;
            parent->child_pointers = (ast_node**)malloc(parent->no_of_children*sizeof(ast_node*));
            parent->child_pointers[0] = child1;
            parent->child_pointers[1] = statement;
            free(root->child_pointers[0]);
            free(root->child_pointers[1]);
            free(root->child_pointers[2]);
            free(root->child_pointers[3]);
            free(root->child_pointers[4]);
            free(root->child_pointers[5]);
            free(root->child_pointers[6]);
            return parent;}
        case 138:
{            ast_node* child1 = create_ast(root->child_pointers[0]);
            ast_node* child2 = create_ast(root->child_pointers[2]);
            ast_node* parent = initializeNewNode();
            parent->name = "FORRANGE";
            parent->isTerminal = False;
            parent->next= NULL;
            parent->no_of_children = 2;
            parent->child_pointers = (ast_node**)malloc(parent->no_of_children*sizeof(ast_node*));
            parent->child_pointers[0] = child1;
            parent->child_pointers[1] = child2;
            free(root->child_pointers[0]);
            free(root->child_pointers[1]);
            free(root->child_pointers[2]);
            return parent;}
        case 139:
{            ast_node* child1 = create_ast(root->child_pointers[0]);
            ast_node* child2 = create_ast(root->child_pointers[1]);
            ast_node* parent = initializeNewNode();
            parent->name = "FORINDEX";
            parent->isTerminal = False;
            parent->next= NULL;
            parent->no_of_children = 2;
            parent->child_pointers = (ast_node**)malloc(parent->no_of_children*sizeof(ast_node*));
            parent->child_pointers[0] = child1;
            parent->child_pointers[1] = child2;
            char* temp = (char*) malloc(sizeof(char)*5);
            sprintf(temp,"%s",child2->tempName);
            if(parent->child_pointers[0]&&!strcmp(parent->child_pointers[0]->name,"MINUS")){
                sprintf(temp,"%s%s","-",child2->tempName);
            }
            parent->tempName = temp;
            free(root->child_pointers[0]);
            free(root->child_pointers[1]);
            return parent;}

        case 140:
            // Create node for terminal token
            {ast_node* parent = initializeNewNode();
            parent->name = "NUM";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            parent->tempName = parent->token->lexeme;
            return parent;}
        case 141:
            // Create node for terminal token
            {ast_node* parent = initializeNewNode();
            parent->name = "PLUS";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;}
        case 142:
            // Create node for terminal token        
            {ast_node* parent = initializeNewNode();
            parent->name = "MINUS";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;}
        case 143:
            // Since epsilon, just free the parse tree node "eps"
            {free(root->child_pointers[0]);
            return NULL;}
    }
}


void change_tree(treeNodes* root)
/*This function modifies the parse tree so that parent has pointer to all its children
 instead of just the left most child*/
{
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

void recursive_print_tree(ast_node* root,int listcount,FILE* fp){
    int currentNodeCount = astNodeCount;
    if(root==NULL) {
        fprintf(fp,"//This node was made NULL and thereby does not exist!:");
        return;}
    
    int num_of_children = root->no_of_children;
    if(root->isTerminal && root->token!=NULL){
        if(root->token->lexeme!=NULL){
           fprintf(fp,"\n\t%s\n",root->token->lexeme);
        }
        if(!strcmp(root->token->type,"RNUM")){//TODO add
            fprintf(fp,"\n\t%f\n",root->token->values.rnum);
        }
        if(!strcmp(root->token->type,"NUM")){
            fprintf(fp,"\n\t%d\n",root->token->values.num);
        }
    }
    
    
    fprintf(fp,"\n%d\t%s\n",astNodeCount,root->name);
    fprintf(fp,"No of children: %d \n",root->no_of_children);
    fprintf(fp,"Childrens:");

    for(int i=0;i<num_of_children;i++){
        if(root->child_pointers[i]!=NULL)
        fprintf(fp,"%s ,",root->child_pointers[i]->name);
    } 
    fprintf(fp,"\n");

    fprintf(fp,"Next pointers:");
    ast_node * temp=root->next;
    Boolean emptyList = 1;
    while(temp)
    {
        if(emptyList)
            emptyList=0;
        fprintf(fp,"%s ,",temp->name);
        temp=temp->next;
    }
    if(emptyList)
        fprintf(fp,"NULL");    
    fprintf(fp,"\n");
    
    
    ast_node * temp2=root->next;
    astNodeCount++;
    for(int i=0;i<num_of_children;i++){
        fprintf(fp,"\nIterating over next child of Node %d:\n",currentNodeCount);
        recursive_print_tree(root->child_pointers[i],0,fp);
    }
    if(listcount==0){
        while(temp2)
        {
            fprintf(fp,"\nIterating over next list element of node %d:\n",currentNodeCount);
            recursive_print_tree(temp2,listcount+1,fp);
            temp2=temp2->next;
        }
    }
}


void create_abstract_tree(){
    parse_tree* ptree = get_ptree();
    change_tree(ptree->root);
    new_root = create_ast(ptree->root);
    printf("\nPRINTING AST RULES\n");
    FILE* fp;
    fp = fopen("ast_t10.txt","w+");
    if(fp == NULL)
    {
        printf("Error opening file:ast.txt\n!");   
        exit(1);             
    }
    recursive_print_tree(new_root,0,fp);   
    fclose(fp);
}

ast_node* get_ast_root(){
    return new_root;
}
