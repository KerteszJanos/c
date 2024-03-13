#ifndef Reverse
#define Reverse

void reverse(char* str, int lineMAX);
int stringIsdigit(char* str);
void withoutNewLine(char* line);

void readUserInput(char*** reversedfiles, int* indPtr, int lineMAX);
void readFiles(char*** reversedfiles, int* indPtr, int lineMAX, int argc, char* argv[]);

void printWithNums(int ind, char** reversedfiles, int lineMAX);
void printWithoutNums(int ind, char** reversedfiles, int lineMAX);

#endif