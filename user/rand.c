#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char const *argv[])
{
  int count = 10;

  if(argc > 1) {
    count = atoi(argv[1]);
  }

  printf("rand(%d) numbers:\n", count);
  for(int i = 0; i < count; i++) {
    printf("%d\n", myrand());
  }

  exit(0);
}
