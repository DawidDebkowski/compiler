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
#include <algorithm>

using namespace std;

extern int yylineno;
int yylex();
void yyerror(const char *s);

// Global Analysis State
bool analyze_mode = false;
int loop_depth = 0;
map<string, long long> var_usage_counts;
map<string, int> var_register_assignments;
extern Symbol* reg_descriptors[8]; // Track which variable is in which register (0-7)

void reset_parser_state() {
    code.clear();
    symbol_table.clear();
    procedures_map.clear();
    memory_offset = 1000;
    current_procedure = "";
    calls_mul.clear(); calls_div.clear(); calls_mod.clear();
    loop_stack.clear(); if_stack.clear();
    loop_depth = 0;
    addr_mul = -1; addr_div = -1; addr_mod = -1;
    for(int i=0; i<8; i++) reg_descriptors[i] = nullptr;
}

void check_reg_assignment(char* name) {
    if (analyze_mode) return;
    string sname = (current_procedure == "") ? string(name) : current_procedure + "_" + string(name);
    if (var_register_assignments.count(sname)) {
         Symbol* s = get_variable(string(name));
         if (s) {
             s->cache_reg = var_register_assignments[sname];
             reg_descriptors[s->cache_reg] = s;
         }
    }
}

void spill_registers(int max_reg) {
    for(int i=4; i<=max_reg; i++) {
        if(reg_descriptors[i]) {
            emit("SWP", i);
            emit("STORE", reg_descriptors[i]->address);
            emit("SWP", i);
        }
    }
}

void restore_registers(int max_reg) {
    bool needed = false;
    for(int i=4; i<=max_reg; i++) if(reg_descriptors[i]) needed = true;
    if(!needed) return;
    
    long long temp = memory_offset++;
    emit("STORE", temp); 
    
    for(int i=4; i<=max_reg; i++) {
        if(reg_descriptors[i]) {
            emit("LOAD", reg_descriptors[i]->address); 
            emit("SWP", i); 
        }
    }
    
    emit("LOAD", temp); 
}

%}

%code requires {
    #include "types.hpp"
}

%union {
    char* str;
    long long num;
    struct Location* loc; 
    struct Operand operand;
}

%token <str> PROCEDURE IS IN END PROGRAM IF THEN ELSE ENDIF WHILE DO ENDWHILE REPEAT UNTIL FOR FROM TO DOWNTO READ WRITE
%token <str> T I O
%token <str> ASSIGN EQ NEQ GT LT GEQ LEQ
%token <str> PLUS MINUS MULT DIV MOD
%token <str> COMMA COLON SEMICOLON LPAREN RPAREN LBRACKET RBRACKET
%token <str> pidentifier 
%token <num> num
%token <str> ENDFOR

%type <operand> value
%type <loc> identifier
%type <str> type

%%

