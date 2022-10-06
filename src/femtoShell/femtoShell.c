#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <limits.h>

// printf color codes
void red() { printf("\033[1;31m"); }
void green() { printf("\033[1;32m"); }
void yellow() { printf("\033[1;33m"); }
void blue() { printf("\033[1;34m"); }
void reset() { printf("\033[0m"); }

// extern char **environ;
// char **environ;

void printPrompt(void);

void printPrompt(void)
{
	char username[NAME_MAX + 1];
	char hostname[_SC_HOST_NAME_MAX + 1];
	char fullPath[PATH_MAX + 1];

	getlogin_r(username, NAME_MAX + 1);
	gethostname(hostname, _SC_HOST_NAME_MAX + 1);
	getcwd(fullPath, PATH_MAX + 1);

	green();
	printf("%s@%s", username, hostname);
	reset();
	printf(":");
	blue();
	printf("%s", fullPath);
	reset();
	printf("# ");
}

int tokenizer(char *inputStr, char separator, char ***output);

int tokenizer(char *inputStr, char separator, char ***output)
{
	int len = strlen(inputStr);
	char *cpdInputStr = (char *)malloc(len + 1);
	strcpy(cpdInputStr, inputStr);
	int numTokens = 0;
	char **currentToken;

	for (int i = 0; i < len; i++)
	{
		if ((cpdInputStr[i] == separator))
		{
			cpdInputStr[i] = '\0';
			numTokens++;
			for (int j = i + 1; j < len - 1; j++)
			{
				if (cpdInputStr[j] != separator)
					break;
				else
					i = j;
			}
		}
	}

	// Tokens is larger than separatos by one
	numTokens++;
	// Adding 1 for a pointer to NULL teminator on the output
	*output = malloc((numTokens + 1) * sizeof(char *));

	if (*output == NULL)
	{
		perror("malloc() call in str_separate");
		exit(1);
	}

	currentToken = *output;
	*currentToken = cpdInputStr;

	for (int i = 0; i < len; i++)
	{
		if (cpdInputStr[i] == '\0')
		{
			// Skip multiple separator
			for (int j = i + 1; j < len - 1; j++)
			{
				if (cpdInputStr[j] != separator)
					break;
				else
					i = j;
			}
			currentToken++;
			*currentToken = &(cpdInputStr[i + 1]);
		}
	}

	// Adding the pointer to NULL teminator at the last element of the output
	currentToken++;
	*currentToken = NULL;
	return numTokens;
}

int main(int argc, char **argv)
{

	char *line = NULL;
	char **cmdArgv = NULL;
	int cmdArgc;
	size_t len = 0;
	ssize_t lineSize = 0;
	pid_t ret_wait, ch_pid;
	int wstatus;

	while (1)
	{
		printPrompt();
		// gets(input);
		lineSize = getline(&line, &len, stdin);
		line[lineSize - 1] = '\0';
		cmdArgc = tokenizer(line, ' ', &cmdArgv);
		// for (int i = 0; i < cmdArgc; i++)
		// {
		// 	printf("argv[%d]: %s\n", i, cmdArgv[i]);
		// }
		if (strcmp(line, "exit") == 0)
		{
			printf("Good Bye :) \n");
			break;
		}

		ch_pid = fork();

		if (ch_pid == 0)
		{
			// child code
			printf("child:\n");
			printf("pid = %d, ppid = %d\n", getpid(), getppid());
			printf("ch_pid = %d\n", ch_pid);
			execvp(cmdArgv[0], cmdArgv);
			printf("%s Not a command\n", line);
			exit(0);
		}
		else if (ch_pid > 0)
		{
			// patrent code
			printf("parent:\n");
			printf("pid = %d, ppid = %d\n", getpid(), getppid());
			printf("ch_pid = %d\n", ch_pid);
			// sleep(20);
			ret_wait = wait(&wstatus);
		}
		else
		{
			printf("Error: fork can't create a new chiled\n");
			exit(EXIT_FAILURE);
		}

		printf("pid = %d, ppid = %d\n", getpid(), getppid());
		printf("You Said: %s\n", line);

		free(cmdArgv[0]);
		free(cmdArgv);
	}

	free(line);

	return 0;
}

// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <sys/types.h>
// #include <unistd.h>
// #include <sys/wait.h>
// #include <limits.h>

