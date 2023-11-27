#ifndef HEAD_H
#define HEAD_H

#include "../parse/parse.h"
#include "../../include/main.h"
#include "../../include/shell.h"

#include <string.h>
void print_lines(FILE *file, int n) ;
int head();
void head_info_init();
void head_info_check(char *args0, char *args1);
#endif