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
static void freeResources(char **argsVec, int argsNum);

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
    char **tokens = malloc(sizeof(char *) * MAX_LINE);
    if (tokens == NULL)
    {
      printfColor("\nFailed to allocate memory for tokens.\n", white);
      continue;
    }

    // Now i consume the args
    int index = 0;
    while (toRead != NULL)
    {
      tokens[index] = malloc(sizeof(char) * MAX_LENGHT);
      if (tokens[index] == NULL)
      {
        printfColor("\nFailed to allocate memory for token.\n", white);
        index = 0;
        freeResources(tokens, index);
        break;
      }
      strcpy(tokens[index++], toRead);
      toRead = my_strtok(NULL, SPACE);
    }

    if (index)
    {
      for (int i = 0; i < index; i++)
      {
        if (!strcmp(tokens[i], "|") && i > 0 && i + 1 < index)
        {
          parsing_pipe_commands(tokens, index, i);
          pipe_exec++;
          break;
        }
      }

      if (!pipe_exec)
      {
        exec_command(tokens, index, NOT_PIPE, STANDARD, FG);
      }

      for (int i = 0; i < index; i++)
      {
        free(tokens[i]);
      }

      free(tokens);
    }
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
      if (!strcmp(argsVec[argsNum - 1], "&"))
      {
        argsNum--;
        fg = BG;
      }
      // Checking if the amount of args is correct
      if (commands[i].args == argsNum - 1)
      {
        args_check = 1;
        to_execute = i;
      }
    }
  }

  if (found && args_check)
  {
    if (isPipe)
    {
      return p_create((void (*)(int, char **))commands[to_execute].function, argsNum,
                      argsVec, fg, fd);
    }
    else
    {
      return p_create((void (*)(int, char **))commands[to_execute].function, argsNum,
                      argsVec, fg, STANDARD);
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

void parsing_pipe_commands(char **argsVec, int argsNum, int pipePos)
{

  // We separate the two commands
  uint64_t pids[2];
  char **argsVec1 = malloc(sizeof(char *) * MAX_COMMAND);
  if (argsVec1 == NULL)
  {
    printfColor("\nFailed to allocate memory for argsVec1.\n", white);
    return;
  }

  char **argsVec2 = malloc(sizeof(char *) * MAX_COMMAND);
  if (argsVec2 == NULL)
  {
    printfColor("\nFailed to allocate memory for argsVec2.\n", white);
    free(argsVec1);
    return;
  }

  int argc1 = 0;
  int argc2 = 0;

  uint64_t pipe = open_pipe(pipe_fd);

  int i = 0;
  for (; i < pipePos; i++)
  {
    argsVec1[i] = malloc(sizeof(char) * MAX_LENGHT);
    if (argsVec1[i] == NULL)
    {
      printfColor("\nFailed to allocate memory for argsVec1[%d].\n", white, i);
      freeResources(argsVec1, argc1);
      free(argsVec2);
      return;
    }
    strcpy(argsVec1[i], argsVec[i]);
    argc1++;
  }

  pids[0] = exec_pipe_command(argsVec1, argc1, STDIN, pipe, FG);

  if (pids[0] == -1)
  {
    close_pipe(pipe);
    freeResources(argsVec1, argc1);
    free(argsVec2);
    return;
  }

  i++;

  for (int j = 0; i < argsNum; j++)
  {
    argsVec2[j] = malloc(sizeof(char) * MAX_LENGHT);
    if (argsVec2[j] == NULL)
    {
      printfColor("\nFailed to allocate memory for argsVec2[%d].\n", white, j);
      freeResources(argsVec1, argc1);
      freeResources(argsVec2, argc2);
      return;
    }
    strcpy(argsVec2[j], argsVec[i++]);
    argc2++;
  }

  pids[1] = exec_pipe_command(argsVec2, argc2, pipe, STDOUT, FG);

  close_pipe(pipe);

  freeResources(argsVec1, argc1);
  freeResources(argsVec2, argc2);

  return;
}

int exec_pipe_command(char **argsVec, int argsNum, int fdIn, int fdOut, int fg)
{
  int fd[2];

  fd[0] = fdIn;
  fd[1] = fdOut;

  return exec_command(argsVec, argsNum, WITH_PIPE, fd, fg);
}

static void freeResources(char **argsVec, int argsNum)
{
  for (int i = 0; i < argsNum; i++)
  {
    free(argsVec[i]);
  }
  free(argsVec);
}
