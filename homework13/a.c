#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>


#define FILE_WRITE 0
#define FILE_WRITE_APPEND 1
#define FILE_READ 2


void run_cmd(const char *prog, const char *fi, int flag)
{
    int res = fork();
    if (res == -1){
        perror("fork");
        return;
    }
    if (!res){
        if (flag == FILE_WRITE || flag == FILE_WRITE_APPEND){
            int flags = O_WRONLY | O_CREAT;
            if (flag == FILE_WRITE)
                flags |= O_TRUNC;
            else 
                flags |= O_APPEND;
            int fd = open(fi, flags, 0644);
            if (fd < 0){
                perror("open");
                exit(1);
            }
            res = dup2(fd, 1); 
            if (res < 0){
                perror("dup2");
                exit(1);
            }
            close(fd);
        }
        else if (flag == FILE_READ){
            int fd = open(fi, O_RDONLY);
            if (fd < 0){
                perror("open");
                exit(1);
            }
            res = dup2(fd, 0);
            if (res < 0){
                perror("dup2");
                exit(1);
            }
            close(fd);
        }
        else {
            fprintf(stderr, "wrong flag\n");
            exit(1);
        }
        execlp(prog, prog, NULL);
        perror("execlp");
        exit(1);
    }
    wait(NULL);
}

int main(int argc, char **argv)
{
    if (argc != 4){
        fprintf(stderr, "invalid args\n");
        return 1;
    }
    run_cmd(argv[1], argv[2], atoi(argv[3]));
    return 0;
}

