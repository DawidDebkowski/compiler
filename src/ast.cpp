#include "ast.hpp"
#include "codegen.hpp"
#include "symtable.hpp"
#include "math_kernel.hpp"
#include <iostream>

extern void yyerror(const char* s);

void NumberNode::codegen_to_reg(int reg) {
    gen_const(reg, value);
}

// uses 0-2 regs
void IdentifierNode::codegen_address(int reg) {
    Symbol* s = get_variable(name);
    if (!s) {
        string msg = "Undefined variable " + name;
        yyerror(msg.c_str());
        exit(1);
    }

    if (!is_array) {
        if (s->is_param) {
            // Parameter passed by reference (address is at s->address)
            // Load the ADDRESS stored in the parameter slot
            emit("LOAD", s->address); 
            if (reg != 0) { emit("SWP", reg); emit("RST", 0); emit("ADD", reg);} 
            if (reg != 0) {
                 emit("SWP", reg);
            }
        } else {
            // Global scalar
            gen_const(reg, s->address);
        }
    } else {
        // Array Access
        if (is_index_const) {
            // arr[num]
            long long offset = index_val - s->start;
             if (s->is_param) {
                 emit("LOAD", s->address); // Load base address
                 gen_const(2, offset);
                 emit("ADD", 2); // Base + Offset
             } else {
                 gen_const(0, s->address + offset); // Static address
             }
        } else {
             // arr[var]
             // Calculate Index Value first

            // Should be IdentifierNode*
            Symbol* idx = get_variable(index_name);
            
            if (idx->is_param) {
                emit("LOAD", idx->address); // load address of param
                emit("RLOAD", 0); // load value
            } else {
                emit("LOAD", idx->address);
            }
             
             // Calc offset
             if (s->start > 0) {
                 gen_const(2, s->start);
                 emit("SUB", 2);
             }
             
             if (s->is_param) {
                 // Param is address of array start
                 emit("SWP", 2); // r2 = Index - Start
                 emit("LOAD", s->address); // r0 = Base Address
                 emit("ADD", 2); // r0 = Base + Index - Start
             } else {
                 gen_const(2, s->address);
                 emit("ADD", 2);
             }
        }
        
        if (reg != 0) emit("SWP", reg); 
    }
}

void IdentifierNode::codegen_to_reg(int reg) {
    Symbol* s = get_variable(name);
    if (!s) { yyerror(("Undefined variable " + name).c_str()); exit(1); }
    
    if (!is_array) {
        if (s->is_param) {
             emit("LOAD", s->address); // Load Address
             emit("RLOAD", 0); // Load Value
        } else {
             emit("LOAD", s->address);
        }
    } else {
        codegen_address(0); // Address in r0
        emit("RLOAD", 0);   // Value in r0
    }
    
    if (reg != 0) emit("SWP", reg);
}

// --- BinaryOpNode ---

