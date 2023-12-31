#include "./include/main.h"

int main()
{
    int size = 1024;
    char *buffer = (char *)malloc(size); // 用于命令输入

    set_promat(); // 设置shell格式

    while (1)
    {
        memset(buffer, 0, 1024);
        printf(" \x1b[36m  \x1b[0m");    // 图标，可去掉
        printf("\x1b[36m%s\x1b[0m\n", promat); // 固定格式，为当前目录
        printf("\x1b[36m>>>\x1b[0m");          // 固定格式

        fgets(buffer, size, stdin); // 输入
        if (buffer[0] == '\n')      // 只输入回车
        {
            printf("\n");
            continue;
        }

        parse_buffer(buffer); // 对输入分词

        int back_ret = background();
        if (back_ret == -1 || back_ret == 1) // 返回1或-1 说明命令执行完成，继续下一轮；否则返回0，说明没有此标志，继续往下执行
            continue;

        int pipe_ret = parse_pipe();
        if (pipe_ret == -1 || pipe_ret == 1) // 返回1或-1 说明命令执行完成，继续下一轮；否则返回0，说明没有此标志，继续往下执行
            continue;

        if (check_cmd() == -1) // 检查命令是否合法
            continue;

        if (parse_redir() == -1)
            continue; // 检查是否有重定向标志
        dispatcher(); // 命令分发

        printf("\n");
    }
}