#ifndef AST_HPP
#define AST_HPP

#include <vector>
#include <string>
#include <iostream>
#include "types.hpp"

// Forward declaration
struct Symbol;

enum class BinaryOp { PLUS, MINUS, MULT, DIV, MOD };
enum class ConditionOp { EQ, NEQ, LT, GT, LEQ, GEQ };

class ASTNode {
protected:
    int line_number;
public:
    ASTNode(int ln) : line_number(ln) {}
    virtual ~ASTNode() = default;
    
    // Abstract interface
    virtual void codegen() = 0;
    virtual void validate() {} 
    virtual void print(std::ostream& out, int indent = 0) const = 0;
};

class StatementNode : public ASTNode {
public:
    StatementNode(int ln) : ASTNode(ln) {}
};

class ExpressionNode : public ASTNode {
public:
    ExpressionNode(int ln) : ASTNode(ln) {}
    
    // Check if expression is a compile-time constant
    virtual bool is_constant() const { return false; }
    virtual long long evaluate() const { return 0; }
    
    // Helper to load result into a specific register (convention: usually reg[0] aka 'a')
    virtual void codegen_to_reg(int reg) = 0; 
    
    // Default codegen puts result in r[0]
    void codegen() override { codegen_to_reg(0); }
};

// --- Values ---

class ValueNode : public ExpressionNode {
public:
    ValueNode(int ln) : ExpressionNode(ln) {}
};

class NumberNode : public ValueNode {
public:
    long long value;
    NumberNode(long long val, int ln) : ValueNode(ln), value(val) {}
    bool is_constant() const override { return true; }
    long long evaluate() const override { return value; }
    void codegen_to_reg(int reg) override;
    void print(std::ostream& out, int indent = 0) const override;
};

class IdentifierNode : public ValueNode {
public:
    std::string name;
    std::string index_name; // For arr[var]
    ASTNode* index_expr; // For simpler handling in AST if index is complex? 
                         // Spec says arr[num] or arr[pidentifier]. 
                         // So index is either string (var) or long long (num).
    
    bool is_array;
    bool is_index_const; // true if arr[num]
    long long index_val; // For arr[10]

    IdentifierNode(std::string n, int ln) 
        : ValueNode(ln), name(n), index_expr(nullptr), is_array(false), is_index_const(false), index_val(0) {}
        
    void set_array_access(std::string idx, int ln) {
        is_array = true;
        is_index_const = false;
        index_name = idx;
    }
    
    void set_array_access(long long idx, int ln) {
        is_array = true;
        is_index_const = true;
        index_val = idx;
    }

    void codegen_to_reg(int reg) override;
    
    // Helper to get address into register (for WRITE / READ / ASSIGN)
    void codegen_address(int reg); 
    void print(std::ostream& out, int indent = 0) const override;
};

// --- Expressions ---

class BinaryOpNode : public ExpressionNode {
    ExpressionNode* left;
    ExpressionNode* right;
    BinaryOp op;
public:
    BinaryOpNode(ExpressionNode* l, BinaryOp o, ExpressionNode* r, int ln)
        : ExpressionNode(ln), left(l), right(r), op(o) {}
        
    void codegen_to_reg(int reg) override;
    void validate() override;
    void print(std::ostream& out, int indent = 0) const override;
    ~BinaryOpNode() { delete left; delete right; }
};

// --- Conditions ---

class ConditionNode : public ASTNode {
    ExpressionNode* left;
    ExpressionNode* right;
    ConditionOp op;
public:
    ConditionNode(ExpressionNode* l, ConditionOp o, ExpressionNode* r, int ln)
        : ASTNode(ln), left(l), right(r), op(o) {}
        
    // Generates code that Jumps to `target_label` if condition is FALSE
    // OR Jumps if TRUE?
    // Usually: code code... if FALSE(cond) jump target.
    void codegen_jump_false(long long target_instruction_index);
    void codegen() override {} // Standard codegen might not be used directly
    void validate() override;
    void print(std::ostream& out, int indent = 0) const override;
    ~ConditionNode() { delete left; delete right; }
};

// --- Statements ---

class AssignmentNode : public StatementNode {
    IdentifierNode* target;
    ExpressionNode* expr;
public:
    AssignmentNode(IdentifierNode* t, ExpressionNode* e, int ln)
        : StatementNode(ln), target(t), expr(e) {}
    void codegen() override;
    void validate() override;
    void print(std::ostream& out, int indent = 0) const override;
    ~AssignmentNode() { delete target; delete expr; }
};

