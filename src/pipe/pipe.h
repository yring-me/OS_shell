#ifndef PIPE_H
#define PIPE_H
#include "../parse/parse.h"
#include "../../include/main.h"
#include "../../include/shell.h"

struct pipe_info // 管道信息
{
    int is_pipe;              // 是否有管道
    char temp_file_name[128]; // 临时文件名字
    char next_cmd[10][256];   // 后一个命令及其参数
};

int parse_pipe();
void pipe_info_reset();
void pipe_info_init();
#endif