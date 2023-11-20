#include "ps.h"

int isNum(const char *str) {
    while (*str) {
        if (!isdigit(*str))
        return 0;
        str++;
    }
    return 1;
}


void syscall_ps(){
    DIR *dir = opendir("/proc");
    if (dir == NULL) {
        perror("Error opening directory");
        exit(EXIT_FAILURE);
    }

    printf("%-10s %-5s   %-5s%-5s  %-15s %-15s %-5s %-5s %-10s %-10s %-20s\n",
           "USER", "PID", "%CPU", "%MEM", "VSZ(Bytes)", "RSS(Pages)", "TTY", "STAT", "START(s)", "TIME(s)", "COMMAND");

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (isNum(entry->d_name)) {
            printProcessInfo(entry->d_name);
        }
    }

    closedir(dir);
}


// 打印进程信息
void printProcessInfo(const char *pid) {
    char path[50];
    snprintf(path, sizeof(path), "/proc/%s/stat", pid);

    FILE *file = fopen(path, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char comm[256];
    char state;
    int ppid, pgrp, session, tty_nr, tpgid;
    unsigned int flags;
    unsigned long minflt, cminflt, majflt, cmajflt, utime, stime;
    long cutime, cstime, priority, nice , num_threads, itrealvalue, starttime, vsize, rss;


    fscanf(file, "%*d %255s %c %d %d %d %d %d %u %lu %lu %lu %lu %lu %lu %ld %ld %ld %ld %ld %ld %ld %ld %ld",
    comm, &state, &ppid, &pgrp, &session, &tty_nr, &tpgid, &flags,
    &minflt, &cminflt, &majflt, &cmajflt, &utime, &stime, &cutime, &cstime,
    &priority, &nice , &num_threads, &itrealvalue, &starttime, &vsize, &rss);

    fclose(file);

    // 获取用户名
    char username[256];
    FILE *statusFile = fopen("/proc/self/status", "r");
    if (statusFile == NULL) {
        perror("Error opening status file");
        exit(EXIT_FAILURE);
    }

    while (fgets(path, sizeof(path), statusFile) != NULL) {
        if (strstr(path, "Uid:") != NULL) {
            sscanf(path, "%*s %s", username);
            break;
        }
    }
    fclose(statusFile);

    // CPU使用率
    double cputime = (double)(utime + stime) / sysconf(_SC_CLK_TCK);
    double current = (double)(clock() / CLOCKS_PER_SEC);
    double start = (double)starttime;
    double total = start - current;
    double cpu_use = cputime / total;

    // 获取物理内存大小，计算内存使用率  
    long mem = rss * 4;
    FILE *meminfo_file = fopen("/proc/meminfo", "r");
    if (meminfo_file == NULL) {
        perror("Error opening /proc/meminfo");
        exit(EXIT_FAILURE);
    }
    unsigned long total_memory = 0;
    while (1) {
        char line[256];
        if (fgets(line, sizeof(line), meminfo_file) == NULL) {
            break;
        }
        if (sscanf(line, "MemTotal: %lu kB", &total_memory) == 1) {
            break;
        }
    }
    fclose(meminfo_file);

    double mem_use = (double)(rss * 4) / (double)total_memory;

    //"USER", "PID", "%CPU", "%MEM", "VSZ", "RSS", "TTY", "STAT", "START", "TIME", "COMMAND"
    printf("%-10s %-5s %6.2f %-6.2f %-15lu %-15lu %-5d %-5c %-10.0f %-10.0f %-20s\n",
           username, pid, cpu_use, mem_use, vsize, rss, tty_nr, state, start, total, comm);
}
