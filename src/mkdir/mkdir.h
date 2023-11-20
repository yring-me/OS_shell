#ifndef MKDIR_H
#define MKDIR_H
#include "../parse/parse.h"
#include "../../include/main.h"
#include "../../include/shell.h"

extern struct mk_info // ls 信息
{
    int is_p;       // 是否带-a参数
    int is_m;       // 是否带-l参数
    char path[256]; // 路径名字
    mode_t mode;    // 权限
} mk_info;

void mkdir_recursive(const char *path, mode_t mode);
void mkdir_info_check(char *args0, char *args1, char *args2, char *args3, char *args4);
void mk_info_reset();
void mk_info_init();

#endif