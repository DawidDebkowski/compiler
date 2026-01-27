#include "codegen.hpp"
#include <iostream>

vector<Instruction> code;
vector<long long> loop_stack;
vector<long long> if_stack;

long long addr_mul = -1;
long long addr_div = -1;
long long addr_mod = -1;

vector<long long> calls_mul;
vector<long long> calls_div;
vector<long long> calls_mod;

Symbol* reg_descriptors[8] = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};

// I
void emit(string opcode) {
    code.push_back({opcode, 0, false, ""});
}
// hate
void emit(string opcode, long long arg) {
    code.push_back({opcode, arg, true, ""});
}
// C
void emit(string opcode, string comment) {
    code.push_back({opcode, 0, false, comment});
}
// ++
void emit(string opcode, long long arg, string comment) {
    code.push_back({opcode, arg, true, comment});
}

// I
void add_comment(string comment) {
    if (code.empty()) return;
    if(code[code.size()-1].comment == "") {
        code[code.size()-1].comment = comment;
    } else {
        code[code.size()-1].comment += comment;
    }
}

// generates const to reg, uses only reg
void gen_const(int reg, BigInt value) {
    if (value < 0) value = 0; 
    
    emit("RST", reg);
    if (value == 0) return;
    
    unsigned int len = cln::integer_length(value);
    
    emit("INC", reg); // First 1 (MSB)
    
    for(int i = len - 2; i >= 0; i--) {
        emit("SHL", reg);
        if (cln::logbitp(i, value)) {
            emit("INC", reg);
        }
    }
}

// removes STORE x; LOAD x; if LOAD is not target of jumps,
// then correctly remaps jumps
void optimize_code() {
    bool changed = true;
    while (changed) {
        changed = false;
        vector<bool> is_target(code.size() + 1, false);
        for (const auto& instr : code) {
            if (instr.has_arg && (instr.opcode == "JUMP" || instr.opcode == "JZERO" || instr.opcode == "JPOS" || instr.opcode == "CALL")) {
                if (instr.arg >= 0 && instr.arg < (long long)is_target.size())
                    is_target[instr.arg] = true;
            }
        }

        vector<bool> to_delete(code.size(), false);
        for (size_t i = 0; i < code.size(); ++i) {
            if (to_delete[i]) continue;
            
            // STORE x, LOAD x -> Remove LOAD x
            if (code[i].opcode == "STORE" && i+1 < code.size()) {
                 if (code[i+1].opcode == "LOAD" && 
                     code[i].arg == code[i+1].arg && 
                     !to_delete[i] && 
                     !is_target[i+1]) { 
                     to_delete[i+1] = true;
                     changed = true;
                 }
            }
            
            // JUMP next
            if (code[i].opcode == "JUMP" && code[i].arg == (long long)(i + 1)) {
                 to_delete[i] = true;
                 changed = true;
            }
        }
        
        if (!changed) break;

        vector<Instruction> new_code;
        vector<long long> map_idx(code.size() + 1);
        long long new_idx = 0;
        
        for (size_t i = 0; i < code.size(); ++i) {
            map_idx[i] = new_idx;
            if (!to_delete[i]) {
                new_code.push_back(code[i]);
                new_idx++;
            }
        }
        map_idx[code.size()] = new_idx; 
        
        for (auto& instr : new_code) {
            if (instr.has_arg && (instr.opcode == "JUMP" || instr.opcode == "JZERO" || instr.opcode == "JPOS" || instr.opcode == "CALL")) {
                 instr.arg = map_idx[instr.arg];
            }
        }
        
        code = new_code;
    }
}
