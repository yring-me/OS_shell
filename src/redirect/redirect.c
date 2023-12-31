#include "redirect.h"

// 检测命令中是否有重定向符号
int parse_redir()
{
    redir_init();
    for (int i = 0; i < 9; i++)
    {

        // 如果是“>>"型，则重定向方式为追加，打开文件是 “a+”
        if (strcmp(args[i], ">>") == 0 && redir_info.is_output_redir == 0)
        {
            redir_info.is_output_redir = 2;
            redir_info.out_flag = O_RDWR | O_CREAT | O_APPEND;
            // “>>"后的参数应当是重定向的文件，保存起来
            sprintf(redir_info.out_file_name, "%s", args[i + 1]);

            int fd;
            // 打开相应文件
            if ((fd = open(redir_info.out_backup_name, O_RDWR | O_CREAT | O_APPEND, 0777)) == -1)
            {
                // cout << "open failed !" << endl;
                printf("\x1b[31mfailed to open the file:%s[\x1b[0m", redir_info.out_backup_name);
                return -1;
            }
            redir_info.out_fd = fd;

            memset(args[i], 0, sizeof(args[i]));
            memset(args[i + 1], 0, sizeof(args[i + 1]));

            // 将输出重定向到文件
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
                printf("\x1b[31mfailed to open the file:%s[\x1b[0m", redir_info.out_backup_name);
                return -1;
            }
            memset(args[i], 0, sizeof(args[i]));
            memset(args[i + 1], 0, sizeof(args[i + 1]));
            dup2(fd, 1);
        }
    }
    for (int i = 0; i < 9; i++)
    {
        if (strcmp(args[i], "<") == 0 && redir_info.is_input_redir == 0)
        {
            redir_info.is_input_redir = 1;
            sprintf(redir_info.in_file_name, "%s", args[i + 1]);
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

            // puts(buffer);
        }
    }
    return 0;
}

// 初始化重定向相关信息
void redir_init()
{
    memset(redir_info.in_file_name, 0, sizeof(redir_info.in_file_name));   // 输入重定向初始化
    memset(redir_info.out_file_name, 0, sizeof(redir_info.out_file_name)); // 输出重定义初始化
    sprintf(redir_info.out_backup_name, "%s", "/tmp/used_for_>>_redir");   // 用于临时保存信息，需要对转义字符处理
    redir_info.is_input_redir = 0;                                         // 是否重定向输入
    redir_info.is_output_redir = 0;                                        // 是否重定向输出
    redir_info.out_flag = 0;                                               // 重定向输出方式

    // 保存默认的文件描述符
    old_fd.stdin_fd = dup(0);
    old_fd.stdout_fd = dup(1);
    old_fd.sterr_fd = dup(2);
}

// 重置文件描述符
void fd_reset()
{
    fflush(stdout); // 需要刷新缓冲区，把缓冲区内的内容输出出去
    dup2(old_fd.stdin_fd, 0);
    dup2(old_fd.stdout_fd, 1);
    dup2(old_fd.sterr_fd, 2);
    fflush(stdout);
}

// 重置重定向信息
void redir_info_reset()
{
    memset(redir_info.in_file_name, 0, sizeof(redir_info.in_file_name));
    memset(redir_info.out_file_name, 0, sizeof(redir_info.out_file_name));
    redir_info.is_input_redir = 0;
    redir_info.is_output_redir = 0;
    redir_info.out_flag = 0;
    remove(redir_info.out_backup_name); // 删除临时文件
}

// 用于处理缓冲区内的转义字符，只有在重定向的时候会用到
void clean_buffer()
{

    if (redir_info.out_flag == 0)
        return;
    fd_reset();

    FILE *src_r = fopen(redir_info.out_backup_name, "r"); // 打开临时文件

    FILE *src_w = NULL;
    // 根据重定向方式打开文件
    if (redir_info.out_flag == (O_RDWR | O_CREAT | O_APPEND))
        src_w = fopen(redir_info.out_file_name, "a+");
    else
        src_w = fopen(redir_info.out_file_name, "w+");

    if (src_w == NULL)
    {
        printf("\x1b[31mfailed to open the file:%s\x1b[0m\n", redir_info.out_file_name);

        fclose(src_r);
        // 重置信息
        redir_info_reset();
        return;
    }
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
        fputc(ch, src_w);
    }

    fclose(src_r);
    fclose(src_w);
    // 重置信息
    redir_info_reset();
}
