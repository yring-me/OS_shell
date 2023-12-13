#include "touch.h"

void syscall_touch(char *args0)
{
    if (args0 == NULL)
    {
        printf("Use touch <file> to create file.");
        return;
    }
    const char *filename = args0;
    FILE *file = fopen(filename, "a");
    if (file == NULL)
    {
        printf("Error touch file");
        return;
    }
    fclose(file);

    struct utimbuf new_times;
    new_times.actime = new_times.modtime = time(NULL);
    if (utime(filename, &new_times) == -1)
    {
        printf("Error updating file timestamp");
        return;
    }

    // printf("File '%s' created successfully.\n", filename);
}