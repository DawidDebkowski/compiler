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

void emit(string opcode);
void emit(string opcode, long long arg);
void gen_const(int reg, long long value);
void optimize_code();

#endif
