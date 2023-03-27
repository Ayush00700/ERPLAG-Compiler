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
#include "ast.h"
#include <string.h>

int main(int argc, char* args[]){ //DRIVER
    clock_t l_start_time, l_end_time;
    double l_total_CPU_time, l_total_CPU_time_in_seconds;
    clock_t p_start_time, p_end_time;
    double p_total_CPU_time, p_total_CPU_time_in_seconds;
    int lflag=0;
    int pflag=0;
    char* buffer = args[1];
    // strcpy(buffer,args[1]);
    char* parseBuffer = args[2];
    int bufferSize = atoi(args[3]);
    // printf("arg[1]:%s\n",buffer);
    // printf("arg[2]:%s\n",parseBuffer);
    // printf("arg[3]:%d\n",bufferSize);
    int run = 1;
    int third = 0; 
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
        run=0;//COMMENT OUT LATER TODO
    printf("################WELCOME TO ERPLAG COMPILER MENU######################\n\n");
    printf("OPTIONS\n");
        printf("\n1. Printing code without comments\n");
        printf("2. Token List by LEXER (with Errors)\n");
        printf("3. Parsing and Displaying PARSE TREE by PARSER (with Errors)\n");
        printf("4. For printing total time taken by LEXER+PARSER\n");
        printf("0. EXIT\n");
        int choice = 4;

        // For testing purpose -->
        // int choice = 4;
        // char buffer[50] = "t1.txt";
        // char parseBuffer[50] = "o1.txt";

      
        // printf("Enter your choice: ");
        // scanf("%d",&choice);
        fputs("\033c", stdout); //CLEARS STDOUT SCREEN
        switch (choice)
            {
            case 0:
                run = 0;
                break;

            /*Printing code without comments*/
            case 1:
                printf(" ");
                char cleanFile[100];
                printf("Enter name of new clean file where you want the code without comments\n");
                scanf("%s",cleanFile);
                removeComments(buffer,cleanFile);
                printf("\nTask 1 done, again going to Options\n");
                break;

            /*Token List by LEXER with Errors*/
            case 2:
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

                printf("\nTask 2 done, again going to Options\n");
                lflag=1;
                l_end_time = clock();
                l_total_CPU_time = (double) (l_end_time - l_start_time);
                l_total_CPU_time_in_seconds = l_total_CPU_time / CLOCKS_PER_SEC;
                fclose(fp);
                break;
            case 3:
                if(lflag==0){
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
                /*Parsing and Displaying PARSE TREE by PARSER (with Errors)*/
                print_parse_tree(parseBuffer);
                printf("\nTask 3 done, again going to Options\n");
                pflag=1;
                break;
            case 4:
                /*For printing total time taken by LEXER+PARSER*/
                if (pflag!=1){
                    if(lflag==0){
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
                    print_parse_tree(parseBuffer);
                    create_abstract_tree();//creation of abstract syntax tree 

                    /*Parsing and Displaying PARSE TREE by PARSER (with Errors)*/
                    //TODO PRINT PARSE TREE

                    pflag=1;
                    // fclose(fp);
                }                
                printf("Total CPU Time:%f\n",l_total_CPU_time+p_total_CPU_time);
                printf("Total CPU Time (in seconds):%f\n",l_total_CPU_time_in_seconds+p_total_CPU_time_in_seconds);
                // timed();
                printf("\nTask 4 done, again going to Options\n");
                break;
            default:
                printf("Wrong input, Enter Again\n");
        }
    }
}

