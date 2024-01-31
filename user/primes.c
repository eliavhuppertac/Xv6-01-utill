#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define MAX 35

void init_pipe(int *p)
{
  close(p[0]);
  for (int i = 2; i <= MAX; i++)
  {
    write(p[1], &i, sizeof(int));
  }
  close(p[1]);
}

void sieve(int *p)
{
  close(p[1]);

  int prime = 0;
  if (read(p[0], &prime, sizeof(int)) > 0)
  {
    fprintf(1, "prime %d\n", prime);
  }
  else
  {
    close(p[0]);
    exit(0);
  }

  int continued_p[2];
  pipe(continued_p);

  int pid = fork();
  if (pid > 0)
  {
    close(continued_p[0]);
    int buffer;
    while (read(p[0], &buffer, sizeof(int)) > 0)
    {
      if (buffer % prime != 0)
      {
        write(continued_p[1], &buffer, sizeof(int));
      }
    }

    close(p[0]);
    close(continued_p[1]);
    wait(0);
  }
  else
  {
    close(p[0]);
    sieve(continued_p);
    exit(0);
  }

  exit(0);
}

int main(int argc, char *argv[])
{
  int p[2];
  int pid;
  pipe(p);

  pid = fork();
  if (pid == 0)
  {
    sieve(p);
    exit(0);
  }
  else
  {
    // int status = 0;
    init_pipe(p);
    wait(0);
    // while(wait(&status) > 0);
  }
  exit(0);
}
