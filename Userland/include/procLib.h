#ifndef PROCLIB_H
#define PROCLIB_H

#include <stdint.h>
#include <syscallsAPI.h>

uint64_t p_create(void (*entryPoint)(int, char **), int argc, char **argv, int fg, int *fd);
void p_display();

#endif

