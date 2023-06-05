// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/* sampleCodeModule.c */
#include <procLib.h>

void shell();

int main()
{
	char *argv[] = {"Shell"};
	p_create((void (*)(int, char **)) &shell, 1, argv, 1, 0); // This is the entry point from the kernel to the userland
	// shell();
	return 0;
}
