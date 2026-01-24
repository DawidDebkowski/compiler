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
    bool is_passed_to_proc; 
    long long start;
    long long end;
    
    // Spec flags
    string mod; // "T", "I", "O", ""
    bool is_iterator = false;
    bool is_initialized = false;
    
    // Address Optimization
    bool use_fast_access = false; // true if base >= start and we can use static offset
    long long constant_offset = 0; // base - start (if fast access)

    // Register Allocation
    int reg_in_use = -1; // -1 if in memory, 0-7 if in register
};

struct ProcedureInfo {
    long long address;
    long long times_called;
    long long ra_address; // Return address storage location
    vector<long long> param_addresses;
    vector<string> param_mods;     // "T", "I", "O", ""
    vector<bool> param_is_array;
};

struct Instruction {
    string opcode;
    long long arg;
    bool has_arg;
    string comment;
};


#endif
