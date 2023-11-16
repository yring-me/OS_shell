#include <stdio.h>
#include <string.h>
#include "shell.h"

char arg[10][128] = {0};
char history[100][128] = {0};
int history_total = -1;

void dispatcher();

void parse_buffer(char *buffer_in)
{
    char *token = 0;
    int index = 0;

    buffer_in[strlen(buffer_in) - 1] = 0;

    sprintf((char *)history[++history_total], "%s", buffer_in);

    token = strtok(buffer_in, " ");

    while (token != NULL)
    {

        sprintf((char *)arg[index++], "%s", token);

        token = strtok(NULL, " ");
    }
}

void dispatcher()
{
    int id = -1;

    for (int i = 0; i < sizeof(cmd_list) / sizeof(shell); i++)
    {
        char temp[128];

        // printf("%s-%s\n", arg[0], cmd_list[i].name);
        // printf("%d",strcmp((char *)arg[0], cmd_list[i].name))
        if (strcmp((char *)arg[0], cmd_list[i].name) == 0)
        {
            id = cmd_list[i].id;
            break;
        }
    }

    if (id == -1)
    {
        printf("\x1b[31mcmd not found!\x1b[0m\n");
        return;
    }

    syscall_handler_t handler = sys_table[id];
    handler(arg[1], arg[2], arg[3], arg[4]);
    memset(arg, 0, sizeof(arg));
}
