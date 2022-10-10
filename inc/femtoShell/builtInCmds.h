#ifndef BUILT_IN_CMDS_H
#define BUILT_IN_CMDS_H

typedef struct _node
{
	char *name;
	char *value;
	struct _node *next;
} node_t;

extern node_t *shellVarsHead;


int Exit(int cmdArgc, char **cmdArgv);
int cd(int cmdArgc, char **cmdArgv);
int set(int cmdArgc, char **cmdArgv);
int unset(int cmdArgc, char **cmdArgv);
int export(int cmdArgc, char **cmdArgv);


char *addVariable(char *varName, char *varValue);
char *searchForVariable(char *varName);

#endif // BUILT_IN_CMDS_H