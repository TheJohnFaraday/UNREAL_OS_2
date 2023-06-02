#include <stdint.h>
#include <MemoryManager.h>
#include <video.h>
#include <stdbool.h>
#include <lib.h>

void * memset(void * destination, int32_t c, uint64_t length)
{
	uint8_t chr = (uint8_t)c;
	char * dst = (char*)destination;

	while(length--)
		dst[length] = chr;

	return destination;
}

int32_t memcompare(const void * s1,const void * s2, uint64_t length)
{
	for(int i =0;i<length;i++)
	{
		if(*((uint8_t *)s1+i)!=*((uint8_t *)s2+i))
			return 1;
	}
	return 0;
}

void * memcpy(void * destination, const void * source, uint64_t length)
{
	/*
	* memcpy does not support overlapping buffers, so always do it
	* forwards. (Don't change this without adjusting memmove.)
	*
	* For speedy copying, optimize the common case where both pointers
	* and the length are word-aligned, and copy word-at-a-time instead
	* of byte-at-a-time. Otherwise, copy by bytes.
	*
	* The alignment logic below should be portable. We rely on
	* the compiler to be reasonably intelligent about optimizing
	* the divides and modulos out. Fortunately, it is.
	*/
	uint64_t i;

	if ((uint64_t)destination % sizeof(uint32_t) == 0 &&
		(uint64_t)source % sizeof(uint32_t) == 0 &&
		length % sizeof(uint32_t) == 0)
	{
		uint32_t *d = (uint32_t *) destination;
		const uint32_t *s = (const uint32_t *)source;

		for (i = 0; i < length / sizeof(uint32_t); i++)
			d[i] = s[i];
	}
	else
	{
		uint8_t * d = (uint8_t*)destination;
		const uint8_t * s = (const uint8_t*)source;

		for (i = 0; i < length; i++)
			d[i] = s[i];
	}

	return destination;
}
uint64_t strlen(const char *s)
{

	uint64_t count = 0;
	while(*s!=0)
	{
		count++;
		s++;
	}
	return count;

}

char ** args(char ** argv, int argc){
    char ** buffer = allocMemory(sizeof(char *) * argc);
      for (int i = 0; i < argc; i++){
            buffer[i] = allocMemory(sizeof(char) * (strlen(argv[i]) + 1));
            if(buffer[i] == NULL){
                  printString("[Kernel] ERROR: Malloc failure.");
                  printNewline();
                  return -1;
            }
            strcpy(argv[i], buffer[i]);
            
      }
      return buffer;
}

void strcpy(char *str1, char *str2){ 
      int i;
      for (i = 0; str1[i] != 0; i++)
            str2[i] = str1[i];
      str2[i] = 0;
}

void reverse(char* str, int length) {
    int start = 0;
    int end = length - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

char* itoa(int num, char* str, int base) {
    int i = 0;
    bool isNegative = false;

    /* Handle 0 explicitely, otherwise empty string is printed for 0 */
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }

    // In standard itoa(), negative numbers are handled only with base 10. Otherwise numbers are considered unsigned.
    if (num < 0 && base == 10) {
        isNegative = true;
        num = -num;
    }

    // Process individual digits
    while (num != 0) {
        int rem = num % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        num = num / base;
    }

    // If number is negative, append '-'
    if (isNegative) {
        str[i++] = '-';
    }

    str[i] = '\0'; // Append string terminator

    // Reverse the string
    reverse(str, i);

    return str;
}

void enter_region(int * lock){
    while(__xchg__(lock, 1) != 0){
        // spin
    }
}

void exit_region(int * lock){
    __xchg__(lock, 0);
}