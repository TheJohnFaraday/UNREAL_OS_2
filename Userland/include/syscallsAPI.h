#ifndef SYSCALLSAPI_H
#define SYSCALLSAPI_H

#include <stdint.h>
#include <color.h>
#include <stdioAPI.h>

#define STDIN  1
#define STDOUT 1
#define STDERR 2


#define GET_TICKS 0
#define GET_SECONDS_ELAPSED 1
#define GET_TIME 2

#define GET_HOURS 0
#define GET_MINUTES 1
#define GET_SECONDS 2
#define GET_STATUS_REG_A 3

extern int sys_read_asm(int fd, char *buffer, int size);
extern int sys_write_asm(int fd, char * str, int length, Color color);
extern int sys_print_asm(int fd, int size, int coor);
extern int sys_ticker_asm(int fd, int length);
extern int sys_memcpy_asm(uint64_t * dest, int length);
extern uint8_t sys_inb_asm(uint16_t port);
extern void sys_out_asm(uint16_t port, uint8_t data);
extern int sys_accessRTC_asm(int fd);
extern void * sys_malloc_asm(size_t size);
extern void sys_free_asm(void * ptr);
extern uint64_t sys_p_create_asm(void (*entryPoint)(int, char **), int argc, char **argv, int fg, int *fd);
#endif