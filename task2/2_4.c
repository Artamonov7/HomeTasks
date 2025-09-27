#include <stdio.h>

double str2double(char str[])
{
    double res = 0;
    int n = 0, m = 0;
    int gotcomma = 0, gotminus = 0, counter = 0;
    while (*str != '\0' && *str != 'e' && *str != 'E'){
        if (*str == '.'){
            gotcomma = 1;
            str++;
            continue;
        }
        n = n*10 + *str - '0';
        if (gotcomma)
            counter++;
        str++;
    }
    if (*str == 'E' || *str == 'e'){
        str++;
        if (*str == '-'){
            gotminus = 1;
            str++;
        } else if (*str == '+'){
            str++;
        }
        while (*str != '\0'){
            m = m*10 + *str - '0';
            str++;
        }
        if (gotminus) 
            m = -m;
    }
    counter -= m;
    res = n;
    if (counter > 0){
        int i;
        for (i = 0; i < counter; i++){
            res = res/10;
        }
    }
    if (counter < 0){
        int i;
        for (i = 0; i > counter; i--){
            res = res*10;
        }
    }
    return res;
}

int main(void)
{
    char str[1024];
    while (scanf("%s", str) != EOF){
        printf("%.10g\n", str2double(str));
    }
    return 0;
}