void BinaryOpNode::codegen_to_reg(int reg) {
    // Standard BinOp: Left in r1, Right in r0
    
    // Check for Constant Power of 2
    // TODO: special cases for 1-10
    if (op == BinaryOp::MULT) {
        if (right->is_constant()) {
             long long v = right->evaluate();
             if (v > 0 && (v & (v - 1)) == 0) {
                 left->codegen_to_reg(0);
                 int shifts = 0;
                 while (v > 1) { v >>= 1; shifts++; }
                 for(int k=0; k<shifts; k++) emit("SHL", 0, "optimizing right mult 2");
                 if (reg != 0) emit("SWP", reg, "optimizing mult 2 with wrong register");
                 return;
             }
        }
        if (left->is_constant()) {
             long long v = left->evaluate();
             if (v > 0 && (v & (v - 1)) == 0) {
                 right->codegen_to_reg(0);
                 int shifts = 0;
                 while (v > 1) { v >>= 1; shifts++; }
                 for(int k=0; k<shifts; k++) emit("SHL", 0, "optimizing left mult 2");
                 if (reg != 0) emit("SWP", reg, "optimizing mult 2 with wrong register");
                 return;
             }
        }
    }
    if (op == BinaryOp::DIV && right->is_constant()) {
         long long v = right->evaluate();
         if (v > 0 && (v & (v - 1)) == 0) {
             left->codegen_to_reg(0);
             int shifts = 0;
             while (v > 1) { v >>= 1; shifts++; }
             for(int k=0; k<shifts; k++) emit("SHR", 0, "optimizing left div 2");
             if (reg != 0) emit("SWP", reg, "with wrong register DIV");
             return;
         }
    }
    left->codegen_to_reg(1);
    right->codegen_to_reg(0);
    
    switch(op) {
        case BinaryOp::PLUS:
            emit("ADD", 1); 
            break;
        case BinaryOp::MINUS:
            right->codegen_to_reg(1);
            left->codegen_to_reg(0);
            emit("SUB", 1);
            break;
        case BinaryOp::MULT:
            left->codegen_to_reg(1);
            right->codegen_to_reg(2);    
            emit("CALL", 0); calls_mul.push_back(code.size()-1); emit("SWP", 1);
            break;
        case BinaryOp::DIV:
            left->codegen_to_reg(1);
            right->codegen_to_reg(2);    
            emit("CALL", 0); calls_div.push_back(code.size()-1); emit("SWP", 1);
            break;
        case BinaryOp::MOD:
            left->codegen_to_reg(1);
            right->codegen_to_reg(2);    
            emit("CALL", 0); calls_mod.push_back(code.size()-1); emit("SWP", 1);
            break;
    }
    
    if (reg != 0) emit("SWP", reg);
}

void BinaryOpNode::validate() {
    left->validate();
    right->validate();
}


void AssignmentNode::validate() {
    // Check iterator modification
    Symbol* s = get_variable(target->name);
    if (s) {
        if (s->is_iterator) { yyerror("Cannot modify loop iterator"); exit(1); }
        s->is_initialized = true;
    }
    target->validate();
    expr->validate(); 
}

void AssignmentNode::codegen() {
    // Safe way: Evaluate Expr first
    // If Assign: A[i] := B + C.
    // Calc B+C -> Result in r0.
    // SWP 1 (Save result).
    // Calc Address of A[i] -> r0.
    // SWP 1 (Addr in r1, Result in r0).
    // STORE/RSTORE. 
    
    if (target->is_array || (get_variable(target->name)->is_param)) {
        // Compute Address of Target
        expr->codegen_to_reg(0);

        emit("SWP", 3);

        target->codegen_address(0); 
        
        // address in r0, value in r3
        emit("SWP", 3);

        // address in r3, value in r0
        emit("RSTORE", 3);
    } else {
        // Simple global scalar
        expr->codegen_to_reg(0);
        Symbol* s = get_variable(target->name);
        emit("STORE", s->address);
    }
}

// --- Read / Write ---

void ReadNode::codegen() {
    Symbol* s = get_variable(target->name);
    
    if (target->is_array || s->is_param) {
        // Compute Address
        target->codegen_address(1);
        
        emit("READ"); // Value in r0
        
        emit("RSTORE", 1);
    } else {
        emit("READ");
        emit("STORE", s->address);
    }
}

void ReadNode::validate() {
    Symbol* s = get_variable(target->name);
    if (s && s->is_iterator) { yyerror("Cannot read into loop iterator"); exit(1); }
    if (s && s->mod == "I") { yyerror("Cannot read into I parameter"); exit(1); }
    if (s) s->is_initialized = true;
}

void WriteNode::codegen() {
    expr->codegen_to_reg(0);
    emit("WRITE");
}

void WriteNode::validate() {
    expr->validate();
}

void ConditionNode::validate() {
    left->validate();
    right->validate();
}

