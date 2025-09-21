#include <stdio.h>

int main()
{
    int i;
    int count = 0;
    for (i = 111; i < 1000; i++){
        if((i/100 == i/10 - (i/100)*10 ) || ((i/10) - (i/100)*10 == i - i/10*10) || (i/100 == i - i/10*10)){
            count++;
        }
    }
    printf("%d\n", count);
}
