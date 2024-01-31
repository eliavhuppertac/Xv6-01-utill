#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"



int
main(int argc, char *argv[])
{
  uint i;

  if(argc <= 1){
    fprintf(2, "Usage: %s #ticks\n", argv[0]);
    exit(1);
  }

  i = atoi(argv[1]);
  sleep(i);
  exit(0);
}
