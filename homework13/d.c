#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void run_cmds(int n, char **progs)
{
    int i, res;
    for (i = 0; i < n; i++){
        res = fork();
        if (res < 0){
            perror("fork");
            exit(1);
        }
        if (!res){
            execlp(progs[i], progs[i], NULL);
            perror("execlp");
            exit(1);
        }
    }
    for (i = 0; i < n; i++){
        wait(NULL);
    }
}

int main(int argc, char **argv)
{
    if (argc < 2){
        fprintf(stderr, "no comands entered\n");
        return 1;
    }
    run_cmds(argc - 1, argv + 1);
    return 0;
}

