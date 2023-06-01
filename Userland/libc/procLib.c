
#include "procLib.h"

uint64_t p_create(void (*entryPoint)(int, char **), int argc, char **argv, int fg, int *fd){
	return sys_p_create_asm(entryPoint, argc, argv, fg, fd);
}

void p_display(){
	return sys_processDisplay_asm();
}