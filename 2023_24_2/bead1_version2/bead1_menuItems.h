#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>

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

	int f=open("poems.txt", O_WRONLY | O_APPEND);
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
	
	char content[115];
	strcpy(content,poemName);
	strcat(content,"\n");
	strcat(content,poem);
	strcat(content,"\n\n");
	write(f, &content, strlen(content));

	printf("New poem added successfully!\n");
	close(f);
}

void showListOfPoems()
{
	int f = open("poems.txt", O_RDONLY);
	if (f < 0) 
	{
		perror("Error at opening the file\n");
		exit(1);
	}

	char c;
	while(read(f,&c,sizeof(c)))
	{
		printf("%c",c);
	}
	close(f);
}

void deletePoem()
{
	printf("Write the name of the poem you want to delete: ");
	char delPoemName[11] = "testPoem1";
	char c;

	scanf("%10[^\n]", delPoemName);
	do //clear puffer
	{
		scanf("%c",&c);
	}
	while(c != '\n');

	int f = open("poems.txt", O_RDONLY);
	if (f < 0) 
	{
		perror("Error at opening the file\n");
		exit(1);
	}

	char poemNames[64][11];
	char poems[64][101];

	int poemCounter = 0;
	int i = 0;
	int prevItem = 2; //0 if prev item was a poemName, 1 if poem, 2 if empty line

	while(read(f,&c,sizeof(c)))
	{
		if(c == '\n')
		{
			if(prevItem == 0)
			{
				poems[poemCounter][i] = '\0';
				prevItem = 1;
				poemCounter++;
			}
			else if(prevItem == 1)
			{
				prevItem = 2;
			}
			else if (prevItem == 2)
			{
				poemNames[poemCounter][i] = '\0';
				prevItem = 0;
			}			
			i = 0;
		}
		else
		{
			if(prevItem == 2)
			{
				poemNames[poemCounter][i] = c;
			}
			else if(prevItem == 0)
			{
				poems[poemCounter][i] = c;
			}	
			i++;
		}
	}
	close(f);

	//write without the specific poem
	f = open("poems.txt", O_WRONLY | O_TRUNC);
	if (f < 0) 
	{
		perror("Error at opening the file\n");
		exit(1);
	}

	bool success = false;
	char endChar = '\n';
	for(int i = 0; i < poemCounter; ++i)
	{
		if(strcmp(poemNames[i], delPoemName) != 0 )
		{
		 	write(f, &poemNames[i], strlen(poemNames[i]));
			write(f, &endChar, sizeof(endChar));
			write(f, &poems[i], strlen(poems[i]));
			write(f, &endChar, sizeof(endChar));
			write(f, &endChar, sizeof(endChar));
		}
		else
		{
			success = true;
		}
	}
	if(success)
	{
		printf("Poem named %s successfully deleted!\n", delPoemName);
	}
	else
	{
		printf("There is no poem named %s\n", delPoemName);
	}
	close(f);
}

void editPoem()
{
	printf("Write the name of the poem you want to delete: ");
	char editPoemName[11] = "testPoem1";
	char c;

	scanf("%10[^\n]", editPoemName);
	do //clear puffer
	{
		scanf("%c",&c);
	}
	while(c != '\n');

	int f = open("poems.txt", O_RDONLY);
	if (f < 0) 
	{
		perror("Error at opening the file\n");
		exit(1);
	}

	char poemNames[64][11];
	char poems[64][101];

	int poemCounter = 0;
	int i = 0;
	int prevItem = 2; //0 if prev item was a poemName, 1 if poem, 2 if empty line

	while(read(f,&c,sizeof(c)))
	{
		if(c == '\n')
		{
			if(prevItem == 0)
			{
				poems[poemCounter][i] = '\0';
				prevItem = 1;
				poemCounter++;
			}
			else if(prevItem == 1)
			{
				prevItem = 2;
			}
			else if (prevItem == 2)
			{
				poemNames[poemCounter][i] = '\0';
				prevItem = 0;
			}			
			i = 0;
		}
		else
		{
			if(prevItem == 2)
			{
				poemNames[poemCounter][i] = c;
			}
			else if(prevItem == 0)
			{
				poems[poemCounter][i] = c;
			}	
			i++;
		}
	}
	close(f);

	printf("You can type the edited version under the original one!\n\n");
	
	for(int i = 0; i < poemCounter; ++i)
	{
		if(strcmp(poemNames[i], editPoemName) == 0 )
		{
			printf("%s\n",poems[i]);
		}
	}
	char editedPoem[101];
	scanf("%100[^\n]",editedPoem);
	do //clear puffer
	{
		scanf("%c",&c);
	}
	while(c != '\n');
	
	f = open("poems.txt", O_WRONLY | O_TRUNC);
	if (f < 0) 
	{
		perror("Error at opening the file\n");
		exit(1);
	}

	bool success = false;
	char endChar = '\n';
	for(int i = 0; i < poemCounter; ++i)
	{
		if(strcmp(poemNames[i], editPoemName) != 0 )
		{
		 	write(f, &poemNames[i], strlen(poemNames[i]));
			write(f, &endChar, sizeof(endChar));
			write(f, &poems[i], strlen(poems[i]));
			write(f, &endChar, sizeof(endChar));
			write(f, &endChar, sizeof(endChar));
		}
		else
		{
		 	write(f, &poemNames[i], strlen(poemNames[i]));
			write(f, &endChar, sizeof(endChar));
			write(f, &editedPoem, strlen(editedPoem));
			write(f, &endChar, sizeof(endChar));
			write(f, &endChar, sizeof(endChar));
			success = true;
		}
	}
	if(success)
	{
		printf("Poem named %s successfully edited!\n", editPoemName);
	}
	else
	{
		printf("There is no poem named %s\n", editPoemName);
	}
	close(f);
}
