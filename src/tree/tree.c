#include "tree.h"

void syscall_tree(char *args0, char *args1)
{
    // 处理 -a 参数，以及默认是当前路径
    char path[256] = {0};
    int is_a = 0;
    if (strlen(args0) == 0 && strlen(args1) == 0)
        sprintf(path, "%s", ".");
    if (strcmp(args0, "-a") == 0 && strlen(args1) != 0)
    {
        sprintf(path, "%s", args1);
        is_a = 1;
    }

    if (strcmp(args0, "-a") == 0 && strlen(args1) == 0)
    {
        sprintf(path, "%s", ".");
        is_a = 1;
    }

    if (args0[0] != '-' && strlen(path) == 0)
        sprintf(path, "%s", args0);
    printf("%s\n", path);
    tree(path, 0, is_a);
}

void tree(char *direntName, int level, int is_a)
{
    // 定义一个目录流指针
    DIR *p_dir = NULL;

    // 定义一个目录结构体指针
    struct dirent *entry = NULL;

    // 打开目录，返回一个目录流指针指向第一个目录项
    p_dir = opendir(direntName);
    if (p_dir == NULL)
    {
        printf("\x1b[31mopendir error\n\x1b[0m");
        return;
    }

    // 循环读取每个目录项, 当返回NULL时读取完毕
    while ((entry = readdir(p_dir)) != NULL)
    {
        // 备份之前的目录名
        char *backupDirName = NULL;

        if (is_a == 0 && entry->d_name[0] == '.')
            continue;
        else if (is_a == 1 && strcmp(entry->d_name, "..") == 0 || (strcmp(entry->d_name, ".") == 0))
        {
            continue;
        }

        int i;
        for (i = 0; i < level; i++)
        {
            printf("│");
            printf("   ");
        }
        int temp = telldir(p_dir);
        if (readdir(p_dir) == NULL)
        {
            printf("└──");
        }
        else
        {
            printf("├──"); // └
        }

        if (entry->d_type == DT_DIR)
            printf("\x1b[96m");
        else if (entry->d_type == DT_REG)
            printf("\x1b[32m");
        printf("%s\n\x1b[0m", entry->d_name);

        // 如果目录项仍是一个目录的话，进入目录
        if (entry->d_type == DT_DIR)
        {
            // 当前目录长度
            int curDirentNameLen = strlen(direntName) + 1;

            // 备份
            backupDirName = (char *)malloc(curDirentNameLen);
            memset(backupDirName, 0, curDirentNameLen);
            memcpy(backupDirName, direntName, curDirentNameLen);

            strcat(direntName, "/");
            strcat(direntName, entry->d_name);
            tree(direntName, level + 1, is_a);

            // 恢复之前的目录名
            memcpy(direntName, backupDirName, curDirentNameLen);
            free(backupDirName);
            backupDirName = NULL;
        }
    }

    closedir(p_dir);
}
