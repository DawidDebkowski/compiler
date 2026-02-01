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
#include <fstream>

#include "types.hpp"
#include "codegen.hpp"
#include "symtable.hpp"
#include "math_kernel.hpp"
#include "ast.hpp"

using namespace std;

extern int yylineno;
int yylex();
void yyerror(const char *s);

extern RootNode* parsed_root;


#line 98 "build/parser.tab.cpp"

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
  YYSYMBOL_procedures = 50,                /* procedures  */
  YYSYMBOL_proc_head = 51,                 /* proc_head  */
  YYSYMBOL_52_1 = 52,                      /* $@1  */
  YYSYMBOL_main = 53,                      /* main  */
  YYSYMBOL_commands = 54,                  /* commands  */
  YYSYMBOL_command = 55,                   /* command  */
  YYSYMBOL_proc_call = 56,                 /* proc_call  */
  YYSYMBOL_args = 57,                      /* args  */
  YYSYMBOL_declarations = 58,              /* declarations  */
  YYSYMBOL_args_decl = 59,                 /* args_decl  */
  YYSYMBOL_type = 60,                      /* type  */
  YYSYMBOL_expression = 61,                /* expression  */
  YYSYMBOL_condition = 62,                 /* condition  */
  YYSYMBOL_value = 63,                     /* value  */
  YYSYMBOL_identifier = 64                 /* identifier  */
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
#define YYLAST   267

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  48
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  17
/* YYNRULES -- Number of rules.  */
#define YYNRULES  51
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  135

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
static const yytype_uint8 yyrline[] =
{
       0,    67,    67,    74,    80,    86,    89,    89,   102,   103,
     106,   107,   110,   113,   118,   123,   127,   131,   135,   139,
     142,   145,   150,   156,   160,   166,   169,   172,   175,   180,
     185,   192,   193,   194,   195,   198,   199,   200,   201,   202,
     203,   206,   207,   208,   209,   210,   211,   214,   215,   218,
     219,   223
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
  "ENDFOR", "$accept", "program_all", "procedures", "proc_head", "$@1",
  "main", "commands", "command", "proc_call", "args", "declarations",
  "args_decl", "type", "expression", "condition", "value", "identifier", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-39)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -39,    34,    16,   -39,   -28,    40,   -39,     0,    42,    -3,
     -39,    -2,   222,    13,     7,    27,   222,    11,   -38,   -38,
     222,    28,    45,   -38,    12,     3,   -39,    32,    61,    46,
     222,    49,   -39,   -39,   -39,   -32,    50,    53,   222,    15,
     -39,    87,    -1,   -39,    84,   184,    82,    70,    71,    69,
      18,   -39,   -39,   -39,   -38,    76,   137,    73,    27,   -39,
     -39,   -39,   149,   222,   -38,   -38,   -38,   -38,   -38,   -38,
     222,   -38,   -38,   -39,   -39,   -39,    -5,    79,    80,    77,
      43,    81,   -39,    83,    88,   -39,   176,   -39,   -39,   -39,
     -39,   -39,   -39,   195,    85,    47,    89,   -39,   -39,   -39,
     -39,   -38,   -38,   -38,   -38,   -38,    91,    93,   -39,   222,
     -39,   -39,   -39,   -38,   -38,   -39,   -39,   -39,   -39,   -39,
     -39,   -39,    90,   211,   113,   115,   100,   -39,   222,   222,
     -39,   157,   168,   -39,   -39
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       5,     0,     0,     1,     0,     0,     2,     0,     0,     0,
       6,     0,     0,    27,     0,    34,     0,     0,     0,     0,
       0,     0,     0,     0,    49,     0,    11,     0,     0,     0,
       0,     0,    31,    32,    33,     0,     0,     0,     0,    49,
      47,     0,     0,    48,     0,     0,     0,     0,     0,     0,
       0,     9,    10,    19,     0,     0,     0,    25,    34,     7,
      30,     4,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    20,    21,    24,     0,     0,     0,     0,
      35,     0,     8,     0,     0,     3,     0,    41,    42,    43,
      44,    45,    46,     0,     0,     0,     0,    22,    51,    50,
      12,     0,     0,     0,     0,     0,     0,     0,    29,     0,
      14,    15,    16,     0,     0,    23,    36,    37,    38,    39,
      40,    28,     0,     0,     0,     0,     0,    13,     0,     0,
      26,     0,     0,    17,    18
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -39,   -39,   -39,   -39,   -39,   -39,   -16,   -24,   -39,   -39,
     126,   -39,    95,   -39,   -14,    37,    17
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     1,     2,     8,    15,     6,    25,    26,    27,    76,
      14,    35,    36,    79,    41,    42,    28
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      37,    52,    12,    16,    45,    44,    58,    39,    40,    51,
      59,    18,    30,    52,    56,    19,    38,     7,    20,     4,
      21,    52,    62,     5,    22,    23,    64,    65,    66,    67,
      68,    69,    52,    96,     3,    43,    43,    97,    52,    47,
      43,    10,    13,    13,     9,    31,    11,    86,    24,    31,
      32,    33,    34,    49,    93,    50,    29,    94,    50,    61,
      48,    18,    52,    77,    78,    19,   113,   114,    20,    52,
      21,    43,    53,    46,    22,    23,   101,   102,   103,   104,
     105,    43,    43,    43,    43,    43,    43,    54,    43,    43,
      39,    80,    55,   123,    57,    60,    63,    70,    24,    52,
      72,    87,    88,    89,    90,    91,    92,    52,    52,    95,
      73,    74,   131,   132,    75,    81,    83,   100,    43,    43,
      43,    43,    43,    98,    99,   112,   128,   106,   129,   107,
      43,    43,   122,   108,   115,   121,   126,    17,   116,   117,
     118,   119,   120,    82,   130,    18,     0,     0,     0,    19,
     124,   125,    20,    84,    21,    85,     0,    18,    22,    23,
       0,    19,     0,     0,    20,    18,    21,     0,     0,    19,
      22,    23,    20,     0,    21,     0,    18,     0,    22,    23,
      19,     0,    24,    20,    18,    21,   109,   110,    19,    22,
      23,    20,    18,    21,    24,     0,    19,    22,    23,    20,
      71,    21,    24,    18,   133,    22,    23,    19,     0,   111,
      20,     0,    21,    24,     0,   134,    22,    23,     0,    18,
       0,    24,   127,    19,     0,     0,    20,     0,    21,    24,
      18,     0,    22,    23,    19,     0,     0,    20,     0,    21,
      24,     0,     0,    22,    23,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    24,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    24
};

