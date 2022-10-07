#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <limits.h>
#include <signal.h>

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

	if (cpdInputStr == NULL)
	{
		perror("malloc() call in str_separate");
		exit(1);
	}

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

	// Adding the pointer to NULL at the last element of the output
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
	// Ignoring the following signals Ctrl+C(SIGINT), Ctrl+Z(SIGTSTP), Ctrl+\(SIGQUIT)
	signal(SIGINT, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);

	while (1)
	{
		printPrompt();
		// gets(input);
		lineSize = getline(&line, &len, stdin);
		// Handling the empty line from the user
		if (lineSize == 1ll)
		{
			continue;
		}
		line[lineSize - 1] = '\0';
		cmdArgc = tokenizer(line, ' ', &cmdArgv);
		// for (int i = 0; i < cmdArgc; i++)
		// {
		// 	printf("argv[%d]: %s\n", i, cmdArgv[i]);
		// }
		if (strcmp(cmdArgv[0], "exit") == 0)
		{
			printf("Good Bye :) \n");
			break;
		}
		else if (strcmp(cmdArgv[0], "cd") == 0)
		{
			static char priviousPath[PATH_MAX + 1] = {0};
			char currentPath[PATH_MAX + 1];
			int status = 0;

			getcwd(currentPath, PATH_MAX + 1);

			if (cmdArgc == 1)
			{
				chdir(getenv("HOME"));
			}
			else if (cmdArgc > 2)
			{
				printf("cd: too many arguments\n");
				status = -1;
			}
			else if (strcmp(cmdArgv[1], "-") == 0)
			{

				if ((priviousPath[0] != 0))
				{
					if (chdir(priviousPath) == -1)
					{
						printf("cd: %s: No such file or directory\n", priviousPath);
						status = -1;
					}
				}
				else
				{
					printf("cd: OLDPWD not set\n");
					status = -1;
				}
			}
			else if (chdir(cmdArgv[1]) == -1)
			{
				printf("cd: %s: No such file or directory\n", cmdArgv[1]);
				status = -1;
			}

			if (status == 0)
			{
				strcpy(priviousPath, currentPath);
				priviousPath[strlen(currentPath)] = 0;
			}
			
			continue;
		}

		ch_pid = fork();

		if (ch_pid == 0)
		{
			// child code
			// printf("child:\n");
			// printf("pid = %d, ppid = %d\n", getpid(), getppid());
			// printf("ch_pid = %d\n", ch_pid);
			execvp(cmdArgv[0], cmdArgv);
			printf("%s Not a command\n", line);
			exit(0);
		}
		else if (ch_pid > 0)
		{
			// patrent code
			// printf("parent:\n");
			// printf("pid = %d, ppid = %d\n", getpid(), getppid());
			// printf("ch_pid = %d\n", ch_pid);
			// sleep(20);
			ret_wait = wait(&wstatus);
		}
		else
		{
			printf("Error: fork can't create a new chiled\n");
			exit(EXIT_FAILURE);
		}

		// printf("pid = %d, ppid = %d\n", getpid(), getppid());
		// printf("%ld\n", lineSize);
		// printf("You Said: %s\n", line);

		free(cmdArgv[0]);
		free(cmdArgv);
	}

	free(line);

	return 0;
}
