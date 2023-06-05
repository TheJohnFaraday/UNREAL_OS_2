// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <keyboard_block_stack.h>

BlockedStack *createBlockedStack()
{
    BlockedStack *stack = allocMemory(sizeof(BlockedStack));
    stack->top = -1;
    return stack;
}

int isBlockedStackEmpty(BlockedStack *stack)
{
    return stack->top == -1;
}

int isBlockedStackFull(BlockedStack *stack)
{
    return stack->top == MAX_STACK_SIZE - 1;
}

void pushBlockedProcess(BlockedStack *stack, uint64_t pid)
{
    if (isBlockedStackFull(stack))
    {
        return; // Stack overflow
    }
    stack->stack[++stack->top] = pid;
}

uint64_t popBlockedProcess(BlockedStack *stack)
{
    if (isBlockedStackEmpty(stack))
    {
        return -1; // Stack underflow
    }
    return stack->stack[stack->top--];
}

uint64_t peekBlockedProcess(BlockedStack *stack)
{
    if (isBlockedStackEmpty(stack))
    {
        return -1; // Stack is empty
    }
    return stack->stack[stack->top];
}

void clearBlockedStack(BlockedStack *stack)
{
    stack->top = -1;
}

void destroyBlockedStack(BlockedStack *stack)
{
    free(stack);
}
