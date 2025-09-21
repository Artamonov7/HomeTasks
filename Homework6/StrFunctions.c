#include <stdio.h>

void strcopy(char *s, char *t)
{
    int i = 0;
    while (s[i] != '\0'){
        s[i] = t[i];
        i++;
    }
    s[i] = t[i];
}

void strcat1(char *s, char *t)
{
    int i = 0;
    while (s[i] != '\0'){
        i++;
    }
    int j = 0;
    while (t[j] != '\0'){
        s[i] = s[j];
        i++;
        j++;
    }
}

int strcmp(char *s, char *t)
{
    int i = 0;
    while (s[i] == t[i] && s[i] != '\0' && t[i] != '\0'){
        i++;
    }
    if (s[i] != '\0' && t[i] != '\0'){
        if (s[i] > t[i]){
            return 1;
        }
        else if (s[i] == t[i]){
            return 0;
            }
        else if (s[i] < t[i]){
            return -1;
        }
    }
    return 0;
}

char *strchr1(char *s, char c)
{
    int i = 0;
    while (s[i] != '\0' && s[i] != c){
        i++;
    }
    if (s[i] == c){
        return &s[i];
    }
    return 0;
}

char *strstr(char *s, char *t)
{
    int i = 0;
    while (s[i] != '\0' && strcmp(&s[i], t) != 0){
        i++;
    }
    if (s[i] != '\0'){
        return &s[i];
    }
    return 0;
}

int strlen1(char *s)
{
    int i = 0;
    while (s[i] != '\0'){
        i++;
    }
    return i;
}

void reverse(char *s)
{
    int i;
    int n = strlen1(s);
    char c;
    for (i = 0; i < n/2; i++){
        c = s[n - i - 1];
        s[n - i - 1] = s[i];
        s[i] = c;
    }
}

int main()
{
    char s[20] = "aaaabcaaa";
    char t[20] = "abc";
    char c = 'c';
    printf("%p\n%p\n", s, strstr(s, t));
    printf("%p\n", strchr1(s, c));
    printf("%d\n", strcmp(s, t));
    strcopy(s, t);
    printf("%s\n", s);
    strcat1(s, t);
    printf("%s\n", s);
    reverse(s);
    printf("%s\n", s);
    return 0;

}
