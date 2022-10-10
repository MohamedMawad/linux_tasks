#include <femtoShell.h>

node_t shellVarsFirstNode = {NULL, NULL, NULL};
node_t *shellVarsHead = &shellVarsFirstNode;

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

int Exit(int cmdArgc, char **cmdArgv)
{
	exit(0);
	return 0;
}

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
