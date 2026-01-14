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
extern string current_procedure; 
extern string current_call_proc;
extern int current_arg_idx;

// Helper to access yyerror from C++ code
void yyerror(const char *s);

void add_symbol(string name, bool is_array, bool is_param, string mod, long long start, long long end);
Symbol* get_variable(string name);

#endif
