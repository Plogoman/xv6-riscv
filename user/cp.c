#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"

int main(int argc, char *argv[]) {
    int fd_src, fd_dst, n;
    char buf[512];

    if (argc == 2 && strcmp(argv[1], "?") == 0) {
        printf("Usage: cp <source> <destination>\nCopies the content of the source file to the destination file.\n");
        exit(0);
    }

    if(argc != 3){
        fprintf(2, "Usage: cp <source> <destination>\n(Use 'cp ?' for help)\n");
        exit(1);
    }

    if((fd_src = open(argv[1], O_RDONLY)) < 0){
        fprintf(2, "cp: cannot open source '%s'\n", argv[1]);
        exit(1);
    }

    if((fd_dst = open(argv[2], O_CREATE | O_WRONLY)) < 0){
        fprintf(2, "cp: cannot create destination '%s'\n", argv[2]);
        close(fd_src);
        exit(1);
    }

    while((n = read(fd_src, buf, sizeof(buf))) > 0){
        if(write(fd_dst, buf, n) != n){
            fprintf(2, "cp: write error to '%s'\n", argv[2]);
            close(fd_src);
            close(fd_dst);
            exit(1);
        }
    }

    if (n < 0) {
        fprintf(2, "cp: read error from '%s'\n", argv[1]);
    }

    close(fd_src);
    close(fd_dst);
    exit(0);
}
