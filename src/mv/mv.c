#include "mv.h"

int syscall_mv(char *src, char *dest)
{
    // 移动就简单的理解为 复制 然后删除源文件
    if (strlen(src) == 0 || strlen(dest) == 0)
    {
        printf("Usage:mv source target\n");
        return -1;
    }
    struct stat info1;
    struct stat info2;

    if (stat(src, &info1) == -1)
    {
        printf("%sNo such a file: %s%s\n", COLOR_RED, src, COLOR_RESET);
        return -1;
    }
    // 复制文件到目录,目录不存在
    if (stat(dest, &info2) != 0 && strrchr(dest, '/') != NULL)
    {

        char *path_dir = strrchr(dest, '/');
        if (path_dir)
            *path_dir = 0;

        struct stat temp;
        if (stat(dest, &temp) != 0)
        {
            printf("%s%s No such directory%s\n", COLOR_RED, dest, COLOR_RESET);
            return -1;
        }
        *path_dir = '/';
    }

    if (S_ISDIR(info1.st_mode)) // src是目录
    {

        if (copy_folder(src, dest) == 0) // 复制文件夹
            syscall_rm(src, "-r");
    }

    else if (S_ISREG(info1.st_mode)) // src是文件
    {
        if (stat(dest, &info2) == 0) // dest存在
        {
            if (S_ISDIR(info2.st_mode)) // dest是目录
            {
                if (copy_file_to_dir(src, dest) == 0) // 复制文件到目录下
                    syscall_rm(src, "-r");
            }
            else if (S_ISREG(info2.st_mode)) // dert是文件
            {
                if (copy_file_to_file(src, dest) == 0) // 复制文件到文件
                    syscall_rm(src, "-r");
            }
            else
            {
                printf("Use mv <file1> <file2> to rename file; mv <file> <dir> to move file");
                return -1;
            }
        }
        else // dest不存在，当作文件来处理
        {
            if (copy_file_to_file(src, dest) == 0) // 复制文件到文件
                syscall_rm(src, "-r");
        }
    }

    else
    {
        printf("Use mv <file1> <file2> or <dir1> <dir2> to rename file/directory;\n mv <file> <dir> or <dir1> <dir2> to move file");
        return -1;
    }

    return 1;
}