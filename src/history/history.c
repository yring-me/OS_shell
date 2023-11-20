#include "history.h"

char history[100][128] = {0};
int history_total = -1;
// 历史记录信息
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
        printf("%d %s\n", index, (char *)history[index]);
    }
}