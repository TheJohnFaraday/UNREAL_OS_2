#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdint.h>

#define COMMAND_NUMBER 24
#define MAX_PARAMS 4
#define MAX_LENGHT 100
#define ALIGNMENT 16
#define BYTE_GROUP 4
#define REGISTERS 17
#define ENTER '\x0d'

typedef char args[MAX_PARAMS][MAX_LENGHT];

typedef struct
{
  char *name;
  void (*function)(int argsNum, char **argsVec);
  int args;

} commands_style;

// static commands_style commands[COMMAND_NUMBER];

void inforeg(int argsNum, char **argsVec);
void help(int argsNum, char **argsVec);
void div_zero_exception_tester(int argsNum, char **argsVec);
void invalid_opcode_exception_tester(int argsNum, char **argsVec);
void tron_command(int argsNum, char **argsVec);
void time(int argsNum, char **argsVec);
void getContent(int argsNum, char **argsVec);
void clear(int argsNum, char **argsVec);
void biggie(int argsNum, char **argsVec);
void smalls(int argsNum, char **argsVec);
void mem_test(int argsNum, char **argsVec);
void proc_test(int argsNum, char **argsVec);
void prio_test(int argsNum, char **argsVec);
void dump(int argsNum, char **argsVec);
void ps(int argsNum, char **argsVec);
void loop(int argsNum, char **argsVec);
void kill_command(int argc, char **argv);
void nice(int argsNum, char **argsVec);
void block_command(int argsNum, char **argsVec);
void cat(int argsNum, char **argsVec);
void wc(int argsNum, char **argsVec);
void filter(int argsNum, char **argsVec);
void phylo(int argsNum, char **argsVec);
void sem_test(int argsNum, char **argsVec);

void invalidOpcodeTester();

uint64_t getStatusRegA();
uint64_t getSeconds();
uint64_t getMinutes();
uint64_t getHours();
int uint_64toint(uint64_t number);

int calculateHours();

#endif
