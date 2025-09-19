#include <stdio.h>

int func()
{
    printf("function was called\n");
    return 0;
}

int main()
{   
    int a = 0;
    printf("a = %d => (a && func()) = %d \n", a, a && func());
    
    a = 1;
    printf("a = %d => (a || func()) = %d \n", a, a || func());
    
    return 0;

}
