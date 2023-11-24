#ifndef PIPE_H
#define PIPE_H
#include "../parse/parse.h"
#include "../../include/main.h"
#include "../../include/shell.h"

struct pipe_info // 管道信息
{
    int is_pipe;              // 是否有管道
    char temp_file_name[128]; // 临时文件名字
    char next_cmd[5][4096];   // 后一个命令及其参数

    int is_in_redir;  // 是否输入重定向
    int is_out_redir; // 是否输出重定向
    int out_index;
    char temp_in_file_name[128];
    char temp_out_file_name[128];

    int old_stdin_fd;
    int old_stdout_fd;
    int old_stderr_fd;
};

int parse_pipe();
void pipe_info_reset();
void pipe_info_init();
void pipe_redir();
void pipe_reset(int pos);
void pipe_fd_reset();
#endif