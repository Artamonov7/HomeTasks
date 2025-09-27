#include <stdio.h>

double mysqrt(double x, double eps)
{
    double xnext = 1, xprev;
    do {
        xprev = xnext;
        xnext = (xprev + x/xprev)/2.0;
    } while ((xnext - xprev) >= eps || (xprev - xnext) >= eps);
    return xnext;
}

int main(void)
{
    double eps, x;
    scanf("%lf", &eps);
    while (scanf("%lf", &x) > 0){
        printf("%.10g\n", mysqrt(x, eps));
    }
    return 0;
}
