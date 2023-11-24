#include "pipe.h"

struct pipe_info pipe_info;

void pipe_info_init()
{
    pipe_info.is_pipe = 0;
    memset(pipe_info.next_cmd, 0, sizeof(pipe_info.next_cmd));
    sprintf(pipe_info.temp_file_name, "%s", "/tmp/used_for_pipe");
}

void pipe_info_reset()
{
    pipe_info.is_pipe = 0;
    memset(pipe_info.next_cmd, 0, sizeof(pipe_info.next_cmd));
    remove(pipe_info.temp_file_name);
}

int parse_pipe()
{
    pipe_info_init();
    for (int i = 0; i < 9; i++)
    {
        if (strcmp(args[i], "|") == 0 && pipe_info.is_pipe == 0)
        {
            pipe_info.is_pipe = 1;
            int pos = i;
            // “|"后的参数应当是第二个命令
            for (int j = 0; j < 9; j++)
            {
                sprintf(pipe_info.next_cmd[j], "%s", args[pos + j + 1]);
                memset(args[pos + j], 0, sizeof(args[pos + j]));
            }

            redir_init();
            int fd;
            // 打开相应文件
            if ((fd = open(pipe_info.temp_file_name, O_RDWR | O_CREAT | O_TRUNC, 0777)) == -1)
            {
                // cout << "open failed !" << endl;
                printf("\x1b[31m%d[\x1b[0m", errno);
                return -1;
            }
            redir_info.out_fd = fd;

            // 将输出重定向到文件
            dup2(fd, 1);
            if (dispatcher() == -1)
                return -1;
            fd_reset();

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
            if (dispatcher() == -1)
                return -1;

            fclose(src_r);
            remove(pipe_info.temp_file_name);
            return 1;
        }
    }

    return 0;
}
