#ifndef SYMTABLE_HPP
#define SYMTABLE_HPP

#include <string>
#include <map>
#include <vector>
#include "types.hpp"

using namespace std;

// Globals for context
extern map<string, Symbol> symbol_table;
extern map<string, ProcedureInfo> procedures_map;

extern long long memory_offset;
extern long long lhs_hold_addr;
extern bool unsafety_detected; // Global flag for Unsafe Mode
extern string current_procedure; 
extern std::vector<int> current_for_stack;
extern int for_id_counter;

// Helper to access yyerror from C++ code
void yyerror(const char *s);

void add_symbol(string name, bool is_array, bool is_param, string mod, long long start, long long end);
Symbol* get_variable(string name);

// Inlining Support
void add_substitution(string key, Symbol* s);
void remove_substitution(string key);

#endif
