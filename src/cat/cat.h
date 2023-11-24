#ifndef CAT_H
#define CAT_H
#include "../parse/parse.h"
#include "../../include/main.h"
#include "../../include/shell.h"
extern struct cat_info
{
    int nflag;
    int bflag;
    int num;
    char file[256];
} cat_info;

int cat();
void cat_info_check(char *args0, const char *args1, const char *args2);
void cat_info_init();
void cat_info_reset();
#endif
