// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <fcntl.h>
// #include <errno.h>
// #include <string.h>
// #include <dirent.h>

// // 自定义错误处理函数
// void my_error(const char *strerr)
// {
//     perror(strerr);
//     exit(1);
// }

// // 输出目录结构
// void PrintDirentStruct(char direntName[], int level)
// {
//     // 定义一个目录流指针
//     DIR *p_dir = NULL;

//     // 定义一个目录结构体指针
//     struct dirent *p_dirent = NULL;

//     // 打开目录，返回一个目录流指针指向第一个目录项
//     p_dir = opendir(direntName);
//     if (p_dir == NULL)
//     {
//         my_error("opendir error");
//     }

//     // 循环读取每个目录项, 当返回NULL时读取完毕
//     while ((p_dirent = readdir(p_dir)) != NULL)
//     {
//         // 备份之前的目录名
//         char *backupDirName = NULL;

//         if (p_dirent->d_name[0] == '.')
//         {
//             continue;
//         }

//         int i;
//         for (i = 0; i < level; i++)
//         {
//             printf("|");
//             printf("     ");
//         }
//         printf("|--- ");
//         printf("%s\n", p_dirent->d_name);

//         // 如果目录项仍是一个目录的话，进入目录
//         if (p_dirent->d_type == DT_DIR)
//         {
//             // 当前目录长度
//             int curDirentNameLen = strlen(direntName) + 1;

//             // 备份
//             backupDirName = (char *)malloc(curDirentNameLen);
//             memset(backupDirName, 0, curDirentNameLen);
//             memcpy(backupDirName, direntName, curDirentNameLen);

//             strcat(direntName, "/");
//             strcat(direntName, p_dirent->d_name);
//             PrintDirentStruct(direntName, level + 1);

//             // 恢复之前的目录名
//             memcpy(direntName, backupDirName, curDirentNameLen);
//             free(backupDirName);
//             backupDirName = NULL;
//         }
//     }

//     closedir(p_dir);
// }

// int main(int argc, char *argu[])
// {
//     // 目录名
//     char direntName[256];
//     memset(direntName, 0, sizeof(direntName));

//     if (argc == 1)
//     {
//         direntName[0] = '.';
//     }
//     else if (argc == 2)
//     {
//         strcat(direntName, argu[1]);
//     }
//     else
//     {
//         my_error("argument error");
//     }

//     // 输出目录结构
//     printf("%s\n", direntName);
//     PrintDirentStruct(direntName, 0);
//     return 0;
// }
