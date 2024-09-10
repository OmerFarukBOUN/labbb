%{
#include "test.tab.h"
#include <math.h>
#include <stdlib.h>
%}

%%
([0-9]+|([0-9]*\.[0-9]+)([eE][-+]?[0-9]+)?) {
        yylval.value = atoi(yytext); return NUMBER;
}

[ \t]   ;                /* ignore white space */

\n      |
.       return yytext[0];
%%
