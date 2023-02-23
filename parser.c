#include "parser.h"

int noOfLines(FILE* fptr){
    char* strtok_result;
    int linecount = 0;
    fseek (fptr , 0 , SEEK_END);
    long fptr_size = ftell (fptr);
    rewind (fptr);
    char buffer[fptr_size];
    size_t result;
    result = fread (buffer,1,fptr_size,fptr);
    strtok_result = strtok(buffer, "\n");
    while(strtok_result!=NULL){
        linecount++;
        strtok_result = strtok(NULL,"\n");
    }
    return linecount;
}

void populateGrammar(FILE* fptr,ruleLL rules[]){
    char* strtok_result;
    int linecount = 0;
    fseek (fptr , 0 , SEEK_END);
    long fptr_size = ftell (fptr);
    rewind (fptr);
    char buffer[fptr_size];
    size_t result;
    result = fread (buffer,1,fptr_size,fptr);
    strtok_result = strtok(buffer, "\n");
    while(strtok_result!=NULL){
        linecount++;
        strtok_result = strtok(NULL,"\n");
    }
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



