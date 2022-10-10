#include <femtoShell.h>

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
