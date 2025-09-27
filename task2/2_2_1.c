#include <stdio.h>

int main(void)
{
    double a, x, res = 0;
    scanf("%lf", &x);
    while(scanf("%lf", &a) != EOF){
        res = res*x + a;
    }
    printf("%lf\n", res);
    return 0;
}
