#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char promat[512] = {};

extern char history[100][128];
extern int history_total;
extern void parse_buffer(char *); // 对命令进行分析
extern void dispatcher();         // 对命令进行分发
extern void set_promat();         // 设置格式
extern void redir_init();         // 重定向信息初始化
extern void parse_redir();        // 分析重定向信息
extern void fd_reset();           // 描述符重置
extern void clean_buffer();       // 清空缓冲区
extern int check_cmd();           // 检查命令是否合法
extern void ls_info_init();       // ls信息初始化

#endif