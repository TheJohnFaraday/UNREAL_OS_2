#include <MemoryManager.h>

typedef struct MemoryManagerCDT {
    //Pointer to the possible new memory block
    uint8_t *nextAddress;
} MemoryManagerCDT;

typedef struct Heap_Node{
    size_t size;
    uint8_t status;
    struct Heap_Node *prev;
    struct Heap_Node *next;
} Heap_Node;

static MemoryManagerADT memoryManager;
static int first_time = 0;

void createMemoryManager(void *const restrict memoryForMemoryManager, void *const restrict managedMemory) {
    memoryManager = (MemoryManagerADT)memoryForMemoryManager;
    memoryManager->nextAddress = managedMemory;
}

void *allocMemory(const size_t memoryToAllocate) {
    // We start running through the memory in search for free "blocks"
    uint8_t *current_mem_pos = (void *)START_MEM_USERS;

    Heap_Node *current_node = NULL;
    while (current_mem_pos < memoryManager->nextAddress) {
        current_node = (Heap_Node *)current_mem_pos;

        if (current_node->size >= memoryToAllocate && current_node->status == FREE) {
            // Splitting the node if there is enough space
            if (current_node->size >= memoryToAllocate + sizeof(Heap_Node) + (size_t)MINIMUM_ALLOC_SIZE) {
                Heap_Node *new_node = (Heap_Node *)(current_mem_pos + sizeof(Heap_Node) + memoryToAllocate);
                new_node->size = current_node->size - memoryToAllocate - sizeof(Heap_Node);
                new_node->status = FREE;

                new_node->prev = current_node;
                new_node->next = current_node->next;
                if (current_node->next != NULL) {
                    current_node->next->prev = new_node;
                }
                current_node->next = new_node;

                current_node->size = memoryToAllocate;
            }
            current_node->status = USED;
            return (void *)(current_node + 1);
        }

        current_mem_pos += sizeof(Heap_Node) + current_node->size;
    }

    // Saving the size and status of the new block at the end of the list
    Heap_Node *new_node = (Heap_Node *)memoryManager->nextAddress;
    new_node->size = memoryToAllocate;
    new_node->status = USED;
    if(!first_time){
        new_node->prev = NULL;
        new_node->next = NULL;
        first_time = 1;
    } else {
        if(current_node != NULL){
            new_node->prev = current_node;
            new_node->next = NULL;
            current_node->next = new_node;
        }
    }
    memoryManager->nextAddress += sizeof(Heap_Node) + memoryToAllocate;

    char *allocation_toReturn = (char *)(new_node + 1);

    return (void *)allocation_toReturn;
}

void free(void *ptr) {
    Heap_Node *node = ((Heap_Node *)ptr) - 1;

    if (node->status == USED) {
        node->status = FREE;

        // Coalescing adjacent free blocks
        Heap_Node *prev_node = node->prev;
        Heap_Node *next_node = node->next;

        if (prev_node != NULL && prev_node->status == FREE) {
            prev_node->next = next_node;
            if (next_node != NULL) {
                next_node->prev = prev_node;
            }
            prev_node->size += node->size + sizeof(Heap_Node);
            node = prev_node;
        }

        if (next_node != NULL && next_node->status == FREE) {
            node->next = next_node->next;
            if (next_node->next != NULL) {
                next_node->next->prev = node;
            }
            node->size += next_node->size + sizeof(Heap_Node);
        }
    }
}
