#ifndef MEMLIB_H
#define MEMLIB_H

#include <stddef.h>
#include <syscallsAPI.h>

void * malloc(size_t size);
void free(void * ptr);
void mem_dump();

#endif