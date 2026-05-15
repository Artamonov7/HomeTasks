%{
#include <stdio.h>
#include <stdlib.h>

int yylex(void);
void yyerror(const char *s);
%}

%union {
    int ival;
}

%token <ival> NUMBER
%left '+' '-'
%left '*' '/'

%type <ival> s expr term primary

%%

s     : s expr '\n'      { printf("Result: %d\n", $2); }
      | s '\n'
      |
      ;

expr  : expr '+' term    { $$ = $1 + $3; }
      | expr '-' term    { $$ = $1 - $3; }
      | term             { $$ = $1; }
      ;

term  : term '*' primary { $$ = $1 * $3; }
      | term '/' primary { if ($3 == 0) {
                                yyerror("division by zero");
                                $$ = 0;
                           } else $$ = $1 / $3; }
      | primary          { $$ = $1; }
      ;

primary : NUMBER         { $$ = $1; }
        | '(' expr ')'   { $$ = $2; }
        ;

%%

int yylex() {
    int c;
    while ((c = getchar()) == ' ' || c == '\t');
    
    if (c == EOF)
        return 0;
    if (c >= '0' && c <= '9') {
        int value = 0;
        while (c >= '0' && c <= '9') {
            value = value * 10 + (c - '0');
            c = getchar();
        }
        ungetc(c, stdin);
        yylval.ival = value;
        return NUMBER;
    }
    if (c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')' || c == '\n')
        return c;
    return c;
}

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

int main() {
    yyparse();
    return 0;
}
