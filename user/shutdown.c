#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char const *argv[])
{
  printf("Shutting Down...\n");
  shutdown();
  printf("Shutdown Failed\n");  // Should not reach here
  exit(0);
}
