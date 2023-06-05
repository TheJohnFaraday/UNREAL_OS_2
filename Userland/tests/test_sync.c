// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <test_sync.h>

#define SEM_NAME "test_semaphore"
#define TOTAL_PAIR_PROCESSES 4

int64_t global; // shared memory

int SEM_ID = 1;

void slowInc(int64_t *p, int64_t inc)
{
  int64_t aux = *p;
  yield(); // This makes the race condition highly probable
  aux += inc;
  *p = aux;
}

void my_process_inc(uint64_t argc, char *argv[])
{

  uint64_t n;
  int64_t inc;
  int8_t use_sem;

  if ((n = satoi(argv[1])) <= 0)
    return;
  if ((inc = satoi(argv[2])) == 0)
    return;
  if ((use_sem = satoi(argv[3])) < 0)
    return;

  if (use_sem)
    if (!sem_open(SEM_ID, SEM_NAME, 1))
    {
      printfColor("test_sync: ERROR opening semaphore\n", red);
      return;
    }

  uint64_t i;
  for (i = 0; i < n; i++)
  {
    if (use_sem)
      sem_wait(SEM_ID);
    slowInc(&global, inc);
    if (use_sem)
      sem_post(SEM_ID);
  }

  if (use_sem)
    sem_close(SEM_ID);

  return;
}

void test_sync(char **argsVec)
{ //{n, use_sem, 0}
  uint64_t pids[2 * TOTAL_PAIR_PROCESSES];

  char *argv1[] = {"inc_process", argsVec[1], "1", argsVec[2]};
  char *argv2[] = {"dec_process", argsVec[1], "-1", argsVec[2]};

  global = 0;

  uint64_t i;

  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++)
  {
    pids[i] = sys_p_create_asm((void(*)(int, char **)) my_process_inc, 4, argv1, 0, NULL);
    pids[i + TOTAL_PAIR_PROCESSES] = sys_p_create_asm((void (*)(int, char **)) my_process_inc, 4, argv2, 0, NULL);
  }

  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++)
  {
    waitpid(pids[i]);
    waitpid(pids[i + TOTAL_PAIR_PROCESSES]);
  }

  printfColor("\nFinal value: %d\n", white, global);

  return;
}
