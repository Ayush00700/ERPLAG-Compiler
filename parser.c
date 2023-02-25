#include <stdio.h>
#include "lexer.h"

typedef enum Boolean {False,True}Boolean;
typedef struct ruleNode{
    char* nodeInfo; 
    Boolean isTerminal;
    struct ruleNode* nextNode;
}ruleNode;

typedef struct rule{
    ruleNode *head;
    ruleNode *tail;
    int lineNo;
}rule;


typedef struct entry{
    char* key;
    unsigned int entry_number;
    unsigned int order;
    struct entry* next;
}entry;

int non_terminals = 0;
int terminals = 0;

entry* non_Terminals_table[TABLE_SIZE];
entry* Terminals_table[TABLE_SIZE];

unsigned int hash(char* key){
    unsigned int hashval = 0;
    for(;*key !='\0';key++){
        hashval = *key + 11*hashval;
    }
    return hashval%TABLE_SIZE;
}

void print_tables(entry* table[]){
 for (int i=0;i<TABLE_SIZE;i++){
        if(table[i]==NULL){
            printf("\t%d\t---\n",i);
        }else {
           entry* current;
           current = table[i];
           printf("\t%d\t",i);
           while(current!=NULL){
           printf("%s:<%d>order<%d> \t",current->key,current->entry_number,current->order);
           current = current->next;
           }
           printf("\n");
        }
    }

}
int set_add(char* key,entry* table[],int entry_number){
    unsigned int index = hash(key);
    entry* current = table[index];

    while(current!=NULL){
        if(strcmp(current->key,key)==0){
            return 0;
        }
        current = current->next;
    }

    entry* new_node = (entry*)malloc(sizeof(entry));
    new_node -> key = key;
    new_node -> next = table[index];
    new_node->entry_number = index;
    new_node->order = entry_number+1;
    table[index] = new_node;
    return 1;

}

int set_contains(char* key,entry* table[]){
    unsigned int index = hash(key);
    entry* current = table[index];

    while(current!=NULL){
        if(strcmp(current->key,key)==0){
            // printf(" %d %d",current->entry_number,current->order);
            return current->order;
        }
        current = current->next;
    }

    return 0;

}

void firstnfollow(){
    //first of rules
    //first of nt
    //see the first where eps comes and find follow
    //first(bottom up)
    //follow(top down)

    entry* first_set;

}



int lines = 0;

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



void insert_in_rule(rule* rules, int i, Boolean isTerminal, char* buff){
        ruleNode* r2 = (ruleNode*)malloc(sizeof(ruleNode));
        r2->nodeInfo = buff;
        r2->isTerminal = isTerminal;
        r2->nextNode = NULL;
        rules[i-1].tail->nextNode = r2;
        rules[i-1].tail = r2;
}


void linepop(FILE* fp,int i,rule* rules){
    char* buff = (char*) malloc(sizeof(char)*50);
    memset(buff,'\0',50);
    fscanf(fp,"<%s> ",buff);
    buff[strlen(buff) - 1] = '\0';
    ruleNode* r1 = (ruleNode*)malloc(sizeof(ruleNode));

    r1->nodeInfo = buff;
    r1->isTerminal = False;
    r1->nextNode = NULL;
    (rules+i-1)->head = r1;
    rules[i-1].tail = r1;
    rules[i-1].lineNo = i;
    int r = set_add(buff,non_Terminals_table,non_terminals);
    if(r==1)non_terminals++;
    char c;
    c = fgetc(fp);// moving to RHS of production
    c = fgetc(fp);
    c = fgetc(fp);
    int break_point = 0;
    c = fgetc(fp);
    
    while(c!='\n'){
    if(c == '<'){
        char* buff = (char*) malloc(sizeof(char)*50);
        ungetc(c,fp);
        memset(buff,'\0',50);
        fscanf(fp,"<%s>",buff); //read a non terminal 
        buff[strlen(buff) - 1] = '\0';
        insert_in_rule(rules,i,False,buff);
        r = set_add(buff,non_Terminals_table,non_terminals);
        if(r==1) non_terminals++;
    }
    else{
        char* buff = (char*) malloc(sizeof(char)*20);
        ungetc(c,fp);
        memset(buff,'\0',20);
        fscanf(fp,"%s",buff); //read a terminal 
        insert_in_rule(rules,i,True,buff);
        r = set_add(buff,Terminals_table,terminals);
        if(r==1) terminals++;

    }

    if(break_point)break;
    c = fgetc(fp);
    c = fgetc(fp);

    if(c==EOF) break;
    }
}

void print_grammar(rule* rules){
      for(int i=0;i<lines;i++){
        printf("<%s> ==> ",rules[i].head->nodeInfo);
        ruleNode* currNode = rules[i].head->nextNode;
        while(currNode!=NULL){
            if(currNode->isTerminal==True){
            printf("%s ",currNode->nodeInfo);
            }
            else{ printf("<%s> ",currNode->nodeInfo);
            }
            currNode = currNode->nextNode;
        
        }
        printf("\n");
    }
}

rule* populate_grammar(){
    FILE* fp = fopen("grammar.txt","r");
    lines = noOfLines(fp);
    rule* rules = (rule*)malloc(sizeof(rule)*(lines+1));
    fclose(fp);
    fp = fopen("grammar.txt","r");
    for(int i=1;i<=lines;i++){
        linepop(fp,i,rules);
    }

    return rules;
}

int main(){
    rule* rules = populate_grammar();
    // print_grammar(rules);
    print_tables(non_Terminals_table);
    printf("\n\n");
    print_tables(Terminals_table);
    printf("%d %d\n",non_terminals,terminals);
 

}