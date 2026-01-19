#include "math_kernel.hpp"
#include "codegen.hpp"
#include <vector>
#include <string>

using namespace std;

// Spill Memory Locations - not working now
const long long SPILL_RE = 2;
const long long SPILL_RF = 3;
const long long SPILL_RG = 4;

// Input: rb (left-side), rc (right-side)
// Output: rb (Result)
// Clobbers: ra, rb, rc, rd, re, rf,
// Preserves: re, rf, rg (via Spill), rh
void generate_mul() {
    addr_mul = code.size();
    
    // 1. Save Return Address
    emit("SWP", 3); // rd = RA
    
    // 3. Initialize Accumulator
    emit("RST", 4); // re = 0 (Result)
    
    long long loop_start = code.size();
    
    // 4. Check Termination (rc == 0)
    emit("RST", 0); emit("ADD", 2);
    emit("JZERO", 0); // Placeholder
    long long jump_end = code.size() - 1;
    
    // 5. Parity Check & Shift rc
    // rf = copy(rc)
    emit("RST", 0); emit("ADD", 2); emit("SWP", 5); // rf = rc
    
    // rc = rc >> 1
    emit("SHR", 2);
    
    // ra = (rc << 1)
    emit("RST", 0); emit("ADD", 2); emit("SHL", 0);
    
    // Parity = rf - ra
    emit("SWP", 5); // ra = rf, rf = (rc<<1)
    emit("SUB", 5); // ra = Old_rc - (Old_rc_even_part)
    
    emit("JZERO", 0); 
    long long jump_even = code.size() - 1;
    
    // 6. If Odd: re += rb
    emit("RST", 0); emit("ADD", 4); emit("ADD", 1); emit("SWP", 4);
    
    code[jump_even].arg = code.size();
    
    // 7. rb = rb << 1
    emit("SHL", 1);
    
    // 8. Loop
    emit("JUMP", loop_start);
    
    // 9. Cleanup
    code[jump_end].arg = code.size();
    
    // Move result re -> rb
    emit("RST", 0); emit("ADD", 4); emit("SWP", 1);
    
    // Restore RA
    emit("SWP", 3);
    
    emit("RTRN");
}

// poprzec na liste laboratoryjna
// w program 0 jest mnozenie
void generate_div() {
    addr_div = code.size();
    // Input: rb (Dividend), rc (Divisor)
    // Output: rb (Quotient), rc (Remainder)
    
    emit("SWP", 3); // Save RA
    
    // Check Zero Divisor
    emit("RST", 0); emit("ADD", 2);
    emit("JZERO", 0); 
    long long jump_zero = code.size() - 1;
    
    // Init registers
    emit("RST", 6); // rg = 0 (Quotient)
    emit("RST", 5); emit("INC", 5); // rf = 1 (Mask)
    emit("RST", 0); emit("ADD", 2); emit("SWP", 4); // re=rc
    
    // --- LOOP UP ---
    long long loop1 = code.size();
    
    // Check re > rb (re - rb > 0)
    emit("RST", 0); emit("ADD", 4); emit("SUB", 1);
    emit("JPOS", 0);
    long long jump_out1 = code.size() - 1;
    
    emit("SHL", 4);
    emit("SHL", 5);
    emit("JUMP", loop1);
    
    code[jump_out1].arg = code.size();
    
    // Undo last shift
    emit("SHR", 4);
    emit("SHR", 5);
    
    // --- LOOP DOWN ---
    long long loop2 = code.size();
    
    // Check rf == 0
    emit("RST", 0); emit("ADD", 5);
    emit("JZERO", 0);
    long long jump_out2 = code.size() - 1;
    
    // Check re > rb? (re - rb > 0)
    emit("RST", 0); emit("ADD", 4); emit("SUB", 1);
    emit("JPOS", 0);
    long long jump_skip = code.size() - 1;
    
    // rb -= re
    emit("SWP", 1); emit("SUB", 4); emit("SWP", 1);
    
    // rg += rf
    emit("SWP", 6); emit("ADD", 5); emit("SWP", 6);
    
    code[jump_skip].arg = code.size();
    
    emit("SHR", 4);
    emit("SHR", 5);
    emit("JUMP", loop2);
    
    code[jump_out2].arg = code.size();
    
    // Finalize
    // rc = Remainder (current rb)
    // rb = Quotient (current rg)
    
    // rc = rb
    emit("RST", 0); emit("ADD", 1); emit("SWP", 2);
    // rb = rg
    emit("RST", 0); emit("ADD", 6); emit("SWP", 1);
    
    // Restore RA
    emit("SWP", 3);
    emit("RTRN");
    
    // Zero Handler
    code[jump_zero].arg = code.size();
    emit("RST", 1); emit("RST", 2); // Result 0, Rem 0
    emit("SWP", 3);
    emit("RTRN");
}

void generate_mod() {
    addr_mod = code.size();
    emit("SWP", 7); // Save RA to rh (7) as div uses rd (3)
    emit("CALL", addr_div);
    emit("RST", 0);
    emit("ADD", 2); 
    emit("SWP", 1); 
    emit("SWP", 7);
    emit("RTRN");
}
