%{
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <fstream>

#include "types.hpp"
#include "codegen.hpp"
#include "symtable.hpp"
#include "math_kernel.hpp"
#include "ast.hpp"

using namespace std;

extern int yylineno;
int yylex();
void yyerror(const char *s);

extern RootNode* parsed_root;

%}

%code requires {
    #include "types.hpp"
    #include "ast.hpp"
}

%union {
    char* str;
    long long num;
    ASTNode* node;
    StatementNode* stmt;
    ExpressionNode* expr;
    IdentifierNode* ident;
    ConditionNode* cond;
    std::vector<StatementNode*>* stmt_list;
    std::vector<ProcedureNode*>* proc_list;
    std::vector<IdentifierNode*>* ident_list;
    std::vector<ValueNode*>* val_list;
}

%token <str> PROCEDURE IS IN END PROGRAM IF THEN ELSE ENDIF WHILE DO ENDWHILE REPEAT UNTIL FOR FROM TO DOWNTO READ WRITE
%token <str> T I O
%token <str> ASSIGN EQ NEQ GT LT GEQ LEQ
%token <str> PLUS MINUS MULT DIV MOD
%token <str> COMMA COLON SEMICOLON LPAREN RPAREN LBRACKET RBRACKET
%token <str> pidentifier 
%token <num> num
%token <str> ENDFOR

%type <expr> value expression
%type <ident> identifier
%type <cond> condition
%type <stmt> command proc_call
%type <stmt_list> commands main
%type <proc_list> procedures
%type <val_list> args
%type <str> type

%%

program_all : procedures main {
     parsed_root = new RootNode(*$1, *$2, yylineno);
     delete $1;
     delete $2;
   }
   ;

procedures : procedures PROCEDURE proc_head IS declarations IN commands END {
        $1->push_back(new ProcedureNode(current_procedure, *$7, yylineno));
        delete $7;
        current_procedure = "";
        $$ = $1;
    }
    | procedures PROCEDURE proc_head IS IN commands END {
        $1->push_back(new ProcedureNode(current_procedure, *$6, yylineno));
        delete $6;
        current_procedure = "";
        $$ = $1;
    }
    | /* empty */ { $$ = new std::vector<ProcedureNode*>(); }
    ;

proc_head : pidentifier LPAREN { 
        current_procedure = string($1);
        if (procedures_map.count(current_procedure)) {
             yyerror(("Redeclaration of procedure " + current_procedure).c_str());
             exit(1);
        }
        ProcedureInfo info;
        info.address = -1; 
        procedures_map[current_procedure] = info;
    } args_decl RPAREN {
    }
    ;

main : PROGRAM IS declarations IN commands END { $$ = $5; }
    | PROGRAM IS IN commands END { $$ = $4; }
    ;

commands : commands command { $1->push_back($2); $$ = $1; }
    | command { $$ = new std::vector<StatementNode*>(); $$->push_back($1); }
    ;

command : identifier ASSIGN expression SEMICOLON {
        $$ = new AssignmentNode($1, $3, yylineno);
    }
    | IF condition THEN commands ELSE commands ENDIF {
        $$ = new IfNode($2, *$4, *$6, yylineno);
        delete $4;
        delete $6;
    }
    | IF condition THEN commands ENDIF {
        std::vector<StatementNode*> empty;
        $$ = new IfNode($2, *$4, empty, yylineno);
        delete $4;
    }
    | WHILE condition DO commands ENDWHILE {
        $$ = new WhileNode($2, *$4, yylineno);
        delete $4;
    }
    | REPEAT commands UNTIL condition SEMICOLON {
        $$ = new RepeatNode($4, *$2, yylineno);
        delete $2;
    }
    | FOR pidentifier FROM value TO value DO commands ENDFOR {
        $$ = new ForNode(string($2), $4, $6, false, *$8, yylineno);
        delete $8;
    }
    | FOR pidentifier FROM value DOWNTO value DO commands ENDFOR {
        $$ = new ForNode(string($2), $4, $6, true, *$8, yylineno);
        delete $8;
    }
    | proc_call SEMICOLON {
        $$ = $1;
    }
    | READ identifier SEMICOLON {
        $$ = new ReadNode($2, yylineno);
    }
    | WRITE value SEMICOLON {
        $$ = new WriteNode($2, yylineno);
    }
    ;

proc_call : pidentifier LPAREN args RPAREN {
        $$ = new ProcCallNode(string($1), *$3, yylineno);
        delete $3;
    }
    ;

args : args COMMA pidentifier {
        $1->push_back(new IdentifierNode(string($3), yylineno));
        $$ = $1;
    }
    | pidentifier {
        $$ = new std::vector<ValueNode*>();
        $$->push_back(new IdentifierNode(string($1), yylineno));
    }
    ;

