#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"

#define MAX_LINES 256
#define MAX_LINE_LEN 512

typedef struct {
    char **lines;
    int count;
} FileContent;

int read_file(int fd, FileContent *content, const char *filename) {
    char buf[MAX_LINE_LEN];
    int n, i, line_idx = 0, char_idx = 0;
    char current_line[MAX_LINE_LEN];

    content->count = 0;
    content->lines = (char**)malloc(MAX_LINES * sizeof(char*));
    if(!content->lines) {
        fprintf(2, "diff: memory allocation failed\n");
        return -1;
    }

    while((n = read(fd, buf, sizeof(buf))) > 0) {
        for(i = 0; i < n; i++) {
            if(buf[i] == '\n') {
                current_line[char_idx] = '\0';
                if(line_idx >= MAX_LINES) {
                    fprintf(2, "diff: file '%s' exceeds max lines of %d\n", filename, MAX_LINES);
                    return -1;
                }
                content->lines[line_idx] = (char*)malloc(char_idx + 1);
                if(!content->lines[line_idx]) {
                     fprintf(2, "diff: memory allocation failed\n");
                     return -1;
                }
                strcpy(content->lines[line_idx], current_line);
                line_idx++;
                content->count++;
                char_idx = 0;
            } else {
                if(char_idx < MAX_LINE_LEN - 1) {
                    current_line[char_idx++] = buf[i];
                } else {
                    fprintf(2, "diff: line too long in file '%s'\n", filename);
                    return -1;
                }
            }
        }
    }

    if(char_idx > 0) {
        current_line[char_idx] = '\0';
        if(line_idx >= MAX_LINES) {
            fprintf(2, "diff: file '%s' exceeds max lines of %d\n", filename, MAX_LINES);
            return -1;
        }
        content->lines[line_idx] = (char*)malloc(char_idx + 1);
        if(!content->lines[line_idx]) {
            fprintf(2, "diff: memory allocation failed\n");
            return -1;
        }
        strcpy(content->lines[line_idx], current_line);
        content->count++;
    }

    if(n < 0) {
        fprintf(2, "diff: read error on file '%s'\n", filename);
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
    if (argc == 2 && strcmp(argv[1], "?") == 0) {
        printf("Usage: diff <file1> <file2>\nCompares two files line by line and shows the differences.\n");
        exit(0);
    }
    if(argc != 3) {
        fprintf(2, "Usage: diff <file1> <file2>\n(Use 'diff ?' for help)\n");
        exit(1);
    }

    int fd1, fd2;
    if((fd1 = open(argv[1], O_RDONLY)) < 0) {
        fprintf(2, "diff: cannot open '%s'\n", argv[1]);
        exit(1);
    }
    if((fd2 = open(argv[2], O_RDONLY)) < 0) {
        fprintf(2, "diff: cannot open '%s'\n", argv[2]);
        close(fd1);
        exit(1);
    }

    FileContent file1, file2;
    if(read_file(fd1, &file1, argv[1]) < 0) {
        close(fd1); close(fd2); exit(1);
    }
    if(read_file(fd2, &file2, argv[2]) < 0) {
        close(fd1); close(fd2); free_file(&file1); exit(1);
    }
    close(fd1); close(fd2);

    int max_lines = file1.count > file2.count ? file1.count : file2.count;
    int found_diff = 0;
    for(int i = 0; i < max_lines; i++) {
        if(i < file1.count && i < file2.count) {
            if(strcmp(file1.lines[i], file2.lines[i]) != 0) {
                found_diff = 1;
                printf("< %s\n> %s\n", file1.lines[i], file2.lines[i]);
            }
        } else if(i >= file1.count) {
            found_diff = 1;
            printf("> %s\n", file2.lines[i]);
        } else {
            found_diff = 1;
            printf("< %s\n", file1.lines[i]);
        }
    }

    if(!found_diff) {
        printf("Files are identical.\n");
    }

    free_file(&file1);
    free_file(&file2);
    exit(0);
}
