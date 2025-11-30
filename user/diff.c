#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"

#define MAX_LINES 256
#define MAX_LINE 512

typedef struct {
    char **lines;
    int count;
} FileContent;

// Read entire file into memory, line by line
int read_file(int fd, FileContent *content) {
    char buf[512];
    int n, i, line_idx = 0, char_idx = 0;
    char current_line[MAX_LINE];

    content->count = 0;
    content->lines = (char**)malloc(MAX_LINES * sizeof(char*));

    if(!content->lines) {
        fprintf(2, "diff: malloc failed\n");
        return -1;
    }

    while((n = read(fd, buf, sizeof(buf))) > 0) {
        for(i = 0; i < n; i++) {
            if(buf[i] == '\n') {
                current_line[char_idx] = '\0';

                if(line_idx >= MAX_LINES) {
                    fprintf(2, "diff: too many lines\n");
                    return -1;
                }

                // Allocate memory for this line and copy it
                content->lines[line_idx] = (char*)malloc(char_idx + 1);
                if(!content->lines[line_idx]) {
                    fprintf(2, "diff: malloc failed\n");
                    return -1;
                }
                strcpy(content->lines[line_idx], current_line);

                line_idx++;
                content->count++;
                char_idx = 0;
            } else {
                if(char_idx < MAX_LINE - 1) {
                    current_line[char_idx] = buf[i];
                    char_idx++;
                }
            }
        }
    }

    // Handle last line if it doesn't end with newline
    if(char_idx > 0) {
        current_line[char_idx] = '\0';

        if(line_idx >= MAX_LINES) {
            fprintf(2, "diff: too many lines\n");
            return -1;
        }

        content->lines[line_idx] = (char*)malloc(char_idx + 1);
        if(!content->lines[line_idx]) {
            fprintf(2, "diff: malloc failed\n");
            return -1;
        }
        strcpy(content->lines[line_idx], current_line);

        line_idx++;
        content->count++;
    }

    if(n < 0) {
        fprintf(2, "diff: read error\n");
        return -1;
    }

    return 0;
}

void free_file(FileContent *content) {
    for(int i = 0; i < content->count; i++) {
        free(content->lines[i]);
    }
    free(content->lines);
}

int main(int argc, char *argv[]) {
    int fd1, fd2;
    FileContent file1, file2;
    int i, max_lines, found_diff = 0;

    if(argc != 3) {
        fprintf(2, "Usage: diff <file1> <file2>\n");
        exit(1);
    }

    if((fd1 = open(argv[1], O_RDONLY)) < 0) {
        fprintf(2, "diff: cannot open %s\n", argv[1]);
        exit(1);
    }

    if((fd2 = open(argv[2], O_RDONLY)) < 0) {
        fprintf(2, "diff: cannot open %s\n", argv[2]);
        close(fd1);
        exit(1);
    }

    // Read both files
    if(read_file(fd1, &file1) < 0) {
        close(fd1);
        close(fd2);
        exit(1);
    }

    if(read_file(fd2, &file2) < 0) {
        close(fd1);
        close(fd2);
        exit(1);
    }

    close(fd1);
    close(fd2);

    // Compare files line by line
    max_lines = file1.count > file2.count ? file1.count : file2.count;

    for(i = 0; i < max_lines; i++) {
        if(i < file1.count && i < file2.count) {
            // Both files have this line
            if(strcmp(file1.lines[i], file2.lines[i]) != 0) {
                found_diff = 1;
                printf("Line %d differs:\n", i + 1);
                printf("< %s\n", file1.lines[i]);
                printf("> %s\n", file2.lines[i]);
            }
        } else if(i >= file1.count) {
            // file1 ended, file2 has more lines
            found_diff = 1;
            printf("Line %d only in file2.txt:\n", i + 1);
            printf("> %s\n", file2.lines[i]);
        } else {
            // file2 ended, file1 has more lines
            found_diff = 1;
            printf("Line %d only in file1.txt:\n", i + 1);
            printf("< %s\n", file1.lines[i]);
        }
    }

    if(!found_diff) {
        printf("Files are identical\n");
    }

    // Free allocated memory
    free_file(&file1);
    free_file(&file2);

    exit(0);
}
