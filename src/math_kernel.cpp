#include "math_kernel.hpp"
#include "codegen.hpp"
#include <vector>
#include <string>

using namespace std;

void generate_mul() {
    addr_mul = code.size();
    // rb * rc -> ra
    // Returns result in ra. 
    // Uses rd to store return address.
    // Clobbers rb, rc, re, rf, rg.
    
    emit("SWP", 3); // Save Return Address (ra) to rd
    
    emit("RST", 0); // ra = 0 (Accumulator for result) (Actually let's use re as accumulator)
    emit("RST", 4); // re = 0
    
    // Check if Rc or Rb is 0
    // ... Optimization
    
    long long loop_start = code.size();
    
    // If rc == 0 JUMP END
    emit("RST", 0);
    emit("ADD", 2); // ra = rc
    emit("JZERO", code.size() + 20); // Placeholder jump to END (we will fix later or use relative logic?)
    long long jump_is_zero = code.size() - 1; // Store index to patch
    
    // Verify odd parity of rc
    // rf = rc
    emit("RST", 5); // rf
    emit("ADD", 2); // rf = rc
    emit("SHR", 5); // rf = rc / 2
    emit("SHL", 5); // rf = (rc / 2) * 2
    
    // ra = rc - rf
    emit("RST", 0);
    emit("ADD", 2); // ra = rc
    emit("SUB", 5); // ra = rc - rf. If 1, Odd. If 0, Even.
    
    emit("JZERO", code.size() + 4); // If 0 (Even), Skip Add.
    long long jump_is_even = code.size() - 1;
    
    // Odd: re = re + rb
    emit("RST", 0);
    emit("ADD", 4); // ra = re
    emit("ADD", 1); // ra = re + rb
    emit("SWP", 4); // re = ra (new sum)
    
    // Backpatch jump_is_even
    code[jump_is_even].arg = code.size();
    
    // Double rb
    emit("SHL", 1);
    // Halve rc
    emit("SHR", 2);
    
    emit("JUMP", loop_start);
    
    // Backpatch jump_is_zero
    code[jump_is_zero].arg = code.size();
    
    // Move result re to ra
    // But ra might be needed for SWP rd logic.
    // Wait, SWP rd puts current ra into rd and rd into ra.
    // rd holds return address.
    // So we want: ra = result.
    // code:
    // ra = re
    // swap rd (ra = ret_addr, rd = result)
    // RTRN (jumps to ra)
    // But caller expects result in ra ??
    // Standard convention: ra is accumulator/result.
    // But RTRN uses Value in Accumulator (ra) as Address?
    // Check mw.cc: case RTRN: lr = r[0]; ...
    // YES. RTRN sets IP to r[0].
    
    // So we MUST put Return Address in r[0] before executing RTRN.
    // So Result CANNOT be returned in r[0].
    // Result must be returned in rb or rc or globally?
    // Let's convention: Result in rb.
    
    // So:
    // Move re (result) to rb.
    emit("RST", 0);
    emit("ADD", 4); // ra = re
    emit("SWP", 1); // rb = re, ra = old_rb. (Result now in rb)
    
    // Restore Return Address from rd to ra
    emit("RST", 0);
    emit("ADD", 3); // ra = rd
    
    emit("RTRN");
}

void generate_div() {
    addr_div = code.size();
    // rb / rc -> rb (Quotient), rc (Remainder)
    emit("SWP", 3); // Save ret addr in rd
    
    // Check divide by zero (rc=0) -> result 0?
    emit("RST", 0);
    emit("ADD", 2);
    emit("JZERO", code.size() + 2); 
    long long jmp_zero = code.size() - 1;
    
    // Stub: rb=0, rc=0
    emit("RST", 1);
    emit("RST", 2);

    // End stub
    code[jmp_zero].arg = code.size();
    emit("RST", 0);
    emit("ADD", 3);
    emit("RTRN");
}

void generate_mod() {
    addr_mod = code.size();
    emit("SWP", 3);
    emit("CALL", addr_div);
    emit("RST", 0);
    emit("ADD", 2); 
    emit("SWP", 1); 
    emit("RST", 0);
    emit("ADD", 3); 
    emit("RTRN");
}
