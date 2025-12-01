#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char const *argv[])
{
  if (argc == 2 && strcmp(argv[1], "?") == 0) {
    printf("Usage: rand [count]\nGenerates random numbers. Default count is 10.\n");
    exit(0);
  }

  if (argc > 2) {
    fprintf(2, "Usage: rand [count]\n(Use 'rand ?' for help)\n");
    exit(1);
  }

  int count = 10; // Default
  if(argc == 2) {
    count = atoi(argv[1]);
    if (count <= 0) {
        fprintf(2, "rand: count must be a positive integer.\n");
        exit(1);
    }
  }

  printf("rand(%d) numbers:\n", count);
  for(int i = 0; i < count; i++) {
    printf("%d\n", myrand());
  }

  exit(0);
}
