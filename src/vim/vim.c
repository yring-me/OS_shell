#include "vim.h"
void syscall_vim(char *args0) {
    const char *filename = args0;

    // 创建子进程
    pid_t pid = fork();
    if (pid == -1) {
        perror("Fork failed");
        return;
    } 
    else if (pid == 0) {  // 子进程
        // 使用 execlp 调用 vim
        execlp("vim", "vim", filename, (char *)NULL);

        // 如果 execlp 返回，说明发生了错误
        perror("Exec failed");
        exit(EXIT_FAILURE);
    } 
    else {  // 父进程
        // 等待子进程结束
        wait(NULL);
    }
}