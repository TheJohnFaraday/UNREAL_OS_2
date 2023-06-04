#ifndef KEYBOARD_BLOCL_STACK_H
#define KEYBOARD_BLOCL_STACK_H

#include <stdint.h>
#include <MemoryManager.h>

#define MAX_STACK_SIZE 100

typedef struct {
    uint64_t stack[MAX_STACK_SIZE];
    int top;
} BlockedStack;

BlockedStack *createBlockedStack();
int isBlockedStackEmpty(BlockedStack *stack);
int isBlockedStackFull(BlockedStack *stack);
void pushBlockedProcess(BlockedStack *stack, uint64_t pid);
uint64_t popBlockedProcess(BlockedStack *stack);
uint64_t peekBlockedProcess(BlockedStack *stack);
void clearBlockedStack(BlockedStack *stack);
void destroyBlockedStack(BlockedStack *stack);

#endif