#include <stdio.h>

int main()
{
    int a = 0, b = 0, c = 1, d;
    d = (a != b < c);
    printf("a = %d, b = %d, c = %d => (a != b < c) = %d \n", a, b, c, d);
    
    a = 1, b = 1, c = 1;
    d = (a != b <= c);
    printf("a = %d, b = %d, c = %d => (a != b <= c) = %d \n", a, b, c, d);
    
    a = 1, b = 0, c = 1;
    d = (a != b > c);
    printf("a = %d, b = %d, c = %d => (a != b > c) = %d \n", a, b, c, d);
    
    a = 1, b = 1, c = 0;
    d = (a != b >= c);
    printf("a = %d, b = %d, c = %d => (a != b >= c) = %d \n", a, b, c, d);
    
    a = 0, b = 0, c = 0;
    d = (a == b < c);
    printf("a = %d, b = %d, c = %d => (a == b < c) = %d \n", a, b, c, d);

    a = 0, b = 1, c = 1;
    d = (a == b <= c);
    printf("a = %d, b = %d, c = %d => (a == b <= c) = %d \n", a, b, c, d);

    a = 0, b = 0, c = 1;
    d = (a == b > c);
    printf("a = %d, b = %d, c = %d => (a == b > c) = %d \n", a, b, c, d);

    a = 0, b = 0, c = 0;
    d = (a == b >= c);
    printf("a = %d, b = %d, c = %d => (a == b >= c) = %d \n", a, b, c, d);
    
    printf("program ends");
    return 0;

}
