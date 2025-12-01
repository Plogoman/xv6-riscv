#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char const *argv[])
{
  if (argc == 2 && strcmp(argv[1], "?") == 0) {
    printf("Usage: fact <number>\nComputes the factorial of a non-negative integer.\n");
    exit(0);
  }

  if (argc != 2) {
    fprintf(2, "Usage: fact <number>\n(Use 'fact ?' for help)\n");
    exit(1);
  }

  int num = atoi(argv[1]);
  if (num < 0) {
    fprintf(2, "fact: factorial is not defined for negative numbers\n");
    exit(1);
  }

  unsigned long long fact = 1;
  for (int i = 1; i <= num; i++)
  {
    fact *= i;
  }
  printf("The factorial of %d is %l\n", num, fact);

  exit(0);
}
