#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define NLINES 10
#define LINELEN 512

int main(int argc, char *argv[]) {
    if (argc == 2 && strcmp(argv[1], "?") == 0) {
        printf("Usage: tail <file>\nPrints the last 10 lines of a file.\n");
        exit(0);
    }

    if(argc != 2){
        fprintf(2, "Usage: tail <file>\n(Use 'tail ?' for help)\n");
        exit(1);
    }

    int fd = open(argv[1], 0);
    if(fd < 0){
        fprintf(2, "tail: cannot open '%s'\n", argv[1]);
        exit(1);
    }

    char *line_ptrs[NLINES];
    for(int i = 0; i < NLINES; i++){
        line_ptrs[i] = malloc(LINELEN);
        if(!line_ptrs[i]){
            fprintf(2, "tail: memory allocation failed\n");
            exit(1);
        }
        line_ptrs[i][0] = '\0';
    }

    char buf[LINELEN];
    int line_idx = 0;
    int total_lines = 0;
    int bytes_read;
    int current_pos = 0;

    while((bytes_read = read(fd, buf + current_pos, 1)) == 1) {
        if (buf[current_pos] == '\n') {
            buf[current_pos] = '\0';
            strcpy(line_ptrs[line_idx], buf);
            line_idx = (line_idx + 1) % NLINES;
            total_lines++;
            current_pos = 0;
        } else if (current_pos < LINELEN - 1) {
            current_pos++;
        } else {
            // Line too long, reset buffer and continue to next line
            while((bytes_read = read(fd, buf, 1)) == 1 && buf[0] != '\n');
            current_pos = 0;
        }
    }

    if (current_pos > 0) { // Handle file not ending with newline
        buf[current_pos] = '\0';
        strcpy(line_ptrs[line_idx], buf);
        total_lines++;
    }

    close(fd);

    int start_line = total_lines > NLINES ? total_lines % NLINES : 0;
    int num_to_print = total_lines > NLINES ? NLINES : total_lines;

    for (int i = 0; i < num_to_print; i++) {
        printf("%s\n", line_ptrs[(start_line + i) % NLINES]);
    }

    for(int i = 0; i < NLINES; i++) {
        free(line_ptrs[i]);
    }

    exit(0);
}