program_all : { 
     emit("JUMP", 0); // JUMP to Main
   } procedures main {
      if (!analyze_mode) {
          code[0].arg = symbol_table["main_start"].address; 
          emit("HALT");

          if (!calls_mul.empty()) {
              generate_mul();
              for(long long idx : calls_mul) code[idx].arg = addr_mul;
          }
          if (!calls_div.empty() || !calls_mod.empty()) {
              if (addr_div == -1) generate_div();
              for(long long idx : calls_div) code[idx].arg = addr_div;
          }
          if (!calls_mod.empty()) {
              generate_mod();
              for(long long idx : calls_mod) code[idx].arg = addr_mod;
          }
      }
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
        else {
             if ($1->sym->cache_reg != -1) {
                 emit("SWP", $1->sym->cache_reg); 
             } else {
                 emit("STORE", $1->address);
             }
        }
        delete $1;
    }
    | IF condition THEN commands ELSE {
        long long jump_over_else = code.size();
        emit("JUMP", 0); 
        long long false_jump_idx = if_stack.back();
        if_stack.pop_back();
        if (!analyze_mode) code[false_jump_idx].arg = code.size();
        if_stack.push_back(jump_over_else);
    } commands ENDIF {
        long long jump_over_else = if_stack.back();
        if_stack.pop_back();
        if (!analyze_mode) code[jump_over_else].arg = code.size();
    }
    | IF condition THEN commands ENDIF {
        long long false_jump_idx = if_stack.back();
        if_stack.pop_back();
        if (!analyze_mode) code[false_jump_idx].arg = code.size();
    }
    | WHILE {
        loop_stack.push_back(code.size());
        loop_depth++;
    } condition DO commands ENDWHILE {
         loop_depth--;
         long long jump_out = if_stack.back();
         if_stack.pop_back();
         long long start = loop_stack.back();
         loop_stack.pop_back();
         emit("JUMP", start);
         if (!analyze_mode) code[jump_out].arg = code.size();
    }
    | REPEAT {
        loop_depth++;
        loop_stack.push_back(code.size());
    } commands UNTIL condition SEMICOLON {
         loop_depth--;
         long long jump = if_stack.back(); 
         if_stack.pop_back();
         long long start = loop_stack.back();
         loop_stack.pop_back();
         if (!analyze_mode) code[jump].arg = start;
    }
    | FOR pidentifier FROM value {
          Symbol* iter = get_variable(string($2));
          if (!iter) {
              add_symbol(string($2), false, false, "", 0, 0);
              iter = get_variable(string($2));
          }
          if (iter->is_iterator) yyerror("Nested loop with same iterator");
          if (iter->is_param) yyerror("Iterator must be local variable");
          
          iter->is_iterator = true;
          iter->is_initialized = true;
          
          string scoped_name = (current_procedure == "") ? string($2) : current_procedure + "_" + string($2);
          if (analyze_mode) {
               var_usage_counts[scoped_name] += (pow(10, loop_depth) * 10);
          } else {
               if (var_register_assignments.count(scoped_name)) 
                   iter->cache_reg = var_register_assignments[scoped_name];
          }

          if (iter->cache_reg != -1) emit("SWP", iter->cache_reg);
          else emit("STORE", iter->address);
    } TO value {
          emit("SWP", 1); 
          Symbol* iter = get_variable(string($2));
          
          if (iter->cache_reg != -1) {
              emit("RST", 0); emit("ADD", 1);
              emit("SUB", iter->cache_reg);
              emit("INC", 0);
          } else {
              emit("LOAD", iter->address);
              emit("SWP", 1); 
              emit("SUB", 1); 
              emit("INC", 0); 
          }
          
          long long count_addr = memory_offset++;
          emit("STORE", count_addr);
          
          long long start_loop = code.size();
          emit("LOAD", count_addr);
          emit("JZERO", 0);
          long long jump_out = code.size()-1;
          
          loop_stack.push_back(start_loop);
          loop_stack.push_back(jump_out);
          loop_stack.push_back(count_addr);
          loop_depth++;
    } DO commands ENDFOR {
          loop_depth--;
          long long count_addr = loop_stack.back(); loop_stack.pop_back();
          long long jump_out = loop_stack.back(); loop_stack.pop_back();
          long long start_loop = loop_stack.back(); loop_stack.pop_back();
          
          emit("LOAD", count_addr);
          emit("DEC", 0);
          emit("STORE", count_addr);
          // FOR TO END
          
          Symbol* iter = get_variable(string($2));
          if (iter->cache_reg != -1) {
              emit("INC", iter->cache_reg);
          } else {
              emit("LOAD", iter->address);
              emit("INC", 0);
              emit("STORE", iter->address);
          }
          
          emit("JUMP", start_loop);
          if (!analyze_mode) code[jump_out].arg = code.size();
          
          iter->is_iterator = false;
    }
    | FOR pidentifier FROM value {
          Symbol* iter = get_variable(string($2));
          if (!iter) {
              add_symbol(string($2), false, false, "", 0, 0);
              iter = get_variable(string($2));
          }
          if (iter->is_iterator) yyerror("Nested loop with same iterator");
          if (iter->is_param) yyerror("Iterator must be local variable");
          iter->is_iterator = true;
          iter->is_initialized = true;

          string scoped_name = (current_procedure == "") ? string($2) : current_procedure + "_" + string($2);
          if (analyze_mode) {
               var_usage_counts[scoped_name] += (pow(10, loop_depth) * 10);
          } else {
               if (var_register_assignments.count(scoped_name)) 
                   iter->cache_reg = var_register_assignments[scoped_name];
          }

          if (iter->cache_reg != -1) emit("SWP", iter->cache_reg);
          else emit("STORE", iter->address);
    } DOWNTO value {
          emit("SWP", 1); 
          Symbol* iter = get_variable(string($2));
          
          if (iter->cache_reg != -1) {
              emit("RST", 0); emit("ADD", iter->cache_reg);
              emit("SUB", 1);
              emit("INC", 0);
          } else {
              emit("LOAD", iter->address);
              emit("SUB", 1); 
              emit("INC", 0); 
          }
          
          long long count_addr = memory_offset++;
          emit("STORE", count_addr);
          
          long long start_loop = code.size();
          emit("LOAD", count_addr);
          emit("JZERO", 0);
          long long jump_out = code.size()-1;
          
          loop_stack.push_back(start_loop);
          loop_stack.push_back(jump_out);
          loop_stack.push_back(count_addr);
          loop_depth++;
    } DO commands ENDFOR {
          loop_depth--;
          long long count_addr = loop_stack.back(); loop_stack.pop_back();
          long long jump_out = loop_stack.back(); loop_stack.pop_back();
          long long start_loop = loop_stack.back(); loop_stack.pop_back();
          
          emit("LOAD", count_addr);
          emit("DEC", 0);
          emit("STORE", count_addr);
          // FOR DOWNTO END
          
          Symbol* iter = get_variable(string($2));
          if (iter->cache_reg != -1) {
              emit("DEC", iter->cache_reg);
          } else {
              emit("LOAD", iter->address);
              emit("DEC", 0);
              emit("STORE", iter->address);
          }
          
          emit("JUMP", start_loop);
          if (!analyze_mode) code[jump_out].arg = code.size();
          
          iter->is_iterator = false;
    }
    | proc_call SEMICOLON
    | READ identifier SEMICOLON {
        if ($2->sym->is_iterator) yyerror("Cannot read into loop iterator");
        if ($2->sym->mod == "I") yyerror("Cannot read into I parameter");
        $2->sym->is_initialized = true;

        emit("READ");
        
        if ($2->sym->cache_reg != -1) {
             emit("SWP", $2->sym->cache_reg);
        } else if ($2->reg != -1) {
             emit("RSTORE", $2->reg);
        } else {
             emit("STORE", $2->address);
        }
        delete $2;
    }
    | WRITE value SEMICOLON {
        emit("WRITE");
    }
    ;

