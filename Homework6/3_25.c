#include <stdio.h>

int main()
{
    double a, x = 0;
    int n = 0;
    scanf("%lf", &a);
    while (x <= a){
        n++;
        x = x + 1.0/n;
    }
    printf("%d\n", n);
}
