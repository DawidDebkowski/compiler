# Compilation Project Status & Next Steps

## Completed Tasks Analysis (Based on Prompt)

### Krok 1: Infrastruktura (Bufor i Tablica Symboli) - **DONE**
- Implemented `vector<Instruction> code` in `codegen.cpp`.
- Implemented `Symbol` struct and `symbol_table` map in `symtable.cpp`.
- Array support (base + range) is implemented.
- Procedure variable prefixing (`procName_varName`) is implemented for static scoping.

### Krok 2: "Kernel Matematyczny" - **DONE**
- Implemented `math_kernel.cpp` with `generate_mul`, `generate_div`, `generate_mod`.
- Uses Russian Peasant algorithm (shifts/adds).
- Optimized to only inject procedures if they are actually called (`calls_mul.empty()` check).

### Krok 3: Generowanie Wyrażeń z Optymalizacją Rejestrową - **PARTIAL / DONE**
- Basic expression generation works (`SWP`, `ADD`, `SUB`).
- **Optimization**: Constant folding for powers of 2 (shifts instead of MUL/DIV calls) is implemented and verified (`program0` results).
- Register usage: Using `SWP` to save left operands is standard practice here.

### Krok 4: Control Flow i Backpatching - **DONE**
- `if_stack` and `loop_stack` used for backpatching.
- Conditions implemented via subtraction and `JPOS`/`JZERO`.

### Krok 5: Pętle FOR (Licznik w dół) - **DONE**
- FOR loops calculate iterations and use `DEC` + `JZERO` (implied logic check, though exact "down counter" implementation details in `parser.y` closely follow this).

### Krok 6: Procedury i Parametry - **DONE**
- Reference passing implemented via pointers.
- `RLOAD`/`RSTORE` (Indirect addressing) used for parameters.
- Array parameters passed by base address.

### Krok 7: Peephole Optimizer - **DONE**
- `optimize_code()` implemented in `codegen.cpp`.
- iteratively removes `STORE x; LOAD x` and `JUMP next`.

---

## Next Steps (Project Roadmap)

### 1. Advanced Logic Validation (Complex Tests)
While basic tests pass, edge cases in complex logic (recursion simulation via loops, nested arrays with parameters) need more verification.
- **Action**: Create a "torture test" that combines nested FOR loops, array parameters, and math operations to ensure register clobbering doesn't happen (specifically `r1` vs `r2` usage seen in `example6`).

### 2. Further Peephole Patterns
The current optimizer is basic.
- **Idea**: `ADD 0`, `SUB 0` removal.
- **Idea**: `JZERO target` immediately followed by `JUMP target` (logic redundancy).
- **Idea**: `SHL; SHR` pairs that might cancel out if no overflow (risky but possible).

### 3. Register Allocation Strategy (Long Term)
The prompt mentioned "Minimize instructions and cycles".
- Currently, every variable is LOAD/STORED.
- **Goal**: Analyze "hot" variables (loop counters) and try to keep them in `r[3]`-`r[6]` (if unused by Kernel) for the duration of the loop. This requires Liveness Analysis, which is non-trivial in a One-Pass compiler, but rudimentary heuristic (e.g., "if variable is loop iterator I, keep in register") works.

### 4. Math Kernel Selection
- Current logic only checks if *any* MUL/DIV is used.
- **Optimization**: If a program only uses `a * 10`, we currently generate the full MUL kernel. We could inline small constant multiplications (shifts + adds) to avoid the 50-instruction overhead entirely.

### 5. Final Code Cleanup
- Remove debug prints and unused global flags.
- Ensure all error messages give line numbers (mostly done).
- Verify `Makefile` clean/distclean targets.

## Immediate Action Items
1.  Verify `exampleA` (Matrix Multiplication?) logic manually since no expected output exists.
2.  Review `parser.y` for any left-over hardcoded register assumptions (like the `r1` fix recently made).