proc_call : pidentifier LPAREN { 
        current_call_proc = string($1);
        current_arg_idx = 0;
        spill_registers(7);
    } args RPAREN {
        emit("CALL", procedures_map[$1].address);
        restore_registers(7);
    }
    ;

declarations : declarations COMMA pidentifier {
        add_symbol(string($3), false, false, "", 0, 0);
        check_reg_assignment($3);
    }
    | declarations COMMA pidentifier LBRACKET num COLON num RBRACKET {
        add_symbol(string($3), true, false, "", $5, $7);
        check_reg_assignment($3);
    }
    | pidentifier {
        add_symbol(string($1), false, false, "", 0, 0);
        check_reg_assignment($1);
    }
    | pidentifier LBRACKET num COLON num RBRACKET {
        add_symbol(string($1), true, false, "", $3, $5);
        check_reg_assignment($1);
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
        
        // Usage Analysis
        string scoped_name = (current_procedure == "") ? string($1) : current_procedure + "_" + string($1);
        if (analyze_mode) {
             long long weight = 1;
             for(int k=0; k<loop_depth; k++) weight *= 10;
             var_usage_counts[scoped_name] += weight; // Params getting passed have cost
        }
        
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
            if (s->cache_reg != -1) {
                 emit("SWP", s->cache_reg); 
                 emit("STORE", s->address);
                 emit("SWP", s->cache_reg);
            }
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
    | value { emit("SWP", 1); } MULT value { 
        bool optimized = false;
        // Case 1: Right operand is constant power of 2 (Var * Const)
        if ($4.is_const && $4.val > 0) {
            long long v = $4.val;
            if ((v & (v - 1)) == 0) { // Power of 2
                // Remove constant load
                for(int k=0; k<$4.instructions_count; k++) code.pop_back();
                
                // r1 contains first operand. r0 is now free/unknown.
                // Apply shifts to r1
                int shifts = 0;
                while (v > 1) { v >>= 1; shifts++; }
                for(int k=0; k<shifts; k++) emit("SHL", 1);
                
                // Move result to r0 (expected output register)
                emit("SWP", 1);
                
                optimized = true;
            }
        }
        
        // Case 2: Left operand is constant power of 2 (Const * Var)
        if (!optimized && $1.is_const && $1.val > 0) {
             long long v = $1.val;
             if ((v & (v - 1)) == 0) {
                 // The Var is in r0 (a). The Const is in r1 (b) (put there by SWP 1).
                 // We want Var * Const.
                 // Just shift r0.
                 int shifts = 0;
                 while (v > 1) { v >>= 1; shifts++; }
                 for(int k=0; k<shifts; k++) emit("SHL", 0);
                 
                 optimized = true;
             }
        }
        
        if (!optimized) {
            spill_registers(5);
            emit("SWP", 2); emit("CALL", 0); calls_mul.push_back(code.size()-1); emit("SWP", 1); 
            restore_registers(5);
        }
    }
    | value { emit("SWP", 1); } DIV value { 
        bool optimized = false;
        if ($4.is_const && $4.val > 0) {
            long long v = $4.val;
            if ((v & (v - 1)) == 0) { // Power of 2
                // Remove constant load
                for(int k=0; k<$4.instructions_count; k++) code.pop_back();
                
                // r1 contains first operand. r0 is now free/unknown.
                // Apply shifts to r1
                int shifts = 0;
                while (v > 1) { v >>= 1; shifts++; }
                for(int k=0; k<shifts; k++) emit("SHR", 1);
                
                // Move result to r0
                emit("SWP", 1);
                
                optimized = true;
            }
        }
        
        if (!optimized) {
            spill_registers(6);
            emit("SWP", 2); emit("CALL", 0); calls_div.push_back(code.size()-1); emit("SWP", 1); 
            restore_registers(6);
        }
    }
    | value { emit("SWP", 1); } MOD value { 
        bool optimized = false;
        if ($4.is_const && $4.val > 0) {
            long long v = $4.val;
            if ((v & (v - 1)) == 0) { // Power of 2
                // Remove constant load
                for(int k=0; k<$4.instructions_count; k++) code.pop_back();
                
                // x % 2^k  ==  x - ((x >> k) << k)
                // r1 has x.
                
                int shifts = 0;
                while (v > 1) { v >>= 1; shifts++; }
                
                // Save x to proper register to preserve it?
                // Logic:
                // r1 = x
                // r2 = x (copy) -- Need instruction COPY or logic
                // No COPY in VM? 
                // We have r0..r7.
                // SWP 2 (r2 <-> r0). r0 is trash.
                // r1 has x. emit("SWP", 2); -> r1 trash, r2 = x ?? No SWP x swaps r0 and rx.
                // SWP x: r0 <-> rx.
                
                emit("SWP", 1); // r0 = x, r1 = trash
                emit("SWP", 2); // r2 = x, r0 = trash/r2_old
                emit("RST", 0); emit("ADD", 2); // r0 = x
                
                // Calculate (x >> k) << k
                 for(int k=0; k<shifts; k++) emit("SHR", 0);
                 for(int k=0; k<shifts; k++) emit("SHL", 0);
                 
                 // Subtract from original x (in r2)
                 // r0 = (x>>k)<<k
                 // r2 = x
                 emit("SWP", 2); // r0 = x, r2 = (x>>k)<<k
                 emit("SUB", 2); // r0 = x - ...
                 
                 optimized = true;
            }
        }
    
        if (!optimized) {
            spill_registers(6);
            emit("SWP", 2); emit("CALL", 0); calls_mod.push_back(code.size()-1); emit("SWP", 1); 
            restore_registers(6);
        }
    } 
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

value : num { 
        long long start = code.size();
        gen_const(0, $1); /* Generates code into r0 */
        $$.is_const = true;
        $$.val = $1;
        $$.instructions_count = code.size() - start;
    }
    | identifier {
        if (!$1->sym->is_initialized) yyerror("Usage of uninitialized variable");
        
        if ($1->sym->cache_reg != -1) {
             // Cached in Register!
             // Load from Reg X to Reg A
             emit("RST", 0); 
             emit("ADD", $1->sym->cache_reg);
        } else {
            if ($1->reg != -1) emit("RLOAD", $1->reg);
            else emit("LOAD", $1->address);
        }
        delete $1;
        $$.is_const = false;
    }
    ;

identifier : pidentifier {
         Symbol* s = get_variable(string($1));
         if (!s) yyerror(("Undefined identifier " + string($1)).c_str());
         
         string scoped_name = (current_procedure == "") ? string($1) : current_procedure + "_" + string($1);
         if (analyze_mode) {
             long long weight = 1;
             for(int k=0; k<loop_depth; k++) weight *= 10;
             var_usage_counts[scoped_name] += weight;
         } else {
             if (var_register_assignments.count(scoped_name)) {
                 s->cache_reg = var_register_assignments[scoped_name];
                 // Update reg_descriptors
                 reg_descriptors[s->cache_reg] = s;
             }
         }

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
             gen_const(2, offset);
             emit("ADD", 2);
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
         
         if (arr->start > 0) { gen_const(2, arr->start); emit("SUB", 2); }
         
         if (arr->is_param) {
             emit("SWP", 2); 
             emit("LOAD", arr->address); 
             emit("ADD", 2); 
         } else {
             gen_const(2, arr->address); 
             emit("ADD", 2);
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
void yyrestart(FILE *input_file);

int main(int argc, char* argv[]) {
    if (argc > 1) {
        yyin = fopen(argv[1], "r");
        if (!yyin) {
            cerr << "Cannot open input file: " << argv[1] << endl;
            return 1;
        }
    }

    // PASS 1: Analysis
    analyze_mode = true;
    if (yyparse() != 0) return 1;
    
    // Assign Registers
    vector<pair<long long, string>> usage_list;
    for (auto const& [name, count] : var_usage_counts) {
        usage_list.push_back({count, name});
    }
    sort(usage_list.rbegin(), usage_list.rend());
    
    // Use r4(e), r5(f), r6(g) primarily. Maybe r1(b), r2(c), r7(h)?
    // User requested r1-r7.
    // r3 is RA - Reserved.
    // r1, r2 are used by Math Kernel Inputs. If we spill around calls, we can use them!
    // But parser uses r1, r2 as temps for Expressions.
    // Safest set without rewriting Expression Logic: r4, r5, r6, r7. (r7 used for Arr offset).
    // Let's stick to r4, r5, r6 for now to avoid Reg1/Reg2 clobbering in `expression` rule.
    // If I had time to rewrite `expression` to use any free register, I would.
    // For now, let's try 4, 5, 6.
    
    int available_regs[] = {4, 5, 6}; 
    int reg_limit = 3;
    int reg_idx = 0;
    
    for (auto p : usage_list) {
         if (reg_idx >= reg_limit) break;
         string name = p.second;
         // Note: Symbol table is cleared, so we don't know if is_array.
         // We assume scalar. Arrays usage counts should be on the array name?
         // Ideally Pass 1 checks is_array.
         // Let's filter out known arrays if possible. 
         // Since symtable is gone, we can't check `is_array`.
         // Improvise: Register assignment is tentative. In Pass 2, `add_symbol` will check.
         var_register_assignments[name] = available_regs[reg_idx];
         reg_idx++;
    }

    // PASS 2: Codegen
    fseek(yyin, 0, SEEK_SET);
    yyrestart(yyin);
    reset_parser_state();
    analyze_mode = false;
    yylineno = 1;

    if (yyparse() == 0) {
        // Optimize
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
            fprintf(out, "\n");
        }

        if (out != stdout) fclose(out);
    }
    return 0;
}
