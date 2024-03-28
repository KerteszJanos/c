#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>

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
	char c;
	char path[17] = "poems/";
	int f;
	struct dirent *pDirent;
	DIR *pDir;
	pDir = opendir("./poems");
	if(pDir == NULL)
	{
		perror("Error at opening the directory\n");
		exit(1);
	}
	while((pDirent = readdir(pDir)) != NULL)
	{
		if(pDirent->d_name[0] != '.')
		{
			printf("Poem name: %s\n", pDirent->d_name);
			printf("Poem content: ");
			strcpy(path,"poems/");
			strcat(path,pDirent->d_name);
			f = open(path, O_RDONLY);
			if(f<0)
			{
				perror("Error at opening the file\n");
				exit(1);
			}
			while(read(f,&c,sizeof(c)))
			{
				printf("%c",c);
			}
			printf("\n\n");
		}
	}
	closedir(pDir);
}

void deletePoem()
{
	printf("Write the name of the poem you want to delete: ");
	char delPoemName[11];
	char c;

	scanf("%10[^\n]", delPoemName);
	do //clear puffer
	{
		scanf("%c",&c);
	}
	while(c != '\n');

	char path[17] = "poems/";
	strcat(path,delPoemName);
	if(remove(path))
	{
		perror("Unable to delete the file!\n");
		//exit(1);
	}
	else
	{
		printf("%s poem deleted successfully!\n", delPoemName);
	}
}

void editPoem()
{
	printf("Write the name of the poem you want to edit: ");
	char editPoemName[11];
	char c;

	scanf("%10[^\n]", editPoemName);
	do //clear puffer
	{
		scanf("%c",&c);
	}
	while(c != '\n');
	
	char path[17] = "poems/";
	strcat(path, editPoemName);

	int f = open(path, O_RDONLY);
	if(f<0)
	{
		perror("Error at opening the file\n");
		//exit(1);
	}
	printf("You can type the edited version under the original one!\n\n");
	while(read(f,&c,sizeof(c)))
	{
		printf("%c",c);
	}
	printf("\n");
	close(f);

	f = open(path, O_WRONLY | O_TRUNC);
	if(f<0)
	{
		perror("Error at opening the file\n");
		//exit(1);
	}
	char editedPoem[101];
	scanf("%100[^\n]",editedPoem);
	do //clear puffer
	{
		scanf("%c",&c);
	}
	while(c != '\n');

	write(f, &editedPoem, strlen(editedPoem));
	printf("Poem edited successfully!\n");
	close(f);
}
