#include <scheduler.h>
#include <lib.h>
#include <stdint.h>

#define MAX_LENGTH 20
#define MAX_BLOCKED_PIDS 100
#define MAX_SEMS 200

typedef struct sem_t {
    uint64_t id;
    uint32_t value;
    char name[MAX_LENGTH];
    int blockedPIDs[MAX_BLOCKED_PIDS];
    uint16_t blockedPIDsCount;
    uint32_t linkedProcesses;
    int mutex;
}sem_t;

sem_t sems[MAX_SEMS];




