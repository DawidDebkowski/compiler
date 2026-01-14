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

#include "types.hpp"
#include "codegen.hpp"
#include "symtable.hpp"
#include "math_kernel.hpp"
#include <algorithm>

using namespace std;

extern int yylineno;
int yylex();
void yyerror(const char *s);

// Global Analysis State
bool analyze_mode = false;
int loop_depth = 0;
map<string, long long> var_usage_counts;
map<string, int> var_register_assignments;
extern Symbol* reg_descriptors[8]; // Track which variable is in which register (0-7)

void reset_parser_state() {
    code.clear();
    symbol_table.clear();
    procedures_map.clear();
    memory_offset = 1000;
    current_procedure = "";
    calls_mul.clear(); calls_div.clear(); calls_mod.clear();
    loop_stack.clear(); if_stack.clear();
    loop_depth = 0;
    addr_mul = -1; addr_div = -1; addr_mod = -1;
    for(int i=0; i<8; i++) reg_descriptors[i] = nullptr;
}

void check_reg_assignment(char* name) {
    if (analyze_mode) return;
    string sname = (current_procedure == "") ? string(name) : current_procedure + "_" + string(name);
    if (var_register_assignments.count(sname)) {
         Symbol* s = get_variable(string(name));
         if (s) {
             s->cache_reg = var_register_assignments[sname];
             reg_descriptors[s->cache_reg] = s;
         }
    }
}

void spill_registers(int max_reg) {
    for(int i=4; i<=max_reg; i++) {
        if(reg_descriptors[i]) {
            emit("SWP", i);
            emit("STORE", reg_descriptors[i]->address);
            emit("SWP", i);
        }
    }
}

void restore_registers(int max_reg) {
    bool needed = false;
    for(int i=4; i<=max_reg; i++) if(reg_descriptors[i]) needed = true;
    if(!needed) return;
    
    long long temp = memory_offset++;
    emit("STORE", temp); 
    
    for(int i=4; i<=max_reg; i++) {
        if(reg_descriptors[i]) {
            emit("LOAD", reg_descriptors[i]->address); 
            emit("SWP", i); 
        }
    }
    
    emit("LOAD", temp); 
}


#line 155 "build/parser.tab.cpp"

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
  YYSYMBOL_69_15 = 69,                     /* $@15  */
  YYSYMBOL_70_16 = 70,                     /* $@16  */
  YYSYMBOL_proc_call = 71,                 /* proc_call  */
  YYSYMBOL_72_17 = 72,                     /* $@17  */
  YYSYMBOL_declarations = 73,              /* declarations  */
  YYSYMBOL_args_decl = 74,                 /* args_decl  */
  YYSYMBOL_type = 75,                      /* type  */
  YYSYMBOL_args = 76,                      /* args  */
  YYSYMBOL_argument = 77,                  /* argument  */
  YYSYMBOL_expression = 78,                /* expression  */
  YYSYMBOL_79_18 = 79,                     /* $@18  */
  YYSYMBOL_80_19 = 80,                     /* $@19  */
  YYSYMBOL_81_20 = 81,                     /* $@20  */
  YYSYMBOL_82_21 = 82,                     /* $@21  */
  YYSYMBOL_83_22 = 83,                     /* $@22  */
  YYSYMBOL_condition = 84,                 /* condition  */
  YYSYMBOL_85_23 = 85,                     /* $@23  */
  YYSYMBOL_86_24 = 86,                     /* $@24  */
  YYSYMBOL_87_25 = 87,                     /* $@25  */
  YYSYMBOL_88_26 = 88,                     /* $@26  */
  YYSYMBOL_89_27 = 89,                     /* $@27  */
  YYSYMBOL_90_28 = 90,                     /* $@28  */
  YYSYMBOL_value = 91,                     /* value  */
  YYSYMBOL_identifier = 92                 /* identifier  */
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
#define YYLAST   278

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  48
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  45
/* YYNRULES -- Number of rules.  */
#define YYNRULES  80
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  164

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
       0,   111,   111,   111,   134,   134,   149,   149,   162,   165,
     165,   178,   182,   178,   185,   189,   185,   194,   195,   198,
     198,   228,   228,   240,   245,   245,   257,   257,   268,   290,
     268,   342,   363,   342,   414,   415,   431,   436,   436,   446,
     450,   454,   458,   464,   469,   476,   477,   478,   479,   482,
     483,   486,   527,   551,   552,   552,   553,   553,   554,   554,
     597,   597,   624,   624,   674,   674,   681,   681,   688,   688,
     693,   693,   698,   698,   703,   703,   711,   718,   735,   764,
     782
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
  "command", "$@9", "$@10", "$@11", "$@12", "$@13", "$@14", "$@15", "$@16",
  "proc_call", "$@17", "declarations", "args_decl", "type", "args",
  "argument", "expression", "$@18", "$@19", "$@20", "$@21", "$@22",
  "condition", "$@23", "$@24", "$@25", "$@26", "$@27", "$@28", "value",
  "identifier", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-56)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-75)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -56,     5,   -56,   -56,     7,   -10,    38,   -56,     8,    57,
      41,   -56,    -1,    13,    60,    14,   -56,    24,    -2,     2,
     -56,   -56,   -56,   -56,   -19,    23,   233,    26,   -56,    28,
     -56,   233,    14,   -56,   -56,   -20,   -56,   -56,    29,    33,
     -20,   -30,     0,   -56,    39,   -56,    43,   233,    37,   233,
      42,    40,    45,   -56,    75,    85,   -56,   -20,   233,    68,
      58,    62,   -56,   -14,   -56,   -56,   -56,    71,    59,    54,
      72,   152,   -56,   -56,   233,    73,    98,    74,    99,   100,
     102,   117,   179,   -20,   -56,   -56,    10,    96,    97,   -20,
     101,   -56,   103,   -56,   206,   -20,   -20,   -20,   -20,   -20,
     -20,   233,   -20,   123,   -56,   -56,   -18,   -56,   -56,   -56,
     104,    88,   -56,   106,   -56,   -56,   -56,   -56,   -56,   -56,
     -56,   -56,   217,   107,   127,   128,    10,   -56,   -56,   116,
     125,   126,   129,   133,   118,   233,   -56,   -56,   -20,   -20,
     -56,   -20,   -20,   -20,   -20,   -20,   -56,   225,   -56,   -56,
     -56,   -56,   -56,   -56,   -56,   -56,   150,   153,   233,   233,
     160,   168,   -56,   -56
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       2,     0,     8,     1,     0,     0,     0,     3,     0,     0,
      11,     9,     0,     0,     0,    48,     6,    41,     0,     0,
      15,    45,    46,    47,     0,     0,     0,     0,     4,     0,
      12,     0,    48,    10,    44,     0,    24,    26,     0,     0,
       0,    78,     0,    18,     0,    19,     0,     0,    39,     0,
       0,     0,    78,    76,     0,    64,    77,     0,     0,     0,
       0,     0,    37,     0,     7,    17,    34,     0,     0,     0,
       0,     0,    16,    43,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    35,    36,     0,     0,     0,     0,
       0,     5,     0,    13,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    28,    51,    52,     0,    50,    80,    79,
       0,    53,    42,     0,    21,    23,    65,    67,    69,    71,
      73,    75,     0,     0,     0,     0,     0,    38,    20,     0,
       0,     0,     0,     0,     0,     0,    25,    27,     0,     0,
      49,     0,     0,     0,     0,     0,    40,     0,    29,    32,
      55,    57,    59,    61,    63,    22,     0,     0,     0,     0,
       0,     0,    30,    33
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,
     -56,   -56,   -56,   -31,   -41,   -56,   -56,   -56,   -56,   -56,
     -56,   -56,   -56,   -56,   -56,   158,   -56,   146,   -56,    53,
     -56,   -56,   -56,   -56,   -56,   -56,   -55,   -56,   -56,   -56,
     -56,   -56,   -56,    12,    -6
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     1,     2,     4,    47,    26,     9,    15,     7,    13,
      49,    14,    31,    42,    43,    67,   135,    57,    58,   124,
     156,   125,   157,    44,    86,    18,    24,    25,   106,   107,
     110,   129,   130,   131,   132,   133,    54,    75,    76,    77,
      78,    79,    80,    55,    45
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      50,    65,    81,    28,    16,     3,    64,    30,    35,    65,
       5,    62,    36,    63,     6,    37,    69,    38,    71,    32,
     126,    39,    40,    33,   127,    52,    53,    82,    65,    56,
      65,    87,    88,    60,    56,     8,    29,    21,    22,    23,
      29,    65,    10,    94,    17,    41,   -14,   123,    72,    11,
      35,    56,    61,    65,    36,   104,   105,    37,    17,    38,
      91,    12,    35,    39,    40,    20,    36,    27,    34,    37,
     122,    38,    46,    48,    59,    39,    40,    56,    52,    66,
      70,    65,    68,    56,    74,    73,    83,    41,    63,    56,
      56,    56,    56,    56,    56,   103,    56,    89,    84,    41,
      95,   111,    85,    97,   147,    90,    65,   116,   117,   118,
     119,   120,   121,   -66,   -68,   -70,   -72,   -74,    92,    65,
      65,   -54,   -56,   -58,   -60,   -62,    96,   160,   161,    98,
     101,    99,    56,    56,   100,    56,    56,    56,    56,    56,
     108,   109,   113,   -31,   128,   112,   138,   137,   139,   141,
     148,   149,   134,   150,   151,   152,   153,   154,    93,   142,
      35,   143,   146,   158,    36,   144,   159,    37,    35,    38,
     145,    19,    36,    39,    40,    37,    35,    38,    51,   140,
      36,    39,    40,    37,     0,    38,     0,    35,     0,    39,
      40,    36,     0,     0,    37,   102,    38,    41,     0,     0,
      39,    40,     0,     0,     0,    41,     0,   162,     0,     0,
       0,     0,     0,    41,    35,   163,   114,   115,    36,     0,
       0,    37,     0,    38,    41,    35,     0,    39,    40,    36,
       0,   136,    37,    35,    38,     0,   155,    36,    39,    40,
      37,    35,    38,     0,     0,    36,    39,    40,    37,     0,
      38,    41,     0,     0,    39,    40,     0,     0,     0,     0,
       0,     0,    41,     0,     0,     0,     0,     0,     0,     0,
      41,     0,     0,     0,     0,     0,     0,     0,    41
};

