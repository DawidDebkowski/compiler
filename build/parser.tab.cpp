/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "src/parser.y"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <vector>
#include <string>
#include <map>
#include <iostream>

using namespace std;

extern int yylineno;
int yylex();
void yyerror(const char *s);

struct Instruction {
    string opcode;
    long long arg;
    bool has_arg;
};

// Global Code Buffer
vector<Instruction> code;

void emit(string opcode) {
    code.push_back({opcode, 0, false});
}

void emit(string opcode, long long arg) {
    code.push_back({opcode, arg, true});
}

// Symbol Table and Helpers
struct Symbol {
    long long address;
    bool is_array;
    bool is_param;
    long long start;
    long long end;
    
    // Spec flags
    string mod; // "T", "I", "O", ""
    bool is_iterator;
    bool is_initialized;
};

struct ProcedureInfo {
    long long address;
    vector<long long> param_addresses;
    vector<string> param_mods;     // "T", "I", "O", ""
    vector<bool> param_is_array;
};

map<string, Symbol> symbol_table;
map<string, ProcedureInfo> procedures_map;

long long memory_offset = 1000;
long long current_loop_start = -1; 

// Procedure handling
string current_procedure = ""; 
string current_call_proc = "";
int current_arg_idx = 0;

void add_symbol(string name, bool is_array, bool is_param, string mod, long long start, long long end) {
    if (is_array && start > end) {
        yyerror(("Invalid array range for " + name).c_str());
        exit(1);
    }

    string key = (current_procedure == "") ? name : current_procedure + "_" + name;
    if (symbol_table.count(key)) {
        yyerror(("Redeclaration: " + name).c_str());
        exit(1);
    }
    
    Symbol s;
    s.address = memory_offset;
    s.is_array = is_array;
    s.is_param = is_param;
    s.start = start;
    s.end = end;
    s.mod = mod;
    s.is_iterator = false;
    
    // Point 5: O means undefined value.
    if (mod == "O") s.is_initialized = false;
    else s.is_initialized = true; 

    symbol_table[key] = s;
    
    if (current_procedure != "" && is_param) {
        procedures_map[current_procedure].param_addresses.push_back(memory_offset);
        procedures_map[current_procedure].param_mods.push_back(mod);
        procedures_map[current_procedure].param_is_array.push_back(is_array);
        
        // Spec Check: T must be array
         if (mod == "T" && !is_array) {
            yyerror(("Parameter " + name + " marked T must be an array").c_str());
         }
         if (mod != "T" && is_array) {
             // Spec point 3: "nazwa tablicy ... powinna być poprzedzona literą T"
             yyerror(("Array parameter " + name + " must be marked with T").c_str());
         }
    }

    if (is_array) memory_offset += (end - start + 1);
    else memory_offset++;
}




Symbol* get_variable(string name) {
    // Try local first
    string key = current_procedure + "_" + name;
    if (symbol_table.find(key) != symbol_table.end()) {
        return &symbol_table[key];
    }
    // Try global (if any? arguments?)
    // In this language, do we have globals accessible in procs?
    // "Variables have static lifetime".
    // Usually local variables shadow globals or are unique?
    // Let's assume declarations are local to procedure.
    // If not found, check unprefixed (global)?
    if (symbol_table.count(name)) return &symbol_table[name];
    
    return nullptr;
}


void gen_const(int reg, long long value) {
    if (value < 0) value = 0; // Unsigned VM? Or signed? Prompt says ra is accumulator. No negative numbers mentioned explicitly but SUB exists. Instructions say "long long".
    // Assuming positive constants for now.
    
    emit("RST", reg);
    if (value == 0) return;
    
    // Binary expansion
    // e.g. 10 = 1010
    // 0: RST
    // MSB (1): INC
    // Next (0): SHL
    // Next (1): SHL, INC
    // Next (0): SHL
    
    unsigned long long v = value;
    int msb = 0;
    for(int i=63; i>=0; i--) {
        if ((v >> i) & 1) {
            msb = i;
            break;
        }
    }
    
    emit("INC", reg); // First 1
    
    for(int i=msb-1; i>=0; i--) {
        emit("SHL", reg);
        if ((v >> i) & 1) {
            emit("INC", reg);
        }
    }
}

// Math Lib Addresses
long long addr_mul = -1;
long long addr_div = -1;
long long addr_mod = -1;

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

// Helper for Identifier location
struct Location {
    long long address; 
    int reg;           
    Symbol* sym;
};

// struct Procedure replaced by map check

vector<long long> loop_stack;
vector<long long> if_stack;


