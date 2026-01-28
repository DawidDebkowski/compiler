#include "tac.hpp"
#include "symtable.hpp"
#include <sstream>

std::vector<TACInstruction> tac_program;
static int temp_counter = 0;
static int label_counter = 0;

Operand makeTemp() {
    std::string name = "_t" + std::to_string(temp_counter++);
    // Allocate memory for temp immediately in Naive approach
    add_symbol(name, false, false, "", 0, 0); 
    return Operand::makeVar(name, get_variable(name));
}

std::string makeLabel() {
    return "L" + std::to_string(label_counter++);
}

void emitTAC(TACOp op, Operand dest, Operand src1, Operand src2, std::string comment) {
    tac_program.emplace_back(op, dest, src1, src2, comment);
}

std::string Operand::toString() const {
    if (type == CONST) {
        // cln::cl_I printing is tricky without specific streams, let's cast if small or use cln output
        // For simplicity assuming it fits in long long for display or use <<
        std::stringstream ss;
        ss << val;
        return ss.str();
    } else if (type == VAR) {
        return name;
    } else if (type == LABEL) {
        return name;
    }
    return "";
}

std::string opToString(TACOp op) {
    switch (op) {
        case TACOp::ADD: return "ADD";
        case TACOp::SUB: return "SUB";
        case TACOp::MUL: return "MUL";
        case TACOp::DIV: return "DIV";
        case TACOp::MOD: return "MOD";
        case TACOp::COPY: return "COPY";
        case TACOp::LOAD: return "LOAD";
        case TACOp::STORE: return "STORE";
        case TACOp::ARR_L: return "ARR_L";
        case TACOp::ARR_S: return "ARR_S";
        case TACOp::JUMP: return "JUMP";
        case TACOp::JUMP_EQ: return "JUMP_EQ";
        case TACOp::JUMP_NEQ: return "JUMP_NEQ";
        case TACOp::JUMP_LT: return "JUMP_LT";
        case TACOp::JUMP_GT: return "JUMP_GT";
        case TACOp::JUMP_LEQ: return "JUMP_LEQ";
        case TACOp::JUMP_GEQ: return "JUMP_GEQ";
        case TACOp::PARAM: return "PARAM";
        case TACOp::CALL: return "CALL";
        case TACOp::RETURN: return "RETURN";
        case TACOp::PROLOGUE: return "PROLOGUE";
        case TACOp::READ: return "READ";
        case TACOp::WRITE: return "WRITE";
        case TACOp::LABEL: return "LABEL";
        case TACOp::HALT: return "HALT";
        case TACOp::NOPE: return "NOPE";
    }
    return "?";
}

std::string TACInstruction::toString() const {
    std::stringstream ss;
    if (op == TACOp::LABEL) {
        ss << dest.toString() << ":";
    } else {
        ss << "  " << opToString(op) << " ";
        if (dest.type != Operand::NONE) ss << dest.toString();
        if (src1.type != Operand::NONE) ss << ", " << src1.toString();
        if (src2.type != Operand::NONE) ss << ", " << src2.toString();
    }
    if (!comment.empty()) {
        ss << "  # " << comment;
    }
    return ss.str();
}

void printTAC(std::ostream& out) {
    for (const auto& instr : tac_program) {
        out << instr.toString();
        if(!instr.comment.empty()) {
            out << "#" << instr.comment;
        }
        out << "\n";
    }
}