static const yytype_int16 yycheck[] =
{
      16,    25,     5,     5,    20,    19,    38,    45,    46,     6,
      42,     8,     5,    37,    30,    12,     5,    45,    15,     3,
      17,    45,    38,     7,    21,    22,    27,    28,    29,    30,
      31,    32,    56,    38,     0,    18,    19,    42,    62,    22,
      23,    41,    45,    45,     4,    38,     4,    63,    45,    38,
      23,    24,    25,    41,    70,    43,    43,    71,    43,     6,
      23,     8,    86,    45,    46,    12,    19,    20,    15,    93,
      17,    54,    40,    45,    21,    22,    33,    34,    35,    36,
      37,    64,    65,    66,    67,    68,    69,    26,    71,    72,
      45,    54,    46,   109,    45,    45,     9,    13,    45,   123,
      18,    64,    65,    66,    67,    68,    69,   131,   132,    72,
      40,    40,   128,   129,    45,    39,    43,    40,   101,   102,
     103,   104,   105,    44,    44,    40,    13,    46,    13,    46,
     113,   114,    39,    45,    45,    44,    46,    11,   101,   102,
     103,   104,   105,     6,    44,     8,    -1,    -1,    -1,    12,
     113,   114,    15,    58,    17,     6,    -1,     8,    21,    22,
      -1,    12,    -1,    -1,    15,     8,    17,    -1,    -1,    12,
      21,    22,    15,    -1,    17,    -1,     8,    -1,    21,    22,
      12,    -1,    45,    15,     8,    17,    10,    11,    12,    21,
      22,    15,     8,    17,    45,    -1,    12,    21,    22,    15,
      16,    17,    45,     8,    47,    21,    22,    12,    -1,    14,
      15,    -1,    17,    45,    -1,    47,    21,    22,    -1,     8,
      -1,    45,    11,    12,    -1,    -1,    15,    -1,    17,    45,
       8,    -1,    21,    22,    12,    -1,    -1,    15,    -1,    17,
      45,    -1,    -1,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    45
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    49,    50,     0,     3,     7,    53,    45,    51,     4,
      41,     4,     5,    45,    58,    52,     5,    58,     8,    12,
      15,    17,    21,    22,    45,    54,    55,    56,    64,    43,
       5,    38,    23,    24,    25,    59,    60,    54,     5,    45,
      46,    62,    63,    64,    62,    54,    45,    64,    63,    41,
      43,     6,    55,    40,    26,    46,    54,    45,    38,    42,
      45,     6,    54,     9,    27,    28,    29,    30,    31,    32,
      13,    16,    18,    40,    40,    45,    57,    45,    46,    61,
      63,    39,     6,    43,    60,     6,    54,    63,    63,    63,
      63,    63,    63,    54,    62,    63,    38,    42,    44,    44,
      40,    33,    34,    35,    36,    37,    46,    46,    45,    10,
      11,    14,    40,    19,    20,    45,    63,    63,    63,    63,
      63,    44,    39,    54,    63,    63,    46,    11,    13,    13,
      44,    54,    54,    47,    47
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    48,    49,    50,    50,    50,    52,    51,    53,    53,
      54,    54,    55,    55,    55,    55,    55,    55,    55,    55,
      55,    55,    56,    57,    57,    58,    58,    58,    58,    59,
      59,    60,    60,    60,    60,    61,    61,    61,    61,    61,
      61,    62,    62,    62,    62,    62,    62,    63,    63,    64,
      64,    64
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     8,     7,     0,     0,     5,     6,     5,
       2,     1,     4,     7,     5,     5,     5,     9,     9,     2,
       3,     3,     4,     3,     1,     3,     8,     1,     6,     4,
       2,     1,     1,     1,     0,     1,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     1,     1,     1,
       4,     4
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
  case 2: /* program_all: procedures main  */
#line 67 "src/parser.y"
                              {
     parsed_root = new RootNode(*(yyvsp[-1].proc_list), *(yyvsp[0].stmt_list), yylineno);
     delete (yyvsp[-1].proc_list);
     delete (yyvsp[0].stmt_list);
   }
#line 1259 "build/parser.tab.cpp"
    break;

  case 3: /* procedures: procedures PROCEDURE proc_head IS declarations IN commands END  */
#line 74 "src/parser.y"
                                                                            {
        (yyvsp[-7].proc_list)->push_back(new ProcedureNode(current_procedure, *(yyvsp[-1].stmt_list), yylineno));
        delete (yyvsp[-1].stmt_list);
        current_procedure = "";
        (yyval.proc_list) = (yyvsp[-7].proc_list);
    }
#line 1270 "build/parser.tab.cpp"
    break;

  case 4: /* procedures: procedures PROCEDURE proc_head IS IN commands END  */
#line 80 "src/parser.y"
                                                        {
        (yyvsp[-6].proc_list)->push_back(new ProcedureNode(current_procedure, *(yyvsp[-1].stmt_list), yylineno));
        delete (yyvsp[-1].stmt_list);
        current_procedure = "";
        (yyval.proc_list) = (yyvsp[-6].proc_list);
    }
#line 1281 "build/parser.tab.cpp"
    break;

  case 5: /* procedures: %empty  */
#line 86 "src/parser.y"
                  { (yyval.proc_list) = new std::vector<ProcedureNode*>(); }
#line 1287 "build/parser.tab.cpp"
    break;

  case 6: /* $@1: %empty  */
#line 89 "src/parser.y"
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
#line 1302 "build/parser.tab.cpp"
    break;

  case 7: /* proc_head: pidentifier LPAREN $@1 args_decl RPAREN  */
#line 98 "src/parser.y"
                       {
    }
#line 1309 "build/parser.tab.cpp"
    break;

  case 8: /* main: PROGRAM IS declarations IN commands END  */
#line 102 "src/parser.y"
                                               { (yyval.stmt_list) = (yyvsp[-1].stmt_list); }
#line 1315 "build/parser.tab.cpp"
    break;

  case 9: /* main: PROGRAM IS IN commands END  */
#line 103 "src/parser.y"
                                 { (yyval.stmt_list) = (yyvsp[-1].stmt_list); }
#line 1321 "build/parser.tab.cpp"
    break;

  case 10: /* commands: commands command  */
#line 106 "src/parser.y"
                            { (yyvsp[-1].stmt_list)->push_back((yyvsp[0].stmt)); (yyval.stmt_list) = (yyvsp[-1].stmt_list); }
#line 1327 "build/parser.tab.cpp"
    break;

  case 11: /* commands: command  */
#line 107 "src/parser.y"
              { (yyval.stmt_list) = new std::vector<StatementNode*>(); (yyval.stmt_list)->push_back((yyvsp[0].stmt)); }
#line 1333 "build/parser.tab.cpp"
    break;

  case 12: /* command: identifier ASSIGN expression SEMICOLON  */
#line 110 "src/parser.y"
                                                 {
        (yyval.stmt) = new AssignmentNode((yyvsp[-3].ident), (yyvsp[-1].expr), yylineno);
    }
#line 1341 "build/parser.tab.cpp"
    break;

  case 13: /* command: IF condition THEN commands ELSE commands ENDIF  */
#line 113 "src/parser.y"
                                                     {
        (yyval.stmt) = new IfNode((yyvsp[-5].cond), *(yyvsp[-3].stmt_list), *(yyvsp[-1].stmt_list), yylineno);
        delete (yyvsp[-3].stmt_list);
        delete (yyvsp[-1].stmt_list);
    }
#line 1351 "build/parser.tab.cpp"
    break;

  case 14: /* command: IF condition THEN commands ENDIF  */
#line 118 "src/parser.y"
                                       {
        std::vector<StatementNode*> empty;
        (yyval.stmt) = new IfNode((yyvsp[-3].cond), *(yyvsp[-1].stmt_list), empty, yylineno);
        delete (yyvsp[-1].stmt_list);
    }
#line 1361 "build/parser.tab.cpp"
    break;

  case 15: /* command: WHILE condition DO commands ENDWHILE  */
#line 123 "src/parser.y"
                                           {
        (yyval.stmt) = new WhileNode((yyvsp[-3].cond), *(yyvsp[-1].stmt_list), yylineno);
        delete (yyvsp[-1].stmt_list);
    }
#line 1370 "build/parser.tab.cpp"
    break;

  case 16: /* command: REPEAT commands UNTIL condition SEMICOLON  */
#line 127 "src/parser.y"
                                                {
        (yyval.stmt) = new RepeatNode((yyvsp[-1].cond), *(yyvsp[-3].stmt_list), yylineno);
        delete (yyvsp[-3].stmt_list);
    }
#line 1379 "build/parser.tab.cpp"
    break;

  case 17: /* command: FOR pidentifier FROM value TO value DO commands ENDFOR  */
#line 131 "src/parser.y"
                                                             {
        (yyval.stmt) = new ForNode(string((yyvsp[-7].str)), (yyvsp[-5].expr), (yyvsp[-3].expr), false, *(yyvsp[-1].stmt_list), yylineno);
        delete (yyvsp[-1].stmt_list);
    }
#line 1388 "build/parser.tab.cpp"
    break;

  case 18: /* command: FOR pidentifier FROM value DOWNTO value DO commands ENDFOR  */
#line 135 "src/parser.y"
                                                                 {
        (yyval.stmt) = new ForNode(string((yyvsp[-7].str)), (yyvsp[-5].expr), (yyvsp[-3].expr), true, *(yyvsp[-1].stmt_list), yylineno);
        delete (yyvsp[-1].stmt_list);
    }
#line 1397 "build/parser.tab.cpp"
    break;

  case 19: /* command: proc_call SEMICOLON  */
#line 139 "src/parser.y"
                          {
        (yyval.stmt) = (yyvsp[-1].stmt);
    }
#line 1405 "build/parser.tab.cpp"
    break;

  case 20: /* command: READ identifier SEMICOLON  */
#line 142 "src/parser.y"
                                {
        (yyval.stmt) = new ReadNode((yyvsp[-1].ident), yylineno);
    }
#line 1413 "build/parser.tab.cpp"
    break;

  case 21: /* command: WRITE value SEMICOLON  */
#line 145 "src/parser.y"
                            {
        (yyval.stmt) = new WriteNode((yyvsp[-1].expr), yylineno);
    }
#line 1421 "build/parser.tab.cpp"
    break;

  case 22: /* proc_call: pidentifier LPAREN args RPAREN  */
#line 150 "src/parser.y"
                                           {
        (yyval.stmt) = new ProcCallNode(string((yyvsp[-3].str)), *(yyvsp[-1].val_list), yylineno);
        delete (yyvsp[-1].val_list);
    }
#line 1430 "build/parser.tab.cpp"
    break;

  case 23: /* args: args COMMA pidentifier  */
#line 156 "src/parser.y"
                              {
        (yyvsp[-2].val_list)->push_back(new IdentifierNode(string((yyvsp[0].str)), yylineno));
        (yyval.val_list) = (yyvsp[-2].val_list);
    }
#line 1439 "build/parser.tab.cpp"
    break;

  case 24: /* args: pidentifier  */
#line 160 "src/parser.y"
                  {
        (yyval.val_list) = new std::vector<ValueNode*>();
        (yyval.val_list)->push_back(new IdentifierNode(string((yyvsp[0].str)), yylineno));
    }
#line 1448 "build/parser.tab.cpp"
    break;

  case 25: /* declarations: declarations COMMA pidentifier  */
#line 166 "src/parser.y"
                                              {
        add_symbol(string((yyvsp[0].str)), false, false, "", 0, 0);
    }
#line 1456 "build/parser.tab.cpp"
    break;

  case 26: /* declarations: declarations COMMA pidentifier LBRACKET num COLON num RBRACKET  */
#line 169 "src/parser.y"
                                                                     {
        add_symbol(string((yyvsp[-5].str)), true, false, "", (yyvsp[-3].num), (yyvsp[-1].num));
    }
#line 1464 "build/parser.tab.cpp"
    break;

  case 27: /* declarations: pidentifier  */
#line 172 "src/parser.y"
                  {
        add_symbol(string((yyvsp[0].str)), false, false, "", 0, 0);
    }
#line 1472 "build/parser.tab.cpp"
    break;

  case 28: /* declarations: pidentifier LBRACKET num COLON num RBRACKET  */
#line 175 "src/parser.y"
                                                  {
        add_symbol(string((yyvsp[-5].str)), true, false, "", (yyvsp[-3].num), (yyvsp[-1].num));
    }
#line 1480 "build/parser.tab.cpp"
    break;

  case 29: /* args_decl: args_decl COMMA type pidentifier  */
#line 180 "src/parser.y"
                                             {
       string m = string((yyvsp[-1].str));
       bool is_arr = (m == "T");
       add_symbol(string((yyvsp[0].str)), is_arr, true, m, 0, 0); free((yyvsp[-1].str));
    }
#line 1490 "build/parser.tab.cpp"
    break;

  case 30: /* args_decl: type pidentifier  */
#line 185 "src/parser.y"
                       {
       string m = string((yyvsp[-1].str));
       bool is_arr = (m == "T");
       add_symbol(string((yyvsp[0].str)), is_arr, true, m, 0, 0); free((yyvsp[-1].str));
    }
#line 1500 "build/parser.tab.cpp"
    break;

  case 31: /* type: T  */
#line 192 "src/parser.y"
         { (yyval.str) = strdup("T"); }
#line 1506 "build/parser.tab.cpp"
    break;

  case 32: /* type: I  */
#line 193 "src/parser.y"
         { (yyval.str) = strdup("I"); }
#line 1512 "build/parser.tab.cpp"
    break;

  case 33: /* type: O  */
#line 194 "src/parser.y"
         { (yyval.str) = strdup("O"); }
#line 1518 "build/parser.tab.cpp"
    break;

  case 34: /* type: %empty  */
#line 195 "src/parser.y"
                   { (yyval.str) = strdup(""); }
#line 1524 "build/parser.tab.cpp"
    break;

  case 35: /* expression: value  */
#line 198 "src/parser.y"
                   { (yyval.expr) = (yyvsp[0].expr); }
#line 1530 "build/parser.tab.cpp"
    break;

  case 36: /* expression: value PLUS value  */
#line 199 "src/parser.y"
                       { (yyval.expr) = new BinaryOpNode((yyvsp[-2].expr), BinaryOp::PLUS, (yyvsp[0].expr), yylineno); }
#line 1536 "build/parser.tab.cpp"
    break;

  case 37: /* expression: value MINUS value  */
#line 200 "src/parser.y"
                        { (yyval.expr) = new BinaryOpNode((yyvsp[-2].expr), BinaryOp::MINUS, (yyvsp[0].expr), yylineno); }
#line 1542 "build/parser.tab.cpp"
    break;

  case 38: /* expression: value MULT value  */
#line 201 "src/parser.y"
                       { (yyval.expr) = new BinaryOpNode((yyvsp[-2].expr), BinaryOp::MULT, (yyvsp[0].expr), yylineno); }
#line 1548 "build/parser.tab.cpp"
    break;

  case 39: /* expression: value DIV value  */
#line 202 "src/parser.y"
                      { (yyval.expr) = new BinaryOpNode((yyvsp[-2].expr), BinaryOp::DIV, (yyvsp[0].expr), yylineno); }
#line 1554 "build/parser.tab.cpp"
    break;

  case 40: /* expression: value MOD value  */
#line 203 "src/parser.y"
                      { (yyval.expr) = new BinaryOpNode((yyvsp[-2].expr), BinaryOp::MOD, (yyvsp[0].expr), yylineno); }
#line 1560 "build/parser.tab.cpp"
    break;

  case 41: /* condition: value EQ value  */
#line 206 "src/parser.y"
                           { (yyval.cond) = new ConditionNode((yyvsp[-2].expr), ConditionOp::EQ, (yyvsp[0].expr), yylineno); }
#line 1566 "build/parser.tab.cpp"
    break;

  case 42: /* condition: value NEQ value  */
#line 207 "src/parser.y"
                      { (yyval.cond) = new ConditionNode((yyvsp[-2].expr), ConditionOp::NEQ, (yyvsp[0].expr), yylineno); }
#line 1572 "build/parser.tab.cpp"
    break;

  case 43: /* condition: value GT value  */
#line 208 "src/parser.y"
                     { (yyval.cond) = new ConditionNode((yyvsp[-2].expr), ConditionOp::GT, (yyvsp[0].expr), yylineno); }
#line 1578 "build/parser.tab.cpp"
    break;

  case 44: /* condition: value LT value  */
#line 209 "src/parser.y"
                     { (yyval.cond) = new ConditionNode((yyvsp[-2].expr), ConditionOp::LT, (yyvsp[0].expr), yylineno); }
#line 1584 "build/parser.tab.cpp"
    break;

  case 45: /* condition: value GEQ value  */
#line 210 "src/parser.y"
                      { (yyval.cond) = new ConditionNode((yyvsp[-2].expr), ConditionOp::GEQ, (yyvsp[0].expr), yylineno); }
#line 1590 "build/parser.tab.cpp"
    break;

  case 46: /* condition: value LEQ value  */
#line 211 "src/parser.y"
                      { (yyval.cond) = new ConditionNode((yyvsp[-2].expr), ConditionOp::LEQ, (yyvsp[0].expr), yylineno); }
#line 1596 "build/parser.tab.cpp"
    break;

  case 47: /* value: num  */
#line 214 "src/parser.y"
            { (yyval.expr) = new NumberNode((yyvsp[0].num), yylineno); }
#line 1602 "build/parser.tab.cpp"
    break;

  case 48: /* value: identifier  */
#line 215 "src/parser.y"
                 { (yyval.expr) = (yyvsp[0].ident); }
#line 1608 "build/parser.tab.cpp"
    break;

  case 49: /* identifier: pidentifier  */
#line 218 "src/parser.y"
                         { (yyval.ident) = new IdentifierNode(string((yyvsp[0].str)), yylineno); }
#line 1614 "build/parser.tab.cpp"
    break;

  case 50: /* identifier: pidentifier LBRACKET num RBRACKET  */
#line 219 "src/parser.y"
                                        {
         (yyval.ident) = new IdentifierNode(string((yyvsp[-3].str)), yylineno);
         (yyval.ident)->set_array_access((yyvsp[-1].num), yylineno);
    }
#line 1623 "build/parser.tab.cpp"
    break;

  case 51: /* identifier: pidentifier LBRACKET pidentifier RBRACKET  */
#line 223 "src/parser.y"
                                                {
         (yyval.ident) = new IdentifierNode(string((yyvsp[-3].str)), yylineno);
         (yyval.ident)->set_array_access(string((yyvsp[-1].str)), yylineno);
    }
#line 1632 "build/parser.tab.cpp"
    break;


#line 1636 "build/parser.tab.cpp"

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

#line 229 "src/parser.y"


void yyerror(const char *s) {
    cerr << "Error: " << s << " at line " << yylineno << endl;
    exit(1);
}

extern FILE *yyin;

RootNode* parsed_root = nullptr;

int main(int argc, char* argv[]) {
    if (argc > 1) {
        yyin = fopen(argv[1], "r");
        if (!yyin) {
            cerr << "Cannot open input file: " << argv[1] << endl;
            return 1;
        }
    }

    if (yyparse() == 0 && parsed_root) {
        parsed_root->validate();
        
        // printing ast
        /* if (argc > 2) {
             string out_name = argv[2];
             string ast_name = out_name + ".ast";
             ofstream ast_file(ast_name);
             if (ast_file.is_open()) {
                 parsed_root->print(ast_file);
                 ast_file.close();
                 // cout << "AST written to " << ast_name << endl;
             }
        } */
        
        parsed_root->codegen();
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
            if(instr.comment != "") {
                fprintf(out, " # %s", instr.comment.c_str());
            }
            fprintf(out, "\n");
        }

        if (out != stdout) fclose(out);
        delete parsed_root;
    }
    return 0;
}
