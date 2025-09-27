#include <stdio.h>

int main(void)
{
    int n;
    double a, x, res = 0, derfake = 0, der;
    double in = 0, inmember;
    scanf("%d", &n);
    scanf("%lf", &x);
    while (scanf("%lf", &a) != EOF){
        der = derfake;
        res = res*x + a;
        derfake = derfake*x + res;
        inmember = a/(n + 1);
        in = (in + inmember)*x;
        n--;
    }
    printf("%lf\n%lf\n%lf\n", res, der, in);
    return 0;
}
          
