#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <stddef.h>
#include <stdint.h>

#define START_MEM 0x500000
#define START_MEM_USERS 0x600000
#define END_MEM 0x700000
#define MINIMUM_ALLOC_SIZE 0x20
#define USED 1
#define FREE 0


typedef struct MemoryManagerCDT *MemoryManagerADT;

void createMemoryManager(void *const restrict memoryForMemoryManager, void *const restrict managedMemory);

void *allocMemory(const size_t memoryToAllocate);

void free(void *ptr);

#endif
