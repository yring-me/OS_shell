#include "function.h"

int to_uppercase(char *buffer)
{
    int index = 0;
    while (buffer[index] != 0)
    {
        if (buffer[index] >= 'a' && buffer[index] <= 'z')
            buffer[index] -= 32;
        index++;
    }

    printf("%s\n", buffer);
    return 1;
}

int to_lowercase(char *buffer)
{
    int index = 0;
    while (buffer[index] != 0)
    {
        if (buffer[index] >= 'A' && buffer[index] <= 'Z')
            buffer[index] += 32;
        index++;
    }

    printf("%s\n", buffer);
    return 1;
}