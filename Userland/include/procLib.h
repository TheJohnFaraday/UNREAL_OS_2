#ifndef PROCLIB_H
#define PROCLIB_H

#include <stdint.h>
#include <syscallsAPI.h>

uint64_t p_create(void (*entryPoint)(int, char **), int argc, char **argv, int fg, int *fd);
void p_display();
uint64_t get_pid();
uint64_t block(uint64_t pid);
uint64_t unblock(uint64_t pid);
uint64_t kill(uint64_t pid);
void yield();
void waitpid(uint64_t pid);
void changePriority(uint64_t pid, int priority);
void toggle(uint64_t pid);

#endif

