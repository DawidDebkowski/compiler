#include "math_kernel.hpp"
#include "codegen.hpp"
#include <vector>
#include <string>

using namespace std;

const long long SPILL_RE = 2;
const long long SPILL_RF = 3;
const long long SPILL_RG = 4;

// Registers used:
//  r0 (ra): Scratch / Accumulator
//  r1 (rb): Multiplicand (Left)
//  r2 (rc): Multiplier (Right)
//  r3 (rd): Result
//  r4 (re): Return Address Backup
void generate_mul() {
    // Pseudocode:
    //  Result = 0
    //  While Multiplier > 0:
    //      If Multiplier is Odd:
    //          Result += Multiplicand
    //      Multiplicand << 1
    //      Multiplier >> 1
    addr_mul = code.size();
    
    // 1. Save Return Address to r4
    emit("SWP", 4); 

    // 2. Initialize Result (r3) to 0
    emit("RST", 3); 

    emit("RST", 0, "IF C > B <=> B - C == 0");
    emit("ADD", 2);
    emit("SUB", 1);
    emit("JZERO", code.size()+4, "SWAP THEM");
    emit("SWP", 2);
    emit("SWP", 1);
    emit("SWP", 2);


    long long loop_start = code.size();

    // 3. Load Multiplier (r2) to Check Zero/Parity
    emit("RST", 0); 
    emit("ADD", 2); 
    
    // 4. Check Termination: If Multiplier == 0, End
    emit("JZERO", 0);
    long long jump_end = code.size() - 1;

    // 5. Parity Check (Is r2 Odd?)
    //    r2 = floor(r2 / 2) * 2;
    //    Bit = Old_r2 - New_r2;
    
    emit("SHR", 2); // r2 = floor(r2 / 2)
    emit("SHL", 2); // r2 = floor(r2 / 2) * 2 (LSB cleared)
    
    // r0 (Old r2) - r2 (Even part) = LSB
    emit("SUB", 2); 
    
    emit("JZERO", 0); // If LSB is 0, Skip Addition
    long long jump_even = code.size() - 1;

    // 6. Odd Case: Result (r3) += Multiplicand (r1)
    emit("RST", 0);
    emit("ADD", 3); // r0 = Result
    emit("ADD", 1); // r0 = Result + Multiplicand
    emit("SWP", 3); // Store back in r3

    // Label: jump_even
    code[jump_even].arg = code.size();

    // 7. Prepare Multiplier (r2) for next iteration
    //    Actually do the division by 2
    emit("SHR", 2); 

    // 8. Double the Multiplicand (r1)
    emit("SHL", 1);

    // 9. Loop
    emit("JUMP", loop_start);

    // --- END ---
    code[jump_end].arg = code.size();
    
    // Move Result (r3) to Output (r1)
    emit("RST", 0); emit("ADD", 3); emit("SWP", 1);
    
    // Restore RA from r4
    emit("RST", 0); emit("ADD", 4);

    emit("RTRN");
}

