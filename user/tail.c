#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define NLINES 10
#define LINELEN 128

int main(int argc, char *argv[]) {
    char buf[LINELEN];
    char *line_ptrs[NLINES];
    int fd, i, total = 0, j = 0;
    int n;

    if(argc != 2){
        fprintf(2, "Usage: tail <file>\n");
        exit(1);
    }

    for(i = 0; i < NLINES; i++){
        line_ptrs[i] = malloc(LINELEN);
        if(!line_ptrs[i]){
            fprintf(2, "Memory allocation failed\n");
            exit(1);
        }
        line_ptrs[i][0] = 0;
    }

    fd = open(argv[1], 0);
    if(fd < 0){
        fprintf(2, "tail: cannot open %s\n", argv[1]);
        exit(1);
    }

    // Read the file line by line
    while(1){
        j = 0;
        while((n = read(fd, &buf[j], 1)) == 1 && buf[j] != '\n' && j < LINELEN-2)
            j++;
        buf[j] = 0;

        strcpy(line_ptrs[total % NLINES], buf);
        total++;

        if(n != 1) break; // End of file
    }
    close(fd);

    int start = (total >= NLINES) ? total % NLINES : 0;
    int count = (total < NLINES) ? total : NLINES;
    for(i = 0; i < count; i++){
        int idx = (start + i) % NLINES;
        printf("%s\n", line_ptrs[idx]);
    }

    // Free memory
    for(i = 0; i < NLINES; i++)
        free(line_ptrs[i]);

    exit(0);
}