class IfNode : public StatementNode {
    ConditionNode* condition;
    std::vector<StatementNode*> then_block;
    std::vector<StatementNode*> else_block;
public:
    IfNode(ConditionNode* cond, std::vector<StatementNode*>& tb, std::vector<StatementNode*>& eb, int ln)
        : StatementNode(ln), condition(cond), then_block(tb), else_block(eb) {}
    void codegen() override;
    void validate() override;
    void print(std::ostream& out, int indent = 0) const override;
    ~IfNode() { delete condition; for(auto s: then_block) delete s; for(auto s: else_block) delete s; }
};

class WhileNode : public StatementNode {
    ConditionNode* condition;
    std::vector<StatementNode*> body;
public:
    WhileNode(ConditionNode* cond, std::vector<StatementNode*>& b, int ln)
        : StatementNode(ln), condition(cond), body(b) {}
    void codegen() override;
    void validate() override;
    void print(std::ostream& out, int indent = 0) const override;
    ~WhileNode() { delete condition; for(auto s: body) delete s; }
};

class RepeatNode : public StatementNode {
    ConditionNode* condition;
    std::vector<StatementNode*> body;
public:
    RepeatNode(ConditionNode* cond, std::vector<StatementNode*>& b, int ln)
        : StatementNode(ln), condition(cond), body(b) {}
    void codegen() override;
    void validate() override;
    void print(std::ostream& out, int indent = 0) const override;
    ~RepeatNode() { delete condition; for(auto s: body) delete s; }
};

class ForNode : public StatementNode {
    std::string iterator;
    ExpressionNode* start_val;
    ExpressionNode* end_val;
    bool downto;
    std::vector<StatementNode*> body;
public:
    ForNode(std::string iter, ExpressionNode* start, ExpressionNode* end, bool down, std::vector<StatementNode*>& b, int ln)
        : StatementNode(ln), iterator(iter), start_val(start), end_val(end), downto(down), body(b) {}
    void codegen() override;
    void validate() override;
    void print(std::ostream& out, int indent = 0) const override;
    ~ForNode() { delete start_val; delete end_val; for(auto s: body) delete s; }
};

class ProcCallNode : public StatementNode {
    std::string proc_name;
    std::vector<ValueNode*> args;
public:
    ProcCallNode(std::string name, std::vector<ValueNode*>& a, int ln)
        : StatementNode(ln), proc_name(name), args(a) {}
    void codegen() override;
    void validate() override;
    void print(std::ostream& out, int indent = 0) const override;
    ~ProcCallNode() { for(auto a: args) delete a; }
};

class ReadNode : public StatementNode {
    IdentifierNode* target;
public:
    ReadNode(IdentifierNode* t, int ln) : StatementNode(ln), target(t) {}
    void codegen() override;
    void validate() override;
    void print(std::ostream& out, int indent = 0) const override;
    ~ReadNode() { delete target; }
};

class WriteNode : public StatementNode {
    ExpressionNode* expr; // Can write value
public:
    WriteNode(ExpressionNode* e, int ln) : StatementNode(ln), expr(e) {}
    void codegen() override;
    void validate() override;
    void print(std::ostream& out, int indent = 0) const override;
    ~WriteNode() { delete expr; }
};

// --- Root ---

class ProcedureNode : public ASTNode {
    std::string name;
    // Args declaration info is stored in Symbol Table, not necessarily here, 
    // but AST should own the body.
    std::vector<StatementNode*> body; 
public:
    ProcedureNode(std::string n, std::vector<StatementNode*> b, int ln)
        : ASTNode(ln), name(n), body(std::move(b)) {}
    void codegen() override;
    void validate() override;
    void print(std::ostream& out, int indent = 0) const override;
    ~ProcedureNode() { for(auto s: body) delete s; }
};

class RootNode : public ASTNode {
    std::vector<ProcedureNode*> procedures;
    std::vector<StatementNode*> main_block;
public:
    RootNode(std::vector<ProcedureNode*>& procs, std::vector<StatementNode*>& main, int ln)
        : ASTNode(ln), procedures(procs), main_block(main) {}
    void codegen() override;
    void validate() override;
    void print(std::ostream& out, int indent = 0) const override;
    ~RootNode() { for(auto p: procedures) delete p; for(auto s: main_block) delete s; }
};

#endif
