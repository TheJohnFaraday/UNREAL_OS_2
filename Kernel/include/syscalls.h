#ifndef SYSCALLS_H_
#define SYSCALLS_H_

#include <stddef.h>

#define STDIN  1
#define STDOUT 1
#define STDERR 2

#define PLAYER_ONE 1
#define PLAYER_TWO 2
#define CLEAR_SCREEN 3

#define GET_TICKS 0
#define GET_SECONDS_ELAPSED 1
#define GET_TIME 2

#define GET_HOURS 0
#define GET_MINUTES 1
#define GET_SECONDS 2
#define GET_STATUS_REG_A 3

uint64_t sys_read(int fd, char * str, int length);
uint64_t sys_write(int fd, char * str, int length, Color color);
uint64_t sys_print(int fd, int length, int coor);
uint64_t sys_ticker(int fd, int length);
uint64_t sys_memcpy(uint64_t * dest, int length);
uint64_t sys_accessRTC(int fd);
uint64_t sys_allocMem(size_t size);
void sys_free(void * ptr);
void sys_m_dump();
uint64_t sys_p_create(void (*entryPoint)(int, char **), int argc, char **argv, int fg, int *fd);
void sys_processDisplay();
uint64_t sys_getPID();
uint64_t sys_block(uint64_t pid);
uint64_t sys_unblock(uint64_t pid);
uint64_t sys_kill(uint64_t pid);

#endif