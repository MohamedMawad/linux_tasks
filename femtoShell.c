#include <stdio.h>
#include <string.h>
#define MAX_IN_SIZE 200

int main(int argc, char **argv)
{

	char input[MAX_IN_SIZE] = {0};
	
	while(1)
	{
		printf("Femto_Shell_Prompt:# ");
		gets(input);
		
		if (strcmp(input, "exit") == 0)
		{
			printf("Good Bye :) \n");
			break;
		}

		printf("You Said: %s\n", input);
	}
	return 0;

}
