#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "reverse.h"

void reverse(char* str, int lineMAX)
{
    int len = strlen(str);
    int min;
    if (lineMAX < len)
    {
        min = lineMAX;
    }
    else
    {
        min = len;
    }    
    
    for(int i = 0; i < min/2;++i){
        char tmp     = str[i];
        str[i]       = str[min-i-1];
        str[min-i-1] = tmp;
    }
}

int stringIsdigit(char* str)
{
    int notNums = 0;
    int i = 0;
    while (notNums == 0 && i < strlen(str))
    {
        if (!(str[i] >= '0' && str[i] <= '9'))
        {
            notNums++;
        }
        i++;
    }    
    return notNums;
}

void withoutNewLine(char* line)
{
    for (int i = 0; i < strlen(line); i++)
    {
        if (line[i] == '\n')
        {
            line[i] = '\0';
        }        
    }
    
}

void readUserInput(char*** reversedfiles, int* indPtr, int lineMAX)
{
    int arrayMaxLength = 8;
            char line[1024];
            while(NULL != fgets(line, 1024, stdin))
            {
                if (*indPtr == arrayMaxLength)
                {
                    arrayMaxLength = arrayMaxLength*2;
                    (*reversedfiles) = realloc((*reversedfiles), sizeof(char*)*arrayMaxLength);
                    if ((*reversedfiles) == NULL)
                    {
                        printf("Memory allocation failed!");
                        exit(1);
                    }
                }

                (*reversedfiles)[*indPtr] = malloc(sizeof(char)*lineMAX);
                withoutNewLine(line);
                memcpy((*reversedfiles)[*indPtr], line, lineMAX);
                reverse((*reversedfiles)[*indPtr], lineMAX);
                *indPtr = *indPtr+1;
            }
}
void readFiles(char*** reversedfiles, int* indPtr, int lineMAX, int argc, char* argv[])
{
    int arrayMaxLength = 8;
    for (int i = 3; i < argc; i++)
    {
        FILE *fp = fopen(argv[i], "r");
        if(fp == NULL) {
           printf("File opening unsuccessful: %s\n",argv[i]);
           continue;
        }
        char str[1024] = "";
        while (fgets(str, 1024, fp) != NULL)
        {
            if (*indPtr == arrayMaxLength)
            {
                arrayMaxLength = arrayMaxLength*2;
                (*reversedfiles) = realloc((*reversedfiles), sizeof(char*)*arrayMaxLength);
                if ((*reversedfiles) == NULL)
                {
                    printf("Memory allocation failed!");
                    exit(1);
                }
            }            
            (*reversedfiles)[*indPtr] = malloc(sizeof(char)*(lineMAX+1));
            withoutNewLine(str);
            memcpy((*reversedfiles)[*indPtr], str, (lineMAX+1));
            (*reversedfiles)[*indPtr][lineMAX] = '\0';
            reverse((*reversedfiles)[*indPtr], lineMAX);  
            *indPtr = *indPtr+1; 
        }
        fclose(fp);               
    }
}

void printWithNums(int ind, char** reversedfiles, int lineMAX)
{
    for (int i = ind-1; i >= 0; i--)
    {
        reversedfiles[i][lineMAX] = '\0';
        if (i != 0)
        {
            printf("%d %s\n", i+1, reversedfiles[i]);
        }
        else
        {
            printf("%d %s", i+1, reversedfiles[i]);
        }                
    }
    //printeles utan fel kell szabaditani
    for(int i = 0; i < ind;++i)
    {
        free(reversedfiles[i]);
    }
    free(reversedfiles);
}
void printWithoutNums(int ind, char** reversedfiles, int lineMAX)
{
    for (int i = ind-1; i >= 0; i--)
    {
        reversedfiles[i][lineMAX] = '\0';
        if (i != 0)
        {
        printf("%s\n",reversedfiles[i]);                    
        }
        else
        {
            printf("%s",reversedfiles[i]);
        }                
    }
    //printeles utan fel kell szabaditani
    for(int i = 0; i < ind;++i)
    {
        free(reversedfiles[i]);
    }
    free(reversedfiles);
}