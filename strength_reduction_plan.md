# Strength Reduction for Array Access Plan

## Objective
Optimize array accesses in loops by maintaining a pointer that increments instead of recalculating base + index each time. Critical for patterns like `FOR i FROM 1 TO n DO sito[i] := ...; ENDFOR`.

## Feasibility with Current Structure
Yes, possible. IdentifierNode handles array accesses. In ForNode::codegen, detect array ops where index is the iterator, and replace with pointer arithmetic.

## Prerequisites
- VM supports register-based addressing (RLOAD, RSTORE).
- Extend codegen to track loop iterator and detect array accesses.

## Steps

### 1. Detect Array Access Patterns (`src/ast.cpp`)
- In ForNode::codegen, scan body for IdentifierNode with `is_array` and `index_name == iterator`.
- Collect such accesses: list of IdentifierNode* that use iterator as index.

### 2. Introduce Shadow Pointer
- For each unique array base (e.g., `sito`), allocate a register for the pointer.
- Before loop: Compute initial address `&sito + start_val`, store in pointer register.
- Inside loop: Use RLOAD/RSTORE with pointer register instead of LOAD base, LOAD index, ADD.
- After iterator update: INC pointer register.

### 3. Modify IdentifierNode::codegen_address (`src/ast.cpp`)
- Add a flag or parameter to use shadow pointer if available.
- In ForNode, pass a map<string, int> array_to_pointer_reg to body codegen.
- In IdentifierNode::codegen_address, if array and index is iterator and pointer_reg available, emit RLOAD pointer_reg instead of full address calc.

### 4. Handle Multiple Arrays
- If multiple arrays indexed by iterator, allocate separate pointers.
- Ensure no conflicts with other register usage (integrate with existing register allocation).

## Implementation Details
- Register Allocation: Use temp registers (e.g., r[10] onwards) for pointers.
- Direction: For DOWNTO, use DEC instead of INC.
- Non-iterator Indices: If index is not iterator, fall back to normal.
- Nested Loops: Only apply if iterator matches, and perhaps disable for nested to avoid complexity.

## Benefits
- Replaces multiple instructions (LOAD base, LOAD index, ADD) with INC per iteration.
- Significant speedup for array-heavy loops like Sieve.

## Challenges
- AST Modification: Pass context through codegen calls.
- Register Pressure: Ensure enough registers; may need to spill if many arrays.</content>
<parameter name="filePath">/home/dawid/Projects/pwr/sem5/jftt/gembiler/strength_reduction_plan.md