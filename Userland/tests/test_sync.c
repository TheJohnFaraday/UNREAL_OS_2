#include <test_sync.h>

#define SEM_NAME "test_semaphore"
#define TOTAL_PAIR_PROCESSES 10

int64_t global; // shared memory

int SEM_ID = 1;

void slowInc(int64_t *p, int64_t inc)
{
  int64_t aux = *p;
  yield(); // This makes the race condition highly probable
  aux += inc;
  *p = aux;
}

uint64_t my_process_inc(uint64_t argc, char *argv[])
{

  uint64_t n;
  int64_t inc;
  int8_t use_sem;
  char *name = argv[0];

  if ((n = satoi(argv[1])) <= 0)
    return -1;
  if ((inc = satoi(argv[2])) == 0)
    return -1;

  use_sem = 1;

  if (use_sem)
    if (!sem_open(SEM_ID, SEM_NAME, 1))
    {
      printfColor("test_sync: ERROR opening semaphore\n", red);
      return -1;
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

  return 0;
}

uint64_t test_sync()
{ //{n, use_sem, 0}
  uint64_t pids[2 * TOTAL_PAIR_PROCESSES];

  char *argv1[] = {"inc_process", "10", "1"};
  char *argv2[] = {"dec_process", "10", "-1"};

  global = 0;

  uint64_t i;

  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++)
  {
    pids[i] = sys_p_create_asm(my_process_inc, 3, argv1, 0, NULL);
    pids[i + TOTAL_PAIR_PROCESSES] = sys_p_create_asm((void (*)(int, char **)) my_process_inc, 3, argv2, 0, NULL);
  }

  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++)
  {
    waitpid(pids[i]);
    waitpid(pids[i + TOTAL_PAIR_PROCESSES]);
  }

  printfColor("Final value: %d\n", white, global);

  return 0;
}
