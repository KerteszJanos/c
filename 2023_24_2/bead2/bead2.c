#include <stdlib.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <dirent.h>
#include <stdbool.h>
#include <time.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/msg.h>

//helper menu methods
void readPoemsFromFile(int f, char poemNames[][11], char poems[][101], int *poemCounter)
{
    char c;
    int i = 0;
    int prevItem = 2; //0 if prev item was a poemName, 1 if poem, 2 if empty line

    while (read(f, &c, sizeof(c)))
    {
        if (c == '\n')
        {
            if (prevItem == 0)
            {
                poems[*poemCounter][i] = '\0';
                prevItem = 1;
                (*poemCounter)++;
            }
            else if (prevItem == 1)
            {
                prevItem = 2;
            }
            else if (prevItem == 2)
            {
                poemNames[*poemCounter][i] = '\0';
                prevItem = 0;
            }
            i = 0;
        }
        else
        {
            if (prevItem == 2)
            {
                poemNames[*poemCounter][i] = c;
            }
            else if (prevItem == 0)
            {
                poems[*poemCounter][i] = c;
            }
            i++;
        }
    }
}


//menuItemMethods
void addNewPoem()
{
	char c;
	char poemName[11];
	char poem[101];
	
	do
	{
		printf("Write the name of the poem (the max len is 10 char): ");
		scanf("%10[^\n]",poemName);
		do //clear puffer
		{
			scanf("%c",&c);
		}
		while(c != '\n');
	
		if(!strlen(poemName))
		{
			printf("You need to give a name!\n");
		}
	}
	while(!strlen(poemName));

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
void deletePoem(char poemName[11])
{
	char delPoemName[11];
	if(strlen(poemName) == 0)
	{
		char c;
        	printf("Write the name of the poem you want to delete: ");
        	scanf("%10[^\n]", delPoemName);
		do //clear puffer
		{
			scanf("%c",&c);
		}
		while(c != '\n');
	}
	else
	{
		strcpy(delPoemName, poemName);
	}

	int f = open("poems.txt", O_RDONLY);
	if (f < 0) 
	{
		perror("Error at opening the file\n");
		exit(1);
	}

	char poemNames[64][11];
	char poems[64][101];

	int poemCounter = 0;
	readPoemsFromFile(f, poemNames, poems, &poemCounter);

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
	char editPoemName[11] = "";
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
	readPoemsFromFile(f, poemNames, poems, &poemCounter);
	
	int poemInd = -1;
	for(int i = 0; i < poemCounter; ++i)
	{
		if(strcmp(poemNames[i], editPoemName) == 0 )
		{
			poemInd = i;
		}
	}
	
	if(poemInd == -1)
	{
		printf("There is no poem named %s\n", editPoemName);
	}
	else
	{
		printf("You can type the edited version under the original one!\n\n%s\n", poems[poemInd]);
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
			}
		}
		printf("Poem named %s successfully edited!\n", editPoemName);
		close(f);
	}
}
void SendChildSprinkle(pid_t childId, int* childsThoseAlreadySprinkledCtr); //place this here bc Sprinkle method needs it
void Sprinkle(pid_t child1, pid_t child2, pid_t child3, pid_t child4, int rndNum, int* childsThoseAlreadySprinkledCtr)
{
	switch(rndNum)
	{
		case 0: //child1
			SendChildSprinkle(child1, childsThoseAlreadySprinkledCtr);
			break;
		case 1: //child2
			SendChildSprinkle(child2, childsThoseAlreadySprinkledCtr);
			break;
		case 2: //child3
			SendChildSprinkle(child3, childsThoseAlreadySprinkledCtr);
			break;
		case 3: //child4
			SendChildSprinkle(child4, childsThoseAlreadySprinkledCtr);
			break;
		default:
			break;
	}
}



