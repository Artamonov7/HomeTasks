#include "token_list.h"

int main(void)
{
    struct token_list *tlist = read_tokens();
    print_list(tlist);
    sort_list(tlist);
    print_list(tlist);
    delete_list(tlist);
    return 0;
}

