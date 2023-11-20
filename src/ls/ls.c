#include "ls.h"

struct ls_info ls_info = {0};

void syscall_ls(char *args0, const char *agrs1, const char *args2)
{
    ls_info_init();                     // ls信息初始化
    ls_info_check(args0, agrs1, args2); // 处理参数信息，并把相关信息赋值给ls_info结构体
    if (ls_info.is_l == 1)
        ls_l(); // 带 -l 参数
    else
        ls();        // 不带 -l 参数
    ls_info_reset(); // 重置ls_info结构体
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
