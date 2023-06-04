#include <stdint.h>
#include <pipes.h>

static int getFreePipe();
static uint32_t newPipe(uint32_t pipeId);

static int setCharAtIdx(int idx, char c);
static int getPipeIdx(uint32_t pipeId);

uint32_t baseSemID = 300;
static PipeArray pipesAdmin;

//pipe functions

static uint32_t newPipe(uint32_t pipeId){
    int free = getFreePipe();
    if (free == -1){
        ncPrint("[Kernel] ERROR: No more pipe slots available.");
        return -1;
    }

    Pipe * pipe = &pipesAdmin.pipes[free];
    pipe->id = pipeId;
    pipe->state = OCCUPIED;
    pipe->readIdx = 0;
    pipe->writeIdx = 0;
    pipe->totalProcesses = 0;

    int rLock = semOpen(baseSemID++,0);
    int wLock = semOpen(baseSemID++,0);
    if (rLock == -1 || wLock == -1){
        ncPrint("[Kernel] ERROR: Error creating pipes (mutex variables failure).");
        return -1;
    }
    return pipeId;
}

uint32_t pipeOpen(uint32_t pipeId){
    int idx = getPipeIdx(pipeId);
    if (idx == -1)
    {
        idx = newPipe(pipeId);
        if (idx == -1){
            return -1;
        }
    }
    pipesAdmin.pipes[idx].totalProcesses++;
    return pipeId;
}

int pipeClose(uint32_t pipeId){
    int idx = getPipeIdx(pipeId);
    if (idx == -1)
        return -1;
    Pipe * pipe = &pipesAdmin.pipes[idx];
    if (--pipe->totalProcesses > 0)
        return 1;

    pipe->state = EMPTY;
    semClose(pipe->readLock);
    semClose(pipe->writeLock);
    return 1;
}

int pipeRead(uint32_t pipeId){
    int idx = getPipeIdx(pipeId);
    if (idx == -1)
        return -1;

    Pipe * pipe = &pipesAdmin.pipes[idx];

    semWait(pipe->readLock);

    char c = pipe->buffer[pipe->readIdx];
    pipe->readIdx = (pipe->readIdx + 1) % LEN;

    semPost(pipe->writeLock);

    return c;
}

uint32_t pipeWrite(uint32_t pipeId, char *str){
    int idx = getPipeIdx(pipeId);
    if (idx == -1){
        return -1;
    }
    while (*str != 0){
        setCharAtIdx(idx, *str++); 
        // Para tener mas control sobre la concurrencia, se toma y libera el semáforo
        // cada vez que se escribe un char
    }
    return pipeId;
}

static int setCharAtIdx(int idx, char c){
    Pipe * pipe = &pipesAdmin.pipes[idx];

    semWait(pipe->writeLock);

    pipe->buffer[pipe->writeIdx] = c;
    pipe->writeIdx = (pipe->writeIdx + 1) % LEN;

    semPost(pipe->readLock);

    return 0;
}

static int getPipeIdx(uint32_t pipeId){
    for (int i = 0; i < MAX_PIPES; i++){
        if (pipesAdmin.pipes[i].state == OCCUPIED && pipesAdmin.pipes[i].id == pipeId)
            return i;
    }
    return -1;
}

static int getFreePipe(){
    for (int i = 0; i < MAX_PIPES; i++){
        if (pipesAdmin.pipes[i].state == EMPTY)
            return i;
    }
    return -1;
}

void printIndividualPipe(Pipe pipe){
    ncPrintDec(pipe.id);
    ncPrint("         ");
    ncPrintDec((int)pipe.totalProcesses);
    ncPrint("         ");
    ncPrint(pipe.buffer);
    ncNewline();
}

void dumpPipes(){
    ncPrint("ID         NOfProcs         Content");
    ncNewline();
    for(int i = 0; i < baseSemID; i++){
        if(pipesAdmin.pipes[i].state == OCCUPIED){
            printIndividualPipe(pipesAdmin.pipes[i]);
        }
    }
}