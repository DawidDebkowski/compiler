# Simple Register Allocation Strategy (AST-Based)

## 1. The Core Concept: "Write-Back Cache"
Since we cannot do global analysis (no CFG/TAC), we treat registers `r1` through `r7` as a **write-back cache** for your variables. 

- **Basic Block Scope:** The cache is valid *only* within a linear sequence of instructions.
- **Barriers:** Any Jump, Label, Procedure Call, or complex Math Kernel operation invalidates the cache (requires flushing).

## 2. Register Tracking
We need a global state structure in `codegen.cpp` (or `ast.cpp`) to track what is currently in the registers.

```cpp
struct RegisterState {
    string var_name; // Name of the variable currently held
    bool is_valid;   // Is this register holding meaningful data?
    bool is_dirty;   // True if register content > memory content (needs STORE)
    long long last_used; // For LRU eviction
};

RegisterState regs[8]; // r0..r7. Note: r0 is Accumulator, usually very volatile.
```

## 3. The Rules

### A. Allocation Strategy (r1 - r7)
We prioritize using `r1` through `r7` for variables. `r0` is the "Busy" register used for immediate math inputs/outputs.

### B. "Lazy Load"
When the AST needs to read variable `X`:
1.  **Check:** Is `X` already in a register (e.g., `r3`)?
2.  **Hit:** Use `r3` directly. (Emit nothing, or `SWP` if needed).
3.  **Miss:** 
    *   Find a free register (or evict one).
    *   Emit `LOAD` and move to that register.
    *   Update State.

### C. "Lazy Store"
When `AssignmentNode` executes `X := Expr`:
1.  Evaluate `Expr` (Result is in `r0`).
2.  **Do NOT emit `STORE X`**.
3.  Instead: 
    *   Allocate a register for `X` (e.g., `r2`).
    *   Move `r0` -> `r2`.
    *   Mark `r2` as `{ var: "X", dirty: true }`.

### D. Spilling (Flushing)
We must loop through all registers and emit `STORE` for any **dirty** register when we hit a barrier.

**The "Barrier" List (When to Spill All):**
1.  **Labels:** Before generating a jump target label (we don't know who jumps here or what their register state is).
2.  **Jumps:** `JUMP`, `JZERO`, `JPOS` (we need memory to be consistent for the destination).
3.  **Procedure Calls:** `CALL` or inline expansion (Callee behavior is unknown or complex).
4.  **Math Kernel:** Before `MULT`, `DIV`, `MOD` (because they clobber `r0-r6`).
    *   *Optimization:* You only need to spill registers that the kernel *actually clobbers*.

## 4. Implementation Steps in `ast.cpp`

### Step 1: `flush_registers()`
Add a helper function.
```cpp
void flush_registers() {
    for(int i=1; i<8; i++) {
        if (regs[i].is_valid && regs[i].is_dirty) {
             Symbol* s = get_variable(regs[i].var_name);
             emit("STORE", s->address); // (requires moving reg to r0 first via SWP)
             regs[i].is_dirty = false;
        }
        regs[i].is_valid = false; // Invalidate after flush for safety at barriers
    }
}
```

### Step 2: Update `IdentifierNode::codegen_to_reg`
Modify logic to check the cache:
```cpp
void IdentifierNode::codegen_to_reg(int target_reg) {
    int cached_reg = find_in_registers(name);
    
    if (cached_reg != -1) {
        // Optimization: It's already in a register!
        if (cached_reg != target_reg) {
            // Move cached_reg -> target_reg
            // Note: VM might not have direct MOV rx, ry.
            // Usually need SWP paths via r0. 
            // If target is r0: SWP cached_reg (puts val in r0, junk in cached)
            // BE CAREFUL: SWP destroys the value in the cached register (swaps it out).
            // Unless we swap back? Or just mark cached as invalid?
        }
    } else {
        // Standard Load...
    }
}
```
**Critical VM Constraint Check:**  
VM dosnt have `COPY rx ry`.
If you only have `SWP rx`, then "reading" a variable from `r2` into `r0` (Accumulator) is destructive to `r2` (it gets `r0`'s junk). 
*   **Resolution:** If `SWP` is the only way to move data, you strictly cannot "copy" registers easily without a temp. 
*   *Workaround:* If you `SWP 2` (to get value into `r0`), you must `SWP 2` *back* immediately if you want to preserve the cache, OR mark `r2` as invalid.

### Step 3: Update `AssignmentNode`
Remove the explicit `STORE`.
```cpp
void AssignmentNode::codegen() {
    expr->codegen_to_reg(0); // Result in r0
    
    // Allocate a register (e.g., r3)
    // SWP 3 (Now val is in r3, r0 has garbage)
    // Mark r3 as { "var_name", dirty: true }
}
```

### Step 4: Barriers in `While`, `If`, `For`
Insert `flush_registers()` calls:
```cpp
void IfNode::codegen() {
    flush_registers(); // Barrier before condition/branching
    condition->codegen();
    // ...
    flush_registers(); // Barrier before label
    // ...
}
```

## 5. Challenges & Critiques

1.  **The `SWP` Problem:** 
    Your architecture (Accumulator + Swap) is hostile to register allocation. 
    *   To use `r2` in an `ADD`, you usually do `ADD 2`. This is fine (non-destructive).
    *   But to *move* `r2` to `r3`, you need `r0`.
    *   If you only have `SWP`, maintaining the cache is hard because accessing a value moves it.
    *   **Verdict:** Only implement caching for instructions like `ADD rX`, `SUB rX` which read without moving. For generic "Move", just accept the cost.

2.  **Math Kernel Complexity:**
    Your `div` uses `r0`..`r6`. It practically wipes the entire register file.
    **Constraint:** Any division implies a full memory flush.

3.  **Arrays:**
    Do not cache array elements (`arr[i]`). Tracking aliasing (is `arr[i]` the same as `arr[j]`?) is impossible here. Always invalidates cache or bypasses it. `tab[i] := 5` should probably flush the cache if we want to be safe, or at least treat `tab[i]` as distinct from all scalars.

## 6. Recommended "Byte-Sized" Starting Point
Don't implement the full system yet. Start with this:
**"The Accumulator Linger"**
Just track `r0`.
If `AssignmentNode` leaves value `X` in `r0`:
1.  Record `r0` has `X`.
2.  Next instruction: If it needs `LOAD X`, and `r0` still has `X`, skip the load.
This handles sequences like:
```
a := b + c;
d := a + 1; // 'a' is likely still in r0 from previous line
```
