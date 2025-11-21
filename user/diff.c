#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"

int main(int argc, char *argv[]) {
    int fd1, fd2, n1, n2;
    char buf1[512], buf2[512];

    if(argc != 3){
        fprintf(2, "Usage: diff <file1> <file2>\n");
        exit(1);
    }

    if((fd1 = open(argv[1], O_RDONLY)) < 0){
        fprintf(2, "diff: cannot open %s\n", argv[1]);
        exit(1);
    }

    if((fd2 = open(argv[2], O_RDONLY)) < 0){
        fprintf(2, "diff: cannot open %s\n", argv[2]);
        close(fd1);
        exit(1);
    }

    while(1){
        n1 = read(fd1, buf1, sizeof(buf1));
        n2 = read(fd2, buf2, sizeof(buf2));

        if(n1 != n2 || memcmp(buf1, buf2, n1) != 0){
            printf("Files differ\n");
            close(fd1);
            close(fd2);
            exit(0);
        }

        if(n1 == 0)
            break;
    }

    printf("Files are identical\n");
    close(fd1);
    close(fd2);
    exit(0);
}
