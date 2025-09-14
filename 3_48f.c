
#include <stdio.h>

int main()
{
    char ch;
    int c, h, a, r;
    c = h = a = r = 0;
    scanf("%c", &ch);
    while (ch != '.'){
        if (ch == 'c'){
            c++;
        }
        if (ch == 'h'){
            h++;
        }
        if (ch == 'a'){
            a++;
        }
        if (ch == 'r'){
            r++;
        }
        scanf("%c", &ch);
    }
    if (c && h && a && r){
        printf("yes \n");
    }
    else {
        printf("no \n");
    }
    return 0;
}
