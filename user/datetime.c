#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char const *argv[])
{
  if (argc == 2 && strcmp(argv[1], "?") == 0) {
    printf("Usage: datetime\nPrints the current system date and time.\n");
    exit(0);
  }

  if (argc != 1) {
    fprintf(2, "Usage: datetime\n(Use 'datetime ?' for help)\n");
    exit(1);
  }

  struct datetime dt;
  if(datetime(&dt) == 0) {
    printf("%d-%d-%d %d:%d:%d\n",
           dt.year, dt.month, dt.day,
           dt.hour, dt.minute, dt.second);
  } else {
    fprintf(2, "datetime: failed to retrieve date and time\n");
    exit(1);
  }

  exit(0);
}
