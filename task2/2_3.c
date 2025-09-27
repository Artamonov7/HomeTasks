#include <stdio.h>

int fib_iter(int i)
{
    int prev1 = 1, prev2 = 0, j, n;
    if (i == 0)
        return 0;
    if (i == 1)
        return 1;
    for (j = 1; j < i; j++){
        n = prev1 + prev2;
        prev2 = prev1;
        prev1 = n;
    }
    return n;
}

int fib_rec(int i)
{
    if (i == 0)
        return 0;
    if (i == 1)
        return 1;
    return fib_rec(i - 1) + fib_rec(i - 2);
}

int main(void)
{
    int i;
    while (scanf("%d", &i) != EOF){
        printf("%d\n", fib_iter(i));
        printf("%d\n", fib_rec(i));
    }
    return 0;
}
