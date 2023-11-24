#include "cat.h"

struct cat_info cat_info;

int syscall_cat(char *args0, char *args1, char *args2)
{
    cat_info_init();
    cat_info_check(args0, args1, args2);
    if (cat() == -1)
    {
        cat_info_reset();
        return -1;
    }
    cat_info_reset();
    return 0;
}

// cat 命令主体
int cat()
{
    FILE *fp;
    if ((fp = fopen(cat_info.file, "r")) == NULL)
    { // 判断是不是空文件
        // 打印错误信息
        perror(cat_info.file);
        return -1;
    }
    char buff[1024] = {0};
    while (fgets(buff, sizeof(buff), fp))
    { // 按行读取
        // 都不成立 直接打印
        if (!cat_info.nflag && !cat_info.bflag)
        {
            printf("%s", buff);
            continue;
        }
        if (buff[0] != '\n')
        {
            cat_info.num++;
            printf("%d\t%s", cat_info.num, buff);
        }
        else
        {
            // nflag成立 打印行号 不成立 不打印行号
            if (cat_info.nflag && !cat_info.bflag)
            {
                cat_info.num++;
                printf("%d\t\n", cat_info.num);
            }
            else
            {
                printf("\n");
            }
        }
    }
    fclose(fp); // 关闭文件
    return 1;
}

// 处理 cat 的参数信息
void cat_info_check(char *args0, const char *args1, const char *args2)
{
    // 检查是否为合法参数，但只警告
    cat_info.num = 0;
    if (args0[0] == '-' && strcmp(args0, "-b") != 0 && strcmp(args0, "-n") != 0)
        printf("\x1b[33mWarning Invalid Arguments %s\x1b[0m\n", args0);
    if (args1[0] == '-' && strcmp(args1, "-b") != 0 && strcmp(args1, "-n") != 0)
        printf("\x1b[33mWarning Invalid Arguments %s\x1b[0m\n", args1);
    if (args2[0] == '-' && strcmp(args2, "-b") != 0 && strcmp(args2, "-n") != 0)
        printf("\x1b[33mWarning Invalid Arguments %s\x1b[0m\n", args2);

    // 检查有 -b 或者 -n 标志
    if (strcmp(args0, "-b") == 0 || strcmp(args1, "-b") == 0 || strcmp(args2, "-b") == 0)
        cat_info.bflag = 1;
    if (strcmp(args0, "-n") == 0 || strcmp(args1, "-n") == 0 || strcmp(args2, "-n") == 0)
        cat_info.nflag = 1;
    if (cat_info.bflag == 1 && cat_info.nflag == 1)
    {
        printf("\x1b[33mWarning Conflict Arguments -b -n \x1b[0m\n");
        cat_info.nflag = 0;
        cat_info.bflag = 0;
    }

    // 处理路径信息
    if (strlen(args2) != 0 && args2[0] != '-')
        sprintf(cat_info.file, "%s", args2);

    if (strlen(args1) != 0 && args1[0] != '-')
    {
        memset(cat_info.file, 0, sizeof(cat_info.file));
        sprintf(cat_info.file, "%s", args1);
    }

    if (strlen(args0) != 0 && args0[0] != '-')
    {
        memset(cat_info.file, 0, sizeof(cat_info.file));
        sprintf(cat_info.file, "%s", args0);
    }
}

void cat_info_init()
{
    cat_info.bflag = 0;
    cat_info.nflag = 0;
    cat_info.num = 0;
    memset(cat_info.file, 0, sizeof(cat_info.file));
}

void cat_info_reset()
{
    cat_info.bflag = 0;
    cat_info.nflag = 0;
    cat_info.num = 0;
    memset(cat_info.file, 0, sizeof(cat_info.file));
}
