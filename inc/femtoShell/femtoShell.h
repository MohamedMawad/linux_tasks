#ifndef FEMTO_SHELL_H
#define FEMTO_SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <limits.h>
#include <signal.h>
#include <ctype.h>

#include <parser.h>
#include <prompt.h>
#include <cmdTypes.h>
#include <builtInCmds.h>

#define CMD_LINE_MAX 1024
#define CMD_ARGV_MAX 128

#endif // FEMTO_SHELL_H