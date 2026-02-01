/*
 * Kod kompilatora na Języki Formalne i Techniki Translacji
 *
 * Autor: Dawid Dębkowski 
 * Numer Indeksu: 279714
 * 2026-02-01
*/

Narzędzia:
----------------------------------------
bison (GNU Bison) 3.8.2
flex 2.6.4
GNU Make 4.4.1
g++ 15.2.1 20260103


Biblioteki:
----------------------------------------
cln 1.3.7

Uruchomienie:
----------------------------------------
kompilator <plik_wejsciowy> <plik_wyjsciowy>

Pliki:
----------------------------------------
ReadMe.txt
Makefile
src/ast.cpp
src/ast.hpp
src/codegen.cpp
src/codegen.hpp
src/lexer.l
src/math_kernel.cpp
src/math_kernel.hpp
src/parser.y
src/symtable.cpp
src/symtable.hpp
src/types.hpp
