#include <stdint.h>
#include <shell.h>
#include <stdioAPI.h>
#include <tron.h>
#include <syscallsAPI.h>
#include <color.h>
#include <test_mm.h>
#include <test_processes.h>
#include <procLib.h>

// Prints on screen the help menu
void help(int argsNum, char ** argsVec){
    printfColor("\n", white);
    printfColor("Command List:\n", white);
    //printfColor(" *inforeg -> Prints the registers of screenshot taken previously\n",white);
    //printfColor(" *divzero -> Tests div zero exception\n",white);
    //printfColor(" *invopcode -> Tests invalid opcode exception\n",white);
    //printfColor(" *time -> Prints the current time\n",white);
    printfColor(" *clear -> Clears the screen\n",white);
    //printfColor(" *tron -> Lets you play tron\n",white);
    printfColor(" *biggie -> Lets you zoom in\n",white);
    printfColor(" *smalls -> Lets you zoom out\n",white);
    //printfColor(" *getContent -> Print the next 32 bytes to the memory address you pass as an argument \n",white);
    printfColor(" *ps -> Report a snapshot of the current processes \n",white);
    printfColor(" *help -> Prints this menu\n",white);
    printfColor(" *memTest -> Run the memory manager Test\n",white);
    printfColor(" *procTest -> Run the round robin scheduler Test\n",white);
    printfColor(" *mem -> Report a snapshot of the current state of the memory\n",white);
}

// Prints on screen the registers of the screenshot taken previously
void inforeg(int argsNum, char ** argsVec){
    uint64_t registerVector[REGISTERS] = {0};
    // Copy the registers to the vector from the screenshot
    sys_memcpy_asm(registerVector,REGISTERS);

    printfColor("\n", white);
    printfColor("Registers:\n", white);
    printfColor("RAX: 0x%x\n", white,registerVector[15]);
    printfColor("RBX: 0x%x\n", white,registerVector[14]);
    printfColor("RCX: 0x%x\n", white,registerVector[13]);
    printfColor("RDX: 0x%x\n", white,registerVector[12]);
    printfColor("RBP: 0x%x\n", white,registerVector[11]);
    printfColor("RSP: 0x%x\n", white,registerVector[10]);
    printfColor("RDI: 0x%x\n", white,registerVector[9]);
    printfColor("RSI: 0x%x\n", white,registerVector[8]);
    printfColor("R8: 0x%x\n", white,registerVector[7]);
    printfColor("R9: 0x%x\n", white,registerVector[6]);
    printfColor("R10: 0x%x\n", white,registerVector[5]);
    printfColor("R11: 0x%x\n", white,registerVector[4]);
    printfColor("R12: 0x%x\n", white,registerVector[3]);
    printfColor("R13: 0x%x\n", white,registerVector[2]);
    printfColor("R14: 0x%x\n", white,registerVector[1]);
    printfColor("R15: 0x%x\n", white,registerVector[0]);
}

static uint32_t font_size = 16;

// Increases the font size
void biggie(int argsNum, char ** argsVec)
{
	if(font_size==8)
		font_size = 16;
	else if(font_size==16)
		font_size =32;
	printfColor("\x1b\x5bsetFontSize %d",white,font_size);
}

// Decreases the font size
void smalls(int argsNum, char ** argsVec)
{
	if(font_size==32)
		font_size = 16;
	else if (font_size==16)
		font_size = 8;
	printfColor("\x1b\x5bsetFontSize %d",white,font_size);
}

// Function to test div zero exception
void div_zero_exception_tester(int argsNum, char ** argsVec){
    int j=1,i=0;
    j=j/i;
}

// Function to test invalid opcode exception
void invalid_opcode_exception_tester(int argsNum, char ** argsVec){
    invalidOpcodeTester();
}

// Function to run tron
void tron_command(int argsNum, char ** argsVec){
    tron();
}

// Clears shell
void clear(int argsNum, char ** argsVec){
    clearScreen();
}

// Prints the 32 bytes of memory starting at the address passed as an argument
void getContent(int argsNum, char ** argsVec){
    char * address = (char *) argsVec[0];
    uintptr_t realAddress = (uintptr_t)hex2int(address);
    if (realAddress % ALIGNMENT != 0){
        realAddress -= (realAddress % ALIGNMENT);
    }
    uint8_t * targetAddress = (uint8_t *) realAddress;

    printfColor("\n",white);
    for(int i = 0; i < 8; i++){
        printfColor("0x%x | %x %x %x %x |", white,targetAddress, targetAddress[BYTE_GROUP], targetAddress[BYTE_GROUP + 1],
        targetAddress[BYTE_GROUP + 2], targetAddress[BYTE_GROUP + 3]);
        printfColor(" %c %c %c %c", white,targetAddress[BYTE_GROUP], targetAddress[BYTE_GROUP + 1], targetAddress[BYTE_GROUP + 2], targetAddress[BYTE_GROUP + 3]);
        printfColor("\n",white);
        targetAddress += 4;
    }
    
}

