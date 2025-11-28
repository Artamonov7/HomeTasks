#ifndef TOKEN_LIST_H_SENTRY
#define TOKEN_LIST_H_SENTRY

struct token_list{
    char **argv;
    int size;
    int used;
};

int is_input_finished(void);
void add_to_list(struct token_list *tlist, char *str);
struct token_list *read_tokens(void);
void sort_list(struct token_list *tlist);
void print_list(struct token_list *tlist);
void delete_list(struct token_list *tlist);

#endif

