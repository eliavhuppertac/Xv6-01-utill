#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

#define BUF_SIZE 1024

int loadArgs(int argc, char **argv, char *command_args[MAXARG])
{
  for (int i = 0; i < argc - 2; i++)
  {
    command_args[i + 1] = argv[i + 2];
  }

  int arg_index = argc - 2;
  char buffer[BUF_SIZE];
  char *cursor = buffer;
  int char_count = 0;
  int buffer_size = 0;


  while (1)
  {
    int flag = 0;
    int bytes_read = read(0, cursor, 1);
    if (bytes_read == 0)
    {
      *cursor = '\0';
      break;
    }
    else if (bytes_read < 0)
    {
      fprintf(2, "Error reading stdin.\n");
      exit(1);
    }
    else if (char_count < BUF_SIZE - 1)
    {

      char_count++;
      buffer_size++;

      if (*cursor == ' ' || *cursor == '\n'){
        if (*cursor == '\n'){
          flag = 1;
        }
        *cursor = '\0';

        command_args[++arg_index] = malloc(buffer_size);
        strcpy(command_args[arg_index], buffer);
        buffer_size = 0;
        cursor = buffer;

        if (flag){
          break;
        }
      }
      else{
        cursor++;
      }

    }
  }

  return char_count;
}

int main(int argc, char **argv)
{

  if (argc < 2)
  {
    fprintf(2, "Usage: %s command [initial-arguments]\n", argv[0]);
    exit(1);
  }

  char *command = argv[1];
  // char *command = malloc(strlen(argv[1] + 1)); 
  // strcpy(command, argv[1]);

  
  char *command_args[MAXARG];

  while (loadArgs(argc, argv, command_args) != 0)
  {
    if (fork() == 0)
    {
      command_args[0] = command;
      exec(command, command_args);
      exit(1); // should never get here
    }
    wait(0);
  }

  exit(0);
}
