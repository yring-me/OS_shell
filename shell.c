#include "shell.h"

// 处理样式，具体的我想输出当前路径
void set_promat()
{
    char temp[500] = {""};
    getcwd(temp, sizeof(temp));
    sprintf((char *)(promat), "%s", temp);
}

// 历史记录信息
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

void syscall_ls(char *args0, const char *agrs1, const char *args2)
{
    ls_info_check(args0, agrs1, args2); // 处理参数信息，并把相关信息赋值给ls_info结构体
    if (ls_info.is_l == 1)
        ls_l(); // 带 -l 参数
    else
        ls();        // 不带 -l 参数
    ls_info_reset(); // 重置ls_info结构体
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

void syscall_rm(char *src, char *arg1)
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

void syscall_mv(char *src, char *dest)
{
    // 移动就简单的理解为 复制 然后删除源文件
    if (syscall_cp(src, dest) == -1)
    {
        return;
    }
    syscall_rm(src, "-r");
}

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

// 处理 ls 的参数信息
void ls_info_check(char *args0, const char *args1, const char *args2)
{
    // 检查是否为合法参数，但只警告
    if (args0[0] == '-' && strcmp(args0, "-l") != 0 && strcmp(args0, "-a") != 0 && strcmp(args0, "-al") != 0 && strcmp(args0, "-la") != 0)
        printf("\x1b[33mWarning Invalid Arguments %s\x1b[0m\n", args0);
    if (args1[0] == '-' && strcmp(args1, "-l") != 0 && strcmp(args1, "-a") != 0 && strcmp(args1, "-al") != 0 && strcmp(args1, "-la") != 0)
        printf("\x1b[33mWarning Invalid Arguments %s\x1b[0m\n", args1);
    if (args2[0] == '-' && strcmp(args2, "-l") != 0 && strcmp(args2, "-a") != 0 && strcmp(args2, "-al") != 0 && strcmp(args2, "-la") != 0)
        printf("\x1b[33mWarning Invalid Arguments %s\x1b[0m\n", args2);

    // 检查有 -l 或者 -a 标志
    if (strcmp(args0, "-l") == 0 || strcmp(args1, "-l") == 0 || strcmp(args2, "-l") == 0)
        ls_info.is_l = 1;
    if (strcmp(args0, "-a") == 0 || strcmp(args1, "-a") == 0 || strcmp(args2, "-a") == 0)
        ls_info.is_a = 1;
    if (strcmp(args0, "-al") == 0 || strcmp(args1, "-al") == 0 || strcmp(args2, "-al") == 0)
    {
        ls_info.is_l = 1;
        ls_info.is_a = 1;
    }
    if (strcmp(args0, "-la") == 0 || strcmp(args1, "-la") == 0 || strcmp(args2, "-la") == 0)
    {
        ls_info.is_l = 1;
        ls_info.is_a = 1;
    }

    // 处理路径信息
    if (strlen(args2) != 0 && args2[0] != '-')
        sprintf(ls_info.path, "%s", args2);

    if (strlen(args1) != 0 && args1[0] != '-')
    {
        memset(ls_info.path, 0, sizeof(ls_info.path));
        sprintf(ls_info.path, "%s", args1);
    }

    if (strlen(args0) != 0 && args0[0] != '-')
    {
        memset(ls_info.path, 0, sizeof(ls_info.path));
        sprintf(ls_info.path, "%s", args0);
    }
}

// 重置 ls_info 信息
void ls_info_reset()
{
    ls_info.is_a = 0;
    ls_info.is_l = 0;
    memset(ls_info.path, 0, sizeof(ls_info.path));
    sprintf(ls_info.path, "%s", ".");
}

// 初始化 ls_info 信息
void ls_info_init()
{
    ls_info.is_a = 0;
    ls_info.is_l = 0;
    memset(ls_info.path, 0, sizeof(ls_info.path));
    sprintf(ls_info.path, "%s", ".");
}

// ls -l
void ls_l()
{
    DIR *dir;
    struct dirent *entry;
    static char *perm[] = {"---", "--x", "-w-", "-wx", "r--", "r-x", "rw-", "rwx"}; // 文件权限对应输出
    unsigned mask = 0700;
    if ((dir = opendir(ls_info.path)) == NULL)
    {
        fprintf(stderr, "Error: Cannot open directory '%s'. %s\n", ls_info.path, strerror(errno));
        return;
    }

    // readdir 函数：用于读取目录中的条目。它返回一个指向 dirent 结构体的指针，该结构体包含有关目录中下一个文件或子目录的信息。
    while ((entry = readdir(dir)) != NULL)
    {
        if (ls_info.is_a == 0 && entry->d_name[0] == '.') // 处理是否带有-a参数
            continue;
        // 构建文件路径：使用 sprintf 函数构建文件的完整路径
        char file_path[MAX_INPUT_SIZE];
        sprintf(file_path, "%s/%s", ls_info.path, entry->d_name);

        // 获取文件信息：使用 stat 函数获取文件的详细信息，包括文件类型、权限、所有者、组等
        struct stat file_stat;
        if (stat(file_path, &file_stat) == -1)
        {
            perror("stat");
            exit(EXIT_FAILURE);
        }

        // 获取所有者和组信息：使用 getpwuid 和 getgrgid 函数获取文件所有者和组的详细信息
        struct passwd *owner_info = getpwuid(file_stat.st_uid);
        struct group *group_info = getgrgid(file_stat.st_gid);
        char time_str[20];
        strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", localtime(&file_stat.st_mtime));
        char file_type = '-';
        if (S_ISDIR(file_stat.st_mode))
        {
            file_type = 'd'; // 目录
        }
        else if (S_ISLNK(file_stat.st_mode))
        {
            file_type = 'l'; // 符号链接
        }

        // 输出文件信息：根据获取到的文件信息，使用 printf 函数输出文件的详细信息
        printf("%c%3s%3s%3s %5hu %s %s %10lld %s ",
               file_type,
               perm[(file_stat.st_mode & mask) >> 2 * 3],
               perm[(file_stat.st_mode & (mask >> 3)) >> 1 * 3],
               perm[(file_stat.st_mode & (mask >> 6)) >> 0 * 3],
               file_stat.st_nlink,
               (owner_info != NULL) ? owner_info->pw_name : "",
               (group_info != NULL) ? group_info->gr_name : "",
               file_stat.st_size,
               time_str);
        switch (entry->d_type) // 处理颜色
        {
        // 普通文件 绿色
        case DT_REG:
            printf("\x1b[32m%s\n\x1b[0m", entry->d_name);
            break;

        // 文件夹 蓝色
        case DT_DIR:
            printf("\x1b[96m%s\n\x1b[0m", entry->d_name);
            break;

        // 链接文件 紫色
        case DT_LNK:
            printf("\x1b[35m%s\n\x1b[0m", entry->d_name);
            break;
        default:
            printf("%s\n", entry->d_name);
        }
    }
    closedir(dir);
}

// ls
void ls()
{
    DIR *dir;
    struct dirent *entry;

    if ((dir = opendir(ls_info.path)) == NULL)
    {
        fprintf(stderr, "\x1b[31mError: Cannot open directory '%s'. %s\x1b[0m\n", ls_info.path, strerror(errno));
        ls_info_reset();
        return;
    }

    while ((entry = readdir(dir)) != NULL)
    {
        if (ls_info.is_a == 0 && entry->d_name[0] == '.') // 处理是否带有 -a 参数
            continue;
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
