#include "../parse/parse.h"
#include "../../include/main.h"
#include "../../include/shell.h"

int syscall_clear()
{
    system("clear");
    return 1;
}
