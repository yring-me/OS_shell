#ifndef VIM_H
#define VIM_H

#include "../parse/parse.h"
#include "../../include/main.h"
#include "../../include/shell.h"

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void syscall_vim(char *args0);

#endif