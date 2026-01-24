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

void generate_div() {
    addr_div = code.size();
    
    // Opcjonalnie: Zapisz RA (SWP 3) - pomijam dla czystej wydajności, jeśli to "inline"
    emit("SWP", 3); 

    // r1(rb) = Dzielna / Reszta
    // r2(rc) = Dzielnik (stały)
    // r4(re) = Przesuwany Dzielnik
    // r5(rf) = Licznik pętli (tylko maska do sprawdzania końca)
    // r6(rg) = Wynik

    // 1. Init
    emit("RST", 6); // rg = 0
    emit("RST", 5); emit("INC", 5); // rf = 1 (tylko do liczenia iteracji)
    emit("RST", 0); emit("ADD", 2); emit("SWP", 4); // re = rc

    // 2. ALIGN (W górę) - bez zmian, bo musimy znaleźć sufit
    long long loop_up = code.size();

    // Czy re > rb?
    emit("RST", 0); emit("ADD", 4); emit("SUB", 1);
    emit("JPOS", 0); 
    long long jump_peak = code.size() - 1;

    emit("SHL", 4); // re << 1
    emit("SHL", 5); // rf << 1
    emit("JUMP", loop_up);

    // 3. KOREKTA
    code[jump_peak].arg = code.size();
    emit("SHR", 4);
    emit("SHR", 5);

    // 4. COMPUTE (W dół) - TU JEST OPTYMALIZACJA
    long long loop_down = code.size();

    // A. Czy koniec pętli? (rf == 0) - koszt 7 cykli
    emit("RST", 0); emit("ADD", 5);
    emit("JZERO", 0);
    long long jump_end = code.size() - 1;

    // B. Przesuń dotychczasowy wynik w lewo (rg << 1) - koszt 1 cykl
    // To robi miejsce na nowy bit.
    emit("SHL", 6);

    // C. Sprawdź dopasowanie (re > rb?) - koszt 12 cykli
    emit("RST", 0); emit("ADD", 4); emit("SUB", 1);
    emit("JPOS", 0); // Jeśli nie pasuje, skaczemy
    long long jump_skip = code.size() - 1;

    // D. JEŚLI PASUJE:
    // Odejmij (rb -= re) - koszt 15 cykli
    emit("SWP", 1); emit("SUB", 4); emit("SWP", 1);
    
    // Wstaw bit 1 (INC rg) - koszt 1 cykl!
    // Ponieważ zrobiliśmy SHL na początku, LSB jest 0. INC robi z niego 1.
    emit("INC", 6);

    code[jump_skip].arg = code.size();

    // E. Przesuń okno w dół - koszt 2 cykle
    emit("SHR", 4);
    emit("SHR", 5);
    emit("JUMP", loop_down);

    // 5. FINALIZE
    code[jump_end].arg = code.size();
    
    // Ustawienie wyników: rc = Reszta, rb = Iloraz
    // emit("RST", 0); emit("ADD", 1); emit("SWP", 2); // rc = stara reszta
    emit("RST", 0); emit("ADD", 6); emit("SWP", 1); // rb = wynik
    
    // Restore RA
    emit("RST", 0); emit("ADD", 3);
    emit("RTRN");
}

// addr_mod = code.size();

// Wejście: rb (Liczba), rc (Modulo)
// Wyjście: rb (Wynik rb % rc)
void generate_mod() {
    addr_mod = code.size();
    // 1. Init
    // rb = Dzielna
    // rc = Dzielnik
    // re = Temp (przesuwany dzielnik)
    // rf = Maska (licznik pętli)
    
    // Nie potrzebujemy rg (wyniku dzielenia)!

    emit("SWP", 3); // Zapis RA (opcjonalnie)
    
    emit("RST", 5); emit("INC", 5); // rf = 1
    emit("RST", 0); emit("ADD", 2); emit("SWP", 4); // re = rc

    // --- ALIGN (W górę) ---
    long long loop_up = code.size();

    // Czy re > rb?
    emit("RST", 0); emit("ADD", 4); emit("SUB", 1);
    emit("JPOS", 0); 
    long long jump_peak = code.size() - 1;

    emit("SHL", 4); // re << 1
    emit("SHL", 5); // rf << 1
    emit("JUMP", loop_up);

    // --- KOREKTA ---
    code[jump_peak].arg = code.size();
    emit("SHR", 4);
    emit("SHR", 5);

    // --- COMPUTE (W dół) ---
    long long loop_down = code.size();

    // Koniec pętli? (rf == 0)
    emit("RST", 0); emit("ADD", 5);
    emit("JZERO", 0);
    long long jump_end = code.size() - 1;

    // Czy re > rb? (Czy można odjąć?)
    emit("RST", 0); emit("ADD", 4); emit("SUB", 1);
    emit("JPOS", 0); // Jeśli za duże, pomiń
    long long jump_skip = code.size() - 1;

    // Wykonaj odejmowanie (rb -= re)
    // To jest jedyna operacja, która nas interesuje! Zmniejszamy liczbę.
    emit("SWP", 1); emit("SUB", 4); emit("SWP", 1);
    
    // TU BYŁA OPTYMALIZACJA "INC rg" - TERAZ JEST USUNIĘTA.
    // Nie liczymy ile razy się zmieściło. Po prostu odejmujemy.

    code[jump_skip].arg = code.size();

    // Przesuń w dół
    emit("SHR", 4);
    emit("SHR", 5);
    emit("JUMP", loop_down);

    // --- KONIEC ---
    code[jump_end].arg = code.size();

    // Wynik jest już w rb!
    // (W dzieleniu musieliśmy zamieniać rc z rb, tutaj rb to już wynik)

    emit("RST", 0); emit("ADD", 3); // RA
    emit("RTRN");
}