// Dawid Dębkowski 279714
#ifndef CODEGEN_HPP
#define CODEGEN_HPP

#include <string>
#include <vector>
#include "types.hpp"

using namespace std;

// Code Buffer
extern vector<Instruction> code;

// Math Lib Addresses
extern long long addr_mul;
extern long long addr_div;
extern long long addr_mod;

extern vector<long long> calls_mul;
extern vector<long long> calls_div;
extern vector<long long> calls_mod;

// Register Allocation (Simple Tracker)
// I didn't finally implement this, but I'm leaving it here
struct Symbol;
extern Symbol* reg_descriptors[8];

// Simple ra tracker
// It turned out almost useless
struct AccState {
    bool valid;
    string variable; // Name of variable currently held in r0
    long long value_const; // For constant tracking
    bool is_const;
};
extern AccState acc_tracker;
void reset_acc_tracker();

void emit(string opcode, string comment);
void emit(string opcode, long long arg, string comment);
void emit(string opcode);
void emit(string opcode, long long arg);
void add_comment(string comment);
void gen_const(int reg, BigInt value);
void optimize_code();

#endif
