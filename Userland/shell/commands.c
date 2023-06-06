// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdint.h>
#include <commands.h>
#include <stdioAPI.h>
#include <tron.h>
#include <syscallsAPI.h>
#include <color.h>
#include <test_mm.h>
#include <test_processes.h>
#include <test_sync.h>
#include <test_prio.h>
#include <procLib.h>
#include <phylo.h>

// Prints on screen the help menu
void help(int argsNum, char **argsVec)
{
    printfColor("\n", white);
    printfColor("Command List:\n", white);

    printfColor(" * biggie -> Lets you zoom in\n", white);
    printfColor(" * smalls -> Lets you zoom out\n", white);
    printfColor(" * ps -> Report a snapshot of the current processes \n", white);
    printfColor(" * help -> Prints this menu\n", white);
    printfColor(" * memTest -> Run the memory manager Test\n", white);
    printfColor(" * procTest -> Run the round robin scheduler Test\n", white);
    printfColor(" * prioTest -> Run the round robin priority based Tess\n", white);
    printfColor(" * mem -> Report a snapshot of the current state of the memory\n", white);
    printfColor(" * semTest -> Run a semaphore test with syncronization (semTest n use_sem)\n", white);
    printfColor(" * nice -> Change the priority of a process\n", white);
    printfColor(" * block -> Block a process Ex: block (PID)\n", white);
    printfColor(" * kill -> Kill a process Ex: kill (PID)\n", white);
    printfColor(" * loop -> Print the ID of the process every X seconds Ex: loop (seconds)\n", white);
    printfColor(" * cat -> Print the content of a file Ex: cat (string)\n", white);
    printfColor(" * wc -> Print the number of lines, words and characters of a file Ex: wc (string)\n", white);
    printfColor(" * filter -> Print the content of a file in uppercase Ex: filter (string)\n", white);
    printfColor(" * phylo -> Run the philosophers problem\n", white);
    printfColor(" * clear -> Clears the screen\n", white);

    // printfColor(" *inforeg -> Prints the registers of screenshot taken previously\n",white);
    // printfColor(" *divzero -> Tests div zero exception\n",white);
    // printfColor(" *invopcode -> Tests invalid opcode exception\n",white);
    // printfColor(" *time -> Prints the current time\n",white);
    // printfColor(" *tron -> Lets you play tron\n",white);
    // printfColor(" *getContent -> Print the next 32 bytes to the memory address you pass as an argument \n",white);
}

// Prints on screen the registers of the screenshot taken previously
void inforeg(int argsNum, char **argsVec)
{
    uint64_t registerVector[REGISTERS] = {0};
    // Copy the registers to the vector from the screenshot
    sys_memcpy_asm(registerVector, REGISTERS);

    printfColor("\n", white);
    printfColor("Registers:\n", white);
    printfColor("RAX: 0x%x\n", white, registerVector[15]);
    printfColor("RBX: 0x%x\n", white, registerVector[14]);
    printfColor("RCX: 0x%x\n", white, registerVector[13]);
    printfColor("RDX: 0x%x\n", white, registerVector[12]);
    printfColor("RBP: 0x%x\n", white, registerVector[11]);
    printfColor("RSP: 0x%x\n", white, registerVector[10]);
    printfColor("RDI: 0x%x\n", white, registerVector[9]);
    printfColor("RSI: 0x%x\n", white, registerVector[8]);
    printfColor("R8: 0x%x\n", white, registerVector[7]);
    printfColor("R9: 0x%x\n", white, registerVector[6]);
    printfColor("R10: 0x%x\n", white, registerVector[5]);
    printfColor("R11: 0x%x\n", white, registerVector[4]);
    printfColor("R12: 0x%x\n", white, registerVector[3]);
    printfColor("R13: 0x%x\n", white, registerVector[2]);
    printfColor("R14: 0x%x\n", white, registerVector[1]);
    printfColor("R15: 0x%x\n", white, registerVector[0]);
}

static uint32_t font_size = 16;

// Increases the font size
void biggie(int argsNum, char **argsVec)
{
    if (font_size == 8)
        font_size = 16;
    else if (font_size == 16)
        font_size = 32;
    printfColor("\x1b\x5bsetFontSize %d", white, font_size);
}

// Decreases the font size
void smalls(int argsNum, char **argsVec)
{
    if (font_size == 32)
        font_size = 16;
    else if (font_size == 16)
        font_size = 8;
    printfColor("\x1b\x5bsetFontSize %d", white, font_size);
}

// Function to test div zero exception
void div_zero_exception_tester(int argsNum, char **argsVec)
{
    int j = 1, i = 0;
    j = j / i;
}

// Function to test invalid opcode exception
void invalid_opcode_exception_tester(int argsNum, char **argsVec)
{
    invalidOpcodeTester();
}

