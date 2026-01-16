#ifndef CODEGEN_HPP
#define CODEGEN_HPP

#include <string>
#include <vector>
#include "types.hpp"

using namespace std;

// Code Buffer
extern vector<Instruction> code;
extern vector<long long> loop_stack;
extern vector<long long> if_stack;

// Math Lib Addresses
extern long long addr_mul;
extern long long addr_div;
extern long long addr_mod;

extern vector<long long> calls_mul;
extern vector<long long> calls_div;
extern vector<long long> calls_mod;

// Register Allocation (Simple Tracker)
// Maps Register ID (0-7) to a Symbol or NULL if free/trash.
// Used to decide if we need to spill before Math Kernel calls.
// 4=e, 5=f, 6=g are clobbered by Kernel.
struct Symbol; // Forward declaration
extern Symbol* reg_descriptors[8];

void emit(string opcode);
void emit(string opcode, long long arg);
void gen_const(int reg, long long value);
void optimize_code();

#endif
