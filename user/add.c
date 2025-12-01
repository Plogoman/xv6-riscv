#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char const *argv[])
{
  if (argc == 2 && strcmp(argv[1], "?") == 0)
  {
    printf("Usage: add <number1> <number2>\nAdds two integer numbers and prints the result.\n");
    exit(0);
  }

  if (argc != 3)
  {
    fprintf(2, "Usage: add <number1> <number2>\n(Use 'add ?' for help)\n");
    exit(1);
  }

  int num1 = atoi(argv[1]);
  int num2 = atoi(argv[2]);

  printf("The sum of %d and %d is %d\n", num1, num2, (num1 + num2));

  exit(0);
}
