#ifndef PARSER_H
#define PARSER_H

int parseCmdLine(char *cmdLine, char **cmdArgv);
void readCommand(char *cmdLine, int maxLineSize);

void ignoreSignals(void);
void enableSignals(void);

#endif // PARSER_H