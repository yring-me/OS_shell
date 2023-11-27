#include "head.h"

#define DEFAULT_LINES 10

struct head_info {
    char file[256];
    int n;
};

struct head_info head_info;

void head_info_init() {
    memset(head_info.file, 0, sizeof(head_info.file));
    head_info.n = 10;
}

void head_info_check(char *args0, char *args1) {
    if (args0[0] != '-') {
        strcpy(head_info.file, args0);
        if (args1 != NULL) {
            head_info.n = atoi(args1);
            }
        else{
            if (args1==NULL)
                head_info.n=10;
        }
    } else {
        if (args1 != NULL) {
            strcpy(head_info.file, args1);
        }
        head_info.n = atoi(args0 + 1);
    }
}

int head() {
    FILE *file;
    char line[1000];
    int count = 0;

    file = fopen(head_info.file, "r");
    if (file == NULL) {
        perror(head_info.file);
        printf("can't open file");
        return -1;
    }
    int num=0;
    if(head_info.n<4)
        num=4;
    else 
        num=head_info.n;

    while (fgets(line, sizeof(line), file) != NULL && count < num) {
        printf("%s", line);
        count++;
    }

    fclose(file);
    return 0;
}

void syscall_head(char *args0, char *args1) {
    head_info_init();
    head_info_check(args0, args1);
    
    if (head() == -1) {
        return;
    }
    return;
}