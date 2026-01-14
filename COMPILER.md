# Compiler Documentation

This document links the architectural decisions, internal modules, and algorithms used in the compiler project.

## Project Structure

The project is broken down into modular components to handle specific stages of compilation:

```
src/
├── parser.y        # Grammar definitions and main compiler logic (Bison)
├── lexer.l         # Tokenizer definitions (Flex)
├── codegen.cpp     # Assembly code buffer and emission helpers
├── symtable.cpp    # Symbol table and memory management
├── math_kernel.cpp # Software implementations of arithmetic (MUL, DIV, MOD)
└── types.hpp       # Shared structs (Symbol, Instruction)
```

## Compilation Stages

### 1. Lexical Analysis (`lexer.l`)
The input source code is tokenized into keywords (`PROGRAM`, `IS`, etc.), operators (`:=`, `+`, etc.), identifiers, and numbers. Comments (`# ...`) are stripped.

### 2. Syntax Analysis (`parser.y`)
The core logic resides here. It defines the grammar rules. Instead of building an AST, the compiler uses **syntax-directed translation**:
- **Declarations**: Calls `symtable` to allocate memory.
- **Commands**: Immediately calls `codegen` to emit corresponding Assembly instructions.
- **Control Flow**: Uses `if_stack` and `loop_stack` to handle backpatching for jumps (e.g., jumping over `ELSE` blocks or looping back for `REPEAT`).

### 3. Symbol Management (`symtable.cpp`)
- **Scoping**: Variables are prefixed with the procedure name (e.g., `proc_x`) to ensure local scope logic.
- **Memory**: Variables are allocated sequentially starting continuously from address 1000.
- **Validation**: Checks for redeclarations, uninitialized variable usage, and valid array parameters.

### 4. Code Generation (`codegen.cpp`)
- buffers instructions in a `vector<Instruction>`.
- `emit(opcode, arg)`: Adds an instruction to the buffer.
- `optimize_code()`: (Optional) A peephole optimizer meant to remove redundant `STORE`/`LOAD` pairs and jumps to next lines.

### 5. Math Kernel (`math_kernel.cpp`)
The target VM only supports `ADD`, `SUB`, `SHL`, `SHR`. Complex arithmetic is implemented in software via subroutines injected at the start of the code.

#### Algorithms Used:
- **Multiplication (`generate_mul`)**: 
  - **Algorithm**: standard Russian Peasant Multiplication (Double and Halve).
  - **Logic**: 
    1. If factor B is odd, add A to result.
    2. Double A, Halve B.
    3. Repeat until B is 0.

- **Division (`generate_div`)**: 
  - **Algorithm**: Binary Long Division (Shift and Subtract).
  - **Logic**:
    1. Align divisor with dividend by shifting left.
    2. If aligned divisor <= dividend, subtract and add corresponding bit to quotient.
    3. Shift right and repeat.

- **Modulo (`generate_mod`)**:
  - Wrapper that calls division and returns the remainder explicitly.

## Register Usage in VM
The compiler uses the VM's registers (`r[0]`-`r[7]`) as temporary scratchpads during evaluation.

- `r[0]` (`ra`): Main Accumulator / arithmetic workspace.
- `r[1]` (`rb`): Secondary operand / Math Kernel input.
- `r[2]` (`rc`): Math Kernel input.
- `r[3]` (`rd`): Return Address storage for function calls.
- `r[4]` (`re`) - `r[6]` (`rg`): Internal calculation registers for Math Kernel.
- `r[7]` (`rh`): Auxiliary storage (e.g., saving `rd` during nested calls).

## Build & Test

### Building
```bash
make
```
This produces the `compiler` executable.

### Testing
Tests are organized by category.
```bash
make test CATEGORY=basic    # Run basic validation
make test CATEGORY=labor4   # Run specific lab tests
```

Output is verified against `.ans` files (if present) in `tests/ans/`.
