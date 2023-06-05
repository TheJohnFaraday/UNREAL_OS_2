// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <test_prio.h>

#define MINOR_WAIT 1000000 // TODO: Change this value to prevent a process from flooding the screen
#define WAIT 10000000      // TODO: Change this value to make the wait long enough to see theese processes beeing run at least twice

#define TOTAL_PROCESSES 3
#define LOWEST 0  // TODO: Change as required
#define MEDIUM 1  // TODO: Change as required
#define HIGHEST 2 // TODO: Change as required

int64_t prio[TOTAL_PROCESSES] = {LOWEST, MEDIUM, HIGHEST};

void test_prio()
{
  int64_t pids[TOTAL_PROCESSES];
  char *argv[] = {"endlessLoop", 0};
  uint64_t i;

  for (i = 0; i < TOTAL_PROCESSES; i++)
    pids[i] = p_create((void (*)(int, char **)) & endless_loop_print, 2, argv, 0, 0);

  bussy_wait(WAIT);
  printfColor("\nCHANGING PRIORITIES...\n", white);

  for (i = 0; i < TOTAL_PROCESSES; i++)
    changePriority(pids[i], prio[i]);

  bussy_wait(WAIT);
  printfColor("\nBLOCKING...\n", white);

  for (i = 0; i < TOTAL_PROCESSES; i++)
    block(pids[i]);

  printfColor("CHANGING PRIORITIES WHILE BLOCKED...\n", white);

  for (i = 0; i < TOTAL_PROCESSES; i++)
    changePriority(pids[i], MEDIUM);

  printfColor("UNBLOCKING...\n", white);

  for (i = 0; i < TOTAL_PROCESSES; i++)
    unblock(pids[i]);

  bussy_wait(WAIT);
  printfColor("\nKILLING...\n", white);

  for (i = 0; i < TOTAL_PROCESSES; i++)
    kill(pids[i]);
}
