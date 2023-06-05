#ifndef SYSCALLSAPI_H
#define SYSCALLSAPI_H

#include "color.h"
#include <stddef.h>
#include <stdint.h>

#define STDIN 0
#define STDOUT 1
#define STDERR 2

#define GET_TICKS 0
#define GET_SECONDS_ELAPSED 1
#define GET_TIME 2

#define GET_HOURS 0
#define GET_MINUTES 1
#define GET_SECONDS 2
#define GET_STATUS_REG_A 3

#define SEM_OPEN 0
#define SEM_WAIT 1
#define SEM_POST 2
#define SEM_CLOSE 3
#define SEM_UNLINK 4

extern int sys_read_asm(int fd, char *buffer, int size);
extern int sys_write_asm(int fd, char *str, int length, Color color);
extern int sys_print_asm(int fd, int size, int coor);
extern int sys_ticker_asm(int fd, int length);
extern int sys_memcpy_asm(uint64_t *dest, int length);
extern uint8_t sys_inb_asm(uint16_t port);
extern void sys_out_asm(uint16_t port, uint8_t data);
extern int sys_accessRTC_asm(int fd);
extern void *sys_malloc_asm(size_t size);
extern void sys_free_asm(void *ptr);
extern void sys_m_dump_asm();
extern uint64_t sys_p_create_asm(void (*entryPoint)(int, char **), int argc,
                                 char **argv, int fg, int *fd);
extern void sys_processDisplay_asm();
extern uint64_t sys_getPID_asm();
extern uint64_t sys_block_asm(uint64_t pid);
extern uint64_t sys_unblock_asm(uint64_t pid);
extern uint64_t sys_kill_asm(uint64_t pid);
extern uint64_t sys_sem_asm(int fd, uint32_t id, char *name, uint32_t value);
extern void sys_yield_asm();
extern void sys_waitpid_asm(uint64_t pid);
extern void sys_priority_asm(uint64_t pid, int priority);
extern void sys_toggle_asm(uint64_t pid);
extern uint64_t sys_open_pipe_asm(uint64_t pipeId);
extern uint64_t sys_close_pipe_asm(uint64_t pipeId);
extern uint64_t sys_print_pipes_asm();
#endif