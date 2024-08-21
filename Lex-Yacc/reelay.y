%code requires {
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <iostream>
}

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <unordered_set>
#include <iostream>

extern int yylex();
extern char* yytext;
extern FILE* yyin;

void yyerror(const char* s);

%}


%token SINCE OR AND NOT PRE ONCE HISTORICALLY EXISTS FORALL '(' ')' '{' '}' '[' ']' ',' ':' '.' '\"' EQUALS IMPLIES '*'

%token <str> IDENTIFIER
%token <num> NUMBER

%define api.value.type union

%type <temped> reelay_expr or_t and_t since_t unary atom_list atom_list_inside atom
%type <liste> string_list
%type <timed> time_id
%type <atomed> atom_value


%start reelay_expr

%locations
%debug
%%

reelay_expr         : or_t {$$ = $1;}
                    | reelay_expr IMPLIES or_t {$$ = implied($1, $3);}
                    ;

or_t                : and_t {$$ = $1;}
                    | or_t OR and_t {$$ = ored($1, $3);}
                    ;

and_t               : since_t {$$ = $1;}
                    | and_t AND since_t {$$ = anded($1, $3);}
                    ;

since_t             : since_t SINCE unary {time_id_s* time_des = malloc(sizeof(time_id_s)); time_des->from = 0; time_des->to =1; $$ = sinced($1, $3, time_des);}
                    | since_t SINCE time_id unary {$$ = sinced($1, $4, $3);}
                    | unary
                    ;

unary               : atom_list {$$ = $1;}
                    | NOT unary {$$ = negated($2);}
                    | PRE unary {$$ = pred($2);}
                    | ONCE time_id unary {$$ = onced($3, $2);}
                    | HISTORICALLY time_id unary {$$ = historied($3, $2);}
                    | EXISTS '[' string_list ']' '.' unary {$$ = existed($3, $6);}
                    | FORALL '[' string_list ']' '.' unary {$$ = foralled($3, $6);}
                    ;

atom_list           : '{' atom_list_inside '}' {$$ = $2;}
                    | '(' reelay_expr ')' {$$ = $2;}
                    ;

atom_list_inside    : atom_list_inside ',' atom {$$ = anded($1, $3);}
                    | atom {$$ = $1;}
                    ;

atom                : '\"' IDENTIFIER '\"' ':' atom_value {$$ = atom_handler($2, $5);}
                    | '\"' IDENTIFIER '\"' EQUALS atom_value {$$ = atom_handler($2, $5);}
                    ;

atom_value          : '\"' IDENTIFIER '\"' {$$ = atom_value_handler($2, 0);}
                    | '*' IDENTIFIER {$$ = atom_value_handler($2, 1);}
                    ;

string_list         : IDENTIFIER {$$ = list_handler_'*'t($1);}
                    | string_list ',' IDENTIFIER {$$ = list_handler($1, $3);}
                    ;

time_id             : '[' NUMBER ':' NUMBER ']' {$$ = time_id_handler($2, $4);}
                    | '[' NUMBER ':' ']' {$$ = time_id_handler($2,-1);}
                    | '[' ':' NUMBER ']' {$$ = time_id_handler(0, $3);}
                    ;

%%
void yyerror(const char* s) {
    fprintf(stderr, "Error: %s\n", s);
}

int main(int argc, char** argv) {
    if (argc > 1) {
        yyin = fopen(argv[1], "r");
        if (!yyin) {
            perror(argv[1]);
            return 1;
        }
    }

    yyparse();

    return 0;
}

