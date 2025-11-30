#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(void) {
    printf("Testing countsyscall system call\n");
    printf("================================\n\n");

    // Make some getpid() calls (syscall number 11)
    printf("Calling getpid() 5 times...\n");
    for(int i = 0; i < 5; i++) {
        getpid();
    }

    // Check count for getpid
    int count = countsyscall(11);
    printf("getpid was called: %d times\n\n", count);

    // Make some write calls (syscall number 16)
    printf("Making 3 write calls...\n");
    write(1, "Call 1\n", 7);
    write(1, "Call 2\n", 7);
    write(1, "Call 3\n", 7);

    count = countsyscall(16);
    printf("write was called: %d times\n\n", count);

    // Test fork (syscall number 1)
    printf("Forking process...\n");
    int pid = fork();
    if(pid == 0) {
        // Child process
        count = countsyscall(1);
        printf("Child: fork was called %d times\n", count);
        exit(0);
    } else {
        // Parent process
        wait(0);
        count = countsyscall(1);
        printf("Parent: fork was called %d times\n", count);
    }

    printf("\nTest completed!\n");
    exit(0);
}
