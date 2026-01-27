#ifndef TAC_HPP
#define TAC_HPP

#include "types.hpp"
#include <string>
#include <vector>
#include <iostream>

enum class TACOp {
    ADD, SUB, MUL, DIV, MOD,
    COPY,           // dest = src1
    LOAD, STORE,    // LOAD: dest = *src1, STORE: *dest = src1
    ARR_L, ARR_S,   // ARR_L: dest = src1[src2], ARR_S: dest[src1] = src2
    JUMP, 
    JUMP_EQ, JUMP_NEQ, JUMP_LT, JUMP_GT, JUMP_LEQ, JUMP_GEQ,
    PARAM, CALL, RETURN,
    PROLOGUE,
    READ, WRITE,
    LABEL, HALT,
    NOPE            // No-op
};

struct Operand {
    enum Type { NONE, VAR, CONST, LABEL } type;
    BigInt val;        // For CONST
    std::string name;  // For VAR and LABEL
    Symbol* sym;       // For VAR, lookup in symtable

    Operand() : type(NONE), val(0), sym(nullptr) {}
    
    static Operand makeVar(std::string name, Symbol* s) {
        Operand op;
        op.type = VAR;
        op.name = name;
        op.sym = s;
        return op;
    }

    static Operand makeConst(BigInt v) {
        Operand op;
        op.type = CONST;
        op.val = v;
        return op;
    }

    static Operand makeLabel(std::string l) {
        Operand op;
        op.type = LABEL;
        op.name = l;
        return op;
    }

    bool isConstant() const { return type == CONST; }
    bool isVariable() const { return type == VAR; }
    
    std::string toString() const;
};

struct TACInstruction {
    TACOp op;
    Operand dest;
    Operand src1;
    Operand src2;
    std::string comment;

    TACInstruction(TACOp o, Operand d, Operand s1, Operand s2, std::string c = "")
        : op(o), dest(d), src1(s1), src2(s2), comment(c) {}
        
    std::string toString() const;
};

// Global TAC list
extern std::vector<TACInstruction> tac_program;

// Helper to generate temporary variable
Operand makeTemp();

// Helper to generate unique label
std::string makeLabel();

void emitTAC(TACOp op, Operand dest, Operand src1, Operand src2, std::string comment = "");

// Print TAC to stream (for debugging)
void printTAC(std::ostream& out);

#endif
