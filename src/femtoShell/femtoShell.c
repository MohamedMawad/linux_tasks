#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <limits.h>
#include <signal.h>
#include <ctype.h>

// printf color codes
void red() { printf("\033[1;31m"); }
void green() { printf("\033[1;32m"); }
void yellow() { printf("\033[1;33m"); }
void blue() { printf("\033[1;34m"); }
void reset() { printf("\033[0m"); }


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


typedef struct _node
{
	char *name;
	char *value;
	struct _node *next;
} node_t;

node_t shellVarsFirstNode = {NULL, NULL, NULL};
node_t *shellVarsHead = &shellVarsFirstNode;

char *searchForVariable(char *varName);

char *searchForVariable(char *varName)
{
	node_t *currentNode = shellVarsHead->next;
	char *retValue = NULL;

	while (currentNode != NULL)
	{
		if (strcmp(currentNode->name, varName) == 0)
		{
			retValue = currentNode->value;
			break;
		}

		currentNode = currentNode->next;
	}

	return retValue;
}

char *addVariable(char *varName, char *varValue);

char *addVariable(char *varName, char *varValue)
{
	node_t *currentNode = shellVarsHead->next;
	char *addedVarValue = varValue;
	int varNameUpdated = 0;

	// check that the varValue is an immediate value or a vraiable starting with $ sign
	if (varValue[0] == '$')
	{
		addedVarValue = searchForVariable(&varValue[1]);

		if (addedVarValue == NULL)
		{
			addedVarValue = getenv(&varValue[1]);
		}
	}

	// Search for the variable name and update it if found
	while (currentNode != NULL)
	{
		if (strcmp(currentNode->name, varName) == 0)
		{
			if (currentNode->value != addedVarValue)
			{
				free(currentNode->value);

				if (addedVarValue == NULL)
				{
					currentNode->value = (char *)malloc(1);
					strcpy(currentNode->value, "");
				}
				else
				{
					currentNode->value = (char *)malloc(strlen(addedVarValue) + 1);
					strcpy(currentNode->value, addedVarValue);
				}
				addedVarValue = currentNode->value;
			}

			varNameUpdated = 1;

			if (getenv(currentNode->name) != NULL)
			{
				setenv(currentNode->name, currentNode->value, 1);
			}

			break;
		}

		currentNode = currentNode->next;
	}

	// create a new variable if not found
	if (varNameUpdated == 0)
	{
		node_t *newVar = (node_t *)malloc(sizeof(node_t));

		newVar->name = (char *)malloc(strlen(varName) + 1);
		strcpy(newVar->name, varName);

		if (addedVarValue == NULL)
		{
			newVar->value = (char *)malloc(1);
			strcpy(newVar->value, "");
		}
		else
		{
			newVar->value = (char *)malloc(strlen(addedVarValue) + 1);
			strcpy(newVar->value, addedVarValue);
		}

		newVar->next = shellVarsHead->next;
		shellVarsHead->next = newVar;

		if (getenv(newVar->name) != NULL)
		{
			setenv(newVar->name, newVar->value, 1);
		}
	}

	return addedVarValue;
}

int set(int cmdArgc, char **cmdArgv);

int set(int cmdArgc, char **cmdArgv)
{
	node_t *currentNode = shellVarsHead->next;
	while (currentNode != NULL)
	{
		printf("%s=%s\n", currentNode->name, currentNode->value);
		currentNode = currentNode->next;
	}

	return 0;
}

int unset(int cmdArgc, char **cmdArgv)
{
	node_t *currentNode = NULL;
	node_t *previousNode = NULL;

	for (int i = 1; i < cmdArgc; i++)
	{
		currentNode = shellVarsHead->next;
		previousNode = shellVarsHead;
		unsetenv(cmdArgv[i]);

		while (currentNode != NULL)
		{
			if (strcmp(currentNode->name, cmdArgv[i]) == 0)
			{
				previousNode->next = currentNode->next;
				currentNode->next = NULL;
				free(currentNode->name);
				free(currentNode->value);
				free(currentNode);
			}
			currentNode = currentNode->next;
			previousNode = previousNode->next;
		}
	}

	// TODO: Check and return the status instead of hardcoded 0
	return 0;
}
int export(int cmdArgc, char **cmdArgv);

int export(int cmdArgc, char **cmdArgv)
{
	char *varValue;

	for (int i = 1; i < cmdArgc; i++)
	{
		if (strchr(cmdArgv[i], '=') == NULL)
		{
			varValue = searchForVariable(cmdArgv[i]);

			if (varValue != NULL)
			{
				setenv(cmdArgv[i], varValue, 1);
			}
		}
		else
		{
			if (isalpha(cmdArgv[i][0]) || (cmdArgv[i][0] == '_'))
			{
				char *assignmentOp = strchr(cmdArgv[i], '=');
				*assignmentOp = 0;

				varValue = addVariable(cmdArgv[i], ++assignmentOp);
				setenv(cmdArgv[i], varValue, 1);
			}
			else
			{
				printf("export: %s: not a valid identifier\n", cmdArgv[i]);
			}
		}
	}

	// TODO: Check and return the status instead of hardcoded 0
	return 0;
}