#line 380 "build/parser.tab.cpp"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "parser.tab.hpp"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_PROCEDURE = 3,                  /* PROCEDURE  */
  YYSYMBOL_IS = 4,                         /* IS  */
  YYSYMBOL_IN = 5,                         /* IN  */
  YYSYMBOL_END = 6,                        /* END  */
  YYSYMBOL_PROGRAM = 7,                    /* PROGRAM  */
  YYSYMBOL_IF = 8,                         /* IF  */
  YYSYMBOL_THEN = 9,                       /* THEN  */
  YYSYMBOL_ELSE = 10,                      /* ELSE  */
  YYSYMBOL_ENDIF = 11,                     /* ENDIF  */
  YYSYMBOL_WHILE = 12,                     /* WHILE  */
  YYSYMBOL_DO = 13,                        /* DO  */
  YYSYMBOL_ENDWHILE = 14,                  /* ENDWHILE  */
  YYSYMBOL_REPEAT = 15,                    /* REPEAT  */
  YYSYMBOL_UNTIL = 16,                     /* UNTIL  */
  YYSYMBOL_FOR = 17,                       /* FOR  */
  YYSYMBOL_FROM = 18,                      /* FROM  */
  YYSYMBOL_TO = 19,                        /* TO  */
  YYSYMBOL_DOWNTO = 20,                    /* DOWNTO  */
  YYSYMBOL_READ = 21,                      /* READ  */
  YYSYMBOL_WRITE = 22,                     /* WRITE  */
  YYSYMBOL_T = 23,                         /* T  */
  YYSYMBOL_I = 24,                         /* I  */
  YYSYMBOL_O = 25,                         /* O  */
  YYSYMBOL_ASSIGN = 26,                    /* ASSIGN  */
  YYSYMBOL_EQ = 27,                        /* EQ  */
  YYSYMBOL_NEQ = 28,                       /* NEQ  */
  YYSYMBOL_GT = 29,                        /* GT  */
  YYSYMBOL_LT = 30,                        /* LT  */
  YYSYMBOL_GEQ = 31,                       /* GEQ  */
  YYSYMBOL_LEQ = 32,                       /* LEQ  */
  YYSYMBOL_PLUS = 33,                      /* PLUS  */
  YYSYMBOL_MINUS = 34,                     /* MINUS  */
  YYSYMBOL_MULT = 35,                      /* MULT  */
  YYSYMBOL_DIV = 36,                       /* DIV  */
  YYSYMBOL_MOD = 37,                       /* MOD  */
  YYSYMBOL_COMMA = 38,                     /* COMMA  */
  YYSYMBOL_COLON = 39,                     /* COLON  */
  YYSYMBOL_SEMICOLON = 40,                 /* SEMICOLON  */
  YYSYMBOL_LPAREN = 41,                    /* LPAREN  */
  YYSYMBOL_RPAREN = 42,                    /* RPAREN  */
  YYSYMBOL_LBRACKET = 43,                  /* LBRACKET  */
  YYSYMBOL_RBRACKET = 44,                  /* RBRACKET  */
  YYSYMBOL_pidentifier = 45,               /* pidentifier  */
  YYSYMBOL_num = 46,                       /* num  */
  YYSYMBOL_ENDFOR = 47,                    /* ENDFOR  */
  YYSYMBOL_YYACCEPT = 48,                  /* $accept  */
  YYSYMBOL_program_all = 49,               /* program_all  */
  YYSYMBOL_50_1 = 50,                      /* $@1  */
  YYSYMBOL_procedures = 51,                /* procedures  */
  YYSYMBOL_52_2 = 52,                      /* $@2  */
  YYSYMBOL_53_3 = 53,                      /* $@3  */
  YYSYMBOL_proc_head = 54,                 /* proc_head  */
  YYSYMBOL_55_4 = 55,                      /* $@4  */
  YYSYMBOL_main = 56,                      /* main  */
  YYSYMBOL_57_5 = 57,                      /* $@5  */
  YYSYMBOL_58_6 = 58,                      /* $@6  */
  YYSYMBOL_59_7 = 59,                      /* $@7  */
  YYSYMBOL_60_8 = 60,                      /* $@8  */
  YYSYMBOL_commands = 61,                  /* commands  */
  YYSYMBOL_command = 62,                   /* command  */
  YYSYMBOL_63_9 = 63,                      /* $@9  */
  YYSYMBOL_64_10 = 64,                     /* $@10  */
  YYSYMBOL_65_11 = 65,                     /* $@11  */
  YYSYMBOL_66_12 = 66,                     /* $@12  */
  YYSYMBOL_67_13 = 67,                     /* $@13  */
  YYSYMBOL_68_14 = 68,                     /* $@14  */
  YYSYMBOL_proc_call = 69,                 /* proc_call  */
  YYSYMBOL_70_15 = 70,                     /* $@15  */
  YYSYMBOL_declarations = 71,              /* declarations  */
  YYSYMBOL_args_decl = 72,                 /* args_decl  */
  YYSYMBOL_type = 73,                      /* type  */
  YYSYMBOL_args = 74,                      /* args  */
  YYSYMBOL_expression = 75,                /* expression  */
  YYSYMBOL_76_16 = 76,                     /* $@16  */
  YYSYMBOL_77_17 = 77,                     /* $@17  */
  YYSYMBOL_78_18 = 78,                     /* $@18  */
  YYSYMBOL_79_19 = 79,                     /* $@19  */
  YYSYMBOL_80_20 = 80,                     /* $@20  */
  YYSYMBOL_condition = 81,                 /* condition  */
  YYSYMBOL_82_21 = 82,                     /* $@21  */
  YYSYMBOL_83_22 = 83,                     /* $@22  */
  YYSYMBOL_84_23 = 84,                     /* $@23  */
  YYSYMBOL_85_24 = 85,                     /* $@24  */
  YYSYMBOL_86_25 = 86,                     /* $@25  */
  YYSYMBOL_87_26 = 87,                     /* $@26  */
  YYSYMBOL_value = 88,                     /* value  */
  YYSYMBOL_identifier = 89                 /* identifier  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   233

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  48
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  42
/* YYNRULES -- Number of rules.  */
#define YYNRULES  76
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  160

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   302


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   330,   330,   330,   341,   341,   347,   347,   353,   356,
     356,   369,   373,   369,   376,   380,   376,   385,   386,   389,
     399,   399,   411,   416,   424,   424,   433,   442,   433,   481,
     489,   481,   527,   528,   538,   543,   543,   551,   554,   557,
     560,   565,   568,   573,   574,   575,   576,   579,   603,   629,
     630,   630,   631,   631,   632,   632,   633,   633,   634,   634,
     637,   637,   644,   644,   651,   651,   656,   656,   661,   661,
     666,   666,   674,   675,   683,   698,   716
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "PROCEDURE", "IS",
  "IN", "END", "PROGRAM", "IF", "THEN", "ELSE", "ENDIF", "WHILE", "DO",
  "ENDWHILE", "REPEAT", "UNTIL", "FOR", "FROM", "TO", "DOWNTO", "READ",
  "WRITE", "T", "I", "O", "ASSIGN", "EQ", "NEQ", "GT", "LT", "GEQ", "LEQ",
  "PLUS", "MINUS", "MULT", "DIV", "MOD", "COMMA", "COLON", "SEMICOLON",
  "LPAREN", "RPAREN", "LBRACKET", "RBRACKET", "pidentifier", "num",
  "ENDFOR", "$accept", "program_all", "$@1", "procedures", "$@2", "$@3",
  "proc_head", "$@4", "main", "$@5", "$@6", "$@7", "$@8", "commands",
  "command", "$@9", "$@10", "$@11", "$@12", "$@13", "$@14", "proc_call",
  "$@15", "declarations", "args_decl", "type", "args", "expression",
  "$@16", "$@17", "$@18", "$@19", "$@20", "condition", "$@21", "$@22",
  "$@23", "$@24", "$@25", "$@26", "value", "identifier", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-42)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-71)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -42,     5,   -42,   -42,     4,   -25,     8,   -42,    18,    42,
      72,   -42,    -3,    33,    85,     9,   -42,    48,    -1,     1,
     -42,   -42,   -42,   -42,    10,    58,   188,    67,   -42,    60,
     -42,   188,     9,   -42,   -42,    11,    11,   -42,    69,    77,
      11,   -28,     2,   -42,    93,   111,   100,   188,    97,   188,
      23,    99,   102,   -42,   143,    70,   -42,   140,   188,   144,
     118,   120,   -42,    27,   -42,   -42,   -42,    11,   131,    43,
     138,   126,   -42,   -42,   188,   139,   152,   156,   163,   155,
     162,   188,    59,    11,   -42,   -42,   154,   160,   164,   167,
      75,   168,   -42,   172,   -42,   180,    11,    11,    11,    11,
      11,    11,   153,    11,   193,   -42,    24,   -42,   -42,   -42,
     181,   182,   183,   179,   184,   -42,   171,   -42,   -42,   -42,
     -42,   -42,   -42,   -42,   -42,   -42,   186,   200,   202,   175,
     -42,    11,    11,    11,    11,    11,   185,   188,   -42,    11,
      11,   -42,   -42,   -42,   -42,   -42,   -42,   -42,   161,   -42,
     -42,   -42,   210,   211,   188,   188,   134,   142,   -42,   -42
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       2,     0,     8,     1,     0,     0,     0,     3,     0,     0,
      11,     9,     0,     0,     0,    46,     6,    39,     0,     0,
      15,    43,    44,    45,     0,     0,     0,     0,     4,     0,
      12,     0,    46,    10,    42,     0,     0,    24,     0,     0,
       0,    74,     0,    18,     0,     0,     0,     0,    37,     0,
       0,     0,    74,    72,     0,    60,    73,     0,     0,     0,
       0,     0,    35,     0,     7,    17,    32,     0,     0,     0,
       0,     0,    16,    41,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    33,    34,     0,     0,     0,     0,
      49,     0,     5,     0,    13,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    26,    48,     0,    76,    75,    19,
       0,     0,     0,     0,     0,    40,     0,    20,    22,    61,
      63,    65,    67,    69,    71,    23,     0,     0,     0,     0,
      36,     0,     0,     0,     0,     0,     0,     0,    25,     0,
       0,    47,    51,    53,    55,    57,    59,    38,     0,    27,
      30,    21,     0,     0,     0,     0,     0,     0,    28,    31
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -42,   -42,   -42,   -42,   -42,   -42,   -42,   -42,   -42,   -42,
     -42,   -42,   -42,   -31,   -41,   -42,   -42,   -42,   -42,   -42,
     -42,   -42,   -42,   214,   -42,   196,   -42,   -42,   -42,   -42,
     -42,   -42,   -42,   -33,   -42,   -42,   -42,   -42,   -42,   -42,
      -4,   -14
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     1,     2,     4,    47,    26,     9,    15,     7,    13,
      49,    14,    31,    42,    43,   137,    58,   127,   152,   128,
     153,    44,    86,    18,    24,    25,   106,    89,   110,   111,
     112,   113,   114,    54,    75,    76,    77,    78,    79,    80,
      55,    45
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      50,    65,    16,    57,    28,     3,    30,     5,    64,    65,
      35,     6,    10,    62,    36,    63,    69,    37,    71,    38,
       8,    56,    56,    39,    40,    60,    56,    82,    65,    72,
      65,    35,    21,    22,    23,    36,    61,    29,    37,    29,
      38,    65,    17,    95,    39,    40,    12,    41,    32,    92,
     102,    35,    33,    56,    65,    36,    52,    53,    37,    11,
      38,    65,   129,    90,    39,    40,   130,    35,    41,    56,
     126,    36,    87,    88,    37,   103,    38,   -14,    17,   104,
      39,    40,    56,    56,    56,    56,    56,    56,    41,    56,
      20,    27,   119,   120,   121,   122,   123,   124,   -62,   -64,
     -66,   -68,   -70,    34,    41,    48,   148,    65,   -50,   -52,
     -54,   -56,   -58,    46,    59,    65,    65,    56,    56,    56,
      56,    56,    52,   156,   157,    56,    56,   142,   143,   144,
     145,   146,    94,    66,    35,   149,   150,    67,    36,    68,
      70,    37,    35,    38,    73,    63,    36,    39,    40,    37,
      35,    38,    74,    81,    36,    39,    40,    37,    84,    38,
      85,    35,    83,    39,    40,    36,    96,   125,    37,    35,
      38,    41,   151,    36,    39,    40,    37,    91,    38,    41,
      97,   158,    39,    40,    93,    98,   100,    41,    35,   159,
     117,   118,    36,    99,   101,    37,    35,    38,    41,   105,
      36,    39,    40,    37,   107,    38,    41,   109,   108,    39,
      40,   116,   115,   -29,   131,   134,   132,   136,   133,   139,
     141,   135,   140,   154,   155,    41,   138,    19,    51,   147,
       0,     0,     0,    41
};