static const yytype_int16 yycheck[] =
{
      31,    42,    57,     5,     5,     0,     6,     5,     8,    50,
       3,    41,    12,    43,     7,    15,    47,    17,    49,    38,
      38,    21,    22,    42,    42,    45,    46,    58,    69,    35,
      71,    45,    46,    39,    40,    45,    38,    23,    24,    25,
      38,    82,     4,    74,    45,    45,     5,   102,     6,    41,
       8,    57,    40,    94,    12,    45,    46,    15,    45,    17,
       6,     4,     8,    21,    22,     5,    12,    43,    45,    15,
     101,    17,    46,    45,    45,    21,    22,    83,    45,    40,
      43,   122,    39,    89,     9,    45,    18,    45,    43,    95,
      96,    97,    98,    99,   100,    83,   102,    26,    40,    45,
      27,    89,    40,    29,   135,    46,   147,    95,    96,    97,
      98,    99,   100,    28,    29,    30,    31,    32,    46,   160,
     161,    33,    34,    35,    36,    37,    28,   158,   159,    30,
      13,    31,   138,   139,    32,   141,   142,   143,   144,   145,
      44,    44,    39,    20,    40,    44,    19,    40,    20,    33,
     138,   139,    46,   141,   142,   143,   144,   145,     6,    34,
       8,    35,    44,    13,    12,    36,    13,    15,     8,    17,
      37,    13,    12,    21,    22,    15,     8,    17,    32,   126,
      12,    21,    22,    15,    -1,    17,    -1,     8,    -1,    21,
      22,    12,    -1,    -1,    15,    16,    17,    45,    -1,    -1,
      21,    22,    -1,    -1,    -1,    45,    -1,    47,    -1,    -1,
      -1,    -1,    -1,    45,     8,    47,    10,    11,    12,    -1,
      -1,    15,    -1,    17,    45,     8,    -1,    21,    22,    12,
      -1,    14,    15,     8,    17,    -1,    11,    12,    21,    22,
      15,     8,    17,    -1,    -1,    12,    21,    22,    15,    -1,
      17,    45,    -1,    -1,    21,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    45,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      45,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    45
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    49,    50,     0,    51,     3,     7,    56,    45,    54,
       4,    41,     4,    57,    59,    55,     5,    45,    73,    73,
       5,    23,    24,    25,    74,    75,    53,    43,     5,    38,
       5,    60,    38,    42,    45,     8,    12,    15,    17,    21,
      22,    45,    61,    62,    71,    92,    46,    52,    45,    58,
      61,    75,    45,    46,    84,    91,    92,    65,    66,    45,
      92,    91,    41,    43,     6,    62,    40,    63,    39,    61,
      43,    61,     6,    45,     9,    85,    86,    87,    88,    89,
      90,    84,    61,    18,    40,    40,    72,    45,    46,    26,
      46,     6,    46,     6,    61,    27,    28,    29,    30,    31,
      32,    13,    16,    91,    45,    46,    76,    77,    44,    44,
      78,    91,    44,    39,    10,    11,    91,    91,    91,    91,
      91,    91,    61,    84,    67,    69,    38,    42,    40,    79,
      80,    81,    82,    83,    46,    64,    14,    40,    19,    20,
      77,    33,    34,    35,    36,    37,    44,    61,    91,    91,
      91,    91,    91,    91,    91,    11,    68,    70,    13,    13,
      61,    61,    47,    47
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    48,    50,    49,    52,    51,    53,    51,    51,    55,
      54,    57,    58,    56,    59,    60,    56,    61,    61,    63,
      62,    64,    62,    62,    65,    62,    66,    62,    67,    68,
      62,    69,    70,    62,    62,    62,    62,    72,    71,    73,
      73,    73,    73,    74,    74,    75,    75,    75,    75,    76,
      76,    77,    77,    78,    79,    78,    80,    78,    81,    78,
      82,    78,    83,    78,    85,    84,    86,    84,    87,    84,
      88,    84,    89,    84,    90,    84,    91,    91,    92,    92,
      92
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     3,     0,     9,     0,     8,     0,     0,
       5,     0,     0,     8,     0,     0,     7,     2,     1,     0,
       5,     0,     8,     5,     0,     6,     0,     6,     0,     0,
      11,     0,     0,    11,     2,     3,     3,     0,     5,     3,
       8,     1,     6,     4,     2,     1,     1,     1,     0,     3,
       1,     1,     1,     1,     0,     4,     0,     4,     0,     4,
       0,     4,     0,     4,     0,     4,     0,     4,     0,     4,
       0,     4,     0,     4,     0,     4,     1,     1,     1,     4,
       4
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
#line 111 "src/parser.y"
              { 
     emit("JUMP", 0); // JUMP to Main
   }
#line 1372 "build/parser.tab.cpp"
    break;

  case 3: /* program_all: $@1 procedures main  */
#line 113 "src/parser.y"
                     {
      if (!analyze_mode) {
          code[0].arg = symbol_table["main_start"].address; 
          emit("HALT");

          if (!calls_mul.empty()) {
              generate_mul();
              for(long long idx : calls_mul) code[idx].arg = addr_mul;
          }
          if (!calls_div.empty() || !calls_mod.empty()) {
              if (addr_div == -1) generate_div();
              for(long long idx : calls_div) code[idx].arg = addr_div;
          }
          if (!calls_mod.empty()) {
              generate_mod();
              for(long long idx : calls_mod) code[idx].arg = addr_mod;
          }
      }
   }
#line 1396 "build/parser.tab.cpp"
    break;

  case 4: /* $@2: %empty  */
#line 134 "src/parser.y"
                                                               {
        procedures_map[current_procedure].address = code.size();
        
        // Save Return Address
        long long ra_addr = memory_offset++;
        procedures_map[current_procedure].ra_address = ra_addr;
        emit("STORE", ra_addr);
        
    }
#line 1410 "build/parser.tab.cpp"
    break;

  case 5: /* procedures: procedures PROCEDURE proc_head IS declarations IN $@2 commands END  */
#line 142 "src/parser.y"
                   {
        // Restore Return Address
        long long ra_addr = procedures_map[current_procedure].ra_address;
        emit("LOAD", ra_addr);
        emit("RTRN");
        current_procedure = "";
    }
#line 1422 "build/parser.tab.cpp"
    break;

  case 6: /* $@3: %empty  */
#line 149 "src/parser.y"
                                           {
        procedures_map[current_procedure].address = code.size();
        
        long long ra_addr = memory_offset++;
        procedures_map[current_procedure].ra_address = ra_addr;
        emit("STORE", ra_addr);
        
    }
#line 1435 "build/parser.tab.cpp"
    break;

  case 7: /* procedures: procedures PROCEDURE proc_head IS IN $@3 commands END  */
#line 156 "src/parser.y"
                   {
        long long ra_addr = procedures_map[current_procedure].ra_address;
        emit("LOAD", ra_addr);
        emit("RTRN");
        current_procedure = "";
    }
#line 1446 "build/parser.tab.cpp"
    break;

  case 9: /* $@4: %empty  */
#line 165 "src/parser.y"
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
#line 1461 "build/parser.tab.cpp"
    break;

  case 10: /* proc_head: pidentifier LPAREN $@4 args_decl RPAREN  */
#line 174 "src/parser.y"
                       {
    }
#line 1468 "build/parser.tab.cpp"
    break;

  case 11: /* $@5: %empty  */
#line 178 "src/parser.y"
                  { 
         current_procedure = "main"; 
         Symbol s; s.address = code.size();
         symbol_table["main_start"] = s; 
       }
#line 1478 "build/parser.tab.cpp"
    break;

  case 12: /* $@6: %empty  */
#line 182 "src/parser.y"
                         {
         symbol_table["main_start"].address = code.size();
       }
#line 1486 "build/parser.tab.cpp"
    break;

  case 14: /* $@7: %empty  */
#line 185 "src/parser.y"
                 { 
         current_procedure = "main"; 
         Symbol s; s.address = code.size(); 
         symbol_table["main_start"] = s;
       }
#line 1496 "build/parser.tab.cpp"
    break;

  case 15: /* $@8: %empty  */
#line 189 "src/parser.y"
            {
         symbol_table["main_start"].address = code.size();
       }
#line 1504 "build/parser.tab.cpp"
    break;

  case 19: /* $@9: %empty  */
#line 198 "src/parser.y"
                     {
        // Save LHS address if it is in a register (because RHS calculation might clobber it)
        if ((yyvsp[0].loc)->reg != -1) {
             emit("SWP", (yyvsp[0].loc)->reg); // Move Addr to r[0]
             emit("STORE", lhs_hold_addr);
             emit("SWP", (yyvsp[0].loc)->reg); // Move back (optional but safest state)
        }
    }
#line 1517 "build/parser.tab.cpp"
    break;

  case 20: /* command: identifier $@9 ASSIGN expression SEMICOLON  */
#line 205 "src/parser.y"
                                  {
        if ((yyvsp[-4].loc)->sym->is_iterator) yyerror("Cannot modify loop iterator");
        if ((yyvsp[-4].loc)->sym->mod == "I") yyerror("Cannot modify initialized parameter (I)");
        
        (yyvsp[-4].loc)->sym->is_initialized = true;
        
        if ((yyvsp[-4].loc)->reg != -1) {
             // Restore Address from lhs_hold_addr
             emit("SWP", 1); // Save Expression Result to rb
             emit("LOAD", lhs_hold_addr); // Load Addr to ra
             emit("SWP", (yyvsp[-4].loc)->reg); // Move Addr to reg (e.g. r[7])
             emit("SWP", 1); // Restore Expression Result to ra
             emit("RSTORE", (yyvsp[-4].loc)->reg);
        }
        else {
             if ((yyvsp[-4].loc)->sym->cache_reg != -1) {
                 emit("SWP", (yyvsp[-4].loc)->sym->cache_reg); 
             } else {
                 emit("STORE", (yyvsp[-4].loc)->address);
             }
        }
        delete (yyvsp[-4].loc);
    }
#line 1545 "build/parser.tab.cpp"
    break;

  case 21: /* $@10: %empty  */
#line 228 "src/parser.y"
                                      {
        long long jump_over_else = code.size();
        emit("JUMP", 0); 
        long long false_jump_idx = if_stack.back();
        if_stack.pop_back();
        if (!analyze_mode) code[false_jump_idx].arg = code.size();
        if_stack.push_back(jump_over_else);
    }
#line 1558 "build/parser.tab.cpp"
    break;

  case 22: /* command: IF condition THEN commands ELSE $@10 commands ENDIF  */
#line 235 "src/parser.y"
                     {
        long long jump_over_else = if_stack.back();
        if_stack.pop_back();
        if (!analyze_mode) code[jump_over_else].arg = code.size();
    }
#line 1568 "build/parser.tab.cpp"
    break;

  case 23: /* command: IF condition THEN commands ENDIF  */
#line 240 "src/parser.y"
                                       {
        long long false_jump_idx = if_stack.back();
        if_stack.pop_back();
        if (!analyze_mode) code[false_jump_idx].arg = code.size();
    }
#line 1578 "build/parser.tab.cpp"
    break;

  case 24: /* $@11: %empty  */
#line 245 "src/parser.y"
            {
        loop_stack.push_back(code.size());
        loop_depth++;
    }
#line 1587 "build/parser.tab.cpp"
    break;

  case 25: /* command: WHILE $@11 condition DO commands ENDWHILE  */
#line 248 "src/parser.y"
                                     {
         loop_depth--;
         long long jump_out = if_stack.back();
         if_stack.pop_back();
         long long start = loop_stack.back();
         loop_stack.pop_back();
         emit("JUMP", start);
         if (!analyze_mode) code[jump_out].arg = code.size();
    }
#line 1601 "build/parser.tab.cpp"
    break;

  case 26: /* $@12: %empty  */
#line 257 "src/parser.y"
             {
        loop_depth++;
        loop_stack.push_back(code.size());
    }
#line 1610 "build/parser.tab.cpp"
    break;

  case 27: /* command: REPEAT $@12 commands UNTIL condition SEMICOLON  */
#line 260 "src/parser.y"
                                         {
         loop_depth--;
         long long jump = if_stack.back(); 
         if_stack.pop_back();
         long long start = loop_stack.back();
         loop_stack.pop_back();
         if (!analyze_mode) code[jump].arg = start;
    }
#line 1623 "build/parser.tab.cpp"
    break;

  case 28: /* $@13: %empty  */
#line 268 "src/parser.y"
                                 {
          Symbol* iter = get_variable(string((yyvsp[-2].str)));
          if (!iter) {
              add_symbol(string((yyvsp[-2].str)), false, false, "", 0, 0);
              iter = get_variable(string((yyvsp[-2].str)));
          }
          if (iter->is_iterator) yyerror("Nested loop with same iterator");
          if (iter->is_param) yyerror("Iterator must be local variable");
          
          iter->is_iterator = true;
          iter->is_initialized = true;
          
          string scoped_name = (current_procedure == "") ? string((yyvsp[-2].str)) : current_procedure + "_" + string((yyvsp[-2].str));
          if (analyze_mode) {
               var_usage_counts[scoped_name] += (pow(10, loop_depth) * 10);
          } else {
               if (var_register_assignments.count(scoped_name)) 
                   iter->cache_reg = var_register_assignments[scoped_name];
          }

          if (iter->cache_reg != -1) emit("SWP", iter->cache_reg);
          else emit("STORE", iter->address);
    }
#line 1651 "build/parser.tab.cpp"
    break;

  case 29: /* $@14: %empty  */
#line 290 "src/parser.y"
               {
          emit("SWP", 1); 
          Symbol* iter = get_variable(string((yyvsp[-5].str)));
          
          if (iter->cache_reg != -1) {
              emit("RST", 0); emit("ADD", 1);
              emit("SUB", iter->cache_reg);
              emit("INC", 0);
          } else {
              emit("LOAD", iter->address);
              emit("SWP", 1); 
              emit("SUB", 1); 
              emit("INC", 0); 
          }
          
          long long count_addr = memory_offset++;
          emit("STORE", count_addr);
          
          long long start_loop = code.size();
          emit("LOAD", count_addr);
          emit("JZERO", 0);
          long long jump_out = code.size()-1;
          
          loop_stack.push_back(start_loop);
          loop_stack.push_back(jump_out);
          loop_stack.push_back(count_addr);
          loop_depth++;
    }
#line 1684 "build/parser.tab.cpp"
    break;

  case 30: /* command: FOR pidentifier FROM value $@13 TO value $@14 DO commands ENDFOR  */
#line 317 "src/parser.y"
                         {
          loop_depth--;
          long long count_addr = loop_stack.back(); loop_stack.pop_back();
          long long jump_out = loop_stack.back(); loop_stack.pop_back();
          long long start_loop = loop_stack.back(); loop_stack.pop_back();
          
          emit("LOAD", count_addr);
          emit("DEC", 0);
          emit("STORE", count_addr);
          // FOR TO END
          
          Symbol* iter = get_variable(string((yyvsp[-9].str)));
          if (iter->cache_reg != -1) {
              emit("INC", iter->cache_reg);
          } else {
              emit("LOAD", iter->address);
              emit("INC", 0);
              emit("STORE", iter->address);
          }
          
          emit("JUMP", start_loop);
          if (!analyze_mode) code[jump_out].arg = code.size();
          
          iter->is_iterator = false;
    }
#line 1714 "build/parser.tab.cpp"
    break;

  case 31: /* $@15: %empty  */
#line 342 "src/parser.y"
                                 {
          Symbol* iter = get_variable(string((yyvsp[-2].str)));
          if (!iter) {
              add_symbol(string((yyvsp[-2].str)), false, false, "", 0, 0);
              iter = get_variable(string((yyvsp[-2].str)));
          }
          if (iter->is_iterator) yyerror("Nested loop with same iterator");
          if (iter->is_param) yyerror("Iterator must be local variable");
          iter->is_iterator = true;
          iter->is_initialized = true;

          string scoped_name = (current_procedure == "") ? string((yyvsp[-2].str)) : current_procedure + "_" + string((yyvsp[-2].str));
          if (analyze_mode) {
               var_usage_counts[scoped_name] += (pow(10, loop_depth) * 10);
          } else {
               if (var_register_assignments.count(scoped_name)) 
                   iter->cache_reg = var_register_assignments[scoped_name];
          }

          if (iter->cache_reg != -1) emit("SWP", iter->cache_reg);
          else emit("STORE", iter->address);
    }
#line 1741 "build/parser.tab.cpp"
    break;

  case 32: /* $@16: %empty  */
#line 363 "src/parser.y"
                   {
          emit("SWP", 1); 
          Symbol* iter = get_variable(string((yyvsp[-5].str)));
          
          if (iter->cache_reg != -1) {
              emit("RST", 0); emit("ADD", iter->cache_reg);
              emit("SUB", 1);
              emit("INC", 0);
          } else {
              emit("LOAD", iter->address);
              emit("SUB", 1); 
              emit("INC", 0); 
          }
          
          long long count_addr = memory_offset++;
          emit("STORE", count_addr);
          
          long long start_loop = code.size();
          emit("LOAD", count_addr);
          emit("JZERO", 0);
          long long jump_out = code.size()-1;
          
          loop_stack.push_back(start_loop);
          loop_stack.push_back(jump_out);
          loop_stack.push_back(count_addr);
          loop_depth++;
    }
#line 1773 "build/parser.tab.cpp"
    break;

  case 33: /* command: FOR pidentifier FROM value $@15 DOWNTO value $@16 DO commands ENDFOR  */
#line 389 "src/parser.y"
                         {
          loop_depth--;
          long long count_addr = loop_stack.back(); loop_stack.pop_back();
          long long jump_out = loop_stack.back(); loop_stack.pop_back();
          long long start_loop = loop_stack.back(); loop_stack.pop_back();
          
          emit("LOAD", count_addr);
          emit("DEC", 0);
          emit("STORE", count_addr);
          // FOR DOWNTO END
          
          Symbol* iter = get_variable(string((yyvsp[-9].str)));
          if (iter->cache_reg != -1) {
              emit("DEC", iter->cache_reg);
          } else {
              emit("LOAD", iter->address);
              emit("DEC", 0);
              emit("STORE", iter->address);
          }
          
          emit("JUMP", start_loop);
          if (!analyze_mode) code[jump_out].arg = code.size();
          
          iter->is_iterator = false;
    }
#line 1803 "build/parser.tab.cpp"
    break;

  case 35: /* command: READ identifier SEMICOLON  */
#line 415 "src/parser.y"
                                {
        if ((yyvsp[-1].loc)->sym->is_iterator) yyerror("Cannot read into loop iterator");
        if ((yyvsp[-1].loc)->sym->mod == "I") yyerror("Cannot read into I parameter");
        (yyvsp[-1].loc)->sym->is_initialized = true;

        emit("READ");
        
        if ((yyvsp[-1].loc)->sym->cache_reg != -1) {
             emit("SWP", (yyvsp[-1].loc)->sym->cache_reg);
        } else if ((yyvsp[-1].loc)->reg != -1) {
             emit("RSTORE", (yyvsp[-1].loc)->reg);
        } else {
             emit("STORE", (yyvsp[-1].loc)->address);
        }
        delete (yyvsp[-1].loc);
    }
#line 1824 "build/parser.tab.cpp"
    break;

  case 36: /* command: WRITE value SEMICOLON  */
#line 431 "src/parser.y"
                            {
        emit("WRITE");
    }
#line 1832 "build/parser.tab.cpp"
    break;

  case 37: /* $@17: %empty  */
#line 436 "src/parser.y"
                               { 
        current_call_proc = string((yyvsp[-1].str));
        current_arg_idx = 0;
        spill_registers(7);
    }
#line 1842 "build/parser.tab.cpp"
    break;

  case 38: /* proc_call: pidentifier LPAREN $@17 args RPAREN  */
#line 440 "src/parser.y"
                  {
        emit("CALL", procedures_map[(yyvsp[-4].str)].address);
        restore_registers(7);
    }
#line 1851 "build/parser.tab.cpp"
    break;

  case 39: /* declarations: declarations COMMA pidentifier  */
#line 446 "src/parser.y"
                                              {
        add_symbol(string((yyvsp[0].str)), false, false, "", 0, 0);
        check_reg_assignment((yyvsp[0].str));
    }
#line 1860 "build/parser.tab.cpp"
    break;

  case 40: /* declarations: declarations COMMA pidentifier LBRACKET num COLON num RBRACKET  */
#line 450 "src/parser.y"
                                                                     {
        add_symbol(string((yyvsp[-5].str)), true, false, "", (yyvsp[-3].num), (yyvsp[-1].num));
        check_reg_assignment((yyvsp[-5].str));
    }
#line 1869 "build/parser.tab.cpp"
    break;

  case 41: /* declarations: pidentifier  */
#line 454 "src/parser.y"
                  {
        add_symbol(string((yyvsp[0].str)), false, false, "", 0, 0);
        check_reg_assignment((yyvsp[0].str));
    }
#line 1878 "build/parser.tab.cpp"
    break;

  case 42: /* declarations: pidentifier LBRACKET num COLON num RBRACKET  */
#line 458 "src/parser.y"
                                                  {
        add_symbol(string((yyvsp[-5].str)), true, false, "", (yyvsp[-3].num), (yyvsp[-1].num));
        check_reg_assignment((yyvsp[-5].str));
    }
#line 1887 "build/parser.tab.cpp"
    break;

  case 43: /* args_decl: args_decl COMMA type pidentifier  */
#line 464 "src/parser.y"
                                             {
       string m = string((yyvsp[-1].str));
       bool is_arr = (m == "T");
       add_symbol(string((yyvsp[0].str)), is_arr, true, m, 0, 0); free((yyvsp[-1].str));
    }
#line 1897 "build/parser.tab.cpp"
    break;

  case 44: /* args_decl: type pidentifier  */
#line 469 "src/parser.y"
                       {
       string m = string((yyvsp[-1].str));
       bool is_arr = (m == "T");
       add_symbol(string((yyvsp[0].str)), is_arr, true, m, 0, 0); free((yyvsp[-1].str));
    }
#line 1907 "build/parser.tab.cpp"
    break;

  case 45: /* type: T  */
#line 476 "src/parser.y"
         { (yyval.str) = strdup("T"); }
#line 1913 "build/parser.tab.cpp"
    break;

  case 46: /* type: I  */
#line 477 "src/parser.y"
         { (yyval.str) = strdup("I"); }
#line 1919 "build/parser.tab.cpp"
    break;

  case 47: /* type: O  */
#line 478 "src/parser.y"
         { (yyval.str) = strdup("O"); }
#line 1925 "build/parser.tab.cpp"
    break;

  case 48: /* type: %empty  */
#line 479 "src/parser.y"
                   { (yyval.str) = strdup(""); }
#line 1931 "build/parser.tab.cpp"
    break;

  case 51: /* argument: pidentifier  */
#line 486 "src/parser.y"
                       {
        Symbol* s = get_variable(string((yyvsp[0].str)));
        if (!s) yyerror(("Undefined variable " + string((yyvsp[0].str))).c_str());
        
        // Usage Analysis
        string scoped_name = (current_procedure == "") ? string((yyvsp[0].str)) : current_procedure + "_" + string((yyvsp[0].str));
        if (analyze_mode) {
             long long weight = 1;
             for(int k=0; k<loop_depth; k++) weight *= 10;
             var_usage_counts[scoped_name] += weight; // Params getting passed have cost
        }
        
        ProcedureInfo& info = procedures_map[current_call_proc];
        if (current_arg_idx >= (int)info.param_addresses.size()) yyerror("Too many arguments for procedure call");
        
        long long param_addr = info.param_addresses[current_arg_idx];
        bool p_array = info.param_is_array[current_arg_idx];
        
        if (p_array && !s->is_array) yyerror(("Expected array for parameter " + to_string(current_arg_idx+1)).c_str());
        if (!p_array && s->is_array) yyerror(("Expected scalar for parameter " + to_string(current_arg_idx+1)).c_str());

        current_arg_idx++;

        if (s->is_param) {
            emit("LOAD", s->address); 
            emit("STORE", param_addr); 
        } else {
            if (s->cache_reg != -1) {
                 emit("SWP", s->cache_reg); 
                 emit("STORE", s->address);
                 emit("SWP", s->cache_reg);
            }
            if (s->is_array) {
                gen_const(0, s->address); 
                emit("STORE", param_addr);
            } else {
                gen_const(0, s->address);
                emit("STORE", param_addr);
            }
        }
    }
#line 1977 "build/parser.tab.cpp"
    break;

  case 52: /* argument: num  */
#line 527 "src/parser.y"
          {
        ProcedureInfo& info = procedures_map[current_call_proc];
        if (current_arg_idx >= (int)info.param_addresses.size()) yyerror("Too many arguments for procedure call");
        
        long long param_addr = info.param_addresses[current_arg_idx];
        bool p_array = info.param_is_array[current_arg_idx];
        
        if (p_array) yyerror(("Expected array for parameter " + to_string(current_arg_idx+1) + " but got number").c_str());
        
        current_arg_idx++;

        // Store constant in temporary memory
        long long temp_addr = memory_offset++;
        gen_const(0, (yyvsp[0].num));
        emit("STORE", temp_addr);
        
        // Pass address of temp
        gen_const(0, temp_addr);
        emit("STORE", param_addr);
    }
#line 2002 "build/parser.tab.cpp"
    break;

  case 54: /* $@18: %empty  */
#line 552 "src/parser.y"
            { emit("SWP", 1); }
#line 2008 "build/parser.tab.cpp"
    break;

  case 55: /* expression: value $@18 PLUS value  */
#line 552 "src/parser.y"
                                           { emit("ADD", 1); }
#line 2014 "build/parser.tab.cpp"
    break;

  case 56: /* $@19: %empty  */
#line 553 "src/parser.y"
            { emit("SWP", 1); }
#line 2020 "build/parser.tab.cpp"
    break;

  case 57: /* expression: value $@19 MINUS value  */
#line 553 "src/parser.y"
                                            { emit("SWP", 1); emit("SUB", 1); }
#line 2026 "build/parser.tab.cpp"
    break;

  case 58: /* $@20: %empty  */
#line 554 "src/parser.y"
            { emit("SWP", 1); }
#line 2032 "build/parser.tab.cpp"
    break;

  case 59: /* expression: value $@20 MULT value  */
#line 554 "src/parser.y"
                                           { 
        bool optimized = false;
        // Case 1: Right operand is constant power of 2 (Var * Const)
        if ((yyvsp[0].operand).is_const && (yyvsp[0].operand).val > 0) {
            long long v = (yyvsp[0].operand).val;
            if ((v & (v - 1)) == 0) { // Power of 2
                // Remove constant load
                for(int k=0; k<(yyvsp[0].operand).instructions_count; k++) code.pop_back();
                
                // r1 contains first operand. r0 is now free/unknown.
                // Apply shifts to r1
                int shifts = 0;
                while (v > 1) { v >>= 1; shifts++; }
                for(int k=0; k<shifts; k++) emit("SHL", 1);
                
                // Move result to r0 (expected output register)
                emit("SWP", 1);
                
                optimized = true;
            }
        }
        
        // Case 2: Left operand is constant power of 2 (Const * Var)
        if (!optimized && (yyvsp[-3].operand).is_const && (yyvsp[-3].operand).val > 0) {
             long long v = (yyvsp[-3].operand).val;
             if ((v & (v - 1)) == 0) {
                 // The Var is in r0 (a). The Const is in r1 (b) (put there by SWP 1).
                 // We want Var * Const.
                 // Just shift r0.
                 int shifts = 0;
                 while (v > 1) { v >>= 1; shifts++; }
                 for(int k=0; k<shifts; k++) emit("SHL", 0);
                 
                 optimized = true;
             }
        }
        
        if (!optimized) {
            spill_registers(5);
            emit("SWP", 2); emit("CALL", 0); calls_mul.push_back(code.size()-1); emit("SWP", 1); 
            restore_registers(5);
        }
    }
#line 2080 "build/parser.tab.cpp"
    break;

  case 60: /* $@21: %empty  */
#line 597 "src/parser.y"
            { emit("SWP", 1); }
#line 2086 "build/parser.tab.cpp"
    break;

  case 61: /* expression: value $@21 DIV value  */
#line 597 "src/parser.y"
                                          { 
        bool optimized = false;
        if ((yyvsp[0].operand).is_const && (yyvsp[0].operand).val > 0) {
            long long v = (yyvsp[0].operand).val;
            if ((v & (v - 1)) == 0) { // Power of 2
                // Remove constant load
                for(int k=0; k<(yyvsp[0].operand).instructions_count; k++) code.pop_back();
                
                // r1 contains first operand. r0 is now free/unknown.
                // Apply shifts to r1
                int shifts = 0;
                while (v > 1) { v >>= 1; shifts++; }
                for(int k=0; k<shifts; k++) emit("SHR", 1);
                
                // Move result to r0
                emit("SWP", 1);
                
                optimized = true;
            }
        }
        
        if (!optimized) {
            spill_registers(6);
            emit("SWP", 2); emit("CALL", 0); calls_div.push_back(code.size()-1); emit("SWP", 1); 
            restore_registers(6);
        }
    }
#line 2118 "build/parser.tab.cpp"
    break;

  case 62: /* $@22: %empty  */
#line 624 "src/parser.y"
            { emit("SWP", 1); }
#line 2124 "build/parser.tab.cpp"
    break;

  case 63: /* expression: value $@22 MOD value  */
#line 624 "src/parser.y"
                                          { 
        bool optimized = false;
        if ((yyvsp[0].operand).is_const && (yyvsp[0].operand).val > 0) {
            long long v = (yyvsp[0].operand).val;
            if ((v & (v - 1)) == 0) { // Power of 2
                // Remove constant load
                for(int k=0; k<(yyvsp[0].operand).instructions_count; k++) code.pop_back();
                
                // x % 2^k  ==  x - ((x >> k) << k)
                // r1 has x.
                
                int shifts = 0;
                while (v > 1) { v >>= 1; shifts++; }
                
                // Save x to proper register to preserve it?
                // Logic:
                // r1 = x
                // r2 = x (copy) -- Need instruction COPY or logic
                // No COPY in VM? 
                // We have r0..r7.
                // SWP 2 (r2 <-> r0). r0 is trash.
                // r1 has x. emit("SWP", 2); -> r1 trash, r2 = x ?? No SWP x swaps r0 and rx.
                // SWP x: r0 <-> rx.
                
                emit("SWP", 1); // r0 = x, r1 = trash
                emit("SWP", 2); // r2 = x, r0 = trash/r2_old
                emit("RST", 0); emit("ADD", 2); // r0 = x
                
                // Calculate (x >> k) << k
                 for(int k=0; k<shifts; k++) emit("SHR", 0);
                 for(int k=0; k<shifts; k++) emit("SHL", 0);
                 
                 // Subtract from original x (in r2)
                 // r0 = (x>>k)<<k
                 // r2 = x
                 emit("SWP", 2); // r0 = x, r2 = (x>>k)<<k
                 emit("SUB", 2); // r0 = x - ...
                 
                 optimized = true;
            }
        }
    
        if (!optimized) {
            spill_registers(6);
            emit("SWP", 2); emit("CALL", 0); calls_mod.push_back(code.size()-1); emit("SWP", 1); 
            restore_registers(6);
        }
    }
#line 2177 "build/parser.tab.cpp"
    break;

  case 64: /* $@23: %empty  */
#line 674 "src/parser.y"
                  { emit("SWP", 1); }
#line 2183 "build/parser.tab.cpp"
    break;

  case 65: /* condition: value $@23 EQ value  */
#line 674 "src/parser.y"
                                               {
        emit("SWP", 2); 
        emit("RST", 0); emit("ADD", 1); emit("SUB", 2); emit("SWP", 3);
        emit("RST", 0); emit("ADD", 2); emit("SUB", 1); emit("ADD", 3);
        emit("JPOS", 0); 
        if_stack.push_back(code.size()-1); 
    }
#line 2195 "build/parser.tab.cpp"
    break;

  case 66: /* $@24: %empty  */
#line 681 "src/parser.y"
            { emit("SWP", 1); }
#line 2201 "build/parser.tab.cpp"
    break;

  case 67: /* condition: value $@24 NEQ value  */
#line 681 "src/parser.y"
                                          {
        emit("SWP", 2); 
        emit("RST", 0); emit("ADD", 1); emit("SUB", 2); emit("SWP", 3);
        emit("RST", 0); emit("ADD", 2); emit("SUB", 1); emit("ADD", 3);
        emit("JZERO", 0); 
        if_stack.push_back(code.size()-1); 
    }
#line 2213 "build/parser.tab.cpp"
    break;

  case 68: /* $@25: %empty  */
#line 688 "src/parser.y"
            { emit("SWP", 1); }
#line 2219 "build/parser.tab.cpp"
    break;

  case 69: /* condition: value $@25 GT value  */
#line 688 "src/parser.y"
                                         { 
        emit("SWP", 1); emit("SUB", 1); 
        emit("JZERO", 0); 
        if_stack.push_back(code.size()-1); 
    }
#line 2229 "build/parser.tab.cpp"
    break;

  case 70: /* $@26: %empty  */
#line 693 "src/parser.y"
            { emit("SWP", 1); }
#line 2235 "build/parser.tab.cpp"
    break;

  case 71: /* condition: value $@26 LT value  */
#line 693 "src/parser.y"
                                         { 
        emit("SUB", 1); 
        emit("JZERO", 0); 
        if_stack.push_back(code.size()-1); 
    }
#line 2245 "build/parser.tab.cpp"
    break;

  case 72: /* $@27: %empty  */
#line 698 "src/parser.y"
            { emit("SWP", 1); }
#line 2251 "build/parser.tab.cpp"
    break;

  case 73: /* condition: value $@27 GEQ value  */
#line 698 "src/parser.y"
                                          { 
        emit("SUB", 1);
        emit("JPOS", 0);
        if_stack.push_back(code.size()-1);
    }
#line 2261 "build/parser.tab.cpp"
    break;

  case 74: /* $@28: %empty  */
#line 703 "src/parser.y"
            { emit("SWP", 1); }
#line 2267 "build/parser.tab.cpp"
    break;

  case 75: /* condition: value $@28 LEQ value  */
#line 703 "src/parser.y"
                                          { 
        emit("SWP", 1); 
        emit("SUB", 1); 
        emit("JPOS", 0);
        if_stack.push_back(code.size()-1);
    }
#line 2278 "build/parser.tab.cpp"
    break;

  case 76: /* value: num  */
#line 711 "src/parser.y"
            { 
        long long start = code.size();
        gen_const(0, (yyvsp[0].num)); /* Generates code into r0 */
        (yyval.operand).is_const = true;
        (yyval.operand).val = (yyvsp[0].num);
        (yyval.operand).instructions_count = code.size() - start;
    }
#line 2290 "build/parser.tab.cpp"
    break;

  case 77: /* value: identifier  */
#line 718 "src/parser.y"
                 {
        if (!(yyvsp[0].loc)->sym->is_initialized) yyerror("Usage of uninitialized variable");
        
        if ((yyvsp[0].loc)->sym->cache_reg != -1) {
             // Cached in Register!
             // Load from Reg X to Reg A
             emit("RST", 0); 
             emit("ADD", (yyvsp[0].loc)->sym->cache_reg);
        } else {
            if ((yyvsp[0].loc)->reg != -1) emit("RLOAD", (yyvsp[0].loc)->reg);
            else emit("LOAD", (yyvsp[0].loc)->address);
        }
        delete (yyvsp[0].loc);
        (yyval.operand).is_const = false;
    }
#line 2310 "build/parser.tab.cpp"
    break;

  case 78: /* identifier: pidentifier  */
#line 735 "src/parser.y"
                         {
         Symbol* s = get_variable(string((yyvsp[0].str)));
         if (!s) yyerror(("Undefined identifier " + string((yyvsp[0].str))).c_str());
         
         string scoped_name = (current_procedure == "") ? string((yyvsp[0].str)) : current_procedure + "_" + string((yyvsp[0].str));
         if (analyze_mode) {
             long long weight = 1;
             for(int k=0; k<loop_depth; k++) weight *= 10;
             var_usage_counts[scoped_name] += weight;
         } else {
             if (var_register_assignments.count(scoped_name)) {
                 s->cache_reg = var_register_assignments[scoped_name];
                 // Update reg_descriptors
                 reg_descriptors[s->cache_reg] = s;
             }
         }

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
#line 2344 "build/parser.tab.cpp"
    break;

  case 79: /* identifier: pidentifier LBRACKET num RBRACKET  */
#line 764 "src/parser.y"
                                        {
         Symbol* s = get_variable(string((yyvsp[-3].str)));
         if (!s) yyerror(("Undefined identifier " + string((yyvsp[-3].str))).c_str());
         (yyval.loc) = new Location();
         (yyval.loc)->sym = s;
         if (s->is_param) {
             emit("LOAD", s->address); 
             long long offset = (yyvsp[-1].num) - s->start;
             gen_const(2, offset);
             emit("ADD", 2);
             emit("SWP", 7);
             (yyval.loc)->reg = 7;
             (yyval.loc)->address = -1;
         } else {
            (yyval.loc)->address = s->address + (yyvsp[-1].num) - s->start;
            (yyval.loc)->reg = -1;
         }
    }
#line 2367 "build/parser.tab.cpp"
    break;

  case 80: /* identifier: pidentifier LBRACKET pidentifier RBRACKET  */
#line 782 "src/parser.y"
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
         
         if (arr->start > 0) { gen_const(2, arr->start); emit("SUB", 2); }
         
         if (arr->is_param) {
             emit("SWP", 2); 
             emit("LOAD", arr->address); 
             emit("ADD", 2); 
         } else {
             gen_const(2, arr->address); 
             emit("ADD", 2);
         }
         
         emit("SWP", 7);
         (yyval.loc) = new Location();
         (yyval.loc)->address = -1;
         (yyval.loc)->reg = 7;
         (yyval.loc)->sym = arr;
    }
#line 2403 "build/parser.tab.cpp"
    break;


#line 2407 "build/parser.tab.cpp"

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

#line 815 "src/parser.y"


void yyerror(const char *s) {
    cerr << "Error: " << s << " at line " << yylineno << endl;
    exit(1);
}

extern FILE *yyin;
void yyrestart(FILE *input_file);

int main(int argc, char* argv[]) {
    if (argc > 1) {
        yyin = fopen(argv[1], "r");
        if (!yyin) {
            cerr << "Cannot open input file: " << argv[1] << endl;
            return 1;
        }
    }

    // PASS 1: Analysis
    analyze_mode = true;
    if (yyparse() != 0) return 1;
    
    // Assign Registers
    vector<pair<long long, string>> usage_list;
    for (auto const& [name, count] : var_usage_counts) {
        usage_list.push_back({count, name});
    }
    sort(usage_list.rbegin(), usage_list.rend());
    
    // Use r4(e), r5(f), r6(g) primarily. Maybe r1(b), r2(c), r7(h)?
    // User requested r1-r7.
    // r3 is RA - Reserved.
    // r1, r2 are used by Math Kernel Inputs. If we spill around calls, we can use them!
    // But parser uses r1, r2 as temps for Expressions.
    // Safest set without rewriting Expression Logic: r4, r5, r6, r7. (r7 used for Arr offset).
    // Let's stick to r4, r5, r6 for now to avoid Reg1/Reg2 clobbering in `expression` rule.
    // If I had time to rewrite `expression` to use any free register, I would.
    // For now, let's try 4, 5, 6.
    
    int available_regs[] = {4, 5, 6}; 
    int reg_limit = 3;
    int reg_idx = 0;
    
    for (auto p : usage_list) {
         if (reg_idx >= reg_limit) break;
         string name = p.second;
         // Note: Symbol table is cleared, so we don't know if is_array.
         // We assume scalar. Arrays usage counts should be on the array name?
         // Ideally Pass 1 checks is_array.
         // Let's filter out known arrays if possible. 
         // Since symtable is gone, we can't check `is_array`.
         // Improvise: Register assignment is tentative. In Pass 2, `add_symbol` will check.
         var_register_assignments[name] = available_regs[reg_idx];
         reg_idx++;
    }

    // PASS 2: Codegen
    fseek(yyin, 0, SEEK_SET);
    yyrestart(yyin);
    reset_parser_state();
    analyze_mode = false;
    yylineno = 1;

    if (yyparse() == 0) {
        // Optimize
        optimize_code();
        
        FILE* out = stdout;
        if (argc > 2) {
            out = fopen(argv[2], "w");
            if (!out) {
                cerr << "Cannot open output file: " << argv[2] << endl;
                return 1;
            }
        }

        for (const auto& instr : code) {
            fprintf(out, "%s", instr.opcode.c_str());
            if (instr.has_arg) {
                if (instr.opcode == "RST" || instr.opcode == "INC" || instr.opcode == "DEC" || 
                    instr.opcode == "SHL" || instr.opcode == "SHR" || instr.opcode == "SWP" || 
                    instr.opcode == "ADD" || instr.opcode == "SUB" || instr.opcode == "COPY" ||
                    instr.opcode == "RLOAD" || instr.opcode == "RSTORE") {
                    
                    if (instr.arg >= 0 && instr.arg < 26) {
                        char reg = 'a' + (char)instr.arg;
                        fprintf(out, " %c", reg);
                    } else {
                        fprintf(out, " %lld", instr.arg);
                    }
                } else {
                    fprintf(out, " %lld", instr.arg);
                }
            }
            fprintf(out, "\n");
        }

        if (out != stdout) fclose(out);
    }
    return 0;
}
