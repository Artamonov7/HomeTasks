#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

typedef double (*math_function)(double);

double f1(double x)
{
    return sin(x);
}

double f2(double x)
{
    return pow(x, 13.0/2.0);
}

double f3(double x)
{
    return 1.0 / sqrt(1.0 + 15.0*cos(x)*cos(x));
}

double f4(double x)
{
    if (3*x - x*x <= 1e-12)
        return 0;
    else
        return 1.0 / ((25*x*x + 1) * sqrt(3*x - x*x));
}

double quadrature_trapezoid(math_function f, double a, double b, double n, double quadprev)
{
    int i;
    double h = (b - a) / n;
    double quad = quadprev / 2.0;
    for (i = 1; i < n; i += 2){
        quad += f(a + i*h)*h;
    }
    return quad;
}

int main(int argc, char **argv)
{
    int n, func_no;
    double a, b;
    math_function f;

    if (argc < 2)
        exit(1);
    func_no = atoi(argv[1]);
    switch (func_no){
    case 1:
        f = f1;
        a = 0;
        b = M_PI;
        break;
    case 2:
        f = f2;
        a = 0;
        b = 1;
        break;
    case 3:
        f = f3;
        a = 0;
        b = M_PI / 2.0;
        break;
    case 4:
        f = f4;
        a = 0;
        b = 3;
        break;
    default:
        exit(1);
    }

    double e = 0.000001;
    double R[20][20];
    R[0][0] = (b - a) / 2.0 * (f(a) + f(b));
    R[1][0] = quadrature_trapezoid(f, a, b, 2, R[0][0]);
    R[1][1] = (4*R[1][0] - R[0][0])/3;
    int i = 1, j;
    while ((fabs(R[i][i] - R[i - 1][i - 1])) >= e){
        i++;
        n = 1 << i;
        R[i][0] = quadrature_trapezoid(f, a, b, n, R[i-1][0]);
        for (j = 1; j <= i; j++){
            R[i][j] = (pow(4, j) * R[i][j - 1] - R[i - 1][j - 1]) / (pow(4, j) - 1);
        }
    }
    int k;
    for (k = 0; k < i + 1; k++){
        for (j = 0; j < k; j++){
            printf("%.10f ", R[k][j]);
        }
        printf("\n");
    }
    printf("result: %.10f\n", R[i][i]);
}
