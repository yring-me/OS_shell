#include "cp.h"

int syscall_cp(char *src, char *dest)
{

    struct stat file_stat;
    if (stat(src, &file_stat) == -1) // 文件不存在
    {
        printf("\x1b[31mError:dirctory no exist\x1b[0m\n");
        return -1;
    }

    if (strlen(src) == 0 || strlen(dest) == 0) // 源文件或目的不存在
    {
        printf("\x1b[31mUsage:cp <src_file>  <dest_file>\x1b[0m\n");
        return -1;
    }

    if (S_ISDIR(file_stat.st_mode) == 1)
    {
        if (copy_folder(src, dest) == -1) // 复制文件夹
            return -1;
    }

    else if (S_ISREG(file_stat.st_mode) == 1)
    {
        if (copy_file(src, dest) == -1) // 复制文件
            return -1;
    }

    return 0;
}

// 复制文件
int copy_file(char *src, char *dest)
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

    if (mkdir(dest, 0777)) // 如果不存在就用mkdir函数来创建
    {
        printf("\x1b[31mfile has exited\x1b[0m\n");
        return -1;
    }

    DIR *srcDp = opendir(src);
    if (srcDp == NULL)
    {
        printf("\x1b[31mfaile to open %s\x1b[0m\n", src);
        return -1;
    }
    struct dirent *srcDirent = NULL;
    int flag = 0;
    while ((srcDirent = readdir(srcDp)) != NULL)
    {
        flag++;
        if (flag > 2) // 去除隐藏文件 . ..
        {
            bzero(newsrcPath, sizeof(newsrcPath)); // 清空
            bzero(newdestPath, sizeof(newdestPath));

            sprintf(newsrcPath, "%s/%s", src, srcDirent->d_name); // 保存新的文件路径
            sprintf(newdestPath, "%s/%s", dest, srcDirent->d_name);

            if (srcDirent->d_type == DT_DIR) // 文件夹的拷贝
                copy_folder(newsrcPath, newdestPath);
            else // 普通文件
                copy_file(newsrcPath, newdestPath);
        }
    }
    return 0;
}
