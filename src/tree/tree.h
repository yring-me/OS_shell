#ifndef TREE_H
#define TREE_H

#include "../parse/parse.h"
#include "../../include/main.h"
#include "../../include/shell.h"

#include <string.h>
void tree(char *direntName, int level, int is_a, int isLast[]);
int insensitiveCompare(const void *a, const void *b);

extern int tree_file;
extern int tree_folder;
#endif