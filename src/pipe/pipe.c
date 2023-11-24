#include "pipe.h"

struct pipe_info pipe_info;

int parse_pipe()
{
    pipe_info_init();
    for (int i = 0; i < 9; i++)
    {
        if (strcmp(args[i], "<") == 0 && pipe_info.is_in_redir == 0)
        {
            pipe_info.is_in_redir = 1;
            sprintf(pipe_info.temp_in_file_name, "%s", args[i + 1]);
            FILE *src = fopen(args[i + 1], "r");
            if (src == NULL)
            {
                printf("%sInvalid file:%s%s\n", COLOR_RED, args[i + 1], COLOR_RESET);
                return -1;
            }
            char buffer[4096] = {0};

            fread(buffer, sizeof(buffer), 1, src);
            memset(args[i], 0, sizeof(args[i]));
            memset(args[i + 1], 0, sizeof(args[i + 1]));
            memcpy(args[i], buffer, strlen(buffer)); // 直接替换参数即可
            fclose(src);
            // puts(buffer);
        }

        if (strcmp(args[i], ">>") == 0 && redir_info.is_output_redir == 0)
        {
            pipe_info.is_out_redir = 2;
            pipe_info.out_index = i;
            sprintf(pipe_info.temp_out_file_name, "%s", args[i + 1]);
            memset(args[i], 0, sizeof(args[i]));
            memset(args[i + 1], 0, sizeof(args[i + 1]));
        }

        if (strcmp(args[i], ">") == 0 && redir_info.is_output_redir == 0)
        {
            pipe_info.is_out_redir = 1;
            pipe_info.out_index = i;
            sprintf(pipe_info.temp_out_file_name, "%s", args[i + 1]);
            memset(args[i], 0, sizeof(args[i]));
            memset(args[i + 1], 0, sizeof(args[i + 1]));
        }
    }

    for (int i = 0; i < 9; i++)
    {
        if (strcmp(args[i], "|") == 0 && pipe_info.is_pipe == 0)
        {

            pipe_info.is_pipe = 1;
            int pos = i;
            // “|"后的参数应当是第二个命令
            for (int j = 0; j < 5; j++)
            {
                sprintf(pipe_info.next_cmd[j], "%s", args[pos + j + 1]);
                memset(args[pos + j], 0, sizeof(args[pos + j]));
            }

            pipe_redir();

            if (dispatcher() == -1)
                return -1;

            pipe_reset(pos);
            if (dispatcher() == -1)
                return -1;

            pipe_info_reset();
            pipe_fd_reset();
            printf("\n");
            return 1;
        }
    }

    return 0;
}

void pipe_info_init()
{
    pipe_info.is_pipe = 0;
    pipe_info.is_in_redir = 0;
    pipe_info.is_out_redir = 0;
    pipe_info.out_index = 0;
    pipe_info.old_stdin_fd = dup(0);
    pipe_info.old_stdout_fd = dup(1);
    pipe_info.old_stderr_fd = dup(2);

    memset(pipe_info.next_cmd, 0, sizeof(pipe_info.next_cmd));
    sprintf(pipe_info.temp_file_name, "%s", "/tmp/used_for_pipe");
    sprintf(pipe_info.temp_in_file_name, "%s", "/tmp/used_for_in_pipe");
    sprintf(pipe_info.temp_out_file_name, "%s", "/tmp/used_for_out_pipe");
}

void pipe_info_reset()
{
    pipe_info.is_pipe = 0;
    pipe_info.is_in_redir = 0;
    pipe_info.is_out_redir = 0;
    pipe_info.out_index = 0;
    memset(pipe_info.next_cmd, 0, sizeof(pipe_info.next_cmd));
    remove(pipe_info.temp_file_name);
}

void pipe_fd_reset()
{
    fflush(stdout); // 需要刷新缓冲区，把缓冲区内的内容输出出去
    dup2(pipe_info.old_stdin_fd, 0);
    dup2(pipe_info.old_stdout_fd, 1);
    dup2(pipe_info.old_stderr_fd, 2);
    fflush(stdout);
}

void pipe_redir()
{
    int fd;
    // 打开相应文件
    if ((fd = open(pipe_info.temp_file_name, O_RDWR | O_CREAT | O_TRUNC, 0777)) == -1)
    {
        // cout << "open failed !" << endl;
        printf("\x1b[31m%d\x1b[0m\n", errno);
    }

    // 将输出重定向到文件
    dup2(fd, 1);
}

void pipe_reset(int pos)
{
    pipe_fd_reset();

    for (int j = 0; j < 10; j++)
    {
        if (strlen(pipe_info.next_cmd[j]) == 0) // 优先保留原来的参数
        {
            pos = j;
            break;
        }
        sprintf(args[j], "%s", pipe_info.next_cmd[j]);
    }

    FILE *src_r = fopen(pipe_info.temp_file_name, "r"); // 打开临时文件
    int index = 0;
    while (1)
    {
        char ch = fgetc(src_r);

        if (ch == EOF) // EOF 本来表示文件末尾，意味着读取结束
            break;     // 到文件末尾则退出while循环

        // 这个if用于处理转义信息
        if (ch == '\x1b')
        {
            fseek(src_r, 2, SEEK_CUR);
            if (fgetc(src_r) == 'm')
                continue;
            else
                fseek(src_r, 1, SEEK_CUR);
            continue;
        }
        args[pos][index++] = ch;
    }
    args[pos][index - 1] = 0;
    fclose(src_r);
    pipe_fd_reset();
    if (pipe_info.is_out_redir == 1)
    {
        int fd;
        if ((fd = open(pipe_info.temp_out_file_name, O_RDWR | O_CREAT | O_TRUNC, 0777)) == -1)
        {
            // cout << "open failed !" << endl;
            printf("\x1b[31m%d\x1b[0m\n", errno);
            return;
        }
        memset(args[pipe_info.out_index], 0, sizeof(args[pipe_info.out_index]));
        memset(args[pipe_info.out_index + 1], 0, sizeof(args[pipe_info.out_index + 1]));
        // 将输出重定向到文件
        dup2(fd, 1);
    }

    if (pipe_info.is_out_redir == 2)
    {
        int fd;
        if ((fd = open(pipe_info.temp_out_file_name, O_RDWR | O_CREAT | O_APPEND, 0777)) == -1)
        {
            // cout << "open failed !" << endl;
            printf("\x1b[31m%d[\x1b[0m", errno);
            return;
        }

        memset(args[pipe_info.out_index], 0, sizeof(args[pipe_info.out_index]));
        memset(args[pipe_info.out_index + 1], 0, sizeof(args[pipe_info.out_index + 1]));
        // 将输出重定向到文件
        dup2(fd, 1);
    }
}