void ConditionNode::codegen_jump_false(long long target_instruction_index) {
     switch(op) {
         case ConditionOp::EQ:
             // Jump if L != R
             // Optimization: VAR == 0 -> Jump if VAR != 0 (VAR > 0)
             if(right->is_constant() && right->evaluate() == 0) {
                 left->codegen_to_reg(0);
                 emit("JPOS", 0);
             } else if(left->is_constant() && left->evaluate() == 0) {
                 right->codegen_to_reg(0);
                 emit("JPOS", 0);
             } else {
                 // |L-R| > 0 => NEQ (jump)
                 left->codegen_to_reg(1);  // L -> r1
                 right->codegen_to_reg(0); // R -> r0
                 
                 emit("SWP", 2);  // r0(R) -> r2
                 
                 // Calc L - R
                 emit("RST", 0); emit("ADD", 1); emit("SUB", 2); emit("SWP", 3); // r3 = max(L-R, 0)
                 
                 // Calc R - L
                 emit("RST", 0); emit("ADD", 2); emit("SUB", 1); emit("ADD", 3); // r0 = max(R-L, 0) + r3
                 
                 emit("JPOS", 0); // If |L-R| > 0, they are unequal -> Jump
             }
             break;

         case ConditionOp::NEQ:
             // Jump if L == R
             // Optimization: VAR != 0 -> Jump if VAR == 0
             if(right->is_constant() && right->evaluate() == 0) {
                 left->codegen_to_reg(0);
                 emit("JZERO", 0);
             } else if(left->is_constant() && left->evaluate() == 0) {
                 right->codegen_to_reg(0);
                 emit("JZERO", 0);
             } else {
                 // |L-R| == 0 => EQ (jump)
                 left->codegen_to_reg(1);
                 right->codegen_to_reg(0);
                 
                 emit("SWP", 2); 
                 emit("RST", 0); emit("ADD", 1); emit("SUB", 2); emit("SWP", 3);
                 emit("RST", 0); emit("ADD", 2); emit("SUB", 1); emit("ADD", 3);
                 
                 emit("JZERO", 0);
             }
             break;

         case ConditionOp::GT:
             right->codegen_to_reg(1);// R -> r1
             left->codegen_to_reg(0); // L -> r0
             
             emit("SUB", 1, ">"); // L - R
             
             emit("JZERO", 0); // If 0, then L <= R (False) -> Jump
             break;

          case ConditionOp::LT:
              // L < R. Jump if False (L >= R).
              // L >= R <=> R - L == 0 (3 - 5 = 0)
              
              left->codegen_to_reg(1);  // L -> r1
              right->codegen_to_reg(0); // R -> r0
              
              emit("SUB", 1); // R - L
              
              emit("JZERO", 0, "<");
              break;

          case ConditionOp::GEQ:
               // L >= R. Jump if False (L < R).
               // L < R <=> R - L > 0
               
               left->codegen_to_reg(1);  // L -> r1
               right->codegen_to_reg(0); // R -> r0
               
               emit("SUB", 1); // R - L
               
               emit("JPOS", 0, ">="); // If R - L > 0, then R > L (L < R, False) -> Jump
               break;

          case ConditionOp::LEQ:
               // L <= R. Jump if False (L > R).
               // L > R <=> L - R > 0.
               
               right->codegen_to_reg(1);
               left->codegen_to_reg(0);
               
               emit("SUB", 1); // L - R
               
               emit("JPOS", 0, "<="); // If L - R > 0, then L > R (False) -> Jump
               break;
     }
}

void IfNode::codegen() {
    // Condition
    long long jump_idx_pos;
    condition->codegen_jump_false(0);
    jump_idx_pos = code.size() - 1;
    
    // Then Block
    for(auto s : then_block) s->codegen();
    
    long long jump_over_else = -1;
    if (!else_block.empty()) {
        emit("JUMP", 0);
        jump_over_else = code.size() - 1;
    }
    
    // Backpatch False Jump
    code[jump_idx_pos].arg = code.size();
    
    // Else
    if (!else_block.empty()) {
        for(auto s : else_block) s->codegen();
        code[jump_over_else].arg = code.size();
    }
}

