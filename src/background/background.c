#include "background.h"

int background()
{
    int flag = 0;
    for (int i = 0; i < 10; i++)
    {
        if (strcmp(args[i], "&") == 0)
        {
            memset(args[i], 0, sizeof(args[i]));
            flag = 1;
            break;
        }
    }
    if (flag == 0)
        return 0;

    for (int i = 1; i < 10; i++)
    {
        strcat(args[0], " ");
        strcat(args[0], args[i]);
    }

    pid_t pid = fork();
    if (pid == 0)
    {

        printf("\n");
        printf("%schild pid:%d working...%s\n", COLOR_YELLOW, getpid(), COLOR_RESET);

        // sleep(1000);可以加此语句来验证没有阻塞
        system((const char *)args); // 这里也可以转为调用此程序的内部shell，做一次dispatch，简单处理一下返回逻辑就ok了，不过实在不想写了：）
        printf("%schild pid:%d done%s\n", COLOR_GREEN, getpid(), COLOR_RESET);
        exit(1);
    }
    memset(args, 0, sizeof(args));
    // exit(1);
    return 1;
}