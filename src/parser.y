%{
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <vector>
#include <string>
#include <map>
#include <iostream>

#include "types.hpp"
#include "codegen.hpp"
#include "symtable.hpp"
#include "math_kernel.hpp"

using namespace std;

extern int yylineno;
int yylex();
void yyerror(const char *s);

%}

%union {
    char* str;
    long long num;
    struct Location* loc; 
}

%token <str> PROCEDURE IS IN END PROGRAM IF THEN ELSE ENDIF WHILE DO ENDWHILE REPEAT UNTIL FOR FROM TO DOWNTO READ WRITE
%token <str> T I O
%token <str> ASSIGN EQ NEQ GT LT GEQ LEQ
%token <str> PLUS MINUS MULT DIV MOD
%token <str> COMMA COLON SEMICOLON LPAREN RPAREN LBRACKET RBRACKET
%token <str> pidentifier 
%token <num> num
%token <str> ENDFOR

%type <loc> identifier
%type <str> type

%%

program_all : { 
     emit("JUMP", 0); // JUMP to Main
     generate_mul();
     generate_div();
     generate_mod();
   } procedures main {
      code[0].arg = symbol_table["main_start"].address; 
      emit("HALT");
   }
   ;

procedures : procedures PROCEDURE proc_head IS declarations IN {
        procedures_map[current_procedure].address = code.size();
        
        // Save Return Address
        long long ra_addr = memory_offset++;
        procedures_map[current_procedure].ra_address = ra_addr;
        emit("STORE", ra_addr);
        
    } commands END {
        // Restore Return Address
        long long ra_addr = procedures_map[current_procedure].ra_address;
        emit("LOAD", ra_addr);
        emit("RTRN");
        current_procedure = "";
    }
    | procedures PROCEDURE proc_head IS IN {
        procedures_map[current_procedure].address = code.size();
        
        long long ra_addr = memory_offset++;
        procedures_map[current_procedure].ra_address = ra_addr;
        emit("STORE", ra_addr);
        
    } commands END {
        long long ra_addr = procedures_map[current_procedure].ra_address;
        emit("LOAD", ra_addr);
        emit("RTRN");
        current_procedure = "";
    }
    | /* empty */
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

main : PROGRAM IS { 
         current_procedure = "main"; 
         Symbol s; s.address = code.size();
         symbol_table["main_start"] = s; 
       } declarations IN {
         symbol_table["main_start"].address = code.size();
       } commands END
    | PROGRAM IS { 
         current_procedure = "main"; 
         Symbol s; s.address = code.size(); 
         symbol_table["main_start"] = s;
       } IN {
         symbol_table["main_start"].address = code.size();
       } commands END
    ;

commands : commands command
    | command
    ;