// Registers:
//  r0 (ra): Scratch
//  r1 (rb): Dividend (Input) -> Remainder
//  r2 (rc): Divisor (Constant)
//  r3 (rd): Return Address Backup
//  r4 (re): Shifted Divisor
//  r5 (rf): Bit Mask
//  r6 (rg): Quotient
void generate_div() {
    // Pseudocode:
    //  Align Shifted_Divisor (r4) with Dividend (r1)
    //  While Mask > 0:
    //     Quotient << 1
    //     If Shifted_Divisor <= Remainder:
    //        Remainder -= Shifted_Divisor
    //        Quotient++
    //     Shifted_Divisor >> 1
    //     Mask >> 1
    addr_div = code.size();
    
    emit("SWP", 3); 

    emit("SWP", 2);
    emit("JPOS", code.size() + 4, "DIV 0 CHECK");
    emit("RST", 1);
    emit("SWP", 3);
    emit("RTRN");
    
    emit("SWP", 2);

    // 1. Initialize
    emit("RST", 6); // rg = 0 (Quotient)
    emit("RST", 5); emit("INC", 5); // rf = 1 (Mask)
    emit("RST", 0); emit("ADD", 2); emit("SWP", 4); // re = rc (Divisor copy)

    // 2. Align (Scale Up)
    long long loop_up = code.size();

    // Check: r4 > r1 ?
    emit("RST", 0); emit("ADD", 4); emit("SUB", 1);
    emit("JPOS", 0); 
    long long jump_peak = code.size() - 1;

    emit("SHL", 4); // re << 1
    emit("SHL", 5); // rf << 1
    emit("JUMP", loop_up);

    // 3. Correction (Overshoot)
    code[jump_peak].arg = code.size();
    emit("SHR", 4);
    emit("SHR", 5);

    // 4. Compute (Scale Down)
    long long loop_down = code.size();

    // A. Check Termination (rf == 0)
    emit("RST", 0); emit("ADD", 5);
    emit("JZERO", 0);
    long long jump_end = code.size() - 1;

    // B. Shift Quotient
    emit("SHL", 6);

    // C. Compare (r4 <= r1 ?)
    // If r4 - r1 > 0, Jump
    emit("RST", 0); emit("ADD", 4); emit("SUB", 1);
    emit("JPOS", 0);
    long long jump_skip = code.size() - 1;

    // D. Subtract and Set Bit
    emit("SWP", 1); emit("SUB", 4); emit("SWP", 1); // r1 -= r4
    emit("INC", 6); // Set LSB

    code[jump_skip].arg = code.size();

    // E. Shift Down
    emit("SHR", 4);
    emit("SHR", 5);
    emit("JUMP", loop_down);

    // 5. Finalize
    code[jump_end].arg = code.size();
    
    // Result: Quotient in r6 -> r1
    emit("RST", 0); emit("ADD", 6); emit("SWP", 1); 
    
    // Restore RA from r3
    emit("RST", 0); emit("ADD", 3);
    emit("RTRN");
}

// addr_mod = code.size();

// Registers:
//  r0 (ra): Scratch
//  r1 (rb): Dividend (Input) -> Remainder (Result)
//  r2 (rc): Divisor (Constant)
//  r3 (rd): Return Address Backup
//  r4 (re): Shifted Divisor
//  r5 (rf): Bit Mask
void generate_mod() {
    // Pseudocode:
    //  Align Shifted_Divisor (r4) with Dividend (r1)
    //  While Mask > 0:
    //     If Shifted_Divisor <= Remainder:
    //        Remainder -= Shifted_Divisor
    //     Shifted_Divisor >> 1
    //     Mask >> 1
    addr_mod = code.size();
    
    emit("SWP", 3); // RA IN 3
    emit("SWP", 2);
    emit("JPOS", code.size() + 4, "DIV 0 CHECK");
    emit("RST", 1);
    emit("SWP", 3);
    emit("RTRN");
    
    emit("SWP", 2);
    // 1. Initialize
    emit("RST", 5); emit("INC", 5); // rf = 1 (Mask)
    emit("RST", 0); emit("ADD", 2); emit("SWP", 4); // re = rc (Divisor copy)

    // 2. Align (Scale Up)
    long long loop_up = code.size();

    // Check: r4 > r1 ?
    emit("RST", 0); emit("ADD", 4); emit("SUB", 1);
    emit("JPOS", 0); 
    long long jump_peak = code.size() - 1;

    emit("SHL", 4); // re << 1
    emit("SHL", 5); // rf << 1
    emit("JUMP", loop_up);

    // 3. Correction (Overshoot)
    code[jump_peak].arg = code.size();
    emit("SHR", 4);
    emit("SHR", 5);

    // 4. Compute (Scale Down)
    long long loop_down = code.size();

    // A. Check Termination (rf == 0)
    emit("RST", 0); emit("ADD", 5);
    emit("JZERO", 0);
    long long jump_end = code.size() - 1;

    // B. Compare (r4 <= r1 ?)
    // If r4 - r1 > 0, Jump
    emit("RST", 0); emit("ADD", 4); emit("SUB", 1);
    emit("JPOS", 0); // If too large, skip
    long long jump_skip = code.size() - 1;

    // C. Correction (rb -= re)
    emit("SWP", 1); emit("SUB", 4); emit("SWP", 1);
    
    code[jump_skip].arg = code.size();

    // D. Shift Down
    emit("SHR", 4);
    emit("SHR", 5);
    emit("JUMP", loop_down);

    // 5. Finalize
    code[jump_end].arg = code.size();

    // Restore RA from r3
    emit("RST", 0); emit("ADD", 3); 
    emit("RTRN");
}