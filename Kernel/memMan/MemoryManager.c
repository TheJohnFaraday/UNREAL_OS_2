#include <MemoryManager.h>
#include <stdint.h>

typedef struct MemoryManagerCDT {
	uint8_t *nextAddress;
} MemoryManagerCDT;

static MemoryManagerADT memoryManager;
static char * memoryStartPosition = (void *)START_MEM_USERS;

void createMemoryManager(void *const restrict memoryForMemoryManager, void *const restrict managedMemory) {
	memoryManager = (MemoryManagerADT) memoryForMemoryManager;
	memoryManager->nextAddress = managedMemory;
}

void *allocMemory(const size_t memoryToAllocate) {
	//Saving the size of the block
	*(size_t *)memoryManager->nextAddress = memoryToAllocate;
    memoryManager->nextAddress += sizeof(size_t);

	//Saving status data
	*memoryManager->nextAddress = USED;
    memoryManager->nextAddress++;

	char *allocation_toReturn = (char *)memoryManager->nextAddress;
    memoryManager->nextAddress += memoryToAllocate;

	return (void *) allocation_toReturn;
}

void free(void *ptr) {
  uint8_t *bytePtr = (uint8_t *)ptr;
    if (*(bytePtr - 1) == USED) {
        *(bytePtr - 1) = FREE;
    }
}
