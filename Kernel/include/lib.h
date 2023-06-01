#ifndef LIB_H
#define LIB_H

#include <stdint.h>


void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);
int32_t memcompare(const void * s1,const void * s2, uint64_t length);
uint64_t strlen(const char *s);
char ** args(char ** argv, int argc);
void strcpy(char * dest, char * src);
void timerInterrupt();
void reverse(char* str, int length);
char * itoa(int num, char * str, int base);


char *cpuVendor(char *result);
extern uint64_t getHours();
extern uint64_t getMinutes();
extern uint64_t getSeconds();
extern uint64_t getStatusRegA();

#endif
