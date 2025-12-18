%{
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

extern int yylineno;
int yylex();
void yyerror(const char *s);
%}

%token PROCEDURE IS IN END PROGRAM IF THEN ELSE ENDIF WHILE DO ENDWHILE REPEAT UNTIL FOR FROM TO DOWNTO READ WRITE
%token T I O
%token ASSIGN EQ NEQ GT LT GEQ LEQ
%token PLUS MINUS MULT DIV MOD
%token COMMA COLON SEMICOLON LPAREN RPAREN LBRACKET RBRACKET
%token pidentifier num
%token ENDFOR

%%

program_all : procedures main
    ;

procedures : procedures PROCEDURE proc_head IS declarations IN commands END
    | procedures PROCEDURE proc_head IS IN commands END
    | /* empty */
    ;

main : PROGRAM IS declarations IN commands END
    | PROGRAM IS IN commands END
    ;

commands : commands command
    | command
    ;

command : identifier ASSIGN expression SEMICOLON
    | IF condition THEN commands ELSE commands ENDIF
    | IF condition THEN commands ENDIF
    | WHILE condition DO commands ENDWHILE
    | REPEAT commands UNTIL condition SEMICOLON
    | FOR pidentifier FROM value TO value DO commands ENDFOR
    | FOR pidentifier FROM value DOWNTO value DO commands ENDFOR
    | proc_call SEMICOLON
    | READ identifier SEMICOLON
    | WRITE value SEMICOLON
    ;

proc_head : pidentifier LPAREN args_decl RPAREN
    ;

proc_call : pidentifier LPAREN args RPAREN
    ;

declarations : declarations COMMA pidentifier
    | declarations COMMA pidentifier LBRACKET num COLON num RBRACKET
    | pidentifier
    | pidentifier LBRACKET num COLON num RBRACKET
    ;

args_decl : args_decl COMMA type pidentifier
    | type pidentifier
    ;

type : T 
    | I 
    | O 
    | /* empty */
    ;

args : args COMMA pidentifier
    | pidentifier
    ;

expression : value
    | value PLUS value
    | value MINUS value
    | value MULT value
    | value DIV value
    | value MOD value
    ;

condition : value EQ value
    | value NEQ value
    | value GT value
    | value LT value
    | value GEQ value
    | value LEQ value
    ;

value : num
    | identifier
    ;

identifier : pidentifier
    | pidentifier LBRACKET pidentifier RBRACKET
    | pidentifier LBRACKET num RBRACKET
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s at line %d\n", s, yylineno);
}

int main() {
    return yyparse();
}