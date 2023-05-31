#ifndef STDIOAPI_H
#define STDIOAPI_H

#include <stdarg.h>
#include <stdint.h>
#include <color.h>
#include <stddef.h>

void putChar(const char c);
char getChar();
int printfColor(const char* fmt, Color color, ...);
int scanf(const char* fmt, ...);
char* itoa(int num, char* str, int base);
void reverse(char * str, int length);
void clearScreen();
uint32_t hex2int(char *hex);
void * memset(void * destiny, int32_t c, uint64_t length);
void * malloc(size_t size);
void free(void * ptr);
uint64_t p_create(void (*entryPoint)(int, char **), int argc, char **argv, int fg, int *fd);
void p_display();

#endif