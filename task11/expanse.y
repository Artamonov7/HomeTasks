%{
#include <stdio.h>
int yylex(void);
void yyerror(char *s);
%}

%%
P : S { printf("expanse: %d\n", $1); }
  ;

S : '(' S ')' S { $$ = $4 + 1; }
  | /*      */  { $$ = 0; }
  ;
%%

int yylex() {
    int c = getchar();
    if (c == '\n')
        return 0;
    else if (c == '(' || c == ')') {
        yylval = c;
        return c;
    }
    else
        return 0;
}

void yyerror(char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

int main() {
    printf("Enter a bracket string: ");
    yyparse();
    return 0;
}

