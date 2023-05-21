#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <stddef.h>

#define START_MEM 0x500000
#define START_MEM_USERS 0x600000
#define USED 1
#define FREE 0


typedef struct MemoryManagerCDT *MemoryManagerADT;

void createMemoryManager(void *const restrict memoryForMemoryManager, void *const restrict managedMemory);

void *allocMemory(const size_t memoryToAllocate);

void free(void *ptr);

#endif