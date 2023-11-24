#ifndef LS_H
#define LS_H
#include "../parse/parse.h"
#include "../../include/main.h"
#include "../../include/shell.h"
extern struct ls_info // ls 信息
{
    int is_a;       // 是否带-a参数
    int is_l;       // 是否带-l参数
    char path[256]; // 路径名字
} ls_info;

void ls_info_check(char *args0, const char *agrs1, const char *args2);
void ls_info_reset();
void ls_info_init();
int ls_l();
int ls();
#endif