#include <stdio.h>
#include <setjmp.h>

jmp_buf begin;

char curlex;

int deep, maxdeep;

void getlex(void);

int expr(void);

int add(void);

int mult(void);

void error();

void getlex()
{
    while ((curlex = getchar()) == ' ');
}

void error(void)
{
    printf("\nОШИБКА!\n");
    while (getchar() != '\n');
    longjmp(begin, 1);
}

int expr()
{
    int e = add();
    while (curlex == '+' || curlex == '-'){
        int is_plus = (curlex == '+');
        getlex();
        if (is_plus)
            e += add();
        else
            e -= add();
    }

    return e;
}

int add()
{
    int a = mult();
    while (curlex == '*' || curlex == '/'){
        int is_mult = (curlex == '*');
        getlex();
        if (is_mult)
            a *= mult();
        else
            a /= mult();
    }
    return a;
}

int mult()
{
    int m;
    switch (curlex){
        case '0': case '1': case '2': case '3': case '4': case '5': 
        case '6': case '7': case '8': case '9': m = curlex - '0';
        break;
        case '(': getlex();
		deep++;
                m = expr();
        if (curlex == ')') {
		if (deep > maxdeep){
			maxdeep = deep;	
		}
		deep--;
		break;
	}
        default : error();
    }
    getlex();
    if (curlex == '^'){
        getlex();
        int res = 1, i;
        int n = mult();
        if (n < 0)
            error();
        for (i = 0; i < n; i++)
            res *= m;
        return res;
    }
    return m;
}

int main(void)
{
    int result;
    deep = 0, maxdeep = 0;
    setjmp(begin);
    printf("==>");
    getlex();
    result = expr();
    if (curlex != '\n') error();
    printf("\n%d\n", result);
    printf("%d\n", maxdeep);
    return 0;
}

