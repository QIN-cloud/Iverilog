/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 2 "parse.y" /* yacc.c:339  */

/*
 * Copyright (c) 2001-2020 Stephen Williams (steve@icarus.com)
 *
 *    This source code is free software; you can redistribute it
 *    and/or modify it in source code form under the terms of the GNU
 *    General Public License as published by the Free Software
 *    Foundation; either version 2 of the License, or (at your option)
 *    any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

# include  "parse_misc.h"
# include  "compile.h"
# include  "delay.h"
# include  <list>
# include  <cstdio>
# include  <cstdlib>
# include  <cassert>
# include  "ivl_alloc.h"
# include  "version_base.h"

/*
 * These are bits in the lexor.
 */
extern FILE*yyin;

vector <const char*> file_names;

/*
 * Local variables.
 */

/*
 * When parsing a modpath list, this is the processed destination that
 * the source items will attach themselves to.
 */
static struct __vpiModPath*modpath_dst = 0;

#line 114 "parse.cc" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "parse.h".  */
#ifndef YY_YY_PARSE_H_INCLUDED
# define YY_YY_PARSE_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    K_A = 258,
    K_APV = 259,
    K_ARITH_ABS = 260,
    K_ARITH_DIV = 261,
    K_ARITH_DIV_R = 262,
    K_ARITH_DIV_S = 263,
    K_ARITH_MOD = 264,
    K_ARITH_MOD_R = 265,
    K_ARITH_MOD_S = 266,
    K_ARITH_MULT = 267,
    K_ARITH_MULT_R = 268,
    K_ARITH_SUB = 269,
    K_ARITH_SUB_R = 270,
    K_ARITH_SUM = 271,
    K_ARITH_SUM_R = 272,
    K_ARITH_POW = 273,
    K_ARITH_POW_R = 274,
    K_ARITH_POW_S = 275,
    K_ARRAY = 276,
    K_ARRAY_2U = 277,
    K_ARRAY_2S = 278,
    K_ARRAY_I = 279,
    K_ARRAY_OBJ = 280,
    K_ARRAY_R = 281,
    K_ARRAY_S = 282,
    K_ARRAY_STR = 283,
    K_ARRAY_PORT = 284,
    K_CAST_INT = 285,
    K_CAST_REAL = 286,
    K_CAST_REAL_S = 287,
    K_CAST_2 = 288,
    K_CLASS = 289,
    K_CMP_EEQ = 290,
    K_CMP_EQ = 291,
    K_CMP_EQX = 292,
    K_CMP_EQZ = 293,
    K_CMP_WEQ = 294,
    K_CMP_WNE = 295,
    K_CMP_EQ_R = 296,
    K_CMP_NEE = 297,
    K_CMP_NE = 298,
    K_CMP_NE_R = 299,
    K_CMP_GE = 300,
    K_CMP_GE_R = 301,
    K_CMP_GE_S = 302,
    K_CMP_GT = 303,
    K_CMP_GT_R = 304,
    K_CMP_GT_S = 305,
    K_CONCAT = 306,
    K_CONCAT8 = 307,
    K_DEBUG = 308,
    K_DELAY = 309,
    K_DFF_N = 310,
    K_DFF_N_ACLR = 311,
    K_DFF_N_ASET = 312,
    K_DFF_P = 313,
    K_DFF_P_ACLR = 314,
    K_DFF_P_ASET = 315,
    K_ENUM2 = 316,
    K_ENUM2_S = 317,
    K_ENUM4 = 318,
    K_ENUM4_S = 319,
    K_EVENT = 320,
    K_EVENT_OR = 321,
    K_EXPORT = 322,
    K_EXTEND_S = 323,
    K_FUNCTOR = 324,
    K_IMPORT = 325,
    K_ISLAND = 326,
    K_LATCH = 327,
    K_MODPATH = 328,
    K_NET = 329,
    K_NET_S = 330,
    K_NET_R = 331,
    K_NET_2S = 332,
    K_NET_2U = 333,
    K_NET8 = 334,
    K_NET8_2S = 335,
    K_NET8_2U = 336,
    K_NET8_S = 337,
    K_PARAM_STR = 338,
    K_PARAM_L = 339,
    K_PARAM_REAL = 340,
    K_PART = 341,
    K_PART_PV = 342,
    K_PART_V = 343,
    K_PART_V_S = 344,
    K_PORT = 345,
    K_PORT_INFO = 346,
    K_PV = 347,
    K_REDUCE_AND = 348,
    K_REDUCE_OR = 349,
    K_REDUCE_XOR = 350,
    K_REDUCE_NAND = 351,
    K_REDUCE_NOR = 352,
    K_REDUCE_XNOR = 353,
    K_REPEAT = 354,
    K_RESOLV = 355,
    K_RTRAN = 356,
    K_RTRANIF0 = 357,
    K_RTRANIF1 = 358,
    K_SCOPE = 359,
    K_SFUNC = 360,
    K_SFUNC_E = 361,
    K_SHIFTL = 362,
    K_SHIFTR = 363,
    K_SHIFTRS = 364,
    K_SUBSTITUTE = 365,
    K_THREAD = 366,
    K_TIMESCALE = 367,
    K_TRAN = 368,
    K_TRANIF0 = 369,
    K_TRANIF1 = 370,
    K_TRANVP = 371,
    K_UFUNC_REAL = 372,
    K_UFUNC_VEC4 = 373,
    K_UFUNC_E = 374,
    K_UDP = 375,
    K_UDP_C = 376,
    K_UDP_S = 377,
    K_VAR = 378,
    K_VAR_COBJECT = 379,
    K_VAR_DARRAY = 380,
    K_VAR_QUEUE = 381,
    K_VAR_S = 382,
    K_VAR_STR = 383,
    K_VAR_I = 384,
    K_VAR_R = 385,
    K_VAR_2S = 386,
    K_VAR_2U = 387,
    K_vpi_call = 388,
    K_vpi_call_w = 389,
    K_vpi_call_i = 390,
    K_vpi_func = 391,
    K_vpi_func_r = 392,
    K_vpi_func_s = 393,
    K_ivl_version = 394,
    K_ivl_delay_selection = 395,
    K_vpi_module = 396,
    K_vpi_time_precision = 397,
    K_file_names = 398,
    K_file_line = 399,
    K_PORT_INPUT = 400,
    K_PORT_OUTPUT = 401,
    K_PORT_INOUT = 402,
    K_PORT_MIXED = 403,
    K_PORT_NODIR = 404,
    T_INSTR = 405,
    T_LABEL = 406,
    T_NUMBER = 407,
    T_STRING = 408,
    T_SYMBOL = 409,
    T_VECTOR = 410
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 50 "parse.y" /* yacc.c:355  */

      char*text;
      char **table;
      uint64_t numb;
      bool flag;

      comp_operands_t opa;

      struct symb_s  symb;
      struct symbv_s symbv;

      struct numbv_s numbv;

      struct enum_name_s enum_name;
      std::list<struct enum_name_s>*enum_namev;

      struct symb_s vect;

      struct argv_s argv;
      vpiHandle vpi;

      vvp_delay_t*cdelay;

      int      vpi_enum;

#line 336 "parse.cc" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSE_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 353 "parse.cc" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

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

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
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


#if ! defined yyoverflow || YYERROR_VERBOSE

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
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
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
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  14
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1295

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  171
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  41
/* YYNRULES -- Number of rules.  */
#define YYNRULES  257
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1175

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   410

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     162,   163,   167,   157,   159,   158,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   166,   156,
     168,     2,   169,   170,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   160,     2,   161,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   164,     2,   165,     2,     2,     2,     2,
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
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   137,   137,   139,   139,   143,   144,   148,   150,   152,
     154,   156,   158,   163,   163,   168,   170,   176,   177,   192,
     195,   208,   211,   214,   220,   223,   225,   228,   231,   234,
     237,   240,   243,   246,   249,   252,   259,   262,   265,   271,
     279,   282,   285,   287,   292,   296,   301,   307,   312,   317,
     322,   327,   335,   340,   345,   350,   355,   360,   365,   370,
     375,   380,   385,   390,   395,   400,   405,   410,   415,   420,
     425,   430,   435,   440,   445,   450,   455,   460,   465,   470,
     475,   480,   485,   494,   496,   500,   507,   506,   513,   516,
     519,   522,   525,   528,   531,   534,   539,   544,   547,   550,
     553,   556,   559,   562,   567,   571,   575,   580,   584,   590,
     595,   601,   611,   614,   617,   620,   628,   631,   636,   639,
     649,   656,   663,   668,   673,   678,   698,   701,   708,   714,
     719,   721,   723,   725,   732,   735,   742,   745,   748,   751,
     754,   757,   760,   763,   766,   769,   775,   779,   783,   787,
     791,   795,   799,   803,   807,   813,   817,   821,   825,   829,
     833,   840,   843,   846,   849,   854,   857,   860,   863,   866,
     869,   872,   875,   878,   881,   884,   890,   889,   894,   899,
     903,   904,   908,   909,   913,   924,   926,   930,   932,   937,
     943,   945,   947,   949,   954,   959,   967,   969,   974,   975,
     982,   983,   987,   988,   992,  1001,  1006,  1014,  1022,  1047,
    1053,  1060,  1066,  1071,  1077,  1085,  1087,  1091,  1096,  1098,
    1100,  1102,  1104,  1106,  1108,  1110,  1116,  1122,  1131,  1137,
    1146,  1152,  1164,  1171,  1174,  1184,  1185,  1189,  1190,  1191,
    1192,  1193,  1197,  1199,  1201,  1203,  1205,  1207,  1209,  1211,
    1213,  1218,  1220,  1225,  1226,  1230,  1232,  1234
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "K_A", "K_APV", "K_ARITH_ABS",
  "K_ARITH_DIV", "K_ARITH_DIV_R", "K_ARITH_DIV_S", "K_ARITH_MOD",
  "K_ARITH_MOD_R", "K_ARITH_MOD_S", "K_ARITH_MULT", "K_ARITH_MULT_R",
  "K_ARITH_SUB", "K_ARITH_SUB_R", "K_ARITH_SUM", "K_ARITH_SUM_R",
  "K_ARITH_POW", "K_ARITH_POW_R", "K_ARITH_POW_S", "K_ARRAY", "K_ARRAY_2U",
  "K_ARRAY_2S", "K_ARRAY_I", "K_ARRAY_OBJ", "K_ARRAY_R", "K_ARRAY_S",
  "K_ARRAY_STR", "K_ARRAY_PORT", "K_CAST_INT", "K_CAST_REAL",
  "K_CAST_REAL_S", "K_CAST_2", "K_CLASS", "K_CMP_EEQ", "K_CMP_EQ",
  "K_CMP_EQX", "K_CMP_EQZ", "K_CMP_WEQ", "K_CMP_WNE", "K_CMP_EQ_R",
  "K_CMP_NEE", "K_CMP_NE", "K_CMP_NE_R", "K_CMP_GE", "K_CMP_GE_R",
  "K_CMP_GE_S", "K_CMP_GT", "K_CMP_GT_R", "K_CMP_GT_S", "K_CONCAT",
  "K_CONCAT8", "K_DEBUG", "K_DELAY", "K_DFF_N", "K_DFF_N_ACLR",
  "K_DFF_N_ASET", "K_DFF_P", "K_DFF_P_ACLR", "K_DFF_P_ASET", "K_ENUM2",
  "K_ENUM2_S", "K_ENUM4", "K_ENUM4_S", "K_EVENT", "K_EVENT_OR", "K_EXPORT",
  "K_EXTEND_S", "K_FUNCTOR", "K_IMPORT", "K_ISLAND", "K_LATCH",
  "K_MODPATH", "K_NET", "K_NET_S", "K_NET_R", "K_NET_2S", "K_NET_2U",
  "K_NET8", "K_NET8_2S", "K_NET8_2U", "K_NET8_S", "K_PARAM_STR",
  "K_PARAM_L", "K_PARAM_REAL", "K_PART", "K_PART_PV", "K_PART_V",
  "K_PART_V_S", "K_PORT", "K_PORT_INFO", "K_PV", "K_REDUCE_AND",
  "K_REDUCE_OR", "K_REDUCE_XOR", "K_REDUCE_NAND", "K_REDUCE_NOR",
  "K_REDUCE_XNOR", "K_REPEAT", "K_RESOLV", "K_RTRAN", "K_RTRANIF0",
  "K_RTRANIF1", "K_SCOPE", "K_SFUNC", "K_SFUNC_E", "K_SHIFTL", "K_SHIFTR",
  "K_SHIFTRS", "K_SUBSTITUTE", "K_THREAD", "K_TIMESCALE", "K_TRAN",
  "K_TRANIF0", "K_TRANIF1", "K_TRANVP", "K_UFUNC_REAL", "K_UFUNC_VEC4",
  "K_UFUNC_E", "K_UDP", "K_UDP_C", "K_UDP_S", "K_VAR", "K_VAR_COBJECT",
  "K_VAR_DARRAY", "K_VAR_QUEUE", "K_VAR_S", "K_VAR_STR", "K_VAR_I",
  "K_VAR_R", "K_VAR_2S", "K_VAR_2U", "K_vpi_call", "K_vpi_call_w",
  "K_vpi_call_i", "K_vpi_func", "K_vpi_func_r", "K_vpi_func_s",
  "K_ivl_version", "K_ivl_delay_selection", "K_vpi_module",
  "K_vpi_time_precision", "K_file_names", "K_file_line", "K_PORT_INPUT",
  "K_PORT_OUTPUT", "K_PORT_INOUT", "K_PORT_MIXED", "K_PORT_NODIR",
  "T_INSTR", "T_LABEL", "T_NUMBER", "T_STRING", "T_SYMBOL", "T_VECTOR",
  "';'", "'+'", "'-'", "','", "'['", "']'", "'('", "')'", "'{'", "'}'",
  "':'", "'*'", "'<'", "'>'", "'?'", "$accept", "source_file",
  "header_lines_opt", "header_lines", "header_line", "footer_lines", "$@1",
  "name_strings", "program", "statement", "$@2", "$@3",
  "class_properties_opt", "class_properties", "class_property",
  "dimensions_opt", "dimensions", "dimension", "enum_type",
  "enum_type_names", "enum_type_name", "local_flag", "label_opt",
  "operands_opt", "operands", "operand", "argument_opt", "argument_list",
  "argument", "symbol_access", "symbols", "numbers", "symbols_net",
  "symbol", "symbol_opt", "modpath_src_list", "port_type", "modpath_src",
  "udp_table", "signed_t_number", "delay", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,   392,   393,   394,
     395,   396,   397,   398,   399,   400,   401,   402,   403,   404,
     405,   406,   407,   408,   409,   410,    59,    43,    45,    44,
      91,    93,    40,    41,   123,   125,    58,    42,    60,    62,
      63
};
# endif

