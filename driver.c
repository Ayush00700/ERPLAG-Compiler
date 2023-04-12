/* Group-20
--------------------
1. Rajan Sahu       2019B4A70572P
2. Yash Goyal       2019B4A70638P
3. Ayush Agarwal    2019B4A70652P
4. Vasu Swaroop     2019B4A70656P
5. A Sudarshan      2019B4A70744P
*/

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "codegen.h"
#include <string.h>

extern func_entry* global_TABLE[TABLE_SIZE];
extern int SEMANTIC_ERRORS;
extern int LEXICAL_ERRORS;
extern int PARSER_ERRORS;



void print_file(char* filename){


    FILE* fp = fopen(filename, "r");
    if(fp == NULL){
        return;
    }
    char c = fgetc(fp);

    while(c!=EOF){

        printf("%c",c);
        c = fgetc(fp);
    }

    fclose(fp);

}



int main(int argc, char* args[]){ //DRIVER
    clock_t l_start_time, l_end_time;
    double l_total_CPU_time, l_total_CPU_time_in_seconds;
    int lflag=0;
    int pflag=0;
    char* buffer = args[1];
    int bufferSize = atoi(args[2]);
    int run = 1;
    int third = 0; 
    int size_of_parse_tree_nodes = -1;
    int num_of_parse_tree_nodes = -1;
    int size_of_ast_nodes = -1;
    int num_of_ast_nodes = -1;

    printf("GROUP MEMBERS:\nName\t\t\tID\n");
    printf("Ayush Agarwal\t\t2019B4A70652P\n");
    printf("Rajan Sahu\t\t2019B4A70572P\n");
    printf("Yash Goyal\t\t2019B4A70638P\n");
    printf("Vasu Swaroop\t\t2019B4A70656P\n");
    printf("A Sudarshan\t\t2019B4A70744P\n\n\n");

    printf("SHORT DESCRIPTION\n");
    printf("(a) FIRST and FOLLOW set have been automated\n");
    printf("(b) Both lexical and syntax analysis modules implemented\n");
    printf("(c) Module works for all test cases\n");
    printf("(d) Module handles errors efficiently and gives correct Parse Tree\n");
    printf("(e) No Segmentation Fault\n\n\n");

    l_start_time = clock();
    FILE* fp;
    fp = fopen(buffer,"r");
    if(fp == NULL)
    {
        printf("Error opening file:%s\n!",buffer);   
        exit(1);             
    }
    printf("Running file %s\n", buffer);
    populate_hash_table();
    call_lexer(fp,4096); //TODO VARIABLE ARG
    int a = postProcessing();
    if(!a){
        printf("Found no code\n");
        exit(0);
    }
    fclose(fp);
    int num = parseCompletely(0);
    if(!LEXICAL_ERRORS && !PARSER_ERRORS){
        num_of_parse_tree_nodes = num;
        size_of_parse_tree_nodes = num_of_parse_tree_nodes*sizeof(treeNodes);
        num_of_ast_nodes = create_abstract_tree(); //creation of abstract syntax tree 
        size_of_ast_nodes = sizeof(ast_node)*num_of_ast_nodes;
        FILE* f_mem = fopen("Memory_consumption.txt","w");
        fprintf(f_mem,"Parse tree Number of nodes = %d, Allocated Memory = %d Bytes\n",num_of_parse_tree_nodes,size_of_parse_tree_nodes);
        fprintf(f_mem,"AST Number of nodes = %d, Allocated Memory = %d Bytes",num_of_ast_nodes,size_of_ast_nodes);
        double anse = size_of_parse_tree_nodes-size_of_ast_nodes;
        anse = anse/(size_of_parse_tree_nodes);
        anse = anse*100;
        fprintf(f_mem,"Compression percentage = %lf",anse);
        fclose(f_mem);
        semantic();
        ast_node* ast_root = get_ast_root();
        get_global_symbol_table(ast_root);
        if(!SEMANTIC_ERRORS){
            print_symbol_table();
            // symbol_table.txt
            print_activation();
            // activation.txt
            print_static_dynamic_arrays();
            // static_dynamic_array.txt
            // code generation done
            FILE* fptr = fopen("intermediate_code.txt","w");
            ir_code* intermediate_code =  getIRList(ast_root,global_TABLE);
            print_ir_code(fptr,intermediate_code);
            fclose(fptr);

            FILE* assembly = fopen("assembly.asm", "w");
            starter(assembly,intermediate_code);
        }
        l_end_time = clock();
        l_total_CPU_time = (double) (l_end_time - l_start_time);
        l_total_CPU_time_in_seconds = l_total_CPU_time / CLOCKS_PER_SEC;
    }
    
    
    while(run){
        printf("\n################WELCOME TO ERPLAG COMPILER MENU######################\n\n");
        printf("OPTIONS\n");
        printf("\n1. Printing the token list generated by the lexer\n");
        printf("2. Parsing to verify the syntactic correctness of the input source code and producing parse tree\n");
        printf("3. For printing the Abstract Syntax Tree in appropriate format.\n");
        printf("4. Memory usage printing\n");
        printf("5. Symbol table printing\n");
        printf("6. Activation Record Printing\n");
        printf("7. Static and Dynamic Array Printing\n");
        printf("8. Errors reporting and total compiling time\n");
        printf("9. print the assembly code\n");
        printf("0. EXIT\n");
        int choice;
        printf("Enter your choice: ");
        scanf("%d",&choice);
        fputs("\033c", stdout); //CLEARS STDOUT SCREEN
        switch (choice)
        {
            case 0:{
                run = 0;
                remove("parse_tree.txt");
                remove("ast.txt");
                remove("Memory_consumption.txt");
                remove("parsing_errors.txt");
                remove("semantic_errors.txt");
                remove("intermediate_code.txt");
                remove("symbol_table.txt");
                remove("activation.txt");
                remove("static_dynamic_array.txt");
                break;
            }
            /*Token List by LEXER with Errors*/
            case 1:
            {
                printf("TOKENS ARE .... (first to last) \n");
                printtokens();
                printf("LEXICAL ERRORS ARE .... (first to last) \n");
                pop_error_tokens();
                printf("\nTask 1 done, again going to Options\n");
                break;
            }
            /*Running the parser*/
            case 2:
            {
                if(PARSER_ERRORS){
                    printf("There were parsing errors\n");
                }
                // print the parse tree
               
                print_file("parse_tree.txt");
                printf("\nTask 2 done, again going to Options\n");
                break;

            }
            case 3:
            {
                if(LEXICAL_ERRORS){
                    printf("There were lexical errors\n");
                    printf("\nTask 3 done, again going to Options\n");
                    break;
                }
                if(PARSER_ERRORS){
                    printf("There were parsing errors\n");
                    printf("\nTask 3 done, again going to Options\n");
                    break;
                }
                // print the ast tree
                print_file("ast.txt");
                printf("\nTask 3 done, again going to Options\n");
                break;
            }
            case 4:
            {
                if(LEXICAL_ERRORS){
                    printf("There were lexical errors\n");
                    printf("\nTask 4 done, again going to Options\n");
                    break;
                }
                if(PARSER_ERRORS){
                    printf("There were parsing errors\n");
                    printf("\nTask 4 done, again going to Options\n");
                    break;
                }
                // print memory file
                print_file("Memory_consumption.txt");
                printf("\nTask 4 done, again going to Options\n");
                break;
            }
            case 5:
            {
                if(LEXICAL_ERRORS){
                    printf("There were lexical errors\n");
                    printf("\nTask 5 done, again going to Options\n");
                    break;
                }
                if(PARSER_ERRORS){
                    printf("There were parsing errors\n");
                    printf("\nTask 5 done, again going to Options\n");
                    break;
                }
                if(SEMANTIC_ERRORS){
                    printf("There were semantic errors\n");
                    printf("\nTask 5 done, again going to Options\n");
                    break;
                }
                print_file("symbol_table.txt");
                printf("\nTask 5 done, again going to Options\n");
                break;
            }
            case 6:
            {
                if(LEXICAL_ERRORS){
                    printf("There were lexical errors\n");
                    printf("\nTask 6 done, again going to Options\n");
                    break;
                }
                if(PARSER_ERRORS){
                    printf("There were parsing errors\n");
                    printf("\nTask 6 done, again going to Options\n");
                    break;
                }
                if(SEMANTIC_ERRORS){
                    printf("There were semantic errors\n");
                    printf("\nTask 6 done, again going to Options\n");
                    break;
                }
                print_file("activation.txt");
                printf("\nTask 6 done, again going to Options\n");
                break;
            }
            case 7:
            {
                if(LEXICAL_ERRORS){
                    printf("There were lexical errors\n");
                    printf("\nTask 7 done, again going to Options\n");
                    break;
                }
                if(PARSER_ERRORS){
                    printf("There were parsing errors\n");
                    printf("\nTask 7 done, again going to Options\n");
                    break;
                }
                if(SEMANTIC_ERRORS){
                    printf("There were semantic errors\n");
                    printf("\nTask 7 done, again going to Options\n");
                    break;
                }
                print_file("static_dynamic_array.txt");
                printf("\nTask 7 done, again going to Options\n");
                break;
            }
            case 8:
            {
                if(PARSER_ERRORS){
                    printf("FOLLOWING ARE THE SYNTACTIC ERRORS\n");
                    print_file("parsing_errors.txt");
                }
                else{
                    printf("FOLLOWING ARE THE SEMANTIC ERRORS\n");
                    print_file("semantic_errors.txt");
                }
                //print the total print time
                printf("Total CPU Time (in micro-seconds):%f\n",l_total_CPU_time);
                printf("Total CPU Time (in seconds):%f\n",l_total_CPU_time_in_seconds);
                printf("\nTask 8 done, again going to Options\n");
                break;
            }
            case 9:
            {
                if(LEXICAL_ERRORS){
                    printf("lexical errors \n");
                    break;
                }
                else if(PARSER_ERRORS){
                    printf("syntax errors \n");
                    break;
                }
                else if(SEMANTIC_ERRORS){
                    printf("semantic errors \n");
                    break;
                }
                else{
                    printf("CODEGEN IS SUCCESSFULL\n");
                    printf("FOLLOWING IS THE IR CODE \n");
                    print_file("intermediate_code.txt\n");
                    printf("\nAssembly code is saved in file ((assembly.asm))\n");
                    printf("Total CPU Time:%f\n",l_total_CPU_time);
                    printf("Total CPU Time (in seconds):%f\n",l_total_CPU_time_in_seconds);
                    printf("\nTask 9 done, again going to Options\n");
                }
                break;
            }
            default:
                printf("Wrong input, Enter Again\n");
        }
    }
}

