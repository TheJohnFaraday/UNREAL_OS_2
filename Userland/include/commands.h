#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdint.h>

#define COMMAND_NUMBER 22
#define MAX_PARAMS 4
#define MAX_LENGHT 20
#define ALIGNMENT 16
#define BYTE_GROUP 4
#define REGISTERS 17

typedef char args[MAX_PARAMS][MAX_LENGHT];

typedef struct
{
    char * name;
    void (*function) (int argsNum, args argsVec);
    int args;

}commands_style;


//static commands_style commands[COMMAND_NUMBER];

void inforeg(int argsNum, args argsVec);
void help(int argsNum, args argsVec);
void div_zero_exception_tester(int argsNum, args argsVec);
void invalid_opcode_exception_tester(int argsNum, args argsVec);
void tron_command(int argsNum, args argsVec);
void time(int argsNum, args argsVec);
void getContent(int argsNum, args argsVec);
void clear(int argsNum, args argsVec);
void biggie(int argsNum, args argsVec);
void smalls(int argsNum, args argsVec);
void mem_test(int argsNum, args argsVec);
void proc_test(int argsNum, args argsVec);
void dump(int argsNum, args argsVec);
void ps(int argsNum, args argsVec);
void loop(int argsNum, args argsVec);
void kill_command(int argsNum, args argsVec);
void nice(int argsNum, args argsVec);
void block_command(int argsNum, args argsVec);
void cat(int argsNum, args argsVec);
void wc(int argsNum, args argsVec);
void filter(int argsNum, args argsVec);
void phylo(int argsNum, args argsVec);


static commands_style commands[] = {
    {"biggie", biggie, 0},
    {"smalls", smalls, 0},
    {"inforeg", inforeg, 0},
    {"help", help, 0},
    {"divzero", div_zero_exception_tester, 0},
    {"invopcode",invalid_opcode_exception_tester, 0},
    {"time",time, 0},
    {"clear", clear, 0},
    {"tron",tron_command, 0},
    {"getContent", getContent, 1},
    {"memTest", mem_test, 0},
    {"procTest", proc_test, 0},
    {"dum", dump, 0},
    {"ps", ps, 0},
    {"loop", loop, 1},
    {"kill", kill_command, 1},
    {"nice", nice, 2},
    {"block", block_command, 1},
    {"cat", cat, 0},
    {"wc", wc, 0},
    {"filter", filter, 0},
    {"phylo", phylo, 0}
};

void invalidOpcodeTester();

uint64_t getStatusRegA();
uint64_t getSeconds();
uint64_t getMinutes();
uint64_t getHours();
int uint_64toint(uint64_t number);

int calculateHours();

#endif
