#include "pwd.h"
int syscall_pwd()
{
    char temp[500] = {0};
    getcwd(temp, sizeof(temp));
    printf("%s\n", temp);
    return 1;
}