static const yytype_int16 yycheck[] =
{
      31,    42,     5,    36,     5,     0,     5,     3,     6,    50,
       8,     7,     4,    41,    12,    43,    47,    15,    49,    17,
      45,    35,    36,    21,    22,    39,    40,    58,    69,     6,
      71,     8,    23,    24,    25,    12,    40,    38,    15,    38,
      17,    82,    45,    74,    21,    22,     4,    45,    38,     6,
      81,     8,    42,    67,    95,    12,    45,    46,    15,    41,
      17,   102,    38,    67,    21,    22,    42,     8,    45,    83,
     103,    12,    45,    46,    15,    16,    17,     5,    45,    83,
      21,    22,    96,    97,    98,    99,   100,   101,    45,   103,
       5,    43,    96,    97,    98,    99,   100,   101,    28,    29,
      30,    31,    32,    45,    45,    45,   137,   148,    33,    34,
      35,    36,    37,    46,    45,   156,   157,   131,   132,   133,
     134,   135,    45,   154,   155,   139,   140,   131,   132,   133,
     134,   135,     6,    40,     8,   139,   140,    26,    12,    39,
      43,    15,     8,    17,    45,    43,    12,    21,    22,    15,
       8,    17,     9,    13,    12,    21,    22,    15,    40,    17,
      40,     8,    18,    21,    22,    12,    27,    14,    15,     8,
      17,    45,    11,    12,    21,    22,    15,    46,    17,    45,
      28,    47,    21,    22,    46,    29,    31,    45,     8,    47,
      10,    11,    12,    30,    32,    15,     8,    17,    45,    45,
      12,    21,    22,    15,    44,    17,    45,    40,    44,    21,
      22,    39,    44,    20,    33,    36,    34,    46,    35,    19,
      45,    37,    20,    13,    13,    45,    40,    13,    32,    44,
      -1,    -1,    -1,    45
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    49,    50,     0,    51,     3,     7,    56,    45,    54,
       4,    41,     4,    57,    59,    55,     5,    45,    71,    71,
       5,    23,    24,    25,    72,    73,    53,    43,     5,    38,
       5,    60,    38,    42,    45,     8,    12,    15,    17,    21,
      22,    45,    61,    62,    69,    89,    46,    52,    45,    58,
      61,    73,    45,    46,    81,    88,    89,    81,    64,    45,
      89,    88,    41,    43,     6,    62,    40,    26,    39,    61,
      43,    61,     6,    45,     9,    82,    83,    84,    85,    86,
      87,    13,    61,    18,    40,    40,    70,    45,    46,    75,
      88,    46,     6,    46,     6,    61,    27,    28,    29,    30,
      31,    32,    61,    16,    88,    45,    74,    44,    44,    40,
      76,    77,    78,    79,    80,    44,    39,    10,    11,    88,
      88,    88,    88,    88,    88,    14,    81,    65,    67,    38,
      42,    33,    34,    35,    36,    37,    46,    63,    40,    19,
      20,    45,    88,    88,    88,    88,    88,    44,    61,    88,
      88,    11,    66,    68,    13,    13,    61,    61,    47,    47
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    48,    50,    49,    52,    51,    53,    51,    51,    55,
      54,    57,    58,    56,    59,    60,    56,    61,    61,    62,
      63,    62,    62,    62,    64,    62,    65,    66,    62,    67,
      68,    62,    62,    62,    62,    70,    69,    71,    71,    71,
      71,    72,    72,    73,    73,    73,    73,    74,    74,    75,
      76,    75,    77,    75,    78,    75,    79,    75,    80,    75,
      82,    81,    83,    81,    84,    81,    85,    81,    86,    81,
      87,    81,    88,    88,    89,    89,    89
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     3,     0,     9,     0,     8,     0,     0,
       5,     0,     0,     8,     0,     0,     7,     2,     1,     4,
       0,     8,     5,     5,     0,     6,     0,     0,    11,     0,
       0,    11,     2,     3,     3,     0,     5,     3,     8,     1,
       6,     4,     2,     1,     1,     1,     0,     3,     1,     1,
       0,     4,     0,     4,     0,     4,     0,     4,     0,     4,
       0,     4,     0,     4,     0,     4,     0,     4,     0,     4,
       0,     4,     1,     1,     1,     4,     4
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* $@1: %empty  */
#line 330 "src/parser.y"
              { 
     emit("JUMP", 0); // JUMP to Main
     generate_mul();
     generate_div();
     generate_mod();
   }
#line 1582 "build/parser.tab.cpp"
    break;

  case 3: /* program_all: $@1 procedures main  */
#line 335 "src/parser.y"
                     {
      code[0].arg = symbol_table["main_start"].address; 
      emit("HALT");
   }
#line 1591 "build/parser.tab.cpp"
    break;

  case 4: /* $@2: %empty  */
#line 341 "src/parser.y"
                                                               {
        procedures_map[current_procedure].address = code.size();
    }
#line 1599 "build/parser.tab.cpp"
    break;

  case 5: /* procedures: procedures PROCEDURE proc_head IS declarations IN $@2 commands END  */
#line 343 "src/parser.y"
                   {
        emit("RTRN");
        current_procedure = "";
    }
#line 1608 "build/parser.tab.cpp"
    break;

  case 6: /* $@3: %empty  */
#line 347 "src/parser.y"
                                           {
        procedures_map[current_procedure].address = code.size();
    }
#line 1616 "build/parser.tab.cpp"
    break;

  case 7: /* procedures: procedures PROCEDURE proc_head IS IN $@3 commands END  */
#line 349 "src/parser.y"
                   {
        emit("RTRN");
        current_procedure = "";
    }
#line 1625 "build/parser.tab.cpp"
    break;

  case 9: /* $@4: %empty  */
#line 356 "src/parser.y"
                               { 
        current_procedure = string((yyvsp[-1].str));
        if (procedures_map.count(current_procedure)) {
             yyerror(("Redeclaration of procedure " + current_procedure).c_str());
             exit(1);
        }
        ProcedureInfo info;
        info.address = -1; 
        procedures_map[current_procedure] = info;
    }
#line 1640 "build/parser.tab.cpp"
    break;

  case 10: /* proc_head: pidentifier LPAREN $@4 args_decl RPAREN  */
#line 365 "src/parser.y"
                       {
    }
#line 1647 "build/parser.tab.cpp"
    break;

  case 11: /* $@5: %empty  */
#line 369 "src/parser.y"
                  { 
         current_procedure = "main"; 
         Symbol s; s.address = code.size();
         symbol_table["main_start"] = s; 
       }
#line 1657 "build/parser.tab.cpp"
    break;

  case 12: /* $@6: %empty  */
#line 373 "src/parser.y"
                         {
         symbol_table["main_start"].address = code.size();
       }
#line 1665 "build/parser.tab.cpp"
    break;

  case 14: /* $@7: %empty  */
#line 376 "src/parser.y"
                 { 
         current_procedure = "main"; 
         Symbol s; s.address = code.size(); 
         symbol_table["main_start"] = s;
       }
#line 1675 "build/parser.tab.cpp"
    break;

  case 15: /* $@8: %empty  */
#line 380 "src/parser.y"
            {
         symbol_table["main_start"].address = code.size();
       }
#line 1683 "build/parser.tab.cpp"
    break;

  case 19: /* command: identifier ASSIGN expression SEMICOLON  */
#line 389 "src/parser.y"
                                                 {
        if ((yyvsp[-3].loc)->sym->is_iterator) yyerror("Cannot modify loop iterator");
        if ((yyvsp[-3].loc)->sym->mod == "I") yyerror("Cannot modify initialized parameter (I)");
        
        (yyvsp[-3].loc)->sym->is_initialized = true;
        
        if ((yyvsp[-3].loc)->reg != -1) emit("RSTORE", (yyvsp[-3].loc)->reg);
        else emit("STORE", (yyvsp[-3].loc)->address);
        delete (yyvsp[-3].loc);
    }
#line 1698 "build/parser.tab.cpp"
    break;

  case 20: /* $@9: %empty  */
#line 399 "src/parser.y"
                                      {
        long long jump_over_else = code.size();
        emit("JUMP", 0); 
        long long false_jump_idx = if_stack.back();
        if_stack.pop_back();
        code[false_jump_idx].arg = code.size();
        if_stack.push_back(jump_over_else);
    }
#line 1711 "build/parser.tab.cpp"
    break;

  case 21: /* command: IF condition THEN commands ELSE $@9 commands ENDIF  */
#line 406 "src/parser.y"
                     {
        long long jump_over_else = if_stack.back();
        if_stack.pop_back();
        code[jump_over_else].arg = code.size();
    }
#line 1721 "build/parser.tab.cpp"
    break;

  case 22: /* command: IF condition THEN commands ENDIF  */
#line 411 "src/parser.y"
                                       {
        long long false_jump_idx = if_stack.back();
        if_stack.pop_back();
        code[false_jump_idx].arg = code.size();
    }
#line 1731 "build/parser.tab.cpp"
    break;

  case 23: /* command: WHILE condition DO commands ENDWHILE  */
#line 416 "src/parser.y"
                                           {
         long long jump_out = if_stack.back();
         if_stack.pop_back();
         long long start = loop_stack.back();
         loop_stack.pop_back();
         emit("JUMP", start);
         code[jump_out].arg = code.size();
    }
#line 1744 "build/parser.tab.cpp"
    break;

  case 24: /* $@10: %empty  */
#line 424 "src/parser.y"
             {
        loop_stack.push_back(code.size());
    }
#line 1752 "build/parser.tab.cpp"
    break;

  case 25: /* command: REPEAT $@10 commands UNTIL condition SEMICOLON  */
#line 426 "src/parser.y"
                                         {
         long long jump_if_false = if_stack.back(); 
         if_stack.pop_back();
         long long start = loop_stack.back();
         loop_stack.pop_back();
         code[jump_if_false].arg = start;
    }
#line 1764 "build/parser.tab.cpp"
    break;

  case 26: /* $@11: %empty  */
#line 433 "src/parser.y"
                                 {
          Symbol* iter = get_variable(string((yyvsp[-2].str)));
          if (iter->is_iterator) yyerror("Nested loop with same iterator");
          if (iter->is_param) yyerror("Iterator must be local variable");
          
          iter->is_iterator = true;
          iter->is_initialized = true;
          
          emit("STORE", iter->address);
    }
#line 1779 "build/parser.tab.cpp"
    break;

  case 27: /* $@12: %empty  */
#line 442 "src/parser.y"
               {
          emit("SWP", 1); 
          Symbol* iter = get_variable(string((yyvsp[-5].str)));
          emit("LOAD", iter->address);
          
          emit("SWP", 1); 
          emit("SUB", 1); 
          emit("INC", 0); 
          
          long long count_addr = memory_offset++;
          emit("STORE", count_addr);
          
          long long start_loop = code.size();
          emit("LOAD", count_addr);
          emit("JZERO", 0);
          long long jump_out = code.size()-1;
          
          loop_stack.push_back(start_loop);
          loop_stack.push_back(jump_out);
          loop_stack.push_back(count_addr);
    }
#line 1805 "build/parser.tab.cpp"
    break;

  case 28: /* command: FOR pidentifier FROM value $@11 TO value $@12 DO commands ENDFOR  */
#line 462 "src/parser.y"
                         {
          long long count_addr = loop_stack.back(); loop_stack.pop_back();
          long long jump_out = loop_stack.back(); loop_stack.pop_back();
          long long start_loop = loop_stack.back(); loop_stack.pop_back();
          
          emit("LOAD", count_addr);
          emit("DEC", 0);
          emit("STORE", count_addr);
          
          Symbol* iter = get_variable(string((yyvsp[-9].str)));
          emit("LOAD", iter->address);
          emit("INC", 0);
          emit("STORE", iter->address);
          
          emit("JUMP", start_loop);
          code[jump_out].arg = code.size();
          
          iter->is_iterator = false;
    }
#line 1829 "build/parser.tab.cpp"
    break;

  case 29: /* $@13: %empty  */
#line 481 "src/parser.y"
                                 {
          Symbol* iter = get_variable(string((yyvsp[-2].str)));
          if (iter->is_iterator) yyerror("Nested loop with same iterator");
          if (iter->is_param) yyerror("Iterator must be local variable");
          iter->is_iterator = true;
          iter->is_initialized = true;

          emit("STORE", iter->address); 
    }
#line 1843 "build/parser.tab.cpp"
    break;

  case 30: /* $@14: %empty  */
#line 489 "src/parser.y"
                   {
          emit("SWP", 1); 
          Symbol* iter = get_variable(string((yyvsp[-5].str)));
          emit("LOAD", iter->address);
          
          emit("SUB", 1); 
          emit("INC", 0); 
          
          long long count_addr = memory_offset++;
          emit("STORE", count_addr);
          
          long long start_loop = code.size();
          emit("LOAD", count_addr);
          emit("JZERO", 0);
          long long jump_out = code.size()-1;
          
          loop_stack.push_back(start_loop);
          loop_stack.push_back(jump_out);
          loop_stack.push_back(count_addr);
    }
#line 1868 "build/parser.tab.cpp"
    break;

  case 31: /* command: FOR pidentifier FROM value $@13 DOWNTO value $@14 DO commands ENDFOR  */
#line 508 "src/parser.y"
                         {
          long long count_addr = loop_stack.back(); loop_stack.pop_back();
          long long jump_out = loop_stack.back(); loop_stack.pop_back();
          long long start_loop = loop_stack.back(); loop_stack.pop_back();
          
          emit("LOAD", count_addr);
          emit("DEC", 0);
          emit("STORE", count_addr);
          
          Symbol* iter = get_variable(string((yyvsp[-9].str)));
          emit("LOAD", iter->address);
          emit("DEC", 0);
          emit("STORE", iter->address);
          
          emit("JUMP", start_loop);
          code[jump_out].arg = code.size();
          
          iter->is_iterator = false;
    }
#line 1892 "build/parser.tab.cpp"
    break;

  case 33: /* command: READ identifier SEMICOLON  */
#line 528 "src/parser.y"
                                {
        if ((yyvsp[-1].loc)->sym->is_iterator) yyerror("Cannot read into loop iterator");
        if ((yyvsp[-1].loc)->sym->mod == "I") yyerror("Cannot read into I parameter");
        (yyvsp[-1].loc)->sym->is_initialized = true;

        emit("READ");
        if ((yyvsp[-1].loc)->reg != -1) emit("RSTORE", (yyvsp[-1].loc)->reg);
        else emit("STORE", (yyvsp[-1].loc)->address);
        delete (yyvsp[-1].loc);
    }
#line 1907 "build/parser.tab.cpp"
    break;

  case 34: /* command: WRITE value SEMICOLON  */
#line 538 "src/parser.y"
                            {
        emit("WRITE");
    }
#line 1915 "build/parser.tab.cpp"
    break;

  case 35: /* $@15: %empty  */
#line 543 "src/parser.y"
                               { 
        current_call_proc = string((yyvsp[-1].str));
        current_arg_idx = 0;
    }
#line 1924 "build/parser.tab.cpp"
    break;

  case 36: /* proc_call: pidentifier LPAREN $@15 args RPAREN  */
#line 546 "src/parser.y"
                  {
        emit("CALL", procedures_map[(yyvsp[-4].str)].address);
    }
#line 1932 "build/parser.tab.cpp"
    break;

  case 37: /* declarations: declarations COMMA pidentifier  */
#line 551 "src/parser.y"
                                              {
        add_symbol(string((yyvsp[0].str)), false, false, "", 0, 0);
    }
#line 1940 "build/parser.tab.cpp"
    break;

  case 38: /* declarations: declarations COMMA pidentifier LBRACKET num COLON num RBRACKET  */
#line 554 "src/parser.y"
                                                                     {
        add_symbol(string((yyvsp[-5].str)), true, false, "", (yyvsp[-3].num), (yyvsp[-1].num));
    }
#line 1948 "build/parser.tab.cpp"
    break;

  case 39: /* declarations: pidentifier  */
#line 557 "src/parser.y"
                  {
        add_symbol(string((yyvsp[0].str)), false, false, "", 0, 0);
    }
#line 1956 "build/parser.tab.cpp"
    break;

  case 40: /* declarations: pidentifier LBRACKET num COLON num RBRACKET  */
#line 560 "src/parser.y"
                                                  {
        add_symbol(string((yyvsp[-5].str)), true, false, "", (yyvsp[-3].num), (yyvsp[-1].num));
    }
#line 1964 "build/parser.tab.cpp"
    break;

  case 41: /* args_decl: args_decl COMMA type pidentifier  */
#line 565 "src/parser.y"
                                             {
       add_symbol(string((yyvsp[0].str)), false, true, string((yyvsp[-1].str)), 0, 0); free((yyvsp[-1].str));
    }
#line 1972 "build/parser.tab.cpp"
    break;

  case 42: /* args_decl: type pidentifier  */
#line 568 "src/parser.y"
                       {
       add_symbol(string((yyvsp[0].str)), false, true, string((yyvsp[-1].str)), 0, 0); free((yyvsp[-1].str));
    }
#line 1980 "build/parser.tab.cpp"
    break;

  case 43: /* type: T  */
#line 573 "src/parser.y"
         { (yyval.str) = strdup("T"); }
#line 1986 "build/parser.tab.cpp"
    break;

  case 44: /* type: I  */
#line 574 "src/parser.y"
         { (yyval.str) = strdup("I"); }
#line 1992 "build/parser.tab.cpp"
    break;

  case 45: /* type: O  */
#line 575 "src/parser.y"
         { (yyval.str) = strdup("O"); }
#line 1998 "build/parser.tab.cpp"
    break;

  case 46: /* type: %empty  */
#line 576 "src/parser.y"
                   { (yyval.str) = strdup(""); }
#line 2004 "build/parser.tab.cpp"
    break;

  case 47: /* args: args COMMA pidentifier  */
#line 579 "src/parser.y"
                              {
        Symbol* s = get_variable(string((yyvsp[0].str)));
        if (!s) yyerror(("Undefined variable " + string((yyvsp[0].str))).c_str());
        
        ProcedureInfo& info = procedures_map[current_call_proc];
        if (current_arg_idx >= (int)info.param_addresses.size()) yyerror("Too many arguments for procedure call");
        
        long long param_addr = info.param_addresses[current_arg_idx];
        string p_mod = info.param_mods[current_arg_idx];
        bool p_array = info.param_is_array[current_arg_idx];
        
        if (p_array && !s->is_array) yyerror(("Expected array for parameter " + to_string(current_arg_idx+1)).c_str());
        if (!p_array && s->is_array) yyerror(("Expected scalar for parameter " + to_string(current_arg_idx+1)).c_str());

        current_arg_idx++;

        if (s->is_param) {
            emit("LOAD", s->address); 
            emit("STORE", param_addr); 
        } else {
            gen_const(0, s->address);
            emit("STORE", param_addr);
        }
    }
#line 2033 "build/parser.tab.cpp"
    break;

  case 48: /* args: pidentifier  */
#line 603 "src/parser.y"
                  {
        Symbol* s = get_variable(string((yyvsp[0].str)));
        if (!s) yyerror(("Undefined variable " + string((yyvsp[0].str))).c_str());
        
        ProcedureInfo& info = procedures_map[current_call_proc];
        if (current_arg_idx >= (int)info.param_addresses.size()) yyerror("Too many arguments for procedure call");
        
        long long param_addr = info.param_addresses[current_arg_idx];
        string p_mod = info.param_mods[current_arg_idx];
        bool p_array = info.param_is_array[current_arg_idx];
        
        if (p_array && !s->is_array) yyerror(("Expected array for parameter " + to_string(current_arg_idx+1)).c_str());
        if (!p_array && s->is_array) yyerror(("Expected scalar for parameter " + to_string(current_arg_idx+1)).c_str());

        current_arg_idx++;

        if (s->is_param) {
            emit("LOAD", s->address); 
            emit("STORE", param_addr);
        } else {
            gen_const(0, s->address);
            emit("STORE", param_addr);
        }
    }
#line 2062 "build/parser.tab.cpp"
    break;

  case 50: /* $@16: %empty  */
#line 630 "src/parser.y"
            { emit("SWP", 1); }
#line 2068 "build/parser.tab.cpp"
    break;

  case 51: /* expression: value $@16 PLUS value  */
#line 630 "src/parser.y"
                                           { emit("ADD", 1); }
#line 2074 "build/parser.tab.cpp"
    break;

  case 52: /* $@17: %empty  */
#line 631 "src/parser.y"
            { emit("SWP", 1); }
#line 2080 "build/parser.tab.cpp"
    break;

  case 53: /* expression: value $@17 MINUS value  */
#line 631 "src/parser.y"
                                            { emit("SWP", 1); emit("SUB", 1); }
#line 2086 "build/parser.tab.cpp"
    break;

  case 54: /* $@18: %empty  */
#line 632 "src/parser.y"
            { emit("SWP", 1); }
#line 2092 "build/parser.tab.cpp"
    break;

  case 55: /* expression: value $@18 MULT value  */
#line 632 "src/parser.y"
                                           { emit("SWP", 2); emit("CALL", addr_mul); emit("SWP", 1); }
#line 2098 "build/parser.tab.cpp"
    break;

  case 56: /* $@19: %empty  */
#line 633 "src/parser.y"
            { emit("SWP", 1); }
#line 2104 "build/parser.tab.cpp"
    break;

  case 57: /* expression: value $@19 DIV value  */
#line 633 "src/parser.y"
                                          { emit("SWP", 2); emit("CALL", addr_div); emit("SWP", 1); }
#line 2110 "build/parser.tab.cpp"
    break;

  case 58: /* $@20: %empty  */
#line 634 "src/parser.y"
            { emit("SWP", 1); }
#line 2116 "build/parser.tab.cpp"
    break;

  case 59: /* expression: value $@20 MOD value  */
#line 634 "src/parser.y"
                                          { emit("SWP", 2); emit("CALL", addr_mod); }
#line 2122 "build/parser.tab.cpp"
    break;

  case 60: /* $@21: %empty  */
#line 637 "src/parser.y"
                  { emit("SWP", 1); }
#line 2128 "build/parser.tab.cpp"
    break;

  case 61: /* condition: value $@21 EQ value  */
#line 637 "src/parser.y"
                                               {
        emit("SWP", 2); 
        emit("RST", 0); emit("ADD", 1); emit("SUB", 2); emit("SWP", 3);
        emit("RST", 0); emit("ADD", 2); emit("SUB", 1); emit("ADD", 3);
        emit("JPOS", 0); 
        if_stack.push_back(code.size()-1); 
    }
#line 2140 "build/parser.tab.cpp"
    break;

  case 62: /* $@22: %empty  */
#line 644 "src/parser.y"
            { emit("SWP", 1); }
#line 2146 "build/parser.tab.cpp"
    break;

  case 63: /* condition: value $@22 NEQ value  */
#line 644 "src/parser.y"
                                          {
        emit("SWP", 2); 
        emit("RST", 0); emit("ADD", 1); emit("SUB", 2); emit("SWP", 3);
        emit("RST", 0); emit("ADD", 2); emit("SUB", 1); emit("ADD", 3);
        emit("JZERO", 0); 
        if_stack.push_back(code.size()-1); 
    }
#line 2158 "build/parser.tab.cpp"
    break;

  case 64: /* $@23: %empty  */
#line 651 "src/parser.y"
            { emit("SWP", 1); }
#line 2164 "build/parser.tab.cpp"
    break;

  case 65: /* condition: value $@23 GT value  */
#line 651 "src/parser.y"
                                         { 
        emit("SWP", 1); emit("SUB", 1); 
        emit("JZERO", 0); 
        if_stack.push_back(code.size()-1); 
    }
#line 2174 "build/parser.tab.cpp"
    break;

  case 66: /* $@24: %empty  */
#line 656 "src/parser.y"
            { emit("SWP", 1); }
#line 2180 "build/parser.tab.cpp"
    break;

  case 67: /* condition: value $@24 LT value  */
#line 656 "src/parser.y"
                                         { 
        emit("SUB", 1); 
        emit("JZERO", 0); 
        if_stack.push_back(code.size()-1); 
    }
#line 2190 "build/parser.tab.cpp"
    break;

  case 68: /* $@25: %empty  */
#line 661 "src/parser.y"
            { emit("SWP", 1); }
#line 2196 "build/parser.tab.cpp"
    break;

  case 69: /* condition: value $@25 GEQ value  */
#line 661 "src/parser.y"
                                          { 
        emit("SUB", 1);
        emit("JPOS", 0);
        if_stack.push_back(code.size()-1);
    }
#line 2206 "build/parser.tab.cpp"
    break;

  case 70: /* $@26: %empty  */
#line 666 "src/parser.y"
            { emit("SWP", 1); }
#line 2212 "build/parser.tab.cpp"
    break;

  case 71: /* condition: value $@26 LEQ value  */
#line 666 "src/parser.y"
                                          { 
        emit("SWP", 1); 
        emit("SUB", 1); 
        emit("JPOS", 0);
        if_stack.push_back(code.size()-1);
    }
#line 2223 "build/parser.tab.cpp"
    break;

  case 72: /* value: num  */
#line 674 "src/parser.y"
            { gen_const(0, (yyvsp[0].num)); }
#line 2229 "build/parser.tab.cpp"
    break;

  case 73: /* value: identifier  */
#line 675 "src/parser.y"
                 {
        if (!(yyvsp[0].loc)->sym->is_initialized) yyerror("Usage of uninitialized variable");
        if ((yyvsp[0].loc)->reg != -1) emit("RLOAD", (yyvsp[0].loc)->reg);
        else emit("LOAD", (yyvsp[0].loc)->address);
        delete (yyvsp[0].loc);
    }
#line 2240 "build/parser.tab.cpp"
    break;

  case 74: /* identifier: pidentifier  */
#line 683 "src/parser.y"
                         {
         Symbol* s = get_variable(string((yyvsp[0].str)));
         if (!s) yyerror(("Undefined identifier " + string((yyvsp[0].str))).c_str());
         (yyval.loc) = new Location();
         (yyval.loc)->address = s->address;
         (yyval.loc)->sym = s;
         if (s->is_param) {
             emit("LOAD", s->address);
             emit("SWP", 7); 
             (yyval.loc)->reg = 7;
             (yyval.loc)->address = -1;
         } else {
             (yyval.loc)->reg = -1;
         }
    }
#line 2260 "build/parser.tab.cpp"
    break;

  case 75: /* identifier: pidentifier LBRACKET num RBRACKET  */
#line 698 "src/parser.y"
                                        {
         Symbol* s = get_variable(string((yyvsp[-3].str)));
         if (!s) yyerror(("Undefined identifier " + string((yyvsp[-3].str))).c_str());
         (yyval.loc) = new Location();
         (yyval.loc)->sym = s;
         if (s->is_param) {
             emit("LOAD", s->address); 
             long long offset = (yyvsp[-1].num) - s->start;
             gen_const(1, offset);
             emit("ADD", 1);
             emit("SWP", 7);
             (yyval.loc)->reg = 7;
             (yyval.loc)->address = -1;
         } else {
            (yyval.loc)->address = s->address + (yyvsp[-1].num) - s->start;
            (yyval.loc)->reg = -1;
         }
    }
#line 2283 "build/parser.tab.cpp"
    break;

  case 76: /* identifier: pidentifier LBRACKET pidentifier RBRACKET  */
#line 716 "src/parser.y"
                                                {
         Symbol* arr = get_variable(string((yyvsp[-3].str)));
         Symbol* idx = get_variable(string((yyvsp[-1].str)));
         if (!arr) yyerror(("Undefined array " + string((yyvsp[-3].str))).c_str());
         if (!idx) yyerror(("Undefined variable " + string((yyvsp[-1].str))).c_str());
         if (!idx->is_initialized) yyerror("Usage of uninitialized variable for loop index");
         
         if (idx->is_param) {
             emit("LOAD", idx->address);
             emit("RLOAD", 0); 
         } else {
             emit("LOAD", idx->address);
         }
         
         if (arr->start > 0) { gen_const(1, arr->start); emit("SUB", 1); }
         
         if (arr->is_param) {
             emit("SWP", 1); 
             emit("LOAD", arr->address); 
             emit("ADD", 1); 
         } else {
             gen_const(1, arr->address); 
             emit("ADD", 1);
         }
         
         emit("SWP", 7);
         (yyval.loc) = new Location();
         (yyval.loc)->address = -1;
         (yyval.loc)->reg = 7;
         (yyval.loc)->sym = arr;
    }
#line 2319 "build/parser.tab.cpp"
    break;


#line 2323 "build/parser.tab.cpp"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 749 "src/parser.y"


void yyerror(const char *s) {
    cerr << "Error: " << s << " at line " << yylineno << endl;
    exit(1);
}

void optimize_code() {
    vector<bool> is_target(code.size() + 1, false);
    for (const auto& instr : code) {
        if (instr.has_arg && (instr.opcode == "JUMP" || instr.opcode == "JZERO" || instr.opcode == "JPOS" || instr.opcode == "CALL")) {
            if (instr.arg >= 0 && instr.arg < (long long)is_target.size())
                is_target[instr.arg] = true;
        }
    }

    vector<bool> to_delete(code.size(), false);
    for (size_t i = 0; i < code.size(); ++i) {
        if (to_delete[i]) continue;
        
        // STORE x, LOAD x -> Remove LOAD x
        if (code[i].opcode == "STORE" && i+1 < code.size()) {
             if (code[i+1].opcode == "LOAD" && 
                 code[i].arg == code[i+1].arg && 
                 !to_delete[i] && // Ensure STORE wasn't deleted (unlikely)
                 !is_target[i+1]) { 
                 to_delete[i+1] = true;
             }
        }
        
        // JUMP next
        if (code[i].opcode == "JUMP" && code[i].arg == (long long)(i + 1)) {
             to_delete[i] = true;
        }
    }
    
    vector<Instruction> new_code;
    vector<long long> map_idx(code.size() + 1);
    long long new_idx = 0;
    
    for (size_t i = 0; i < code.size(); ++i) {
        map_idx[i] = new_idx;
        if (!to_delete[i]) {
            new_code.push_back(code[i]);
            new_idx++;
        }
    }
    map_idx[code.size()] = new_idx; 
    
    for (auto& instr : new_code) {
        if (instr.has_arg && (instr.opcode == "JUMP" || instr.opcode == "JZERO" || instr.opcode == "JPOS" || instr.opcode == "CALL")) {
             instr.arg = map_idx[instr.arg];
        }
    }
    
    code = new_code;
}

int main() {
    if (yyparse() == 0) {
        optimize_code();
        for (const auto& instr : code) {
            cout << instr.opcode;
            if (instr.has_arg) cout << " " << instr.arg;
            cout << endl;
        }
    }
    return 0;
}
