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
        memset(arg, 0, sizeof(arg));
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

        // sprintf((char *)temp, "%s", (char *)arg[0]);
        if (strncmp((char *)arg[0], cmd_list[i].name, strlen(cmd_list[i].name)) == 0)
        {
            id = cmd_list->id;
        }
    }

    if (id == -1)
    {
        printf("cmd not found!\n");
        return;
    }
    syscall_handler_t handler = sys_table[id];
    handler(arg);
}