command : identifier {
        // Save LHS address if it is in a register (because RHS calculation might clobber it)
        if ($1->reg != -1) {
             emit("SWP", $1->reg); // Move Addr to r[0]
             emit("STORE", lhs_hold_addr);
             emit("SWP", $1->reg); // Move back (optional but safest state)
        }
    } ASSIGN expression SEMICOLON {
        if ($1->sym->is_iterator) yyerror("Cannot modify loop iterator");
        if ($1->sym->mod == "I") yyerror("Cannot modify initialized parameter (I)");
        
        $1->sym->is_initialized = true;
        
        if ($1->reg != -1) {
             // Restore Address from lhs_hold_addr
             emit("SWP", 1); // Save Expression Result to rb
             emit("LOAD", lhs_hold_addr); // Load Addr to ra
             emit("SWP", $1->reg); // Move Addr to reg (e.g. r[7])
             emit("SWP", 1); // Restore Expression Result to ra
             emit("RSTORE", $1->reg);
        }
        else emit("STORE", $1->address);
        delete $1;
    }
    | IF condition THEN commands ELSE {
        long long jump_over_else = code.size();
        emit("JUMP", 0); 
        long long false_jump_idx = if_stack.back();
        if_stack.pop_back();
        code[false_jump_idx].arg = code.size();
        if_stack.push_back(jump_over_else);
    } commands ENDIF {
        long long jump_over_else = if_stack.back();
        if_stack.pop_back();
        code[jump_over_else].arg = code.size();
    }
    | IF condition THEN commands ENDIF {
        long long false_jump_idx = if_stack.back();
        if_stack.pop_back();
        code[false_jump_idx].arg = code.size();
    }
    | WHILE {
        loop_stack.push_back(code.size());
    } condition DO commands ENDWHILE {
         long long jump_out = if_stack.back();
         if_stack.pop_back();
         long long start = loop_stack.back();
         loop_stack.pop_back();
         emit("JUMP", start);
         code[jump_out].arg = code.size();
    }
    | REPEAT {
        loop_stack.push_back(code.size());
    } commands UNTIL condition SEMICOLON {
         long long jump_if_false = if_stack.back(); 
         if_stack.pop_back();
         long long start = loop_stack.back();
         loop_stack.pop_back();
         code[jump_if_false].arg = start;
    }
    | FOR pidentifier FROM value {
          Symbol* iter = get_variable(string($2));
          if (!iter) {
              // Implicit declaration of iterator
              add_symbol(string($2), false, false, "", 0, 0);
              iter = get_variable(string($2));
          }
          if (iter->is_iterator) yyerror("Nested loop with same iterator");
          if (iter->is_param) yyerror("Iterator must be local variable");
          
          iter->is_iterator = true;
          iter->is_initialized = true;
          
          emit("STORE", iter->address);
    } TO value {
          emit("SWP", 1); 
          Symbol* iter = get_variable(string($2));
          emit("LOAD", iter->address);
          
          emit("SWP", 1); 
          emit("SUB", 1); 
          emit("INC", 0); 
          
          long long count_addr = memory_offset++;
          emit("STORE", count_addr);
          
          long long start_loop = code.size();
          emit("LOAD", count_addr);
          emit("JZERO", 0);
          long long jump_out = code.size()-1;
          
          loop_stack.push_back(start_loop);
          loop_stack.push_back(jump_out);
          loop_stack.push_back(count_addr);
    } DO commands ENDFOR {
          long long count_addr = loop_stack.back(); loop_stack.pop_back();
          long long jump_out = loop_stack.back(); loop_stack.pop_back();
          long long start_loop = loop_stack.back(); loop_stack.pop_back();
          
          emit("LOAD", count_addr);
          emit("DEC", 0);
          emit("STORE", count_addr);
          
          Symbol* iter = get_variable(string($2));
          emit("LOAD", iter->address);
          emit("INC", 0);
          emit("STORE", iter->address);
          
          emit("JUMP", start_loop);
          code[jump_out].arg = code.size();
          
          iter->is_iterator = false;
    }
    | FOR pidentifier FROM value {
          Symbol* iter = get_variable(string($2));
          if (iter->is_iterator) yyerror("Nested loop with same iterator");
          if (iter->is_param) yyerror("Iterator must be local variable");
          iter->is_iterator = true;
          iter->is_initialized = true;

          emit("STORE", iter->address); 
    } DOWNTO value {
          emit("SWP", 1); 
          Symbol* iter = get_variable(string($2));
          emit("LOAD", iter->address);
          
          emit("SUB", 1); 
          emit("INC", 0); 
          
          long long count_addr = memory_offset++;
          emit("STORE", count_addr);
          
          long long start_loop = code.size();
          emit("LOAD", count_addr);
          emit("JZERO", 0);
          long long jump_out = code.size()-1;
          
          loop_stack.push_back(start_loop);
          loop_stack.push_back(jump_out);
          loop_stack.push_back(count_addr);
    } DO commands ENDFOR {
          long long count_addr = loop_stack.back(); loop_stack.pop_back();
          long long jump_out = loop_stack.back(); loop_stack.pop_back();
          long long start_loop = loop_stack.back(); loop_stack.pop_back();
          
          emit("LOAD", count_addr);
          emit("DEC", 0);
          emit("STORE", count_addr);
          
          Symbol* iter = get_variable(string($2));
          emit("LOAD", iter->address);
          emit("DEC", 0);
          emit("STORE", iter->address);
          
          emit("JUMP", start_loop);
          code[jump_out].arg = code.size();
          
          iter->is_iterator = false;
    }
    | proc_call SEMICOLON
    | READ identifier SEMICOLON {
        if ($2->sym->is_iterator) yyerror("Cannot read into loop iterator");
        if ($2->sym->mod == "I") yyerror("Cannot read into I parameter");
        $2->sym->is_initialized = true;

        emit("READ");
        if ($2->reg != -1) emit("RSTORE", $2->reg);
        else emit("STORE", $2->address);
        delete $2;
    }
    | WRITE value SEMICOLON {
        emit("WRITE");
    }
    ;