declarations : declarations COMMA pidentifier {
        add_symbol(string($3), false, false, "", 0, 0);
    }
    | declarations COMMA pidentifier LBRACKET num COLON num RBRACKET {
        add_symbol(string($3), true, false, "", $5, $7);
    }
    | pidentifier {
        add_symbol(string($1), false, false, "", 0, 0);
    }
    | pidentifier LBRACKET num COLON num RBRACKET {
        add_symbol(string($1), true, false, "", $3, $5);
    }
    ;

args_decl : args_decl COMMA type pidentifier {
       string m = string($3);
       bool is_arr = (m == "T");
       add_symbol(string($4), is_arr, true, m, 0, 0); free($3);
    }
    | type pidentifier {
       string m = string($1);
       bool is_arr = (m == "T");
       add_symbol(string($2), is_arr, true, m, 0, 0); free($1);
    }
    ;

type : T { $$ = strdup("T"); }
     | I { $$ = strdup("I"); }
     | O { $$ = strdup("O"); }
     | /* empty */ { $$ = strdup(""); }
     ;

expression : value { $$ = $1; }
    | value PLUS value { $$ = new BinaryOpNode($1, BinaryOp::PLUS, $3, yylineno); }
    | value MINUS value { $$ = new BinaryOpNode($1, BinaryOp::MINUS, $3, yylineno); }
    | value MULT value { $$ = new BinaryOpNode($1, BinaryOp::MULT, $3, yylineno); }
    | value DIV value { $$ = new BinaryOpNode($1, BinaryOp::DIV, $3, yylineno); }
    | value MOD value { $$ = new BinaryOpNode($1, BinaryOp::MOD, $3, yylineno); }
    ;

condition : value EQ value { $$ = new ConditionNode($1, ConditionOp::EQ, $3, yylineno); }
    | value NEQ value { $$ = new ConditionNode($1, ConditionOp::NEQ, $3, yylineno); }
    | value GT value { $$ = new ConditionNode($1, ConditionOp::GT, $3, yylineno); }
    | value LT value { $$ = new ConditionNode($1, ConditionOp::LT, $3, yylineno); }
    | value GEQ value { $$ = new ConditionNode($1, ConditionOp::GEQ, $3, yylineno); }
    | value LEQ value { $$ = new ConditionNode($1, ConditionOp::LEQ, $3, yylineno); }
    ;

value : num { $$ = new NumberNode($1, yylineno); }
    | identifier { $$ = $1; }
    ;

identifier : pidentifier { $$ = new IdentifierNode(string($1), yylineno); }
    | pidentifier LBRACKET num RBRACKET {
         $$ = new IdentifierNode(string($1), yylineno);
         $$->set_array_access($3, yylineno);
    }
    | pidentifier LBRACKET pidentifier RBRACKET {
         $$ = new IdentifierNode(string($1), yylineno);
         $$->set_array_access(string($3), yylineno);
    }
    ;

%%

void yyerror(const char *s) {
    cerr << "Error: " << s << " at line " << yylineno << endl;
    exit(1);
}

extern FILE *yyin;

RootNode* parsed_root = nullptr;

int main(int argc, char* argv[]) {
    if (argc > 1) {
        yyin = fopen(argv[1], "r");
        if (!yyin) {
            cerr << "Cannot open input file: " << argv[1] << endl;
            return 1;
        }
    }

    if (yyparse() == 0 && parsed_root) {
        parsed_root->validate();
        
        if (argc > 2) {
             string out_name = argv[2];
             string ast_name = out_name + ".ast";
             ofstream ast_file(ast_name);
             if (ast_file.is_open()) {
                 parsed_root->print(ast_file);
                 ast_file.close();
                 // cout << "AST written to " << ast_name << endl;
             }
        }
        
        parsed_root->codegen();
        optimize_code();
        
        FILE* out = stdout;
        if (argc > 2) {
            out = fopen(argv[2], "w");
            if (!out) {
                cerr << "Cannot open output file: " << argv[2] << endl;
                return 1;
            }
        }

        for (const auto& instr : code) {
            fprintf(out, "%s", instr.opcode.c_str());
            if (instr.has_arg) {
                if (instr.opcode == "RST" || instr.opcode == "INC" || instr.opcode == "DEC" || 
                    instr.opcode == "SHL" || instr.opcode == "SHR" || instr.opcode == "SWP" || 
                    instr.opcode == "ADD" || instr.opcode == "SUB" || instr.opcode == "COPY" ||
                    instr.opcode == "RLOAD" || instr.opcode == "RSTORE") {
                    
                    if (instr.arg >= 0 && instr.arg < 26) {
                        char reg = 'a' + (char)instr.arg;
                        fprintf(out, " %c", reg);
                    } else {
                        fprintf(out, " %lld", instr.arg);
                    }
                } else {
                    fprintf(out, " %lld", instr.arg);
                }
            }
            if(instr.comment != "") {
                fprintf(out, " # %s", instr.comment.c_str());
            }
            fprintf(out, "\n");
        }

        if (out != stdout) fclose(out);
        delete parsed_root;
    }
    return 0;
}
