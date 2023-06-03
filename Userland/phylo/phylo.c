#include <phylo.h>

#define MAX_PHILOSOPHERS 10
#define MIN_PHILOSOPHERS 4
#define MUTEX_ID 999
#define MUTEX_NAME "tableMutex"
#define SEM_ID 1000
#define SEM_NAME "forks"

philosopher_t * philosophers[MAX_PHILOSOPHERS];
static int philosophersCount = 0;
static int tableMutex;
static int working;

#define RIGHT(i) ((i + 1)) % (philosophersCount)
#define LEFT(i) ((i) + philosophersCount - 1) % (philosophersCount)

int addPhylo();
int removePhylo();
void attemptsForForks(int i);
void releaseForks(int i);
void checkForks(int i);
void printAll(int argc, char ** argv);
void lifecycle(int argc, char ** argv);

void runPhylo(){
    working = 1;
    tableMutex = sem_open(MUTEX_ID, MUTEX_NAME, 1);
    if (tableMutex < 0){
        printfColor("Error opening mutex\n", red);
        return;
    }
    printfColor("Phylosophers problem\n", white);
    printfColor("Instructions:\n", white);
    printfColor("    - Press 'a' to add a philosopher\n", white);
    printfColor("    - Press 'r' to remove a philosopher\n", white);
    printfColor("    - Press 'q' to quit\n", white);

    sleep(2);

    for (int i = 0; i < MIN_PHILOSOPHERS; i++){
        addPhylo();
    }
    char * args[] = {"printAll"};
    int pid = p_create(printAll, 1, args, 0, NULL);
    while (working) {
        char c = getChar();
        switch (c) {
            case 'a':
                if (addPhylo() < 0){
                    printfColor("Can't add more philosophers (10 max)\n", red);
                }
                else{
                    printfColor("Philosopher added\n", white);
                }
                break;
            case 'r':
                if (removePhylo() < 0){
                    printfColor("Can't remove more philosophers (4 min)\n", red);
                }
                else{
                    printfColor("Philosopher removed\n", white);
                }
                break;
            case 'q':
                printfColor("Quitting...\n", white);
                working = 0;
                break;
            default:
                break;
        }
    }

    for (int i = 0; i < philosophersCount; i++){
        sem_close(philosophers[i]->sem_id);
        kill(philosophers[i]->id);
        free(philosophers[i]);
    }
    philosophersCount = 0;
    kill(pid);
    sem_close(tableMutex);
}

int addPhylo(){
    if (philosophersCount >= MAX_PHILOSOPHERS){
        return -1;
    }
    sem_wait(tableMutex);

    philosopher_t * newPhylo = malloc(sizeof(philosopher_t));
    if (newPhylo == NULL){
        return -1;
    }
    char buffer[3];
    char * name[] = {"phylo", itoa(philosophersCount, buffer, 10)};
    newPhylo->id = p_create(lifecycle, 2, name, 0, NULL);

    newPhylo->state = THINKING;
    newPhylo->sem_id = sem_open(SEM_ID + philosophersCount, SEM_NAME, 0);
    
    philosophers[philosophersCount++] = newPhylo;

    sem_post(tableMutex);

    return 0;
}

int removePhylo(){
    if (philosophersCount <= MIN_PHILOSOPHERS){
        return -1;
    }
    sem_wait(tableMutex); //Chequear si falla por esto

    philosopher_t * phylo = philosophers[--philosophersCount];
    sem_close(phylo->sem_id);
    kill(phylo->id);
    free(phylo);

    sem_post(tableMutex);

    return 0;
}

void lifecycle(int argc, char ** argv){
    int i = atoi(argv[1]);
    while (working){
        attemptsForForks(i);
        sleep(1);
        releaseForks(i);
        sleep(1);
    }
}

void attemptsForForks(int i){
    sem_wait(tableMutex);
    philosophers[i]->state = HUNGRY;
    checkForks(i);
    sem_post(tableMutex);
    sem_wait(philosophers[i]->sem_id);
}

void releaseForks(int i){
    sem_wait(tableMutex);
    philosophers[i]->state = THINKING;
    checkForks(LEFT(i));
    checkForks(RIGHT(i));
    sem_post(tableMutex);
}

void checkForks(int i){
    if (philosophers[i]->state == HUNGRY && philosophers[LEFT(i)]->state != EATING && philosophers[RIGHT(i)]->state != EATING){
        philosophers[i]->state = EATING;
        sem_post(philosophers[i]->sem_id);
    }
}

void printAll(int argc, char ** argv){
    while (working){
        sem_wait(tableMutex);
        for(int i = 0; i < philosophersCount; i++) {
            if (philosophers[i]->state == EATING){
                putChar('E');
                putChar(' ');
            }
            else{
                putChar('.');
                putChar(' ');
            }
        }
        putChar('\n');
        sem_post(tableMutex);
        yield();
    }
}
