#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char const *argv[])
{
  struct datetime dt;

  if(datetime(&dt) == 0) {
    printf("%d-%d-%d %d:%d:%d\n",
           dt.year, dt.month, dt.day,
           dt.hour, dt.minute, dt.second);
  } else {
    printf("datetime failed\n");
  }

  exit(0);
}
