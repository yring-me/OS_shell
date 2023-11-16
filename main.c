#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "main.h"
int main()
{
    int size = 1024;
    char *buffer = (char *)malloc(size);

    set_promat();
    while (1)
    {
        memset(buffer, 0, 1024);
        printf("\x1b[36m%s\x1b[0m\n", promat);
        printf("\x1b[36m>>>\x1b[0m");

        fgets(buffer, size, stdin);

        parse_buffer(buffer);
        dispatcher();
        printf("\n");
    }
}