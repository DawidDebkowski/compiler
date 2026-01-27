#ifndef TAC_BACKEND_HPP
#define TAC_BACKEND_HPP

#include "tac.hpp"

class TACBackend {
public:
    void process();

private:
    void emitLoad(int reg, Operand op);
    void emitStore(int reg, Operand op);
    
    void emitAdd(TACInstruction& instr);
    void emitSub(TACInstruction& instr);
    void emitMul(TACInstruction& instr);
    void emitDiv(TACInstruction& instr);
    void emitMod(TACInstruction& instr);
    void emitCopy(TACInstruction& instr);
    
    void emitJump(TACInstruction& instr);
    void emitJumpCond(TACInstruction& instr);
    
    void emitLoadArray(TACInstruction& instr);
    void emitStoreArray(TACInstruction& instr);
    
    void emitCall(TACInstruction& instr);
    void emitParam(TACInstruction& instr);
    void emitReturn(TACInstruction& instr);
    void emitPrologue(TACInstruction& instr);
};

#endif
