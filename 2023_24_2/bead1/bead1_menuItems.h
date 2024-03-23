#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

void addNewPoem()
{
	char c;
	char poemName[11];
	char poem[101];

	printf("Write the name of the poem (the max len is 10 char): ");
	scanf("%10[^\n]",poemName);
	do //clear puffer
	{
		scanf("%c",&c);
	}
	while(c != '\n');

	char path[17] = "poems/";
	strcat(path,poemName);
	int f=open(path, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	if (f < 0) 
	{
		perror("Error at opening the file\n");
		exit(1);
	}
		
	printf("Write the poem itself (the max len is 100 char): ");
	scanf("%100[^\n]",poem);
	do //clear puffer
	{
		scanf("%c",&c);
	}
	while(c != '\n');

	write(f, &poem, strlen(poem));

	printf("New poem added successfully!\n");
	close(f);
}

void showListOfPoems()
{
	printf("Showing list of poems\n");
}

void deletePoem()
{
	printf("Deleting poem\n");
}

void editPoem()
{
	printf("Editing poem\n");
}
