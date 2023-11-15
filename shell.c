#include <stdio.h>
#include <stdlib.h>
#include "shell.h"
extern char history[100][128];
extern int history_total;

void syscall_history()
{
    int index = -1;
    // printf("%d,%s\n", history_total, (char *)history[history_total]);
    if (history_total == -1)
    {
        printf("\n");
        return;
    }
    for (index = 0; index < history_total + 1; index++)
    {
        printf("%s\n", (char *)history[index]);
    }
}
