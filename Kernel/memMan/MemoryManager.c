#include <MemoryManager.h>

typedef struct MemoryManagerCDT {
	char *nextAddress;
} MemoryManagerCDT;

static MemoryManagerADT memoryManager;

MemoryManagerADT createMemoryManager(void *const restrict memoryForMemoryManager, void *const restrict managedMemory) {
	memoryManager = (MemoryManagerADT) memoryForMemoryManager;
	memoryManager->nextAddress = managedMemory;
}

void *allocMemory(const size_t memoryToAllocate) {
	char *allocation = memoryManager->nextAddress;

	memoryManager->nextAddress += memoryToAllocate;

	return (void *) allocation;
}
