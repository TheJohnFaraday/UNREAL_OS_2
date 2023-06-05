// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <color.h>
#include <memLib.h>
#include <procLib.h>
#include <shell.h>
#include <stdioAPI.h>
#include <string.h>
#include <syscallsAPI.h>
#include <tron.h>
#include <pipeLib.h>
#include <semLib.h>

static int pipe_fd = 2;

void shell()
{
  printfColor("Welcome to the shell!\n", yellow);
  printfColor("Добро пожаловать в shell!", yellow);
  waiting_command();
}

void waiting_command()
{
  while (1)
  {
    printfColor("\nType 'help' to see the available commands\n", yellow);
    char command_reader[MAX_READ];
    printfColor("\n%s", pink, "$ ");
    int count = 0;
    int pipe_exec = 0;

    char current;
    while ((current = getChar()) != ENTER)
    {
      if (current == BACKSPACE || current == DELETE)
      {
        if (count > 0)
        {
          command_reader[count] = '\0';
          count--;
          printfColor("%c", lime, current);
        }
      }
      else if (current != -1)
      {
        command_reader[count] = current;
        count++;
        printfColor("%c", lime, current);
      }
    }
    // Preparing the command for read
    // First i get the line
    char line[count + 1];
    for (int i = 0; i < count; i++)
    {
      line[i] = command_reader[i];
    }
    line[count] = '\0';
    // Now i use strtok for delete the spaces
    char *toRead = my_strtok(line, SPACE);

    // char command[MAX_LENGHT] = {0};
    // Consume the command
    // strcpy(command, toRead);
    // toRead = my_strtok (NULL, SPACE);

    char **tokens = malloc(sizeof(char *) * MAX_LINE);

    // Now i consume the args
    int index = 0;
    while (toRead != NULL)
    {
      tokens[index] = malloc(sizeof(char) * MAX_LENGHT);
      strcpy(tokens[index++], toRead);
      toRead = my_strtok(NULL, SPACE);
    }

    for (int i = 0; i < index; i++)
    {
      if (!strcmp(tokens[i], "|") && i > 0 && i + 1 < index)
      {
        parsing_pipe_commands(tokens, index ,i);
        pipe_exec++;
        break;
      }
    }

    if(!pipe_exec){
      exec_command(tokens, index - 1, 0, 0, 1);
    }

    for (int i = 0; i < index; i++)
    {
      free(tokens[i]);
    }

    free(tokens);
  }
}

// Checks if command is a valid command and executes it
int exec_command(char **argsVec, int argsNum, int isPipe, int *fd, int fg)
{
  int found = 0;
  int args_check = 0;
  int to_execute;
  for (int i = 0; i < COMMAND_NUMBER && !found; i++)
  {
    if (!(strcmp(commands[i].name, argsVec[0])))
    {
      found = 1;
      // BackGround?
     // if (!strcmp(argsVec[argsNum - 1], "&"))
      //{
      //  argsNum--;
      //  fg = 0;
      //}
      // Checking if the amount of args is correct
      if (commands[i].args == argsNum)
      {
        args_check = 1;
        to_execute = i;
      }
    }
  }

  if (found && args_check)
  {
    if(isPipe){
      return p_create((void (*)(int, char **))commands[to_execute].function, argsNum + 1,
             argsVec, fg, fd);
    }
    else {
      return p_create((void (*)(int, char **))commands[to_execute].function, argsNum + 1,
              argsVec, 1, 0);
    }
  }
  else if (found && !args_check)
  {
    printfColor("\n Invalid params\n", white);
    return -1;
  }

  else
  {
    printfColor("\n Command Not found\n", white);
    return -1;
  }
}

void parsing_pipe_commands(char **argsVec, int argsNum ,int pipePos) 
{ 
  
  printfColor("\n PIPE command! \n", white); 

  //We separate the two commands
  uint64_t pids[2];
  char ** argsVec1 = malloc(sizeof(char *) * MAX_COMMAND);
  char ** argsVec2 = malloc(sizeof(char *) * MAX_COMMAND);
  int argc1 = 0;
  int argc2 = 0;

  uint64_t pipe = open_pipe(pipe_fd);

  int i = 0;
  for (; i < pipePos; i++)
  {
      argsVec1[i] = malloc(sizeof(char) * MAX_LENGHT);
      strcpy(argsVec1[i], argsVec[i]);
      argc1++;
  }
  
   pids[0] = exec_pipe_command(argsVec1, argc1, 0, pipe, 1);

   if (pids[0] == -1){
        close_pipe(pipe);
        //Not forget to free resources
        return;
   }

  i++;

  for (int j = 0; i < argsNum; j++)
  {
      argsVec2[j] = malloc(sizeof(char) * MAX_LENGHT);
      strcpy(argsVec2[j], argsVec[i++]);
      argc2++;
  }

  pids[1] = exec_pipe_command(argsVec2, argc2, pipe, 1, 1);

  if (pids[1] == -1){
        close_pipe(pipe);
        //Not forget to free resources
        return;
   }

    //int a = -1;

    //write_pipe(pipe, (char *)&a);
    //sem_wait(pids[0]);
    close_pipe(pipe);

    for (int i = 0; i < argc1; i++)
    {
      free(argsVec1[i]);
    }
    
    free(argsVec1);

    for (int i = 0; i < argc2; i++)
    {
      free(argsVec2[i]);
    }
    
    free(argsVec2);

    return; 

}

int exec_pipe_command(char **argsVec, int argsNum, int fdIn, int fdOut, int fg){
      int fd[2];

      fd[0] = fdIn;
      fd[1] = fdOut;

      return exec_command(argsVec, argsNum-1, 1, fd, fg);
}
