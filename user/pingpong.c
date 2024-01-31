#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define SEND_BYTE "z"

int
main(int argc, char *argv[])
{
  int p[2];
  int pid;
  pipe(p);

  pid= fork();
  if (pid == 0) {
    char buf[10];
    read(p[0], buf, 1);
    fprintf(2, "%d: received ping\n", getpid());
    close(p[0]);

    write(p[1], SEND_BYTE, 1);

    close(p[1]);
    exit(0);
  }else{
    char buf[10];
    write(p[1], SEND_BYTE, 1);
    close(p[1]);
    
    read(p[0], buf, 1);
    fprintf(2, "%d: received pong\n", getpid());
    close(p[0]);

    wait(0);

  }
  exit(0);
}
