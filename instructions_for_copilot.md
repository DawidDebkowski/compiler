# Instructions for Smart Allocator Implementation

## Goal
Implement a "Smart Hole-Filling" Memory Allocator in `symtable.cpp` to optimize array addressing. The goal is to maximize the number of arrays that satisfy `base_address >= start_index`, allowing for `Fast CodeGen` (static offset addition) instead of `Slow CodeGen` (dynamic header lookup).

## constraints
- **Fast Mode**: Used when `base >= start`. We can compile `addr = base + (i - start)` as `addr = i + (base - start)`. The term `(base - start)` is a compile-time positive constant (no underflow).
- **Safe Mode (Header)**: Used when `base < start` (e.g., deeply allocated `a(10:20)` or huge `start`). Stores `start` at `M[base]` and computes dynamic offset.

## Technical Changes

### 1. `src/types.hpp`
- Update `Symbol` struct:
    - Add `bool use_fast_access` (aka optimized layout).
    - Add `long long constant_offset` (precomputed `address - start`).

### 2. `src/symtable.hpp` & `src/symtable.cpp`
- **Data Structures**:
    - Add `struct FreeBlock { long long start; long long size; };`
    - Add `std::vector<FreeBlock> holes;`
- **Logic**:
    - Implement `void add_hole(long long start, long long size)`: Merges adjacent holes.
    - Implement `find_hole(long long size, long long min_start)`: Finds first fit hole.
- **Refactor `add_symbol`**:
    - **Step A (Scalars)**: Try to find *any* hole of size 1. If not found, append to `memory_offset`.
    - **Step B (Arrays)**:
        - Calculate `size`.
        - **Strategy 1 (Natural Fit)**: If `memory_offset >= start`, allocating at `memory_offset` satisfies `base >= start`. Proceed.
        - **Strategy 2 (Alignment)**: If `memory_offset < start`:
            - Check `gap = start - memory_offset`.
            - If `gap` is "small" (e.g., < 1024 words):
                - Create a hole at `[memory_offset, start - 1]`.
                - Set `base = start`. (Now `base == start`, optimized).
            - Else (Gap too big):
                - Must allocate at `memory_offset`.
                - `base < start` -> **FALLBACK to Header Mode**.
    - **Hole Filling**:
        - Before modifying `memory_offset`, always check `holes` behavior.
        - If we create a hole (Strategy 2), add it to `holes`.
        - When allocating scalars, search `holes` first.

### 3. `src/ast.cpp`
- **Update `IdentifierNode::codegen_address`**:
    - usage:
        ```cpp
        if (s->use_fast_access) {
             // Optimized!
             long long k = s->constant_offset;
             if (index_is_const) {
                 val = index + k;
             } else {
                 LOAD index
                 ADD k
             }
        } else {
             // Existing Header Logic
        }
        ```

### 4. `src/parser.y`
- Initialize allocator state properly.

## Implementation Steps
1.  Modify `types.hpp`.
2.  Refactor `symtable.cpp` completely to support holes and alignment logic.
3.  Update `ast.cpp` to use the new flags.
4.  Verify with `make test-all`.
