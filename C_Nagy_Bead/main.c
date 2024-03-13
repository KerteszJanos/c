#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "reverse.h"

struct args
{
    char* linenums;
    int lineMAX;    
};
typedef struct args args;

int main(int argc, char* argv[])
{
    args a = {argv[1], atoi(argv[2])};
    if (argc-1 > 1 && ((strcmp(a.linenums, "linenums") == 0) || (strcmp(a.linenums, "nolinenums") == 0)) && stringIsdigit(argv[2]) == 0)
    {
        char** reversedfiles = malloc(sizeof(char*)*8);
        if (reversedfiles == NULL)
        {
            printf("Memory allocation failed!");
            exit(1);
        }        

        int ind = 0;
        int* indPtr = &ind;

        if (argc == 3)
        {
            readUserInput(&reversedfiles, indPtr, a.lineMAX);            
        }
        else
        {
            readFiles(&reversedfiles, indPtr, a.lineMAX, argc, argv);            
        }

        if (strcmp(a.linenums, "linenums") == 0)
        {
            printWithNums(ind, reversedfiles, a.lineMAX);
        }
        else
        {
            printWithoutNums(ind, reversedfiles, a.lineMAX);
        }
    }
    else
    {
        printf("Usage:\n\trev [SHOW LINE NUMBERS] [MAX LINE LENGTH] files...");
    }
    
    
    return 0;
}