#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#define SYS_HISTORY 99

// 函数指针，用于抽象，统一调用
typedef int (*syscall_handler_t)(char[10][128]);

// shell命令函数声明
void syscall_history();

// shell函数注册
static const syscall_handler_t sys_table[] = {
    [SYS_HISTORY] = (syscall_handler_t)syscall_history};

// 基本shell结构
typedef struct shell
{
    char *name;
    int id;
} shell;

// 命令注册
static const shell cmd_list[] = {
    {
        .name = "history",
        .id = SYS_HISTORY,
    },
};

#endif