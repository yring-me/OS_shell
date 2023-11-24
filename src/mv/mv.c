#include "mv.h"

int syscall_mv(char *src, char *dest)
{
    // 移动就简单的理解为 复制 然后删除源文件
    if (strlen(src) == 0 || strlen(dest) == 0)
    {
        printf("Use mv <file1> <file2> or <dir1> <dir2> to rename file/directory;\n mv <file> <dir> or <dir1> <dir2> to move file");
        return -1;
    }
    struct stat info1;
    struct stat info2;
    if (stat(src, &info1) == 0)
    {
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
    }
    else
    {
        printf("\x1b[31mError:file no exist\x1b[0m\n");
        return -1;
    }
    return 1;
}