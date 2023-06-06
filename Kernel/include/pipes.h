#ifndef PIPES_H
#define PIPES_H

#include <stdint.h>

#define LEN 2048
#define MAX_PIPES 30
#define OCCUPIED 1
#define EMPTY 0

typedef struct
{
    uint32_t id;
    char buffer[LEN];

    int readIdx;
    int writeIdx;

    long totalProcesses;
    int writeLock;
    int readLock;
    int state;
} Pipe;

typedef struct
{
    Pipe pipes[MAX_PIPES];
} PipeArray;

uint32_t pipeOpen(uint32_t pipeId);
int pipeClose(uint32_t pipeId);
int pipeRead(char *str, uint32_t pipeId);
uint32_t pipeWrite(uint32_t pipeId, char *str);
void dumpPipes();

#endif