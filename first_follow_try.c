#include <stdio.h>
#include <string.h>

struct rule
{
    char label;
    char tornt;
    struct rule* next;
};

int belong(char a, char* arr)
/*Returns 1 if the element a belongs to arr else 0*/
{
    int flag=-1;
    for(int i=0; i<sizeof(arr); i++)
    {
        if(arr[i] == a)
        {
            flag=1;
            return flag;
        }
    }

    return flag;
}

char* first(FILE *fp)
{

}

char* follow(FILE *fp)
{

}

void test_belong()
{
    char terminal[] = {'a','b','$'};
    char nonterminal[] = {'S','A','B'};

    int x = belong('a',terminal);
    (x>0)? printf("Yes it belongs!\n"):printf("No it isn't there!\n");

    int y = belong('a',nonterminal);
    (y>0)? printf("Yes it belongs!\n"):printf("No it isn't there!\n");

    int x1 = belong('A',terminal);
    (x1>0)? printf("Yes it belongs!\n"):printf("No it isn't there!\n");

    int y1 = belong('A',nonterminal);
    (y1>0)? printf("Yes it belongs!\n"):printf("No it isn't there!\n");

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
