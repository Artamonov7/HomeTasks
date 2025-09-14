
#include <stdio.h>

int main()
{
    char c1, c2;
    scanf("%c", &c1);
    while (c1 != '.'){
        if (c1 == '?'){
            printf("!");
        }
        else if (('0' < c1) && (c1 <= '9')){
            while (('0' <= c1) && (c1 <= '9')){
                printf("%c", c1);
                scanf("%c", &c1);
            }
        }
        else if (c1 == '0'){
            scanf("%c", &c2);
            while (c2 == '0'){
                scanf("%c", &c2);
            }
            if (c2 == '.'){
                break;
            }
            else if (('0' < c2) && (c2 <= '9')){
                while (('0' <= c2) && (c2 <= '9')){
                    printf("%c", c2);
                    scanf("%c", &c2);
                }
            }   
            else {
                printf("%c %c", c1, c2);
            }
        }
        else {
            printf("%c", c1);
        }
        scanf("%c", &c1);
    }
}

