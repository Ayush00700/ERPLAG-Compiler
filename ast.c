#include "ast.h"

ast_node* copier(ast_node* temp)
/*This function returns a deep copy of the passed node*/
{
    ast_node* new_node = (ast_node*)malloc(sizeof(ast_node));
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
            // Creating AST node's children subtrees
            ast_node* child1 = create_ast(root->child_pointers[0]);
            ast_node* child2 = create_ast(root->child_pointers[1]);
            ast_node* child3 = create_ast(root->child_pointers[2]);
            ast_node* child4 = create_ast(root->child_pointers[3]);
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
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
        case 2:
            // Creating AST node's children subtrees
            ast_node* child1 = create_ast(root->child_pointers[0]);
            ast_node* child2 = create_ast(root->child_pointers[1]);
            // Inserting child1 at beginning of child2 list
            child1->next = child2;
            // Collapsing unnecessary non-terminals
            free(root->child_pointers[0]);
            free(root->child_pointers[1]);
            return child1;
        case 3:
            // Since epsilon, just free the parse tree node "eps"
            free(root->child_pointers[0]);
            return NULL;
        case 4:
            // Create node for terminal token
            ast_node* temp = (ast_node*)malloc(sizeof(ast_node));
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
            ast_node* child1 = create_ast(root->child_pointers[0]);
            ast_node* child2 = create_ast(root->child_pointers[1]);
            // Inserting child1 at the beginning of child2 list
            child1->next = child2;
            // Collapsing unnecessary non-terminals
            free(root->child_pointers[0]);
            free(root->child_pointers[1]);
            return child1;
        case 6:
            // Since epsilon, just free the parse tree node "eps"
            free(root->child_pointers[0]);
            return NULL;
        case 7:
            // Creating AST node's children subtree
            ast_node* toreturn = create_ast(root->child_pointers[4]);
            // Collapsing unnecessary non-terminals
            free(root->child_pointers[0]);
            free(root->child_pointers[1]);
            free(root->child_pointers[2]);
            free(root->child_pointers[3]);
            free(root->child_pointers[4]);
            return toreturn;
        case 8:
            // Create node for terminal token
            ast_node* temp = (ast_node*)malloc(sizeof(ast_node));
            temp->name = "ID";
            temp->isTerminal = True;
            temp->token = root->child_pointers[2]->token;
            // root->child_pointers[1]->inh = temp; //check 
            // Creating AST node's children subtrees
            ast_node* child1 = create_ast(root->child_pointers[7]);
            ast_node* child2 = create_ast(root->child_pointers[10]);
            ast_node* child3 = create_ast(root->child_pointers[11]);
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
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
            return parent;
        case 9:
            // Creating AST node's child subtree
            ast_node* child1 = create_ast(root->child_pointers[2]);
            // Collapsing unnecessary non-terminals
            free(root->child_pointers[0]);
            free(root->child_pointers[1]);
            free(root->child_pointers[2]);
            free(root->child_pointers[3]);
            free(root->child_pointers[4]);
            return child1;
        case 10:
            // Since epsilon, just free the parse tree node "eps"
            free(root->child_pointers[0]);
            return NULL;
        case 11:
            // Create node for terminal token
            ast_node* temp = (ast_node*)malloc(sizeof(ast_node));
            temp->name = "ID";
            temp->isTerminal = True;
            temp->token = root->child_pointers[0]->token;
            // Creating AST node's children subtrees
            ast_node* child1 = create_ast(root->child_pointers[2]);
            ast_node* child2 = create_ast(root->child_pointers[3]);
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            // Assigning node members to group the subtrees
            parent->name = "IPLIST";
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
            return parent;
        case 12:
            // Create node for terminal token
            ast_node* temp = (ast_node*)malloc(sizeof(ast_node));
            temp->name = "ID";
            temp->isTerminal = True;
            temp->token = root->child_pointers[1]->token;
            // Creating AST node's children subtrees
            ast_node* child1 = create_ast(root->child_pointers[3]);
            ast_node* child2 = create_ast(root->child_pointers[4]);
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
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
            return parent;
        case 13:
            // Since epsilon, just free the parse tree node "eps"
            free(root->child_pointers[0]);
            return NULL;
        case 14:
            // Create node for terminal token
            ast_node* temp = (ast_node*)malloc(sizeof(ast_node));
            temp->name = "ID";
            temp->isTerminal = True;
            temp->token = root->child_pointers[0]->token;
            // Creating AST node's children subtrees
            ast_node* child1 = create_ast(root->child_pointers[2]);
            ast_node* child2 = create_ast(root->child_pointers[3]);
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            // Assigning node members to group the subtrees
            parent->name = "OPLIST";
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
            return parent;
        case 15:
            // Create node for terminal token
            ast_node* temp = (ast_node*)malloc(sizeof(ast_node));
            temp->name = "ID";
            temp->isTerminal = True;
            temp->token = root->child_pointers[1]->token;
            // Creating AST node's children subtrees
            ast_node* child1 = create_ast(root->child_pointers[3]);
            ast_node* child2 = create_ast(root->child_pointers[4]);
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
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
            return parent;
        case 16:
            // Since epsilon, just free the parse tree node "eps"
            free(root->child_pointers[0]);
            return NULL;
        case 17:
            // Create node for terminal token
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "INTEGER";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;
        case 18:    
            // Create node for terminal token
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "REAL";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;
        case 19:
            // Create node for terminal token
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "BOOLEAN";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;
        case 20:
            // Creating AST node's children subtrees
            ast_node* child1 = create_ast(root->child_pointers[2]);
            ast_node* child2 = create_ast(root->child_pointers[5]);
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
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
            return parent;
        case 21:
            // Creating AST node's children subtrees
            ast_node* child1 = create_ast(root->child_pointers[0]);
            ast_node* child2 = create_ast(root->child_pointers[2]);
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
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
            return parent;
        case 22:
            // Create node for terminal token
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "INTEGER";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;
        case 23:
            // Create node for terminal token
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "REAL";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;
        case 24:
            // Create node for terminal token
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "BOOLEAN";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;
        case 25:
            // Creating AST child subtree
            ast_node* parent = create_ast(root->child_pointers[1]);
            // Collapsing unnecessary non-terminals
            free(root->child_pointers[0]);
            free(root->child_pointers[1]);
            free(root->child_pointers[2]);
            return parent;
        case 26:
            // Creating AST node's children subtree
            ast_node* child1 = create_ast(root->child_pointers[0]);
            ast_node* child2 = create_ast(root->child_pointers[1]);
            // Inserting child1 to the beginning of child2 list
            child1->next = child2;
            // Collapsing unnecessary non-terminals
            free(root->child_pointers[0]);
            free(root->child_pointers[1]);
            return child1;
        case 27:
            // Since epsilon, just free the parse tree node "eps"
            free(root->child_pointers[0]);
            return NULL;
        case 28:
            // Create AST subtree
            ast_node* child1 = create_ast(root->child_pointers[0]);
            // Collapsing unnecessary non-terminals
            free(root->child_pointers[0]);
            return child1;
        case 29:
            // Create AST subtree
            ast_node* child1 = create_ast(root->child_pointers[0]);
            // Collapsing unnecessary non-terminals
            free(root->child_pointers[0]);
            return child1;
        case 30:
            // Create AST subtree
            ast_node* child1 = create_ast(root->child_pointers[0]);
            // Collapsing unnecessary non-terminals
            free(root->child_pointers[0]);
            return child1;
        case 31:
            // Create AST subtree
            ast_node* child1 = create_ast(root->child_pointers[0]);
            // Collapsing unnecessary non-terminals
            free(root->child_pointers[0]);
            return child1;
        case 32:
            // Create AST subtree
            ast_node* child1 = create_ast(root->child_pointers[0]);
            // Collapsing unnecessary non-terminals
            free(root->child_pointers[0]);
            return child1;
        case 33:
            // Create node for terminal token
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "INPUT";
            parent->isTerminal = True;
            parent->token = root->child_pointers[2]->token;
            // Collapsing unnecessary non-terminals
            free(root->child_pointers[0]);
            free(root->child_pointers[1]);
            free(root->child_pointers[3]);
            free(root->child_pointers[4]);
            return parent;
        case 34:
            // Create AST subtree
            ast_node* child1 = create_ast(root->child_pointers[2]);
            // Discuss if we require the node
            // Maybe required for ignoring this type of io in
            // case of type checking - CHECK
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
            return parent;
        case 35:
            // Create node for terminal token
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "TRUE";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;
        case 36:
            // Create node for terminal token
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "FALSE";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;
        case 37:
        // Create node for terminal token
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "ID";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;
        case 38:
            // Create node for terminal token
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "NUM";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;
        case 39:
            // Create node for terminal token
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "RNUM";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;
        case 40:{
            break;
        }
        case 41:
            // Create node for terminal token
            ast_node* temp = (ast_node*)malloc(sizeof(ast_node));
            temp->name = "ID";
            temp->isTerminal = True;
            temp->token = root->child_pointers[0]->token;
            // This node is required in the sibling subtree
            // So we pass as inherited attribute
            root->child_pointers[1]->inh = temp;
            // Creating AST node child subtree
            ast_node* child1 = create_ast(root->child_pointers[1]);
            // Collapsing unnecessary non-terminals
            free(root->child_pointers[1]);
            return child1;
        case 42:
            // Create node for terminal token
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "NUM";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;
        case 43:
            // Create node for terminal token
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "RNUM";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;
        case 44:
            // Create AST subtree
            ast_node* child1 = create_ast(root->child_pointers[0]);
            // Collapsing unnecessary non-terminals
            free(root->child_pointers[0]);
            return child1;
        case 45:
            // Creating AST node's child subtree
            ast_node* child1 = create_ast(root->child_pointers[1]);
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
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
            return parent;
        case 46:
            // Since epsilon, just free the parse tree node "eps"
            free(root->child_pointers[0]);
            // Return inherited attribute as the synthesized attribute
            return copier(root->inh);
        case 47:
            // Maybe we need a node to indicate the type of simplestatement
            // Create AST subtree
            ast_node* child1 = create_ast(root->child_pointers[0]);
            // Collapsing unnecessary non-terminals
            free(root->child_pointers[0]);
            return child1;
        case 48:
            // Maybe we need a node to indicate the type of simplestatement
            // Create AST subtree
            ast_node* child1 = create_ast(root->child_pointers[0]);
            // Collapsing unnecessary non-terminals
            free(root->child_pointers[0]);
            return child1;
        case 49:
            // Create node for terminal token
            ast_node* temp = (ast_node*)malloc(sizeof(ast_node));
            temp->name = "ID";
            temp->isTerminal = True;
            temp->token = root->child_pointers[0]->token;
            // This node is required in the sibling subtree
            // So we pass as inherited attribute
            root->child_pointers[1]->inh = temp;
            // Create AST subtree
            ast_node* child1 = create_ast(root->child_pointers[1]);
            // Collapsing unnecessary non-terminals
            free(root->child_pointers[1]);
            return child1;
        case 50:
            // Assign inherited attribute of current node as that of the parent
            root->child_pointers[0]->inh = root->inh;
            // Create AST subtree
            ast_node* temp = create_ast(root->child_pointers[0]);
            // Collapsing unnecessary non-terminals
            free(root->child_pointers[0]);
            return temp;
        case 51:
            // Assign inherited attribute of current node as that of the parent
            root->child_pointers[0]->inh = root->inh;
            // Create AST subtree
            ast_node* temp = create_ast(root->child_pointers[0]);
            // Collapsing unnecessary non-terminals
            free(root->child_pointers[0]);
            return temp;
        case 52:
            // Create AST node
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
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
            return parent;
        case 53:
            // Create node for left child of AST node
            ast_node* left = (ast_node*)malloc(sizeof(ast_node));
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
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
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
            return parent;
        case 54:
            // Create AST node
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
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
            return parent;
        case 55:
            // Create node for terminal token
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "NUM";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;
        case 56:
            // Create node for terminal token
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "ID";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;
        case 57:
            // Create node for terminal token
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "PLUS";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;
        case 58:
            // Create node for terminal token
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "MINUS";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;
        case 59:
            // Since epsilon, just free the parse tree node "eps"
            free(root->child_pointers[0]);
            return NULL;
        case 60:
            // Create node for terminal token
            ast_node* temp = (ast_node*)malloc(sizeof(ast_node));
            temp->name = "ID";
            temp->isTerminal = True;
            temp->token = root->child_pointers[3]->token;
            // Creating AST node's children subtrees
            ast_node* child1 = create_ast(root->child_pointers[0]);
            ast_node* child2 = create_ast(root->child_pointers[6]);
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
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
            return parent;
        case 61:
            // Creating AST node's children subtrees
            ast_node* child1 = create_ast(root->child_pointers[0]);
            ast_node* child2 = create_ast(root->child_pointers[1]);
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
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
            return parent;
        case 62:
            // Creating AST node's children subtrees
            ast_node* child1 = create_ast(root->child_pointers[1]);
            ast_node* child2 = create_ast(root->child_pointers[2]);
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
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
            return parent;
        case 63:
            // Since epsilon, just free the parse tree node "eps"
            free(root->child_pointers[0]);
            return NULL;
        case 64:
            // Create node for terminal token
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "NUM";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;
        case 65:
            // Create node for terminal token
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "RNUM";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;
        case 66:
            // Create AST subtree
            ast_node* child1 = create_ast(root->child_pointers[0]);
            // Collapsing unnecessary non-terminals
            free(root->child_pointers[0]);
            return child1;
        case 67:
            // Create node for terminal token
            ast_node* temp = (ast_node*)malloc(sizeof(ast_node));
            temp->name = "ID";
            temp->isTerminal = True;
            temp->token = root->child_pointers[0]->token;
            // Creating AST node's child subtree
            ast_node* child1 = create_ast(root->child_pointers[1]);
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
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
            return parent;
        case 68:
            // Create AST subtree
            ast_node* child1 = create_ast(root->child_pointers[1]);
            // Collapsing unnecessary non-terminals
            free(root->child_pointers[0]);
            free(root->child_pointers[1]);
            free(root->child_pointers[2]);
            free(root->child_pointers[3]);
            return child1;

        case 69:
            // Since epsilon, just free the parse tree node "eps"
            free(root->child_pointers[0]);
            return NULL;
        case 70:
            // Create node for terminal token
            ast_node* temp = (ast_node*)malloc(sizeof(ast_node));
            temp->name = "ID";
            temp->isTerminal = True;
            temp->token = root->child_pointers[0]->token;
            // Create AST subtree
            ast_node* child1 = create_ast(root->child_pointers[1]);
            temp->next = child1;
            // Collapsing unnecessary non-terminals
            free(root->child_pointers[1]);
            return temp;
        case 71:
            ast_node* temp = (ast_node*)malloc(sizeof(ast_node));
            temp->name = "ID";
            temp->isTerminal = True;
            temp->token = root->child_pointers[1]->token;
            ast_node* child1 = create_ast(root->child_pointers[2]);
            temp->next = child1;
            free(root->child_pointers[0]);
            free(root->child_pointers[1]);
            // free(root->child_pointers[2]); /--> missed this?
            return temp;
        case 72:
            // Since epsilon, just free the parse tree node "eps"
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
            // Create node for terminal token
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "PLUS";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;
        case 77:
            // Create node for terminal token
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "MINUS";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;}
        case 78:
       {     ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
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
        {    ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
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
        {    ast_node* left = (ast_node*)malloc(sizeof(ast_node));
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
           { ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
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
{            ast_node* left = (ast_node*)malloc(sizeof(ast_node));
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
{            ast_node* left = (ast_node*)malloc(sizeof(ast_node));
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
            // Since epsilon, just free the parse tree node "eps"
            free(root->child_pointers[0]);
            // Return inherited attribute as the synthesized attribute
            return root->inh;
        case 93:
       {     ast_node* temp = create_ast(root->child_pointers[1]);
            free(root->child_pointers[0]);
            free(root->child_pointers[1]);
            free(root->child_pointers[2]);
            return temp;}
        case 94:
            // Create node for terminal token
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "NUM";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;}
        case 95:
            // Create node for terminal token
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "RNUM";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;}
        case 96:
 {           ast_node* temp = (ast_node*)malloc(sizeof(ast_node));
            temp->name = "ID";
            temp->isTerminal = True;
            temp->token = root->child_pointers[0]->token;
            root->child_pointers[1]->inh = temp;
            ast_node* child1 = create_ast(root->child_pointers[1]);
            free(root->child_pointers[1]);
            return child1;}
        case 97:
 {           ast_node*   parent = (ast_node*)malloc(sizeof(ast_node));
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
            // Since epsilon, just free the parse tree node "eps"
            free(root->child_pointers[0]);
            // Return inherited attribute as the synthesized attribute
            return root->inh;
        // case 99: (REDUNDANT)
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
    {        parent->name = "SIGNED_NUMBER";
            parent->isTerminal = False;
            parent->no_of_children = 2;
            parent->child_pointers = (ast_node**)malloc(parent->no_of_children*sizeof(ast_node*));
            parent->next = NULL;
            parent->child_pointers[0] = root->inh;
            parent->child_pointers[1] = create_ast(root->child_pointers[0]);
            free(root->child_pointers[0]);
            return parent;}
        case 103:
 {           parent->name = "SIGNED_EXPRESSION";
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
{            ast_node* left = (ast_node*)malloc(sizeof(ast_node));
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
            free(root->child_pointers[0]);
            // Return inherited attribute as the synthesized attribute
            return root->inh;
        case 107:
{            ast_node* temp = create_ast(root->child_pointers[0]);
            root->child_pointers[1]->inh = temp;
            free(root->child_pointers[0]);
            ast_node* parent = create_ast(root->child_pointers[1]);
            free(root->child_pointers[1]);
            return parent;}
        case 108:
{            ast_node* left = (ast_node*)malloc(sizeof(ast_node));
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
            free(root->child_pointers[0]);
            // Return inherited attribute as the synthesized attribute
            return root->inh;
        case 110:
            // Create node for terminal token
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "ID";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;}
        case 111:
            // Create node for terminal token
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "NUM";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;}
        case 112:
    {        ast_node* parent = create_ast(root->child_pointers[0]);
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
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "PLUS";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;}
        case 115:
            // Create node for terminal token
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "MINUS";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;}
        case 116:
            // Create node for terminal token
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "MUL";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;}
        case 117:
            // Create node for terminal token
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "DIV";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;}
        case 118:
            // Create node for terminal token
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "AND";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;}
        case 119:
            // Create node for terminal token
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "OR";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;}
        case 120:
            // Create node for terminal token
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "LT";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;}
        case 121:
            // Create node for terminal token
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "LE";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;}
        case 122:
            // Create node for terminal token
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "GT";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;}
        case 123:
            // Create node for terminal token
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "GE";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;}
        case 124:
            // Create node for terminal token
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "EQ";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;}
        case 125:
            // Create node for terminal token
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "NE";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;}

        case 126:
  {          ast_node* child1 = create_ast(root->child_pointers[1]);
            ast_node* child2 = create_ast(root->child_pointers[3]);
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
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
{            ast_node* temp = (ast_node*)malloc(sizeof(ast_node));
            temp->name = "ID";
            temp->isTerminal = True;
            temp->token = root->child_pointers[2]->token;
            ast_node* child1 = create_ast(root->child_pointers[5]);
            ast_node* child2 = create_ast(root->child_pointers[6]);
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
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
{            ast_node* child1 = create_ast(root->child_pointers[1]);
            ast_node* child2 = create_ast(root->child_pointers[3]);
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "CASE";
            parent->isTerminal = False;
            parent->no_of_children = 2;
            parent->child_pointers = (ast_node**)malloc(parent->no_of_children*sizeof(ast_node*));
            parent->child_pointers[0] = child1;
            parent->child_pointers[1] = child2;
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
{            ast_node* child1 = create_ast(root->child_pointers[1]);
            ast_node* child2 = create_ast(root->child_pointers[3]);
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "CASE";
            parent->isTerminal = False;
            parent->no_of_children = 2;
            parent->child_pointers = (ast_node**)malloc(parent->no_of_children*sizeof(ast_node*));
            parent->child_pointers[0] = child1;
            parent->child_pointers[1] = child2;
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
            free(root->child_pointers[0]);
            return NULL;
        case 131:
            // Create node for terminal token
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "NUM";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;}
        case 132:
            // Create node for terminal token
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "TRUE";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;}
        case 133:
            // Create node for terminal token
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "FALSE";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;}
        case 134:
{            ast_node* child1 = create_ast(root->child_pointers[2]);
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "DEFAULTCASE";
            parent->isTerminal = False;
            parent->next= NULL;
            parent->no_of_children = 1;
            parent->child_pointers = (ast_node**)malloc(parent->no_of_children*sizeof(ast_node*));
            parent->child_pointers[0] = child1;
            free(root->child_pointers[0]);
            free(root->child_pointers[1]);
            free(root->child_pointers[2]);
            free(root->child_pointers[3]);
            free(root->child_pointers[4]);
            return parent;}
        case 135:
            // Since epsilon, just free the parse tree node "eps"
            free(root->child_pointers[0]);
            return NULL;
        case 136:
{            ast_node* temp = (ast_node*)malloc(sizeof(ast_node));
            temp->name = "ID";
            temp->isTerminal = True;
            temp->token = root->child_pointers[2]->token;
            // root->child_pointers[1]->inh = temp; //check 
            ast_node* child1 = create_ast(root->child_pointers[4]);
            ast_node* child2 = create_ast(root->child_pointers[7]);
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "FOR";
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
            free(root->child_pointers[8]);
            return parent;}
        case 137:
  {          ast_node* child1 = create_ast(root->child_pointers[2]);
            ast_node* child2 = create_ast(root->child_pointers[5]);
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "WHILE";
            parent->isTerminal = False;
            parent->next= NULL;
            parent->no_of_children = 2;
            parent->child_pointers = (ast_node**)malloc(parent->no_of_children*sizeof(ast_node*));
            parent->child_pointers[0] = child1;
            parent->child_pointers[1] = child2;
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
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
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
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "FORINDEX";
            parent->isTerminal = False;
            parent->next= NULL;
            parent->no_of_children = 2;
            parent->child_pointers = (ast_node**)malloc(parent->no_of_children*sizeof(ast_node*));
            parent->child_pointers[0] = child1;
            parent->child_pointers[1] = child2;
            free(root->child_pointers[0]);
            free(root->child_pointers[1]);
            return parent;}

        case 140:
            // Create node for terminal token
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "NUM";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;}
        case 141:
            // Create node for terminal token
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "PLUS";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;}
        case 142:
            // Create node for terminal token        
            ast_node* parent = (ast_node*)malloc(sizeof(ast_node));
            parent->name = "MINUS";
            parent->isTerminal = True;
            parent->token = root->child_pointers[0]->token;
            return parent;}
        case 143:
            // Since epsilon, just free the parse tree node "eps"
            free(root->child_pointers[0]);
            return NULL;

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

int main(){
    parse_tree* ptree = get_ptree();
    change_tree(ptree->root);
    ast_node* new_root = create_ast(ptree->root);
}   