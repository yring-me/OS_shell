#include "promat.h"

// 处理样式，具体的我想输出当前路径
char promat[512] = {0};
void set_promat()
{
    char temp[500] = {""};
    getcwd(temp, sizeof(temp));
    sprintf((char *)(promat), "%s", temp);
    // printf("%s", promat);
}
