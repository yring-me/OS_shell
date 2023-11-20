#ifndef PARSE_H
#define PARSE_H

#include <stdio.h>
#include <string.h>
#include "parse.h"
#include "../../include/main.h"
#include "../../include/shell.h"

extern char args[10][128];

void dispatcher();
void redir_init();
void clean_buffer();
void parse_buffer(char *);
void set_promat();
void parse_redir();
int check_cmd();

extern struct redir_info // 重定向信息
{
    int is_input_redir;        // 是否为输入重定向
    int is_output_redir;       // 是否为输出重定向
    int out_flag;              // 输出重定向方式
    int out_fd;                // 输出重定向文件描述符
    char out_backup_name[128]; // 临时文件缓冲区
    char in_file_name[128];    // 输入文件名字
    char out_file_name[128];   // 输出文件名字
} redir_info;

extern struct old_fd // 原有重定向信息
{
    int stdin_fd;  // 原有标准输入描述符
    int stdout_fd; // 原有标准输出描述符
    int sterr_fd;  // 原有标准错误描述符
} old_fd;

#endif