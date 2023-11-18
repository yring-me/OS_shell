#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "main.h"
int main()
{
    int size = 1024;
    char *buffer = (char *)malloc(size);
    redir_init();
    ls_info_init();
    set_promat();
    while (1)
    {
        memset(buffer, 0, 1024);
        printf(" \x1b[36m  \x1b[0m");
        printf("\x1b[36m%s\x1b[0m\n", promat);
        printf("\x1b[36m>>>\x1b[0m");

        fgets(buffer, size, stdin);
        if (buffer[0] == '\n')
        {
            printf("\n");
            continue;
        }

        parse_buffer(buffer);

        if (check_cmd() == -1)
            continue;

        parse_redir();

        dispatcher();

        clean_buffer();

        // break;

        printf("\n");
    }
}