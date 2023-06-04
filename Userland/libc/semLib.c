#include <semLib.h>

int sem_wait(uint32_t id)
{
    return sys_sem_asm(SEM_WAIT, id, 0, 0);
}

int sem_post(uint32_t id)
{
    return sys_sem_asm(SEM_POST, id, 0, 0);
}

int sem_open(uint32_t id, char *name, uint32_t value)
{
    return sys_sem_asm(SEM_OPEN, id, name, value);
}

int sem_close(uint32_t id)
{
    return sys_sem_asm(SEM_CLOSE, id, 0, 0);
}

int sem_unlink(char *name)
{
    return sys_sem_asm(SEM_UNLINK, 0, name, 0);
}