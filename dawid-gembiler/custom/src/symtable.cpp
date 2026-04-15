// Dawid Dębkowski 279714
#include "symtable.hpp"
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <algorithm>

map<string, Symbol> symbol_table;
map<string, ProcedureInfo> procedures_map;

long long memory_offset = 5; // Start at 2. 0 unused. 1 reserved for LHS hold. ()
long long lhs_hold_addr = 1; // not used but what if everything breaks? 5h left.
bool unsafety_detected = false;

// its all just for tab[2^62:2^62+1] and whole unsafety
// because LOAD SUB for manual base + (index - start) + 1 is too costly than just a compile time passing of base - start
// and make sure base >= start (less lines, less cost)
// and it should be safe for too large arrays
// In that time I should've probably implemented TAC and variables in registers
struct FreeBlock {
    long long start;
    long long size;
};
std::vector<FreeBlock> holes;

string current_procedure = ""; 
string current_call_proc = "";
int current_arg_idx = 0;

// for i 
//   for i
//      for i
std::vector<int> current_for_stack;
int for_id_counter = 0;

static map<string, Symbol*> substitution_map;

void add_substitution(string key, Symbol* s) {
    substitution_map[key] = s;
}

void remove_substitution(string key) {
    if (substitution_map.count(key)) {
        substitution_map.erase(key);
    }
}

void add_hole(long long start, long long size) {
    if (size <= 0) return;
    holes.push_back({start, size});
    sort(holes.begin(), holes.end(), [](const FreeBlock& a, const FreeBlock& b) {
        return a.start < b.start;
    });
    for (size_t i = 0; i < holes.size() - 1; ) {
        if (holes[i].start + holes[i].size >= holes[i+1].start) {
            long long new_end = std::max(holes[i].start + holes[i].size, holes[i+1].start + holes[i+1].size);
            holes[i].size = new_end - holes[i].start;
            holes.erase(holes.begin() + i + 1);
        } else {
            i++;
        }
    }
}

long long find_hole(long long size, long long min_start) {
    for (size_t i = 0; i < holes.size(); ++i) {
        long long h_start = holes[i].start;
        long long h_end = holes[i].start + holes[i].size;
        long long candidate = std::max(h_start, min_start);
        if (candidate + size <= h_end) {
            long long left_size = candidate - h_start;
            long long right_start = candidate + size;
            long long right_size = h_end - right_start;
            if (left_size > 0) {
                holes[i].size = left_size;
                if (right_size > 0) holes.insert(holes.begin() + i + 1, {right_start, right_size});
            } else {
                if (right_size > 0) holes[i] = {right_start, right_size};
                else holes.erase(holes.begin() + i);
            }
            return candidate;
        }
    }
    return -1;
}

void add_symbol(string name, bool is_array, bool is_param, string mod, long long start, long long end) {
    if (is_array && start > end) {
        yyerror(("Invalid array range for " + name).c_str());
        exit(1);
    }

    string key = (current_procedure == "") ? name : current_procedure + "@" + name;
    if (symbol_table.count(key)) {
        yyerror(("Redeclaration: " + name).c_str());
        exit(1);
    }
    
    Symbol s;
    s.is_array = is_array;
    s.is_param = is_param;
    s.is_passed_to_proc = false;
    s.start = start;
    s.end = end;
    s.mod = mod;
    s.is_iterator = false;
    
    if (mod == "O") s.is_initialized = false;
    else s.is_initialized = true; 

    // Allocation Logic
    long long alloc_size = 1;
    if (is_array && !is_param) {
         alloc_size = (end - start + 1) + 1; // +1 for Header for unsafe arrays
    }

    long long addr = -1;

    // Try Holes
    if (is_array && !is_param) {
         // Arrays need base >= start
         addr = find_hole(alloc_size, start);
    } else {
         // Scalars/Params can be anywhere (min_start=0)
         addr = find_hole(alloc_size, 0);
    }
    
    // Append if no hole
    if (addr == -1) {
        if (is_array && !is_param) {
            // Alignment
            if (memory_offset >= start) {
                // Natural fit
                addr = memory_offset;
                memory_offset += alloc_size;
            } else {
                long long gap = start - memory_offset;
                if (gap < 10000000) {
                    // Fill gap with hole
                    add_hole(memory_offset, gap);
                    addr = start;
                    memory_offset = start + alloc_size;
                } else {
                    // Gap too big -> Unsafe Alloc
                    addr = memory_offset;
                    memory_offset += alloc_size;
                    unsafety_detected = true;
                }
            }
        } else {
            // Scalar/Param
            addr = memory_offset;
            memory_offset += alloc_size;
        }
    }
    
    s.address = addr;
    symbol_table[key] = s;
    
    if (current_procedure != "" && is_param) {
        procedures_map[current_procedure].param_addresses.push_back(addr);
        procedures_map[current_procedure].param_mods.push_back(mod);
        procedures_map[current_procedure].param_is_array.push_back(is_array);
        procedures_map[current_procedure].param_names.push_back(name);
        
         if (mod == "T" && !is_array) {
            yyerror(("Parameter " + name + " marked T must be an array").c_str());
         }
         if (mod != "T" && is_array) {
             yyerror(("Array parameter " + name + " must be marked with T").c_str());
         }
    }
}

Symbol* get_variable(string name) {
    // Try current FOR scope (Iterate from innermost to outermost)
    // Local Loop shadows everything
    for (int i = current_for_stack.size() - 1; i >= 0; i--) {
        string for_key = "for_" + std::to_string(current_for_stack[i]) + "@" + name;
        if (symbol_table.find(for_key) != symbol_table.end()) {
            return &symbol_table[for_key];
        }
    }

    // Check substitution (Inlining)
    string sub_key = current_procedure + "@" + name;
    if (substitution_map.count(sub_key)) {
        return substitution_map[sub_key];
    }

    // Try local proc
    string key = current_procedure + "@" + name;
    if (symbol_table.find(key) != symbol_table.end()) {
        return &symbol_table[key];
    }
    
    // Global
    if (symbol_table.count(name)) return &symbol_table[name];
    
    return nullptr;
}
