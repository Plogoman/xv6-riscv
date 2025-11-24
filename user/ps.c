#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char const *argv[])
{
  struct proc_info pinfo[64];
  int current_pid = getpid();

  int count = getptable(64, pinfo);
  if (count > 0) {
    printf("PID    PPID   STATE     SIZE      NAME\n");

    for (int i = 0; i < count; i++) {
      if (pinfo[i].pid == 0) break;

      // Mark current process
      if(pinfo[i].pid == current_pid) {
        printf("*** ");
      } else {
        printf("    ");
      }

      // Convert state to string
      char *state;
      switch (pinfo[i].state) {
        case 0: state = "UNUSED  "; break;
        case 1: state = "USED    "; break;
        case 2: state = "SLEEPING"; break;
        case 3: state = "RUNNABLE"; break;
        case 4: state = "RUNNING "; break;
        case 5: state = "ZOMBIE  "; break;
        default: state = "UNKNOWN "; break;
      }

      // Print with proper spacing
      printf("%d      %d      %s %d      %s\n",
             pinfo[i].pid,
             pinfo[i].ppid,
             state,
             (int)pinfo[i].sz,
             pinfo[i].name);
    }
  } else {
    printf("getptable failed\n");
  }

  exit(0);
}
