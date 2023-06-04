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
#define MAX_COMMAND 10

void shell();
void reading_command(char **argsVec, int argsNum);
void waiting_command();
void printShellMenu();
void execute_piped_commands();

#endif