#include "help_man.h"

// 所有命令
void syscall_help()
{
    printf("help                                        show all the usages of cmds\n");
    printf("man             [cmd]                       show the usage of the given cmd\n");
    printf("cat             <file_name>                 concatenate and print files\n");
    printf("cd              <path>                      change the current path\n");
    printf("clear                                       clean the screen\n");
    printf("cp              <src_file> <dest_file>      copy the src_file to dest_file\n");
    printf("history                                     show cmd used before\n");
    printf("ls              [-l,[-a]] [path]            list the files in the given path\n");
    printf("mkdir           [-p] [-m] <file_name>       make a dirctory\n");
    printf("mv              <src_file> <dest_file>      move the src_file to dest_file  \n");
    printf("ps                                          show process info\n");
    printf("rm              [-r] <src_file>             remove directory entries\n");
    printf("tree            <path>                      list contents of directories in a tree-like format. \n");
    printf("tail            [-n] <file>                 show the last n lines of the file\n");
    printf("head            [-n] <file>                 show the first n lines of the file\n");
    printf("touch           <file>                      create a file\n");
    printf("vim             <file>                      a powerful editor\n");
    printf("to_uppercase    <string>                    capitalize the string\n");
    printf("to_lowercase    <string>                    change the string to lower-case\n");
    printf("<               <cmd1> < <file>             use the file contents as the input of the cmd \n");
    printf(">               <cmd1> > <file>             rewrite the output of thd cmd to file\n");
    printf(">>              <cmd1> >> <file>            append the output of thd cmd to file\n");
    printf("|               <cmd1> | <cmd2>             use the output of cmd1 as the input of cmd2\n");
    printf("&               <cmd> [args]... &           run the process in the background and run the outer cmd\n");
}

// 指定命令
void syscall_man(char *cmd)
{
    if (strcmp("cat", cmd) == 0)
        printf("cat         <file_name>                 concatenate and print files\n");
    else if (strcmp("cd", cmd) == 0)
        printf("cd          <path>                      change the current path\n");
    else if (strcmp("clear", cmd) == 0)
        printf("clear       clean the screen\n");
    else if (strcmp("cp", cmd) == 0)
        printf("cp          <src_file> <dest_file>      copy the src_file to dest_file\n");
    else if (strcmp("history", cmd) == 0)
        printf("history     show cmd used before\n");
    else if (strcmp("ls", cmd) == 0)
        printf("ls          [-l,[-a]] [path]            list the files in the given path\n");
    else if (strcmp("mkdir", cmd) == 0)
        printf("mkdir       [-p] [-m] <file_name>       make a dirctory\n");
    else if (strcmp("mv", cmd) == 0)
        printf("mv          <src_file> <dest_file>      move the src_file to dest_file  \n");
    else if (strcmp("ps", cmd) == 0)
        printf("ps          show process info\n");
    else if (strcmp("rm", cmd) == 0)
        printf("rm          [-r] <src_file>             remove directory entries\n");
    else if (strcmp("tree", cmd) == 0)
        printf("tree        <path>                      list contents of directories in a tree-like format. \n");
    else if (strcmp("to_uppercase", cmd) == 0)
        printf("to_uppercase     <string>               capitalize the string\n");
    else if (strcmp("to_lowercase", cmd) == 0)
        printf("to_lowercase     <string>               change the string to lower-case\n");
    else if (strcmp("<", cmd) == 0)
        printf("<               <cmd1> < <file>             use the file contents as the input of the cmd \n");
    else if (strcmp(">", cmd) == 0)
        printf(">           <cmd1> > <file>             rewrite the output of thd cmd to file\n");
    else if (strcmp(">>", cmd) == 0)
        printf(">>          <cmd1> >> <file>            append the output of thd cmd to file\n");
    else if (strcmp("|", cmd) == 0)
        printf("|           <cmd1> | <cmd2>             use the output of cmd1 as the input of cmd2\n");
    else if (strcmp("&", cmd) == 0)
        printf("&           <cmd> [args]... &           run the process in the background and run the outer cmd\n");
    else if (strcmp("tail", cmd) == 0)
        printf("tail        [-n] <file>                 show the last n lines of the file\n");
    else if (strcmp("head", cmd) == 0)
        printf("head        [-n] <file>                 show the first n lines of the file\n");
    else if (strcmp("touch", cmd) == 0)
        printf("touch       <file>                      create a file\n");
    else if (strcmp("vim", cmd) == 0)
        printf("vim         <file>                      a powerful editor\n");
    else if (strlen(cmd) == 0)
        syscall_help();
    else
    {
        printf("%s%s%s%s\n", COLOR_YELLOW, "Waring Invalid Parameter ", cmd, COLOR_RESET);
        syscall_help();
    }
}