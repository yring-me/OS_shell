#ifndef PARSE_H
#define PARSE_H

#include <stdio.h>
#include <string.h>
#include "parse.h"
#include "../../include/main.h"
#include "../../include/shell.h"

extern char args[10][128];

void dispatcher();

void parse_buffer(char *);
void set_promat();
int check_cmd();

#endif