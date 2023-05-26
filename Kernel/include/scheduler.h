#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <stdint.h>
#include "lib.h"

#define NAME_SIZE 50

typedef enum{
      READY,
      BLOCKED,
      KILLED
} State;

typedef struct{
      uint64_t gs;
      uint64_t fs;
      uint64_t r15;
      uint64_t r14;
      uint64_t r13;
      uint64_t r12;
      uint64_t r11;
      uint64_t r10;
      uint64_t r9;
      uint64_t r8;
      uint64_t rsi;
      uint64_t rdi;
      uint64_t rbp;
      uint64_t rdx;
      uint64_t rcx;
      uint64_t rbx;
      uint64_t rax;
      uint64_t rip;
      uint64_t cs;
      uint64_t eflags;
      uint64_t rsp;
      uint64_t ss;
      uint64_t base;
} StackFrame;

void initScheduler();
int newProcess(void (*entryPoint)(int, char **), int argc, char **argv, int fg, int *fd);
void *scheduler(void *oldRSP);
uint64_t kill(uint64_t pid);
uint64_t block(uint64_t pid);
uint64_t unblock(uint64_t pid);
int getCurrentPID();
void processDisplay();
void changePriority(uint64_t pid, int priority);
void yield();
int currentReadsFrom();
int currentWritesTo();
void waitForPID(uint64_t pid);
int getCurrentOutFD();
int getCurrentInFD();

#endif




