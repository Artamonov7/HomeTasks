#include <stdio.h>
#include <math.h>

int main()
{
    int n, i;
    double x, res = 0;
    scanf("%d %lf", &n, &x);
    for (i = 0; i < n; i++){
        res = (res + 1)*sin(x);
    }
    printf("%lf\n", res);
}
