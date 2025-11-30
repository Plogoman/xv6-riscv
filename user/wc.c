#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

#define MAX_LINE 1024

typedef struct {
    int lines;
    int words;
    int chars;
    int longest;
} WcFlags;

char buf[512];

void
wc(int fd, char *name, WcFlags flags, int show_all)
{
    int i, n;
    int l = 0, w = 0, c = 0, inword = 0;
    int current_line_len = 0, max_line_len = 0;

    while((n = read(fd, buf, sizeof(buf))) > 0) {
        for(i = 0; i < n; i++) {
            if(flags.chars || show_all) c++;

            if(buf[i] == '\n') {
                if(flags.lines || show_all) l++;
                if(flags.longest) {
                    if(current_line_len > max_line_len)
                        max_line_len = current_line_len;
                    current_line_len = 0;
                }
            } else {
                current_line_len++;
            }

            if(strchr(" \r\t\n\v", buf[i])) {
                inword = 0;
            } else if(!inword) {
                if(flags.words || show_all) w++;
                inword = 1;
            }
        }
    }

    // Handle longest line if file doesn't end with newline
    if(flags.longest && current_line_len > max_line_len)
        max_line_len = current_line_len;

    if(n < 0) {
        fprintf(2, "wc: read error\n");
        exit(1);
    }

    if(flags.lines || show_all) printf("%d ", l);
    if(flags.words || show_all) printf("%d ", w);
    if(flags.chars || show_all) printf("%d ", c);
    if(flags.longest) printf("%d ", max_line_len);
    printf("%s\n", name);
}

int
main(int argc, char *argv[])
{
    int fd, i;
    WcFlags flags;
    int show_all = 1;
    int file_start = 1;

    // Parse flags
    flags.lines = 0;
    flags.words = 0;
    flags.chars = 0;
    flags.longest = 0;

    for(i = 1; i < argc; i++) {
        if(argv[i][0] == '-') {
            show_all = 0;
            for(int j = 1; argv[i][j]; j++) {
                if(argv[i][j] == 'l') flags.lines = 1;
                else if(argv[i][j] == 'w') flags.words = 1;
                else if(argv[i][j] == 'c') flags.chars = 1;
                else if(argv[i][j] == 'L') flags.longest = 1;
            }
            file_start++;
        } else {
            break;
        }
    }

    // If no flags specified, show all
    if(show_all) {
        flags.lines = 1;
        flags.words = 1;
        flags.chars = 1;
    }

    // Handle stdin if no files
    if(file_start >= argc) {
        wc(0, "", flags, show_all);
        exit(0);
    }

    // Process files
    for(i = file_start; i < argc; i++) {
        if((fd = open(argv[i], O_RDONLY)) < 0) {
            fprintf(2, "wc: cannot open %s\n", argv[i]);
            exit(1);
        }
        wc(fd, argv[i], flags, show_all);
        close(fd);
    }
    exit(0);
}