// Prints the current time
void time(int argsNum, char ** argsVec){
    printfColor("\n",white);
    while(1){
        if(sys_accessRTC_asm(GET_STATUS_REG_A) !=0x80){
            printfColor("The time is: %d:%x:%x\n",white,calculateHours() ,sys_accessRTC_asm(GET_MINUTES),sys_accessRTC_asm(GET_SECONDS));
            return;
        }
    }
}

// Changes hour format from UTC to local time (UTC-3)
int calculateHours(){

	int dec = sys_accessRTC_asm(GET_HOURS) & 240;
	dec = dec >> 4;
	int units = sys_accessRTC_asm(GET_HOURS) & 15;
	return ((dec * 10 + units)+21)%24;

}

void mem_test(int argsNum, char ** argsVec){
    test_mm();
}

void proc_test(int argsNum, char ** argsVec){
    test_processes();
}   

void dump(int argsNum, char ** argsVec){
    mem_dump();
    
}

void ps(int argsNum, char ** argsVec){
    p_display();
}

void loop(int argsNum, char ** argsVec){
    // print the ID every argsVec[1] seconds
    int seconds = atoi(argsVec[1]);
    int time = sys_ticker_asm(GET_SECONDS_ELAPSED,0);
    while (1)
    {
        if(sys_ticker_asm(GET_SECONDS_ELAPSED,0) - time >= seconds){
            printfColor("ID: %d\n",white,sys_getPID_asm());
            time = sys_ticker_asm(GET_SECONDS_ELAPSED,0);
        }
    }
    
}

void kill_command(int argc, char ** argv){
    
    int pid = atoi(argv[1]);
    printfColor("\nKilling process %d\n", white, pid);
    sys_kill_asm(pid);
}

void nice(int argsNum, char ** argsVec){
    //sys_nice_asm(argsVec[0], argsVec[1]);
}

void block_command(int argsNum, char ** argsVec){
    sys_block_asm(*(uint64_t *)argsVec[0]);
}

void cat(int argsNum, char ** argsVec){
    // char * filename = argsVec[0];
    // int fd = sys_open_asm(filename);
    // if(fd == -1){
    //     printfColor("File not found\n", white);
    //     return;
    // }
    // char buffer[100];
    // int read = sys_read_asm(fd, buffer, 100);
    // while(read > 0){
    //     printfColor("%s", white, buffer);
    //     read = sys_read_asm(fd, buffer, 100);
    // }
    // sys_close_asm(fd);
}

void wc(int argsNum, char ** argsVec){
    // char * filename = argsVec[0];
    // int fd = sys_open_asm(filename);
    // if(fd == -1){
    //     printfColor("File not found\n", white);
    //     return;
    // }
    // char buffer[100];
    // int read = sys_read_asm(fd, buffer, 100);
    // int lines = 0;
    // int words = 0;
    // int chars = 0;
    // while(read > 0){
    //     for(int i = 0; i < read; i++){
    //         if(buffer[i] == '\n'){
    //             lines++;
    //         }
    //         if(buffer[i] == ' ' || buffer[i] == '\n'){
    //             words++;
    //         }
    //         chars++;
    //     }
    //     read = sys_read_asm(fd, buffer, 100);
    // }
    // printfColor("Lines: %d\n", white, lines);
    // printfColor("Words: %d\n", white, words);
    // printfColor("Chars: %d\n", white, chars);
    // sys_close_asm(fd);
}

void filter(int argsNum, char ** argsVec){
    // char * filename = argsVec[0];
    // int fd = sys_open_asm(filename);
    // if(fd == -1){
    //     printfColor("File not found\n", white);
    //     return;
    // }
    // char buffer[100];
    // int read = sys_read_asm(fd, buffer, 100);
    // while(read > 0){
    //     for(int i = 0; i < read; i++){
    //         if(buffer[i] >= 'a' && buffer[i] <= 'z'){
    //             buffer[i] = buffer[i] - 'a' + 'A';
    //         }
    //     }
    //     sys_write_asm(STDOUT, buffer, read, white);
    //     read = sys_read_asm(fd, buffer, 100);
    // }
    // sys_close_asm(fd);
}

void phylo(int argsNum, char ** argsVec){
    // int n = argsVec[0];
    // int forks[n];
    // for(int i = 0; i < n; i++){
    //     forks[i] = sys_p_create_asm(philosopher, 0, NULL, 1, NULL);
    // }
    // for(int i = 0; i < n; i++){
    //     sys_unblock_asm(forks[i]);
    // }
    // while(1){
    //     sys_unblock_asm(forks[0]);
    // }
}