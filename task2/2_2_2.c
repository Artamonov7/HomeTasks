#include <stdio.h>

int main(void)
{
    double a, x, res = 0, derfake = 0, der;
    scanf("%lf", &x);
    while (scanf("%lf", &a) != EOF){
        der = derfake;
        res = res*x + a;
        derfake = derfake*x + res;
    }
    printf("%lf\n%lf\n", res, der);
    return 0;
}