#define YYPACT_NINF -947

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-947)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -120,   -40,   -28,   -23,    28,   146,   -51,  -120,  -947,   -60,
       0,    16,    39,    75,  -947,   111,   134,   169,   210,   245,
     250,   -29,   337,   343,   345,   164,  1017,  -947,   -57,  -947,
    -947,   -67,  -947,   237,  -947,  -947,  -947,   344,   346,    -4,
     227,   342,   348,   347,   -44,   -26,   352,   349,   351,   353,
     359,   360,   361,   363,   364,   365,   366,   367,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,   360,   360,   393,
     394,   396,   397,   398,   399,   400,   401,   402,   403,   404,
     405,   406,   407,   408,   409,   410,   411,   412,   413,   414,
     415,   416,   417,   418,   419,   422,   372,   420,   421,   423,
     -43,   360,   392,   424,   425,   426,   427,   432,   434,  -143,
     -82,   -80,   -79,   338,   -78,   338,   338,   -76,   435,   436,
     437,   433,   438,   439,   440,   441,   360,   360,   360,   360,
     360,   360,   444,   443,   445,   446,   449,   450,   451,   452,
     453,   454,   455,   456,   457,   459,   460,   338,   461,   462,
     463,   338,   464,   338,   465,   338,   338,  -947,   467,  -947,
    -947,   468,   469,   470,   471,   472,   473,   474,   -15,  -947,
    -947,  -947,  -947,  -947,  -947,  -947,  -947,   475,   476,   477,
     478,  -947,  -947,   479,   480,   482,   -25,   481,   483,   484,
     487,  -947,    52,  -947,   447,   448,   485,   486,   488,   489,
     490,   491,   492,   493,   500,   501,   502,   503,   504,   505,
     506,   507,   508,   509,   510,   512,   513,   514,   515,    53,
      54,   516,   431,   517,   518,   519,   520,   521,   522,   523,
     526,   527,   528,   529,   530,   531,   532,   533,   534,   494,
     542,   -70,   360,   360,   360,   360,   360,   360,   543,   544,
     545,   546,   547,   548,   540,    58,   549,   541,   550,   551,
     552,   360,   360,   554,  -947,   556,   555,   558,   560,   561,
     563,   564,   565,   567,   568,   569,   570,   572,   573,   575,
     576,   577,   557,   566,   571,   574,   578,   579,   580,   582,
     583,   584,   585,   586,   587,   588,   590,   591,   589,   592,
     593,   594,   595,   596,   613,   614,   615,   616,   581,   620,
     618,   619,   597,   623,   627,   622,   629,   630,   628,   633,
     634,   635,   636,   637,   638,   639,  -947,  -947,   640,   641,
    -947,  -947,   642,   559,   643,   644,   645,  -947,   646,   647,
     652,   651,   653,   654,   650,  -947,   360,   360,   360,   360,
     360,   360,   360,   360,   360,   360,   360,   360,   360,   360,
     360,   360,   -30,   -18,   -18,   -18,   -18,   -18,   -18,   -18,
     200,   360,  -947,  -947,   360,   658,   360,   360,   360,   360,
     360,   360,   360,   360,   360,   360,   360,   360,   360,   360,
     360,   360,   659,   660,   661,    59,   360,   655,   656,   657,
     663,   666,   667,   466,   631,   664,   665,   670,  -947,   360,
    -947,  -947,   360,   108,   677,  -947,   673,   360,   674,   675,
     676,   678,   679,   680,   681,   682,   683,   684,   685,   686,
     687,   688,   689,   697,   698,   699,   700,   701,   703,   704,
     708,  -947,  -947,  -947,  -947,  -947,  -947,   702,   360,   710,
     711,   712,   360,   360,   360,   714,   715,   716,   717,   360,
     718,   719,   696,  -947,   720,   721,   722,  -947,   723,   -18,
     724,   725,  -947,   726,   727,   732,   733,   734,   735,   141,
    -947,   -15,   662,   736,   730,   737,  -947,  -947,  -947,   738,
     741,   739,   742,   746,  -947,    60,    65,    66,    67,    87,
      88,    89,    93,    94,    95,    99,   100,   101,   105,   106,
    -947,   747,   750,   -18,   -18,   -18,   -18,   -18,   -18,   -18,
     -18,   748,   749,   118,   119,   632,   120,   124,   125,   126,
     131,   133,   154,   155,   156,   161,   162,   184,   185,   186,
     190,   191,   754,   755,   -55,  -947,   265,   752,   360,   360,
     360,   360,   360,   360,   740,   740,   740,   740,   360,   192,
     753,   360,   758,   756,   360,   757,   -18,   -18,   -18,   -18,
     -18,   -18,   -18,   -18,   -18,   -18,   -18,   -18,   -18,   -18,
     -18,   759,   761,   762,   760,   763,   764,   765,   769,   767,
     197,   768,   770,   771,   202,   203,   204,   766,   772,   773,
     774,   224,   775,   776,   -18,   780,   781,   -18,   786,   -18,
     -18,   -18,   787,   782,   782,   782,   792,   782,   782,   783,
     789,  -947,  -947,  -947,   793,   794,  -947,  -947,   795,   796,
     797,  -947,  -947,  -947,  -947,  -947,  -947,  -947,  -947,  -947,
    -947,  -947,  -947,  -947,  -947,  -947,  -947,  -947,   225,   798,
     799,   800,   790,   804,   802,   806,  -947,  -947,  -947,  -947,
    -947,  -947,  -947,  -947,  -947,  -947,  -947,  -947,  -947,  -947,
    -947,  -947,  -947,  -947,  -947,  -947,  -947,   801,   803,   811,
    -947,   808,  -947,   807,   812,   813,   815,   816,   817,   230,
     231,  -947,   232,   233,   238,   239,  -947,  -947,   249,   825,
    -947,   809,  -947,   -18,   -18,   -18,   -18,   -18,   -18,   -18,
     -18,   -18,   -18,   -18,   -18,   -18,   -18,   -18,   819,   820,
     821,   829,   830,   831,   832,  -947,   360,  -947,   833,   834,
     836,  -947,  -947,  -947,   827,   360,   360,   837,  -947,   835,
     841,   -18,  -947,  -947,   -18,   842,   839,   -18,   -18,   840,
     844,     5,   539,   672,   713,   782,   751,   788,  -947,  -947,
     843,   846,   847,   849,   852,  -947,   -18,   -18,   -18,   -18,
    -947,  -947,   -18,  -947,   848,   850,   851,     6,  -947,   360,
     360,   360,   360,   360,   360,  -947,  -947,  -947,   740,  -947,
    -947,  -947,  -947,  -947,   853,  -947,   360,   854,   856,   857,
     858,   859,   861,   911,   949,   950,   959,   960,   961,   969,
     970,   971,   845,   257,   855,   975,   973,   977,   994,   995,
    1000,   256,   996,   360,   254,   259,   997,  -947,   276,   998,
    1002,  1003,  1004,  -947,  1005,  1006,  -947,  1007,   624,   649,
     706,  -947,  -947,  -947,  1008,  -947,  -947,  1001,  1012,  1013,
     990,  1014,  1016,  -947,  -947,  -947,  -947,  1015,   -18,   -18,
     -18,   -18,   -18,   709,  1018,   848,  -947,  1011,  1019,  1020,
    -947,  1021,  1022,  1023,  1024,  1025,  1026,  -947,  1027,  -119,
     277,  -947,   360,   360,   360,   360,   360,   360,   360,   360,
     360,   360,   360,   360,   360,   360,   360,  1031,  1032,  1029,
    1033,  -947,  1038,  -947,  -947,  -947,   281,  -947,   360,  1039,
     283,   360,   360,   360,  -947,  1040,   835,  -947,  -947,  -947,
    -947,  -947,  -947,  1037,  1041,  1042,     9,  1045,  1046,  1047,
    1048,  1049,  1050,  1051,  1052,  1053,  1054,  1055,  1056,  1060,
    -947,  -947,   360,   360,   791,  -947,   360,   360,  -947,   360,
     360,   360,  1009,  1043,  1062,  -947,   360,   285,  -947,  -947,
     287,   289,   291,   293,   295,   297,   299,   301,   303,   305,
     307,   309,   311,   313,  -947,  -947,  1059,  -947,  1061,  -947,
    1064,   315,   317,  -947,     8,    11,   316,  -947,   321,  1035,
    1044,  1063,  -947,  -947,  1066,  1067,  1068,  1069,  1071,  1072,
    -947,  -947,  -947,  -947,  -947,  -947,  1070,   323,   325,  -947,
    1074,   327,  1075,   329,   331,  1062,  1062,  -947,  -122,  -947,
    -947,   360,  -947,  -947,  -947,  -947,  -947,  -947,  -947,  -947,
    -947,  -947,  -947,  -947,  -947,  -947,  -947,  -947,  1073,  -947,
    -947,   360,  1078,  1079,   360,  -947,    14,  1076,     3,  1010,
    1077,  1080,  1082,  1081,  1083,  1084,  -947,  -947,  -947,  -947,
    1085,  -947,  -947,  1086,  -947,   -61,   -56,  1089,   360,  -140,
    -947,  1091,   333,  1088,  1093,    24,   395,   562,   707,  1092,
    1094,  1095,  1098,  1096,  1100,  1101,  1102,  1087,  1103,  1104,
    1105,  1106,  1107,   360,   -81,   360,   -73,  -947,  -947,   360,
    1108,  1109,  -947,  -947,  -947,  1110,  -947,  -947,  -947,  1114,
    1115,  1117,  1111,  1120,  -947,  -947,  -947,  1118,  -947,  -947,
    1121,  -947,  1105,  -947,  -947,  -947,  -947,   360,  1112,  -947,
     360,  1113,  -947,   360,  1123,  1122,  1124,  1057,  1058,  1127,
    1116,  -947,   785,  -947,  -947,   360,  -947,   360,  -947,  1125,
    -947,  1128,  -947,  -947,  1119,  -947,  1130,  -947,  -947,  -947,
    1126,  -947,  1129,  -947,  -947
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       4,     0,     0,     0,     0,     0,   201,     3,     5,     0,
       0,     0,     0,     0,     1,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   200,   179,   201,    17,
     178,     0,     6,     0,     7,     9,    10,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   199,
     199,   199,   199,   199,   199,   199,   199,   199,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   199,     0,     0,
       0,   199,     0,   199,     0,   199,   199,   117,     0,     2,
      18,     0,     0,     0,     0,     0,     0,     0,   203,     8,
      11,    12,   237,   238,   239,   240,   241,     0,     0,     0,
       0,   128,   134,     0,     0,     0,     0,     0,     0,     0,
       0,   232,     0,   226,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   198,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   207,   208,     0,   202,
     205,   206,     0,     0,     0,     0,     0,   130,     0,     0,
       0,     0,     0,     0,     0,    47,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    49,    50,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   114,     0,
     115,   168,     0,     0,     0,   165,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    97,    98,    99,   100,   101,   102,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   145,     0,     0,     0,   142,     0,     0,
       0,     0,    13,     0,     0,     0,     0,     0,     0,     0,
     116,     0,     0,     0,     0,     0,   135,   132,   131,     0,
       0,     0,     0,     0,   227,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     253,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    84,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   210,   210,   210,     0,   210,   210,     0,
       0,   204,   129,   169,     0,     0,   133,   172,     0,     0,
       0,    52,    53,    54,    55,    56,    57,    58,    59,    63,
      64,    65,    66,    60,    61,    62,    35,   254,     0,     0,
       0,     0,     0,     0,     0,     0,    34,    33,    48,    51,
     176,    67,    69,    70,    71,    81,    82,    72,    68,    73,
      74,    75,    76,    77,    78,    79,    80,     0,     0,     0,
     255,     0,    83,     0,     0,     0,     0,     0,     0,     0,
       0,   194,     0,     0,     0,     0,   112,   104,     0,     0,
     167,     0,    86,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   166,     0,    39,     0,     0,
       0,   109,   110,   111,     0,     0,     0,     0,    23,     0,
       0,     0,   143,   144,     0,     0,     0,     0,     0,     0,
      14,     0,     0,     0,     0,   210,     0,     0,   119,   118,
       0,     0,     0,     0,     0,    32,     0,     0,     0,     0,
      31,    28,     0,    30,   181,     0,     0,     0,    85,     0,
       0,     0,     0,     0,     0,   196,   197,   190,     0,   191,
     192,   193,   113,    19,     0,    96,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   251,     0,     0,
       0,     0,     0,   141,     0,     0,    15,     0,     0,     0,
       0,   215,   213,   216,   209,   211,   217,     0,     0,     0,
       0,     0,     0,   170,   171,   173,   174,     0,     0,     0,
       0,     0,     0,     0,     0,   180,   183,     0,     0,     0,
     256,     0,     0,     0,     0,     0,     0,   195,     0,     0,
       0,   235,   234,   234,   234,   234,   234,   234,   234,   234,
     234,   234,   234,   234,   234,   234,   234,     0,     0,     0,
       0,    40,     0,    42,    43,   103,     0,   107,     0,     0,
       0,     0,     0,     0,    22,     0,     0,   136,   137,   138,
     139,   140,    16,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     177,   182,     0,     0,     0,    88,     0,     0,    89,     0,
       0,     0,     0,     0,     0,    87,     0,     0,   233,   230,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   161,   162,     0,   164,     0,   126,
       0,     0,     0,    46,     0,     0,     0,   252,     0,     0,
       0,     0,   214,   212,     0,     0,     0,     0,     0,     0,
     175,    24,    25,    26,    27,    29,     0,     0,     0,   257,
       0,     0,     0,     0,     0,     0,     0,   228,     0,   236,
     155,   234,   146,   156,   147,   160,   154,   157,   149,   148,
     158,   150,   153,   151,   159,   152,   163,    41,     0,   105,
     108,     0,     0,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    44,    45,    90,    92,
       0,    91,    93,     0,    20,     0,     0,     0,     0,     0,
     231,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     186,     0,     0,     0,     0,     0,     0,   229,   242,     0,
       0,     0,   106,    36,    37,     0,   218,   219,   220,     0,
       0,     0,     0,     0,   120,   121,   122,     0,   124,   125,
       0,   184,   185,   188,    94,    95,   245,     0,     0,   248,
       0,     0,   243,     0,     0,     0,     0,     0,     0,     0,
       0,   123,     0,   187,   246,     0,   249,     0,   244,     0,
      38,     0,   221,   223,     0,   224,     0,   247,   250,   127,
       0,   222,     0,   225,   189
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -947,  -947,  -947,  -947,   621,  -947,  -947,  -947,  -947,   537,
    -947,  -947,  -947,  -947,  -300,  -947,  -947,  -555,  -947,   -72,
    -220,    27,  -947,  -947,  -947,   149,  -592,  -947,  -295,  -560,
     -62,  -946,  -699,  -136,  -389,  -947,  -947,  -323,  -206,  -348,
    -947
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     5,     6,     7,     8,   169,   622,   760,    28,    29,
     806,   784,   874,   875,   876,  1131,  1132,  1133,    30,   700,
     701,   275,    31,   338,   339,   340,   762,   854,   855,   856,
     202,  1028,   967,   203,   969,   890,   187,   891,   838,   523,
     406
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint16 yytable[] =
{
     297,   298,   299,   300,   301,   302,   848,   849,   848,   849,
     262,   273,   848,   849,   201,   229,   230,   848,   849,     1,
       2,     3,     4,  1109,   274,   524,   525,   526,   527,   528,
     529,   530,   763,   764,    15,   766,   767,  1077,   962,   963,
      15,  1078,   341,   964,    16,    17,    18,    19,  1079,   265,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      20,    21,    22,    23,    24,    25,   171,   172,   173,   174,
     175,   176,   276,   201,   278,   280,   283,   177,   287,  1075,
    1076,   201,  1137,   178,   201,   274,   168,   274,   274,   274,
    1140,   274,   404,    33,    26,   850,    34,   850,  1077,    27,
      26,   850,  1103,  1077,   689,    27,   850,  1105,   690,  1104,
     263,   264,   192,     9,  1106,   193,   407,   408,   409,   410,
     411,   412,   520,    45,   521,    10,   194,   349,   522,    46,
      11,   619,   195,   350,   520,   426,   427,   336,   337,   201,
     522,   182,   183,   184,   185,   186,    14,   277,   279,   281,
     282,   284,   285,   286,   288,  1090,    35,  1091,   851,   852,
     853,  1092,   851,  1002,   853,   879,  1086,   356,  1087,   880,
     356,  1052,    36,   860,  1053,   658,   659,   660,   661,   662,
     663,   664,   665,   356,   318,    12,    13,  1115,   322,   405,
     324,    37,   326,   327,   970,   971,   972,   973,   974,   975,
     976,   977,   978,   979,   980,   981,   982,   983,   355,   382,
     383,   356,   356,   356,   420,   555,   641,   356,   556,   356,
     504,   642,   643,   644,   356,   356,   356,    38,   713,   714,
     715,   716,   717,   718,   719,   720,   721,   722,   723,   724,
     725,   726,   727,   645,   646,   647,   356,   356,   356,   648,
     649,   650,   356,   356,   356,   651,   652,   653,   356,   356,
     356,   654,   655,    39,   356,   356,   751,   571,   572,   754,
     557,   756,   757,   758,   668,   669,   671,   356,   356,   356,
     672,   673,   674,   356,   356,   356,   570,   675,    40,   676,
     356,   575,   356,   629,   630,   505,   506,   507,   508,   509,
     510,   511,   512,   513,   514,   515,   516,   517,   518,   519,
     677,   678,   679,   356,   356,   356,    50,   680,   681,   533,
     356,   356,   534,    41,   536,   537,   538,   539,   540,   541,
     542,   543,   544,   545,   546,   547,   548,   549,   550,   551,
     682,   683,   684,   356,   356,   356,   685,   686,   706,   356,
     356,   356,   531,   737,   532,   341,   356,   569,   741,   742,
     743,   356,   356,   356,    42,   807,   808,   809,   810,   811,
     812,   813,   814,   815,   816,   817,   818,   819,   820,   821,
     748,   775,   795,   356,   776,   796,   188,   797,   799,   800,
     798,   798,   798,   179,   801,   802,   600,   798,   356,    43,
     604,   605,   606,   840,    44,   803,   841,   611,   356,   844,
     845,   908,   917,   356,   909,   918,   921,   691,   356,   201,
     504,   922,   693,   694,   695,   696,   697,   698,   868,   869,
     870,   871,   924,   965,   872,   925,   966,   989,   711,   993,
     990,  1030,   356,  1032,  1031,  1033,  1031,  1034,  1031,  1035,
    1031,  1036,  1031,  1037,  1031,  1038,  1031,  1039,  1031,  1040,
    1031,  1041,  1031,  1042,  1031,  1043,  1031,  1044,  1031,  1045,
    1031,  1049,  1031,  1050,   356,   356,  1051,  1055,  1054,  1066,
     925,  1067,   356,  1069,   356,  1072,  1070,  1074,  1073,  1112,
     356,    47,   356,   702,   703,   704,  1088,    48,  1093,    49,
     180,   189,   181,   191,   196,   274,   705,   190,   197,   708,
     198,   200,   199,   204,   201,   205,   206,   207,   208,   209,
     944,   945,   946,   947,   948,   210,   211,   212,   213,   214,
     215,   216,   217,   218,   258,   219,   220,   221,   222,   223,
     224,   225,   226,   228,   227,   231,   266,   232,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,  1116,   170,   251,   252,   253,   254,
     255,   256,   249,   250,   257,   951,   267,  1153,   887,   268,
     269,   270,   259,   260,   271,   261,   272,   292,   289,   290,
     291,   385,   293,   294,   295,   296,   303,   304,   306,   305,
     829,   307,   308,   309,   310,   311,   357,   358,   312,   313,
     314,   315,   316,   317,   319,   320,   321,   323,   325,   328,
     329,   330,   331,   332,   333,   334,   335,   342,    32,   564,
     343,   344,   345,   346,   348,   351,   347,   352,   353,   354,
     631,  1003,  1080,  1029,   359,   360,   402,   361,   362,   363,
     364,   365,   366,   881,   882,   883,   884,   885,   886,   367,
     368,   369,   370,   371,   372,   373,   374,   375,   376,   377,
     889,   378,   379,   380,   381,   384,   386,   387,   388,   389,
     390,   391,   392,   834,   835,   393,   394,   395,   396,   397,
     398,   399,   400,   401,   403,   413,   414,   415,   416,   419,
     422,   417,   423,   857,   418,   421,   428,   430,   425,   429,
     424,   431,   432,   493,   433,   434,   446,   435,   436,   437,
     998,   438,   439,   440,   441,   447,   442,   443,   444,   445,
     448,  1117,     0,   449,   472,   451,   452,   450,   453,   454,
     455,   456,   460,   461,     0,   457,   458,   459,   462,     0,
     476,   463,   464,   465,   466,   467,   968,   968,   968,   968,
     968,   968,   968,   968,   968,   968,   968,   968,   968,   968,
     968,   920,   468,   469,   470,   471,   473,   474,   475,   477,
     478,   479,   480,   481,   482,   483,   484,   485,   486,   487,
     488,   489,   933,   670,   565,   492,   490,   494,   495,     0,
     491,   496,   497,   498,   499,   500,     0,   501,   502,   503,
     535,   552,   553,   554,   558,   559,   560,   934,   632,     0,
    1020,  1021,   561,  1022,  1023,   562,   563,   566,   567,   568,
     889,   573,   574,   576,   577,   578,   858,   579,   580,   581,
     582,   583,   584,   585,   586,   587,   588,   589,   590,   591,
     592,   593,   594,   595,   599,   614,   991,   596,   597,   994,
     995,   996,   598,   601,   602,   603,   607,   608,   609,   610,
     612,   613,   615,   616,   935,   949,  1118,   859,     0,   623,
     624,   617,   618,   620,   621,   625,   626,   627,   628,   634,
    1017,  1018,   633,   699,   636,   968,   635,   637,   638,  1024,
     640,   639,   657,   656,   666,   667,   687,   688,   692,   707,
     709,   728,   710,   729,   730,   861,   712,     0,   744,   731,
       0,   738,   732,   733,   734,   735,   736,     0,     0,   739,
     740,   745,   746,   747,   749,   750,   752,   753,   755,   768,
     759,   761,  1108,  1110,   765,   769,   780,   770,   771,   772,
     773,  1166,   862,   785,  1019,   786,   774,   777,   778,   779,
     781,   782,   783,   787,   788,   805,   789,  1136,  1138,  1139,
    1141,   790,   791,  1142,   792,   793,   794,   804,   822,   823,
     824,   825,   826,   827,   828,   830,   833,   831,   837,  1082,
     832,   836,  1085,   839,   842,   843,   846,   847,   907,   863,
     873,  1154,   864,   865,  1156,   866,   867,  1158,     0,   910,
       0,   877,   878,   892,   888,   893,   894,   895,   896,  1167,
     897,  1168,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
     898,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   899,   900,
     136,   137,   138,   139,   140,   141,   142,   143,   901,   902,
     903,   144,   145,   146,   147,   148,   149,   150,   904,   905,
     906,   911,   912,   913,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     914,   915,   916,   937,   940,   919,   923,   926,   927,   928,
     929,   930,   931,   932,   938,   939,   941,   936,   942,   943,
     952,  1025,   954,   167,   950,  1094,     0,   955,   953,     0,
     958,   956,   957,   986,   959,   960,   961,   984,   985,   987,
     988,   999,   992,   997,  1056,  1000,  1001,  1004,  1005,  1006,
    1007,  1008,  1009,  1057,     0,  1026,     0,  1010,  1011,  1012,
    1013,  1014,  1015,  1016,  1027,  1046,  1048,  1047,  1059,  1060,
    1061,  1062,  1058,  1063,  1064,  1081,  1162,  1163,  1089,  1065,
    1068,  1071,  1083,  1084,  1097,     0,     0,  1100,     0,  1101,
    1102,  1107,  1095,  1111,  1113,  1096,  1098,     0,  1099,  1114,
    1122,  1119,  1127,  1120,  1121,  1123,  1124,  1125,  1126,  1128,
    1129,     0,  1134,  1135,  1145,  1130,  1146,  1147,  1144,  1148,
    1149,  1143,  1150,  1152,  1151,  1155,  1157,  1159,  1160,  1164,
    1170,  1169,  1172,  1161,     0,  1165,     0,     0,  1171,     0,
    1174,     0,     0,     0,     0,  1173
};

