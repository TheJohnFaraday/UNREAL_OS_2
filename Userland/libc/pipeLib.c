
#include "pipeLib.h"

uint64_t open_pipe(uint64_t pipeId)
{
    return sys_open_pipe_asm(pipeId);
}

uint64_t read_pipe(uint64_t pipeId)
{
    return sys_read_pipe_asm(pipeId);
}

uint64_t write_pipe(uint64_t pipeId, char *str)
{
    return sys_write_pipe_asm(pipeId, str);
}

uint64_t close_pipe(uint64_t pipeId)
{
    return sys_close_pipe_asm(pipeId);
}

uint64_t print_pipes()
{
    return sys_print_pipes_asm();
}