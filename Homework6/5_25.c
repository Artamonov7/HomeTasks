#include <stdio.h>

void readmas(int n, double *m)
{
    int i;
    for (i = 0; i < n; i++){
        scanf("%lf", &m[i]);
    }

}
double max(int n, double *m)
{
    double max1 = m[0] + m[n - 1];
    int i;
    for (i = 1; i < n/2 + 1; i++){
        if(max1 < m[i] + m[n - i]){
            max1 = m[i] + m[n - i];
        }
    }
    return max1;
}

int main()
{
    int n;
    scanf("%d", &n);
    double m[n];
    readmas(n, m);
    printf("%lf\n", max(n, m));
}
