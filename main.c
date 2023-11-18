#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "main.h"
int main()
{
    int size = 1024;
    char *buffer = (char *)malloc(size); // 用于命令输入
    redir_init();                        // 重定向信息初始化
    ls_info_init();                      // ls信息初始化
    set_promat();                        // 设置shell格式
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

        parse_buffer(buffer); // 对输入分析

        if (check_cmd() == -1) // 检查命令是否合法
            continue;

        parse_redir(); // 检查是否有重定向标志

        dispatcher(); // 命令分发

        clean_buffer(); // 清空缓冲区

        printf("\n");
    }
}