// Function to run tron
void tron_command(int argsNum, char **argsVec)
{
    tron();
}

// Clears shell
void clear(int argsNum, char **argsVec)
{
    clearScreen();
}

// Prints the 32 bytes of memory starting at the address passed as an argument
void getContent(int argsNum, char **argsVec)
{
    char *address = (char *)argsVec[0];
    uintptr_t realAddress = (uintptr_t)hex2int(address);
    if (realAddress % ALIGNMENT != 0)
    {
        realAddress -= (realAddress % ALIGNMENT);
    }
    uint8_t *targetAddress = (uint8_t *)realAddress;

    printfColor("\n", white);
    for (int i = 0; i < 8; i++)
    {
        printfColor("0x%x | %x %x %x %x |", white, targetAddress, targetAddress[BYTE_GROUP], targetAddress[BYTE_GROUP + 1],
                    targetAddress[BYTE_GROUP + 2], targetAddress[BYTE_GROUP + 3]);
        printfColor(" %c %c %c %c", white, targetAddress[BYTE_GROUP], targetAddress[BYTE_GROUP + 1], targetAddress[BYTE_GROUP + 2], targetAddress[BYTE_GROUP + 3]);
        printfColor("\n", white);
        targetAddress += 4;
    }
}

// Prints the current time
void time(int argsNum, char **argsVec)
{
    printfColor("\n", white);
    while (1)
    {
        if (sys_accessRTC_asm(GET_STATUS_REG_A) != 0x80)
        {
            printfColor("The time is: %d:%x:%x\n", white, calculateHours(), sys_accessRTC_asm(GET_MINUTES), sys_accessRTC_asm(GET_SECONDS));
            return;
        }
    }
}

// Changes hour format from UTC to local time (UTC-3)
int calculateHours()
{

    int dec = sys_accessRTC_asm(GET_HOURS) & 240;
    dec = dec >> 4;
    int units = sys_accessRTC_asm(GET_HOURS) & 15;
    return ((dec * 10 + units) + 21) % 24;
}

void mem_test(int argsNum, char **argsVec)
{
    test_mm();
}

void proc_test(int argsNum, char **argsVec)
{
    test_processes();
}

void prio_test(int argsNum, char **argsVec)
{
    test_prio();
}

void dump(int argsNum, char **argsVec)
{
    mem_dump();
}

void sem_test(int argsNum, char **argsVec)
{
    test_sync(argsVec);
}

void ps(int argsNum, char **argsVec)
{
    p_display();
}

void loop(int argsNum, char **argsVec)
{
    putChar('\n');
    // print the ID every argsVec[1] seconds
    int seconds = atoi(argsVec[1]);
    int time = sys_ticker_asm(GET_SECONDS_ELAPSED, 0);
    while (1)
    {
        if (sys_ticker_asm(GET_SECONDS_ELAPSED, 0) - time >= seconds)
        {
            printfColor("ID: %d\n", white, sys_getPID_asm());
            time = sys_ticker_asm(GET_SECONDS_ELAPSED, 0);
        }
    }
}

void kill_command(int argc, char **argv)
{

    int pid = atoi(argv[1]);
    printfColor("\nKilling process %d\n", white, pid);
    sys_kill_asm(pid);
}

void nice(int argsNum, char **argsVec)
{
    changePriority(atoi(argsVec[0]), atoi(argsVec[1]));
}

void block_command(int argsNum, char **argsVec)
{
    int pid = atoi(argsVec[1]);
    if (pid <= 2)
    {
        printfColor("You can't block this process\n", white);
        return;
    }
    printfColor("Switching state of process %d\n", white, pid);
    toggle(pid);
}

void cat(int argsNum, char **argsVec)
{
    putChar('\n');
    int i = 0;
    while (argsVec[1][i] != '\0' && argsVec[1][i] != '\n')
        putChar(argsVec[1][i++]);
    putChar('\n');
}

void wc(int argsNum, char **argsVec)
{
    char current;
    int count = 1;
    putChar('\n');

    while ((int)(current = getChar()) != EOF && current != '\0')
    {
        if (current == ENTER)
        {
            current = '\n';
        }

        putChar(current);
        if (current == '\n')
        {
            count++;
        }
    }

    printfColor("\nLines: %d\n", white, count);
}

int is_vowel(char c)
{
    if (c >= 'A' && c <= 'Z')
    {
        c = c + 'a' - 'A';
    }
    if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u')
    {
        return 1;
    }
    return 0;
}

void filter(int argsNum, char **argsVec)
{
    char current;
    putChar('\n');

    while ((int)(current = getChar()) != EOF && current != '\0')
    {
        if (is_vowel(current))
        {
            putChar(current);
        }
    }
    putChar('\n');
}

void phylo(int argsNum, char **argsVec)
{
    runPhylo();
}