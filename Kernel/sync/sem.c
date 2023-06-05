// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <sem.h>
#include <stddef.h>
#include <video.h>

sem_t *getSem(uint32_t id);

sem_t sems[MAX_SEMS];

void sem_create(uint32_t id, char *name, uint32_t value)
{
    sem_t *sem = NULL;
    for (int i = 0; i < MAX_SEMS; i++)
    {
        if (sems[i].id == 0)
        {
            sem = &sems[i];
            break;
        }
    }

    if (sem == NULL)
    {
        printString("ERROR: Max number of semaphores reached.");
        printNewline();
        return;
    }

    sem->id = id;
    sem->value = 1; // Mirar esto porq no deberia estar hardcodeado
    sem->linkedProcesses = 0;
    sem->blockedPIDsCount = 0;
    sem->mutex = 0;
    for (int i = 0; i < MAX_LENGTH; i++)
    {
        sem->name[i] = name[i];
    }
}
int sem_wait(uint32_t id)
{
    sem_t *sem = getSem(id);
    if (sem == NULL)
    {
        return -1;
    }
    enter_region(&(sem->mutex));
    if (sem->value > 0)
    {
        sem->value--;
        leave_region(&(sem->mutex));
        return 0;
    }
    else
    {
        int pid = getCurrentPID();
        sem->blockedPIDs[sem->blockedPIDsCount++] = pid;
        leave_region(&(sem->mutex));
        block(pid);
    }
    return 0;
}

int sem_post(uint32_t id)
{
    sem_t *sem = getSem(id);
    if (sem == NULL)
    {
        return -1;
    }
    enter_region(&(sem->mutex));
    if (sem->blockedPIDsCount > 0)
    {
        int pid = sem->blockedPIDs[0];
        for (int i = 0; i < sem->blockedPIDsCount - 1; i++)
        {
            sem->blockedPIDs[i] = sem->blockedPIDs[i + 1];
        }
        sem->blockedPIDsCount--;
        unblock(pid);
        leave_region(&(sem->mutex));
        return 0;
    }
    else
    {
        sem->value++;
    }
    leave_region(&(sem->mutex));
    return 0;
}

int sem_open(uint32_t id, char *name, uint32_t value)
{
    sem_t *sem = getSem(id);

    if (sem == NULL)
    {
        sem_create(id, name, value);
    }

    sem = getSem(id);

    if (sem->linkedProcesses >= MAX_BLOCKED_PIDS)
    {
        printString("ERROR: Max number of processes linked to this semaphore reached.");
        printNewline();
        return -1;
    }

    sem->linkedProcesses++;
    return id;
}

int sem_close(uint32_t id)
{
    // unlinks the semaphore from the process

    sem_t *sem = getSem(id);
    if (sem == NULL)
    {
        return -1;
    }
    sem->linkedProcesses--;
    return 0;

    // sem_t * semPtr = getSem(id);
    // if (semPtr == NULL) {
    //     return -1;
    // }
    // semPtr->linkedProcesses--;
    // if (semPtr->linkedProcesses > 0) {
    //     return 0;
    // }

    // for (int i = 0; i < MAX_BLOCKED_PIDS; i++) {
    //     if (semPtr->blockedPIDs[i] != 0) {
    //         unblock(semPtr->blockedPIDs[i]);
    //         semPtr->blockedPIDs[i] = 0;
    //     }
    // }

    // semPtr->value = 0;
    // semPtr->id = 0;
    // semPtr->blockedPIDsCount = 0;
    // semPtr->linkedProcesses = 0;
    // semPtr->mutex = 0;
    // for (int i = 0; i < MAX_LENGTH; i++) {
    //     semPtr->name[i] = 0;
    // }
    // return 0;
}

sem_t *getSem(uint32_t id)
{
    for (int i = 0; i < MAX_SEMS; i++)
    {
        if (sems[i].id == id)
        {
            return &sems[i];
        }
    }
    return NULL;
}
