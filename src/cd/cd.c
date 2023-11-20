#include "../parse/parse.h"
#include "../../include/main.h"
#include "../../include/shell.h"

void syscall_cd(char *path)
{
    if (strcmp(path, "~") == 0)
    {
        path = getenv("HOME");
    }

    if (chdir(path) == 0)
    {
        set_promat();
        printf("%s\n", redir_info.out_file_name);
        return;
    }
    printf("\x1b[31mError:dirctory no exist\x1b[0m\n");
}