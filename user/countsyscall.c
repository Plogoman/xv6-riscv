#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    if (argc == 2 && strcmp(argv[1], "?") == 0) {
        printf("Usage: countsyscall\nRuns a test suite for the countsyscall system call.\n");
        exit(0);
    }

    if (argc != 1) {
        fprintf(2, "Usage: countsyscall\n(Use 'countsyscall ?' for help)\n");
        exit(1);
    }

    printf("Testing countsyscall system call\n");
    printf("================================\n\n");

    // Test getpid (syscall 11)
    printf("Calling getpid() 5 times...\n");
    for(int i = 0; i < 5; i++) {
        getpid();
    }
    int count = countsyscall(11);
    printf("getpid was called: %d times\n\n", count);

    // Test write (syscall 16)
    printf("Making 3 write calls...\n");
    write(1, "Call 1\n", 7);
    write(1, "Call 2\n", 7);
    write(1, "Call 3\n", 7);
    count = countsyscall(16);
    // Note: The printf calls also use write, so the count will be higher than 3
    printf("write was called: %d times (includes printf calls)\n\n", count);

    // Test fork (syscall 1)
    printf("Forking process...\n");
    int initial_fork_count = countsyscall(1);
    int pid = fork();
    if(pid == 0) {
        // Child process
        count = countsyscall(1);
        printf("Child: fork count is %d\n", count);
        exit(0);
    } else {
        // Parent process
        wait(0);
        count = countsyscall(1);
        printf("Parent: fork count is %d (expected %d)\n", count, initial_fork_count + 1);
    }

    printf("\nTest completed!\n");
    exit(0);
}
