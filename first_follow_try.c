#include <stdio.h>
#include <string.h>
#include "parser.h"
#include "tokeninfo.h"

NonT N[sizeof(arr_NonT)];

int belong(char a, char* arr)
/*Returns 1 if the element a belongs to arr else 0*/
{
    int flag=-1;
    for(int i=0; i<sizeof(arr); i++)
    {
        if(arr[i] == a)
        {
            flag=1;
            return i;
        }
    }

    return -1;
}

// FirstSet first(ruleNode node)
// {
//     // If it is a terminal then return the node
//     if(node.isTerminal)
//     {
//         FirstSet f;
//         f.type = node.nodeInfo->type;
//         f.next = NULL;

//         return f;
//     }

//     // // If it is a nonterminal
//     // else
//     // {
//     //     if
//     // }
// }

char* follow(FILE *fp)
{

}

void test_belong()
{
    char terminal[] = {'a','b','$'};
    char nonterminal[] = {'S','A','B'};

    int x = belong('a',terminal);
    (x != -1)? printf("Yes it belongs!\n"):printf("No it isn't there!\n");

    int y = belong('x',nonterminal);
    (y != -1)? printf("Yes it belongs!\n"):printf("No it isn't there!\n");

    int x1 = belong('A',terminal);
    (x1 != -1)? printf("Yes it belongs!\n"):printf("No it isn't there!\n");

    int y1 = belong('A',nonterminal);
    (y1 != -1)? printf("Yes it belongs!\n"):printf("No it isn't there!\n");

}

int main()
{
    FILE *f;
    struct rule* A[5];
    char terminal[] = {'a','b','$'};
    char nonterminal[] = {'S','A','B'};

    // To test belong
    test_belong();

    f = fopen("sample_grammar.txt","r");

    for(int i=0; i<sizeof(arr_NonT); i++)
    {
        N[i].label = arr_NonT[i];
        // N[i].Fi = 
    }

    // if(f == NULL)
    //     printf("Couldn't open file!");
    
    // else
    // {
    //     printf("File successfully opened");
    //     char c;
    //     int i=0;
    //     while((c = fgetc(f)) != EOF)
    //     {
    //         A[i]->label = c;
    //         A[i]->tornt = 'NT';
    //     }
    // }

    return 0;

}
