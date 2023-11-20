#include "pipe.h"

void syscall_pipe(char *cmd1, char *cmd2)
{
    int fd[2];
    pid_t pid1, pid2;

    if (pipe(fd) < 0)
    {
        perror("pipe error");
        exit(1);
    }

    if ((pid1 = fork()) < 0)
    {
        perror("fork error");
        exit(1);
    }
    else if (pid1 == 0)
    {
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        system(cmd1);
        exit(0);
    }

    if ((pid2 = fork()) < 0)
    {
        perror("fork error");
        exit(1);
    }
    else if (pid2 == 0)
    {
        close(fd[1]);
        dup2(fd[0], STDIN_FILENO);
        system(cmd2);
        exit(0);
    }

    close(fd[0]);
    close(fd[1]);
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
}
