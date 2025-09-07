#include <stdio.h>

void bubble_sort(int *m, int n)
{
    int i, j, tmp;
    for (i = 0; i < n; i++){
        for (j = 0; j < n - 1; j++){
            if (m[j] > m[j + 1]){
                tmp = m[j];
                m[j] = m[j + 1];
                m[j + 1] = tmp;
            }
        }
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
    bubble_sort(m, n);
    write_array(m, n);
}

