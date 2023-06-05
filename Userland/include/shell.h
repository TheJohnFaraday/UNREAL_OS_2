#ifndef SHELL_H
#define SHELL_H

#include "commands.h"
#include "memLib.h"
#include <stddef.h>

#define ENTER '\x0d'
#define SPACE " "
#define BACKSPACE '\x08'
#define DELETE 127
#define MAX_READ 100
#define MAX_LINE 10
#define MAX_COMMAND 3

void shell();
int exec_command(char **argsVec, int argsNum, int isPipe, int *fd, int fg);
void waiting_command();
void printShellMenu();
void parsing_pipe_commands(char **argsVec, int argsNum ,int pipePos);
int exec_pipe_command(char **argsVec, int argsNum, int fdIn, int fdOut, int fg);

#endif