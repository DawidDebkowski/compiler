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
- `optimize_code()`: A peephole optimizer that iteratively removes redundant `STORE`/`LOAD` pairs and jumps to next lines.

### 5. Math Kernel (`math_kernel.cpp`)
The target VM only supports `ADD`, `SUB`, `SHL`, `SHR`. Complex arithmetic is implemented in software via subroutines injected at the start of the code. **Optimization**: Logic is included to only inject the specific procedures (`MUL`, `DIV`, or `MOD`) if they are actually used in the source code.

#### Algorithms Used:
- **Optimization (Constant Folding)**: 
  - Powers of 2 (e.g., `x * 4`, `y / 2`) are detected during parsing.
  - Instead of generating a `CALL` to the math kernel, efficient `SHL`/`SHR` sequences are emitted inline.
  - This drastically reduces instruction count for simple arithmetic (e.g., `program0`).

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
The compiler maps the VM's 8 registers (`a`-`h`) to specific roles to manage state, expression evaluation, and kernel calls.

| Register | Name | Role |
|:---:|:---:|:---|
| **0** | `a` | **Accumulator**. Used for majority of arithmetic, logic, and load/store operations. |
| **1** | `b` | **Expression Operand**. Stores the LHS of a binary expression while RHS is evaluated. |
| **2** | `c` | **Index/Offset**. Used to calculate array offsets and subtraction results. |
| **3** | `d` | **Temp / Math RA**. Used as temporary storage in Conditions and as **Return Address** for `MUL`/`DIV` kernel calls. |
| **4** | `e` | **Kernel Scratch**. Used by Math Kernel (e.g., Result Accumulator in MUL). |
| **5** | `f` | **Kernel Scratch**. Used by Math Kernel (e.g., Parity/Shift tracker). |
| **6** | `g` | **Kernel Scratch**. Used by Math Kernel (e.g., Quotient Accumulator in DIV). |
| **7** | `h` | **Address Pointer**. Stores the effective address for Array Access (`arr[i]`). Also saves RA during `MOD` kernel wrapper. |

### Math Kernel Interface
The arithmetic subroutines (`MUL`, `DIV`, `MOD`) use a specific calling convention:

*   **MUL (`generate_mul`)**: 
    *   **Input**: `b` (Multiplicand), `c` (Multiplier).
    *   **Output**: `b` (Result).
    *   **Clobbers**: `a`, `c`, `d`, `e`, `f`, `g`.
*   **DIV (`generate_div`)**: 
    *   **Input**: `b` (Dividend), `c` (Divisor).
    *   **Output**: `b` (Quotient), `c` (Remainder).
    *   **Clobbers**: `a`, `d`, `e`, `f`, `g`.
*   **MOD (`generate_mod`)**: 
    *   **Input**: `b` (Dividend), `c` (Divisor).
    *   **Output**: `b` (Remainder).
    *   **Note**: Wraps `DIV` but swaps the remainder (`c`) into the result register (`b`). Clobbers `h` (saves RA).

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
