#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "token_list.h"

void add_to_list(struct token_list *tlist, char *str)
{
    if (tlist->size == tlist->used){
        tlist->size += 10;
        tlist->argv = realloc(tlist->argv, tlist->size*sizeof(char *));
    }
    tlist->argv[tlist->used] = str;
    tlist->used++;
}

static int getsym(void)
{
    static char buf[1024];
    static int rc = 0;
    static int given = 0;
    if (given == rc){
        rc = read(0, buf, sizeof(buf));
        if (rc < 1){
            return EOF;
        }
        given = 0;
    }
    return buf[given++];
}

static char *copy_str(char *str, int size)
{
    char *copy = malloc(size + 1);
    int i;
    for (i = 0; i < size; i++){
        copy[i] = str[i];
    }
    copy[size] = '\0';
    return copy;
}

struct token_list *read_tokens(void)
{
    int tmp, flag = 0;
    int size = 10, used = 0;
    char *str = malloc(size);
    struct token_list *tlist = malloc(sizeof(struct token_list));
    memset(tlist, 0, sizeof(struct token_list));
    while ((tmp = getsym()) != EOF){
        if (tmp != '|' && tmp != '&' && tmp != '>' && flag){
            add_to_list(tlist, copy_str(str, used));
            used = 0;
            flag = 0;
        }
        if (tmp == ' ' || tmp == '\t' || tmp == '\n' ||
            tmp == '|' || tmp == '&'  || tmp == ';'  ||
            tmp == '>' || tmp == '<'  || tmp == '('  || tmp == ')'){
            if (used > 0 && !flag){
                add_to_list(tlist, copy_str(str, used));
                used = 0;
            }
            switch(tmp){
                case '(': case ')': case ';': case '<':
                    str[used++] = tmp;
                    add_to_list(tlist, copy_str(str, used));
                    used = 0;
                    break;
                case '|': case '&': case '>': 
                    if (flag){
                        if (str[used - 1] == tmp){
                            str[used++] = tmp;
                            flag = 0;
                        }
                        add_to_list(tlist, copy_str(str, used));
                        used = 0;
                        if (flag) {
                            str[used++] = tmp;
                        }
                    }
                    else {
                        flag = 1;
                        str[used++] = tmp;
                    }
            }
            continue;
        }
        if (used == size){
            size = size*2;
            char *str1 = malloc(size);
            int i;
            for(i = 0; i < used; i++){
                str1[i] = str[i];
            }
            free(str);
            str = str1;
        }
        str[used++] = tmp;
    }
    if (used > 0){
        add_to_list(tlist, copy_str(str, used));
    }
    free(str);
    add_to_list(tlist, NULL);
    tlist->used--;
    return tlist;
}

void sort_list(struct token_list *tlist)
{
    char **m = tlist->argv;
    int n = tlist->used;
    int i, j;
    char *tmp;
    for (i = 0; i < n; i++){
        for (j = 0; j < n - 1; j++){
            if (strcmp(m[j],m[j + 1]) > 0){
                tmp = m[j];
                m[j] = m[j + 1];
                m[j + 1] = tmp;
            }
        }
    }
}

void print_list(struct token_list *tlist)
{
    int i;
    printf("%d\n", tlist->used);
    for (i = 0; i < tlist->used; i++)
        printf("%s\n", tlist->argv[i]);
}

void delete_list(struct token_list *tlist)
{
    int i;
    for (i = 0; i < tlist->used; i++)
        free(tlist->argv[i]);
    free(tlist->argv);
    free(tlist);
}

