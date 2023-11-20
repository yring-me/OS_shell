#include "mv.h"

void syscall_mv(char *src, char *dest)
{
    // 移动就简单的理解为 复制 然后删除源文件
    if (syscall_cp(src, dest) == -1)
    {
        return;
    }
    syscall_rm(src, "-r");
}