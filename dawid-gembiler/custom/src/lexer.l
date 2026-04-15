%{
#include <stdio.h>
#include <stdlib.h>
#include "parser.tab.hpp"

int yylex();
%}

%option noinput nounput
%option yylineno

%%

\#.*                    ; /* Comment */
[ \t\r\n]+              ; /* Whitespace */

"PROCEDURE"             { return PROCEDURE; }
"IS"                    { return IS; }
"IN"                    { return IN; }
"END"                   { return END; }
"ENDFOR"                { return ENDFOR; }
"ENDWHILE"              { return ENDWHILE; }
"PROGRAM"               { return PROGRAM; }
"IF"                    { return IF; }
"THEN"                  { return THEN; }
"ELSE"                  { return ELSE; }
"ENDIF"                 { return ENDIF; }
"WHILE"                 { return WHILE; }
"DO"                    { return DO; }
"REPEAT"                { return REPEAT; }
"UNTIL"                 { return UNTIL; }
"FOR"                   { return FOR; }
"FROM"                  { return FROM; }
"TO"                    { return TO; }
"DOWNTO"                { return DOWNTO; }
"READ"                  { return READ; }
"WRITE"                 { return WRITE; }
"T"                     { return T; }
"I"                     { return I; }
"O"                     { return O; }

":="                    { return ASSIGN; }
"="                     { return EQ; }
"!="                    { return NEQ; }
">="                    { return GEQ; }
"<="                    { return LEQ; }
">"                     { return GT; }
"<"                     { return LT; }
"+"                     { return PLUS; }
"-"                     { return MINUS; }
"*"                     { return MULT; }
"/"                     { return DIV; }
"%"                     { return MOD; }

","                     { return COMMA; }
":"                     { return COLON; }
";"                     { return SEMICOLON; }
"("                     { return LPAREN; }
")"                     { return RPAREN; }
"["                     { return LBRACKET; }
"]"                     { return RBRACKET; }

[_a-z]+                 { yylval.str = strdup(yytext); return pidentifier; }
[0-9]+                  { yylval.num = atoll(yytext); return num; }

.                       { return yytext[0]; }

%%

int yywrap() {
    return 1;
}