//sprinkling helper methods
void handler(int signumber) { }
struct msg_t
{ 
	long mtype;
        char poemName[16]; 
}; 
void SendChildSprinkle(pid_t childId, int* childsThoseAlreadySprinkledCtr)
{
	int f = open("poems.txt", O_RDONLY);
	char poemNames[64][11];
	char poems[64][101];
	int poemCounter = 0;
	readPoemsFromFile(f, poemNames, poems, &poemCounter);
	close(f);
	if(poemCounter < 2)
	{
		printf("At least 2 poems needed to sprinkle.\n");
	}
	else //starts the sprinklering
	{
		signal(SIGUSR1, handler);
		int success=mkfifo("/tmp/am2vz8", 0666); 
//   		if (success==-1)
//    	 	{
//			printf("Error number: %i\n",errno); 
//			exit(EXIT_FAILURE);
//		}
		key_t key = ftok("/tmp/am2vz8",childId);
		int ml = msgget(key, 0666 | IPC_CREAT );
		*childsThoseAlreadySprinkledCtr += 1;

		//send a sigusr1 for the child with a kill
		kill(childId, SIGUSR1);

		char twoPoem[228];
		strcpy(twoPoem, poemNames[0]);
    		strcat(twoPoem, "\n");
		strcat(twoPoem, poems[0]);
		strcat(twoPoem, "\n");
		strcat(twoPoem, poemNames[1]);
		strcat(twoPoem, "\n");
		strcat(twoPoem, poems[1]);
		strcat(twoPoem, "\n");

		int fd = open("/tmp/am2vz8", O_WRONLY);		
		write(fd,twoPoem,sizeof(twoPoem));
    		close(fd);	
		pause();
		
		printf("-Parent:\nChoose one!\n\n");
		kill(childId, SIGUSR1);
		pause();

		struct msg_t m;
   		int status = msgrcv(ml, &m, 16, childId, 0);
   		if (status < 0) 
			perror("msgrcv"); 
		printf("-Parent:\nOkay, then im gonna delet %s from my collection!\n", m.poemName);	
		deletePoem(m.poemName);
		printf("\n");
		kill(childId, SIGUSR1);
		pause();

		unlink("/tmp/am2vz8");
		status = msgctl(ml, IPC_RMID, NULL); 
    		if ( status < 0 ) 
        		perror("msgctl");
	}
}
void ChildProcess(int parentId, int childNum)
{
	signal(SIGUSR1, handler);

	pause(); //waiting for the SIGUSR1 signal (they can only get this, we masked the others)
	
	key_t key = ftok("/tmp/am2vz8",getpid());
	int ml = msgget(key, 0666 | IPC_CREAT );
	char twoPoem[228]= "";
	int fd = open("/tmp/am2vz8", O_RDONLY);
    	read(fd,twoPoem,sizeof(twoPoem));
	close(fd);
	printf("-Child%d:\nI got this two poem:\n%s\n", childNum, twoPoem);
	kill(parentId, SIGUSR1);
	pause();
	
	char *parts[4];
    	int part_count = 0;
   	char *token = strtok(twoPoem, "\n");
    	while (token != NULL) 
	{
        	parts[part_count++] = token;
        	token = strtok(NULL, "\n");
    	}
	int poemNameInd;
	srand(time(NULL));
	if(rand() % 2 == 0)
	{
		poemNameInd = 0;
	}
	else
	{
		poemNameInd = 2;
	}
	printf("-Child%d:\nMy choice is: %s\n\n", childNum, parts[poemNameInd]);
	struct msg_t m;
	m.mtype = getpid();
	strcpy(m.poemName, parts[poemNameInd]);
	int status = msgsnd(ml, &m, sizeof(m.poemName), 0);
    	if (status < 0)
	{
        	perror("msgsnd");
		exit(1);
	}
	kill(parentId, SIGUSR1);
	pause();

	printf("-Child%d (for the bunny family in Baratfa):\n%s\n%s\nMay I sprinkle you?\n", childNum, parts[poemNameInd], parts[poemNameInd + 1]);
	kill(parentId, SIGUSR1);
}

//menu helper methods
int getChoice()
{
	int choice = -1;
	char c;
	do
	{
		printf("Choose one from the following items:\n0 - Quit\n1 - Add new poem\n2 - Show list of poems\n3 - Delete poem\n4 - Edit poem\n5 - Sprinkling\nChoice: ");
		scanf("%d",&choice);
		do //clear puffer
		{
			scanf("%c",&c);
		}
		while(c != '\n');
		system("clear");
		if(choice < 0 || choice > 5)
		{
			printf("Invalid menu item, please choose a number between 0 and 5!\n");
		}
	}
	while(choice < 0 || choice > 5);
	return choice;
}
void shuffle(int *array, size_t n)
{
    if (n > 1) 
    {
        size_t i;
        for (i = 0; i < n - 1; i++) 
        {
          size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
          int t = array[j];
          array[j] = array[i];
          array[i] = t;
        }
    }
}


int main()
{	
	sigset_t sigset;
	sigfillset(&sigset);
	sigdelset(&sigset, SIGUSR1); //send the childs sprinkle with this, and parent get notified when the child is ready to read the poems
	sigprocmask(SIG_BLOCK, &sigset, NULL); //set this mask every process

	pid_t child1 = fork();	
	
	if(child1 < 0)
	{
		perror("The fork calling was not succesful on child1\n");
		exit(1);
	}
	if(child1 > 0) //parent process on child1
	{
		pid_t child2 = fork();	
	
		if(child2 < 0)
		{
			perror("The fork calling was not succesful on child2\n");
			exit(1);
		}
		if(child2 > 0) //parent process on child2
		{
			pid_t child3 = fork();	
	
			if(child3 < 0)
			{
				perror("The fork calling was not succesful on child3\n");
				exit(1);
			}
			if(child3 > 0) //parent process on child3
			{
				pid_t child4 = fork();	
	
				if(child4 < 0)
				{
					perror("The fork calling was not succesful on child4\n");
					exit(1);
				}
				if(child4 > 0) //parent process on child4 (code goes here what only the parent can see)
				{
					system("clear");
					int choice;
					int childsThoseAlreadySprinkledCtr = 0;
					int children[4] = {0, 1, 2, 3};
    					shuffle(children, 4);
					do
					{
						choice = getChoice();			
		
						switch (choice)
						{
							case 0:
								printf("Byee :)\n");
								kill(child1, SIGKILL);
								kill(child2, SIGKILL);
								kill(child3, SIGKILL);
								kill(child4, SIGKILL);
								break;
							case 1:
								addNewPoem();
								break;
							case 2:
								showListOfPoems();
								break;
							case 3:
								deletePoem("");
								break;
							case 4:
								editPoem();
								break;
							case 5:
								if(childsThoseAlreadySprinkledCtr > 3)
								{
									printf("All the children have sprinkled!\n");
								}
								else
								{
									Sprinkle(child1, child2, child3, child4, children[childsThoseAlreadySprinkledCtr], &childsThoseAlreadySprinkledCtr);	
								}
								break;
							default:
								break;
						}	
						if(choice)
						{	
							printf("----------------------------------------------------------\n");
							printf("Press any key to continue!");
							getchar();
							system("clear");
						}
					}
					while(choice !=  0);
				}
				else //child4 process
				{
					ChildProcess(getppid(),4);
				}
			}
			else //child3 process
			{
				ChildProcess(getppid(),3);
			}
		}
		else //child2 process
		{
			ChildProcess(getppid(),2);
		}
	}
	else //child1 process
	{
		ChildProcess(getppid(),1);	
	}
}
