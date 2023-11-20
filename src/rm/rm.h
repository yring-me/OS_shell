#ifndef RM_H
#define RM_H
#include "../parse/parse.h"
#include "../../include/main.h"
#include "../../include/shell.h"

void remove_dir(char *path);
void get_file_path(const char *path, const char *filename, char *filepath);
#endif