// // int main () {
// //   red();
// //   printf("Hello ");
// //   yellow();
// //   printf("world\n");
// // 	blue();
// //   printf("Hello ");
// //   green();
// //   printf("world\n");
// //   reset();
// //   return 0;
// // }

// // printf color codes
// #define RED \033[1;31m
// // void green() { printf("\033[1;32m"); }
// // void yellow() { printf("\033[1;33m"); }
// // void blue() { printf("\033[1;34m"); }
// #define RESET \033[0m

// int main(int argc, char **argv)
// {

// 	char username[NAME_MAX + 1];
// 	char hostname[_SC_HOST_NAME_MAX + 1];
// 	char fullPath[PATH_MAX + 1];
// 	// size_t length;

// 	getlogin_r(username, NAME_MAX + 1);
// 	gethostname(hostname, _SC_HOST_NAME_MAX + 1);
// 	getcwd(fullPath, PATH_MAX + 1);

// 	// green();
// 	printf("RED%s@RESET%s", username, hostname);
// 	// reset();
// 	// printf(":");
// 	// blue();
// 	// printf("%s", fullPath);
// 	// reset();
// 	// printf("# ");
// 	return 0;
// }

// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>

// #define BUFSIZE 100

// int str_separate(char *cpdInputStr, char separator, char ***output)
// {
// 	int len = strlen(cpdInputStr);
// 	int numTokens = 0;
// 	char **currentToken;
// 	int i;
// 	for (i = 0; i < len; i++)
// 	{
// 		if ((cpdInputStr[i] == separator))
// 		{
// 			cpdInputStr[i] = '\0';
// 			numTokens++;
// 			for (int j = i + 1; j < len - 1; j++)
// 			{
// 				if (cpdInputStr[j] != separator)
// 					break;
// 				else
// 					i = j;
// 			}
// 		}
// 	}
// 	numTokens++;
// 	*output = malloc(numTokens * sizeof(char *));
// 	if (*output == NULL)
// 	{
// 		perror("malloc() call in str_separate");
// 		exit(1);
// 	}
// 	currentToken = *output;
// 	*currentToken = cpdInputStr;
// 	for (i = 0; i < len; i++) /* < len: we never get to the last '\0',
// 							   * since strlen gives us the length of
// 							   * the string, not including the NULL */
// 	{
// 		if (cpdInputStr[i] == '\0')
// 		{
// 			currentToken++;
// 			*currentToken = &(cpdInputStr[i + 1]);
// 		}
// 	}
// 	return numTokens;
// }

// int main(int argc, char **argv)
// {
// 	char buf[BUFSIZE];
// 	char **mystrptrs;
// 	char separator = ' ';
// 	int numTokens;
// 	int j;
// 	strncpy(buf, "CSC209 Winter  2008 taught by Karen Reid", 100);

// 	/* Just to prove to ourselves that the string is really in there... */
// 	printf("Contents of buf: %s\n", buf);
// 	printf("Address of buf: %p\n", buf);

// 	numTokens = str_separate(buf, ' ', &mystrptrs);

// 	/* Print out the number of tokens we found */
// 	printf("Tokens found with delimiter '%c': %d\n\n", separator, numTokens);

// 	/* Header for our printout */
// 	printf("Token                        ");
// 	printf("Word                     Starting address\n");
// 	printf("-----------------------------");
// 	printf("-----------------------------------------\n");
// 	for (j = 0; j < numTokens; j++)
// 	{
// 		printf("%5d\t%30s\t%30p\n", j + 1, mystrptrs[j], mystrptrs[j]);
// 	}

// 	/* We'll print this again just to show that buf was modified */
// 	printf("\nContents of buf: %s\n", buf);
// 	printf("(will only display until the first NULL character)\n");

// 	/*
// 	 * str_separate made a call to malloc(); you should make it a
// 	 * practice to always make sure you free() what you malloc()
// 	 * when you're done with it, even though it strictly doesn't
// 	 * matter in this case since we're just exiting.
// 	 *
// 	 * DON'T:
// 	 *   - try to free() memory you didn't allocate.
// 	 *   - try to free() things that have already been free()'d.
// 	 *   - try to dereference a pointer to memory you've free()'d
// 	 */

// 	free(mystrptrs);
// 	return 0;
// }