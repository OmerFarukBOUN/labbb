%{
#include "y.tab.hpp"
#include <string>
%}

%%
(implies)|(->)   { return IMPLIES; }
"since"     { return SINCE; }
(or)|(\|\|)        { return OR; }
(and)|(&&)       { return AND; }
(not)|(!)       { return NOT; }
"pre"       { return PRE; }
"once"      { return ONCE; }
"historically" { return HISTORICALLY; }
"exists"    { return EXISTS; }
"forall"    { return FORALL; }
"("         { return '('; }
")"         { return ')'; }
"{"         { return '{'; }
"}"         { return '}'; }
"["         { return '['; }
"]"         { return ']'; }
","         { return ','; }
":"         { return ':'; }
"."         { return '.'; }
"\""        { return '\"'; }
"*"         { return '*'; }
"=="         { return EQUALS; }
[a-zA-Z_][a-zA-Z0-9_]* {yylval.IDENTIFIER = new std::string(yytext, yytext + yyleng); return IDENTIFIER; }
[0-9]+      {yylval.NUMBER = std::stoi(std::string(yytext, yytext + yyleng)); return NUMBER; }
[ \t\n]     { /* ignore whitespace */ }
"/*"([^*]|"*"[^/])*"*/" { /* Ignore comments */ }
.
%%

int yywrap() {
    return 1;
}

