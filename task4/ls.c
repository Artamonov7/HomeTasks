#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>
#include <pwd.h>
#include <grp.h>
#include <string.h>

const char *get_mode_str(mode_t mode)
{
    static char buf[11];
    const char *rwx = "rwx";
    if (S_ISREG(mode)) 
        buf[0] = '-';
    else if (S_ISDIR(mode))
        buf[0] = 'd';
    else if (S_ISCHR(mode))
        buf[0] = 'c';
    else if (S_ISBLK(mode))
        buf[0] = 'b';
    else if (S_ISFIFO(mode))
        buf[0] = 'p';
    else if (S_ISLNK(mode))
        buf[0] = 'l';
    else if (S_ISSOCK(mode))
        buf[0] = 's';
    else 
        buf[0] = '?';
    int perms = mode & 0777;
    int i, mask = 0400;
    for (i = 0; i < 9; i++){
        if (perms & mask)
            buf[i + 1] = rwx[i % 3];
        else 
            buf[i + 1] = '-';
        mask >>= 1;
    }
    buf[10] = '\0';
    return buf;
}

void print_cur_path(void)
{
    char *path = malloc(PATH_MAX);
    if (!getcwd(path, PATH_MAX))
        perror("getcwd");
    else 
        printf("%s\n", path);
    free(path);
}

void list_dir(int r, int l, int g)
{
    if (r)
        print_cur_path();
    DIR *d = opendir(".");
    if (d == NULL){
        perror("opendir");
        return;
    }
    struct dirent *dent;
    struct stat st;
    while ((dent = readdir(d)) != NULL){
        if (stat(dent->d_name, &st) < 0){
            perror("stat");
            return;
        }
        if (l){
            struct passwd *pd;
            pd = getpwuid(st.st_uid);
            printf("%s ", get_mode_str(st.st_mode));
            
            if (pd)
                printf("%s\t", pd->pw_name);
            else
                printf("%d\t", st.st_uid);
        }
        if (g) {
            struct group *gr;
            gr = getgrgid(st.st_gid);
            if (gr)
                printf("%s\t", gr->gr_name);
            else
                printf("%d\t", st.st_gid);
        }
        if (l)    
            printf("%lu\t", st.st_size);
        printf("%s\n", dent->d_name);
    }
    closedir(d);
    if (r){
        d = opendir(".");
        if (d == NULL){
            perror("opendir");
            return;
        }
        while ((dent = readdir(d)) != NULL){
            if (stat(dent->d_name, &st) < 0){
                perror("stat");
                return;
            }
            if (S_ISDIR(st.st_mode)){
                if (!strcmp(dent->d_name, ".") || !strcmp(dent->d_name, ".."))
                    continue;
                if (chdir(dent->d_name) < 0){
                    perror("chdir");
                    continue;
                }
                list_dir(r, l, g);
                chdir("..");
            }
        }
        closedir(d);
    }
}

int parse_options(int argc, char **argv, int *r, int *l, int *g)
{
    int opt;
    *r = 0; *l = 0; *g = 0;
    while ((opt = getopt(argc, argv, ":Rlg")) != -1) {
        switch (opt) {
        case 'R':
            *r = 1;
            break;
        case 'l':
            *l = 1;
            break;
        case 'g':
            *g = 1;
            break;
        case '?':
            fprintf(stderr, "Usage: %s [-R] [-l] [-g]\n", argv[0]);
            return -1;
        }
    }
    return 0;
}

int main(int argc, char **argv)
{
    int r, l, g;
    if (parse_options(argc, argv, &r, &l, &g) < 0)
        exit(1);
    list_dir(r, l, g);
    return 0;
}
