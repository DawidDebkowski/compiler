#include "tac_backend.hpp"
#include "codegen.hpp"
#include "symtable.hpp"
#include "math_kernel.hpp"
#include <map>
#include <iostream>

std::map<std::string, long long> label_defs;
std::map<std::string, std::vector<long long>> label_refs;
std::vector<Operand> param_queue;

void TACBackend::process() {
    // Initialize Stack Pointer (mem[0]) to strict memory_offset + 1000 or similar
    // Actually, memory_offset is the next free address for variables.
    // Stack can grow from there.
    // gen_const(0, memory_offset + 10);
    // emit("STORE", 0);

    label_defs.clear();
    label_refs.clear();
    param_queue.clear();
    
    for (size_t i = 0; i < tac_program.size(); ++i) {
        TACInstruction& instr = tac_program[i];
        // std::cerr << "Processing op: " << (int)instr.op << "\n";
        
        if (!instr.comment.empty()) {
            add_comment(instr.comment);
        }

        switch (instr.op) {
            case TACOp::LABEL:
                label_defs[instr.dest.name] = code.size();
                break;
            case TACOp::ADD: emitAdd(instr); break;
            case TACOp::SUB: emitSub(instr); break;
            case TACOp::MUL: emitMul(instr); break;
            case TACOp::DIV: emitDiv(instr); break;
            case TACOp::MOD: emitMod(instr); break;
            case TACOp::COPY: emitCopy(instr); break;
            case TACOp::LOAD: 
                // dest = *src1
                emitLoad(0, instr.src1); // ra <- addr
                emit("RLOAD", 0);         // ra <- *ra
                emitStore(0, instr.dest);
                break;
            case TACOp::STORE:
                // *dest = src1
                // We need: rb <- addr, ra <- val. STORE 1 (mem[rb] <- ra)
                emitLoad(0, instr.dest); // ra <- addr
                emit("SWP", 1);          // rb <- addr
                emitLoad(0, instr.src1); // ra <- val
                emit("RSTORE", 1);
                break;
            case TACOp::ARR_L: emitLoadArray(instr); break;
            case TACOp::ARR_S: emitStoreArray(instr); break;
            case TACOp::JUMP: emitJump(instr); break;
            case TACOp::JUMP_EQ: 
            case TACOp::JUMP_NEQ:
            case TACOp::JUMP_LT:
            case TACOp::JUMP_GT:
            case TACOp::JUMP_LEQ:
            case TACOp::JUMP_GEQ: emitJumpCond(instr); break;
            case TACOp::PARAM: emitParam(instr); break;
            case TACOp::CALL: emitCall(instr); break;
            case TACOp::RETURN: emitReturn(instr); break;
            case TACOp::PROLOGUE: emitPrologue(instr); break;
            case TACOp::READ: emit("READ"); emitStore(0, instr.dest); break;
            case TACOp::WRITE: emitLoad(0, instr.src1); emit("WRITE"); break;
            case TACOp::HALT: emit("HALT"); break;
            case TACOp::NOPE: break;
        }
    }
    
    // Backpatching
    for (auto const& [label, refs] : label_refs) {
        if (label_defs.count(label)) {
            long long addr = label_defs[label];
            for (long long ref : refs) {
                code[ref].arg = addr;
            }
        } else {
            if (label != "__read" && label != "__write")
                std::cerr << "Warning: Undefined label " << label << "\n";
        }
    }

    // Generate Kernel Functions and Backpatch CALLs
    if (!calls_mul.empty()) {
          generate_mul();
          for(long long idx : calls_mul) code[idx].arg = addr_mul;
    }
    if (!calls_div.empty()) {
          generate_div();
          for(long long idx : calls_div) code[idx].arg = addr_div;
    }
    if (!calls_mod.empty()) {
          generate_mod();
          for(long long idx : calls_mod) code[idx].arg = addr_mod;
    }
}

// Helpers
void TACBackend::emitLoad(int reg, Operand op) {
    if (op.type == Operand::CONST) {
        gen_const(reg, op.val);
    } else if (op.type == Operand::VAR) {
        emit("LOAD", op.sym->address);
        // if(op.sym->is_param) {
        //     emit("RLOAD", 0);
        // }
        if (reg != 0) emit("SWP", reg);
    }
}

void TACBackend::emitStore(int reg, Operand op) {
    if (op.type == Operand::VAR) {
        if (reg != 0) emit("SWP", reg); // Move val to ra
        emit("STORE", op.sym->address);
        if (reg != 0) emit("SWP", reg); // Restore? Not strictly needed in naive
    }
}

void TACBackend::emitAdd(TACInstruction& instr) {
    // dest = src1 + src2
    emitLoad(1, instr.src1); // rb = src1
    emitLoad(0, instr.src2); // ra = src2
    emit("ADD", 1);          // ra += rb
    emitStore(0, instr.dest);
}

