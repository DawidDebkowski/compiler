#ifndef TYPES_HPP
#define TYPES_HPP

#include <string>
#include <vector>
#include <map>

using namespace std;

// Forward declaration if needed, or included here
struct Symbol {
    long long address;
    bool is_array;
    bool is_param;
    long long start;
    long long end;
    
    // Spec flags
    string mod; // "T", "I", "O", ""
    bool is_iterator = false;
    bool is_initialized = false;
};

struct ProcedureInfo {
    long long address;
    vector<long long> param_addresses;
    vector<string> param_mods;     // "T", "I", "O", ""
    vector<bool> param_is_array;
};

struct Instruction {
    string opcode;
    long long arg;
    bool has_arg;
};

// Helper for Identifier location used in Parser union
struct Location {
    long long address; 
    int reg;           
    Symbol* sym;
};

#endif
