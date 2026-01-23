#include "math_kernel.hpp"
#include "codegen.hpp"
#include <vector>
#include <string>

using namespace std;

// Spill Memory Locations - not working now
const long long SPILL_RE = 2;
const long long SPILL_RF = 3;
const long long SPILL_RG = 4;

// poprzec na liste laboratoryjna
// w program 0 jest mnozenie

// Input: rb (left-side), rc (right-side)
// Output: rb (Result)
// Clobbers: ra, rb, rc, rd, re, rf,
// Preserves: re, rf, rg (via Spill), rh
void generate_mul() {
    addr_mul = code.size();
    
    // 1. Zapisz adres powrotu (RA jest w 'ra', chowamy do rejestru pomocniczego np. r6)
    // Uwaga: zakładam, że r3 to rejestr wyniku, więc RA chowamy np. do r6
    emit("SWP", 6); 

    // 2. Zerowanie wyniku (rd = 0)
    emit("RST", 3); 

    long long loop_start = code.size();

    // 3. Pobierz Mnożnik (rc) do Akumulatora
    //    To jest początek Twojego triku: ra = rc
    emit("RST", 0); 
    emit("ADD", 2); 
    
    // 4. Warunek końca: Jeśli rc == 0, koniec
    emit("JZERO", 0);
    long long jump_end = code.size() - 1;

    // 5. Trik Parzystości: Sprawdź LSB i przygotuj rc
    //    W tym momencie ra = rc (stare)
    
    emit("SHR", 2); // rc = floor(rc / 2)
    emit("SHL", 2); // rc = floor(rc / 2) * 2 (wyzerowany LSB)
    
    // 6. Oblicz bit parzystości: ra = ra - rc
    //    Jeśli było nieparzyste: ra = 1. Jeśli parzyste: ra = 0.
    emit("SUB", 2); 
    
    emit("JZERO", 0); 
    long long jump_even = code.size() - 1;

    // 7. JEŚLI NIEPARZYSTA (ra > 0): Dodaj rb do wyniku rd
    emit("RST", 0);
    emit("ADD", 3); // ra = rd
    emit("ADD", 1); // ra = rd + rb
    emit("SWP", 3); // Zapisz nowy wynik do rd

    // Etykieta: jump_even (omijamy dodawanie)
    code[jump_even].arg = code.size();

    // 8. Przywróć stan dzielenia dla rc (Dokończenie Twojego triku)
    //    Wcześniej zrobiliśmy SHR, potem SHL. Teraz rc jest parzyste.
    //    Musimy zrobić SHR jeszcze raz, żeby rc było faktycznie podzielone przez 2 na nowo.
    emit("SHR", 2); 

    // 9. Podwój Mnożną (rb = rb * 2)
    emit("SHL", 1);

    // 10. Skok do pętli
    emit("JUMP", loop_start);

    // --- KONIEC ---
    code[jump_end].arg = code.size();
    
    // Przenieś wynik rd -> rb (zgodnie z konwencją zwracania)
    emit("RST", 0); emit("ADD", 3); emit("SWP", 1);
    
    // Przywróć RA z r6
    emit("RST", 0); emit("ADD", 6);

    emit("RTRN");
}

// Input: rb (Dividend), rc (Divisor)
// Output: rb (Quotient), rc (Remainder)
// void generate_div() {
//     addr_div = code.size();
    
//     emit("SWP", 3); // Save RA
    
//     // Check Zero Divisor
//     emit("RST", 0); emit("ADD", 2);
//     emit("JZERO", 0); 
//     long long jump_zero = code.size() - 1;
    
//     // Init registers
//     emit("RST", 6); // rg = 0 (Quotient)
//     emit("RST", 5); emit("INC", 5); // rf = 1 (Mask)
//     emit("RST", 0); emit("ADD", 2); emit("SWP", 4); // re=rc
    
//     // --- LOOP UP ---
//     long long loop1 = code.size();
    
