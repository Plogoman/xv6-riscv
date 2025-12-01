#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char const *argv[])
{
  if (argc == 2 && strcmp(argv[1], "?") == 0) {
    printf("Usage: sleep <ticks>\nPauses execution for a specified number of system ticks.\n");
    exit(0);
  }

  if (argc != 2) {
    fprintf(2, "Usage: sleep <ticks>\n(Use 'sleep ?' for help)\n");
    exit(1);
  }

  int sleeptime = atoi(argv[1]);
  if (sleeptime <= 0) {
    fprintf(2, "sleep: ticks must be a positive integer.\n");
    exit(1);
  }

  sleep(sleeptime);

  exit(0);
}
