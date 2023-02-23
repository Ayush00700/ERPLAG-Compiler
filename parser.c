#include "parser.h"

int noOfLines(FILE* fptr){
    char* strtok_result;
    int linecount = 0;
    char buffer[100];
    strtok_result = strtok(buffer, "\n");
    while(strtok_result!=NULL){
        strtok_result = strtok(buffer,"\r\n");
        printf("%s\n",buffer);
        linecount++;
    }
    return linecount;
}

void populateGrammar(FILE* fptr,ruleLL rules[]){

}

void printArray(ruleLL rules[],int noOfRules){
    for(int i=0;i<noOfRules;i++){
        
    }
}


int main(){
    FILE* fptr = fopen("grammar.txt","r+");
    if(fptr==NULL){
        perror("Error opening file");
        exit(1);
    }
    int lines = noOfLines(fptr);
    printf("No of Lines %d",lines);
    ruleLL rules[lines];
    populateGrammar(fptr,rules);
    fclose(fptr);
    printArray(rules,lines);
}



