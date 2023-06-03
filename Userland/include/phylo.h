#ifndef PHYLO_H
#define PHYLO_H

#include <stdioAPI.h>
#include <semLib.h>
#include <procLib.h>
#include <memLib.h>

typedef enum {THINKING = 1, HUNGRY = 2, EATING = 3} state_t;

typedef struct philosopher_t {
    int id;
    int sem_id;
    state_t state;
    
} philosopher_t;

void runPhylo();

#endif