static const yytype_int16 yycheck[] =
{
     136,   137,   138,   139,   140,   141,     3,     4,     3,     4,
      53,   154,     3,     4,   154,    77,    78,     3,     4,   139,
     140,   141,   142,   163,   167,   373,   374,   375,   376,   377,
     378,   379,   624,   625,    91,   627,   628,   159,   157,   158,
      91,   163,   178,   162,   101,   102,   103,   104,   170,   111,
     101,   102,   103,   104,   111,   112,   113,   114,   115,   116,
     111,   112,   113,   114,   115,   116,   133,   134,   135,   136,
     137,   138,   154,   154,   154,   154,   154,   144,   154,  1025,
    1026,   154,   163,   150,   154,   167,   143,   167,   167,   167,
     163,   167,   162,   153,   151,    92,   156,    92,   159,   156,
     151,    92,   163,   159,   159,   156,    92,   163,   163,   170,
     153,   154,   156,   153,   170,   159,   252,   253,   254,   255,
     256,   257,   152,   152,   154,   153,   152,   152,   158,   158,
     153,   479,   158,   158,   152,   271,   272,   152,   153,   154,
     158,   145,   146,   147,   148,   149,     0,   120,   121,   122,
     123,   124,   125,   126,   127,   152,   156,   154,   153,   154,
     155,   158,   153,   154,   155,   159,   152,   159,   154,   163,
     159,   163,   156,   765,   163,   523,   524,   525,   526,   527,
     528,   529,   530,   159,   157,   157,   158,   163,   161,   251,
     163,   152,   165,   166,   893,   894,   895,   896,   897,   898,
     899,   900,   901,   902,   903,   904,   905,   906,   156,   156,
     156,   159,   159,   159,   156,   156,   156,   159,   159,   159,
     356,   156,   156,   156,   159,   159,   159,   152,   576,   577,
     578,   579,   580,   581,   582,   583,   584,   585,   586,   587,
     588,   589,   590,   156,   156,   156,   159,   159,   159,   156,
     156,   156,   159,   159,   159,   156,   156,   156,   159,   159,
     159,   156,   156,   152,   159,   159,   614,   159,   160,   617,
     406,   619,   620,   621,   156,   156,   156,   159,   159,   159,
     156,   156,   156,   159,   159,   159,   422,   156,   154,   156,
     159,   427,   159,   152,   153,   357,   358,   359,   360,   361,
     362,   363,   364,   365,   366,   367,   368,   369,   370,   371,
     156,   156,   156,   159,   159,   159,   152,   156,   156,   381,
     159,   159,   384,   154,   386,   387,   388,   389,   390,   391,
     392,   393,   394,   395,   396,   397,   398,   399,   400,   401,
     156,   156,   156,   159,   159,   159,   156,   156,   156,   159,
     159,   159,   152,   156,   154,   491,   159,   419,   156,   156,
     156,   159,   159,   159,   154,   713,   714,   715,   716,   717,
     718,   719,   720,   721,   722,   723,   724,   725,   726,   727,
     156,   156,   152,   159,   159,   155,   159,   156,   156,   156,
     159,   159,   159,   156,   156,   156,   458,   159,   159,   154,
     462,   463,   464,   751,   154,   156,   754,   469,   159,   757,
     758,   154,   156,   159,   157,   159,   162,   152,   159,   154,
     556,   162,   558,   559,   560,   561,   562,   563,   776,   777,
     778,   779,   156,   156,   782,   159,   159,   156,   574,   156,
     159,   156,   159,   156,   159,   156,   159,   156,   159,   156,
     159,   156,   159,   156,   159,   156,   159,   156,   159,   156,
     159,   156,   159,   156,   159,   156,   159,   156,   159,   156,
     159,   156,   159,   156,   159,   159,   159,   156,   162,   156,
     159,   156,   159,   156,   159,   156,   159,   156,   159,   156,
     159,   154,   159,   565,   566,   567,  1056,   154,  1058,   154,
     156,   159,   156,   156,   152,   167,   568,   159,   159,   571,
     159,   152,   159,   152,   154,   152,   152,   152,   152,   152,
     868,   869,   870,   871,   872,   152,   152,   152,   152,   152,
     152,   152,   152,   152,   162,   153,   153,   153,   153,   153,
     153,   153,   153,   152,   154,   152,   154,   153,   152,   152,
     152,   152,   152,   152,   152,   152,   152,   152,   152,   152,
     152,   152,   152,   152,   169,    28,   152,   152,   152,   152,
     152,   152,   160,   160,   152,   875,   152,  1132,   798,   154,
     154,   154,   162,   162,   152,   162,   152,   154,   153,   153,
     153,   160,   154,   154,   154,   154,   152,   154,   152,   154,
     736,   152,   152,   152,   152,   152,   159,   159,   154,   154,
     154,   154,   153,   153,   153,   153,   153,   153,   153,   152,
     152,   152,   152,   152,   152,   152,   152,   152,     7,   163,
     154,   154,   154,   154,   152,   154,   156,   154,   154,   152,
     491,   936,  1031,   966,   159,   159,   152,   159,   159,   159,
     159,   159,   159,   789,   790,   791,   792,   793,   794,   159,
     159,   159,   159,   159,   159,   159,   159,   159,   159,   159,
     806,   159,   159,   159,   159,   159,   159,   159,   159,   159,
     159,   159,   159,   745,   746,   159,   159,   159,   159,   159,
     159,   159,   159,   159,   152,   152,   152,   152,   152,   159,
     159,   154,   152,   164,   156,   156,   152,   152,   156,   153,
     159,   153,   152,   154,   153,   152,   159,   153,   153,   152,
     926,   153,   153,   153,   152,   159,   153,   152,   152,   152,
     159,   169,    -1,   159,   153,   156,   156,   159,   156,   156,
     156,   156,   152,   152,    -1,   159,   159,   159,   159,    -1,
     153,   159,   159,   159,   159,   159,   892,   893,   894,   895,
     896,   897,   898,   899,   900,   901,   902,   903,   904,   905,
     906,   833,   159,   159,   159,   159,   156,   159,   159,   156,
     153,   159,   153,   153,   156,   152,   152,   152,   152,   152,
     152,   152,   168,   161,   163,   153,   156,   154,   154,    -1,
     159,   156,   156,   156,   152,   154,    -1,   154,   154,   159,
     152,   152,   152,   152,   159,   159,   159,   168,   156,    -1,
     956,   957,   159,   959,   960,   159,   159,   163,   163,   159,
     966,   154,   159,   159,   159,   159,   164,   159,   159,   159,
     159,   159,   159,   159,   159,   159,   159,   159,   159,   152,
     152,   152,   152,   152,   152,   159,   918,   154,   154,   921,
     922,   923,   154,   153,   153,   153,   152,   152,   152,   152,
     152,   152,   152,   152,   168,   166,   169,   164,    -1,   153,
     153,   159,   159,   159,   159,   153,   153,   153,   153,   159,
     952,   953,   156,   153,   156,  1031,   159,   156,   159,   961,
     154,   159,   152,   156,   156,   156,   152,   152,   156,   156,
     152,   152,   156,   152,   152,   164,   159,    -1,   152,   159,
      -1,   153,   159,   159,   159,   156,   159,    -1,    -1,   159,
     159,   159,   159,   159,   159,   159,   156,   156,   152,   156,
     153,   159,  1078,  1079,   152,   156,   156,   154,   154,   154,
     154,   166,   164,   152,   163,   152,   159,   159,   159,   159,
     156,   159,   156,   152,   156,   156,   159,  1103,  1104,  1105,
    1106,   159,   159,  1109,   159,   159,   159,   152,   159,   159,
     159,   152,   152,   152,   152,   152,   159,   153,   153,  1051,
     154,   154,  1054,   152,   152,   156,   156,   153,   153,   156,
     152,  1137,   156,   156,  1140,   156,   154,  1143,    -1,   154,
      -1,   161,   161,   159,   161,   159,   159,   159,   159,  1155,
     159,  1157,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
     159,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,   159,   159,
      93,    94,    95,    96,    97,    98,    99,   100,   159,   159,
     159,   104,   105,   106,   107,   108,   109,   110,   159,   159,
     159,   156,   159,   156,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     156,   156,   152,   152,   164,   159,   159,   159,   156,   156,
     156,   156,   156,   156,   152,   152,   152,   159,   152,   154,
     159,   162,   152,   156,   156,   165,    -1,   156,   159,    -1,
     156,   159,   159,   154,   159,   159,   159,   156,   156,   156,
     152,   154,   153,   153,   159,   154,   154,   152,   152,   152,
     152,   152,   152,   159,    -1,   162,    -1,   156,   156,   156,
     156,   156,   156,   153,   152,   156,   152,   156,   152,   152,
     152,   152,   159,   152,   152,   152,   169,   169,   152,   159,
     156,   156,   154,   154,   152,    -1,    -1,   153,    -1,   154,
     154,   152,   165,   152,   156,   165,   165,    -1,   165,   156,
     152,   159,   165,   159,   159,   159,   156,   156,   156,   156,
     156,    -1,   156,   156,   154,   160,   152,   152,   159,   152,
     159,   163,   152,   152,   156,   163,   163,   154,   156,   152,
     152,   156,   152,   159,    -1,   169,    -1,    -1,   169,    -1,
     161,    -1,    -1,    -1,    -1,   169
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   139,   140,   141,   142,   172,   173,   174,   175,   153,
     153,   153,   157,   158,     0,    91,   101,   102,   103,   104,
     111,   112,   113,   114,   115,   116,   151,   156,   179,   180,
     189,   193,   175,   153,   156,   156,   156,   152,   152,   152,
     154,   154,   154,   154,   154,   152,   158,   154,   154,   154,
     152,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    93,    94,    95,    96,
      97,    98,    99,   100,   104,   105,   106,   107,   108,   109,
     110,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   156,   143,   176,
     180,   133,   134,   135,   136,   137,   138,   144,   150,   156,
     156,   156,   145,   146,   147,   148,   149,   207,   159,   159,
     159,   156,   156,   159,   152,   158,   152,   159,   159,   159,
     152,   154,   201,   204,   152,   152,   152,   152,   152,   152,
     152,   152,   152,   152,   152,   152,   152,   152,   152,   153,
     153,   153,   153,   153,   153,   153,   153,   154,   152,   201,
     201,   152,   153,   152,   152,   152,   152,   152,   152,   152,
     152,   152,   152,   152,   152,   152,   152,   152,   152,   160,
     160,   152,   152,   152,   152,   152,   152,   152,   162,   162,
     162,   162,    53,   153,   154,   201,   154,   152,   154,   154,
     154,   152,   152,   154,   167,   192,   154,   192,   154,   192,
     154,   192,   192,   154,   192,   192,   192,   154,   192,   153,
     153,   153,   154,   154,   154,   154,   154,   204,   204,   204,
     204,   204,   204,   152,   154,   154,   152,   152,   152,   152,
     152,   152,   154,   154,   154,   154,   153,   153,   192,   153,
     153,   153,   192,   153,   192,   153,   192,   192,   152,   152,
     152,   152,   152,   152,   152,   152,   152,   153,   194,   195,
     196,   204,   152,   154,   154,   154,   154,   156,   152,   152,
     158,   154,   154,   154,   152,   156,   159,   159,   159,   159,
     159,   159,   159,   159,   159,   159,   159,   159,   159,   159,
     159,   159,   159,   159,   159,   159,   159,   159,   159,   159,
     159,   159,   156,   156,   159,   160,   159,   159,   159,   159,
     159,   159,   159,   159,   159,   159,   159,   159,   159,   159,
     159,   159,   152,   152,   162,   201,   211,   204,   204,   204,
     204,   204,   204,   152,   152,   152,   152,   154,   156,   159,
     156,   156,   159,   152,   159,   156,   204,   204,   152,   153,
     152,   153,   152,   153,   152,   153,   153,   152,   153,   153,
     153,   152,   153,   152,   152,   152,   159,   159,   159,   159,
     159,   156,   156,   156,   156,   156,   156,   159,   159,   159,
     152,   152,   159,   159,   159,   159,   159,   159,   159,   159,
     159,   159,   153,   156,   159,   159,   153,   156,   153,   159,
     153,   153,   156,   152,   152,   152,   152,   152,   152,   152,
     156,   159,   153,   154,   154,   154,   156,   156,   156,   152,
     154,   154,   154,   159,   204,   201,   201,   201,   201,   201,
     201,   201,   201,   201,   201,   201,   201,   201,   201,   201,
     152,   154,   158,   210,   210,   210,   210,   210,   210,   210,
     210,   152,   154,   201,   201,   152,   201,   201,   201,   201,
     201,   201,   201,   201,   201,   201,   201,   201,   201,   201,
     201,   201,   152,   152,   152,   156,   159,   204,   159,   159,
     159,   159,   159,   159,   163,   163,   163,   163,   159,   201,
     204,   159,   160,   154,   159,   204,   159,   159,   159,   159,
     159,   159,   159,   159,   159,   159,   159,   159,   159,   159,
     159,   152,   152,   152,   152,   152,   154,   154,   154,   152,
     201,   153,   153,   153,   201,   201,   201,   152,   152,   152,
     152,   201,   152,   152,   159,   152,   152,   159,   159,   210,
     159,   159,   177,   153,   153,   153,   153,   153,   153,   152,
     153,   196,   156,   156,   159,   159,   156,   156,   159,   159,
     154,   156,   156,   156,   156,   156,   156,   156,   156,   156,
     156,   156,   156,   156,   156,   156,   156,   152,   210,   210,
     210,   210,   210,   210,   210,   210,   156,   156,   156,   156,
     161,   156,   156,   156,   156,   156,   156,   156,   156,   156,
     156,   156,   156,   156,   156,   156,   156,   152,   152,   159,
     163,   152,   156,   204,   204,   204,   204,   204,   204,   153,
     190,   191,   190,   190,   190,   201,   156,   156,   201,   152,
     156,   204,   159,   210,   210,   210,   210,   210,   210,   210,
     210,   210,   210,   210,   210,   210,   210,   210,   152,   152,
     152,   159,   159,   159,   159,   156,   159,   156,   153,   159,
     159,   156,   156,   156,   152,   159,   159,   159,   156,   159,
     159,   210,   156,   156,   210,   152,   210,   210,   210,   153,
     178,   159,   197,   197,   197,   152,   197,   197,   156,   156,
     154,   154,   154,   154,   159,   156,   159,   159,   159,   159,
     156,   156,   159,   156,   182,   152,   152,   152,   156,   159,
     159,   159,   159,   159,   159,   152,   155,   156,   159,   156,
     156,   156,   156,   156,   152,   156,   181,   210,   210,   210,
     210,   210,   210,   210,   210,   210,   210,   210,   210,   210,
     210,   210,   159,   159,   159,   152,   152,   152,   152,   204,
     152,   153,   154,   159,   201,   201,   154,   153,   209,   152,
     210,   210,   152,   156,   210,   210,   156,   153,     3,     4,
      92,   153,   154,   155,   198,   199,   200,   164,   164,   164,
     197,   164,   164,   156,   156,   156,   156,   154,   210,   210,
     210,   210,   210,   152,   183,   184,   185,   161,   161,   159,
     163,   204,   204,   204,   204,   204,   204,   191,   161,   204,
     206,   208,   159,   159,   159,   159,   159,   159,   159,   159,
     159,   159,   159,   159,   159,   159,   159,   153,   154,   157,
     154,   156,   159,   156,   156,   156,   152,   156,   159,   159,
     201,   162,   162,   159,   156,   159,   159,   156,   156,   156,
     156,   156,   156,   168,   168,   168,   159,   152,   152,   152,
     164,   152,   152,   154,   210,   210,   210,   210,   210,   166,
     156,   185,   159,   159,   152,   156,   159,   159,   156,   159,
     159,   159,   157,   158,   162,   156,   159,   203,   204,   205,
     203,   203,   203,   203,   203,   203,   203,   203,   203,   203,
     203,   203,   203,   203,   156,   156,   154,   156,   152,   156,
     159,   201,   153,   156,   201,   201,   201,   153,   209,   154,
     154,   154,   154,   199,   152,   152,   152,   152,   152,   152,
     156,   156,   156,   156,   156,   156,   153,   201,   201,   163,
     204,   204,   204,   204,   201,   162,   162,   152,   202,   208,
     156,   159,   156,   156,   156,   156,   156,   156,   156,   156,
     156,   156,   156,   156,   156,   156,   156,   156,   152,   156,
     156,   159,   163,   163,   162,   156,   159,   159,   159,   152,
     152,   152,   152,   152,   152,   159,   156,   156,   156,   156,
     159,   156,   156,   159,   156,   202,   202,   159,   163,   170,
     205,   152,   201,   154,   154,   201,   152,   154,   200,   152,
     152,   154,   158,   200,   165,   165,   165,   152,   165,   165,
     153,   154,   154,   163,   170,   163,   170,   152,   204,   163,
     204,   152,   156,   156,   156,   163,   169,   169,   169,   159,
     159,   159,   152,   159,   156,   156,   156,   165,   156,   156,
     160,   186,   187,   188,   156,   156,   204,   163,   204,   204,
     163,   204,   204,   163,   159,   154,   152,   152,   152,   159,
     152,   156,   152,   188,   204,   163,   204,   163,   204,   154,
     156,   159,   169,   169,   152,   169,   166,   204,   204,   156,
     152,   169,   152,   169,   161
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   171,   172,   173,   173,   174,   174,   175,   175,   175,
     175,   175,   175,   177,   176,   178,   178,   179,   179,   180,
     180,   180,   180,   180,   180,   180,   180,   180,   180,   180,
     180,   180,   180,   180,   180,   180,   180,   180,   180,   180,
     180,   180,   180,   180,   180,   180,   180,   180,   180,   180,
     180,   180,   180,   180,   180,   180,   180,   180,   180,   180,
     180,   180,   180,   180,   180,   180,   180,   180,   180,   180,
     180,   180,   180,   180,   180,   180,   180,   180,   180,   180,
     180,   180,   180,   180,   180,   180,   181,   180,   180,   180,
     180,   180,   180,   180,   180,   180,   180,   180,   180,   180,
     180,   180,   180,   180,   180,   180,   180,   180,   180,   180,
     180,   180,   180,   180,   180,   180,   180,   180,   180,   180,
     180,   180,   180,   180,   180,   180,   180,   180,   180,   180,
     180,   180,   180,   180,   180,   180,   180,   180,   180,   180,
     180,   180,   180,   180,   180,   180,   180,   180,   180,   180,
     180,   180,   180,   180,   180,   180,   180,   180,   180,   180,
     180,   180,   180,   180,   180,   180,   180,   180,   180,   180,
     180,   180,   180,   180,   180,   180,   182,   180,   180,   180,
     183,   183,   184,   184,   185,   186,   186,   187,   187,   188,
     189,   189,   189,   189,   190,   190,   191,   191,   192,   192,
     193,   193,   194,   194,   195,   195,   196,   196,   196,   197,
     197,   198,   198,   198,   198,   199,   199,   199,   200,   200,
     200,   200,   200,   200,   200,   200,   201,   201,   202,   202,
     203,   203,   204,   205,   205,   206,   206,   207,   207,   207,
     207,   207,   208,   208,   208,   208,   208,   208,   208,   208,
     208,   209,   209,   210,   210,   211,   211,   211
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     3,     1,     0,     1,     2,     3,     4,     3,
       3,     4,     4,     0,     5,     2,     3,     1,     2,     7,
      11,    10,     8,     6,    10,    10,    10,    10,     7,    10,
       7,     7,     7,     6,     6,     6,    12,    12,    14,     6,
       8,    10,     8,     8,    11,    11,     9,     4,     6,     4,
       4,     6,     6,     6,     6,     6,     6,     6,     6,     6,
       6,     6,     6,     6,     6,     6,     6,     6,     6,     6,
       6,     6,     6,     6,     6,     6,     6,     6,     6,     6,
       6,     6,     6,     6,     5,     7,     0,     9,     9,     9,
      11,    11,    11,    11,    13,    13,     7,     4,     4,     4,
       4,     4,     4,     8,     6,    10,    12,     8,    10,     6,
       6,     6,     6,     7,     4,     4,     4,     2,     6,     6,
      12,    12,    12,    13,    12,    12,     9,    15,     3,     6,
       4,     5,     5,     6,     3,     5,     8,     8,     8,     8,
       8,     7,     4,     6,     6,     4,    10,    10,    10,    10,
      10,    10,    10,    10,    10,    10,    10,    10,    10,    10,
      10,     9,     9,    10,     9,     4,     6,     6,     4,     6,
       8,     8,     6,     8,     8,    10,     0,     9,     1,     1,
       1,     0,     2,     1,     6,     1,     0,     2,     1,     5,
       7,     7,     7,     7,     1,     3,     2,     2,     1,     0,
       1,     0,     1,     0,     3,     1,     1,     1,     1,     2,
       0,     1,     3,     1,     3,     1,     1,     1,     6,     6,
       6,     8,     9,     8,     8,    10,     1,     3,     1,     3,
       1,     3,     1,     1,     0,     1,     3,     1,     1,     1,
       1,     1,     5,     6,     7,     6,     7,     8,     6,     7,
       8,     1,     3,     1,     2,     3,     5,     7
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
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

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



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

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
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
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
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
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
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


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
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
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

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

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
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

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

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
| yyreduce -- Do a reduction.  |
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
        case 7:
#line 149 "parse.y" /* yacc.c:1646  */
    { verify_version((yyvsp[-1].text), NULL); }
#line 2184 "parse.cc" /* yacc.c:1646  */
    break;

  case 8:
#line 151 "parse.y" /* yacc.c:1646  */
    { verify_version((yyvsp[-2].text), (yyvsp[-1].text)); }
#line 2190 "parse.cc" /* yacc.c:1646  */
    break;

  case 9:
#line 153 "parse.y" /* yacc.c:1646  */
    { set_delay_selection((yyvsp[-1].text)); }
#line 2196 "parse.cc" /* yacc.c:1646  */
    break;

  case 10:
#line 155 "parse.y" /* yacc.c:1646  */
    { compile_load_vpi_module((yyvsp[-1].text)); }
#line 2202 "parse.cc" /* yacc.c:1646  */
    break;

  case 11:
#line 157 "parse.y" /* yacc.c:1646  */
    { compile_vpi_time_precision((yyvsp[-1].numb)); }
#line 2208 "parse.cc" /* yacc.c:1646  */
    break;

  case 12:
#line 159 "parse.y" /* yacc.c:1646  */
    { compile_vpi_time_precision(-(yyvsp[-1].numb)); }
#line 2214 "parse.cc" /* yacc.c:1646  */
    break;

  case 13:
#line 163 "parse.y" /* yacc.c:1646  */
    { file_names.reserve((yyvsp[-1].numb)); }
#line 2220 "parse.cc" /* yacc.c:1646  */
    break;

  case 15:
#line 169 "parse.y" /* yacc.c:1646  */
    { file_names.push_back((yyvsp[-1].text)); }
#line 2226 "parse.cc" /* yacc.c:1646  */
    break;

  case 16:
#line 171 "parse.y" /* yacc.c:1646  */
    { file_names.push_back((yyvsp[-1].text)); }
#line 2232 "parse.cc" /* yacc.c:1646  */
    break;

  case 19:
#line 193 "parse.y" /* yacc.c:1646  */
    { compile_functor((yyvsp[-6].text), (yyvsp[-4].text), (yyvsp[-3].numb), 6, 6, (yyvsp[-1].symbv).cnt, (yyvsp[-1].symbv).vect); }
#line 2238 "parse.cc" /* yacc.c:1646  */
    break;

  case 20:
#line 197 "parse.y" /* yacc.c:1646  */
    { unsigned str0 = (yyvsp[-5].numb);
		  unsigned str1 = (yyvsp[-4].numb);
		  compile_functor((yyvsp[-10].text), (yyvsp[-8].text), (yyvsp[-7].numb), str0, str1,
				  (yyvsp[-1].symbv).cnt, (yyvsp[-1].symbv).vect);
		}
#line 2248 "parse.cc" /* yacc.c:1646  */
    break;

  case 21:
#line 209 "parse.y" /* yacc.c:1646  */
    { compile_udp_def(1, (yyvsp[-9].text), (yyvsp[-7].text), (yyvsp[-5].numb), (yyvsp[-3].numb), (yyvsp[-1].table)); }
#line 2254 "parse.cc" /* yacc.c:1646  */
    break;

  case 22:
#line 212 "parse.y" /* yacc.c:1646  */
    { compile_udp_def(0, (yyvsp[-7].text), (yyvsp[-5].text), (yyvsp[-3].numb), 0, (yyvsp[-1].table)); }
#line 2260 "parse.cc" /* yacc.c:1646  */
    break;

  case 23:
#line 215 "parse.y" /* yacc.c:1646  */
    { compile_udp_functor((yyvsp[-5].text), (yyvsp[-3].text), (yyvsp[-1].symbv).cnt, (yyvsp[-1].symbv).vect); }
#line 2266 "parse.cc" /* yacc.c:1646  */
    break;

  case 24:
#line 221 "parse.y" /* yacc.c:1646  */
    { compile_var_array((yyvsp[-9].text), (yyvsp[-7].text), (yyvsp[-5].numb), (yyvsp[-4].numb), (yyvsp[-2].numb), (yyvsp[-1].numb), 0); }
#line 2272 "parse.cc" /* yacc.c:1646  */
    break;

  case 25:
#line 224 "parse.y" /* yacc.c:1646  */
    { compile_var2_array((yyvsp[-9].text), (yyvsp[-7].text), (yyvsp[-5].numb), (yyvsp[-4].numb), (yyvsp[-2].numb), (yyvsp[-1].numb), false); }
#line 2278 "parse.cc" /* yacc.c:1646  */
    break;

  case 26:
#line 226 "parse.y" /* yacc.c:1646  */
    { compile_var2_array((yyvsp[-9].text), (yyvsp[-7].text), (yyvsp[-5].numb), (yyvsp[-4].numb), (yyvsp[-2].numb), (yyvsp[-1].numb), true); }
#line 2284 "parse.cc" /* yacc.c:1646  */
    break;

  case 27:
#line 229 "parse.y" /* yacc.c:1646  */
    { compile_var_array((yyvsp[-9].text), (yyvsp[-7].text), (yyvsp[-5].numb), (yyvsp[-4].numb), (yyvsp[-2].numb), (yyvsp[-1].numb), 2); }
#line 2290 "parse.cc" /* yacc.c:1646  */
    break;

  case 28:
#line 232 "parse.y" /* yacc.c:1646  */
    { compile_real_array((yyvsp[-6].text), (yyvsp[-4].text), (yyvsp[-2].numb), (yyvsp[-1].numb)); }
#line 2296 "parse.cc" /* yacc.c:1646  */
    break;

  case 29:
#line 235 "parse.y" /* yacc.c:1646  */
    { compile_var_array((yyvsp[-9].text), (yyvsp[-7].text), (yyvsp[-5].numb), (yyvsp[-4].numb), (yyvsp[-2].numb), (yyvsp[-1].numb), 1); }
#line 2302 "parse.cc" /* yacc.c:1646  */
    break;

  case 30:
#line 238 "parse.y" /* yacc.c:1646  */
    { compile_string_array((yyvsp[-6].text), (yyvsp[-4].text), (yyvsp[-2].numb), (yyvsp[-1].numb)); }
#line 2308 "parse.cc" /* yacc.c:1646  */
    break;

  case 31:
#line 241 "parse.y" /* yacc.c:1646  */
    { compile_object_array((yyvsp[-6].text), (yyvsp[-4].text), (yyvsp[-2].numb), (yyvsp[-1].numb)); }
#line 2314 "parse.cc" /* yacc.c:1646  */
    break;

  case 32:
#line 244 "parse.y" /* yacc.c:1646  */
    { compile_net_array((yyvsp[-6].text), (yyvsp[-4].text), (yyvsp[-2].numb), (yyvsp[-1].numb)); }
#line 2320 "parse.cc" /* yacc.c:1646  */
    break;

  case 33:
#line 247 "parse.y" /* yacc.c:1646  */
    { compile_array_port((yyvsp[-5].text), (yyvsp[-3].text), (yyvsp[-1].text)); }
#line 2326 "parse.cc" /* yacc.c:1646  */
    break;

  case 34:
#line 250 "parse.y" /* yacc.c:1646  */
    { compile_array_port((yyvsp[-5].text), (yyvsp[-3].text), (yyvsp[-1].numb)); }
#line 2332 "parse.cc" /* yacc.c:1646  */
    break;

  case 35:
#line 253 "parse.y" /* yacc.c:1646  */
    { compile_array_alias((yyvsp[-5].text), (yyvsp[-3].text), (yyvsp[-1].text)); }
#line 2338 "parse.cc" /* yacc.c:1646  */
    break;

  case 36:
#line 260 "parse.y" /* yacc.c:1646  */
    { compile_ufunc_real((yyvsp[-11].text), (yyvsp[-9].text), (yyvsp[-7].numb), (yyvsp[-5].symbv).cnt, (yyvsp[-5].symbv).vect, (yyvsp[-3].symbv).cnt, (yyvsp[-3].symbv).vect, (yyvsp[-1].text), 0); }
#line 2344 "parse.cc" /* yacc.c:1646  */
    break;

  case 37:
#line 263 "parse.y" /* yacc.c:1646  */
    { compile_ufunc_vec4((yyvsp[-11].text), (yyvsp[-9].text), (yyvsp[-7].numb), (yyvsp[-5].symbv).cnt, (yyvsp[-5].symbv).vect, (yyvsp[-3].symbv).cnt, (yyvsp[-3].symbv).vect, (yyvsp[-1].text), 0); }
#line 2350 "parse.cc" /* yacc.c:1646  */
    break;

  case 38:
#line 266 "parse.y" /* yacc.c:1646  */
    { compile_ufunc_vec4((yyvsp[-13].text), (yyvsp[-11].text), (yyvsp[-9].numb), (yyvsp[-5].symbv).cnt, (yyvsp[-5].symbv).vect, (yyvsp[-3].symbv).cnt, (yyvsp[-3].symbv).vect, (yyvsp[-1].text), (yyvsp[-7].text)); }
#line 2356 "parse.cc" /* yacc.c:1646  */
    break;

  case 39:
#line 272 "parse.y" /* yacc.c:1646  */
    { struct symbv_s obj = (yyvsp[-1].symbv);
		  compile_resolver((yyvsp[-5].text), (yyvsp[-3].text), obj.cnt, obj.vect);
		}
#line 2364 "parse.cc" /* yacc.c:1646  */
    break;

  case 40:
#line 280 "parse.y" /* yacc.c:1646  */
    { compile_part_select((yyvsp[-7].text), (yyvsp[-5].text), (yyvsp[-3].numb), (yyvsp[-1].numb)); }
#line 2370 "parse.cc" /* yacc.c:1646  */
    break;

  case 41:
#line 283 "parse.y" /* yacc.c:1646  */
    { compile_part_select_pv((yyvsp[-9].text), (yyvsp[-7].text), (yyvsp[-5].numb), (yyvsp[-3].numb), (yyvsp[-1].numb)); }
#line 2376 "parse.cc" /* yacc.c:1646  */
    break;

  case 42:
#line 286 "parse.y" /* yacc.c:1646  */
    { compile_part_select_var((yyvsp[-7].text), (yyvsp[-5].text), (yyvsp[-3].text), (yyvsp[-1].numb), false); }
#line 2382 "parse.cc" /* yacc.c:1646  */
    break;

  case 43:
#line 288 "parse.y" /* yacc.c:1646  */
    { compile_part_select_var((yyvsp[-7].text), (yyvsp[-5].text), (yyvsp[-3].text), (yyvsp[-1].numb), true); }
#line 2388 "parse.cc" /* yacc.c:1646  */
    break;

  case 44:
#line 294 "parse.y" /* yacc.c:1646  */
    { compile_concat((yyvsp[-10].text), (yyvsp[-7].numb), (yyvsp[-6].numb), (yyvsp[-5].numb), (yyvsp[-4].numb), (yyvsp[-1].symbv).cnt, (yyvsp[-1].symbv).vect); }
#line 2394 "parse.cc" /* yacc.c:1646  */
    break;

  case 45:
#line 298 "parse.y" /* yacc.c:1646  */
    { compile_concat8((yyvsp[-10].text), (yyvsp[-7].numb), (yyvsp[-6].numb), (yyvsp[-5].numb), (yyvsp[-4].numb), (yyvsp[-1].symbv).cnt, (yyvsp[-1].symbv).vect); }
#line 2400 "parse.cc" /* yacc.c:1646  */
    break;

  case 46:
#line 302 "parse.y" /* yacc.c:1646  */
    { compile_substitute((yyvsp[-8].text), (yyvsp[-6].numb), (yyvsp[-4].numb), (yyvsp[-3].numb), (yyvsp[-1].symbv).cnt, (yyvsp[-1].symbv).vect); }
#line 2406 "parse.cc" /* yacc.c:1646  */
    break;

  case 47:
#line 308 "parse.y" /* yacc.c:1646  */
    { struct symbv_s obj = (yyvsp[-1].symbv);
		  compile_arith_abs((yyvsp[-3].text), obj.cnt, obj.vect);
		}
#line 2414 "parse.cc" /* yacc.c:1646  */
    break;

  case 48:
#line 313 "parse.y" /* yacc.c:1646  */
    { struct symbv_s obj = (yyvsp[-1].symbv);
	compile_arith_cast_int((yyvsp[-5].text), (yyvsp[-3].numb), obj.cnt, obj.vect);
      }
#line 2422 "parse.cc" /* yacc.c:1646  */
    break;

  case 49:
#line 318 "parse.y" /* yacc.c:1646  */
    { struct symbv_s obj = (yyvsp[-1].symbv);
	compile_arith_cast_real((yyvsp[-3].text), false, obj.cnt, obj.vect);
      }
#line 2430 "parse.cc" /* yacc.c:1646  */
    break;

  case 50:
#line 323 "parse.y" /* yacc.c:1646  */
    { struct symbv_s obj = (yyvsp[-1].symbv);
	compile_arith_cast_real((yyvsp[-3].text), true, obj.cnt, obj.vect);
      }
#line 2438 "parse.cc" /* yacc.c:1646  */
    break;

  case 51:
#line 328 "parse.y" /* yacc.c:1646  */
    { struct symbv_s obj = (yyvsp[-1].symbv);
	compile_arith_cast_vec2((yyvsp[-5].text), (yyvsp[-3].numb), obj.cnt, obj.vect);
      }
#line 2446 "parse.cc" /* yacc.c:1646  */
    break;

  case 52:
#line 336 "parse.y" /* yacc.c:1646  */
    { struct symbv_s obj = (yyvsp[-1].symbv);
		  compile_arith_div((yyvsp[-5].text), (yyvsp[-3].numb), false, obj.cnt, obj.vect);
		}
#line 2454 "parse.cc" /* yacc.c:1646  */
    break;

  case 53:
#line 341 "parse.y" /* yacc.c:1646  */
    { struct symbv_s obj = (yyvsp[-1].symbv);
		  compile_arith_div_r((yyvsp[-5].text), obj.cnt, obj.vect);
		}
#line 2462 "parse.cc" /* yacc.c:1646  */
    break;

  case 54:
#line 346 "parse.y" /* yacc.c:1646  */
    { struct symbv_s obj = (yyvsp[-1].symbv);
		  compile_arith_div((yyvsp[-5].text), (yyvsp[-3].numb), true, obj.cnt, obj.vect);
		}
#line 2470 "parse.cc" /* yacc.c:1646  */
    break;

  case 55:
#line 351 "parse.y" /* yacc.c:1646  */
    { struct symbv_s obj = (yyvsp[-1].symbv);
		  compile_arith_mod((yyvsp[-5].text), (yyvsp[-3].numb), false, obj.cnt, obj.vect);
		}
#line 2478 "parse.cc" /* yacc.c:1646  */
    break;

  case 56:
#line 356 "parse.y" /* yacc.c:1646  */
    { struct symbv_s obj = (yyvsp[-1].symbv);
		  compile_arith_mod_r((yyvsp[-5].text), obj.cnt, obj.vect);
		}
#line 2486 "parse.cc" /* yacc.c:1646  */
    break;

  case 57:
#line 361 "parse.y" /* yacc.c:1646  */
    { struct symbv_s obj = (yyvsp[-1].symbv);
		  compile_arith_mod((yyvsp[-5].text), (yyvsp[-3].numb), true, obj.cnt, obj.vect);
		}
#line 2494 "parse.cc" /* yacc.c:1646  */
    break;

  case 58:
#line 366 "parse.y" /* yacc.c:1646  */
    { struct symbv_s obj = (yyvsp[-1].symbv);
		  compile_arith_mult((yyvsp[-5].text), (yyvsp[-3].numb), obj.cnt, obj.vect);
		}
#line 2502 "parse.cc" /* yacc.c:1646  */
    break;

  case 59:
#line 371 "parse.y" /* yacc.c:1646  */
    { struct symbv_s obj = (yyvsp[-1].symbv);
		  compile_arith_mult_r((yyvsp[-5].text), obj.cnt, obj.vect);
		}
#line 2510 "parse.cc" /* yacc.c:1646  */
    break;

  case 60:
#line 376 "parse.y" /* yacc.c:1646  */
    { struct symbv_s obj = (yyvsp[-1].symbv);
		  compile_arith_pow((yyvsp[-5].text), (yyvsp[-3].numb), false, obj.cnt, obj.vect);
		}
#line 2518 "parse.cc" /* yacc.c:1646  */
    break;

  case 61:
#line 381 "parse.y" /* yacc.c:1646  */
    { struct symbv_s obj = (yyvsp[-1].symbv);
		  compile_arith_pow_r((yyvsp[-5].text), obj.cnt, obj.vect);
		}
#line 2526 "parse.cc" /* yacc.c:1646  */
    break;

  case 62:
#line 386 "parse.y" /* yacc.c:1646  */
    { struct symbv_s obj = (yyvsp[-1].symbv);
		  compile_arith_pow((yyvsp[-5].text), (yyvsp[-3].numb), true, obj.cnt, obj.vect);
		}
#line 2534 "parse.cc" /* yacc.c:1646  */
    break;

  case 63:
#line 391 "parse.y" /* yacc.c:1646  */
    { struct symbv_s obj = (yyvsp[-1].symbv);
		  compile_arith_sub((yyvsp[-5].text), (yyvsp[-3].numb), obj.cnt, obj.vect);
		}
#line 2542 "parse.cc" /* yacc.c:1646  */
    break;

  case 64:
#line 396 "parse.y" /* yacc.c:1646  */
    { struct symbv_s obj = (yyvsp[-1].symbv);
		  compile_arith_sub_r((yyvsp[-5].text), obj.cnt, obj.vect);
		}
#line 2550 "parse.cc" /* yacc.c:1646  */
    break;

  case 65:
#line 401 "parse.y" /* yacc.c:1646  */
    { struct symbv_s obj = (yyvsp[-1].symbv);
		  compile_arith_sum((yyvsp[-5].text), (yyvsp[-3].numb), obj.cnt, obj.vect);
		}
#line 2558 "parse.cc" /* yacc.c:1646  */
    break;

  case 66:
#line 406 "parse.y" /* yacc.c:1646  */
    { struct symbv_s obj = (yyvsp[-1].symbv);
		  compile_arith_sum_r((yyvsp[-5].text), obj.cnt, obj.vect);
		}
#line 2566 "parse.cc" /* yacc.c:1646  */
    break;

  case 67:
#line 411 "parse.y" /* yacc.c:1646  */
    { struct symbv_s obj = (yyvsp[-1].symbv);
		  compile_cmp_eeq((yyvsp[-5].text), (yyvsp[-3].numb), obj.cnt, obj.vect);
		}
#line 2574 "parse.cc" /* yacc.c:1646  */
    break;

  case 68:
#line 416 "parse.y" /* yacc.c:1646  */
    { struct symbv_s obj = (yyvsp[-1].symbv);
		  compile_cmp_nee((yyvsp[-5].text), (yyvsp[-3].numb), obj.cnt, obj.vect);
		}
#line 2582 "parse.cc" /* yacc.c:1646  */
    break;

  case 69:
#line 421 "parse.y" /* yacc.c:1646  */
    { struct symbv_s obj = (yyvsp[-1].symbv);
		  compile_cmp_eq((yyvsp[-5].text), (yyvsp[-3].numb), obj.cnt, obj.vect);
		}
#line 2590 "parse.cc" /* yacc.c:1646  */
    break;

  case 70:
#line 426 "parse.y" /* yacc.c:1646  */
    { struct symbv_s obj = (yyvsp[-1].symbv);
		  compile_cmp_eqx((yyvsp[-5].text), (yyvsp[-3].numb), obj.cnt, obj.vect);
		}
#line 2598 "parse.cc" /* yacc.c:1646  */
    break;

  case 71:
#line 431 "parse.y" /* yacc.c:1646  */
    { struct symbv_s obj = (yyvsp[-1].symbv);
		  compile_cmp_eqz((yyvsp[-5].text), (yyvsp[-3].numb), obj.cnt, obj.vect);
		}
#line 2606 "parse.cc" /* yacc.c:1646  */
    break;

  case 72:
#line 436 "parse.y" /* yacc.c:1646  */
    { struct symbv_s obj = (yyvsp[-1].symbv);
		  compile_cmp_eq_r((yyvsp[-5].text), obj.cnt, obj.vect);
		}
#line 2614 "parse.cc" /* yacc.c:1646  */
    break;

  case 73:
#line 441 "parse.y" /* yacc.c:1646  */
    { struct symbv_s obj = (yyvsp[-1].symbv);
		  compile_cmp_ne((yyvsp[-5].text), (yyvsp[-3].numb), obj.cnt, obj.vect);
		}
#line 2622 "parse.cc" /* yacc.c:1646  */
    break;

  case 74:
#line 446 "parse.y" /* yacc.c:1646  */
    { struct symbv_s obj = (yyvsp[-1].symbv);
		  compile_cmp_ne_r((yyvsp[-5].text), obj.cnt, obj.vect);
		}
#line 2630 "parse.cc" /* yacc.c:1646  */
    break;

  case 75:
#line 451 "parse.y" /* yacc.c:1646  */
    { struct symbv_s obj = (yyvsp[-1].symbv);
		  compile_cmp_ge((yyvsp[-5].text), (yyvsp[-3].numb), false, obj.cnt, obj.vect);
		}
#line 2638 "parse.cc" /* yacc.c:1646  */
    break;

  case 76:
#line 456 "parse.y" /* yacc.c:1646  */
    { struct symbv_s obj = (yyvsp[-1].symbv);
		  compile_cmp_ge_r((yyvsp[-5].text), obj.cnt, obj.vect);
		}
#line 2646 "parse.cc" /* yacc.c:1646  */
    break;

  case 77:
#line 461 "parse.y" /* yacc.c:1646  */
    { struct symbv_s obj = (yyvsp[-1].symbv);
		  compile_cmp_ge((yyvsp[-5].text), (yyvsp[-3].numb), true, obj.cnt, obj.vect);
		}
#line 2654 "parse.cc" /* yacc.c:1646  */
    break;

  case 78:
#line 466 "parse.y" /* yacc.c:1646  */
    { struct symbv_s obj = (yyvsp[-1].symbv);
		  compile_cmp_gt((yyvsp[-5].text), (yyvsp[-3].numb), false, obj.cnt, obj.vect);
		}
#line 2662 "parse.cc" /* yacc.c:1646  */
    break;

  case 79:
#line 471 "parse.y" /* yacc.c:1646  */
    { struct symbv_s obj = (yyvsp[-1].symbv);
		  compile_cmp_gt_r((yyvsp[-5].text), obj.cnt, obj.vect);
		}
#line 2670 "parse.cc" /* yacc.c:1646  */
    break;

  case 80:
#line 476 "parse.y" /* yacc.c:1646  */
    { struct symbv_s obj = (yyvsp[-1].symbv);
		  compile_cmp_gt((yyvsp[-5].text), (yyvsp[-3].numb), true, obj.cnt, obj.vect);
		}
#line 2678 "parse.cc" /* yacc.c:1646  */
    break;

  case 81:
#line 481 "parse.y" /* yacc.c:1646  */
    { struct symbv_s obj = (yyvsp[-1].symbv);
		  compile_cmp_weq((yyvsp[-5].text), (yyvsp[-3].numb), obj.cnt, obj.vect);
		}
#line 2686 "parse.cc" /* yacc.c:1646  */
    break;

  case 82:
#line 486 "parse.y" /* yacc.c:1646  */
    { struct symbv_s obj = (yyvsp[-1].symbv);
		  compile_cmp_wne((yyvsp[-5].text), (yyvsp[-3].numb), obj.cnt, obj.vect);
		}
#line 2694 "parse.cc" /* yacc.c:1646  */
    break;

  case 83:
#line 495 "parse.y" /* yacc.c:1646  */
    { compile_delay((yyvsp[-5].text), (yyvsp[-3].numb), (yyvsp[-2].cdelay), (yyvsp[-1].symb)); }
#line 2700 "parse.cc" /* yacc.c:1646  */
    break;

  case 84:
#line 497 "parse.y" /* yacc.c:1646  */
    { struct symbv_s obj = (yyvsp[-1].symbv);
      compile_delay((yyvsp[-4].text), (yyvsp[-2].numb), obj.cnt, obj.vect, false);
    }
#line 2708 "parse.cc" /* yacc.c:1646  */
    break;

  case 85:
#line 501 "parse.y" /* yacc.c:1646  */
    { struct symbv_s obj = (yyvsp[-3].symbv);
      if ((yyvsp[-1].numb) != 0) assert(0);
      compile_delay((yyvsp[-6].text), (yyvsp[-4].numb), obj.cnt, obj.vect, true);
    }
#line 2717 "parse.cc" /* yacc.c:1646  */
    break;

  case 86:
#line 507 "parse.y" /* yacc.c:1646  */
    { modpath_dst = compile_modpath((yyvsp[-5].text), (yyvsp[-3].numb), (yyvsp[-2].symb), (yyvsp[-1].symb)); }
#line 2723 "parse.cc" /* yacc.c:1646  */
    break;

  case 87:
#line 509 "parse.y" /* yacc.c:1646  */
    { modpath_dst = 0; }
#line 2729 "parse.cc" /* yacc.c:1646  */
    break;

  case 88:
#line 514 "parse.y" /* yacc.c:1646  */
    { compile_dff((yyvsp[-8].text), (yyvsp[-6].numb), true, (yyvsp[-5].symb), (yyvsp[-3].symb), (yyvsp[-1].symb)); }
#line 2735 "parse.cc" /* yacc.c:1646  */
    break;

  case 89:
#line 517 "parse.y" /* yacc.c:1646  */
    { compile_dff((yyvsp[-8].text), (yyvsp[-6].numb), false, (yyvsp[-5].symb), (yyvsp[-3].symb), (yyvsp[-1].symb)); }
#line 2741 "parse.cc" /* yacc.c:1646  */
    break;

  case 90:
#line 520 "parse.y" /* yacc.c:1646  */
    { compile_dff_aclr((yyvsp[-10].text), (yyvsp[-8].numb), true, (yyvsp[-7].symb), (yyvsp[-5].symb), (yyvsp[-3].symb), (yyvsp[-1].symb)); }
#line 2747 "parse.cc" /* yacc.c:1646  */
    break;

  case 91:
#line 523 "parse.y" /* yacc.c:1646  */
    { compile_dff_aclr((yyvsp[-10].text), (yyvsp[-8].numb), false, (yyvsp[-7].symb), (yyvsp[-5].symb), (yyvsp[-3].symb), (yyvsp[-1].symb)); }
#line 2753 "parse.cc" /* yacc.c:1646  */
    break;

  case 92:
#line 526 "parse.y" /* yacc.c:1646  */
    { compile_dff_aset((yyvsp[-10].text), (yyvsp[-8].numb), true, (yyvsp[-7].symb), (yyvsp[-5].symb), (yyvsp[-3].symb), (yyvsp[-1].symb), 0); }
#line 2759 "parse.cc" /* yacc.c:1646  */
    break;

  case 93:
#line 529 "parse.y" /* yacc.c:1646  */
    { compile_dff_aset((yyvsp[-10].text), (yyvsp[-8].numb), false, (yyvsp[-7].symb), (yyvsp[-5].symb), (yyvsp[-3].symb), (yyvsp[-1].symb), 0); }
#line 2765 "parse.cc" /* yacc.c:1646  */
    break;

  case 94:
#line 532 "parse.y" /* yacc.c:1646  */
    { compile_dff_aset((yyvsp[-12].text), (yyvsp[-10].numb), true, (yyvsp[-9].symb), (yyvsp[-7].symb), (yyvsp[-5].symb), (yyvsp[-3].symb), (yyvsp[-1].text)); }
#line 2771 "parse.cc" /* yacc.c:1646  */
    break;

  case 95:
#line 535 "parse.y" /* yacc.c:1646  */
    { compile_dff_aset((yyvsp[-12].text), (yyvsp[-10].numb), false, (yyvsp[-9].symb), (yyvsp[-7].symb), (yyvsp[-5].symb), (yyvsp[-3].symb), (yyvsp[-1].text)); }
#line 2777 "parse.cc" /* yacc.c:1646  */
    break;

  case 96:
#line 540 "parse.y" /* yacc.c:1646  */
    { compile_latch((yyvsp[-6].text), (yyvsp[-4].numb), (yyvsp[-3].symb), (yyvsp[-1].symb)); }
#line 2783 "parse.cc" /* yacc.c:1646  */
    break;

  case 97:
#line 545 "parse.y" /* yacc.c:1646  */
    { compile_reduce_and((yyvsp[-3].text), (yyvsp[-1].symb)); }
#line 2789 "parse.cc" /* yacc.c:1646  */
    break;

  case 98:
#line 548 "parse.y" /* yacc.c:1646  */
    { compile_reduce_or((yyvsp[-3].text), (yyvsp[-1].symb)); }
#line 2795 "parse.cc" /* yacc.c:1646  */
    break;

  case 99:
#line 551 "parse.y" /* yacc.c:1646  */
    { compile_reduce_xor((yyvsp[-3].text), (yyvsp[-1].symb)); }
#line 2801 "parse.cc" /* yacc.c:1646  */
    break;

  case 100:
#line 554 "parse.y" /* yacc.c:1646  */
    { compile_reduce_nand((yyvsp[-3].text), (yyvsp[-1].symb)); }
#line 2807 "parse.cc" /* yacc.c:1646  */
    break;

  case 101:
#line 557 "parse.y" /* yacc.c:1646  */
    { compile_reduce_nor((yyvsp[-3].text), (yyvsp[-1].symb)); }
#line 2813 "parse.cc" /* yacc.c:1646  */
    break;

  case 102:
#line 560 "parse.y" /* yacc.c:1646  */
    { compile_reduce_xnor((yyvsp[-3].text), (yyvsp[-1].symb)); }
#line 2819 "parse.cc" /* yacc.c:1646  */
    break;

  case 103:
#line 563 "parse.y" /* yacc.c:1646  */
    { compile_repeat((yyvsp[-7].text), (yyvsp[-5].numb), (yyvsp[-3].numb), (yyvsp[-1].symb)); }
#line 2825 "parse.cc" /* yacc.c:1646  */
    break;

  case 104:
#line 568 "parse.y" /* yacc.c:1646  */
    { compile_extend_signed((yyvsp[-5].text), (yyvsp[-3].numb), (yyvsp[-1].symb)); }
#line 2831 "parse.cc" /* yacc.c:1646  */
    break;

  case 105:
#line 573 "parse.y" /* yacc.c:1646  */
    { compile_sfunc((yyvsp[-9].text), (yyvsp[-5].text), (yyvsp[-3].text), (yyvsp[-7].numb), (yyvsp[-6].numb), (yyvsp[-1].symbv).cnt, (yyvsp[-1].symbv).vect, 0); }
#line 2837 "parse.cc" /* yacc.c:1646  */
    break;

  case 106:
#line 577 "parse.y" /* yacc.c:1646  */
    { compile_sfunc((yyvsp[-11].text), (yyvsp[-7].text), (yyvsp[-3].text), (yyvsp[-9].numb), (yyvsp[-8].numb), (yyvsp[-1].symbv).cnt, (yyvsp[-1].symbv).vect, (yyvsp[-5].text)); }
#line 2843 "parse.cc" /* yacc.c:1646  */
    break;

  case 107:
#line 582 "parse.y" /* yacc.c:1646  */
    { compile_sfunc((yyvsp[-7].text), (yyvsp[-3].text), (yyvsp[-1].text), (yyvsp[-5].numb), (yyvsp[-4].numb), 0, 0, 0); }
#line 2849 "parse.cc" /* yacc.c:1646  */
    break;

  case 108:
#line 586 "parse.y" /* yacc.c:1646  */
    { compile_sfunc((yyvsp[-9].text), (yyvsp[-5].text), (yyvsp[-1].text), (yyvsp[-7].numb), (yyvsp[-6].numb), 0, 0, (yyvsp[-3].text)); }
#line 2855 "parse.cc" /* yacc.c:1646  */
    break;

  case 109:
#line 591 "parse.y" /* yacc.c:1646  */
    { struct symbv_s obj = (yyvsp[-1].symbv);
		  compile_shiftl((yyvsp[-5].text), (yyvsp[-3].numb), obj.cnt, obj.vect);
		}
#line 2863 "parse.cc" /* yacc.c:1646  */
    break;

  case 110:
#line 596 "parse.y" /* yacc.c:1646  */
    { struct symbv_s obj = (yyvsp[-1].symbv);
		      compile_shiftr((yyvsp[-5].text), (yyvsp[-3].numb), false, obj.cnt, obj.vect);
		}
#line 2871 "parse.cc" /* yacc.c:1646  */
    break;

  case 111:
#line 602 "parse.y" /* yacc.c:1646  */
    { struct symbv_s obj = (yyvsp[-1].symbv);
		      compile_shiftr((yyvsp[-5].text), (yyvsp[-3].numb), true, obj.cnt, obj.vect);
		}
#line 2879 "parse.cc" /* yacc.c:1646  */
    break;

  case 112:
#line 612 "parse.y" /* yacc.c:1646  */
    { compile_event((yyvsp[-5].text), (yyvsp[-3].text), (yyvsp[-1].symbv).cnt, (yyvsp[-1].symbv).vect); }
#line 2885 "parse.cc" /* yacc.c:1646  */
    break;

  case 113:
#line 615 "parse.y" /* yacc.c:1646  */
    { compile_event((yyvsp[-6].text), (yyvsp[-3].text), (yyvsp[-1].symbv).cnt, (yyvsp[-1].symbv).vect); }
#line 2891 "parse.cc" /* yacc.c:1646  */
    break;

  case 114:
#line 618 "parse.y" /* yacc.c:1646  */
    { compile_named_event((yyvsp[-3].text), (yyvsp[-1].text)); }
#line 2897 "parse.cc" /* yacc.c:1646  */
    break;

  case 115:
#line 621 "parse.y" /* yacc.c:1646  */
    { compile_event((yyvsp[-3].text), 0, (yyvsp[-1].symbv).cnt, (yyvsp[-1].symbv).vect); }
#line 2903 "parse.cc" /* yacc.c:1646  */
    break;

  case 116:
#line 629 "parse.y" /* yacc.c:1646  */
    { compile_code((yyvsp[-3].text), (yyvsp[-2].text), (yyvsp[-1].opa)); }
#line 2909 "parse.cc" /* yacc.c:1646  */
    break;

  case 117:
#line 632 "parse.y" /* yacc.c:1646  */
    { compile_codelabel((yyvsp[-1].text)); }
#line 2915 "parse.cc" /* yacc.c:1646  */
    break;

  case 118:
#line 637 "parse.y" /* yacc.c:1646  */
    { compile_file_line((yyvsp[-5].text), (yyvsp[-3].numb), (yyvsp[-2].numb), (yyvsp[-1].text)); }
#line 2921 "parse.cc" /* yacc.c:1646  */
    break;

  case 119:
#line 640 "parse.y" /* yacc.c:1646  */
    { assert((yyvsp[-1].numb) == 0);
		  compile_file_line((yyvsp[-5].text), (yyvsp[-3].numb), (yyvsp[-2].numb), 0); }
#line 2928 "parse.cc" /* yacc.c:1646  */
    break;

  case 120:
#line 651 "parse.y" /* yacc.c:1646  */
    { compile_vpi_call((yyvsp[-11].text), (yyvsp[-7].text), true, false, (yyvsp[-9].numb), (yyvsp[-8].numb),
			 (yyvsp[-6].argv).argc, (yyvsp[-6].argv).argv, (yyvsp[-4].numb), (yyvsp[-3].numb), (yyvsp[-2].numb)); }
#line 2935 "parse.cc" /* yacc.c:1646  */
    break;

  case 121:
#line 658 "parse.y" /* yacc.c:1646  */
    { compile_vpi_call((yyvsp[-11].text), (yyvsp[-7].text), false, true, (yyvsp[-9].numb), (yyvsp[-8].numb),
			 (yyvsp[-6].argv).argc, (yyvsp[-6].argv).argv, (yyvsp[-4].numb), (yyvsp[-3].numb), (yyvsp[-2].numb)); }
#line 2942 "parse.cc" /* yacc.c:1646  */
    break;

  case 122:
#line 665 "parse.y" /* yacc.c:1646  */
    { compile_vpi_call((yyvsp[-11].text), (yyvsp[-7].text), false, false, (yyvsp[-9].numb), (yyvsp[-8].numb),
			 (yyvsp[-6].argv).argc, (yyvsp[-6].argv).argv, (yyvsp[-4].numb), (yyvsp[-3].numb), (yyvsp[-2].numb)); }
#line 2949 "parse.cc" /* yacc.c:1646  */
    break;

  case 123:
#line 670 "parse.y" /* yacc.c:1646  */
    { compile_vpi_func_call((yyvsp[-12].text), (yyvsp[-8].text), -vpiVectorVal, (yyvsp[-7].numb), (yyvsp[-10].numb), (yyvsp[-9].numb),
			      (yyvsp[-6].argv).argc, (yyvsp[-6].argv).argv, (yyvsp[-4].numb), (yyvsp[-3].numb), (yyvsp[-2].numb)); }
#line 2956 "parse.cc" /* yacc.c:1646  */
    break;

  case 124:
#line 675 "parse.y" /* yacc.c:1646  */
    { compile_vpi_func_call((yyvsp[-11].text), (yyvsp[-7].text), -vpiRealVal, 0, (yyvsp[-9].numb), (yyvsp[-8].numb),
			      (yyvsp[-6].argv).argc, (yyvsp[-6].argv).argv, (yyvsp[-4].numb), (yyvsp[-3].numb), (yyvsp[-2].numb)); }
#line 2963 "parse.cc" /* yacc.c:1646  */
    break;

  case 125:
#line 680 "parse.y" /* yacc.c:1646  */
    { compile_vpi_func_call((yyvsp[-11].text), (yyvsp[-7].text), -vpiStringVal, 0, (yyvsp[-9].numb), (yyvsp[-8].numb),
			      (yyvsp[-6].argv).argc, (yyvsp[-6].argv).argv, (yyvsp[-4].numb), (yyvsp[-3].numb), (yyvsp[-2].numb)); }
#line 2970 "parse.cc" /* yacc.c:1646  */
    break;

  case 126:
#line 699 "parse.y" /* yacc.c:1646  */
    { compile_scope_decl((yyvsp[-8].text), (yyvsp[-6].text), (yyvsp[-4].text), (yyvsp[-3].text), 0, (yyvsp[-2].numb), (yyvsp[-1].numb), (yyvsp[-2].numb), (yyvsp[-1].numb), 0); }
#line 2976 "parse.cc" /* yacc.c:1646  */
    break;

  case 127:
#line 703 "parse.y" /* yacc.c:1646  */
    { compile_scope_decl((yyvsp[-14].text), (yyvsp[-12].text), (yyvsp[-10].text), (yyvsp[-9].text), (yyvsp[-1].text), (yyvsp[-8].numb), (yyvsp[-7].numb), (yyvsp[-5].numb), (yyvsp[-4].numb), (yyvsp[-3].numb)); }
#line 2982 "parse.cc" /* yacc.c:1646  */
    break;

  case 128:
#line 709 "parse.y" /* yacc.c:1646  */
    { compile_scope_recall((yyvsp[-1].text)); }
#line 2988 "parse.cc" /* yacc.c:1646  */
    break;

  case 129:
#line 715 "parse.y" /* yacc.c:1646  */
    { compile_port_info( (yyvsp[-4].numb) /* port_index */, (yyvsp[-3].vpi_enum), (yyvsp[-2].numb) /* width */,
		                     (yyvsp[-1].text) /*&name */ ); }
#line 2995 "parse.cc" /* yacc.c:1646  */
    break;

  case 130:
#line 720 "parse.y" /* yacc.c:1646  */
    { compile_timescale((yyvsp[-2].numb), (yyvsp[-1].numb)); }
#line 3001 "parse.cc" /* yacc.c:1646  */
    break;

  case 131:
#line 722 "parse.y" /* yacc.c:1646  */
    { compile_timescale(-(yyvsp[-2].numb), (yyvsp[-1].numb)); }
#line 3007 "parse.cc" /* yacc.c:1646  */
    break;

  case 132:
#line 724 "parse.y" /* yacc.c:1646  */
    { compile_timescale((yyvsp[-3].numb), -(yyvsp[-1].numb)); }
#line 3013 "parse.cc" /* yacc.c:1646  */
    break;

  case 133:
#line 726 "parse.y" /* yacc.c:1646  */
    { compile_timescale(-(yyvsp[-3].numb), -(yyvsp[-1].numb)); }
#line 3019 "parse.cc" /* yacc.c:1646  */
    break;

  case 134:
#line 733 "parse.y" /* yacc.c:1646  */
    { compile_thread((yyvsp[-1].text), 0); }
#line 3025 "parse.cc" /* yacc.c:1646  */
    break;

  case 135:
#line 736 "parse.y" /* yacc.c:1646  */
    { compile_thread((yyvsp[-3].text), (yyvsp[-1].text)); }
#line 3031 "parse.cc" /* yacc.c:1646  */
    break;

  case 136:
#line 743 "parse.y" /* yacc.c:1646  */
    { compile_variable((yyvsp[-7].text), (yyvsp[-4].text), (yyvsp[-2].numb), (yyvsp[-1].numb), vpiLogicVar, false, (yyvsp[-5].flag)); }
#line 3037 "parse.cc" /* yacc.c:1646  */
    break;

  case 137:
#line 746 "parse.y" /* yacc.c:1646  */
    { compile_variable((yyvsp[-7].text), (yyvsp[-4].text), (yyvsp[-2].numb), (yyvsp[-1].numb), vpiLogicVar, true, (yyvsp[-5].flag)); }
#line 3043 "parse.cc" /* yacc.c:1646  */
    break;

  case 138:
#line 749 "parse.y" /* yacc.c:1646  */
    { compile_variable((yyvsp[-7].text), (yyvsp[-4].text), (yyvsp[-2].numb), (yyvsp[-1].numb), vpiIntegerVar, true, (yyvsp[-5].flag)); }
#line 3049 "parse.cc" /* yacc.c:1646  */
    break;

  case 139:
#line 752 "parse.y" /* yacc.c:1646  */
    { compile_variable((yyvsp[-7].text), (yyvsp[-4].text), (yyvsp[-2].numb), (yyvsp[-1].numb), vpiIntVar, true, (yyvsp[-5].flag)); }
#line 3055 "parse.cc" /* yacc.c:1646  */
    break;

  case 140:
#line 755 "parse.y" /* yacc.c:1646  */
    { compile_variable((yyvsp[-7].text), (yyvsp[-4].text), (yyvsp[-2].numb), (yyvsp[-1].numb), vpiIntVar, false, (yyvsp[-5].flag)); }
#line 3061 "parse.cc" /* yacc.c:1646  */
    break;

  case 141:
#line 758 "parse.y" /* yacc.c:1646  */
    { compile_var_real((yyvsp[-6].text), (yyvsp[-4].text)); }
#line 3067 "parse.cc" /* yacc.c:1646  */
    break;

  case 142:
#line 761 "parse.y" /* yacc.c:1646  */
    { compile_var_string((yyvsp[-3].text), (yyvsp[-1].text)); }
#line 3073 "parse.cc" /* yacc.c:1646  */
    break;

  case 143:
#line 764 "parse.y" /* yacc.c:1646  */
    { compile_var_darray((yyvsp[-5].text), (yyvsp[-3].text), (yyvsp[-1].numb)); }
#line 3079 "parse.cc" /* yacc.c:1646  */
    break;

  case 144:
#line 767 "parse.y" /* yacc.c:1646  */
    { compile_var_queue((yyvsp[-5].text), (yyvsp[-3].text), (yyvsp[-1].numb)); }
#line 3085 "parse.cc" /* yacc.c:1646  */
    break;

  case 145:
#line 770 "parse.y" /* yacc.c:1646  */
    { compile_var_cobject((yyvsp[-3].text), (yyvsp[-1].text)); }
#line 3091 "parse.cc" /* yacc.c:1646  */
    break;

  case 146:
#line 777 "parse.y" /* yacc.c:1646  */
    { compile_net((yyvsp[-9].text), (yyvsp[-6].text), (yyvsp[-4].numb), (yyvsp[-3].numb), vpiLogicVar, false, (yyvsp[-7].flag), (yyvsp[-1].symbv).cnt, (yyvsp[-1].symbv).vect); }
#line 3097 "parse.cc" /* yacc.c:1646  */
    break;

  case 147:
#line 781 "parse.y" /* yacc.c:1646  */
    { compile_net((yyvsp[-9].text), (yyvsp[-6].text), (yyvsp[-4].numb), (yyvsp[-3].numb), vpiLogicVar, true, (yyvsp[-7].flag), (yyvsp[-1].symbv).cnt, (yyvsp[-1].symbv).vect); }
#line 3103 "parse.cc" /* yacc.c:1646  */
    break;

  case 148:
#line 785 "parse.y" /* yacc.c:1646  */
    { compile_net((yyvsp[-9].text), (yyvsp[-6].text), (yyvsp[-4].numb), (yyvsp[-3].numb), vpiIntVar, false, (yyvsp[-7].flag), (yyvsp[-1].symbv).cnt, (yyvsp[-1].symbv).vect); }
#line 3109 "parse.cc" /* yacc.c:1646  */
    break;

  case 149:
#line 789 "parse.y" /* yacc.c:1646  */
    { compile_net((yyvsp[-9].text), (yyvsp[-6].text), (yyvsp[-4].numb), (yyvsp[-3].numb), vpiIntVar, true, (yyvsp[-7].flag), (yyvsp[-1].symbv).cnt, (yyvsp[-1].symbv).vect); }
#line 3115 "parse.cc" /* yacc.c:1646  */
    break;

  case 150:
#line 793 "parse.y" /* yacc.c:1646  */
    { compile_net((yyvsp[-9].text), (yyvsp[-6].text), (yyvsp[-4].numb), (yyvsp[-3].numb), -vpiLogicVar, false, (yyvsp[-7].flag), (yyvsp[-1].symbv).cnt, (yyvsp[-1].symbv).vect); }
#line 3121 "parse.cc" /* yacc.c:1646  */
    break;

  case 151:
#line 797 "parse.y" /* yacc.c:1646  */
    { compile_net((yyvsp[-9].text), (yyvsp[-6].text), (yyvsp[-4].numb), (yyvsp[-3].numb), -vpiLogicVar, false, (yyvsp[-7].flag), (yyvsp[-1].symbv).cnt, (yyvsp[-1].symbv).vect); }
#line 3127 "parse.cc" /* yacc.c:1646  */
    break;

  case 152:
#line 801 "parse.y" /* yacc.c:1646  */
    { compile_net((yyvsp[-9].text), (yyvsp[-6].text), (yyvsp[-4].numb), (yyvsp[-3].numb), -vpiLogicVar, true, (yyvsp[-7].flag), (yyvsp[-1].symbv).cnt, (yyvsp[-1].symbv).vect); }
#line 3133 "parse.cc" /* yacc.c:1646  */
    break;

  case 153:
#line 805 "parse.y" /* yacc.c:1646  */
    { compile_net((yyvsp[-9].text), (yyvsp[-6].text), (yyvsp[-4].numb), (yyvsp[-3].numb), -vpiLogicVar, true, (yyvsp[-7].flag), (yyvsp[-1].symbv).cnt, (yyvsp[-1].symbv).vect); }
#line 3139 "parse.cc" /* yacc.c:1646  */
    break;

  case 154:
#line 809 "parse.y" /* yacc.c:1646  */
    { compile_net_real((yyvsp[-9].text), (yyvsp[-6].text), (yyvsp[-4].numb), (yyvsp[-3].numb), (yyvsp[-7].flag), (yyvsp[-1].symbv).cnt, (yyvsp[-1].symbv).vect); }
#line 3145 "parse.cc" /* yacc.c:1646  */
    break;

  case 155:
#line 815 "parse.y" /* yacc.c:1646  */
    { compile_netw((yyvsp[-9].text), (yyvsp[-7].text), (yyvsp[-6].numb), (yyvsp[-4].numb), (yyvsp[-3].numb), vpiLogicVar, false, (yyvsp[-1].symbv).cnt, (yyvsp[-1].symbv).vect); }
#line 3151 "parse.cc" /* yacc.c:1646  */
    break;

  case 156:
#line 819 "parse.y" /* yacc.c:1646  */
    { compile_netw((yyvsp[-9].text), (yyvsp[-7].text), (yyvsp[-6].numb), (yyvsp[-4].numb), (yyvsp[-3].numb), vpiLogicVar, true, (yyvsp[-1].symbv).cnt, (yyvsp[-1].symbv).vect); }
#line 3157 "parse.cc" /* yacc.c:1646  */
    break;

  case 157:
#line 823 "parse.y" /* yacc.c:1646  */
    { compile_netw((yyvsp[-9].text), (yyvsp[-7].text), (yyvsp[-6].numb), (yyvsp[-4].numb), (yyvsp[-3].numb), vpiIntVar, true, (yyvsp[-1].symbv).cnt, (yyvsp[-1].symbv).vect); }
#line 3163 "parse.cc" /* yacc.c:1646  */
    break;

  case 158:
#line 827 "parse.y" /* yacc.c:1646  */
    { compile_netw((yyvsp[-9].text), (yyvsp[-7].text), (yyvsp[-6].numb), (yyvsp[-4].numb), (yyvsp[-3].numb), -vpiLogicVar, false, (yyvsp[-1].symbv).cnt, (yyvsp[-1].symbv).vect); }
#line 3169 "parse.cc" /* yacc.c:1646  */
    break;

  case 159:
#line 831 "parse.y" /* yacc.c:1646  */
    { compile_netw((yyvsp[-9].text), (yyvsp[-7].text), (yyvsp[-6].numb), (yyvsp[-4].numb), (yyvsp[-3].numb), -vpiLogicVar, true, (yyvsp[-1].symbv).cnt, (yyvsp[-1].symbv).vect); }
#line 3175 "parse.cc" /* yacc.c:1646  */
    break;

  case 160:
#line 835 "parse.y" /* yacc.c:1646  */
    { compile_netw_real((yyvsp[-9].text), (yyvsp[-7].text), (yyvsp[-6].numb), (yyvsp[-4].numb), (yyvsp[-3].numb), (yyvsp[-1].symbv).cnt, (yyvsp[-1].symbv).vect); }
#line 3181 "parse.cc" /* yacc.c:1646  */
    break;

  case 161:
#line 841 "parse.y" /* yacc.c:1646  */
    { compile_param_string((yyvsp[-8].text), (yyvsp[-6].text), (yyvsp[-1].text), (yyvsp[-5].numb), (yyvsp[-4].numb), (yyvsp[-3].numb)); }
#line 3187 "parse.cc" /* yacc.c:1646  */
    break;

  case 162:
#line 844 "parse.y" /* yacc.c:1646  */
    { compile_param_logic((yyvsp[-8].text), (yyvsp[-6].text), (yyvsp[-1].text), false, (yyvsp[-5].numb), (yyvsp[-4].numb), (yyvsp[-3].numb)); }
#line 3193 "parse.cc" /* yacc.c:1646  */
    break;

  case 163:
#line 847 "parse.y" /* yacc.c:1646  */
    { compile_param_logic((yyvsp[-9].text), (yyvsp[-7].text), (yyvsp[-1].text), true, (yyvsp[-6].numb), (yyvsp[-5].numb), (yyvsp[-4].numb) ); }
#line 3199 "parse.cc" /* yacc.c:1646  */
    break;

  case 164:
#line 850 "parse.y" /* yacc.c:1646  */
    { compile_param_real((yyvsp[-8].text), (yyvsp[-6].text), (yyvsp[-1].text), (yyvsp[-5].numb), (yyvsp[-4].numb), (yyvsp[-3].numb)); }
#line 3205 "parse.cc" /* yacc.c:1646  */
    break;

  case 165:
#line 855 "parse.y" /* yacc.c:1646  */
    { compile_island((yyvsp[-3].text), (yyvsp[-1].text)); }
#line 3211 "parse.cc" /* yacc.c:1646  */
    break;

  case 166:
#line 858 "parse.y" /* yacc.c:1646  */
    { compile_island_port((yyvsp[-5].text), (yyvsp[-3].text), (yyvsp[-1].text)); }
#line 3217 "parse.cc" /* yacc.c:1646  */
    break;

  case 167:
#line 861 "parse.y" /* yacc.c:1646  */
    { compile_island_import((yyvsp[-5].text), (yyvsp[-3].text), (yyvsp[-1].text)); }
#line 3223 "parse.cc" /* yacc.c:1646  */
    break;

  case 168:
#line 864 "parse.y" /* yacc.c:1646  */
    { compile_island_export((yyvsp[-3].text), (yyvsp[-1].text)); }
#line 3229 "parse.cc" /* yacc.c:1646  */
    break;

  case 169:
#line 867 "parse.y" /* yacc.c:1646  */
    { compile_island_tranif(0, (yyvsp[-4].text), (yyvsp[-2].text), (yyvsp[-1].text), 0, 1); }
#line 3235 "parse.cc" /* yacc.c:1646  */
    break;

  case 170:
#line 870 "parse.y" /* yacc.c:1646  */
    { compile_island_tranif(0, (yyvsp[-6].text), (yyvsp[-4].text), (yyvsp[-3].text), (yyvsp[-1].text), 1); }
#line 3241 "parse.cc" /* yacc.c:1646  */
    break;

  case 171:
#line 873 "parse.y" /* yacc.c:1646  */
    { compile_island_tranif(1, (yyvsp[-6].text), (yyvsp[-4].text), (yyvsp[-3].text), (yyvsp[-1].text), 1); }
#line 3247 "parse.cc" /* yacc.c:1646  */
    break;

  case 172:
#line 876 "parse.y" /* yacc.c:1646  */
    { compile_island_tranif(0, (yyvsp[-4].text), (yyvsp[-2].text), (yyvsp[-1].text), 0, 0); }
#line 3253 "parse.cc" /* yacc.c:1646  */
    break;

  case 173:
#line 879 "parse.y" /* yacc.c:1646  */
    { compile_island_tranif(0, (yyvsp[-6].text), (yyvsp[-4].text), (yyvsp[-3].text), (yyvsp[-1].text), 0); }
#line 3259 "parse.cc" /* yacc.c:1646  */
    break;

  case 174:
#line 882 "parse.y" /* yacc.c:1646  */
    { compile_island_tranif(1, (yyvsp[-6].text), (yyvsp[-4].text), (yyvsp[-3].text), (yyvsp[-1].text), 0); }
#line 3265 "parse.cc" /* yacc.c:1646  */
    break;

  case 175:
#line 885 "parse.y" /* yacc.c:1646  */
    { compile_island_tranvp((yyvsp[-4].text), (yyvsp[-2].text), (yyvsp[-1].text), (yyvsp[-8].numb), (yyvsp[-7].numb), (yyvsp[-6].numb)); }
#line 3271 "parse.cc" /* yacc.c:1646  */
    break;

  case 176:
#line 890 "parse.y" /* yacc.c:1646  */
    { compile_class_start((yyvsp[-5].text), (yyvsp[-3].text), (yyvsp[-1].numb)); }
#line 3277 "parse.cc" /* yacc.c:1646  */
    break;

  case 177:
#line 892 "parse.y" /* yacc.c:1646  */
    { compile_class_done(); }
#line 3283 "parse.cc" /* yacc.c:1646  */
    break;

  case 178:
#line 895 "parse.y" /* yacc.c:1646  */
    { ; }
#line 3289 "parse.cc" /* yacc.c:1646  */
    break;

  case 184:
#line 914 "parse.y" /* yacc.c:1646  */
    { compile_class_property((yyvsp[-5].numb), (yyvsp[-3].text), (yyvsp[-1].text), (yyvsp[0].numb)); }
#line 3295 "parse.cc" /* yacc.c:1646  */
    break;

  case 185:
#line 925 "parse.y" /* yacc.c:1646  */
    { (yyval.numb) = (yyvsp[0].numb); }
#line 3301 "parse.cc" /* yacc.c:1646  */
    break;

  case 186:
#line 926 "parse.y" /* yacc.c:1646  */
    { (yyval.numb) = 0; }
#line 3307 "parse.cc" /* yacc.c:1646  */
    break;

  case 187:
#line 931 "parse.y" /* yacc.c:1646  */
    { (yyval.numb) = (yyvsp[-1].numb) * (yyvsp[0].numb); }
#line 3313 "parse.cc" /* yacc.c:1646  */
    break;

  case 188:
#line 933 "parse.y" /* yacc.c:1646  */
    { (yyval.numb) = (yyvsp[0].numb); }
#line 3319 "parse.cc" /* yacc.c:1646  */
    break;

  case 189:
#line 938 "parse.y" /* yacc.c:1646  */
    { (yyval.numb) = ((yyvsp[-3].numb) > (yyvsp[-1].numb)? (yyvsp[-3].numb) - (yyvsp[-1].numb) : (yyvsp[-1].numb) - (yyvsp[-3].numb)) + 1; }
#line 3325 "parse.cc" /* yacc.c:1646  */
    break;

  case 190:
#line 944 "parse.y" /* yacc.c:1646  */
    { compile_enum2_type((yyvsp[-6].text), (yyvsp[-3].numb), false, (yyvsp[-1].enum_namev)); }
#line 3331 "parse.cc" /* yacc.c:1646  */
    break;

  case 191:
#line 946 "parse.y" /* yacc.c:1646  */
    { compile_enum2_type((yyvsp[-6].text), (yyvsp[-3].numb), true, (yyvsp[-1].enum_namev)); }
#line 3337 "parse.cc" /* yacc.c:1646  */
    break;

  case 192:
#line 948 "parse.y" /* yacc.c:1646  */
    { compile_enum4_type((yyvsp[-6].text), (yyvsp[-3].numb), false, (yyvsp[-1].enum_namev)); }
#line 3343 "parse.cc" /* yacc.c:1646  */
    break;

  case 193:
#line 950 "parse.y" /* yacc.c:1646  */
    { compile_enum4_type((yyvsp[-6].text), (yyvsp[-3].numb), true, (yyvsp[-1].enum_namev)); }
#line 3349 "parse.cc" /* yacc.c:1646  */
    break;

  case 194:
#line 955 "parse.y" /* yacc.c:1646  */
    { list<struct enum_name_s>*tmp = new list<struct enum_name_s>;
	tmp->push_back((yyvsp[0].enum_name));
	(yyval.enum_namev) = tmp;
      }
#line 3358 "parse.cc" /* yacc.c:1646  */
    break;

  case 195:
#line 960 "parse.y" /* yacc.c:1646  */
    { list<struct enum_name_s>*tmp = (yyvsp[-2].enum_namev);
	tmp->push_back((yyvsp[0].enum_name));
	(yyval.enum_namev) = tmp;
      }
#line 3367 "parse.cc" /* yacc.c:1646  */
    break;

  case 196:
#line 968 "parse.y" /* yacc.c:1646  */
    { (yyval.enum_name).text = (yyvsp[-1].text); (yyval.enum_name).val2 = (yyvsp[0].numb); (yyval.enum_name).val4 = 0; }
#line 3373 "parse.cc" /* yacc.c:1646  */
    break;

  case 197:
#line 970 "parse.y" /* yacc.c:1646  */
    { (yyval.enum_name).text = (yyvsp[-1].text); (yyval.enum_name).val2 = 0; (yyval.enum_name).val4 = (yyvsp[0].vect).text; }
#line 3379 "parse.cc" /* yacc.c:1646  */
    break;

  case 198:
#line 974 "parse.y" /* yacc.c:1646  */
    { (yyval.flag) = true; }
#line 3385 "parse.cc" /* yacc.c:1646  */
    break;

  case 199:
#line 975 "parse.y" /* yacc.c:1646  */
    { (yyval.flag) = false; }
#line 3391 "parse.cc" /* yacc.c:1646  */
    break;

  case 200:
#line 982 "parse.y" /* yacc.c:1646  */
    { (yyval.text) = (yyvsp[0].text); }
#line 3397 "parse.cc" /* yacc.c:1646  */
    break;

  case 201:
#line 983 "parse.y" /* yacc.c:1646  */
    { (yyval.text) = 0; }
#line 3403 "parse.cc" /* yacc.c:1646  */
    break;

  case 202:
#line 987 "parse.y" /* yacc.c:1646  */
    { (yyval.opa) = (yyvsp[0].opa); }
#line 3409 "parse.cc" /* yacc.c:1646  */
    break;

  case 203:
#line 988 "parse.y" /* yacc.c:1646  */
    { (yyval.opa) = 0; }
#line 3415 "parse.cc" /* yacc.c:1646  */
    break;

  case 204:
#line 993 "parse.y" /* yacc.c:1646  */
    { comp_operands_t opa = (yyvsp[-2].opa);
		  assert(opa->argc < 3);
		  assert((yyvsp[0].opa)->argc == 1);
		  opa->argv[opa->argc] = (yyvsp[0].opa)->argv[0];
		  opa->argc += 1;
		  free((yyvsp[0].opa));
		  (yyval.opa) =  opa;
		}
#line 3428 "parse.cc" /* yacc.c:1646  */
    break;

  case 205:
#line 1002 "parse.y" /* yacc.c:1646  */
    { (yyval.opa) = (yyvsp[0].opa); }
#line 3434 "parse.cc" /* yacc.c:1646  */
    break;

  case 206:
#line 1007 "parse.y" /* yacc.c:1646  */
    { comp_operands_t opa = (comp_operands_t)
		  calloc(1, sizeof(struct comp_operands_s));
	opa->argc = 1;
	opa->argv[0].ltype = L_SYMB;
	opa->argv[0].symb = (yyvsp[0].symb);
	(yyval.opa) = opa;
      }
#line 3446 "parse.cc" /* yacc.c:1646  */
    break;

  case 207:
#line 1015 "parse.y" /* yacc.c:1646  */
    { comp_operands_t opa = (comp_operands_t)
		  calloc(1, sizeof(struct comp_operands_s));
	opa->argc = 1;
	opa->argv[0].ltype = L_NUMB;
	opa->argv[0].numb = (yyvsp[0].numb);
	(yyval.opa) = opa;
      }
#line 3458 "parse.cc" /* yacc.c:1646  */
    break;

  case 208:
#line 1023 "parse.y" /* yacc.c:1646  */
    { comp_operands_t opa = (comp_operands_t)
		  calloc(1, sizeof(struct comp_operands_s));
	opa->argc = 1;
	opa->argv[0].ltype = L_STRING;
	opa->argv[0].text = (yyvsp[0].text);
	(yyval.opa) = opa;
      }
#line 3470 "parse.cc" /* yacc.c:1646  */
    break;

  case 209:
#line 1048 "parse.y" /* yacc.c:1646  */
    {
		  argv_sym_lookup(&(yyvsp[0].argv));
		  (yyval.argv) = (yyvsp[0].argv);
		}
#line 3479 "parse.cc" /* yacc.c:1646  */
    break;

  case 210:
#line 1053 "parse.y" /* yacc.c:1646  */
    { struct argv_s tmp;
		  argv_init(&tmp);
		  (yyval.argv) = tmp;
		}
#line 3488 "parse.cc" /* yacc.c:1646  */
    break;

  case 211:
#line 1061 "parse.y" /* yacc.c:1646  */
    { struct argv_s tmp;
	argv_init(&tmp);
	argv_add(&tmp, (yyvsp[0].vpi));
	(yyval.argv) = tmp;
      }
#line 3498 "parse.cc" /* yacc.c:1646  */
    break;

  case 212:
#line 1067 "parse.y" /* yacc.c:1646  */
    { struct argv_s tmp = (yyvsp[-2].argv);
	argv_add(&tmp, (yyvsp[0].vpi));
	(yyval.argv) = tmp;
      }
#line 3507 "parse.cc" /* yacc.c:1646  */
    break;

  case 213:
#line 1072 "parse.y" /* yacc.c:1646  */
    { struct argv_s tmp;
	argv_init(&tmp);
	argv_sym_add(&tmp, (yyvsp[0].text));
	(yyval.argv) = tmp;
      }
#line 3517 "parse.cc" /* yacc.c:1646  */
    break;

  case 214:
#line 1078 "parse.y" /* yacc.c:1646  */
    { struct argv_s tmp = (yyvsp[-2].argv);
	argv_sym_add(&tmp, (yyvsp[0].text));
	(yyval.argv) = tmp;
      }
#line 3526 "parse.cc" /* yacc.c:1646  */
    break;

  case 215:
#line 1086 "parse.y" /* yacc.c:1646  */
    { (yyval.vpi) = vpip_make_string_const((yyvsp[0].text)); }
#line 3532 "parse.cc" /* yacc.c:1646  */
    break;

  case 216:
#line 1088 "parse.y" /* yacc.c:1646  */
    { (yyval.vpi) = vpip_make_binary_const((yyvsp[0].vect).idx, (yyvsp[0].vect).text);
	free((yyvsp[0].vect).text);
      }
#line 3540 "parse.cc" /* yacc.c:1646  */
    break;

  case 217:
#line 1092 "parse.y" /* yacc.c:1646  */
    { (yyval.vpi) = (yyvsp[0].vpi); }
#line 3546 "parse.cc" /* yacc.c:1646  */
    break;

  case 218:
#line 1097 "parse.y" /* yacc.c:1646  */
    { (yyval.vpi) = vpip_make_vthr_A((yyvsp[-3].text), (yyvsp[-1].numb)); }
#line 3552 "parse.cc" /* yacc.c:1646  */
    break;

  case 219:
#line 1099 "parse.y" /* yacc.c:1646  */
    { (yyval.vpi) = vpip_make_vthr_A((yyvsp[-3].text), (yyvsp[-1].text)); }
#line 3558 "parse.cc" /* yacc.c:1646  */
    break;

  case 220:
#line 1101 "parse.y" /* yacc.c:1646  */
    { (yyval.vpi) = vpip_make_vthr_A((yyvsp[-3].text), (yyvsp[-1].vpi)); }
#line 3564 "parse.cc" /* yacc.c:1646  */
    break;

  case 221:
#line 1103 "parse.y" /* yacc.c:1646  */
    { (yyval.vpi) = vpip_make_PV((yyvsp[-5].text), (yyvsp[-3].numb), (yyvsp[-1].numb)); }
#line 3570 "parse.cc" /* yacc.c:1646  */
    break;

  case 222:
#line 1105 "parse.y" /* yacc.c:1646  */
    { (yyval.vpi) = vpip_make_PV((yyvsp[-6].text), -(yyvsp[-3].numb), (yyvsp[-1].numb)); }
#line 3576 "parse.cc" /* yacc.c:1646  */
    break;

  case 223:
#line 1107 "parse.y" /* yacc.c:1646  */
    { (yyval.vpi) = vpip_make_PV((yyvsp[-5].text), (yyvsp[-3].text), (yyvsp[-1].numb)); }
#line 3582 "parse.cc" /* yacc.c:1646  */
    break;

  case 224:
#line 1109 "parse.y" /* yacc.c:1646  */
    { (yyval.vpi) = vpip_make_PV((yyvsp[-5].text), (yyvsp[-3].vpi), (yyvsp[-1].numb)); }
#line 3588 "parse.cc" /* yacc.c:1646  */
    break;

  case 225:
#line 1111 "parse.y" /* yacc.c:1646  */
    { (yyval.vpi) = vpip_make_vthr_APV((yyvsp[-7].text), (yyvsp[-5].numb), (yyvsp[-3].numb), (yyvsp[-1].numb)); }
#line 3594 "parse.cc" /* yacc.c:1646  */
    break;

  case 226:
#line 1117 "parse.y" /* yacc.c:1646  */
    { struct symbv_s obj;
		  symbv_init(&obj);
		  symbv_add(&obj, (yyvsp[0].symb));
		  (yyval.symbv) = obj;
		}
#line 3604 "parse.cc" /* yacc.c:1646  */
    break;

  case 227:
#line 1123 "parse.y" /* yacc.c:1646  */
    { struct symbv_s obj = (yyvsp[-2].symbv);
		  symbv_add(&obj, (yyvsp[0].symb));
		  (yyval.symbv) = obj;
		}
#line 3613 "parse.cc" /* yacc.c:1646  */
    break;

  case 228:
#line 1132 "parse.y" /* yacc.c:1646  */
    { struct numbv_s obj;
		  numbv_init(&obj);
		  numbv_add(&obj, (yyvsp[0].numb));
		  (yyval.numbv) = obj;
		}
#line 3623 "parse.cc" /* yacc.c:1646  */
    break;

  case 229:
#line 1138 "parse.y" /* yacc.c:1646  */
    { struct numbv_s obj = (yyvsp[-2].numbv);
		  numbv_add(&obj, (yyvsp[0].numb));
		  (yyval.numbv) = obj;
		}
#line 3632 "parse.cc" /* yacc.c:1646  */
    break;

  case 230:
#line 1147 "parse.y" /* yacc.c:1646  */
    { struct symbv_s obj;
		  symbv_init(&obj);
		  symbv_add(&obj, (yyvsp[0].symb));
		  (yyval.symbv) = obj;
		}
#line 3642 "parse.cc" /* yacc.c:1646  */
    break;

  case 231:
#line 1153 "parse.y" /* yacc.c:1646  */
    { struct symbv_s obj = (yyvsp[-2].symbv);
		  symbv_add(&obj, (yyvsp[0].symb));
		  (yyval.symbv) = obj;
		}
#line 3651 "parse.cc" /* yacc.c:1646  */
    break;

  case 232:
#line 1165 "parse.y" /* yacc.c:1646  */
    { (yyval.symb).text = (yyvsp[0].text);
		  (yyval.symb).idx = 0;
		}
#line 3659 "parse.cc" /* yacc.c:1646  */
    break;

  case 233:
#line 1172 "parse.y" /* yacc.c:1646  */
    { (yyval.symb) = (yyvsp[0].symb); }
#line 3665 "parse.cc" /* yacc.c:1646  */
    break;

  case 234:
#line 1174 "parse.y" /* yacc.c:1646  */
    { (yyval.symb).text = 0;
		  (yyval.symb).idx = 0;
		}
#line 3673 "parse.cc" /* yacc.c:1646  */
    break;

  case 237:
#line 1189 "parse.y" /* yacc.c:1646  */
    { (yyval.vpi_enum) = vpiInput; }
#line 3679 "parse.cc" /* yacc.c:1646  */
    break;

  case 238:
#line 1190 "parse.y" /* yacc.c:1646  */
    { (yyval.vpi_enum) = vpiOutput; }
#line 3685 "parse.cc" /* yacc.c:1646  */
    break;

  case 239:
#line 1191 "parse.y" /* yacc.c:1646  */
    { (yyval.vpi_enum) = vpiInout; }
#line 3691 "parse.cc" /* yacc.c:1646  */
    break;

  case 240:
#line 1192 "parse.y" /* yacc.c:1646  */
    { (yyval.vpi_enum) = vpiMixedIO; }
#line 3697 "parse.cc" /* yacc.c:1646  */
    break;

  case 241:
#line 1193 "parse.y" /* yacc.c:1646  */
    { (yyval.vpi_enum) = vpiNoDirection; }
#line 3703 "parse.cc" /* yacc.c:1646  */
    break;

  case 242:
#line 1198 "parse.y" /* yacc.c:1646  */
    { compile_modpath_src(modpath_dst, 0, (yyvsp[-4].symb), (yyvsp[-2].numbv), 0, (yyvsp[0].symb), false); }
#line 3709 "parse.cc" /* yacc.c:1646  */
    break;

  case 243:
#line 1200 "parse.y" /* yacc.c:1646  */
    { compile_modpath_src(modpath_dst, 0, (yyvsp[-5].symb), (yyvsp[-3].numbv), 0, (yyvsp[0].symb), true); }
#line 3715 "parse.cc" /* yacc.c:1646  */
    break;

  case 244:
#line 1202 "parse.y" /* yacc.c:1646  */
    { compile_modpath_src(modpath_dst, 0, (yyvsp[-6].symb), (yyvsp[-4].numbv), (yyvsp[-2].symb), (yyvsp[0].symb)); }
#line 3721 "parse.cc" /* yacc.c:1646  */
    break;

  case 245:
#line 1204 "parse.y" /* yacc.c:1646  */
    { compile_modpath_src(modpath_dst, '+', (yyvsp[-5].symb), (yyvsp[-2].numbv), 0, (yyvsp[0].symb), false); }
#line 3727 "parse.cc" /* yacc.c:1646  */
    break;

  case 246:
#line 1206 "parse.y" /* yacc.c:1646  */
    { compile_modpath_src(modpath_dst, '+', (yyvsp[-6].symb), (yyvsp[-3].numbv), 0, (yyvsp[0].symb), true); }
#line 3733 "parse.cc" /* yacc.c:1646  */
    break;

  case 247:
#line 1208 "parse.y" /* yacc.c:1646  */
    { compile_modpath_src(modpath_dst, '+', (yyvsp[-7].symb), (yyvsp[-4].numbv), (yyvsp[-2].symb), (yyvsp[0].symb)); }
#line 3739 "parse.cc" /* yacc.c:1646  */
    break;

  case 248:
#line 1210 "parse.y" /* yacc.c:1646  */
    { compile_modpath_src(modpath_dst, '-', (yyvsp[-5].symb), (yyvsp[-2].numbv), 0, (yyvsp[0].symb), false); }
#line 3745 "parse.cc" /* yacc.c:1646  */
    break;

  case 249:
#line 1212 "parse.y" /* yacc.c:1646  */
    { compile_modpath_src(modpath_dst, '-', (yyvsp[-6].symb), (yyvsp[-3].numbv), 0, (yyvsp[0].symb), true); }
#line 3751 "parse.cc" /* yacc.c:1646  */
    break;

  case 250:
#line 1214 "parse.y" /* yacc.c:1646  */
    { compile_modpath_src(modpath_dst, '-', (yyvsp[-7].symb), (yyvsp[-4].numbv), (yyvsp[-2].symb), (yyvsp[0].symb)); }
#line 3757 "parse.cc" /* yacc.c:1646  */
    break;

  case 251:
#line 1219 "parse.y" /* yacc.c:1646  */
    { (yyval.table) = compile_udp_table(0x0, (yyvsp[0].text)); }
#line 3763 "parse.cc" /* yacc.c:1646  */
    break;

  case 252:
#line 1221 "parse.y" /* yacc.c:1646  */
    { (yyval.table) = compile_udp_table((yyvsp[-2].table),  (yyvsp[0].text)); }
#line 3769 "parse.cc" /* yacc.c:1646  */
    break;

  case 253:
#line 1225 "parse.y" /* yacc.c:1646  */
    { (yyval.numb) = (yyvsp[0].numb); }
#line 3775 "parse.cc" /* yacc.c:1646  */
    break;

  case 254:
#line 1226 "parse.y" /* yacc.c:1646  */
    { (yyval.numb) = -(yyvsp[0].numb); }
#line 3781 "parse.cc" /* yacc.c:1646  */
    break;

  case 255:
#line 1231 "parse.y" /* yacc.c:1646  */
    { (yyval.cdelay) = new vvp_delay_t((yyvsp[-1].numb), (yyvsp[-1].numb)); }
#line 3787 "parse.cc" /* yacc.c:1646  */
    break;

  case 256:
#line 1233 "parse.y" /* yacc.c:1646  */
    { (yyval.cdelay) = new vvp_delay_t((yyvsp[-3].numb), (yyvsp[-1].numb)); }
#line 3793 "parse.cc" /* yacc.c:1646  */
    break;

  case 257:
#line 1235 "parse.y" /* yacc.c:1646  */
    { (yyval.cdelay) = new vvp_delay_t((yyvsp[-5].numb), (yyvsp[-3].numb), (yyvsp[-1].numb)); }
#line 3799 "parse.cc" /* yacc.c:1646  */
    break;


#line 3803 "parse.cc" /* yacc.c:1646  */
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
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
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

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

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

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
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
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
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
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 1238 "parse.y" /* yacc.c:1906  */


int compile_design(const char*path)
{
      yypath = path;
      yyline = 1;
      yyin = fopen(path, "r");
      if (yyin == 0) {
	    fprintf(stderr, "%s: Unable to open input file.\n", path);
	    return -1;
      }

      int rc = yyparse();
      fclose(yyin);
      return rc;
}
