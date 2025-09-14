
#include <stdio.h>

int main()
{
    int n;
    scanf("%d", &n);
    int i, j, k;
    for (i = 0; i < n; i++){
        for (j = 0; j < i + 2; j++){
            for (k = 0; k < n - j; k++){
                printf(" ");
            }
            for (k = 0; k < j*2 + 1; k++){
                printf("*");
            }
            printf("\n");
        }
    }
    return 0;
}
