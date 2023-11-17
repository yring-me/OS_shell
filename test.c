// // #include <unistd.h>
// // // #include <iostream>
// #include <fcntl.h>
// #include <stdio.h>
// #include <errno.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <sys/types.h>
// #include <sys/stat.h>
// // using namespace std;

// int main()
// {

//     FILE *src = fopen("1.txt", "r");
//     // FILE *dest = fopen("2.txt", "a+");
//     char buffer[1024] = {0};
//     int buffer_index = 0;
//     while (1)
//     {
//         char ch = fgetc(src);

//         if (ch == EOF) // EOF 本来表示文件末尾，意味着读取结束
//             break;     // 到文件末尾则退出while循环
//         if (ch == '\x1b')
//         {
//             fseek(src, 2, SEEK_CUR);
//             // printf("%c", fgetc(src));
//             // exit(0);
//             if (fgetc(src) == 'm')
//                 continue;
//             else
//                 fseek(src, 1, SEEK_CUR);
//             continue;
//         }

//         buffer[buffer_index++] = ch;
//     }
//     // printf("\x1b[31m%s\n", "   ");

//     src = fopen("1.txt", "w+");
//     fwrite(buffer, 1024, 1, src);
//     printf("%s\n", buffer);
// }
