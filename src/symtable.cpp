#include "symtable.hpp"
#include <iostream>
#include <stdlib.h>
#include <vector>

map<string, Symbol> symbol_table;
map<string, ProcedureInfo> procedures_map;

long long memory_offset = 5; // Start at 2. 0 unused. 1 reserved for LHS hold.
long long lhs_hold_addr = 1;

string current_procedure = ""; 
string current_call_proc = "";
int current_arg_idx = 0;

std::vector<int> current_for_stack;
int for_id_counter = 0;

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
    s.is_passed_to_proc = false;
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

    if (is_array && !is_param) memory_offset += (end - start + 1 + 1); // +1 for Header
    else memory_offset++;
}

Symbol* get_variable(string name) {
    // Try local proc first
    string key = current_procedure + "_" + name;
    if (symbol_table.find(key) != symbol_table.end()) {
        return &symbol_table[key];
    }
    
    // Try current FOR scope (Iterate from innermost to outermost)
    for (int i = current_for_stack.size() - 1; i >= 0; i--) {
        string for_key = "for_" + std::to_string(current_for_stack[i]) + "_" + name;
        if (symbol_table.find(for_key) != symbol_table.end()) {
            return &symbol_table[for_key];
        }
    }
    
    // Global
    if (symbol_table.count(name)) return &symbol_table[name];
    
    return nullptr;
}
