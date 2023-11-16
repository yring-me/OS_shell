#include "shell.h"

void set_promat()
{
    char temp[500] = {0};
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
        fprintf(stderr, "Error: Cannot open directory '%s'. %s\n", path, strerror(errno));
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

        printf("%s-%d ", entry->d_name, entry->d_type);
        printf("\x1b[0m");
    }
    printf("\n");
    closedir(dir);
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
    FILE *fps, *fpd;
    int ch;
    struct stat file_stat;
    if (stat(src, &file_stat) == -1)
    {
        printf("\x1b[31mError:dirctory no exist\x1b[0m\n");
        return -1;
    }

    if (S_ISDIR(file_stat.st_mode) == 1)
    {
        printf("\x1b[31mcp: %s is a directory (not copied).\x1b[0m\n", src);
        return -1;
    }

    if (strlen(src) == 0 || strlen(dest) == 0)
    {
        printf("\x1b[31mUsage:cp <src_file>  <dest_file>\x1b[0m\n");
        return -1;
    }

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

void syscall_rm(char *src, char *arg1)
{
    struct stat file_stat;
    if (stat(src, &file_stat) == -1)
    {
        printf("\x1b[31m");
        perror("stat");
        printf("\x1b[0m");
    }

    if (S_ISDIR(file_stat.st_mode) == 1 && strcmp("-r", arg1) != 0)
    {
        printf("\x1b[31mcp: %s is a directory (not copied).\x1b[0m\n", src);
        printf("\x1b[31muse -r to delete it.\x1b[0m\n");
        return;
    }

    if ((S_ISDIR(file_stat.st_mode) == 1 && strcmp("-r", arg1) == 0) || (S_ISREG(file_stat.st_mode) == 1))
    {
        printf("\x1b[31m");
        remove(src);
        printf("\x1b[0m");
        return;
    }
}

void syscall_mv(char *src, char *dest)
{
    if (syscall_cp(src, dest) == -1)
    {
        return;
    }
    syscall_rm(src, "-r");
}
