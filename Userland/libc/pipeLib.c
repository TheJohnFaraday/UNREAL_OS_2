// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "pipeLib.h"

uint64_t open_pipe(uint64_t pipeId)
{
    return sys_open_pipe_asm(pipeId);
}

uint64_t close_pipe(uint64_t pipeId)
{
    return sys_close_pipe_asm(pipeId);
}

uint64_t print_pipes()
{
    return sys_print_pipes_asm();
}