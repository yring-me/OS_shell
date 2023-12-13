#include "parse.h"

char args[10][4096];

struct redir_info redir_info;
struct old_fd old_fd;
void parse_buffer(char *buffer_in)
{

    char *token = 0;
    int index = 0;

    buffer_in[strlen(buffer_in) - 1] = 0;

    sprintf((char *)history[++history_total], "%s", buffer_in);

    token = strtok(buffer_in, " ");

    while (token != NULL)
    {

        sprintf((char *)args[index++], "%s", token);

        token = strtok(NULL, " ");
    }
}

// 用于命令分发
int dispatcher()
{
    int id = -1;

    for (int i = 0; i < sizeof(cmd_list) / sizeof(shell); i++)
    {
        char temp[128];

        // 找到调用号
        if (strcmp((char *)args[0], cmd_list[i].name) == 0)
        {
            id = cmd_list[i].id;
            break;
        }
    }

    if (id == -1)
    {
        printf("\x1b[31mcommand not found: %s\x1b[0m\n", args[0]);
        return -1;
    }

    // 根据调用号，找到函数调用
    syscall_handler_t handler = sys_table[id];
    if (handler(args[1], args[2], args[3], args[4], args[5]) == -1)
    {
        memset(args, 0, sizeof(args));
        return -1;
    }
    memset(args, 0, sizeof(args));
    clean_buffer();
    return 1;
}

// 检查命令是否存在
int check_cmd()
{
    int id = -1;

    for (int i = 0; i < sizeof(cmd_list) / sizeof(shell); i++)
    {
        char temp[128];

        if (strcmp((char *)args[0], cmd_list[i].name) == 0)
        {
            id = cmd_list[i].id;
            break;
        }
    }

    if (id == -1)
    {
        printf("\x1b[31mcommand not found: %s\x1b[0m\n", args[0]);
        return -1;
    }

    return 0;
}
