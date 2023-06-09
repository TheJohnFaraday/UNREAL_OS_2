#ifndef STDIOAPI_H
#define STDIOAPI_H

#include "color.h"
#include <stdarg.h>
#include <stdint.h>

#define EOF -1

void putChar(const char c);
char getChar();
int printfColor(const char *fmt, Color color, ...);
int scanf(const char *fmt, ...);
char *itoa(int num, char *str, int base);
void reverse(char *str, int length);
void clearScreen();
uint32_t hex2int(char *hex);
void *memset(void *destiny, int32_t c, uint64_t length);
int atoi(char *str);
void sleep(int seconds);

#endif