void TACBackend::emitSub(TACInstruction& instr) {
    // dest = src1 - src2
    emitLoad(1, instr.src2); // rb = src2
    emitLoad(0, instr.src1); // ra = src1
    emit("SUB", 1);
    emitStore(0, instr.dest);
}

void TACBackend::emitCopy(TACInstruction& instr) {
    emitLoad(0, instr.src1);
    emitStore(0, instr.dest);
}

// --- OPTIMIZATIONS (Ported from BinaryOpNode) ---

void TACBackend::emitMul(TACInstruction& instr) {
    // Check constants
    bool leftConst = instr.src1.isConstant(); 
    bool rightConst = instr.src2.isConstant();
    
    // Optimization logic
    BigInt val = 0;
    Operand variableOp;
    
    if (rightConst) {
        val = instr.src2.val;
        variableOp = instr.src1;
    } else if (leftConst) {
        val = instr.src1.val;
        variableOp = instr.src2;
    }
    
    if (leftConst || rightConst) {
        if (val == 0) {
            emit("RST", 0);
            emitStore(0, instr.dest);
            return;
        }
        if (val == 1) {
            emitLoad(0, variableOp);
            emitStore(0, instr.dest);
            return;
        }
        if (val > 0 && (val & (val - 1)) == 0) {
             emitLoad(0, variableOp);
             int shifts = cln::integer_length(val) - 1;
             for(int k=0; k<shifts; k++) emit("SHL", 0, "mul by 2^k");
             emitStore(0, instr.dest);
             return;
        }
        // Small Constants
        if (val > 0 && val <= 10) {
             long long v_small = cln::cl_I_to_long(val);
             emitLoad(0, variableOp); // ra = x
             
             // Copy ra to rb
             emit("RST", 1); emit("SWP", 1); emit("ADD", 1); // rb = x
             
             if (v_small == 3) { emit("SHL", 0); emit("ADD", 1); }
             else if (v_small == 5) { emit("SHL", 0); emit("SHL", 0); emit("ADD", 1); }
             else if (v_small == 6) { emit("SHL", 0); emit("ADD", 1); emit("SHL", 0); }
             else if (v_small == 7) { emit("SHL", 0); emit("SHL", 0); emit("SHL", 0); emit("SUB", 1); }
             else if (v_small == 9) { emit("SHL", 0); emit("SHL", 0); emit("SHL", 0); emit("ADD", 1); }
             else if (v_small == 10) { emit("SHL", 0); emit("SHL", 0); emit("ADD", 1); emit("SHL", 0); }
             
             emitStore(0, instr.dest);
             return;
        }
    }
    
    // Default MUL call
    emitLoad(1, instr.src1);
    emitLoad(2, instr.src2);
    emit("CALL", 0); calls_mul.push_back(code.size()-1); 
    
    emit("SWP", 1); // Move Result r1 -> r0
    emitStore(0, instr.dest);
}

void TACBackend::emitDiv(TACInstruction& instr) {
    if (instr.src2.isConstant()) {
        BigInt val = instr.src2.val;
        if (val == 0) {
            emit("RST", 0); emitStore(0, instr.dest); return;
        }
        if (val == 1) {
            emitLoad(0, instr.src1); emitStore(0, instr.dest); return;
        }
        if (val > 0 && (val & (val - 1)) == 0) {
             emitLoad(0, instr.src1);
             int shifts = cln::integer_length(val) - 1;
             for(int k=0; k<shifts; k++) emit("SHR", 0, "optimizing left div 2");
             emitStore(0, instr.dest); return;
        }
    }
    
    emitLoad(1, instr.src1);
    emitLoad(2, instr.src2);
    emit("CALL", 0); calls_div.push_back(code.size()-1); 
    emit("SWP", 1); // r1 -> r0
    emitStore(0, instr.dest);
}

void TACBackend::emitMod(TACInstruction& instr) {
    if (instr.src2.isConstant()) {
        BigInt val = instr.src2.val;
        if (val > 0 && (val & (val - 1)) == 0) {
            emitLoad(0, instr.src1); // r0 = x
            
            // r1 = x
            emit("RST", 1); emit("SWP", 1); emit("ADD", 1);
            
            // r1 = x / 2^k
            int shifts = cln::integer_length(val) - 1;
            for(int k=0; k<shifts; k++) emit("SHR", 1);
            
            // r1 = (x / 2^k) * 2^k
            for(int k=0; k<shifts; k++) emit("SHL", 1);
            
            // r0 = r0 - r1
            emit("SUB", 1);
            emitStore(0, instr.dest);
            return;
        }
    }
    
    emitLoad(1, instr.src1);
    emitLoad(2, instr.src2);
    emit("CALL", 0); calls_mod.push_back(code.size()-1); 
    emit("SWP", 1); // r1 -> r0
    emitStore(0, instr.dest);
}

