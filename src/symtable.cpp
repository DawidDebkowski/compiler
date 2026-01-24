#include "symtable.hpp"
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <list>
#include <algorithm>

map<string, Symbol> symbol_table;
map<string, ProcedureInfo> procedures_map;

struct FreeBlock {
    long long start;
    long long size;
};

std::list<FreeBlock> memory_holes;
long long memory_offset = 15; // Start a bit higher to give space for registers/scratch if needed
long long lhs_hold_addr = 1;

// Threshold to skip/fill: if gap is smaller than this, fill it with a hole and align array
const long long GAP_ALIGN_THRESHOLD = 50000; 

string current_procedure = ""; 
string current_call_proc = "";
int current_arg_idx = 0;

std::vector<int> current_for_stack;
int for_id_counter = 0;

void add_hole(long long start, long long size) {
    if (size <= 0) return;
    
    // Insert sorted by start address
    auto it = memory_holes.begin();
    while (it != memory_holes.end() && it->start < start) {
        it++;
    }
    memory_holes.insert(it, {start, size});
    
    // Merge contiguous holes
    for (auto it = memory_holes.begin(); it != memory_holes.end(); ) {
        auto next = std::next(it);
        if (next != memory_holes.end()) {
            if (it->start + it->size == next->start) {
                // Merge
                it->size += next->size;
                memory_holes.erase(next);
                continue; // Stay on 'it' to see if we can merge next one
            }
        }
        it++;
    }
}

long long alloc_hole(long long size, long long min_start) {
    for (auto it = memory_holes.begin(); it != memory_holes.end(); ++it) {
        if (it->size >= size && it->start >= min_start) {
            long long addr = it->start;
            
            // Update hole
            if (it->size > size) {
                it->start += size;
                it->size -= size;
            } else {
                memory_holes.erase(it);
            }
            return addr;
        }
    }
    return -1;
}

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
    s.is_array = is_array;
    s.is_param = is_param;
    s.is_passed_to_proc = false;
    s.start = start;
    s.end = end;
    s.mod = mod;
    s.is_iterator = false;
    
    // Default assumptions
    s.use_fast_access = false;
    s.constant_offset = 0;

    if (mod == "O") s.is_initialized = false;
    else s.is_initialized = true; 

    // ---- MEMORY ALLOCATION STRATEGY ----
    long long needed_size = 1;
    if (is_array && !is_param) {
         needed_size = (end - start + 1) + 1; // Always allocate Header
    }

    if (is_param) {
        long long addr = alloc_hole(1, 0);
        if (addr == -1) {
            addr = memory_offset;
            memory_offset++;
        }
        s.address = addr;
        s.use_fast_access = false; // Params are indirect
    } 
    else if (!is_array) {
        long long addr = alloc_hole(1, 0);
        if (addr == -1) {
            addr = memory_offset;
            memory_offset++;
        }
        s.address = addr;
        s.use_fast_access = false; 
    }
    else {
        // Global Array
        // We ALWAYS allocate a header at s.address.
        // Data starts at s.address + 1.
        // Fast Access is possible if (s.address + 1) >= start.
        // i.e., s.address >= start - 1.
        
        long long min_base_for_fast = start - 1;
        if (min_base_for_fast < 0) min_base_for_fast = 0; // base must be valid
        
        // 1. Try to find a hole that satisfies fast access
        long long good_hole = alloc_hole(needed_size, min_base_for_fast);
        if (good_hole != -1) {
            s.address = good_hole;
            s.use_fast_access = true;
            s.constant_offset = (s.address + 1) - start;
        }
        else {
             // 2. Check linear memory
             if (memory_offset >= min_base_for_fast) {
                 // memory_offset is already high enough.
                 s.address = memory_offset;
                 s.use_fast_access = true;
                 s.constant_offset = (s.address + 1) - start;
                 
                 memory_offset += needed_size;
             } 
             else {
                 // memory_offset < min_base_for_fast.
                 // Gap analysis
                 long long gap = min_base_for_fast - memory_offset;
                 if (gap <= GAP_ALIGN_THRESHOLD) {
                     // Fill gap with hole
                     add_hole(memory_offset, gap);
                     
                     s.address = min_base_for_fast;
                     s.use_fast_access = true;
                     s.constant_offset = (s.address + 1) - start; // Should be 0 usually
                     
                     memory_offset = s.address + needed_size;
                 }
                 else {
                      // fallback: Gap too big. Use Safe/Slow Mode.
                      // Just allocate anywhere.
                      
                      long long any_hole = alloc_hole(needed_size, 0);
                      if (any_hole != -1) {
                          s.address = any_hole;
                      } else {
                          s.address = memory_offset;
                          memory_offset += needed_size;
                      }
                      
                      s.use_fast_access = false; 
                 }
             }
        }
    }
    // ------------------------------------

    symbol_table[key] = s;
    
    if (current_procedure != "" && is_param) {
        procedures_map[current_procedure].param_addresses.push_back(s.address);
        procedures_map[current_procedure].param_mods.push_back(mod);
        procedures_map[current_procedure].param_is_array.push_back(is_array);
        
        if (mod == "T" && !is_array) yyerror(("Parameter " + name + " marked T must be an array").c_str());
        if (mod != "T" && is_array) yyerror(("Array parameter " + name + " must be marked with T").c_str());
    }
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
