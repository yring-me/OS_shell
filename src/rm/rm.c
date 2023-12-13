#include "rm.h"
int syscall_rm(char *src, char *arg1)
{
    struct stat file_stat;
    if (strcmp("-r", src) == 0) // 带 -r 参数的处理，即使得src始终指向文件
    {
        char *temp = src;
        src = arg1;
        arg1 = temp;
    }
    if (strlen(src) == 0)
    {
        printf("\x1b[31mNeed a fild name\x1b[0m\n");
        return -1;
    }
    if (stat(src, &file_stat) == -1)
    {

        printf("\x1b[31mNo such file or directory\x1b[0m\n");
    }

    // 删文件夹 但缺少 -r 参数
    if (S_ISDIR(file_stat.st_mode) == 1 && strcmp("-r", arg1) != 0)
    {
        printf("\x1b[31mrm: %s is a directory (not copied).\x1b[0m\n", src);
        printf("\x1b[31muse -r to delete it.\x1b[0m\n");
        return -1;
    }

    // 删文件夹
    if ((S_ISDIR(file_stat.st_mode) == 1 && strcmp("-r", arg1) == 0))
    {
        remove_dir(src);
        return 1;
    }

    if ((S_ISREG(file_stat.st_mode) == 1))
    {
        printf("\x1b[31m");
        remove(src);
        printf("\x1b[0m");
        return 1;
    }
    return 1;
}

// 删除文件夹
void remove_dir(char *path)
{
    DIR *dir;
    struct dirent *dirinfo;
    struct stat statbuf;
    char file_path[256] = {0};
    lstat(path, &statbuf);
    if (S_ISREG(statbuf.st_mode) == 1) // 删除普通文件
    {
        remove(path);
        return;
    }
    else if (S_ISDIR(statbuf.st_mode) == 1)
    {
        if ((dir = opendir(path)) == NULL)
        {
            return;
        }
        while ((dirinfo = readdir(dir)) != NULL) // 删除文件夹
        {

            get_file_path(path, dirinfo->d_name, file_path);                             // 得到完整路径
            if (strcmp(dirinfo->d_name, ".") == 0 || strcmp(dirinfo->d_name, "..") == 0) // 跳过 . .. 两个特殊目录
                continue;

            remove_dir(file_path);
            // rmdir(file_path);
        }
        closedir(dir);
    }
    remove(path); // 删除当前文件
    return;
}

void get_file_path(const char *path, const char *filename, char *filepath)
{
    strcpy(filepath, path);
    if (filepath[strlen(path) - 1] != '/')
    {
        strcat(filepath, "/");
    }
    strcat(filepath, filename);
}
