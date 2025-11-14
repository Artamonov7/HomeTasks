#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>

int main(void)
{
    char *path = malloc(PATH_MAX);
    if (!getcwd(path, PATH_MAX)){
        perror("getcwd");
        free(path);
        exit(1);
    }
    printf("%s\n", path);
    free(path);
    return 0;
}
