#ifndef TREE_H
#define TREE_H

#include "../parse/parse.h"
#include "../../include/main.h"
#include "../../include/shell.h"

#include <string.h>
int tree(char *direntName, int level, int is_a, int isLast[]);

extern int tree_file;
extern int tree_folder;
#endif