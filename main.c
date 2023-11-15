#include <stdio.h>
#include <string.h>
#include <stdlib.h>
extern char history[100][128];
extern int history_total;
extern void parse_buffer(char *);
extern void syscall_history();
extern void dispatcher();
int main()
{
    int size = 1024;
    char *buffer = (char *)malloc(size);

    // 目前先整三个命令
    for (int i = 0; i < 3; i++)
    {
        memset(buffer, 0, 1024);
        printf(">>>");
        fgets(buffer, size, stdin);

        parse_buffer(buffer);
        dispatcher();
    }
}