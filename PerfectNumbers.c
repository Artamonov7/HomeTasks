
#include <stdio.h>

int main()
{
    int n, sum;
    int i, j;
    scanf("%d", &n);
    for (i = 1; i < n; i++){
        sum = 0;
        for (j = 1; j < i; j++){
            if (i % j == 0){
                sum += j;
            }
        }
        if (sum == i){
            printf("%d ", i);
        }
    }
    printf("\n");
    return 0;
}
