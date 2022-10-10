#include <femtoShell.h>

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


void readCommand(char *cmdLine, int maxLineSize)
{
	size_t cmdLineLen;
	fgets(cmdLine, maxLineSize, stdin);

	// Replace trailing '\n' with NULL terminator
	cmdLineLen = strlen(cmdLine);
	cmdLine[cmdLineLen - 1] = '\0';
}

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
