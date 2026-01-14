# Remaining Implementation Steps

Based on the project prompt and current codebase status, here are the next logical steps to complete the compiler.

## 1. Constant Folding for Arithmetic (Priority High)
**Requirement**: "Dla mnożenia/dzielenia przez potęgi dwójki (stałe), generuj SHL/SHR zamiast CALL."
**Current Status**: `parser.y` currently generates `CALL` instructions for all MULT/DIV/MOD operations, regardless of operands.
**Action**:
- Modify the `expression` grammar rules in `parser.y`.
- Detect if the right operand is a constant number that is a power of 2 (2, 4, 8...).
- If yes:
    - `MULT`: Generate `SHL` instructions (cost: 1 cycle) instead of `CALL mul` (cost: hundreds of cycles).
    - `DIV`: Generate `SHR` instructions.
    - `MOD`: Logic is `num & (power-1)`. Can implement efficient bitwise masking if possible, or leave as CALL.

## 2. Enable Peephole Optimizer (Priority Medium)
**Requirement**: "Stwórz funkcję optimize_code()... Zaimplementuj reguły usuwania nadmiarowego kodu."
**Current Status**: `optimize_code()` exists in `codegen.cpp` but is commented out in `parser.y`.
**Action**:
- Uncomment `optimize_code()` in `parser.y`.
- Run tests to ensure the optimizer handles jump targets correctly (the current implementation has label remapping logic, but needs verification).
- Add a new rule: `LOAD x, STORE x` -> Remove both? (Or just the STORE if LOAD was needed for something else? Usually `STORE x; LOAD x` -> Remove `LOAD x` is safely implemented).

## 3. Parameter Modifiers Verification (Priority Medium)
**Requirement**: "Parametry przekazywane są przez referencję... T, I, O."
**Current Status**: `symtable.cpp` parses modifiers.
**Action**:
- Verify logic for `I` (Input - Read Only inside proc?).
- Verify logic for `O` (Output - Must be written to?).
- Ensure `T` (Array) parameters are correctly dereferenced using indirect addressing (`RLOAD`/`RSTORE`).