void IfNode::validate() {
    condition->validate();
    for(auto s : then_block) s->validate();
    for(auto s : else_block) s->validate();
}

// --- Loops ---

void WhileNode::codegen() {
    long long start = code.size();
    
    condition->codegen_jump_false(0);
    long long jump_out = code.size()-1;
    
    for(auto s : body) s->codegen();
    
    emit("JUMP", start);
    code[jump_out].arg = code.size();
}

void WhileNode::validate() {
    condition->validate();
    for(auto s : body) s->validate();
}

void RepeatNode::codegen() {
     long long start = code.size();
     for(auto s : body) s->codegen();
     
     condition->codegen_jump_false(0);
     long long jump_instr = code.size()-1;
     code[jump_instr].arg = start;
}

void RepeatNode::validate() {
     condition->validate();
     for(auto s : body) s->validate();
}

// --- ForNode ---
// FOR i FROM start TO end DO ...
// FOR i FROM start DOWNTO end DO ...
void ForNode::codegen() {
    current_for_stack.push_back(for_id);
    
    Symbol* iter = get_variable(iterator);
    
    // Init: iter := start
    start_val->codegen_to_reg(0);
    emit("STORE", iter->address);
    
    // Calculate Count / Limit Logic
    // TO: count = end - start + 1
    // DOWNTO: count = start - end + 1
    
    end_val->codegen_to_reg(1); // r1 = end
    emit("LOAD", iter->address); // r0 = start
    
    if (!downto) {
        // TO: r1 - r0 + 1
        emit("SWP", 1); // r1=start, r0=end
        emit("SUB", 1); // end - start
    } else {
        // DOWNTO: r0 - r1 + 1
        emit("SUB", 1); // start - end
    }
    emit("INC", 0);
    
    long long count_addr = memory_offset++;
    emit("STORE", count_addr);
    
    long long loop_start = code.size();
    
    // Check Count
    emit("LOAD", count_addr);
    emit("JZERO", 0);
    long long jump_out = code.size()-1;
    
    // Body
    for(auto s : body) s->codegen();
    
    // Decrement Count
    emit("LOAD", count_addr);
    emit("DEC", 0);
    emit("STORE", count_addr);
    
    // Step Iterator
    emit("LOAD", iter->address);
    if (!downto) emit("INC", 0); else emit("DEC", 0);
    emit("STORE", iter->address);
    
    emit("JUMP", loop_start);
    code[jump_out].arg = code.size();
    
    current_for_stack.pop_back();
}

void ForNode::validate() {
    for_id = ++for_id_counter;
    current_for_stack.push_back(for_id);
    
    string iter_key = "for_" + std::to_string(for_id) + "_" + iterator;
    Symbol s;
    s.address = memory_offset++;
    s.is_array = false;
    s.is_param = false;
    s.start = 0;
    s.end = 0;
    s.mod = "";
    s.is_iterator = true;
    s.is_initialized = true;
    symbol_table[iter_key] = s;
    
    start_val->validate();
    end_val->validate();
    for(auto s : body) s->validate();
    
    current_for_stack.pop_back();
}

// --- ProcCall ---

void ProcCallNode::codegen() {
    // Push Args
    ProcedureInfo& info = procedures_map[proc_name];
    for (size_t i=0; i<args.size(); i++) {
        long long param_addr = info.param_addresses[i];
        
        // Handle Identifier
        if (auto* arg = dynamic_cast<IdentifierNode*>(args[i])) {
            Symbol* s = get_variable(arg->name);
            if (s->is_param) {
                emit("LOAD", s->address); 
                emit("STORE", param_addr); 
            } else {
                // Pass Address
                // Subtract start index to ensure 0-based indexing inside procedure matches real memory layout
                long long adjusted_addr = s->address;
                if (s->is_array) {
                    adjusted_addr -= s->start;
                }
                gen_const(0, adjusted_addr);
                emit("STORE", param_addr);
            }
        }
        // Handle Number
        else if (auto* num = dynamic_cast<NumberNode*>(args[i])) {
             long long temp_addr = memory_offset++;
             gen_const(0, num->value);
             emit("STORE", temp_addr);
             
             gen_const(0, temp_addr);
             emit("STORE", param_addr);
        }
    }
    emit("CALL", info.address);
}

