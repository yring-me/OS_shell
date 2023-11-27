#include "tail.h"

#define DEFAULT_LINES 10

struct tail_info {
    char file[256];
    int n;
};

struct tail_info tail_info;

void tail_info_init() {
    memset(tail_info.file, 0, sizeof(tail_info.file));
    tail_info.n = DEFAULT_LINES;
}

void tail_info_check(char *args0, char *args1) {
    if (args0[0] != '-') {
        strcpy(tail_info.file, args0);
        if (args1 != NULL) {
            tail_info.n = atoi(args1);
        }
    } else {
        if (args1 != NULL) {
            strcpy(tail_info.file, args1);
        }
        tail_info.n = atoi(args0 + 1);
    }
}

int tail() {
    FILE *file;
    char line[1000];
    int count = 0;
    int pos;
    int num = tail_info.n;

    if ((file = fopen(tail_info.file, "r")) == NULL) {
        perror(tail_info.file);
        printf("can't open file");
        return -1;
    }

    fseek(file, 0, SEEK_END);
    pos = ftell(file);
    if (tail_info.n<4)
        num=4;

    while (pos) {
        fseek(file, --pos, SEEK_SET);
        if (fgetc(file) == '\n' && count++ == num -1)
            break;
    }

    while (fgets(line, sizeof(line), file) != NULL) {
        printf("%s", line);
    }

    fclose(file);
    return 0;
}

void syscall_tail(char *args0, char *args1) {
    tail_info_init();
    tail_info_check(args0, args1);
    
    if (tail() == -1) {
        return;
    }
    return;
}