#include <femtoShell.h>

#define BUILT_IN_NUM 5

typedef int (*ptf)(int, char **);
typedef struct
{
	char cmd[NAME_MAX];
	ptf builtInFunction;
} builtInCmd_t;

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
