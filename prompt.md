Rola: Jesteś ekspertem od konstrukcji kompilatorów i optymalizacji kodu niskopoziomowego. Twoim zadaniem jest przeprowadzenie mnie przez implementację generatora kodu w C++ dla narzędzi Bison i Flex.
Kontekst: Mam już działający lekser (Flex) i parser (Bison) dla prostego języka imperatywnego. Język ten nie obsługuje rekurencji, parametry przekazywane są przez referencję, a zmienne mają statyczny czas życia. Celem jest wygenerowanie kodu asemblerowego dla specyficznej Maszyny Wirtualnej (VM), minimalizując liczbę instrukcji oraz czas wykonania (cykle).
Specyfikacja VM (Kluczowe ograniczenia):
1. Rejestry: 8 rejestrów (ra - rh). ra to akumulator.
2. Koszt instrukcji:
    ◦ LOAD/STORE (pamięć): 50 cykli (bardzo drogie!).
    ◦ SWP (zamiana rejestrów), ADD, SUB: 5 cykli.
    ◦ SHL, SHR (przesunięcia bitowe), INC, DEC, JUMP: 1 cykl.
3. Arytmetyka: Maszyna nie ma instrukcji MUL, DIV, MOD. Muszą być zaimplementowane programowo w czasie logarytmicznym (O(logn)).
Zadanie: Zaimplementuj logikę generatora kodu w C++ wewnątrz pliku Bisona (.y), stosując strategię One-Pass Syntax-Directed Translation (bez budowania pełnego drzewa AST).
Plan Implementacji (Krok po kroku):
Krok 1: Infrastruktura (Bufor i Tablica Symboli)
• Zamiast printf, stwórz strukturę Instruction (opcode + arg) i globalny std::vector<Instruction> code_buffer.
• Zaimplementuj Tablicę Symboli mapującą zmienne na stałe adresy pamięci (zmienne globalne).
    ◦ Dla tablic: pamiętaj adres bazowy i zakres (np. tab[10:30]).
    ◦ Dla zmiennych lokalnych/parametrów: używaj prefiksów nazw (np. procName_varName) lub unikalnych ID, aby mapować je na płaską pamięć globalną (brak rekurencji = brak stosu ramki).
Krok 2: "Kernel Matematyczny" (Mnożenie/Dzielenie Logarytmiczne)
• Na początku bufora instrukcji (przed Main) wygeneruj hardcodowane procedury asemblerowe dla mnożenia, dzielenia i modulo.
• Wykorzystaj algorytm "Russian Peasant" (przesunięcia bitowe SHL/SHR i dodawanie), aby spełnić wymóg złożoności logarytmicznej.
• W gramatyce, dla operacji *, /, %, generuj instrukcję CALL do tych procedur.
Krok 3: Generowanie Wyrażeń z Optymalizacją Rejestrową
• W akcjach Bisona dla wyrażeń (np. expr + expr), nie używaj zmiennych tymczasowych w pamięci, jeśli nie musisz.
• Zastosuj schemat z użyciem SWP:
    1. Oblicz lewy operand (wynik w ra).
    2. Wygeneruj SWP b (schowaj wynik do rb – koszt 5).
    3. Oblicz prawy operand (wynik w ra).
    4. Wygeneruj ADD b (lub SUB, CALL itp.).
• Dla mnożenia/dzielenia przez potęgi dwójki (stałe), generuj SHL/SHR zamiast CALL.
Krok 4: Control Flow i Backpatching
• Dla IF i WHILE: Użyj stosu (std::stack<int>) do zapamiętywania indeksów instrukcji skoku (JZERO, JUMP) w buforze.
• Po sparsowaniu bloku (np. ELSE lub końca pętli), wróć do zapamiętanego indeksu i uzupełnij adres skoku (Backpatching).
• Dla warunków (np. a > b): Implementuj jako odejmowanie. LOAD a, SUB b, JPOS .... Pamiętaj o użyciu SWP przy ładowaniu operandów.
Krok 5: Pętle FOR (Licznik w dół)
• Iterator pętli jest niemodyfikowalny przez użytkownika. Oblicz liczbę iteracji (limit−start+1) raz przed pętlą.
• Zapisz licznik w tymczasowej komórce pamięci.
• Wewnątrz pętli używaj tylko DEC na liczniku i JZERO do wyjścia (koszt 1 cykl), zamiast pełnego porównania iteratora z limitem.
Krok 6: Procedury i Parametry
• Parametry są przekazywane przez referencję.
• Przy wywołaniu: Ładuj adres zmiennej, nie jej wartość.
• Wewnątrz procedury: Dla każdego dostępu do parametru używaj instrukcji RLOAD (Load Indirect) i RSTORE.
• Dla tablic w parametrach: Przekazuj adres bazowy. Dostęp do tab[i] realizuj jako: LOAD i, ADD adres_bazowy_z_rejestru, RLOAD/RSTORE.
Krok 7: Peephole Optimizer (Klucz do zwycięstwa)
• Stwórz funkcję optimize_code(), która jest wywoływana po zakończeniu parsowania, a przed zapisem do pliku.
• Zaimplementuj reguły usuwania nadmiarowego kodu:
    ◦ STORE x, LOAD x -> Usuń LOAD x (Wartość jest już w ra. Oszczędność: 50 cykli).
    ◦ LOAD x, INC, STORE x (Dla liczników) -> Pozostaw bez zmian (nie zamieniaj na ADD 1).
    ◦ JUMP label, label: -> Usuń JUMP.
Format Wyjścia: Proszę o kod C++ (fragmenty pliku .y i .h), który realizuje powyższe punkty. Skup się na strukturze CodeGenerator i kluczowych akcjach Bisona.