#include <stdio.h>

int int_nbits()
{
    unsigned char c = -1;
    int n;
    while (c){
        c = c >> 1;
        n++;
    }
    return n;
}

int main()
{
    printf("%d\n", int_nbits());
    return 0;    
}
