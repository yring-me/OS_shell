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

extern char history[100][128];
extern int history_total;
extern char promat[512];

#define SYS_HELP 0
#define SYS_MAN 1
#define SYS_LS 2
#define SYS_CD 3
#define SYS_PWD 4
#define SYS_CP 5
#define SYS_RM 6
#define SYS_MV 7
#define SYS_HISTORY 99

// 函数指针，用于抽象，统一调用
typedef int (*syscall_handler_t)(char *, char *, char *, char *);

// 其他辅助函数
void set_promat();
int copy_file(char *src, char *dest);
int copy_folder(char *src, char *dest);
void remove_dir(char *path);
void get_file_path(const char *path, const char *filename, char *filepath);
// shell命令函数声明
void syscall_history();
void syscall_help(char *args);
void syscall_man();
void syscall_ls(char *path, const char *agrs);
void syscall_cd(char *path);
void syscall_pwd();
int syscall_cp(char *src, char *dest);
void syscall_rm(char *src, char *arg1);
void syscall_mv(char *src, char *dest);

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
};

static struct redir_info
{
    int is_input_redir;
    int is_output_redir;
    int out_flag;
    int out_fd;
    char out_backup_name[128];
    char in_file_name[128];
    char out_file_name[128];
} redir_info;

static struct old_fd
{
    int stdin_fd;
    int stdout_fd;
    int sterr_fd;
} old_fd;

static struct new_fd
{
    int stdin_fd;
    int stdout_fd;
    int sterr_fd;
} new_fd;

#endif