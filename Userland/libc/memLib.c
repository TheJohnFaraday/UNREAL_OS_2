
#include <memLib.h>

void * malloc(size_t size){
	return sys_malloc_asm(size);
}

void free(void * ptr){
	return sys_free_asm(ptr);
}

