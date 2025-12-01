#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char const *argv[])
{
  if (argc == 2 && strcmp(argv[1], "?") == 0) {
    printf("Usage: kbdint\nPrints the number of keyboard interrupts since boot.\n");
    exit(0);
  }

  if (argc != 1) {
    fprintf(2, "Usage: kbdint\n(Use 'kbdint ?' for help)\n");
    exit(1);
  }

  int count = kbdint();
  if (count >= 0) {
    printf("Keyboard Interrupts since boot: %d\n", count);
  } else {
    fprintf(2, "kbdint: syscall failed\n");
    exit(1);
  }

  exit(0);
}
