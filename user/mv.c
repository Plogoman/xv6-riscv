#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    if(argc != 3){
        fprintf(2, "Usage: mv <source> <destination>\n");
        exit(1);
    }

    if(link(argv[1], argv[2]) < 0){
        fprintf(2, "mv: link failed\n");
        exit(1);
    }

    if(unlink(argv[1]) < 0){
        fprintf(2, "mv: unlink failed\n");
        exit(1);
    }

    exit(0);
}
