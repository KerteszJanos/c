#include <stdlib.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <string.h>

#include "bead1_menuItems.h"

int getChoice()
{
	int choice = -1;
	char c;
	do
	{
		printf("Choose one from the following items:\n0 - Quit\n1 - Add new poem\n2 - Show list of poems\n3 - Delete poem\n4 - Edit poem\nChoice: ");
		scanf("%d",&choice);
		do //clear puffer
		{
			scanf("%c",&c);
		}
		while(c != '\n');
		system("clear");
		if(choice < 0 || choice > 4)
		{
			printf("Invalid menu item, please choose a number between 0 and 4!\n");
		}
	}
	while(choice < 0 || choice > 4);
	return choice;
}

int main()
{	
	int choice;
	do
	{
		choice = getChoice();			
		
		switch (choice)
		{
			case 0:
				printf("Byee :)\n");
				break;
			case 1:
				addNewPoem();
				break;
			case 2:
				showListOfPoems();
				break;
			case 3:
				deletePoem();
				break;
			case 4:
				editPoem();
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
