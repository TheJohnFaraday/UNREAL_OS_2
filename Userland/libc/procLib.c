
#include "procLib.h"

uint64_t p_create(void (*entryPoint)(int, char **), int argc, char **argv,
                  int fg, int *fd)
{
  return sys_p_create_asm(entryPoint, argc, argv, fg, fd);
}

void p_display() { return sys_processDisplay_asm(); }

uint64_t get_pid() { return sys_getPID_asm(); }

uint64_t block(uint64_t pid) { return sys_block_asm(pid); }

uint64_t unblock(uint64_t pid) { return sys_unblock_asm(pid); }

uint64_t kill(uint64_t pid) { return sys_kill_asm(pid); }

void yield() { sys_yield_asm(); }

void waitpid(uint64_t pid) { sys_waitpid_asm(pid); }

void changePriority(uint64_t pid, int priority)
{
  return sys_priority_asm(pid, priority);
}

void toggle(uint64_t pid) { return sys_toggle_asm(pid); }