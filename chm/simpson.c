#include <stdio.h>
#include <math.h>
#include <stdlib.h>

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

double quad_simpson(math_function f, double a, double b)
{
    double h = b - a;
    double m = (a + b) / 2;
    return (h/6)*(f(a) + 4*f(m) + f(b));
}

double simpson(math_function f, double a, double b, double eps)
{
    double m = (a + b) / 2;
    double total = quad_simpson(f, a, b);
    double left  = quad_simpson(f, a, m);
    double right = quad_simpson(f, m, b);        
    double halves = left + right;
    double error = fabs(total - halves);
    if (error < eps) {
        return halves;
    }
    else {
        return simpson(f, a, m, eps/2) + simpson(f, m, b, eps/2);
    }
}

int main(int argc, char **argv) 
{
    double a;
    double b;
    double eps = 1e-6;
    math_function f;
    
    if (argc < 2)
        exit(1);
    int func_no = atoi(argv[1]);
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
    double result = simpson(f, a, b, eps);
    printf("result: %.10f\n", result);
    return 0;
}

