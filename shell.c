#include "shell.h"

void set_promat()
{
    char temp[500] = {""};
    getcwd(temp, sizeof(temp));
    sprintf((char *)(promat), "%s", temp);
}

void syscall_history()
{
    int index = -1;
    // printf("%d,%s\n", history_total, (char *)history[history_total]);
    if (history_total == -1)
    {
        printf("\n");
        return;
    }
    for (index = 0; index < history_total + 1; index++)
    {
        printf("%d %s\n", index, (char *)history[index]);
    }
}

// 这俩命令暂时先这样
void syscall_help(char *args)
{
    printf("%s\n", args);
    // printf("%s\n", args);
    // printf("help -- show all the cmds usage\n");
}

void syscall_man()
{
    printf("help -- show all the cmds usage\n");
}

void syscall_ls(char *path, const char *agrs)
{
    DIR *dir;
    struct dirent *entry;
    if (strlen(path) == 0)
    {
        path = ".";
    }

    if ((dir = opendir(path)) == NULL)
    {
        fprintf(stderr, "\x1b[31mError: Cannot open directory '%s'. %s\x1b[0m\n", path, strerror(errno));
        return;
    }

    while ((entry = readdir(dir)) != NULL)
    {
        // 普通文件 绿色
        switch (entry->d_type)
        {
        case DT_REG:
            printf("\x1b[32m");
            break;

        // 文件夹 蓝色
        case DT_DIR:
            printf("\x1b[96m");
            break;

        // 链接文件 紫色
        case DT_LNK:
            printf("\x1b[35m");
            break;
        default:
            break;
        }
        if (strcmp(entry->d_name, redir_info.out_backup_name) != 0)
            printf("%s ", entry->d_name);
        printf("\x1b[0m");
    }
    printf("\n");

    closedir(dir);
}

void syscall_mkdir(char *name)
{
    mkdir(name, 0777);
}

void syscall_cd(char *path)
{
    if (chdir(path) == 0)
    {
        set_promat();
        return;
    }
    printf("\x1b[31mError:dirctory no exist\x1b[0m\n");
}

void syscall_pwd()
{
    char temp[500] = {0};
    getcwd(temp, sizeof(temp));
    printf("%s\n", temp);
}

int syscall_cp(char *src, char *dest)
{

    struct stat file_stat;
    if (stat(src, &file_stat) == -1)
    {
        printf("\x1b[31mError:dirctory no exist\x1b[0m\n");
        return -1;
    }

    if (strlen(src) == 0 || strlen(dest) == 0)
    {
        printf("\x1b[31mUsage:cp <src_file>  <dest_file>\x1b[0m\n");
        return -1;
    }

    if (S_ISDIR(file_stat.st_mode) == 1)
    {
        if (copy_folder(src, dest) == -1)
            return -1;
    }

    else if (S_ISREG(file_stat.st_mode) == 1)
    {
        if (copy_file(src, dest) == -1)
            return -1;
    }

    return 0;
}

int copy_file(char *src, char *dest)
{
    FILE *fps, *fpd;
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
        ch = fgetc(fps);
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

void syscall_rm(char *src, char *arg1)
{
    struct stat file_stat;
    if (strcmp("-r", src) == 0)
    {
        char *temp = src;
        src = arg1;
        arg1 = temp;
    }
    if (strlen(src) == 0)
    {
        printf("\x1b[31mNeed a fild name\x1b[0m\n");
        return;
    }
    if (stat(src, &file_stat) == -1)
    {

        printf("\x1b[31mNo such file or directory\x1b[0m\n");
    }

    // 删文件夹 但缺少 -r 参数
    if (S_ISDIR(file_stat.st_mode) == 1 && strcmp("-r", arg1) != 0)
    {
        printf("\x1b[31mcp: %s is a directory (not copied).\x1b[0m\n", src);
        printf("\x1b[31muse -r to delete it.\x1b[0m\n");
        return;
    }

    // 删文件夹
    if ((S_ISDIR(file_stat.st_mode) == 1 && strcmp("-r", arg1) == 0))
    {
        remove_dir(src);
        return;
    }

    if ((S_ISREG(file_stat.st_mode) == 1))
    {
        printf("\x1b[31m");
        remove(src);
        printf("\x1b[0m");
        return;
    }
}

void remove_dir(char *path)
{
    DIR *dir;
    struct dirent *dirinfo;
    struct stat statbuf;
    char file_path[256] = {0};
    lstat(path, &statbuf);
    if (S_ISREG(statbuf.st_mode) == 1)
    {
        remove(path);
    }
    else if (S_ISDIR(statbuf.st_mode) == 1)
    {
        if ((dir = opendir(path)) == NULL)
        {
            return;
        }
        while ((dirinfo = readdir(dir)) != NULL)
        {

            get_file_path(path, dirinfo->d_name, file_path);
            if (strcmp(dirinfo->d_name, ".") == 0 || strcmp(dirinfo->d_name, "..") == 0)
                continue;

            remove_dir(file_path);
            rmdir(file_path);
        }
        closedir(dir);
    }
    remove(path);
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

void syscall_mv(char *src, char *dest)
{
    if (syscall_cp(src, dest) == -1)
    {
        return;
    }
    syscall_rm(src, "-r");
}
