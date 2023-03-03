#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "parser.h"
#include <string.h>

int main(int argc, char* args[]){ //DRIVER
    clock_t l_start_time, l_end_time;
    double l_total_CPU_time, l_total_CPU_time_in_seconds;
    clock_t p_start_time, p_end_time;
    double p_total_CPU_time, p_total_CPU_time_in_seconds;
    int lflag=0;
    int pflag=0;
    char* testcaseFile = args[1];
    // strcpy(testcaseFile,args[1]);
    char* fileForParseTree = args[2];
    int bufferSize = atoi(args[3]);
    // printf("Test Case File:%s\n",testcaseFile);
    // printf("Parse Tree Printing File:%s\n",fileForParseTree);
    // printf("Buffer Size:%d\n",bufferSize);
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
    printf("################WELCOME TO ERPLAG COMPILER MENU######################\n\n");
    printf("OPTIONS\n");
        printf("\n1. Printing code without comments\n");
        printf("2. Token List by LEXER (with Errors)\n");
        printf("3. Parsing and Displaying PARSE TREE by PARSER (with Errors)\n");
        printf("4. For printing total time taken by LEXER+PARSER\n");
        printf("0. EXIT\n");
        int choice;

        //For testing purpose -->
        // int choice = 4;
        // char buffer[50] = "code_error_case1.txt";

      
        printf("Enter your choice: ");
        scanf("%d",&choice);
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
                removeComments(testcaseFile,cleanFile);
                printf("\nTask 1 done, again going to Options\n");
                break;

            /*Token List by LEXER with Errors*/
            case 2:
                l_start_time = clock();
                FILE* fp;
                fp = fopen(testcaseFile,"r");
                if(fp == NULL)
                {
                    printf("Error opening file:%s\n!",testcaseFile);   
                    exit(1);             
                }
                printf("Running file %s\n", testcaseFile);
                populate_hash_table();
                call_lexer(fp,4096);
                postProcessing();
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
                    fp = fopen(testcaseFile,"r");
                    if(fp == NULL)
                    {
                        printf("Error opening file:%s\n!",testcaseFile);   
                        exit(1);             
                    }

                    printf("Running file %s\n", testcaseFile);
                    populate_hash_table();
                    call_lexer(fp,4096);
                    postProcessing();
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
                p_start_time = clock();
                parseCompletely(lflag);
                p_end_time = clock();
                p_total_CPU_time = (double) (p_end_time - p_start_time);
                p_total_CPU_time_in_seconds = p_total_CPU_time / CLOCKS_PER_SEC;
                /*Parsing and Displaying PARSE TREE by PARSER (with Errors)*/
                print_parse_tree(fileForParseTree);
                printf("\nTask 3 done, again going to Options\n");
                pflag=1;
                break;
            case 4:
                /*For printing total time taken by LEXER+PARSER*/
                if (pflag!=1){
                    if(lflag==0){
                        l_start_time = clock();
                        FILE* fp;
                        fp = fopen(testcaseFile,"r");
                        if(fp == NULL)
                        {
                            printf("Error opening file:%s\n!",testcaseFile);   
                            exit(1);             
                        }
                        printf("Running file %s\n", testcaseFile);
                        populate_hash_table();
                        call_lexer(fp,4096);
                        postProcessing();
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
                    p_start_time = clock();
                    parseCompletely(lflag);
                    p_end_time = clock();
                    p_total_CPU_time = (double) (p_end_time - p_start_time);
                    p_total_CPU_time_in_seconds = p_total_CPU_time / CLOCKS_PER_SEC;
                    print_parse_tree(fileForParseTree);
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

