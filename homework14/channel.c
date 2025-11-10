#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

void run_cmds(int n, char **progs)
{
    int fd[2], res;
    res = pipe(fd);
    if (res < 0){
        perror("pipe");
        exit(1);
    }
    res = fork();
    if (res < 0){
        perror("fork");
        exit(1);
    }
    if (res == 0){
        dup2(fd[1], 1);
        close(fd[1]);
        close(fd[0]);
        execlp(progs[0], progs[0], NULL);
        perror("execlp");
        exit(1);
    }
    
    res = fork();
    if (res < 0){
        perror("fork");
        exit(1);
    }
    if (res == 0){
        dup2(fd[0], 0);
        close(fd[1]);
        close(fd[0]);
        execlp(progs[1], progs[1], NULL);
        perror("execlp");
        exit(1);
    }
    close(fd[0]);
    close(fd[1]);
    wait(NULL);
    wait(NULL);
}

int main(int argc, char **argv)
{
    run_cmds(argc - 1, argv + 1);
    return 0;
}
