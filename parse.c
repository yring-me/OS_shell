#include <stdio.h>
#include <string.h>
#include "shell.h"

char args[10][128] = {0};
char history[100][128] = {0};
int history_total = -1;

void dispatcher();

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

void dispatcher()
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
        printf("\x1b[31mcmd not found!\x1b[0m\n");
        return;
    }

    syscall_handler_t handler = sys_table[id];
    handler(args[1], args[2], args[3], args[4]);
    memset(args, 0, sizeof(args));
}

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
        printf("\x1b[31mcmd not found!\x1b[0m\n");
        return -1;
    }

    return 0;
}

void parse_redir()
{
    for (int i = 0; i < 9; i++)
    {

        if (strcmp(args[i], ">>") == 0 && redir_info.is_output_redir == 0)
        {
            redir_info.is_output_redir = 2;
            redir_info.out_flag = O_RDWR | O_CREAT | O_APPEND;
            sprintf(redir_info.out_file_name, "%s", args[i + 1]);

            int fd;
            if ((fd = open(redir_info.out_backup_name, O_RDWR | O_CREAT | O_APPEND, 0777)) == -1)
            {
                // cout << "open failed !" << endl;
                printf("\x1b[31m%d[\x1b[0m", errno);
                return;
            }
            redir_info.out_fd = fd;

            memset(args[i], 0, sizeof(args[i]));
            memset(args[i + 1], 0, sizeof(args[i + 1]));

            dup2(fd, 1);
        }

        if (strcmp(args[i], ">") == 0 && redir_info.is_output_redir == 0)
        {
            redir_info.is_output_redir = 1;
            redir_info.out_flag = O_RDWR | O_CREAT | O_TRUNC;
            sprintf(redir_info.out_file_name, "%s", args[i + 1]);

            int fd;
            if ((fd = open(redir_info.out_backup_name, O_RDWR | O_CREAT | O_TRUNC, 0777)) == -1) // 打开并清空外部文件
            {
                // cout << "open failed !" << endl;
                printf("\x1b[31m%d[\x1b[0m", errno);
                return;
            }
            memset(args[i], 0, sizeof(args[i]));
            memset(args[i + 1], 0, sizeof(args[i + 1]));
            dup2(fd, 1);
        }

        if (strcmp(args[i], "<") == 0 && redir_info.is_input_redir == 0)
        {
            redir_info.is_input_redir = 1;
            sprintf(redir_info.in_file_name, "%s", args[i + 1]);
            FILE *src = fopen(args[i + 1], "r");
            char buffer[256] = {0};

            fread(buffer, sizeof(buffer), 1, src);
            puts(buffer);
        }
    }
}

void redir_init()
{
    memset(redir_info.in_file_name, 0, sizeof(redir_info.in_file_name));
    memset(redir_info.out_file_name, 0, sizeof(redir_info.out_file_name));
    sprintf(redir_info.out_backup_name, "%s", "used_for_>>_redir");
    redir_info.is_input_redir = 0;
    redir_info.is_output_redir = 0;
    redir_info.out_flag = 0;

    old_fd.stdin_fd = dup(0);
    old_fd.stdout_fd = dup(1);
    old_fd.sterr_fd = dup(2);
}

void fd_reset()
{
    fflush(stdout);
    dup2(old_fd.stdin_fd, 0);
    dup2(old_fd.stdout_fd, 1);
    dup2(old_fd.sterr_fd, 2);
    fflush(stdout);
}

void redir_info_reset()
{
    memset(redir_info.in_file_name, 0, sizeof(redir_info.in_file_name));
    memset(redir_info.out_file_name, 0, sizeof(redir_info.out_file_name));
    redir_info.is_input_redir = 0;
    redir_info.is_output_redir = 0;
    redir_info.out_flag = 0;
    remove(redir_info.out_backup_name);
}

void clean_buffer()
{

    if (redir_info.out_flag == 0)
        return;
    fd_reset();

    FILE *src_r = fopen(redir_info.out_backup_name, "r");

    char buffer[4096] = {0};
    int buffer_index = 0;

    while (1 && buffer_index < 4096)
    {
        char ch = fgetc(src_r);

        if (ch == EOF) // EOF 本来表示文件末尾，意味着读取结束
            break;     // 到文件末尾则退出while循环
        if (ch == '\x1b')
        {
            fseek(src_r, 2, SEEK_CUR);
            // printf("%c", fgetc(src));
            // exit(0);
            if (fgetc(src_r) == 'm')
                continue;
            else
                fseek(src_r, 1, SEEK_CUR);
            continue;
        }
        buffer[buffer_index++] = ch;
    }

    FILE *src_w;
    if (redir_info.out_flag == (O_RDWR | O_CREAT | O_APPEND))
        src_w = fopen(redir_info.out_file_name, "a+");
    else
        src_w = fopen(redir_info.out_file_name, "w+");
    fwrite(buffer, strlen(buffer), 1, src_w);
    fclose(src_r);
    fclose(src_w);
    redir_info_reset();
}
