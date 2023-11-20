#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <sys/wait.h>

// 各模块头文件，如果有
#include "../src/cat/cat.h"
#include "../src/cd/cd.h"
#include "../src/clear/clear.h"
#include "../src/cp/cp.h"
#include "../src/help_man/help_man.h"
#include "../src/history/history.h"
#include "../src/promat/promat.h"
#include "../src/ls/ls.h"
#include "../src/mkdir/mkdir.h"
#include "../src/mv/mv.h"
#include "../src/parse/parse.h"
#include "../src/pipe/pipe.h"
#include "../src/promat/promat.h"
#include "../src/pwd/pwd.h"
#include "../src/rm/rm.h"
#include "../src/tree/tree.h"

#define SYS_HELP 0
#define SYS_MAN 1
#define SYS_LS 2
#define SYS_CD 3
#define SYS_PWD 4
#define SYS_CP 5
#define SYS_RM 6
#define SYS_MV 7
#define SYS_MKDIR 8
#define SYS_TREE 9
#define SYS_PIPE 11
#define SYS_CAT 12
#define SYS_CLEAR 13
#define SYS_HISTORY 99

#define MAX_INPUT_SIZE 1024
// 函数指针，用于抽象，统一调用
typedef int (*syscall_handler_t)(char *, char *, char *, char *);

// shell命令函数声明
void syscall_history();
void syscall_help(char *args);
void syscall_man();
void syscall_ls(char *args0, const char *agrs1, const char *args2);
void syscall_cd(char *path);
void syscall_pwd();
int syscall_cp(char *src, char *dest);
void syscall_rm(char *src, char *arg1);
void syscall_mv(char *src, char *dest);
void syscall_mkdir(char *args0, char *args1, char *args2, char *args3, char *args4);
void syscall_tree(char *args0, char *args1);
void syscall_pipe(char *cmd1, char *cmd2);
int syscall_cat(char *args0, char *args1, char *args2);
void syscall_clear();

// shell函数注册
static const syscall_handler_t sys_table[] = {
    [SYS_HISTORY] = (syscall_handler_t)syscall_history,
    [SYS_HELP] = (syscall_handler_t)syscall_help,
    [SYS_MAN] = (syscall_handler_t)syscall_man,
    [SYS_LS] = (syscall_handler_t)syscall_ls,
    [SYS_CD] = (syscall_handler_t)syscall_cd,
    [SYS_PWD] = (syscall_handler_t)syscall_pwd,
    [SYS_CP] = (syscall_handler_t)syscall_cp,
    [SYS_RM] = (syscall_handler_t)syscall_rm,
    [SYS_MV] = (syscall_handler_t)syscall_mv,
    [SYS_MKDIR] = (syscall_handler_t)syscall_mkdir,
    [SYS_TREE] = (syscall_handler_t)syscall_tree,
    [SYS_PIPE] = (syscall_handler_t)syscall_pipe,
    [SYS_CAT] = (syscall_handler_t)syscall_cat,
    [SYS_CLEAR] = (syscall_handler_t)syscall_clear,

};

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
    {
        .name = "help",
        .id = SYS_HELP,
    },
    {
        .name = "man",
        .id = SYS_MAN,
    },
    {
        .name = "ls",
        .id = SYS_LS,
    },
    {
        .name = "cd",
        .id = SYS_CD,
    },
    {
        .name = "pwd",
        .id = SYS_PWD,
    },
    {
        .name = "cp",
        .id = SYS_CP,
    },
    {
        .name = "rm",
        .id = SYS_RM,
    },
    {
        .name = "mv",
        .id = SYS_MV,
    },
    {
        .name = "mkdir",
        .id = SYS_MKDIR,
    },
    {
        .name = "tree",
        .id = SYS_TREE,
    },
    {
        .name = "|",
        .id = SYS_PIPE,
    },
    {
        .name = "cat",
        .id = SYS_CAT,
    },
    {
        .name = "clear",
        .id = SYS_CLEAR,
    },
};

#endif
