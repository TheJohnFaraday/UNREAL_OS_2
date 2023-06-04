#ifndef SEMLIB_H
#define SEMLIB_H

#include <stdint.h>
#include "syscallsAPI.h"

int sem_open(uint32_t id, char *name, uint32_t initialValue);
int sem_wait(uint32_t id);
int sem_post(uint32_t id);
int sem_close(uint32_t id);
int sem_unlink(char *name);

#endif