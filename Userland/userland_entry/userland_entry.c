/* sampleCodeModule.c */
#include <shell.h>
#include <stdioAPI.h>

int main() {
	//char * argv[] = {"Shell"};
	//p_create((void (*)(int, char **))&shell, 1, (char **)argv, 1, 0); // This is the entry point from the kernel to the userland
	shell();
	return 0; 
}
