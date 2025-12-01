#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char const *argv[])
{
  if (argc == 2 && strcmp(argv[1], "?") == 0) {
    printf("Usage: shutdown\nShuts down the xv6 operating system.\n");
    exit(0);
  }

  if (argc != 1) {
    fprintf(2, "Usage: shutdown\n(Use 'shutdown ?' for help)\n");
    exit(1);
  }

  printf("Shutting Down...\n");
  shutdown();

  // This part should not be reached if shutdown is successful
  fprintf(2, "Shutdown Failed\n");
  exit(1);
}
