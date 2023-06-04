#include <stdint.h>
#include <video.h>
#include <keyboard_driver.h>
#include <syscalls.h>
#include <console_driver.h>
#include <time.h>
#include <lib.h>
#include <color.h>
#include <registorsSnapshot.h>
#include <MemoryManager.h>
#include <scheduler.h>
#include <sem.h>


extern uint64_t registerBuffer;

//Here all the syscall handler functions

//In this case, *str is the buffer and lenght his dim
uint64_t sys_read(int fd, char * str, int length){
    uint64_t whereTo = getCurrentOutFD();
    if(whereTo == STDIN){
        return read_from_console(str,length);
    }
    else{
        return pipeRead(whereTo);
    }
}

// Syscall used to write in the screen
// Arguments, fd is the file descriptor, str is the buffer, length is dim and color is the color
uint64_t sys_write(int fd, char * str, int length, Color color){
    uint64_t whereFrom = getCurrentInFD();
    if(whereFrom == 0){
        switch (fd){
        case STDOUT:
            printStringColor(str, color);
            return length;
        case STDERR:
            printStringColor(str, red);
            return length;
        
        }
    }
    else{
        return pipeWrite(whereFrom, str);
    }
}

uint64_t sys_open_pipe(uint64_t pipeId){
    return (uint64_t) pipeOpen((uint32_t) pipeId);
}

uint64_t sys_read_pipe(uint64_t pipeId){
    return (uint64_t) pipeRead((uint32_t) pipeId);
}

uint64_t sys_write_pipe(uint64_t pipeId, char *str){
    return (uint64_t) pipeWrite((uint32_t) pipeId, str);
}

uint64_t sys_close_pipe(uint64_t pipeId){
    return (uint64_t) pipeClose((uint32_t) pipeId);
}

uint64_t sys_print_pipes(){
    dumpPipes();
    return 0;
}

// Syscall used to print blocks of color in the screen
// Arguments, fd is the player that is printing, length is the x coordinate and coor is the y coordinate
uint64_t sys_print(int fd, int length, int coor){
    switch (fd){
        case PLAYER_ONE:
            return printBlockAt(length,coor,red);
        case PLAYER_TWO:
            return printBlockAt(length,coor,blue);
        case CLEAR_SCREEN:
            setBackgroundColor();
        default:
            return -1;
    }
}

// Syscall used to access tick functions on kernel
uint64_t sys_ticker(int fd, int length){
    switch (fd){
        case GET_TICKS:
            return ticks_elapsed();
        case GET_SECONDS_ELAPSED:
            return seconds_elapsed();
        case GET_TIME:
            return time_elapsed(length);
        default:
            return -1;
    }
}

// Syscall used to copy kernel memory to userland
uint64_t sys_memcpy(uint64_t * dest, int length){
    fetch_saved_registors(dest, length * sizeof(uint64_t));
    return 0;
}

uint64_t sys_accessRTC(int fd){
    switch (fd){
        case GET_HOURS:
            return getHours();
        case GET_MINUTES:
            return getMinutes();
        case GET_SECONDS:
            return getSeconds();
        case GET_STATUS_REG_A:
            return getStatusRegA();
        default:
            return -1;
    }
}

uint64_t sys_allocMem(size_t size){
    return (uint64_t)allocMemory(size);
}

void sys_free(void * ptr){
    return free(ptr);
}

void sys_m_dump(){
    return dump();
}

uint64_t sys_p_create(void (*entryPoint)(int, char **), int argc, char **argv, int fg, int *fd){
    return newProcess(entryPoint, argc, argv, fg, fd);
}

void sys_processDisplay(){
    return processDisplay();
}

uint64_t sys_getPID(){
    return getCurrentPID();
}

uint64_t sys_block(uint64_t pid){
    return block(pid);
}

uint64_t sys_unblock(uint64_t pid){
    return unblock(pid);
}

uint64_t sys_kill(uint64_t pid){
    return kill(pid);
}

uint64_t sys_sem(int fd, uint32_t id, char * name, uint32_t value) {
    switch(fd) {
        case SEM_OPEN:
            return sem_open(id, name, value);
        case SEM_WAIT:
            return sem_wait(id);
        case SEM_POST:
            return sem_post(id);
        case SEM_CLOSE:
            return sem_close(id);
        default:
            return -1;  
    }
}

void sys_yield(){
    yield();
}

void sys_waitpid(uint64_t pid){
    waitpid(pid);
}
void sys_priority(uint64_t pid, int priority){
    return changePriority(pid, priority);
}

void sys_toggle(uint64_t pid){
    return toggle(pid);
}