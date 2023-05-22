#include <MemoryManager.h>
#include <stdint.h>
#include <video.h>

typedef struct MemoryManagerCDT {
	uint8_t *nextAddress;
} MemoryManagerCDT;

static MemoryManagerADT memoryManager;

void createMemoryManager(void *const restrict memoryForMemoryManager, void *const restrict managedMemory) {
	memoryManager = (MemoryManagerADT) memoryForMemoryManager;
	memoryManager->nextAddress = managedMemory;
}

void *allocMemory(const size_t memoryToAllocate) {
	//We start running though the memory in search for free "blocks"
	uint8_t * current_mem_pos = (void *)START_MEM_USERS;
	
	while(current_mem_pos < memoryManager->nextAddress){
		size_t *current_size = (size_t *)current_mem_pos;
		uint8_t * status = (uint8_t *)(current_mem_pos + sizeof(size_t));

		if(*current_size >= memoryToAllocate && *status == FREE){
			*status = USED;
			return (void *)(status + 1);
		}

		current_mem_pos += *current_size + sizeof(size_t) + 1;
	}

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
