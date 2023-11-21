#include "cp.h"
#include <dirent.h>

int syscall_cp(char *args0, char *args1, char *args2)
{
    // -r 复制目录
    if (strcmp(args0, "-r") == 0)
    {
        if (strlen(args1) == 0 || strlen(args2) == 0)
        {
            printf("\x1b[33mUse cp -r <dir1> <dir2> to copy directory");
            return -1;
        }
        struct stat info1;
        if (stat(args1, &info1) == 0)
        {
            if (S_ISDIR(info1.st_mode)) // args1是目录
            {
                if (copy_folder(args1, args2) == -1) // 复制文件夹
                    return -1;
            }
            else
            {
                printf("Use cp <file> <dir> to copy file; cp -r <dir1> <dir2> to copy directory");
                return -1;
            }
        }
        else
        {
            printf("\x1b[31mError:dirctory no exist\x1b[0m\n");
            return -1;
        }
    }
    else // 不是-r选项，复制文件
    {
        if (strlen(args0) == 0 || strlen(args1) == 0)
        {
            printf("Use cp <file> <dir> to copy file; cp -r <dir1> <dir2> to copy directory");
            return -1;
        }
        struct stat info0;
        struct stat info1;

        if (stat(args0, &info0) == 0) // args0存在
        {
            if (S_ISREG(info0.st_mode)) // args0是文件
            {
                if (stat(args1, &info1) == 0) // args1存在
                {
                    if (S_ISDIR(info1.st_mode)) // args1是目录
                    {

                        if (copy_file_to_dir(args0, args1) == -1) // 复制文件到目录下
                            return -1;
                    }
                    else
                    {
                        printf("Use cp <file> <dir> to copy file; cp -r <dir1> <dir2> to copy directory");
                        return -1;
                    }
                }
                else // args1不存在，当作文件来处理
                {
                    if (copy_file_to_file(args0, args1) == -1) // 复制文件到文件
                        return -1;
                }
            }
            else
            {
                printf("Use cp <file> <dir> to copy file; cp -r <dir1> <dir2> to copy directory");
                return -1;
            }
        }
        else
        {
            printf("\x1b[31mError:file no exist\x1b[0m\n");
            return -1;
        }
    }
    return 0;
}

// 复制文件到目录
int copy_file_to_dir(char *src, char *dest)
{
    char dest_path[1024];
    // 构建目标地址：dest/src
    bzero(dest_path, sizeof(dest_path));
    if (dest[strlen(dest) - 1] == '/') // 删除最后的 / 字符 如果有
        dest[strlen(dest) - 1] = 0;
    sprintf(dest_path, "%s%s%s", dest, "/", src);

    if (copy_file_to_file(src, dest_path) == -1) // 复制文件到文件
        return -1;
    return 0;
}

// 复制文件到文件
int copy_file_to_file(char *src, char *dest)
{
    FILE *fps, *fpd; // 源文件 目标文件
    int ch;
    fps = fopen(src, "r");
    if (fps == NULL)
    {
        printf("\x1b[31m");
        perror("fopen()");
        printf("\x1b[0m");
        return -1;
    }
    fpd = fopen(dest, "w");
    if (fps == NULL)
    {
        fclose(fps);
        printf("\x1b[31m");
        perror("fopen()");
        printf("\x1b[0m");
        return -1;
    }

    while (1)
    {
        ch = fgetc(fps); // 读取源文件字符 输出给目的文件
        if (ch == EOF)
        {
            break;
        }
        fputc(ch, fpd);
    }

    fclose(fpd);
    fclose(fps);
    return 0;
}

// 复制文件夹
int copy_folder(char *src, char *dest)
{
    char newsrcPath[4096];
    char newdestPath[4096];

    struct stat info;
    if (stat(dest, &info) != 0) // 目录不存在就创建目录
        mkdir(dest, 0777);

    printf("copy to %s to %s\n", src, dest);

    DIR *srcDp = opendir(src);
    if (srcDp == NULL)
    {
        printf("\x1b[31mfailed to open %s\x1b[0m\n", src);
        return -1;
    }
    struct dirent *entry = NULL;
    while ((entry = readdir(srcDp)) != NULL)
    {
        if (entry->d_type == DT_REG)
        {
            // 普通文件
            bzero(newsrcPath, sizeof(newsrcPath)); // 清空
            bzero(newdestPath, sizeof(newdestPath));
            sprintf(newsrcPath, "%s/%s", src, entry->d_name); // 保存新的文件路径
            sprintf(newdestPath, "%s/%s", dest, entry->d_name);

            copy_file_to_file(newsrcPath, newdestPath);
        }
        else if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
        {
            // 子目录，递归处理
            bzero(newsrcPath, sizeof(newsrcPath)); // 清空
            bzero(newdestPath, sizeof(newdestPath));
            sprintf(newsrcPath, "%s/%s", src, entry->d_name); // 保存新的文件路径
            sprintf(newdestPath, "%s/%s", dest, entry->d_name);

            copy_folder(newsrcPath, newdestPath);
        }
    }
    return 0;
}
