#ifndef MAIN_H
#define MAIN_H

char promat[512] = {};

extern char history[100][128];
extern int history_total;
extern void parse_buffer(char *);
extern void syscall_history();
extern void dispatcher();
extern void set_promat();
#endif