void ProcCallNode::validate() {
     if (procedures_map.find(proc_name) == procedures_map.end()) {
         yyerror(("Undefined procedure " + proc_name).c_str());
         exit(1);
     }
     ProcedureInfo& info = procedures_map[proc_name];
     if (args.size() != info.param_addresses.size()) {
         yyerror("Wrong argument count"); exit(1);
     }
     // Check Types
     for (size_t i=0; i<args.size(); i++) {
         if (auto* arg = dynamic_cast<IdentifierNode*>(args[i])) {
             Symbol* s = get_variable(arg->name);
             if (!s) { yyerror("Undefined arg variable"); exit(1); }
             if (info.param_is_array[i] && !s->is_array) { yyerror("Expected array"); exit(1); }
             if (!info.param_is_array[i] && s->is_array) { yyerror("Expected scalar"); exit(1); }
         } 
         else if (dynamic_cast<NumberNode*>(args[i])) {
             if (info.param_is_array[i]) { yyerror("Expected array, got number"); exit(1); }
         }
     }
}

// --- Procedure & Root ---

void ProcedureNode::codegen() {
    current_procedure = name;
    procedures_map[name].address = code.size();
    long long ra = memory_offset++;
    procedures_map[name].ra_address = ra;
    emit("STORE", ra);
    
    for(auto s : body) s->codegen();
    
    emit("LOAD", ra);
    emit("RTRN");
    current_procedure = "";
}

void ProcedureNode::validate() {
     current_procedure = name;
     for(auto s : body) s->validate();
     current_procedure = "";
}

