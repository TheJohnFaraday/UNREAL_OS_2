// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <memLib.h>

void *malloc(size_t size)
{
	return sys_malloc_asm(size);
}

void free(void *ptr)
{
	return sys_free_asm(ptr);
}

void mem_dump()
{
	return sys_m_dump_asm();
}