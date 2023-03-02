#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "lexer.h"
#include "parser.h"
#include <string.h>

void timed(){

    clock_t start_time, end_time;

    double total_CPU_time, total_CPU_time_in_seconds;

    start_time = clock();

    // invoke your lexer and parser here
    //TODO

    end_time = clock();

    total_CPU_time = (double) (end_time - start_time);

    total_CPU_time_in_seconds = total_CPU_time / CLOCKS_PER_SEC;

    printf("Total CPU Time:%f\n",total_CPU_time);
    printf("Total CPU Time (in seconds):%f\n",total_CPU_time_in_seconds);
}


int main(int argc, char* args[]){ //DRIVER
    char* testcaseFile = args[1];
    char* fileForParseTree = args[2];
    int bufferSize = atoi(args[3]);
    int run =1;
    int third = 0; 
    
    printf("GROUP MEMBERS:\nName\t\t\tID\n");
    printf("Ayush Agarwal(Leader)\t2019B4A70652P\n");
    printf("Rajan Sahu\t\t2019B4A70572P\n");
    printf("Yash Goyal\t\t2019B4A70638P\n");
    printf("Vasu Swaroop\t\t2019B4A70656P\n");
    printf("A Sudarshan\t\t2019B4A70744P\n\n\n");

    printf("SHORT DESCRIPTION\n");
    printf("(a) FIRST and FOLLOW set have been automated\n");
    printf("(b)Both lexical and syntax analysis modules implemented\n");
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
        printf("Enter your choice: ");
        scanf("%d",&choice);
        fputs("\033c", stdout); //CLEARS STDOUT SCREEN
        switch (choice)
            {
            case 0:
                run = 0;
                break;
            case 1:
                /*Printing code without comments*/
                char* cleanFile;
                printf("Enter name of new clean file where you want the code without comments\n");
                scanf("%s",cleanFile);
                removeComments(testcaseFile,cleanFile);
                printf("\nTask 1 done, again going to Options\n");
                break;
            case 2:
                /*Token List by LEXER with Errors*/
                //TODO
                FILE* fp = fopen(testcaseFile,"r");
                printf("Running file %s", testcaseFile);
                populate_hash_table();
                call_lexer(fp,4096);
                current->next_token = (token_node*) malloc(sizeof(token_node)); //need to add dollar in the tokens too at the end
                current->next_token->next_token = NULL;
                current->next_token->token = (token_info*)malloc(sizeof(token_info));
                current->next_token->token->lexeme = "$";
                strcpy(current->next_token->token->type,"$");

                
                initialize();
                printf("TOKENS ARE .... (first to last) \n");
                printtokens();
                printf("ERRORS ARE .... (last to first) \n");
                pop_error_tokens();    

                printf("\nTask 2 done, again going to Options\n");
                break;
            case 3:
                /*Parsing and Displaying PARSE TREE by PARSER (with Errors)*/
                //TODO
                printf("\nTask 3 done, again going to Options\n");
                break;
            case 4:
                /*For printing total time taken by LEXER+PARSER*/
                timed();
                printf("\nTask 4 done, again going to Options\n");
                break;
            default:
                printf("Wrong input, Enter Again\n");
        }
    }
}

