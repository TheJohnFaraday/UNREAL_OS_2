#ifndef PIPELIB_H
#define PIPELIB_H

#include <stdint.h>
#include "syscallsAPI.h"

uint64_t open_pipe(uint64_t pipeId);
uint64_t close_pipe(uint64_t pipeId);
uint64_t print_pipes();

#endif