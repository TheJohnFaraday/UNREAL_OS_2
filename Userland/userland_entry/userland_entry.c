/* sampleCodeModule.c */
#include <shell.h>
#include <procLib.h>

int main()
{
	char *argv[] = {"Shell"};
	p_create((void (*)(int, char **)) & shell, 1, argv, 1, 0); // This is the entry point from the kernel to the userland
	// shell();
	return 0;
}
