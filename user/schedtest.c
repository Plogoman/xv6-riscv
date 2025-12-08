#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"

#define POLICY_RR       0
#define POLICY_FCFS     1
#define POLICY_PRIORITY 2

void burn_cpu(int intensity) {
  int z;
  char src[1024], dst[1024];

  for (z = 0; z < intensity; z++) {
     memmove(dst, src, 1024);
     memmove(src, dst, 1024);
  }
}

void test_fcfs() {
  printf("\n--- TESTING FCFS (Policy 1) ---\n");
  printf("Goal: Processes should finish in the EXACT order of creation (PID order).\n");

  if (set_sched_policy(POLICY_FCFS) < 0) {
      printf("Error: syscall set_sched_policy failed!\n");
      exit(1);
  }

  int n = 5;
  int i;

  for(i = 0; i < n; i++) {
    sleep(1);
    int pid = fork();
    if(pid == 0) {
      printf("[FCFS] Child %d (PID %d) created. Working...\n", i, getpid());
      burn_cpu(200000); // Moderate work
      printf("[FCFS] Child %d (PID %d) FINISHED.\n", i, getpid());
      exit(0);
    }
  }

  printf("[FCFS] Parent waiting...\n");
  for(i = 0; i < n; i++) {
    int finished_pid = wait(0);
    printf("[FCFS] Process %d terminated.\n", finished_pid);
  }
}

void test_priority() {
  printf("\n--- TESTING PRIORITY (Policy 2) ---\n");
  printf("Goal: High Priority (Low Value) processes should finish BEFORE Low Priority ones.\n");

  if (set_sched_policy(POLICY_PRIORITY) < 0) {
      printf("Error: syscall set_sched_policy failed!\n");
      exit(1);
  }

  int pid1, pid2, pid3;

  pid1 = fork();
  if (pid1 == 0) {
      set_priority(getpid(), 20); // Low Priority
      printf("[PRIO] Child (PID %d) started with PRIORITY 20 (Low).\n", getpid());
      burn_cpu(500000);
      printf("[PRIO] Child (PID %d) [Prio 20] FINISHED.\n", getpid());
      exit(0);
  }

  pid2 = fork();
  if (pid2 == 0) {
      set_priority(getpid(), 10); // Medium Priority
      printf("[PRIO] Child (PID %d) started with PRIORITY 10 (Med).\n", getpid());
      burn_cpu(500000);
      printf("[PRIO] Child (PID %d) [Prio 10] FINISHED.\n", getpid());
      exit(0);
  }

  pid3 = fork();
  if (pid3 == 0) {
      set_priority(getpid(), 5); // High Priority
      printf("[PRIO] Child (PID %d) started with PRIORITY 5 (High).\n", getpid());
      burn_cpu(500000);
      printf("[PRIO] Child (PID %d) [Prio 5] FINISHED.\n", getpid());
      exit(0);
  }

  wait(0);
  wait(0);
  wait(0);
}

void test_rr() {
  printf("\n--- TESTING ROUND ROBIN (Policy 0) ---\n");
  printf("Goal: Processes should share CPU (Wait times should be balanced).\n");

  set_sched_policy(POLICY_RR);

  int i;
  for(i=0; i<3; i++) {
    if(fork() == 0) {
      printf("[RR] Child %d (PID %d) running...\n", i, getpid());
      burn_cpu(300000);
      exit(0);
    }
  }

  for(i=0; i<3; i++) wait(0);
}

int main(int argc, char *argv[]) {
  printf("Starting Scheduling Validation Tests...\n");

  test_rr();
  test_fcfs();
  test_priority();

  set_sched_policy(POLICY_RR);

  printf("\nAll tests completed.\n");
  exit(0);
}