void RootNode::codegen() {
    emit("JUMP", 0);
    // Gen Procs
    for(auto p : procedures) p->codegen();
    
    // Main
    long long main_start = code.size();
    code[0].arg = main_start;
    Symbol s; s.address = main_start;
    symbol_table["main_start"] = s;
    
    // emit("INC", 0); // test zaburzenia danych
    // emit("INC", 0);
    // emit("INC", 1);
    // emit("INC", 1);
    // emit("INC", 1);
    // emit("INC", 2);
    // emit("INC", 3);
    // emit("INC", 3);
    // emit("INC", 3);
    // emit("INC", 4);
    // emit("INC", 4);
    // emit("INC", 4);
    // emit("INC", 5);
    // emit("INC", 5);
    // emit("INC", 5);
    // emit("INC", 6);
    // emit("INC", 6);
    // emit("INC", 6);
    // emit("INC", 6);
    // emit("INC", 6);
    // emit("INC", 7);
    // emit("INC", 7);
    // emit("INC", 7);
    // emit("INC", 7);
    // emit("INC", 7);
    // emit("INC", 7);
    for(auto s : main_block) s->codegen();
    
    emit("HALT");
    
    // Kernel Gen
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

void RootNode::validate() {
    for(auto p : procedures) p->validate();
    for(auto s : main_block) s->validate();
}

// --- AST Printing ---

std::string indent_str(int n) {
    return std::string(n * 2, ' ');
}

void NumberNode::print(std::ostream& out, int indent) const {
    out << indent_str(indent) << "Number(" << value << ")" << std::endl;
}

void IdentifierNode::print(std::ostream& out, int indent) const {
    out << indent_str(indent) << "Identifier(" << name;
    if(is_array) {
        if(is_index_const) out << "[" << index_val << "]";
        else if (index_expr) {
            out << "[Expr]"; 
        } else {
             out << "[" << index_name << "]";
        }
    }
    out << ")" << std::endl;
}

void BinaryOpNode::print(std::ostream& out, int indent) const {
    out << indent_str(indent) << "BinaryOp(";
    switch(op) {
        case BinaryOp::PLUS: out << "+"; break;
        case BinaryOp::MINUS: out << "-"; break;
        case BinaryOp::MULT: out << "*"; break;
        case BinaryOp::DIV: out << "/"; break;
        case BinaryOp::MOD: out << "%"; break;
    }
    out << ")" << std::endl;
    left->print(out, indent + 1);
    right->print(out, indent + 1);
}

void ConditionNode::print(std::ostream& out, int indent) const {
    out << indent_str(indent) << "Condition(";
    switch(op) {
        case ConditionOp::EQ: out << "="; break;
        case ConditionOp::NEQ: out << "!="; break;
        case ConditionOp::GT: out << ">"; break;
        case ConditionOp::LT: out << "<"; break;
        case ConditionOp::GEQ: out << ">="; break;
        case ConditionOp::LEQ: out << "<="; break;
    }
    out << ")" << std::endl;
    left->print(out, indent + 1);
    right->print(out, indent + 1);
}

void AssignmentNode::print(std::ostream& out, int indent) const {
    out << indent_str(indent) << "Assignment" << std::endl;
    target->print(out, indent + 1);
    expr->print(out, indent + 1);
}

void IfNode::print(std::ostream& out, int indent) const {
    out << indent_str(indent) << "If" << std::endl;
    condition->print(out, indent + 1);
    out << indent_str(indent) << "Then" << std::endl;
    for(auto s : then_block) s->print(out, indent + 1);
    
    if(!else_block.empty()){
        out << indent_str(indent) << "Else" << std::endl;
        for(auto s : else_block) s->print(out, indent + 1);
    }
}

void WhileNode::print(std::ostream& out, int indent) const {
    out << indent_str(indent) << "While" << std::endl;
    condition->print(out, indent + 1);
    out << indent_str(indent) << "Do" << std::endl;
    for(auto s : body) s->print(out, indent + 1);
}

void RepeatNode::print(std::ostream& out, int indent) const {
    out << indent_str(indent) << "Repeat" << std::endl;
    for(auto s : body) s->print(out, indent + 1);
    out << indent_str(indent) << "Until" << std::endl;
    condition->print(out, indent + 1);
}

void ForNode::print(std::ostream& out, int indent) const {
    out << indent_str(indent) << "For(" << iterator << " " << (downto ? "DOWNTO" : "TO") << ")" << std::endl;
    out << indent_str(indent+1) << "From:" << std::endl;
    start_val->print(out, indent + 2);
    out << indent_str(indent+1) << "To:" << std::endl;
    end_val->print(out, indent + 2);
    out << indent_str(indent) << "Do" << std::endl;
    for(auto s : body) s->print(out, indent + 1);
}

void ProcCallNode::print(std::ostream& out, int indent) const {
    out << indent_str(indent) << "ProcCall(" << proc_name << ")" << std::endl;
    for(auto a : args) a->print(out, indent + 1);
}

void ReadNode::print(std::ostream& out, int indent) const {
    out << indent_str(indent) << "Read" << std::endl;
    target->print(out, indent + 1);
}

void WriteNode::print(std::ostream& out, int indent) const {
    out << indent_str(indent) << "Write" << std::endl;
    expr->print(out, indent + 1);
}

void ProcedureNode::print(std::ostream& out, int indent) const {
    out << indent_str(indent) << "Procedure " << name << std::endl;
    for(auto s : body) s->print(out, indent + 1);
}

void RootNode::print(std::ostream& out, int indent) const {
    out << indent_str(indent) << "Program" << std::endl;
    if(!procedures.empty()) {
        out << indent_str(indent + 1) << "Procedures:" << std::endl;
        for(auto p : procedures) p->print(out, indent + 2);
    }
    out << indent_str(indent + 1) << "Main:" << std::endl;
    for(auto s : main_block) s->print(out, indent + 2);
}
