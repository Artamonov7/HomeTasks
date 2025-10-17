#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct token_list{
    char *token;
    struct token_list *next;
};

void add_to_list(struct token_list **first, char *str)
{
    while(*first) {
        first = &(*first)->next;
    }
    *first = malloc(sizeof(struct token_list));
    (*first)->token = str;
    (*first)->next = NULL;
}

char *copy_str(char *str, int size)
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
    int size = 10, used = 0;
    char *str = malloc(size);
    int tmp;
    struct token_list *first = NULL;
    while ((tmp = getchar()) != EOF){
        if (tmp == ' ' || tmp == '\t' || tmp == '\n'){
            if (used > 0){
                add_to_list(&first, copy_str(str, used));
                used = 0;
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
        add_to_list(&first, copy_str(str, used));
    }
    free(str);
    return first;
}

void print_list(struct token_list *first)
{
    while (first != NULL){
        printf("%s ", first->token);
        first = first->next;
    }
    printf("\n");
}

void delete_elems(struct token_list **first, const char *str)
{
    struct token_list *tmp;
    while (*first){
        if (!strcmp((*first)->token, str)){
            tmp = *first;
            (*first) = (*first)->next;
            free(tmp->token);
            free(tmp);
        } else { 
            first = &(*first)->next;
        }
    }
}

char *save_last(struct token_list *first)
{
    if (!first)
        return NULL;
    while (first->next)
        first = first->next;
    return strdup(first->token);
}

void delete_list(struct token_list *first)
{
    struct token_list *tmp;
    while(first){
        tmp = first;
        first = first->next;
        free(tmp->token);
        free(tmp);
    }
}

int main(void)
{
    struct token_list *first = read_tokens();
    if (!first) 
        return 0;
    char *str = save_last(first);
    delete_elems(&first, str);
    add_to_list(&first, str);
    print_list(first);
    delete_list(first);
    return 0;
}
