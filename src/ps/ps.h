#ifndef PS_H
#define PS_H
#include "../parse/parse.h"
#include "../../include/main.h"
#include "../../include/shell.h"
#include <ctype.h>

int isNum(const char *str);
void printProcessInfo(const char *pid);

#endif