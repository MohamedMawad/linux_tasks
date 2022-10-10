#ifndef CMD_TYPES_H
#define CMD_TYPES_H

int controlCmd(int cmdArgc, char **cmdArgv);
int externalCmd(int cmdArgc, char **cmdArgv);
int builtInCmd(int cmdArgc, char **cmdArgv);


#endif // CMD_TYPES_H