#define CMD_LINE_MAX 1024
#define CMD_ARGV_MAX 128

void ignoreSignals(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}
void enableSignals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGTSTP, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

void readCommand(char *cmdLine, int maxLineSize)
{
	size_t cmdLineLen;
	fgets(cmdLine, maxLineSize, stdin);

	// Replace trailing '\n' with NULL terminator
	cmdLineLen = strlen(cmdLine);
	cmdLine[cmdLineLen - 1] = '\0';
}

int parseCmdLine(char *cmdLine, char **cmdArgv)
{
	char *token;
	char *rest = cmdLine;
	int cmdStartedFlag = 0;
	int cmdArgc = 0;

	while ((token = strtok_r(rest, " ", &rest)))
	{

		if (cmdStartedFlag == 0)
		{
			char *assignmentOp = strchr(token, '=');
			if ((assignmentOp != NULL) &&
				((isalpha(token[0]) || (token[0] == '_'))))
			{
				// TODO: Hide the following two lines inside addVariable function
				char *varValue = assignmentOp + 1;
				// Separate between the variable name and variable value
				*assignmentOp = '\0';
				addVariable(token, varValue);
			}
			else
			{
				cmdStartedFlag = 1;
			}
		}

		if (cmdStartedFlag == 1)
		{
			if (strchr(token, '$') == NULL)
			{
				cmdArgv[cmdArgc++] = token;
			}
			else
			{
				char *var = searchForVariable(&token[1]);
				if (var == NULL)
				{
					var = getenv(&token[1]);

					if (var != NULL)
					{
						cmdArgv[cmdArgc++] = var;
					}
				}
				else
				{
					cmdArgv[cmdArgc++] = var;
				}
			}
		}
	}

	cmdArgv[cmdArgc] = NULL;

	return cmdArgc;
}

// controlCmd function handels empty cmdLine
// TODO: Extend controlCmd to support ! ; | && || > < and other symbols
int controlCmd(int cmdArgc, char **cmdArgv)
{
	int retStatus = 0;
	if (cmdArgc == 0)
	{
		retStatus = 1;
	}

	return retStatus;
}

int externalCmd(int cmdArgc, char **cmdArgv)
{
	int retStatus = 0;
	pid_t ret_wait, ch_pid;
	int wstatus;

	ch_pid = fork();

	if (ch_pid == 0)
	{
		// child code
		enableSignals();
		execvp(cmdArgv[0], cmdArgv);
		printf("%s: command not found\n", cmdArgv[0]);
		exit(0);
	}
	else if (ch_pid > 0)
	{
		// patrent code
		ret_wait = wait(&wstatus);
		retStatus = 1;
	}
	else
	{
		printf("Error: fork can't create a new chiled\n");
		exit(EXIT_FAILURE);
	}

	return retStatus;
}

typedef int (*ptf)(int, char **);

typedef struct
{
	char cmd[NAME_MAX];
	ptf builtInFunction;
} builtInCmd_t;

int cd(int cmdArgc, char **cmdArgv);
int cd(int cmdArgc, char **cmdArgv)
{
	static char previousPath[PATH_MAX + 1] = {0};
	char currentPath[PATH_MAX + 1];
	int status = 0;

	getcwd(currentPath, PATH_MAX + 1);

	if (cmdArgc == 1 || (strcmp(cmdArgv[1], "~") == 0))
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

		if ((previousPath[0] != 0))
		{
			if (chdir(previousPath) == -1)
			{
				printf("cd: %s: No such file or directory\n", previousPath);
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
		strcpy(previousPath, currentPath);
		previousPath[strlen(currentPath)] = 0;
	}

	return 0;
}

int Exit(int cmdArgc, char **cmdArgv);
int Exit(int cmdArgc, char **cmdArgv)
{
	exit(0);
	return 0;
}

#define BUILT_IN_NUM 5
builtInCmd_t builtInCmdList[BUILT_IN_NUM] = {
	{"exit", Exit},
	{"cd", cd},
	{"set", set},
	{"unset", unset},
	{"export", export},
};

int builtInCmd(int cmdArgc, char **cmdArgv)
{
	int retStatus = 0;

	for (int i = 0; i < BUILT_IN_NUM; i++)
	{
		if (strcmp(cmdArgv[0], builtInCmdList[i].cmd) == 0)
		{
			builtInCmdList[i].builtInFunction(cmdArgc, cmdArgv);
			retStatus = 1;
			break;
		}
	}

	return retStatus;
}

int main(int argc, char **argv)
{
	char cmdLine[CMD_LINE_MAX];
	char *cmdArgv[CMD_ARGV_MAX];
	int cmdArgc;

	ignoreSignals();

	while (1)
	{
		printPrompt();

		readCommand(cmdLine, CMD_LINE_MAX);

		cmdArgc = parseCmdLine(cmdLine, cmdArgv);

		if ((controlCmd(cmdArgc, cmdArgv)) ||
			(builtInCmd(cmdArgc, cmdArgv)) ||
			(externalCmd(cmdArgc, cmdArgv)))
		{
			continue;
		}
	}

	return 0;
}
