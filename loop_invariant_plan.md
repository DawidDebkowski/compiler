# Loop Invariant Code Motion Plan

## Objective
Move loop-invariant computations outside the loop to reduce redundant calculations. For example, in `FOR i FROM 1 TO n DO tab[i] := x + y; ENDFOR`, compute `x + y` once before the loop if it doesn't depend on `i` or loop-modified variables.

## Feasibility with Current Structure
Yes, possible. The AST has ForNode with body as vector<StatementNode*>. We can extend ExpressionNode with dependency tracking and analyze the loop body for invariants.

## Prerequisites
- Extend ExpressionNode with `std::set<std::string> get_dependencies()` to collect variable names read by the expression.
- In ForNode::codegen, collect variables modified in the loop body (e.g., via a new method `collect_modified_vars(std::set<std::string>&)` on StatementNode).
- Identify invariant expressions: those where dependencies don't intersect with loop-modified vars or iterator.

## Steps

### 1. Extend ExpressionNode (`src/ast.hpp`, `src/ast.cpp`)
- Add virtual method: `virtual void get_dependencies(std::set<std::string>& deps) const = 0;`
- Implement in subclasses:
  - NumberNode: Empty set.
  - IdentifierNode: Add `name` (and `index_name` if array).
  - BinaryOpNode: Union of left and right dependencies.

### 2. Extend StatementNode for Modified Variables
- Add virtual method: `virtual void collect_modified_vars(std::set<std::string>& mods) {}`
- Implement:
  - AssignmentNode: Add target's name.
  - ReadNode: Add target's name.
  - IfNode, WhileNode, etc.: Recurse on bodies.
  - ForNode: Add iterator, plus recurse on body.

### 3. Modify ForNode::codegen (`src/ast.cpp`)
- Before generating loop code:
  - Collect modified vars in body: `std::set<std::string> modified; collect_modified_vars(modified); modified.insert(iterator);`
  - Traverse body to find invariant expressions in assignments: e.g., in AssignmentNode, check if expr's dependencies are disjoint from modified.
  - For each invariant, create a temporary variable (or register), compute it before loop, and replace the expression with the temp in the AST (or during codegen).
- Generate loop with replaced expressions.

### 4. Temporary Storage
- Use a new register or memory slot for hoisted values.
- Since VM has registers, allocate a temp register for each hoisted value.

## Implementation Details
- Hoisting: Modify AST in-place or use a codegen-time substitution.
- Ensure no side effects: Only hoist pure expressions (no procedure calls, etc.).
- Size/Speed: Reduces instructions inside loop, improves performance for large n.

## Challenges
- AST modification: Safest to do during codegen without changing AST.
- Nested loops: Apply recursively, but check invariants per loop level.</content>
<parameter name="filePath">/home/dawid/Projects/pwr/sem5/jftt/gembiler/loop_invariant_plan.md