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
#include <wait.h>

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
#define SYS_MKDIR 8
#define SYS_TREE 9
#define SYS_PIPE 11
#define SYS_CAT 12
#define SYS_CLEAR 13
#define SYS_HISTORY 99

#define MAX_INPUT_SIZE 1024
// 函数指针，用于抽象，统一调用
typedef int (*syscall_handler_t)(char *, char *, char *, char *);

// 其他辅助函数
void ls_info_check(char *args0, const char *agrs1, const char *args2);
void ls_info_reset();
void ls_info_init();
void ls_l();
void ls();
void set_promat();
int copy_file(char *src, char *dest);
int copy_folder(char *src, char *dest);
void remove_dir(char *path);
void get_file_path(const char *path, const char *filename, char *filepath);
void tree(char *direntName, int level, int is_a);
void cat_file(const char *file);
int cat(int argc, char *argv);

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
void syscall_mkdir(char *name);
void syscall_tree(char *args0, char *args1);
void syscall_pipe(char *cmd1, char *cmd2);
int syscall_cat(char *argv);
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

static struct redir_info // 重定向信息
{
    int is_input_redir;        // 是否为输入重定向
    int is_output_redir;       // 是否为输出重定向
    int out_flag;              // 输出重定向方式
    int out_fd;                // 输出重定向文件描述符
    char out_backup_name[128]; // 临时文件缓冲区
    char in_file_name[128];    // 输入文件名字
    char out_file_name[128];   // 输出文件名字
} redir_info;

static struct old_fd // 原有重定向信息
{
    int stdin_fd;  // 原有标准输入描述符
    int stdout_fd; // 原有标准输出描述符
    int sterr_fd;  // 原有标准错误描述符
} old_fd;

static struct ls_info // ls 信息
{
    int is_a;       // 是否带-a参数
    int is_l;       // 是否带-l参数
    char path[256]; // 路径名字
} ls_info;
#endif
