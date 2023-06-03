#include <test_processes.h>

#define MAX_PROCESSES 5
#define MAJOR_WAIT 10000000

enum State { RUNNING,
             BLOCKED,
             KILLED };

typedef struct P_rq {
  int32_t pid;
  enum State state;
} p_rq;

int64_t test_processes() {
  uint8_t rq;
  uint8_t alive = 0;
  uint8_t action;

  p_rq p_rqs[MAX_PROCESSES];

  while (1) {

    // Create max_processes processes
    for (rq = 0; rq < MAX_PROCESSES; rq++) {
      char *argv[] = {"endlessLoop"};
      p_rqs[rq].pid = p_create((void (*)(int, char **))&endless_loop, 1, argv,0,0);

      if (p_rqs[rq].pid == -1) {
        printfColor("\ntest_processes: ERROR creating process", white);
        return -1;
      } else {
        printfColor("\nProcess created with PID: %d", white, p_rqs[rq].pid);
        p_rqs[rq].state = RUNNING;
        alive++;
      }
    }

    // Randomly kills, blocks or unblocks processes until every one has been killed
    while (alive > 0) {

      for (rq = 0; rq < MAX_PROCESSES; rq++) {
        action = GetUniform(100) % 2;

        switch (action) {
          case 0:
            if (p_rqs[rq].state == RUNNING || p_rqs[rq].state == BLOCKED) {
              if (kill(p_rqs[rq].pid) == -1) {
                printfColor("\nERROR killing process", white);
                return -1;
              }
              printfColor("\nProcess killed with PID: %d", white, p_rqs[rq].pid);
              p_rqs[rq].state = KILLED;
              alive--;
              bussy_wait(MAJOR_WAIT);
            }
            break;

          case 1:
            if (p_rqs[rq].state == RUNNING) {
              if (block(p_rqs[rq].pid) == -1) {
                printfColor("\nERROR blocking process", white);
                return -1;
              }
              printfColor("\nProcess blocked with PID: %d", white, p_rqs[rq].pid);
              p_rqs[rq].state = BLOCKED;
            }
            break;
        }
      }

      // Randomly unblocks processes
      for (rq = 0; rq < MAX_PROCESSES; rq++)
        if (p_rqs[rq].state == BLOCKED && GetUniform(100) % 2) {
          if (unblock(p_rqs[rq].pid) == -1) {
            printfColor("\ntest_processes: ERROR unblocking process", white);
            return -1;
          }
          printfColor("\nProcess unblocked with PID: %d", white, p_rqs[rq].pid);
          p_rqs[rq].state = RUNNING;
        }
    }

    //return 0;
  }
}
