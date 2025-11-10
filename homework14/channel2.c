#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

void run_cmds(int n, char **progs)
{
    int fd[2], res, i, in_fd = -1;
    for(i = 0; i < n; i++){
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
            if (in_fd != -1){
                dup2(in_fd, 0);
                close(in_fd);
            }
            if (i != n - 1)
                dup2(fd[1], 1);
            close(fd[1]);
            close(fd[0]);
            execlp(progs[i], progs[i], NULL);
            perror("execlp");
            exit(1);
        }
        if (in_fd != -1)
            close(in_fd);
        in_fd = fd[0];
        close(fd[1]);
    }
    close(in_fd);
    for (i = 0; i < n; i++)
        wait(NULL);
}

int main(int argc, char **argv)
{
    run_cmds(argc - 1, argv + 1);
    return 0;
}
