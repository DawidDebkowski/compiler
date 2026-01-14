%{
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <vector>
#include <string>
#include <map>
#include <iostream>

using namespace std;

extern int yylineno;
int yylex();
void yyerror(const char *s);

struct Instruction {
    string opcode;
    long long arg;
    bool has_arg;
};

// Global Code Buffer
vector<Instruction> code;

void emit(string opcode) {
    code.push_back({opcode, 0, false});
}

void emit(string opcode, long long arg) {
    code.push_back({opcode, arg, true});
}

// Symbol Table and Helpers
struct Symbol {
    long long address;
    bool is_array;
    bool is_param;
    long long start;
    long long end;
    
    // Spec flags
    string mod; // "T", "I", "O", ""
    bool is_iterator;
    bool is_initialized;
};

struct ProcedureInfo {
    long long address;
    vector<long long> param_addresses;
    vector<string> param_mods;     // "T", "I", "O", ""
    vector<bool> param_is_array;
};

map<string, Symbol> symbol_table;
map<string, ProcedureInfo> procedures_map;

long long memory_offset = 1000;
long long current_loop_start = -1; 

// Procedure handling
string current_procedure = ""; 
string current_call_proc = "";
int current_arg_idx = 0;

void add_symbol(string name, bool is_array, bool is_param, string mod, long long start, long long end) {
    if (is_array && start > end) {
        yyerror(("Invalid array range for " + name).c_str());
        exit(1);
    }

    string key = (current_procedure == "") ? name : current_procedure + "_" + name;
    if (symbol_table.count(key)) {
        yyerror(("Redeclaration: " + name).c_str());
        exit(1);
    }
    
    Symbol s;
    s.address = memory_offset;
    s.is_array = is_array;
    s.is_param = is_param;
    s.start = start;
    s.end = end;
    s.mod = mod;
    s.is_iterator = false;
    
    // Point 5: O means undefined value.
    if (mod == "O") s.is_initialized = false;
    else s.is_initialized = true; 

    symbol_table[key] = s;
    
    if (current_procedure != "" && is_param) {
        procedures_map[current_procedure].param_addresses.push_back(memory_offset);
        procedures_map[current_procedure].param_mods.push_back(mod);
        procedures_map[current_procedure].param_is_array.push_back(is_array);
        
        // Spec Check: T must be array
         if (mod == "T" && !is_array) {
            yyerror(("Parameter " + name + " marked T must be an array").c_str());
         }
         if (mod != "T" && is_array) {
             // Spec point 3: "nazwa tablicy ... powinna być poprzedzona literą T"
             yyerror(("Array parameter " + name + " must be marked with T").c_str());
         }
    }

    if (is_array) memory_offset += (end - start + 1);
    else memory_offset++;
}




Symbol* get_variable(string name) {
    // Try local first
    string key = current_procedure + "_" + name;
    if (symbol_table.find(key) != symbol_table.end()) {
        return &symbol_table[key];
    }
    // Try global (if any? arguments?)
    // In this language, do we have globals accessible in procs?
    // "Variables have static lifetime".
    // Usually local variables shadow globals or are unique?
    // Let's assume declarations are local to procedure.
    // If not found, check unprefixed (global)?
    if (symbol_table.count(name)) return &symbol_table[name];
    
    return nullptr;
}


void gen_const(int reg, long long value) {
    if (value < 0) value = 0; // Unsigned VM? Or signed? Prompt says ra is accumulator. No negative numbers mentioned explicitly but SUB exists. Instructions say "long long".
    // Assuming positive constants for now.
    
    emit("RST", reg);
    if (value == 0) return;
    
    // Binary expansion
    // e.g. 10 = 1010
    // 0: RST
    // MSB (1): INC
    // Next (0): SHL
    // Next (1): SHL, INC
    // Next (0): SHL
    
    unsigned long long v = value;
    int msb = 0;
    for(int i=63; i>=0; i--) {
        if ((v >> i) & 1) {
            msb = i;
            break;
        }
    }
    
    emit("INC", reg); // First 1
    
    for(int i=msb-1; i>=0; i--) {
        emit("SHL", reg);
        if ((v >> i) & 1) {
            emit("INC", reg);
        }
    }
}

// Math Lib Addresses
long long addr_mul = -1;
long long addr_div = -1;
long long addr_mod = -1;

void generate_mul() {
    addr_mul = code.size();
    // rb * rc -> ra
    // Returns result in ra. 
    // Uses rd to store return address.
    // Clobbers rb, rc, re, rf, rg.
    
    emit("SWP", 3); // Save Return Address (ra) to rd
    
    emit("RST", 0); // ra = 0 (Accumulator for result) (Actually let's use re as accumulator)
    emit("RST", 4); // re = 0
    
    // Check if Rc or Rb is 0
    // ... Optimization
    
    long long loop_start = code.size();
    
    // If rc == 0 JUMP END
    emit("RST", 0);
    emit("ADD", 2); // ra = rc
    emit("JZERO", code.size() + 20); // Placeholder jump to END (we will fix later or use relative logic?)
    long long jump_is_zero = code.size() - 1; // Store index to patch
    
    // Verify odd parity of rc
    // rf = rc
    emit("RST", 5); // rf
    emit("ADD", 2); // rf = rc
    emit("SHR", 5); // rf = rc / 2
    emit("SHL", 5); // rf = (rc / 2) * 2
    
    // ra = rc - rf
    emit("RST", 0);
    emit("ADD", 2); // ra = rc
    emit("SUB", 5); // ra = rc - rf. If 1, Odd. If 0, Even.
    
    emit("JZERO", code.size() + 4); // If 0 (Even), Skip Add.
    long long jump_is_even = code.size() - 1;
    
    // Odd: re = re + rb
    emit("RST", 0);
    emit("ADD", 4); // ra = re
    emit("ADD", 1); // ra = re + rb
    emit("SWP", 4); // re = ra (new sum)
    
    // Backpatch jump_is_even
    code[jump_is_even].arg = code.size();
    
    // Double rb
    emit("SHL", 1);
    // Halve rc
    emit("SHR", 2);
    
    emit("JUMP", loop_start);
    
    // Backpatch jump_is_zero
    code[jump_is_zero].arg = code.size();
    
    // Move result re to ra
    // But ra might be needed for SWP rd logic.
    // Wait, SWP rd puts current ra into rd and rd into ra.
    // rd holds return address.
    // So we want: ra = result.
    // code:
    // ra = re
    // swap rd (ra = ret_addr, rd = result)
    // RTRN (jumps to ra)
    // But caller expects result in ra ??
    // Standard convention: ra is accumulator/result.
    // But RTRN uses Value in Accumulator (ra) as Address?
    // Check mw.cc: case RTRN: lr = r[0]; ...
    // YES. RTRN sets IP to r[0].
    
    // So we MUST put Return Address in r[0] before executing RTRN.
    // So Result CANNOT be returned in r[0].
    // Result must be returned in rb or rc or globally?
    // Let's convention: Result in rb.
    
    // So:
    // Move re (result) to rb.
    emit("RST", 0);
    emit("ADD", 4); // ra = re
    emit("SWP", 1); // rb = re, ra = old_rb. (Result now in rb)
    
    // Restore Return Address from rd to ra
    emit("RST", 0);
    emit("ADD", 3); // ra = rd
    
    emit("RTRN");
}

void generate_div() {
    addr_div = code.size();
    // rb / rc -> rb (Quotient), rc (Remainder)
    emit("SWP", 3); // Save ret addr in rd
    
    // Check divide by zero (rc=0) -> result 0?
    emit("RST", 0);
    emit("ADD", 2);
    emit("JZERO", code.size() + 2); 
    long long jmp_zero = code.size() - 1;
    
    // Stub: rb=0, rc=0
    emit("RST", 1);
    emit("RST", 2);

    // End stub
    code[jmp_zero].arg = code.size();
    emit("RST", 0);
    emit("ADD", 3);
    emit("RTRN");
}

void generate_mod() {
    addr_mod = code.size();
    emit("SWP", 3);
    emit("CALL", addr_div);
    emit("RST", 0);
    emit("ADD", 2); 
    emit("SWP", 1); 
    emit("RST", 0);
    emit("ADD", 3); 
    emit("RTRN");
}

// Helper for Identifier location
struct Location {
    long long address; 
    int reg;           
    Symbol* sym;
};

// struct Procedure replaced by map check

vector<long long> loop_stack;
vector<long long> if_stack;

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
    } commands END {
        emit("RTRN");
        current_procedure = "";
    }
    | procedures PROCEDURE proc_head IS IN {
        procedures_map[current_procedure].address = code.size();
    } commands END {
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

command : identifier ASSIGN expression SEMICOLON {
        if ($1->sym->is_iterator) yyerror("Cannot modify loop iterator");
        if ($1->sym->mod == "I") yyerror("Cannot modify initialized parameter (I)");
        
        $1->sym->is_initialized = true;
        
        if ($1->reg != -1) emit("RSTORE", $1->reg);
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
    | WHILE condition DO commands ENDWHILE {
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
       add_symbol(string($4), false, true, string($3), 0, 0); free($3);
    }
    | type pidentifier {
       add_symbol(string($2), false, true, string($1), 0, 0); free($1);
    }
    ;

type : T { $$ = strdup("T"); }
     | I { $$ = strdup("I"); }
     | O { $$ = strdup("O"); }
     | /* empty */ { $$ = strdup(""); }
     ;

args : args COMMA pidentifier {
        Symbol* s = get_variable(string($3));
        if (!s) yyerror(("Undefined variable " + string($3)).c_str());
        
        ProcedureInfo& info = procedures_map[current_call_proc];
        if (current_arg_idx >= (int)info.param_addresses.size()) yyerror("Too many arguments for procedure call");
        
        long long param_addr = info.param_addresses[current_arg_idx];
        string p_mod = info.param_mods[current_arg_idx];
        bool p_array = info.param_is_array[current_arg_idx];
        
        if (p_array && !s->is_array) yyerror(("Expected array for parameter " + to_string(current_arg_idx+1)).c_str());
        if (!p_array && s->is_array) yyerror(("Expected scalar for parameter " + to_string(current_arg_idx+1)).c_str());

        current_arg_idx++;

        if (s->is_param) {
            emit("LOAD", s->address); 
            emit("STORE", param_addr); 
        } else {
            gen_const(0, s->address);
            emit("STORE", param_addr);
        }
    }
    | pidentifier {
        Symbol* s = get_variable(string($1));
        if (!s) yyerror(("Undefined variable " + string($1)).c_str());
        
        ProcedureInfo& info = procedures_map[current_call_proc];
        if (current_arg_idx >= (int)info.param_addresses.size()) yyerror("Too many arguments for procedure call");
        
        long long param_addr = info.param_addresses[current_arg_idx];
        string p_mod = info.param_mods[current_arg_idx];
        bool p_array = info.param_is_array[current_arg_idx];
        
        if (p_array && !s->is_array) yyerror(("Expected array for parameter " + to_string(current_arg_idx+1)).c_str());
        if (!p_array && s->is_array) yyerror(("Expected scalar for parameter " + to_string(current_arg_idx+1)).c_str());

        current_arg_idx++;

        if (s->is_param) {
            emit("LOAD", s->address); 
            emit("STORE", param_addr);
        } else {
            gen_const(0, s->address);
            emit("STORE", param_addr);
        }
    }
    ;

expression : value
    | value { emit("SWP", 1); } PLUS value { emit("ADD", 1); }
    | value { emit("SWP", 1); } MINUS value { emit("SWP", 1); emit("SUB", 1); }
    | value { emit("SWP", 1); } MULT value { emit("SWP", 2); emit("CALL", addr_mul); emit("SWP", 1); }
    | value { emit("SWP", 1); } DIV value { emit("SWP", 2); emit("CALL", addr_div); emit("SWP", 1); }
    | value { emit("SWP", 1); } MOD value { emit("SWP", 2); emit("CALL", addr_mod); } 
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

void optimize_code() {
    vector<bool> is_target(code.size() + 1, false);
    for (const auto& instr : code) {
        if (instr.has_arg && (instr.opcode == "JUMP" || instr.opcode == "JZERO" || instr.opcode == "JPOS" || instr.opcode == "CALL")) {
            if (instr.arg >= 0 && instr.arg < (long long)is_target.size())
                is_target[instr.arg] = true;
        }
    }

    vector<bool> to_delete(code.size(), false);
    for (size_t i = 0; i < code.size(); ++i) {
        if (to_delete[i]) continue;
        
        // STORE x, LOAD x -> Remove LOAD x
        if (code[i].opcode == "STORE" && i+1 < code.size()) {
             if (code[i+1].opcode == "LOAD" && 
                 code[i].arg == code[i+1].arg && 
                 !to_delete[i] && // Ensure STORE wasn't deleted (unlikely)
                 !is_target[i+1]) { 
                 to_delete[i+1] = true;
             }
        }
        
        // JUMP next
        if (code[i].opcode == "JUMP" && code[i].arg == (long long)(i + 1)) {
             to_delete[i] = true;
        }
    }
    
    vector<Instruction> new_code;
    vector<long long> map_idx(code.size() + 1);
    long long new_idx = 0;
    
    for (size_t i = 0; i < code.size(); ++i) {
        map_idx[i] = new_idx;
        if (!to_delete[i]) {
            new_code.push_back(code[i]);
            new_idx++;
        }
    }
    map_idx[code.size()] = new_idx; 
    
    for (auto& instr : new_code) {
        if (instr.has_arg && (instr.opcode == "JUMP" || instr.opcode == "JZERO" || instr.opcode == "JPOS" || instr.opcode == "CALL")) {
             instr.arg = map_idx[instr.arg];
        }
    }
    
    code = new_code;
}

int main() {
    if (yyparse() == 0) {
        optimize_code();
        for (const auto& instr : code) {
            cout << instr.opcode;
            if (instr.has_arg) cout << " " << instr.arg;
            cout << endl;
        }
    }
    return 0;
}
