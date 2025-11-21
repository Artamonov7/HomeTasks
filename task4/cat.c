#include <stdio.h>
#include <unistd.h>

int parse_options(int argc, char **argv, int *n)
{
    int opt;
    *n = 0;
    while ((opt = getopt(argc, argv, ":n")) != -1) {
        switch (opt) {
        case 'n':
            *n = 1;
            break;
        case '?':
            fprintf(stderr, "Usage: %s [-n]\n", argv[0]);
            return -1;
        }
    }
    return 0;
}

int main(int argc, char **argv)
{
    if (argc < 2){
        printf("too few arguments");
        return -1;
    }
    int i, line = 1, c, n;
    int flag = 0;
    FILE *f;
    parse_options(argc, argv, &n);
    for (i = 1 + n; i < argc; i++){
        f = fopen(argv[i], "r");
        if (f == NULL){
            perror("fopen");
            continue;
        }
        if (n == 1)
            printf("%d ", line++);
        while ((c = fgetc(f)) != EOF){
            if (flag && n){
                printf("%d ", line++);
                flag = 0;
            }
            printf("%c", c);
            if (c == '\n')
                flag = 1;
        }
        fclose(f);
    }
}
