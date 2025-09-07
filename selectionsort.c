#include <stdio.h>

void selection_sort(int *m, int n)
{
    int i, j, tmp, k;
    for (i = 0; i < n; i++){
        k = i;
        tmp = m[i];
        for (j = i; j < n; j++){
            if (m[j] < tmp){
                tmp = m[j];
                k = j;
            }
        }
        m[k] = m[i];
        m[i] = tmp;
    }
}

void read_array(int *m, int n)
{
    int i;
    for (i = 0; i < n; i++){
        scanf("%d", &m[i]);
    }
}

void write_array(int *m, int n)
{
    int i;
    for (i = 0; i < n; i++){
        printf("%d ", m[i]);
    }
    printf("\n");
}

int main()
{
    int n;
    scanf("%d", &n);
    int m[n];
    read_array(m, n);
    write_array(m, n);
    selection_sort(m, n);
    write_array(m, n);
}


