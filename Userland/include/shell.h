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
#define WITH_PIPE 1
#define NOT_PIPE 0

static commands_style commands[] = {
    {"biggie", biggie, 0},
    {"smalls", smalls, 0},
    {"inforeg", inforeg, 0},
    {"help", help, 0},
    {"divzero", div_zero_exception_tester, 0},
    {"invopcode", invalid_opcode_exception_tester, 0},
    {"time", time, 0},
    {"clear", clear, 0},
    {"tron", tron_command, 0},
    {"getContent", getContent, 1},
    {"memTest", mem_test, 0},
    {"procTest", proc_test, 0},
    {"prioTest", prio_test, 0},
    {"mem", dump, 0},
    {"ps", ps, 0},
    {"loop", loop, 1},
    {"kill", kill_command, 1},
    {"nice", nice, 2},
    {"block", block_command, 1},
    {"cat", cat, 1},
    {"wc", wc, 0},
    {"filter", filter, 0},
    {"phylo", phylo, 0},
    {"semTest", sem_test, 0} // Chequear cuantos args toma
};

void shell();
int exec_command(char **argsVec, int argsNum, int isPipe, int *fd, int fg);
void waiting_command();
void printShellMenu();
void parsing_pipe_commands(char **argsVec, int argsNum ,int pipePos);
int exec_pipe_command(char **argsVec, int argsNum, int fdIn, int fdOut, int fg);

#endif