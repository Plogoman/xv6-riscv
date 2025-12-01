#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char const *argv[])
{
  if (argc == 2 && strcmp(argv[1], "?") == 0) {
    printf("Usage: uptime\nPrints the number of system ticks since boot.\n");
    exit(0);
  }

  if (argc != 1) {
    fprintf(2, "Usage: uptime\n(Use 'uptime ?' for help)\n");
    exit(1);
  }

  printf("uptime: %d ticks\n", uptime());
  exit(0);
}
