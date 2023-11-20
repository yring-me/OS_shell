#include "mkdir.h"
struct mk_info mk_info;
void syscall_mkdir(char *args0, char *args1, char *args2, char *args3, char *args4)
{
    // mkdir(name, 0777);
    mk_info_init();
    mkdir_info_check(args0, args1, args2, args3, args4);
    if (mk_info.is_p == 1)
    {
        mkdir_recursive(mk_info.path, mk_info.mode);
    }
    else
    {
        if (strlen(args0) != 0 && strcmp(args0, "-m") != 0)
        {
            if (mkdir(args0, mk_info.mode) == -1)
                fprintf(stderr, "Failed to create directory: %s\n", args0);
        }
        if (strlen(args1) != 0 && strcmp(args0, "-m") != 0)
        {
            if (mkdir(args1, mk_info.mode) == -1)
                fprintf(stderr, "Failed to create directory: %s\n", args1);
        }
        if (strlen(args2) != 0 && strcmp(args2, "-m") != 0)
        {
            if (mkdir(args2, mk_info.mode) == -1)
                fprintf(stderr, "Failed to create directory: %s\n", args2);
        }
        if (strlen(args3) != 0 && strcmp(args2, "-m") != 0)
        {
            if (mkdir(args3, mk_info.mode) == -1)
                fprintf(stderr, "Failed to create directory: %s\n", args3);
        }
        if (strlen(args4) != 0)
        {
            if (mkdir(args4, mk_info.mode) == -1)
                fprintf(stderr, "Failed to create directory: %s\n", args4);
        }
    }

    mk_info_reset();
}

// 递归创建目录
void mkdir_recursive(const char *path, mode_t mode)
{
    char *p = NULL;
    char *temp_path = strdup(path); // 使用strdup复制路径，以免修改原始路径字符串

    // 逐级创建目录
    for (p = strchr(temp_path + 1, '/'); p; p = strchr(p + 1, '/'))
    {
        *p = '\0'; // 截断路径字符串
        if (mkdir(temp_path, mode) == -1)
        {
            // 如果目录已存在，忽略错误
            if (errno != EEXIST)
            {
                perror("mkdir");
                exit(EXIT_FAILURE);
            }
        }
        *p = '/'; // 恢复路径字符串
    }

    // 创建最终目录
    if (mkdir(temp_path, mode) == -1)
    {
        // 如果目录已存在，忽略错误
        if (errno != EEXIST)
        {
            perror("mkdir");
            exit(EXIT_FAILURE);
        }
    }

    free(temp_path); // 释放分配的内存
}

void mkdir_info_check(char *args0, char *args1, char *args2, char *args3, char *args4)
{
    if (args0[0] == '-' && strcmp(args0, "-p") != 0 && strcmp(args0, "-m") != 0)
        printf("\x1b[33mWarning Invalid Arguments %s\x1b[0m\n", args0);
    if (args2[0] == '-' && strcmp(args2, "-p") != 0 && strcmp(args2, "-m") != 0)
        printf("\x1b[33mWarning Invalid Arguments %s\x1b[0m\n", args2);

    // 检查有 -p 或者 -m 标志
    if (strcmp(args0, "-p") == 0)
    {
        mk_info.is_p = 1;
        if (mk_info.is_p == 1 && strlen(args1) != 0 && args1[0] != '-')
        {
            memset(mk_info.path, 0, sizeof(mk_info.path));
            sprintf(mk_info.path, "%s", args1);
        }
        else
        {
            printf("\x1b[31muse -p <dir> to make dir.\x1b[0m\n");
            return;
        }
    }

    if (strcmp(args2, "-p") == 0)
    {
        mk_info.is_p = 1;
        if (mk_info.is_p == 1 && strlen(args3) != 0 && args3[0] != '-')
        {
            memset(mk_info.path, 0, sizeof(mk_info.path));
            sprintf(mk_info.path, "%s", args3);
        }
        else
        {
            printf("\x1b[31muse -p <dir> to make dir.\x1b[0m\n");
            return;
        }
    }

    if (strcmp(args0, "-m") == 0)
    {
        mk_info.is_m = 1;
        if (mk_info.is_m == 1 && strlen(args1) != 0 && args1[0] != '-')
        {
            long tmp = strtol(args1, NULL, 8);
            mk_info.mode = (mode_t)tmp;
        }
        else
        {
            printf("\x1b[31muse -p <dir> to make dir.\x1b[0m\n");
            return;
        }
    }

    if (strcmp(args2, "-m") == 0)
    {
        mk_info.is_m = 1;
        if (mk_info.is_m == 1 && strlen(args3) != 0 && args3[0] != '-')
        {
            long tmp = strtol(args3, NULL, 8);
            mk_info.mode = (mode_t)tmp;
        }
        else
        {
            printf("\x1b[31muse -p <dir> to make dir.\x1b[0m\n");
            return;
        }
    }
}

// 重置 mk_info 信息
void mk_info_reset()
{
    mk_info.is_p = 0;
    mk_info.is_m = 1;
    memset(mk_info.path, 0, sizeof(mk_info.path));
    sprintf(mk_info.path, "%s", ".");
    mk_info.mode = 0777;
}

// 初始化 mk_info 信息
void mk_info_init()
{
    mk_info.is_p = 0;
    mk_info.is_m = 1;
    memset(mk_info.path, 0, sizeof(mk_info.path));
    sprintf(mk_info.path, "%s", ".");
    mk_info.mode = 0777;
}
