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

int main(int argc, char* args[]){ //DRIVER
    clock_t l_start_time, l_end_time;
    double l_total_CPU_time, l_total_CPU_time_in_seconds;
    clock_t p_start_time, p_end_time;
    double p_total_CPU_time, p_total_CPU_time_in_seconds;
    int lflag=0;
    int pflag=0;
    char* buffer = args[1];
    // strcpy(buffer,args[1]);
    // char* parseBuffer = args[2];
    int bufferSize = atoi(args[2]);
    // printf("arg[1]:%s\n",buffer);
    // printf("arg[2]:%s\n",parseBuffer);
    // printf("arg[3]:%d\n",bufferSize);
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
    while(run){
        //run=0;//COMMENT OUT LATER TODO
    printf("################WELCOME TO ERPLAG COMPILER MENU######################\n\n");
    printf("OPTIONS\n");
        printf("\n1. Printing code without comments\n");
        printf("2. Token List by LEXER (with Errors)\n");
        printf("3. Parsing and Displaying PARSE TREE by PARSER (with Errors)\n");
        printf("4. For printing total time taken by LEXER+PARSER\n");
        printf("0. EXIT\n");
        int choice;

        // For testing purpose -->
        // int choice = 4;
        // char buffer[50] = "t1.txt";
        // char parseBuffer[50] = "o1.txt";

      
        printf("Enter your choice: ");
        scanf("%d",&choice);
        fputs("\033c", stdout); //CLEARS STDOUT SCREEN
        switch (choice)
        {
            case 0:{
                run = 0;
                break;
}
            /*Token List by LEXER with Errors*/
            case 1:{
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
                printf("TOKENS ARE .... (first to last) \n");
                printtokens();
                printf("LEXICAL ERRORS ARE .... (first to last) \n");
                pop_error_tokens();    

                printf("\nTask 1 done, again going to Options\n");
                lflag=1;
                fclose(fp);
                break;}
            /*Running the parser*/
            case 2:{
                if(lflag==0){
                    //l_start_time = clock();
                    FILE* fp;
                    fp = fopen(buffer,"r");
                    if(fp == NULL)
                    {
                        printf("Error opening file:%s \n",buffer);   
                        exit(1);             
                    }

                    printf("Running file %s \n", buffer);
                    populate_hash_table();
                    call_lexer(fp,4096);//todo variable arg
                    int a = postProcessing();
                    if(!a){
                        printf("Found no code\n");
                        exit(0);
                    }
                    printf("TOKENS ARE .... (first to last) \n");
                    printtokens();
                    printf("LEXICAL ERRORS ARE .... (first to last) \n");
                    pop_error_tokens();    
                    lflag=1;
                    // l_end_time = clock();
                    // l_total_CPU_time = (double) (l_end_time - l_start_time);
                    // l_total_CPU_time_in_seconds = l_total_CPU_time / CLOCKS_PER_SEC;
                    fclose(fp);
                }
                else{
                    printf("TOKENS ARE .... (first to last) \n");
                    printtokens();
                    printf("LEXICAL ERRORS ARE .... (first to last) \n");
                    pop_error_tokens();    
                }
                // p_start_time = clock();
                if(!pflag){
                parseCompletely(lflag);
                pflag = 1;
                }
                // p_end_time = clock();
                // p_total_CPU_time = (double) (p_end_time - p_start_time);
                // p_total_CPU_time_in_seconds = p_total_CPU_time / CLOCKS_PER_SEC;
                /*Parsing and Displaying PARSE TREE by PARSER (with Errors)*/
                if(!LEXICAL_ERRORS || !PARSER_ERRORS){
                num_of_parse_tree_nodes = print_parse_tree();
                size_of_parse_tree_nodes = num_of_parse_tree_nodes*sizeof(treeNodes);
                }
                else{
                    printf("there were lexical or parsing errors due to which parsetree was not printed\n");
                }
                printf("\nTask 2 done, again going to Options\n");
                break;}
            case 3:{
                if(lflag==0){
                    //l_start_time = clock();
                    FILE* fp;
                    fp = fopen(buffer,"r");
                    if(fp == NULL)
                    {
                        printf("Error opening file:%s \n",buffer);   
                        exit(1);             
                    }

                    printf("Running file %s \n", buffer);
                    populate_hash_table();
                    call_lexer(fp,4096);//todo variable arg
                    int a = postProcessing();
                    if(!a){
                        printf("Found no code\n");
                        exit(0);
                    }
                    printf("TOKENS ARE .... (first to last) \n");
                    printtokens();
                    printf("LEXICAL ERRORS ARE .... (first to last) \n");
                    pop_error_tokens();    
                    lflag=1;
                    // l_end_time = clock();
                    // l_total_CPU_time = (double) (l_end_time - l_start_time);
                    // l_total_CPU_time_in_seconds = l_total_CPU_time / CLOCKS_PER_SEC;
                    fclose(fp);
                }
                else{
                    printf("TOKENS ARE .... (first to last) \n");
                    printtokens();
                    printf("LEXICAL ERRORS ARE .... (first to last) \n");
                    pop_error_tokens();    
                }
                // p_start_time = clock();
                if(!pflag){
                parseCompletely(lflag);
                pflag = 1;
                }
                // p_end_time = clock();
                // p_total_CPU_time = (double) (p_end_time - p_start_time);
                // p_total_CPU_time_in_seconds = p_total_CPU_time / CLOCKS_PER_SEC;
                /*Parsing and Displaying PARSE TREE by PARSER (with Errors)*/
                if(!LEXICAL_ERRORS || !PARSER_ERRORS){
                num_of_parse_tree_nodes = print_parse_tree();
                size_of_parse_tree_nodes = num_of_parse_tree_nodes*sizeof(treeNodes);
                }
                else{
                    printf("there were lexical or parsing errors due to which parsetree was not printed\n");
                }
                // if(!aflag)
                {
                    num_of_ast_nodes = create_abstract_tree();
                    // aflag=1;
                } //creation of abstract syntax tree 
                size_of_ast_nodes = sizeof(ast_node)*num_of_ast_nodes;
                printf("\nTask 3 done, again going to Options\n");
                break;}
            case 4:{
                if(lflag==0){
                    //l_start_time = clock();
                    FILE* fp;
                    fp = fopen(buffer,"r");
                    if(fp == NULL)
                    {
                        printf("Error opening file:%s \n",buffer);   
                        exit(1);             
                    }

                    printf("Running file %s \n", buffer);
                    populate_hash_table();
                    call_lexer(fp,4096);//todo variable arg
                    int a = postProcessing();
                    if(!a){
                        printf("Found no code\n");
                        exit(0);
                    }
                    printf("TOKENS ARE .... (first to last) \n");
                    printtokens();
                    printf("LEXICAL ERRORS ARE .... (first to last) \n");
                    pop_error_tokens();    
                    lflag=1;
                    // l_end_time = clock();
                    // l_total_CPU_time = (double) (l_end_time - l_start_time);
                    // l_total_CPU_time_in_seconds = l_total_CPU_time / CLOCKS_PER_SEC;
                    fclose(fp);
                }
                else{
                    printf("TOKENS ARE .... (first to last) \n");
                    printtokens();
                    printf("LEXICAL ERRORS ARE .... (first to last) \n");
                    pop_error_tokens();    
                }
                // p_start_time = clock();
                parseCompletely(lflag);
                // p_end_time = clock();
                // p_total_CPU_time = (double) (p_end_time - p_start_time);
                // p_total_CPU_time_in_seconds = p_total_CPU_time / CLOCKS_PER_SEC;
                /*Parsing and Displaying PARSE TREE by PARSER (with Errors)*/
                num_of_parse_tree_nodes = print_parse_tree();
                size_of_parse_tree_nodes = num_of_parse_tree_nodes*sizeof(treeNodes);
                if(LEXICAL_ERRORS || PARSER_ERRORS){
                    return -1;
                }
                num_of_ast_nodes = create_abstract_tree(); //creation of abstract syntax tree 
                size_of_ast_nodes = sizeof(ast_node)*num_of_ast_nodes;
                printf("Parse tree Number of nodes = %d, Allocated Memory = %d Bytes\n",num_of_parse_tree_nodes,size_of_parse_tree_nodes);
                printf("AST Number of nodes = %d, Allocated Memory = %d Bytes",num_of_ast_nodes,size_of_ast_nodes);
                int anse = size_of_parse_tree_nodes-size_of_ast_nodes;
                anse = anse/(size_of_parse_tree_nodes);
                anse = anse*100;
                printf("Compression percentage = %d",anse);
                printf("\nTask 4 done, again going to Options\n");
                pflag=1;
                break;}
            case 5:{
                if(lflag==0){
                    //l_start_time = clock();
                    FILE* fp;
                    fp = fopen(buffer,"r");
                    if(fp == NULL)
                    {
                        printf("Error opening file:%s \n",buffer);   
                        exit(1);             
                    }

                    printf("Running file %s \n", buffer);
                    populate_hash_table();
                    call_lexer(fp,4096);//todo variable arg
                    int a = postProcessing();
                    if(!a){
                        printf("Found no code\n");
                        exit(0);
                    }
                    printf("TOKENS ARE .... (first to last) \n");
                    printtokens();
                    printf("LEXICAL ERRORS ARE .... (first to last) \n");
                    pop_error_tokens();    
                    lflag=1;
                    // l_end_time = clock();
                    // l_total_CPU_time = (double) (l_end_time - l_start_time);
                    // l_total_CPU_time_in_seconds = l_total_CPU_time / CLOCKS_PER_SEC;
                    fclose(fp);
                }
                else{
                    printf("TOKENS ARE .... (first to last) \n");
                    printtokens();
                    printf("LEXICAL ERRORS ARE .... (first to last) \n");
                    pop_error_tokens();    
                }
                // p_start_time = clock();
                parseCompletely(lflag);
                // p_end_time = clock();
                // p_total_CPU_time = (double) (p_end_time - p_start_time);
                // p_total_CPU_time_in_seconds = p_total_CPU_time / CLOCKS_PER_SEC;
                /*Parsing and Displaying PARSE TREE by PARSER (with Errors)*/
                num_of_parse_tree_nodes = print_parse_tree();
                size_of_parse_tree_nodes = num_of_parse_tree_nodes*sizeof(treeNodes);
                if(LEXICAL_ERRORS || PARSER_ERRORS){
                    return -1;
                }
                num_of_ast_nodes = create_abstract_tree(); //creation of abstract syntax tree 
                size_of_ast_nodes = sizeof(ast_node)*num_of_ast_nodes;
                printf("Parse tree Number of nodes = %d, Allocated Memory = %d Bytes\n",num_of_parse_tree_nodes,size_of_parse_tree_nodes);
                printf("AST Number of nodes = %d, Allocated Memory = %d Bytes",num_of_ast_nodes,size_of_ast_nodes);
                int anse = size_of_parse_tree_nodes-size_of_ast_nodes;
                anse = anse/(size_of_parse_tree_nodes);
                anse = anse*100;
                printf("Compression percentage = %d",anse);
                ast_node* ast_root = get_ast_root();
                semantic();
                get_global_symbol_table(ast_root);
                print_symbol_table(); 
                printf("\nTask 5 done, again going to Options\n");
                pflag=1;
                break;}
            case 6:{
                if(lflag==0){
                    //l_start_time = clock();
                    FILE* fp;
                    fp = fopen(buffer,"r");
                    if(fp == NULL)
                    {
                        printf("Error opening file:%s \n",buffer);   
                        exit(1);             
                    }

                    printf("Running file %s \n", buffer);
                    populate_hash_table();
                    call_lexer(fp,4096);//todo variable arg
                    int a = postProcessing();
                    if(!a){
                        printf("Found no code\n");
                        exit(0);
                    }
                    printf("TOKENS ARE .... (first to last) \n");
                    printtokens();
                    printf("LEXICAL ERRORS ARE .... (first to last) \n");
                    pop_error_tokens();    
                    lflag=1;
                    // l_end_time = clock();
                    // l_total_CPU_time = (double) (l_end_time - l_start_time);
                    // l_total_CPU_time_in_seconds = l_total_CPU_time / CLOCKS_PER_SEC;
                    fclose(fp);
                }
                else{
                    printf("TOKENS ARE .... (first to last) \n");
                    printtokens();
                    printf("LEXICAL ERRORS ARE .... (first to last) \n");
                    pop_error_tokens();    
                }
                // p_start_time = clock();
                parseCompletely(lflag);
                // p_end_time = clock();
                // p_total_CPU_time = (double) (p_end_time - p_start_time);
                // p_total_CPU_time_in_seconds = p_total_CPU_time / CLOCKS_PER_SEC;
                /*Parsing and Displaying PARSE TREE by PARSER (with Errors)*/
                num_of_parse_tree_nodes = print_parse_tree();
                size_of_parse_tree_nodes = num_of_parse_tree_nodes*sizeof(treeNodes);
                if(LEXICAL_ERRORS || PARSER_ERRORS){
                    return -1;
                }
                num_of_ast_nodes = create_abstract_tree(); //creation of abstract syntax tree 
                size_of_ast_nodes = sizeof(ast_node)*num_of_ast_nodes;
                printf("Parse tree Number of nodes = %d, Allocated Memory = %d Bytes\n",num_of_parse_tree_nodes,size_of_parse_tree_nodes);
                printf("AST Number of nodes = %d, Allocated Memory = %d Bytes",num_of_ast_nodes,size_of_ast_nodes);
                int anse = size_of_parse_tree_nodes-size_of_ast_nodes;
                anse = anse/(size_of_parse_tree_nodes);
                anse = anse*100;
                printf("Compression percentage = %d",anse);
                ast_node* ast_root = get_ast_root();
                semantic();
                get_global_symbol_table(ast_root);
                print_activation();
                printf("\nTask 6 done, again going to Options\n");
                pflag=1;
                break;}
            case 7:{
                if(lflag==0){
                    //l_start_time = clock();
                    FILE* fp;
                    fp = fopen(buffer,"r");
                    if(fp == NULL)
                    {
                        printf("Error opening file:%s \n",buffer);   
                        exit(1);             
                    }

                    printf("Running file %s \n", buffer);
                    populate_hash_table();
                    call_lexer(fp,4096);//todo variable arg
                    int a = postProcessing();
                    if(!a){
                        printf("Found no code\n");
                        exit(0);
                    }
                    printf("TOKENS ARE .... (first to last) \n");
                    printtokens();
                    printf("LEXICAL ERRORS ARE .... (first to last) \n");
                    pop_error_tokens();    
                    lflag=1;
                    // l_end_time = clock();
                    // l_total_CPU_time = (double) (l_end_time - l_start_time);
                    // l_total_CPU_time_in_seconds = l_total_CPU_time / CLOCKS_PER_SEC;
                    fclose(fp);
                }
                else{
                    printf("TOKENS ARE .... (first to last) \n");
                    printtokens();
                    printf("LEXICAL ERRORS ARE .... (first to last) \n");
                    pop_error_tokens();    
                }
                // p_start_time = clock();
                parseCompletely(lflag);
                // p_end_time = clock();
                // p_total_CPU_time = (double) (p_end_time - p_start_time);
                // p_total_CPU_time_in_seconds = p_total_CPU_time / CLOCKS_PER_SEC;
                /*Parsing and Displaying PARSE TREE by PARSER (with Errors)*/
                num_of_parse_tree_nodes = print_parse_tree();
                size_of_parse_tree_nodes = num_of_parse_tree_nodes*sizeof(treeNodes);
                if(LEXICAL_ERRORS || PARSER_ERRORS){
                    return -1;
                }
                num_of_ast_nodes = create_abstract_tree(); //creation of abstract syntax tree 
                size_of_ast_nodes = sizeof(ast_node)*num_of_ast_nodes;
                printf("Parse tree Number of nodes = %d, Allocated Memory = %d Bytes\n",num_of_parse_tree_nodes,size_of_parse_tree_nodes);
                printf("AST Number of nodes = %d, Allocated Memory = %d Bytes",num_of_ast_nodes,size_of_ast_nodes);
                int anse = size_of_parse_tree_nodes-size_of_ast_nodes;
                anse = anse/(size_of_parse_tree_nodes);
                anse = anse*100;
                printf("Compression percentage = %d",anse);
                ast_node* ast_root = get_ast_root();
                semantic();
                get_global_symbol_table(ast_root);
                print_static_dynamic_arrays();
                printf("\nTask 7 done, again going to Options\n");
                pflag=1;
                break;}
            case 8:{
                /*For printing total time taken by LEXER+PARSER*/
                if (pflag!=1){
                    if(lflag==0){
                        l_start_time = clock();
                        FILE* fp;
                        fp = fopen(buffer,"r");
                        if(fp == NULL)
                        {
                            printf("Error opening file:%s\n",buffer);   
                            exit(1);             
                        }
                        printf("Running file %s\n", buffer);
                        populate_hash_table();
                        call_lexer(fp,4096);
                        int a = postProcessing();
                        if(!a){
                            printf("Found no code\n");
                            exit(0);
                        }
                        printf("TOKENS ARE .... (first to last) \n");
                        printtokens();
                        printf("LEXICAL ERRORS ARE .... (first to last) \n");
                        pop_error_tokens();    
                        lflag=1;
                        l_end_time = clock();
                        l_total_CPU_time = (double) (l_end_time - l_start_time);
                        l_total_CPU_time_in_seconds = l_total_CPU_time / CLOCKS_PER_SEC;
                        fclose(fp);
                    }
                    else{
                        printf("TOKENS ARE .... (first to last) \n");
                        printtokens();
                        printf("LEXICAL ERRORS ARE .... (first to last) \n");
                        pop_error_tokens();    
                    }
                    p_start_time = clock();
                    parseCompletely(lflag);
                    p_end_time = clock();
                    p_total_CPU_time = (double) (p_end_time - p_start_time);
                    p_total_CPU_time_in_seconds = p_total_CPU_time / CLOCKS_PER_SEC;
                    // num_of_parse_tree_nodes = print_parse_tree(parseBuffer);
                    size_of_ast_nodes = create_abstract_tree();//creation of abstract syntax tree 
                    size_of_ast_nodes = sizeof(ast_node)*size_of_ast_nodes;
                    ast_node* ast_root = get_ast_root();
                    semantic();
                    printf("\n================\n\n\n\n================\n");
                    get_global_symbol_table(ast_root);
                    print_activation();
                    print_static_dynamic_arrays();
                    print_symbol_table(); 
                    if(!SEMANTIC_ERRORS)get_global_symbol_table(ast_root);
                    else return -1;
                    FILE* fptr = fopen("intermediate_code.txt","w+");
                    ir_code* intermediate_code =  getIRList(ast_root,global_TABLE);
                    print_ir_code(fptr,intermediate_code);
                    fclose(fptr);
                    printf("this is the next print .... \n\n\n");
                    print_symbol_table();

                    FILE* assembly = fopen("assembly_try.asm", "w");
                    starter(assembly,intermediate_code);

                    // fprintf(assembly,"%s", ast_root->asm_code);
                    /*Parsing and Displaying PARSE TREE by PARSER (with Errors)*/
                    //TODO PRINT PARSE TREE

                    pflag=1;
                    // fclose(fp);
                }                
                printf("Total CPU Time:%f\n",l_total_CPU_time+p_total_CPU_time);
                printf("Total CPU Time (in seconds):%f\n",l_total_CPU_time_in_seconds+p_total_CPU_time_in_seconds);
                // timed();
                printf("\nTask 4 done, again going to Options\n");
                break;}
            default:
                printf("Wrong input, Enter Again\n");
        }
    }
}