void TACBackend::emitLoadArray(TACInstruction& instr) {
    // dest = base[index]
    
    Symbol* varSym = instr.src1.sym;
    long long baseAddr = varSym->address;
    long long startIdx = varSym->start;
    
    emitLoad(0, instr.src2); // ra = idx
    
    long long offset = baseAddr + 1 - startIdx;
    
    if (offset != 0) {
        gen_const(1, offset);
        emit("ADD", 1);
    }
    
    // Now ra = Element Address
    emit("RLOAD", 0); // Load value at Element Address
    emitStore(0, instr.dest);
}

void TACBackend::emitStoreArray(TACInstruction& instr) {
    // base[index] = src2
    
    Symbol* varSym = instr.dest.sym; // Dest is Base for ARR_S (First arg)
    long long baseAddr = varSym->address;
    long long startIdx = varSym->start;
    
    emitLoad(0, instr.src1); // ra = idx
    
    long long offset = baseAddr + 1 - startIdx;
    if (offset != 0) {
        gen_const(1, offset);
        emit("ADD", 1);
    }
    
    emit("SWP", 1); // rb = Address
    
    emitLoad(0, instr.src2); // ra = Value
    
    emit("RSTORE", 1);
}

void TACBackend::emitJump(TACInstruction& instr) {
    long long dummy = 0;
    emit("JUMP", dummy);
    label_refs[instr.dest.name].push_back(code.size() - 1);
}

void TACBackend::emitJumpCond(TACInstruction& instr) {
    TACOp op = instr.op;
    std::string label = instr.dest.name;
    long long dummy = 0;
    
    // Load safely: r1 then r0
    emitLoad(1, instr.src2);
    emitLoad(0, instr.src1);
    
    if (op == TACOp::JUMP_GT) {
        emit("SUB", 1);
        emit("JPOS", dummy);
        label_refs[label].push_back(code.size() - 1);
    }
    else if (op == TACOp::JUMP_LT) {
        emit("SWP", 1); // ra=b, rb=a
        emit("SUB", 1); // b-a
        emit("JPOS", dummy);
        label_refs[label].push_back(code.size() - 1);
    }
    else if (op == TACOp::JUMP_GEQ) {
         emit("SWP", 1); 
         emit("SUB", 1); 
         emit("JZERO", dummy);
         label_refs[label].push_back(code.size() - 1);
    }
    else if (op == TACOp::JUMP_LEQ) {
         emit("SUB", 1);
         emit("JZERO", dummy);
         label_refs[label].push_back(code.size() - 1);
         
         // Also jump if NEG (since SUB is a-b, NEG means a < b)
         // Wait, SUB is max(0, a-b). So if result is 0, a<=b. Correct.
         // JZERO is sufficient.
    }
    else if (op == TACOp::JUMP_EQ) {
        // Jump if ra == rb
        emit("SUB", 1);
        emit("JPOS", dummy); 
        long long fail1 = code.size() - 1;
        
        emitLoad(0, instr.src2); 
        emitLoad(1, instr.src1); 
        emit("SUB", 1);
        emit("JPOS", dummy); 
        long long fail2 = code.size() - 1;
        
        emit("JUMP", dummy);
        label_refs[label].push_back(code.size() - 1);
        
        code[fail1].arg = code.size();
        code[fail2].arg = code.size();
    }
    else if (op == TACOp::JUMP_NEQ) {
        // Jump if ra != rb
        // if ra - rb > 0 then jump
        // if rb - ra > 0 then jump
        
        emit("SUB", 1);
        emit("JPOS", dummy);
        label_refs[label].push_back(code.size() - 1);
        
        emitLoad(0, instr.src2);
        emitLoad(1, instr.src1);
        emit("SUB", 1);
        emit("JPOS", dummy);
        label_refs[label].push_back(code.size() - 1);
    }
}

void TACBackend::emitCall(TACInstruction& instr) {
    std::string procLabel = instr.src1.name;
    std::string procName = procLabel;
    if (procName.rfind("proc_", 0) == 0) {
        procName = procName.substr(5);
    }

    if (procedures_map.count(procName)) {
        ProcedureInfo& info = procedures_map[procName];
        for (size_t i = 0; i < param_queue.size() && i < info.param_addresses.size(); ++i) {
             long long slotAddr = info.param_addresses[i];
             emitLoad(0, param_queue[i]);
             emit("STORE", slotAddr);
        }
    }
    param_queue.clear();
    
    long long dummy = 0;
    emit("CALL", dummy);
    label_refs[procLabel].push_back(code.size() - 1);
}

void TACBackend::emitParam(TACInstruction& instr) {
    param_queue.push_back(instr.src1);
}

void TACBackend::emitReturn(TACInstruction& instr) {
    long long ra_slot = cln::cl_I_to_long(instr.dest.val);
    
    // Restore RA from valid slot
    emit("LOAD", ra_slot); // ra <- mem[ra_slot]
    emit("RTRN");
}

void TACBackend::emitPrologue(TACInstruction& instr) {
    long long ra_slot = cln::cl_I_to_long(instr.dest.val);
    
    // Store RA (currently in ra/r0) to slot
    // RA is pushed to r0 at CALL time by 'r[0] = lr+1' in VM
    emit("STORE", ra_slot);
}
