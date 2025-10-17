#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct count_tree{
    int elem;
    struct count_tree *right;
    struct count_tree *left;
};

void add_to_tree(struct count_tree **root, int n)
{
    if (!*root){
        *root = malloc(sizeof(struct count_tree));
        (*root)->elem = n;
        (*root)->right = NULL;
        (*root)->left = NULL;
        return;
    }

    if (n == (*root)->elem)
        return;

    if (n > (*root)->elem)
        add_to_tree(&(*root)->right, n);
    else
        add_to_tree(&(*root)->left, n);
}

int search_elem(struct count_tree *root, int n)
{
    if (!root)
        return 0;
    if (n == root->elem)
        return 1;

    if (n > root->elem)
        return search_elem(root->right, n);
    else
        return search_elem(root->left, n);
}

void delete_from_tree(struct count_tree **root, int n)
{
    if (!*root)
        return;
    if ((*root)->elem == n){
        if (!(*root)->left && !(*root)->right){
            struct count_tree *tmp = *root;
            *root = NULL;
            free(tmp);
            return;
        }
        if (!(*root)->left){
            struct count_tree *tmp = *root;
            (*root) = (*root)->right;
            free(tmp);
            return;
        }
        if (!(*root)->right){
            struct count_tree *tmp = *root;
            (*root) = (*root)->left;
            free(tmp);
            return;
        }
        struct count_tree *tmp = *root;
        tmp = tmp->right;
        if (!tmp->left){
            (*root)->right = tmp->right;
            (*root)->elem = tmp->elem;
            free(tmp);
            return;
        }
        struct count_tree *tmp2;
        while (tmp->left){
            tmp2 = tmp;
            tmp = tmp->left;
        }
        (*root)->elem = tmp->elem;
        tmp2->left = tmp->left;
        free(tmp);
        return;
    }
    delete_from_tree((*root)->elem < n ? &(*root)->right : &(*root)->left, n);
}

void free_tree(struct count_tree *root)
{
    if(!root)
        return;
    free_tree(root->right);
    free_tree(root->left);
    free(root);   
}

int main(void)
{
    int c;
    int count;
    struct count_tree *root = NULL;
    while ((c = getchar()) != EOF){
        if (c == ' ' || c == '\t' || c == '\n')
            continue;
        scanf("%d", &count);
        switch (c){
        case '+':
            add_to_tree(&root, count);
            break;
        case '-':
            delete_from_tree(&root, count);
            break;
        case '?':
            printf("\n%d %s\n", count, search_elem(root, count) ? "yes" : "no");
        }
    }
    free_tree(root);
    return 0;
}