//     // Check re > rb (re - rb > 0)
//     emit("RST", 0); emit("ADD", 4); emit("SUB", 1);
//     emit("JPOS", 0);
//     long long jump_out1 = code.size() - 1;
    
//     emit("SHL", 4);
//     emit("SHL", 5);
//     emit("JUMP", loop1);
    
//     code[jump_out1].arg = code.size();
    
//     // Undo last shift
//     emit("SHR", 4);
//     emit("SHR", 5);
    
//     // --- LOOP DOWN ---
//     long long loop2 = code.size();
    
//     // Check rf == 0
//     emit("RST", 0); emit("ADD", 5);
//     emit("JZERO", 0);
//     long long jump_out2 = code.size() - 1;
    
//     // Check re > rb? (re - rb > 0)
//     emit("RST", 0); emit("ADD", 4); emit("SUB", 1);
//     emit("JPOS", 0);
//     long long jump_skip = code.size() - 1;
    
//     // rb -= re
//     emit("SWP", 1); emit("SUB", 4); emit("SWP", 1);
    
//     // rg += rf
//     emit("SWP", 6); emit("ADD", 5); emit("SWP", 6);
    
//     code[jump_skip].arg = code.size();
    
//     emit("SHR", 4);
//     emit("SHR", 5);
//     emit("JUMP", loop2);
    
//     code[jump_out2].arg = code.size();
    
//     // Finalize
//     // rc = Remainder (current rb)
//     // rb = Quotient (current rg)
    
//     // rc = rb
//     emit("RST", 0); emit("ADD", 1); emit("SWP", 2);
//     // rb = rg
//     emit("RST", 0); emit("ADD", 6); emit("SWP", 1);
    
//     // Restore RA
//     emit("SWP", 3);
//     emit("RTRN");
    
//     // Zero Handler
//     code[jump_zero].arg = code.size();
//     emit("RST", 1); emit("RST", 2); // Result 0, Rem 0
//     emit("SWP", 3);
//     emit("RTRN");
// }

void generate_div() {
    addr_div = code.size();
    
    // Zapisz RA (jeśli konieczne - koszt 5 cykli). 
    // Jeśli testy tego nie wymagają, wykomentuj.
    emit("SWP", 3); 

    // Inicjalizacja
    emit("RST", 6); // rg = 0 (Wynik)
    emit("RST", 5); emit("INC", 5); // rf = 1 (Maska)
    emit("RST", 0); emit("ADD", 2); emit("SWP", 4); // re = rc (Kopia dzielnika)

    // --- PĘTLA 1: ALIGN (Skalowanie w górę) ---
    long long loop_up = code.size();

    // Sprawdź: czy re > rb? (ra = re - rb)
    // Koszt: 11 cykli
    emit("RST", 0); emit("ADD", 4); emit("SUB", 1);
    emit("JPOS", 0); 
    long long jump_peak = code.size() - 1;

    // Przesuwaj
    emit("SHL", 4); // re << 1
    emit("SHL", 5); // rf << 1
    emit("JUMP", loop_up);

    // --- PRZEJŚCIE ---
    code[jump_peak].arg = code.size();
    emit("SHR", 4); // Cofnij o jeden (bo re > rb)
    emit("SHR", 5);

    // --- PĘTLA 2: COMPUTE (Odejmowanie) ---
    long long loop_down = code.size();

    // 1. Sprawdź koniec (czy maska rf == 0?)
    // Koszt: 6 cykli
    emit("RST", 0); emit("ADD", 5);
    emit("JZERO", 0);
    long long jump_end = code.size() - 1;

    // 2. Sprawdź: czy re > rb? (ra = re - rb)
    // Koszt: 11 cykli
    emit("RST", 0); emit("ADD", 4); emit("SUB", 1); 
    emit("JPOS", 0); // Jeśli re > rb, pomiń odejmowanie
    long long jump_skip = code.size() - 1;

    // 3. Odejmowanie (rb -= re)
    // Najtańsza metoda (bez RST): SWP b (ra=rb), SUB e (ra=rb-re), SWP b (rb=wynik)
    // Koszt: 15 cykli
    emit("SWP", 1); 
    emit("SUB", 4); 
    emit("SWP", 1);

    // 4. Dodanie bitu do wyniku (rg += rf)
    // Koszt: 15 cykli
    emit("SWP", 6); 
    emit("ADD", 5); 
    emit("SWP", 6);

    code[jump_skip].arg = code.size();

    // 5. Przesunięcie w dół
    emit("SHR", 4);
    emit("SHR", 5);
    emit("JUMP", loop_down);

    // --- KONIEC ---
    code[jump_end].arg = code.size();

    // Wyniki na swoje miejsce: rc = Reszta (z rb), rb = Wynik (z rg)
    // emit("RST", 0); emit("ADD", 1); emit("SWP", 2); // rc = rb
    emit("RST", 0); emit("ADD", 6); emit("SWP", 1); // rb = rg

    // Przywróć RA i wyjdź
    emit("RST", 0); emit("ADD", 3); // RA do ra
    emit("RTRN");
}

