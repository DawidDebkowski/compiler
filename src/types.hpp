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
    
    // Opt
    long long usage_count = 0;
    int cache_reg = -1;
};

struct Operand {
     bool is_const;
     long long val;
     int instructions_count;
};

struct ProcedureInfo {
    long long address;
    long long ra_address; // Return address storage location
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
