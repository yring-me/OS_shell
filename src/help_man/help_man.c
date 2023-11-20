#include "help_man.h"

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