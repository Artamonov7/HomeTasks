#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


int main(int argc, char **argv)
{
    if (argc < 3){
        printf("too few arguments\n");
        return -1;
    }
    int f1 = open(argv[1], O_RDONLY);
    if (f1 < 0){
        perror("open");
        return -1;
    }
    int f2 = open(argv[2], O_CREAT | O_WRONLY | O_TRUNC, 0777);
    if (f2 < 0){
        perror("open");
        return -1;
    }
    char buf[1024];
    int used = 0, given = 0;
    while ((used = read(f1, buf, sizeof(char)*1024)) > 0){
        while (given != used)
            given += write(f2, buf + given, sizeof(char)*(used - given));
        given = 0;
    }

    close(f1);
    close(f2);
    return 0;
}   
