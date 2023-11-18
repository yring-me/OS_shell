#ifndef MAIN_H
#define MAIN_H

char promat[512] = {};

extern char history[100][128];
extern int history_total;
extern void parse_buffer(char *);
extern void syscall_history();
extern void dispatcher();
extern void set_promat();
extern void redir_init();
extern void parse_redir();
extern void fd_reset();
extern void clean_buffer();
extern int check_cmd();
extern void ls_info_init();

#endif