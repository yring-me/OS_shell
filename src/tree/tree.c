#include "tree.h"
int tree_file = 0;
int tree_folder = 0;
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
    int isLast[1024] = {0}; // 用于记录父节点是不是最后一个文件
    tree(path, 0, is_a, isLast);

    printf("\x1b[33m%d directories, %d files\x1b[0m\n", tree_folder, tree_file);
    tree_folder = 0;
    tree_file = 0;
}

void tree(char *direntName, int level, int is_a, int isLast[])
{
    // 定义一个目录流指针
    DIR *p_dir = NULL;

    // 定义一个目录结构体指针
    struct dirent *entry = NULL;

    // 定义一个数组记录当前目录的所有文件
    char *entries[1024];
    int count = 0;
    // 打开目录，返回一个目录流指针指向第一个目录项
    p_dir = opendir(direntName);

    if (p_dir == NULL)
    {
        printf("\x1b[31mopendir error\n\x1b[0m");
        return;
    }

    while ((entry = readdir(p_dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
        {
            entries[count] = strdup(entry->d_name);

            count++;
        }
    }

    closedir(p_dir);

    for (int i = 0; i < count; i++)
    {
        if (is_a == 0 && entries[i][0] == '.')
            continue;
        for (int j = 0; j < level; j++)
        {
            printf(isLast[j] ? "    " : "│   "); // 如果是当前子目录的最后一个文件，则需要将前面的树状结构空出来
        }

        printf("%s ", i == count - 1 ? "└──" : "├──");

        char fullPath[1024];
        sprintf(fullPath, "%s/%s", direntName, entries[i]);
        struct stat statbuf;
        stat(fullPath, &statbuf);

        // 根据文件类型来决定显示结果的颜色
        if (S_ISDIR(statbuf.st_mode))
        {
            printf("%s%s%s\n", "\x1b[34m", entries[i], "\x1b[0m");
            isLast[level] = (i == count - 1); // 记录当前目录是否为最后一个文件
            tree(fullPath, level + 1, is_a, isLast);
            tree_folder++;
        }
        else
        {
            printf("%s%s%s\n", "\x1b[32m", entries[i], "\x1b[0m");
            tree_file++;
        }

        free(entries[i]);
    }
}

int insensitiveCompare(const void *a, const void *b)
{
    const char *str1 = *(const char **)a;
    const char *str2 = *(const char **)b;
    return strcasecmp(str1, str2);
}