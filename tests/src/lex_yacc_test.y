%{
#include <stdio.h>
#include <stdlib.h>
int yylex();
int yyerror(char* s);
%}

%union {
    int value;
}

%token <value> NUMBER
%left '-' '+'
%left '*' '/'
%nonassoc UMINUS

%type <value> expression

%%
statement_list: statement '\n'
        |       statement_list statement '\n'
        ;

statement:      expression { printf("= %d\n", $1); }
        ;

expression:     expression '+' expression { $$ = $1 + $3; }
        |       expression '-' expression { $$ = $1 - $3; }
        |       expression '*' expression { $$ = $1 * $3; }
        |       '-' expression %prec UMINUS     { $$ = -$2; }
        |       '(' expression ')'      { $$ = $2; }
        |       NUMBER                  { $$ = $1; }
        ;
%%

int main(void) {
    return yyparse();
}

int yyerror(char *s) {
    fprintf(stderr, "error: %s\n", s);
    return 0;
}