void generate_mod() {
    addr_mod = code.size();
    emit("SWP", 3); 

    // DZIELENIE
    
    // Inicjalizacja
    emit("RST", 6); // rg = 0 (Wynik)
    emit("RST", 5); emit("INC", 5); // rf = 1 (Maska)
    emit("RST", 0); emit("ADD", 2); emit("SWP", 4); // re = rc (Kopia dzielnika)

    // --- PĘTLA 1: ALIGN (Skalowanie w górę) ---
    long long loop_up = code.size();

    // Sprawdź: czy re > rb? (ra = re - rb)
    // Koszt: 11 cykli
    emit("RST", 0); emit("ADD", 4); emit("SUB", 1);
    emit("JPOS", 0); 
    long long jump_peak = code.size() - 1;

    // Przesuwaj
    emit("SHL", 4); // re << 1
    emit("SHL", 5); // rf << 1
    emit("JUMP", loop_up);

    // --- PRZEJŚCIE ---
    code[jump_peak].arg = code.size();
    emit("SHR", 4); // Cofnij o jeden (bo re > rb)
    emit("SHR", 5);

    // --- PĘTLA 2: COMPUTE (Odejmowanie) ---
    long long loop_down = code.size();

    // 1. Sprawdź koniec (czy maska rf == 0?)
    // Koszt: 6 cykli
    emit("RST", 0); emit("ADD", 5);
    emit("JZERO", 0);
    long long jump_end = code.size() - 1;

    // 2. Sprawdź: czy re > rb? (ra = re - rb)
    // Koszt: 11 cykli
    emit("RST", 0); emit("ADD", 4); emit("SUB", 1); 
    emit("JPOS", 0); // Jeśli re > rb, pomiń odejmowanie
    long long jump_skip = code.size() - 1;

    // 3. Odejmowanie (rb -= re)
    // Najtańsza metoda (bez RST): SWP b (ra=rb), SUB e (ra=rb-re), SWP b (rb=wynik)
    // Koszt: 15 cykli
    emit("SWP", 1); 
    emit("SUB", 4); 
    emit("SWP", 1);

    // 4. Dodanie bitu do wyniku (rg += rf)
    // Koszt: 15 cykli
    emit("SWP", 6); 
    emit("ADD", 5); 
    emit("SWP", 6);

    code[jump_skip].arg = code.size();

    // 5. Przesunięcie w dół
    emit("SHR", 4);
    emit("SHR", 5);
    emit("JUMP", loop_down);

    // --- KONIEC ---
    code[jump_end].arg = code.size();

    // Wyniki na swoje miejsce: rc = Reszta (z rb), rb = Wynik (z rg)
    emit("RST", 0); emit("ADD", 1); emit("SWP", 1); // rc = rb
    // emit("RST", 0); emit("ADD", 6); emit("SWP", 1); // rb = rg

    // koniec dzielenia

    emit("SWP", 3); // RA
    emit("RTRN");
}
