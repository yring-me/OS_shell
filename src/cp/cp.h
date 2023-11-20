#ifndef CP_H
#define CP_H
#include "../parse/parse.h"
#include "../../include/main.h"
#include "../../include/shell.h"

int copy_file(char *src, char *dest);
int copy_folder(char *src, char *dest);
#endif