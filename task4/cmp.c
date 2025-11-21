#include <stdio.h>

int main(int argc, char **argv)
{
    if (argc < 3){
        printf("too few arguments\n");
        return -1;
    }
    FILE *f1 = fopen(argv[1], "r");
    if (f1 == NULL){
        perror("open");
        return -1;
    }
    FILE *f2 = fopen(argv[2], "r");
    if (f1 == NULL){
        perror("open");
        return -1;
    }
    int c1, c2;
    int line = 1, ch = 1;
    while (((c1 = fgetc(f1)) != EOF) && ((c2 = fgetc(f2)) != EOF)){
        if (c1 != c2){
            break;
        }
        if (c1 == '\n'){
            line++;
            ch = 1;
        }
        else 
            ch++;
    }
    if (c1 == EOF)
        c2 = fgetc(f2);
    if ((c1 == EOF) && (c2 == EOF)){
        printf("there is no differences\n");
    }
    else
        printf("file %s differs from %s: line %d char %d\n", argv[1], argv[2], line, ch);
    fclose(f1);
    fclose(f2);

}