proc_call : pidentifier LPAREN { 
        current_call_proc = string($1);
        current_arg_idx = 0;
    } args RPAREN {
        emit("CALL", procedures_map[$1].address);
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

args : args COMMA argument 
    | argument 
    ;

argument : pidentifier {
        Symbol* s = get_variable(string($1));
        if (!s) yyerror(("Undefined variable " + string($1)).c_str());
        
        ProcedureInfo& info = procedures_map[current_call_proc];
        if (current_arg_idx >= (int)info.param_addresses.size()) yyerror("Too many arguments for procedure call");
        
        long long param_addr = info.param_addresses[current_arg_idx];
        bool p_array = info.param_is_array[current_arg_idx];
        
        if (p_array && !s->is_array) yyerror(("Expected array for parameter " + to_string(current_arg_idx+1)).c_str());
        if (!p_array && s->is_array) yyerror(("Expected scalar for parameter " + to_string(current_arg_idx+1)).c_str());

        current_arg_idx++;

        if (s->is_param) {
            emit("LOAD", s->address); 
            emit("STORE", param_addr); 
        } else {
            if (s->is_array) {
                gen_const(0, s->address); 
                emit("STORE", param_addr);
            } else {
                gen_const(0, s->address);
                emit("STORE", param_addr);
            }
        }
    }
    | num {
        ProcedureInfo& info = procedures_map[current_call_proc];
        if (current_arg_idx >= (int)info.param_addresses.size()) yyerror("Too many arguments for procedure call");
        
        long long param_addr = info.param_addresses[current_arg_idx];
        bool p_array = info.param_is_array[current_arg_idx];
        
        if (p_array) yyerror(("Expected array for parameter " + to_string(current_arg_idx+1) + " but got number").c_str());
        
        current_arg_idx++;

        // Store constant in temporary memory
        long long temp_addr = memory_offset++;
        gen_const(0, $1);
        emit("STORE", temp_addr);
        
        // Pass address of temp
        gen_const(0, temp_addr);
        emit("STORE", param_addr);
    }
    ;

    ;

expression : value
    | value { emit("SWP", 1); } PLUS value { emit("ADD", 1); }
    | value { emit("SWP", 1); } MINUS value { emit("SWP", 1); emit("SUB", 1); }
    | value { emit("SWP", 1); } MULT value { emit("SWP", 2); emit("CALL", addr_mul); emit("SWP", 1); }
    | value { emit("SWP", 1); } DIV value { emit("SWP", 2); emit("CALL", addr_div); emit("SWP", 1); }
    | value { emit("SWP", 1); } MOD value { emit("SWP", 2); emit("CALL", addr_mod); emit("SWP", 1); } 
    ;

condition : value { emit("SWP", 1); } EQ value {
        emit("SWP", 2); 
        emit("RST", 0); emit("ADD", 1); emit("SUB", 2); emit("SWP", 3);
        emit("RST", 0); emit("ADD", 2); emit("SUB", 1); emit("ADD", 3);
        emit("JPOS", 0); 
        if_stack.push_back(code.size()-1); 
    }
    | value { emit("SWP", 1); } NEQ value {
        emit("SWP", 2); 
        emit("RST", 0); emit("ADD", 1); emit("SUB", 2); emit("SWP", 3);
        emit("RST", 0); emit("ADD", 2); emit("SUB", 1); emit("ADD", 3);
        emit("JZERO", 0); 
        if_stack.push_back(code.size()-1); 
    }
    | value { emit("SWP", 1); } GT value { 
        emit("SWP", 1); emit("SUB", 1); 
        emit("JZERO", 0); 
        if_stack.push_back(code.size()-1); 
    }
    | value { emit("SWP", 1); } LT value { 
        emit("SUB", 1); 
        emit("JZERO", 0); 
        if_stack.push_back(code.size()-1); 
    }
    | value { emit("SWP", 1); } GEQ value { 
        emit("SUB", 1);
        emit("JPOS", 0);
        if_stack.push_back(code.size()-1);
    }
    | value { emit("SWP", 1); } LEQ value { 
        emit("SWP", 1); 
        emit("SUB", 1); 
        emit("JPOS", 0);
        if_stack.push_back(code.size()-1);
    }
    ;

value : num { gen_const(0, $1); }
    | identifier {
        if (!$1->sym->is_initialized) yyerror("Usage of uninitialized variable");
        if ($1->reg != -1) emit("RLOAD", $1->reg);
        else emit("LOAD", $1->address);
        delete $1;
    }
    ;

identifier : pidentifier {
         Symbol* s = get_variable(string($1));
         if (!s) yyerror(("Undefined identifier " + string($1)).c_str());
         $$ = new Location();
         $$->address = s->address;
         $$->sym = s;
         if (s->is_param) {
             emit("LOAD", s->address);
             emit("SWP", 7); 
             $$->reg = 7;
             $$->address = -1;
         } else {
             $$->reg = -1;
         }
    }
    | pidentifier LBRACKET num RBRACKET {
         Symbol* s = get_variable(string($1));
         if (!s) yyerror(("Undefined identifier " + string($1)).c_str());
         $$ = new Location();
         $$->sym = s;
         if (s->is_param) {
             emit("LOAD", s->address); 
             long long offset = $3 - s->start;
             gen_const(1, offset);
             emit("ADD", 1);
             emit("SWP", 7);
             $$->reg = 7;
             $$->address = -1;
         } else {
            $$->address = s->address + $3 - s->start;
            $$->reg = -1;
         }
    }
    | pidentifier LBRACKET pidentifier RBRACKET {
         Symbol* arr = get_variable(string($1));
         Symbol* idx = get_variable(string($3));
         if (!arr) yyerror(("Undefined array " + string($1)).c_str());
         if (!idx) yyerror(("Undefined variable " + string($3)).c_str());
         if (!idx->is_initialized) yyerror("Usage of uninitialized variable for loop index");
         
         if (idx->is_param) {
             emit("LOAD", idx->address);
             emit("RLOAD", 0); 
         } else {
             emit("LOAD", idx->address);
         }
         
         if (arr->start > 0) { gen_const(1, arr->start); emit("SUB", 1); }
         
         if (arr->is_param) {
             emit("SWP", 1); 
             emit("LOAD", arr->address); 
             emit("ADD", 1); 
         } else {
             gen_const(1, arr->address); 
             emit("ADD", 1);
         }
         
         emit("SWP", 7);
         $$ = new Location();
         $$->address = -1;
         $$->reg = 7;
         $$->sym = arr;
    }
    ;

%%

void yyerror(const char *s) {
    cerr << "Error: " << s << " at line " << yylineno << endl;
    exit(1);
}

extern FILE *yyin;

int main(int argc, char* argv[]) {
    if (argc > 1) {
        yyin = fopen(argv[1], "r");
        if (!yyin) {
            cerr << "Cannot open input file: " << argv[1] << endl;
            return 1;
        }
    }

    if (yyparse() == 0) {
        // lets NOT OPTIMIZE CODE NOW
        /* optimize_code(); */
        
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
            if (instr.has_arg) fprintf(out, " %lld", instr.arg);
            fprintf(out, "\n");
        }

        if (out != stdout) fclose(out);
    }
    return 0;
}
