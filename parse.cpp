/* A Bison parser, made from parse.y
   by GNU bison 1.35.  */

#define YYBISON 1  /* Identify Bison output.  */

#define yyparse VLparse
#define yylex VLlex
#define yyerror VLerror
#define yylval VLlval
#define yychar VLchar
#define yydebug VLdebug
#define yynerrs VLnerrs
#define YYLSP_NEEDED 1

# define	IDENTIFIER	257
# define	SYSTEM_IDENTIFIER	258
# define	STRING	259
# define	PATHPULSE_IDENTIFIER	260
# define	BASED_NUMBER	261
# define	DEC_NUMBER	262
# define	REALTIME	263
# define	K_LE	264
# define	K_GE	265
# define	K_EG	266
# define	K_EQ	267
# define	K_NE	268
# define	K_CEQ	269
# define	K_CNE	270
# define	K_LS	271
# define	K_RS	272
# define	K_RSS	273
# define	K_SG	274
# define	K_PO_POS	275
# define	K_PO_NEG	276
# define	K_PSTAR	277
# define	K_STARP	278
# define	K_LOR	279
# define	K_LAND	280
# define	K_NAND	281
# define	K_NOR	282
# define	K_NXOR	283
# define	K_TRIGGER	284
# define	K_always	285
# define	K_and	286
# define	K_assign	287
# define	K_begin	288
# define	K_buf	289
# define	K_bufif0	290
# define	K_bufif1	291
# define	K_case	292
# define	K_casex	293
# define	K_casez	294
# define	K_cmos	295
# define	K_deassign	296
# define	K_default	297
# define	K_defparam	298
# define	K_disable	299
# define	K_edge	300
# define	K_else	301
# define	K_end	302
# define	K_endcase	303
# define	K_endfunction	304
# define	K_endmodule	305
# define	K_endprimitive	306
# define	K_endspecify	307
# define	K_endtable	308
# define	K_endtask	309
# define	K_event	310
# define	K_for	311
# define	K_force	312
# define	K_forever	313
# define	K_fork	314
# define	K_function	315
# define	K_highz0	316
# define	K_highz1	317
# define	K_if	318
# define	K_initial	319
# define	K_inout	320
# define	K_input	321
# define	K_integer	322
# define	K_join	323
# define	K_large	324
# define	K_localparam	325
# define	K_macromodule	326
# define	K_medium	327
# define	K_module	328
# define	K_nand	329
# define	K_negedge	330
# define	K_nmos	331
# define	K_nor	332
# define	K_not	333
# define	K_notif0	334
# define	K_notif1	335
# define	K_or	336
# define	K_output	337
# define	K_parameter	338
# define	K_pmos	339
# define	K_posedge	340
# define	K_primitive	341
# define	K_pull0	342
# define	K_pull1	343
# define	K_pulldown	344
# define	K_pullup	345
# define	K_rcmos	346
# define	K_real	347
# define	K_realtime	348
# define	K_reg	349
# define	K_release	350
# define	K_repeat	351
# define	K_rnmos	352
# define	K_rpmos	353
# define	K_rtran	354
# define	K_rtranif0	355
# define	K_rtranif1	356
# define	K_scalared	357
# define	K_signed	358
# define	K_small	359
# define	K_specify	360
# define	K_specparam	361
# define	K_strong0	362
# define	K_strong1	363
# define	K_supply0	364
# define	K_supply1	365
# define	K_table	366
# define	K_task	367
# define	K_time	368
# define	K_tran	369
# define	K_tranif0	370
# define	K_tranif1	371
# define	K_tri	372
# define	K_tri0	373
# define	K_tri1	374
# define	K_triand	375
# define	K_trior	376
# define	K_trireg	377
# define	K_vectored	378
# define	K_wait	379
# define	K_wand	380
# define	K_weak0	381
# define	K_weak1	382
# define	K_while	383
# define	K_wire	384
# define	K_wor	385
# define	K_xnor	386
# define	K_xor	387
# define	K_Shold	388
# define	K_Speriod	389
# define	K_Srecovery	390
# define	K_Srecrem	391
# define	K_Ssetup	392
# define	K_Swidth	393
# define	K_Ssetuphold	394
# define	KK_attribute	395
# define	K_TAND	396
# define	UNARY_PREC	397
# define	less_than_K_else	398

#line 2 "parse.y"

#include "config.h"

#include "parse_misc.h"
#include "pform.h"
#include "parse_api.h"
#include "HName.h"
#include "PDesign.h"
#include "CfgNode.h"
#include "slice.h"
//#include "cfg.h"
#include "netlist.h"

#include <sstream>

extern void lex_start_table();
extern void lex_end_table();

static svector<PExpr*>* active_range = 0;
static bool active_signed = false;

/* Port declaration lists use this structure for context. */
static struct {
      NetNet::Type port_net_type;
      NetNet::PortType port_type;
      bool sign_flag;
      svector<PExpr*>* range;
} port_declaration_context;

/* Later version of bison (including 1.35) will not compile in stack
   extension if the output is compiled with C++ and either the YYSTYPE
   or YYLTYPE are provided by the source code. However, I can get the
   old behavior back by defining these symbols. */
# define YYSTYPE_IS_TRIVIAL 1
# define YYLTYPE_IS_TRIVIAL 1

/* Recent version of bison expect that the user supply a
   YYLLOC_DEFAULT macro that makes up a yylloc value from existing
   values. I need to supply an explicit version to account for the
   text field, that otherwise won't be copied. */
# define YYLLOC_DEFAULT(Current, Rhs, N)  do {       \
  (Current).first_line   = (Rhs)[1].first_line;      \
  (Current).first_column = (Rhs)[1].first_column;    \
  (Current).last_line    = (Rhs)[N].last_line;       \
  (Current).last_column  = (Rhs)[N].last_column;     \
  (Current).text         = (Rhs)[1].text;   } while (0)

/*
 * These are some common strength pairs that are used as defaults when
 * the user is not otherwise specific.
 */
const static struct str_pair_t pull_strength = { PGate::PULL,  PGate::PULL };
const static struct str_pair_t str_strength = { PGate::NO_STRENGTH, PGate::NO_STRENGTH };

#line 57 "parse.y"
#ifndef YYSTYPE
typedef union {
      bool flag;

      char letter;

	/* text items are C strings allocated by the lexor using
	   strdup. They can be put into lists with the texts type. */
      char*text;
      list<char*>*texts;

      hname_t*hier;

      list<string>*strings;

      struct str_pair_t drive;

      PCase::Item*citem;
      svector<PCase::Item*>*citems;

      lgate*gate;
      svector<lgate>*gates;

      Module::port_t *mport;
      svector<Module::port_t*>*mports;

      named_pexpr_t*named_pexpr;
      svector<named_pexpr_t*>*named_pexprs;
      struct parmvalue_t*parmvalue;

      PExpr*expr;
      svector<PExpr*>*exprs;

      svector<PEEvent*>*event_expr;

      NetNet::Type nettype;
      PGBuiltin::Type gatetype;
      NetNet::PortType porttype;

      PWire*wire;
      svector<PWire*>*wires;

      PEventStatement*event_statement;
      Statement*statement;
      svector<Statement*>*statement_list;

      struct { svector<PExpr*>*range; NetNet::Type ntype; } range_type;

      struct { svector<PExpr*>*range; svector<PExpr*>*delay; } range_delay;
      net_decl_assign_t*net_decl_assign;
      
      verinum* number;

      verireal* realtime;
} yystype;
# define YYSTYPE yystype
# define YYSTYPE_IS_TRIVIAL 1
#endif

#ifndef YYLTYPE
typedef struct yyltype
{
  int first_line;
  int first_column;

  int last_line;
  int last_column;
} yyltype;

# define YYLTYPE yyltype
# define YYLTYPE_IS_TRIVIAL 1
#endif

#ifndef YYDEBUG
# define YYDEBUG 1
#endif



#define	YYFINAL		1183
#define	YYFLAG		-32768
#define	YYNTBASE	190

/* YYTRANSLATE(YYLEX) -- Bison token number corresponding to YYLEX. */
#define YYTRANSLATE(x) ((unsigned)(x) <= 398 ? yytranslate[x] : 315)

/* YYTRANSLATE[YYLEX] -- Bison token number corresponding to YYLEX. */
static const short yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   165,     2,   162,     2,   154,   147,     2,
     160,   161,   152,   150,   157,   151,   170,   153,   171,   172,
       2,     2,     2,     2,     2,     2,     2,     2,   144,   159,
     148,   158,   149,   143,   163,     2,   179,     2,     2,     2,
     176,     2,     2,     2,     2,     2,     2,   182,   184,     2,
     186,   187,   181,     2,     2,     2,     2,     2,     2,     2,
       2,   166,     2,   167,   146,   189,     2,     2,   174,     2,
       2,     2,   175,     2,   178,     2,     2,     2,   177,     2,
     183,     2,   185,   188,   180,     2,     2,     2,     2,     2,
     173,     2,     2,   168,   145,   169,   164,     2,     2,     2,
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
       2,     2,     2,     2,     2,     2,     1,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   155,   156
};

#if YYDEBUG
static const short yyprhs[] =
{
       0,     0,     2,     3,     5,     8,    10,    12,    15,    19,
      22,    23,    27,    29,    31,    35,    41,    46,    50,    54,
      58,    62,    66,    70,    74,    78,    82,    86,    90,    94,
      98,   100,   103,   105,   106,   110,   114,   117,   121,   124,
     126,   130,   134,   138,   140,   141,   145,   147,   150,   154,
     157,   162,   165,   170,   177,   186,   188,   189,   191,   197,
     199,   201,   203,   205,   207,   216,   222,   228,   234,   240,
     246,   252,   254,   255,   257,   259,   261,   263,   265,   267,
     269,   271,   274,   279,   284,   286,   290,   294,   297,   300,
     302,   304,   307,   310,   313,   316,   319,   322,   325,   328,
     331,   334,   337,   340,   344,   348,   352,   356,   360,   364,
     368,   372,   376,   380,   384,   388,   392,   396,   400,   404,
     408,   412,   416,   420,   424,   428,   432,   436,   442,   446,
     448,   449,   452,   454,   456,   458,   460,   462,   467,   474,
     479,   484,   488,   492,   499,   504,   510,   515,   520,   522,
     524,   527,   532,   538,   542,   547,   551,   553,   555,   557,
     559,   561,   563,   565,   567,   569,   571,   573,   575,   577,
     579,   581,   583,   585,   587,   589,   591,   593,   595,   597,
     599,   601,   603,   607,   609,   613,   615,   619,   621,   625,
     629,   636,   643,   650,   657,   659,   660,   664,   668,   669,
     671,   672,   674,   679,   682,   686,   688,   693,   700,   704,
     708,   712,   714,   715,   716,   727,   729,   731,   734,   735,
     740,   743,   747,   752,   759,   766,   773,   779,   785,   792,
     799,   806,   813,   819,   821,   825,   829,   834,   840,   846,
     853,   857,   861,   868,   875,   880,   886,   890,   894,   895,
     903,   904,   913,   916,   920,   924,   927,   933,   937,   941,
     951,   957,   960,   962,   964,   965,   969,   973,   975,   977,
     979,   981,   983,   985,   987,   989,   991,   993,   995,   997,
    1001,  1003,  1004,  1008,  1009,  1014,  1016,  1020,  1024,  1026,
    1029,  1033,  1038,  1043,  1046,  1049,  1050,  1056,  1061,  1063,
    1067,  1069,  1075,  1079,  1087,  1089,  1090,  1092,  1099,  1104,
    1109,  1111,  1115,  1121,  1127,  1132,  1136,  1138,  1140,  1142,
    1144,  1150,  1152,  1153,  1155,  1157,  1159,  1161,  1163,  1164,
    1166,  1170,  1177,  1179,  1183,  1187,  1190,  1193,  1200,  1207,
    1218,  1227,  1238,  1249,  1262,  1275,  1286,  1294,  1296,  1300,
    1302,  1305,  1311,  1315,  1323,  1335,  1343,  1355,  1363,  1375,
    1383,  1395,  1397,  1399,  1401,  1407,  1411,  1417,  1424,  1431,
    1435,  1437,  1442,  1446,  1453,  1457,  1465,  1469,  1477,  1479,
    1483,  1485,  1487,  1488,  1491,  1494,  1499,  1504,  1508,  1510,
    1511,  1513,  1515,  1518,  1521,  1525,  1527,  1533,  1537,  1543,
    1547,  1551,  1552,  1560,  1563,  1568,  1572,  1573,  1581,  1584,
    1589,  1593,  1597,  1600,  1604,  1610,  1617,  1624,  1631,  1638,
    1645,  1652,  1658,  1666,  1672,  1680,  1694,  1706,  1720,  1726,
    1732,  1738,  1741,  1744,  1748,  1754,  1759,  1764,  1770,  1776,
    1782,  1792,  1798,  1808,  1814,  1820,  1823,  1829,  1832,  1835,
    1838,  1840,  1842,  1844,  1846,  1851,  1856,  1861,  1864,  1866,
    1868,  1869,  1870,  1875,  1877,  1879,  1884,  1886,  1889,  1891,
    1894,  1901,  1907,  1909,  1910,  1912,  1915,  1917,  1919,  1921,
    1923,  1925,  1927,  1929,  1931,  1933,  1935,  1937,  1939,  1941,
    1943,  1945,  1947,  1949,  1951,  1953,  1955,  1957,  1959,  1961,
    1963,  1965,  1967,  1969,  1973,  1977,  1981,  1983,  1986,  1988,
    1992
};
static const short yyrhs[] =
{
     191,     0,     0,   210,     0,   191,   210,     0,     7,     0,
       8,     0,     8,     7,     0,    23,   194,    24,     0,    23,
      24,     0,     0,   194,   157,   195,     0,   195,     0,     3,
       0,     3,   158,   218,     0,    95,   233,   271,   275,   159,
       0,    95,   233,   275,   159,     0,    68,   275,   159,     0,
     114,   275,   159,     0,    93,   227,   159,     0,    94,   227,
     159,     0,    84,   255,   159,     0,    71,   260,   159,     0,
      95,     1,   159,     0,    68,     1,   159,     0,   114,     1,
     159,     0,    93,     1,   159,     0,    94,     1,   159,     0,
      84,     1,   159,     0,    71,     1,   159,     0,   196,     0,
     197,   196,     0,   197,     0,     0,   219,   144,   295,     0,
      43,   144,   295,     0,    43,   295,     0,     1,   144,   295,
       0,   200,   199,     0,   199,     0,   160,   105,   161,     0,
     160,    73,   161,     0,   160,    70,   161,     0,   201,     0,
       0,   226,   158,   218,     0,   203,     0,   271,   203,     0,
     204,   157,   203,     0,   162,   209,     0,   162,   160,   208,
     161,     0,   162,   209,     0,   162,   160,   208,   161,     0,
     162,   160,   208,   157,   208,   161,     0,   162,   160,   208,
     157,   208,   157,   208,   161,     0,   206,     0,     0,   218,
       0,   218,   144,   218,   144,   218,     0,     8,     0,     9,
       0,     3,     0,   238,     0,   314,     0,   141,   160,     3,
     157,     5,   157,     5,   161,     0,   160,   213,   157,   214,
     161,     0,   160,   214,   157,   213,   161,     0,   160,   213,
     157,    63,   161,     0,   160,   214,   157,    62,   161,     0,
     160,    63,   157,   213,   161,     0,   160,    62,   157,   214,
     161,     0,   211,     0,     0,   110,     0,   108,     0,    88,
       0,   127,     0,   111,     0,   109,     0,    89,     0,   128,
       0,   163,   226,     0,   163,   160,   216,   161,     0,   163,
     160,     1,   161,     0,   217,     0,   216,    82,   217,     0,
     216,   157,   217,     0,    86,   218,     0,    76,   218,     0,
     218,     0,   220,     0,   150,   220,     0,   151,   220,     0,
     164,   220,     0,   147,   220,     0,   165,   220,     0,   145,
     220,     0,   146,   220,     0,    27,   220,     0,    28,   220,
       0,    29,   220,     0,   165,     1,     0,   146,     1,     0,
     218,   146,   218,     0,   218,   152,   218,     0,   218,   153,
     218,     0,   218,   154,   218,     0,   218,   150,   218,     0,
     218,   151,   218,     0,   218,   147,   218,     0,   218,   145,
     218,     0,   218,    27,   218,     0,   218,    28,   218,     0,
     218,    29,   218,     0,   218,   148,   218,     0,   218,   149,
     218,     0,   218,    17,   218,     0,   218,    18,   218,     0,
     218,    19,   218,     0,   218,    13,   218,     0,   218,    15,
     218,     0,   218,    10,   218,     0,   218,    11,   218,     0,
     218,    14,   218,     0,   218,    16,   218,     0,   218,    25,
     218,     0,   218,    26,   218,     0,   218,   143,   218,   144,
     218,     0,   219,   157,   218,     0,   218,     0,     0,   219,
     157,     0,   192,     0,     9,     0,     5,     0,   226,     0,
       4,     0,   226,   166,   218,   167,     0,   226,   166,   218,
     144,   218,   167,     0,   226,   160,   219,   161,     0,     4,
     160,   219,   161,     0,   160,   218,   161,     0,   168,   219,
     169,     0,   168,   218,   168,   219,   169,   169,     0,    67,
     272,   227,   159,     0,    67,   104,   272,   227,   159,     0,
      83,   272,   227,   159,     0,    66,   272,   227,   159,     0,
     196,     0,   221,     0,   222,   221,     0,     3,   160,   219,
     161,     0,     3,   271,   160,   219,   161,     0,   160,   219,
     161,     0,     3,   160,   269,   161,     0,   224,   157,   223,
       0,   223,     0,    32,     0,    75,     0,    82,     0,    78,
       0,   133,     0,   132,     0,    35,     0,    36,     0,    37,
       0,    79,     0,    80,     0,    81,     0,    77,     0,    98,
       0,    85,     0,    99,     0,    41,     0,    92,     0,   115,
       0,   100,     0,   116,     0,   117,     0,   101,     0,   102,
       0,     3,     0,   226,   170,     3,     0,     3,     0,   227,
     157,     3,     0,   265,     0,   228,   157,   265,     0,   230,
       0,   229,   157,   230,     0,   229,   157,     3,     0,   193,
      67,   231,   233,   272,     3,     0,   193,    66,   231,   233,
     272,     3,     0,   193,    83,   231,   233,   272,     3,     0,
     193,    83,   253,   233,   272,     3,     0,   252,     0,     0,
     160,   228,   161,     0,   160,   229,   161,     0,     0,   104,
       0,     0,   226,     0,   226,   166,   218,   167,     0,   226,
     271,     0,   168,   219,   169,     0,   226,     0,   226,   166,
     218,   167,     0,   226,   166,   218,   144,   218,   167,     0,
     168,   219,   169,     0,   234,   158,   218,     0,   237,   157,
     236,     0,   236,     0,     0,     0,   193,   241,     3,   239,
     243,   232,   159,   240,   249,    51,     0,    74,     0,    72,
       0,   272,   207,     0,     0,   162,   160,   244,   161,     0,
      84,   254,     0,   244,   157,   254,     0,   244,   157,    84,
     254,     0,   193,   252,   233,   242,   227,   159,     0,   193,
     252,   233,   242,   251,   159,     0,   193,   252,   233,   211,
     251,   159,     0,   123,   202,   242,   227,   159,     0,   270,
     233,   242,   227,   159,     0,   270,   252,   233,   272,   227,
     159,     0,    83,   253,   233,   272,   227,   159,     0,    67,
     253,   233,   272,   227,   159,     0,    66,   253,   233,   272,
     227,   159,     0,   270,   233,   242,     1,   159,     0,   196,
       0,    44,   204,   159,     0,    56,   227,   159,     0,   193,
     225,   224,   159,     0,   193,   225,   206,   224,   159,     0,
     193,   225,   211,   224,   159,     0,   193,   225,   211,   206,
     224,   159,     0,    91,   224,   159,     0,    90,   224,   159,
       0,    91,   160,   214,   161,   224,   159,     0,    90,   160,
     213,   161,   224,   159,     0,     3,   261,   224,   159,     0,
      33,   212,   207,   237,   159,     0,   193,    31,   291,     0,
     193,    65,   291,     0,     0,   113,     3,   159,   246,   298,
     295,    55,     0,     0,    61,   273,     3,   159,   247,   222,
     291,    50,     0,   106,    53,     0,   106,   278,    53,     0,
     106,     1,    53,     0,     1,   159,     0,    33,     1,   158,
     218,   159,     0,    33,     1,   159,     0,    61,     1,    50,
       0,   141,   160,     3,   157,     5,   157,     5,   161,   159,
       0,   141,   160,     1,   161,   159,     0,   248,   245,     0,
     245,     0,   248,     0,     0,     3,   158,   218,     0,   251,
     157,   250,     0,   250,     0,   130,     0,   118,     0,   120,
       0,   110,     0,   126,     0,   121,     0,   119,     0,   111,
       0,   131,     0,   122,     0,    95,     0,     3,   158,   218,
       0,   258,     0,     0,   271,   256,   258,     0,     0,   104,
     271,   257,   258,     0,   254,     0,   258,   157,   254,     0,
       3,   158,   218,     0,   259,     0,   271,   259,     0,   260,
     157,   259,     0,   162,   160,   219,   161,     0,   162,   160,
     263,   161,     0,   162,     8,     0,   162,     1,     0,     0,
     170,     3,   160,   218,   161,     0,   170,     3,   160,   161,
       0,   262,     0,   263,   157,   262,     0,   266,     0,   170,
       3,   160,   266,   161,     0,   168,   267,   169,     0,   170,
       3,   160,   168,   267,   169,   161,     0,   264,     0,     0,
       3,     0,     3,   166,   218,   144,   218,   167,     0,     3,
     166,   218,   167,     0,     3,   166,     1,   167,     0,   266,
       0,   267,   157,   266,     0,   170,     3,   160,   218,   161,
       0,   170,     3,   160,     1,   161,     0,   170,     3,   160,
     161,     0,   269,   157,   268,     0,   268,     0,    67,     0,
      83,     0,    66,     0,   166,   218,   144,   218,   167,     0,
     271,     0,     0,   271,     0,    68,     0,    93,     0,    94,
       0,   114,     0,     0,     3,     0,     3,   158,   218,     0,
       3,   166,   218,   144,   218,   167,     0,   274,     0,   275,
     157,   274,     0,   107,   286,   159,     0,   282,   159,     0,
     279,   159,     0,    64,   160,   218,   161,   282,   159,     0,
      64,   160,   218,   161,   279,   159,     0,   134,   160,   288,
     157,   288,   157,   218,   289,   161,   159,     0,   135,   160,
     288,   157,   218,   289,   161,   159,     0,   136,   160,   288,
     157,   288,   157,   218,   289,   161,   159,     0,   138,   160,
     288,   157,   288,   157,   218,   289,   161,   159,     0,   140,
     160,   288,   157,   288,   157,   208,   157,   208,   289,   161,
     159,     0,   137,   160,   288,   157,   288,   157,   208,   157,
     208,   289,   161,   159,     0,   139,   160,   288,   157,   208,
     157,   218,   289,   161,   159,     0,   139,   160,   288,   157,
     218,   161,   159,     0,   208,     0,   277,   157,   208,     0,
     276,     0,   278,   276,     0,   280,   158,   160,   277,   161,
       0,   280,   158,   209,     0,   160,    86,   284,   287,    12,
       3,   161,     0,   160,    86,   284,   287,    12,   160,   220,
     281,   218,   161,   161,     0,   160,    86,   284,   287,    20,
       3,   161,     0,   160,    86,   284,   287,    20,   160,   220,
     281,   218,   161,   161,     0,   160,    76,   284,   287,    12,
       3,   161,     0,   160,    76,   284,   287,    12,   160,   220,
     281,   218,   161,   161,     0,   160,    76,   284,   287,    20,
       3,   161,     0,   160,    76,   284,   287,    20,   160,   220,
     281,   218,   161,   161,     0,    21,     0,    22,     0,   144,
       0,   283,   158,   160,   277,   161,     0,   283,   158,   209,
       0,   283,   158,   160,     1,   161,     0,   160,   284,   287,
      12,   284,   161,     0,   160,   284,   287,    20,   284,   161,
       0,   160,     1,   161,     0,     3,     0,     3,   166,   220,
     167,     0,   284,   157,     3,     0,   284,   157,     3,   166,
     220,   167,     0,     3,   158,   218,     0,     3,   158,   218,
     144,   218,   144,   218,     0,     6,   158,   218,     0,     6,
     158,   160,   218,   157,   218,   161,     0,   285,     0,   286,
     157,   285,     0,   150,     0,   151,     0,     0,    86,   218,
       0,    76,   218,     0,    86,   220,   142,   218,     0,    76,
     220,   142,   218,     0,   220,   142,   218,     0,   220,     0,
       0,   290,     0,   157,     0,   157,   226,     0,   290,   157,
       0,   290,   157,   226,     0,     3,     0,    33,   234,   158,
     218,   159,     0,    42,   234,   159,     0,    58,   234,   158,
     218,   159,     0,    96,   234,   159,     0,    34,   294,    48,
       0,     0,    34,   144,     3,   292,   198,   294,    48,     0,
      34,    48,     0,    34,   144,     3,    48,     0,    34,     1,
      48,     0,     0,    60,   144,     3,   293,   198,   294,    69,
       0,    60,    69,     0,    60,   144,     3,    69,     0,    45,
     226,   159,     0,    30,   226,   159,     0,    59,   291,     0,
      60,   294,    69,     0,    97,   160,   218,   161,   291,     0,
      38,   160,   218,   161,   200,    49,     0,    39,   160,   218,
     161,   200,    49,     0,    40,   160,   218,   161,   200,    49,
       0,    38,   160,   218,   161,     1,    49,     0,    39,   160,
     218,   161,     1,    49,     0,    40,   160,   218,   161,     1,
      49,     0,    64,   160,   218,   161,   295,     0,    64,   160,
     218,   161,   295,    47,   295,     0,    64,   160,     1,   161,
     295,     0,    64,   160,     1,   161,   295,    47,   295,     0,
      57,   160,   235,   158,   218,   159,   218,   159,   235,   158,
     218,   161,   291,     0,    57,   160,   235,   158,   218,   159,
     218,   159,     1,   161,   291,     0,    57,   160,   235,   158,
     218,   159,     1,   159,   235,   158,   218,   161,   291,     0,
      57,   160,     1,   161,   291,     0,   129,   160,   218,   161,
     291,     0,   129,   160,     1,   161,   291,     0,   205,   295,
       0,   215,   295,     0,   163,   152,   295,     0,   163,   160,
     152,   161,   295,     0,   235,   158,   218,   159,     0,   235,
      10,   218,   159,     0,   235,   158,   205,   218,   159,     0,
     235,    10,   205,   218,   159,     0,   235,   158,   215,   218,
     159,     0,   235,   158,    97,   160,   218,   161,   215,   218,
     159,     0,   235,    10,   215,   218,   159,     0,   235,    10,
      97,   160,   218,   161,   215,   218,   159,     0,   125,   160,
     218,   161,   295,     0,     4,   160,   219,   161,   159,     0,
       4,   159,     0,   226,   160,   219,   161,   159,     0,   226,
     159,     0,     1,   159,     0,   294,   291,     0,   291,     0,
     291,     0,   159,     0,   196,     0,    67,   272,   227,   159,
       0,    83,   272,   227,   159,     0,    66,   272,   227,   159,
       0,   297,   296,     0,   296,     0,   297,     0,     0,     0,
     112,   300,   301,    54,     0,   303,     0,   304,     0,   308,
     144,   310,   159,     0,   302,     0,   303,   302,     0,   305,
       0,   304,   305,     0,   308,   144,   309,   144,   310,   159,
       0,    65,     3,   158,   192,   159,     0,   306,     0,     0,
     309,     0,   308,   309,     0,   171,     0,   172,     0,   173,
       0,   143,     0,   174,     0,   152,     0,   154,     0,   175,
       0,   176,     0,   177,     0,   178,     0,   179,     0,   180,
       0,   181,     0,   182,     0,   183,     0,   184,     0,   185,
       0,   186,     0,   187,     0,   188,     0,   189,     0,   150,
       0,   171,     0,   172,     0,   173,     0,   151,     0,    67,
     227,   159,     0,    83,     3,   159,     0,    95,     3,   159,
       0,   311,     0,   312,   311,     0,     3,     0,   313,   157,
       3,     0,    87,     3,   160,   313,   161,   159,   312,   307,
     299,    52,     0
};

#endif

#if YYDEBUG
/* YYRLINE[YYN] -- source line where rule number YYN was defined. */
static const short yyrline[] =
{
       0,   223,   223,   226,   227,   230,   232,   234,   242,   243,
     244,   248,   254,   263,   270,   293,   296,   299,   302,   305,
     308,   311,   312,   317,   321,   325,   329,   333,   337,   341,
     348,   349,   353,   354,   358,   365,   370,   375,   382,   388,
     396,   397,   398,   402,   403,   407,   421,   422,   426,   430,
     435,   443,   448,   453,   459,   469,   470,   474,   478,   484,
     495,   506,   516,   517,   518,   526,   530,   534,   538,   542,
     546,   553,   554,   558,   559,   560,   561,   565,   566,   567,
     568,   572,   583,   590,   597,   599,   605,   614,   622,   630,
     641,   643,   645,   651,   657,   663,   669,   675,   681,   687,
     693,   699,   704,   709,   715,   721,   727,   733,   739,   745,
     751,   757,   763,   769,   775,   781,   787,   793,   799,   805,
     811,   817,   823,   829,   835,   841,   847,   853,   866,   871,
     876,   882,   891,   898,   904,   910,   917,   924,   932,   941,
     948,   954,   956,   963,   985,   992,   999,  1007,  1015,  1024,
    1026,  1043,  1053,  1066,  1077,  1089,  1097,  1106,  1107,  1108,
    1109,  1110,  1111,  1112,  1113,  1114,  1115,  1116,  1117,  1118,
    1119,  1120,  1121,  1122,  1123,  1124,  1125,  1126,  1127,  1128,
    1129,  1137,  1141,  1153,  1158,  1183,  1189,  1198,  1204,  1210,
    1231,  1246,  1261,  1276,  1294,  1295,  1299,  1300,  1301,  1304,
    1304,  1310,  1317,  1332,  1343,  1355,  1362,  1371,  1380,  1390,
    1399,  1405,  1414,  1414,  1414,  1427,  1427,  1429,  1437,  1438,
    1442,  1443,  1444,  1449,  1458,  1467,  1475,  1481,  1489,  1493,
    1501,  1506,  1511,  1522,  1526,  1527,  1534,  1538,  1542,  1546,
    1553,  1557,  1562,  1566,  1574,  1583,  1588,  1594,  1606,  1606,
    1625,  1625,  1641,  1644,  1648,  1657,  1663,  1669,  1675,  1684,
    1689,  1695,  1696,  1700,  1701,  1713,  1723,  1729,  1735,  1736,
    1737,  1738,  1739,  1740,  1741,  1742,  1743,  1744,  1748,  1752,
    1768,  1769,  1769,  1774,  1774,  1782,  1783,  1793,  1807,  1808,
    1813,  1832,  1838,  1844,  1856,  1861,  1866,  1873,  1883,  1888,
    1911,  1919,  1930,  1939,  1948,  1949,  1965,  1973,  1995,  2012,
    2028,  2030,  2043,  2050,  2058,  2068,  2074,  2082,  2083,  2084,
    2088,  2105,  2106,  2111,  2112,  2113,  2114,  2115,  2116,  2125,
    2130,  2139,  2152,  2157,  2165,  2166,  2169,  2172,  2175,  2178,
    2182,  2186,  2190,  2194,  2199,  2204,  2209,  2215,  2216,  2220,
    2221,  2225,  2226,  2230,  2231,  2232,  2233,  2234,  2235,  2236,
    2237,  2241,  2242,  2243,  2247,  2248,  2249,  2256,  2259,  2262,
    2269,  2274,  2279,  2284,  2292,  2304,  2310,  2314,  2322,  2323,
    2327,  2328,  2329,  2333,  2335,  2337,  2341,  2345,  2349,  2354,
    2356,  2360,  2362,  2364,  2366,  2369,  2381,  2388,  2399,  2405,
    2418,  2425,  2425,  2437,  2443,  2449,  2457,  2457,  2469,  2475,
    2483,  2490,  2497,  2503,  2510,  2516,  2522,  2528,  2534,  2536,
    2538,  2540,  2546,  2552,  2556,  2560,  2567,  2572,  2577,  2581,
    2587,  2591,  2599,  2609,  2616,  2623,  2629,  2635,  2643,  2651,
    2657,  2665,  2672,  2680,  2687,  2695,  2703,  2711,  2719,  2727,
    2732,  2740,  2741,  2746,  2748,  2755,  2762,  2772,  2778,  2783,
    2785,  2790,  2790,  2796,  2797,  2801,  2814,  2820,  2829,  2835,
    2844,  2858,  2870,  2871,  2875,  2881,  2893,  2894,  2895,  2896,
    2897,  2898,  2899,  2900,  2901,  2902,  2903,  2904,  2905,  2906,
    2907,  2908,  2909,  2910,  2911,  2912,  2913,  2914,  2915,  2919,
    2920,  2921,  2922,  2926,  2928,  2934,  2943,  2945,  2954,  2960,
    2969
};
#endif


#if (YYDEBUG) || defined YYERROR_VERBOSE

/* YYTNAME[TOKEN_NUM] -- String name of the token TOKEN_NUM. */
static const char *const yytname[] =
{
  "$", "error", "$undefined.", "IDENTIFIER", "SYSTEM_IDENTIFIER", "STRING", 
  "PATHPULSE_IDENTIFIER", "BASED_NUMBER", "DEC_NUMBER", "REALTIME", 
  "K_LE", "K_GE", "K_EG", "K_EQ", "K_NE", "K_CEQ", "K_CNE", "K_LS", 
  "K_RS", "K_RSS", "K_SG", "K_PO_POS", "K_PO_NEG", "K_PSTAR", "K_STARP", 
  "K_LOR", "K_LAND", "K_NAND", "K_NOR", "K_NXOR", "K_TRIGGER", "K_always", 
  "K_and", "K_assign", "K_begin", "K_buf", "K_bufif0", "K_bufif1", 
  "K_case", "K_casex", "K_casez", "K_cmos", "K_deassign", "K_default", 
  "K_defparam", "K_disable", "K_edge", "K_else", "K_end", "K_endcase", 
  "K_endfunction", "K_endmodule", "K_endprimitive", "K_endspecify", 
  "K_endtable", "K_endtask", "K_event", "K_for", "K_force", "K_forever", 
  "K_fork", "K_function", "K_highz0", "K_highz1", "K_if", "K_initial", 
  "K_inout", "K_input", "K_integer", "K_join", "K_large", "K_localparam", 
  "K_macromodule", "K_medium", "K_module", "K_nand", "K_negedge", 
  "K_nmos", "K_nor", "K_not", "K_notif0", "K_notif1", "K_or", "K_output", 
  "K_parameter", "K_pmos", "K_posedge", "K_primitive", "K_pull0", 
  "K_pull1", "K_pulldown", "K_pullup", "K_rcmos", "K_real", "K_realtime", 
  "K_reg", "K_release", "K_repeat", "K_rnmos", "K_rpmos", "K_rtran", 
  "K_rtranif0", "K_rtranif1", "K_scalared", "K_signed", "K_small", 
  "K_specify", "K_specparam", "K_strong0", "K_strong1", "K_supply0", 
  "K_supply1", "K_table", "K_task", "K_time", "K_tran", "K_tranif0", 
  "K_tranif1", "K_tri", "K_tri0", "K_tri1", "K_triand", "K_trior", 
  "K_trireg", "K_vectored", "K_wait", "K_wand", "K_weak0", "K_weak1", 
  "K_while", "K_wire", "K_wor", "K_xnor", "K_xor", "K_Shold", "K_Speriod", 
  "K_Srecovery", "K_Srecrem", "K_Ssetup", "K_Swidth", "K_Ssetuphold", 
  "KK_attribute", "K_TAND", "'?'", "':'", "'|'", "'^'", "'&'", "'<'", 
  "'>'", "'+'", "'-'", "'*'", "'/'", "'%'", "UNARY_PREC", 
  "less_than_K_else", "','", "'='", "';'", "'('", "')'", "'#'", "'@'", 
  "'~'", "'!'", "'['", "']'", "'{'", "'}'", "'.'", "'0'", "'1'", "'x'", 
  "'b'", "'f'", "'F'", "'l'", "'h'", "'B'", "'r'", "'R'", "'M'", "'n'", 
  "'N'", "'p'", "'P'", "'Q'", "'q'", "'_'", "main", "source_file", 
  "number", "attribute_list_opt", "attribute_list", "attribute", 
  "block_item_decl", "block_item_decls", "block_item_decls_opt", 
  "case_item", "case_items", "charge_strength", "charge_strength_opt", 
  "defparam_assign", "defparam_assign_list", "delay1", "delay3", 
  "delay3_opt", "delay_value", "delay_value_simple", "description", 
  "drive_strength", "drive_strength_opt", "dr_strength0", "dr_strength1", 
  "event_control", "event_expression_list", "event_expression", 
  "expression", "expression_list", "expr_primary", "function_item", 
  "function_item_list", "gate_instance", "gate_instance_list", "gatetype", 
  "identifier", "list_of_identifiers", "list_of_ports", 
  "list_of_port_declarations", "port_declaration", "net_type_opt", 
  "list_of_ports_opt", "signed_opt", "lavalue", "lpvalue", "assign", 
  "assign_list", "module", "@1", "@2", "module_start", "range_delay", 
  "module_parameter_port_list_opt", "module_parameter_port_list", 
  "module_item", "@3", "@4", "module_item_list", "module_item_list_opt", 
  "net_decl_assign", "net_decl_assigns", "net_type", "var_type", 
  "parameter_assign", "parameter_assign_decl", "@5", "@6", 
  "parameter_assign_list", "localparam_assign", "localparam_assign_list", 
  "parameter_value_opt", "parameter_value_byname", 
  "parameter_value_byname_list", "port", "port_opt", "port_reference", 
  "port_reference_list", "port_name", "port_name_list", "port_type", 
  "range", "range_opt", "range_or_type_opt", "register_variable", 
  "register_variable_list", "specify_item", "specify_delay_value_list", 
  "specify_item_list", "specify_edge_path_decl", "specify_edge_path", 
  "polarity_operator", "specify_simple_path_decl", "specify_simple_path", 
  "specify_path_identifiers", "specparam", "specparam_list", 
  "spec_polarity", "spec_reference_event", "spec_notifier_opt", 
  "spec_notifier", "statement", "@7", "@8", "statement_list", 
  "statement_opt", "task_item", "task_item_list", "task_item_list_opt", 
  "udp_body", "@9", "udp_entry_list", "udp_comb_entry", 
  "udp_comb_entry_list", "udp_sequ_entry_list", "udp_sequ_entry", 
  "udp_initial", "udp_init_opt", "udp_input_list", "udp_input_sym", 
  "udp_output_sym", "udp_port_decl", "udp_port_decls", "udp_port_list", 
  "udp_primitive", 0
};
#endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives. */
static const short yyr1[] =
{
       0,   190,   190,   191,   191,   192,   192,   192,   193,   193,
     193,   194,   194,   195,   195,   196,   196,   196,   196,   196,
     196,   196,   196,   196,   196,   196,   196,   196,   196,   196,
     197,   197,   198,   198,   199,   199,   199,   199,   200,   200,
     201,   201,   201,   202,   202,   203,   204,   204,   204,   205,
     205,   206,   206,   206,   206,   207,   207,   208,   208,   209,
     209,   209,   210,   210,   210,   211,   211,   211,   211,   211,
     211,   212,   212,   213,   213,   213,   213,   214,   214,   214,
     214,   215,   215,   215,   216,   216,   216,   217,   217,   217,
     218,   218,   218,   218,   218,   218,   218,   218,   218,   218,
     218,   218,   218,   218,   218,   218,   218,   218,   218,   218,
     218,   218,   218,   218,   218,   218,   218,   218,   218,   218,
     218,   218,   218,   218,   218,   218,   218,   218,   219,   219,
     219,   219,   220,   220,   220,   220,   220,   220,   220,   220,
     220,   220,   220,   220,   221,   221,   221,   221,   221,   222,
     222,   223,   223,   223,   223,   224,   224,   225,   225,   225,
     225,   225,   225,   225,   225,   225,   225,   225,   225,   225,
     225,   225,   225,   225,   225,   225,   225,   225,   225,   225,
     225,   226,   226,   227,   227,   228,   228,   229,   229,   229,
     230,   230,   230,   230,   231,   231,   232,   232,   232,   233,
     233,   234,   234,   234,   234,   235,   235,   235,   235,   236,
     237,   237,   239,   240,   238,   241,   241,   242,   243,   243,
     244,   244,   244,   245,   245,   245,   245,   245,   245,   245,
     245,   245,   245,   245,   245,   245,   245,   245,   245,   245,
     245,   245,   245,   245,   245,   245,   245,   245,   246,   245,
     247,   245,   245,   245,   245,   245,   245,   245,   245,   245,
     245,   248,   248,   249,   249,   250,   251,   251,   252,   252,
     252,   252,   252,   252,   252,   252,   252,   252,   253,   254,
     255,   256,   255,   257,   255,   258,   258,   259,   260,   260,
     260,   261,   261,   261,   261,   261,   262,   262,   263,   263,
     264,   264,   264,   264,   265,   265,   266,   266,   266,   266,
     267,   267,   268,   268,   268,   269,   269,   270,   270,   270,
     271,   272,   272,   273,   273,   273,   273,   273,   273,   274,
     274,   274,   275,   275,   276,   276,   276,   276,   276,   276,
     276,   276,   276,   276,   276,   276,   276,   277,   277,   278,
     278,   279,   279,   280,   280,   280,   280,   280,   280,   280,
     280,   281,   281,   281,   282,   282,   282,   283,   283,   283,
     284,   284,   284,   284,   285,   285,   285,   285,   286,   286,
     287,   287,   287,   288,   288,   288,   288,   288,   288,   289,
     289,   290,   290,   290,   290,   290,   291,   291,   291,   291,
     291,   292,   291,   291,   291,   291,   293,   291,   291,   291,
     291,   291,   291,   291,   291,   291,   291,   291,   291,   291,
     291,   291,   291,   291,   291,   291,   291,   291,   291,   291,
     291,   291,   291,   291,   291,   291,   291,   291,   291,   291,
     291,   291,   291,   291,   291,   291,   291,   291,   291,   294,
     294,   295,   295,   296,   296,   296,   296,   297,   297,   298,
     298,   300,   299,   301,   301,   302,   303,   303,   304,   304,
     305,   306,   307,   307,   308,   308,   309,   309,   309,   309,
     309,   309,   309,   309,   309,   309,   309,   309,   309,   309,
     309,   309,   309,   309,   309,   309,   309,   309,   309,   310,
     310,   310,   310,   311,   311,   311,   312,   312,   313,   313,
     314
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN. */
static const short yyr2[] =
{
       0,     1,     0,     1,     2,     1,     1,     2,     3,     2,
       0,     3,     1,     1,     3,     5,     4,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       1,     2,     1,     0,     3,     3,     2,     3,     2,     1,
       3,     3,     3,     1,     0,     3,     1,     2,     3,     2,
       4,     2,     4,     6,     8,     1,     0,     1,     5,     1,
       1,     1,     1,     1,     8,     5,     5,     5,     5,     5,
       5,     1,     0,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     4,     4,     1,     3,     3,     2,     2,     1,
       1,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     5,     3,     1,
       0,     2,     1,     1,     1,     1,     1,     4,     6,     4,
       4,     3,     3,     6,     4,     5,     4,     4,     1,     1,
       2,     4,     5,     3,     4,     3,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     1,     3,     1,     3,     1,     3,     3,
       6,     6,     6,     6,     1,     0,     3,     3,     0,     1,
       0,     1,     4,     2,     3,     1,     4,     6,     3,     3,
       3,     1,     0,     0,    10,     1,     1,     2,     0,     4,
       2,     3,     4,     6,     6,     6,     5,     5,     6,     6,
       6,     6,     5,     1,     3,     3,     4,     5,     5,     6,
       3,     3,     6,     6,     4,     5,     3,     3,     0,     7,
       0,     8,     2,     3,     3,     2,     5,     3,     3,     9,
       5,     2,     1,     1,     0,     3,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     3,
       1,     0,     3,     0,     4,     1,     3,     3,     1,     2,
       3,     4,     4,     2,     2,     0,     5,     4,     1,     3,
       1,     5,     3,     7,     1,     0,     1,     6,     4,     4,
       1,     3,     5,     5,     4,     3,     1,     1,     1,     1,
       5,     1,     0,     1,     1,     1,     1,     1,     0,     1,
       3,     6,     1,     3,     3,     2,     2,     6,     6,    10,
       8,    10,    10,    12,    12,    10,     7,     1,     3,     1,
       2,     5,     3,     7,    11,     7,    11,     7,    11,     7,
      11,     1,     1,     1,     5,     3,     5,     6,     6,     3,
       1,     4,     3,     6,     3,     7,     3,     7,     1,     3,
       1,     1,     0,     2,     2,     4,     4,     3,     1,     0,
       1,     1,     2,     2,     3,     1,     5,     3,     5,     3,
       3,     0,     7,     2,     4,     3,     0,     7,     2,     4,
       3,     3,     2,     3,     5,     6,     6,     6,     6,     6,
       6,     5,     7,     5,     7,    13,    11,    13,     5,     5,
       5,     2,     2,     3,     5,     4,     4,     5,     5,     5,
       9,     5,     9,     5,     5,     2,     5,     2,     2,     2,
       1,     1,     1,     1,     4,     4,     4,     2,     1,     1,
       0,     0,     4,     1,     1,     4,     1,     2,     1,     2,
       6,     5,     1,     0,     1,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     3,     3,     1,     2,     1,     3,
      10
};

/* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
   doesn't specify something else to do.  Zero means the default is an
   error. */
static const short yydefact[] =
{
      10,     0,     0,     0,    10,     0,     3,    62,    63,    13,
       9,     0,    12,     0,     0,     4,   216,   215,     0,     0,
       8,     0,     0,     0,   212,   181,   136,   134,     5,     6,
     133,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   130,   132,    14,    90,   135,    11,   508,     0,
       0,   218,   130,     7,    98,    99,   100,    96,   102,    97,
      94,    91,    92,     0,    93,   101,    95,   129,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   130,     0,     0,     0,     0,     0,
       0,   198,   129,     0,   141,   130,   131,   142,   121,   122,
     119,   123,   120,   124,   116,   117,   118,   125,   126,   111,
     112,   113,     0,   110,   103,   109,   114,   115,   107,   108,
     104,   105,   106,     0,     0,   182,   509,     0,     0,     0,
      10,     0,   140,     0,   128,     0,   139,     0,   137,     0,
       0,     0,   506,   473,     0,     0,     0,   306,     0,     0,
       0,     0,     0,   187,   304,   185,   300,   213,     0,   127,
       0,   183,     0,     0,     0,     0,   472,     0,   507,    64,
       0,   220,     0,   219,     0,   310,     0,     0,   195,   195,
     195,   305,   196,    10,   197,     0,   143,   138,     0,   503,
     504,   505,     0,   461,     0,     0,     0,   221,     0,     0,
       0,   302,     0,   271,   275,   269,   274,   270,   273,   277,
     272,   268,   276,   200,   194,   200,   278,   200,   200,   186,
     189,   188,     0,   295,     0,     0,     0,     0,   319,   317,
       0,     0,   318,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    44,     0,     0,   233,   262,     0,     0,   200,
     184,     0,     0,   510,   279,   222,   309,     0,   308,   311,
       0,     0,   199,   322,   322,   322,   322,   255,     0,     0,
       0,     0,    71,    56,     0,    46,     0,     0,     0,     0,
       0,   324,   325,   326,   327,   323,     0,   200,   200,     0,
     329,   332,     0,     0,     0,   288,     0,     0,   200,     0,
       0,   285,     0,   280,   281,     0,   130,   156,     0,   130,
       0,     0,     0,     0,     0,     0,     0,     0,   252,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   349,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    43,
     322,     0,     0,   157,   163,   164,   165,   173,     0,   158,
     169,   160,   166,   167,   168,   159,   171,   174,   170,   172,
     176,   179,   180,   175,   177,   178,   162,   161,     0,   200,
     261,   214,   322,   200,     0,   479,   498,   481,   482,   476,
     477,   478,   480,   483,   484,   485,   486,   487,   488,   489,
     490,   491,   492,   493,   494,   495,   496,   497,     0,   466,
     463,   464,   468,     0,   474,     0,     0,   301,   321,     0,
       0,     0,     0,   294,   293,   130,   130,     0,     0,   257,
       0,     0,    75,    79,    74,    78,    73,    77,    76,    80,
       0,     0,     0,    55,     0,     0,     0,   234,     0,    47,
     235,   258,     0,   322,   322,    24,     0,     0,     0,    17,
      29,     0,     0,    22,   289,   322,    28,   283,    21,     0,
       0,   130,     0,     0,     0,     0,   241,     0,   240,    26,
      19,    27,    20,    23,     0,     0,   254,     0,     0,     0,
     378,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     370,     0,     0,   382,   253,   350,   336,     0,   335,     0,
     248,    25,    18,     0,     0,     0,     0,    56,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   130,     0,     0,   205,     0,   246,   247,   130,     0,
       0,     0,   322,     0,   322,   471,   462,   467,     0,   469,
       0,     0,   475,   307,     0,   191,   190,   192,   193,     0,
       0,   298,     0,   244,     0,     0,     0,     0,     0,    61,
      59,    60,     0,    51,   130,   201,     0,   211,     0,     0,
      48,    45,   250,     0,     0,   330,     0,   333,   287,   290,
       0,     0,   286,   282,     0,     0,   316,     0,   130,     0,
     153,   155,     0,     0,    16,     0,     0,     0,     0,   334,
       0,     0,   388,     0,     0,     0,     0,     0,     0,     0,
     369,     0,   382,   382,   380,   381,     0,     0,     0,   352,
       0,   365,   460,    42,    41,    40,     0,   217,     0,     0,
     448,   445,   130,     0,     0,     0,   403,     0,   450,     0,
       0,     0,     0,     0,     0,     0,     0,   412,   408,     0,
       0,     0,     0,     0,     0,     0,     0,    49,     0,     0,
      81,     0,   452,   451,   431,   432,   447,   130,     0,     0,
       0,     0,     0,     0,   236,     0,     0,     0,     0,     0,
       0,     0,   502,   476,   477,   478,     0,     0,   303,     0,
     291,     0,   292,   256,     0,     0,     0,     0,     0,     0,
       0,    57,     0,     0,   203,     0,     0,   245,     0,     0,
       0,     0,     0,     0,   284,     0,   151,     0,   154,     0,
       0,     0,    15,     0,   374,     0,   376,   379,   384,    90,
     383,    90,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   372,     0,     0,   347,     0,     0,     0,
     322,   322,   322,   453,   458,   459,     0,   226,   260,     0,
       0,   411,     0,   405,   401,   400,   449,     0,     0,     0,
     397,   410,     0,   205,     0,     0,   406,   413,     0,     0,
     399,     0,     0,     0,     0,     0,   433,     0,     0,     0,
       0,     0,    84,    89,   208,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   237,     0,   238,     0,
     267,     0,   183,     0,     0,   232,   227,     0,   499,   500,
     501,     0,   465,     0,   299,    70,    69,    67,    65,    68,
      66,     0,    52,     0,   204,     0,   209,   210,   320,   322,
     322,   322,   148,   149,     0,   231,   230,     0,   229,     0,
     315,   152,   243,   242,     0,     0,     0,     0,     0,     0,
     387,     0,   389,     0,     0,     0,     0,    57,     0,   371,
       0,     0,     0,     0,     0,     0,     0,     0,   351,   366,
     364,     0,     0,     0,   457,     0,     0,     0,     0,   404,
      33,     0,     0,     0,     0,     0,     0,   409,    33,     0,
       0,     0,     0,     0,     0,    50,    83,    88,    87,     0,
       0,     0,    82,     0,     0,   206,     0,     0,     0,     0,
     436,     0,     0,     0,   435,   239,     0,     0,   225,   223,
     224,   228,     0,   297,     0,     0,     0,   202,     0,   322,
       0,     0,   150,     0,   331,     0,   314,     0,   338,   337,
       0,     0,   386,   385,     0,   395,   391,     0,   390,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   367,   368,   348,     0,     0,     0,
     249,     0,   444,   396,    30,    32,     0,     0,     0,    39,
       0,     0,     0,     0,     0,     0,   428,     0,   398,     0,
     423,   421,   414,   443,   430,   429,   434,    85,    86,   446,
       0,     0,   438,   441,     0,   437,   439,   265,   266,   470,
     296,     0,    53,     0,     0,     0,     0,     0,   251,   313,
     312,     0,     0,   389,   392,     0,   393,   389,     0,   389,
     389,   346,     0,   357,     0,   359,     0,   353,     0,   355,
       0,   373,   456,   454,   455,     0,    31,     0,   418,     0,
       0,    36,     0,   415,    38,     0,   419,   416,   420,   417,
       0,     0,     0,     0,   207,     0,     0,     0,    58,   147,
       0,   144,   146,   375,   377,     0,   340,   394,     0,     0,
       0,     0,     0,   361,   362,   363,     0,     0,     0,     0,
     259,   402,    37,    35,    34,     0,     0,   407,   424,   422,
       0,     0,    54,   145,     0,     0,   389,     0,     0,   389,
       0,     0,     0,     0,     0,     0,     0,     0,   339,   341,
       0,   342,   345,     0,     0,     0,     0,     0,     0,     0,
       0,   442,   440,     0,     0,   358,   360,   354,   356,     0,
       0,     0,   344,   343,     0,   426,     0,     0,     0,   427,
     425,     0,     0,     0
};

static const short yydefgoto[] =
{
    1181,     4,    43,     5,    11,    12,   255,  1005,  1006,  1009,
    1010,   349,   350,   285,   286,   542,   443,   444,   766,   583,
       6,   282,   283,   440,   441,   543,   811,   812,   102,  1011,
      45,   863,   864,   317,   318,   378,    46,   172,   161,   162,
     163,   223,   141,   273,   586,   545,   587,   588,     7,    51,
     195,    18,   516,   101,   156,   256,   642,   729,   257,   258,
     830,   831,   224,   228,   311,   312,   470,   601,   313,   305,
     306,   279,   571,   572,   164,   165,   166,   186,   606,   607,
     259,   418,   517,   296,   301,   302,   339,   767,   340,   341,
     342,  1116,   343,   344,   503,   490,   491,   637,   623,   977,
     978,   683,   910,   918,   659,   684,   774,   775,   776,   204,
     262,   408,   409,   410,   411,   412,   176,   177,   413,   414,
     707,   152,   153,    49,     8
};

static const short yypact[] =
{
     222,   187,    18,   -50,   336,   445,-32768,-32768,-32768,   -18,
  -32768,   109,-32768,    53,   186,-32768,-32768,-32768,   231,  2569,
  -32768,   259,   284,   226,-32768,-32768,   230,-32768,-32768,   396,
  -32768,   138,   138,   138,   138,   100,   138,   138,   138,  2569,
     138,   115,  2569,-32768,  4859,-32768,   493,-32768,-32768,   289,
     407,   269,  2569,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,  3293,-32768,-32768,-32768,  2619,    -6,  2569,
    2569,  2569,  2569,  2569,  2569,  2569,  2569,  2569,  2569,  2569,
    2569,  2569,  2569,  2569,  2569,  2569,  2569,  2569,  2569,  2569,
    2569,  2569,  2569,  2569,  2569,  2569,   473,   494,   310,   403,
     406,   423,  4859,   355,-32768,  2569,  2569,-32768,   320,   320,
     356,   356,   356,   356,   542,   542,   542,  4879,  5024,   926,
     618,   618,  4594,  1032,   618,   926,   320,   320,   463,   463,
  -32768,-32768,-32768,   392,  2645,-32768,-32768,   344,   605,   567,
     127,   471,-32768,    59,  4859,  2569,-32768,  2569,-32768,   635,
     658,   664,-32768,   331,   511,   675,   410,   522,   715,   758,
     368,   411,   412,-32768,-32768,-32768,-32768,-32768,   561,  4859,
    2863,-32768,   466,   615,   623,   784,-32768,   687,-32768,-32768,
     645,-32768,   136,-32768,  2009,-32768,   189,   649,   881,   881,
     555,    76,-32768,   345,-32768,  5013,-32768,-32768,   827,-32768,
  -32768,-32768,   685,-32768,   766,  2569,   675,-32768,   698,  2790,
     715,-32768,    79,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,   765,-32768,   765,-32768,   765,   765,-32768,
  -32768,-32768,   714,   730,    93,    29,   635,   214,   781,   781,
     686,   151,   781,    90,   133,   161,   716,   737,   177,   318,
     877,   761,   753,   786,  5259,-32768,-32768,  5156,   883,   779,
  -32768,   527,  5431,-32768,  4859,-32768,-32768,  2569,-32768,-32768,
     715,   828,-32768,   842,   842,   842,   842,-32768,   130,   163,
     540,   437,-32768,   822,  2569,-32768,   616,   191,   991,   629,
     960,-32768,-32768,-32768,-32768,-32768,  1010,   765,   765,   855,
     -52,-32768,   655,   857,   860,-32768,   656,  1017,   765,   862,
     842,-32768,   864,   868,-32768,   156,  2335,-32768,   660,  2364,
     666,   867,   669,   869,   672,   870,   114,   974,-32768,   871,
     750,   873,   875,   876,   878,   879,   880,   884,   446,-32768,
    1081,   882,   872,   895,   897,   898,   903,   676,   272,-32768,
     842,   835,  1614,-32768,-32768,-32768,-32768,-32768,  1614,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,    92,   765,
  -32768,-32768,   842,   765,   904,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,   983,-32768,
    5431,  5431,-32768,  5243,-32768,  2886,   203,-32768,-32768,  1053,
    1061,  1063,  1064,-32768,-32768,  2021,  2569,   680,  2569,-32768,
     911,   912,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
     913,   915,   153,-32768,    99,  4614,   991,-32768,  2569,-32768,
  -32768,-32768,   914,   842,   842,-32768,  2569,  2569,  1079,-32768,
  -32768,  2569,  1017,-32768,-32768,   842,-32768,-32768,-32768,   675,
     675,  2051,   925,   927,   483,   163,-32768,   928,-32768,-32768,
  -32768,-32768,-32768,-32768,  1079,   683,-32768,  2569,   929,   933,
  -32768,   688,   234,   234,   234,   234,   234,   234,   234,   931,
     920,  1091,  1091,   370,-32768,-32768,-32768,   164,-32768,   173,
  -32768,-32768,-32768,   934,   935,   940,   635,   822,   941,   946,
     945,   632,   991,    99,   890,   947,   948,   949,    99,   991,
     956,    99,  1614,  1168,   957,    99,   961,   963,   964,   190,
     152,  2569,   167,   167,   452,    75,-32768,-32768,  2287,   163,
     129,   689,   247,   848,   842,-32768,-32768,-32768,  5290,-32768,
    5337,  5384,-32768,-32768,   944,-32768,-32768,-32768,-32768,  1103,
     501,-32768,   507,-32768,  4080,   280,   309,   448,   467,-32768,
  -32768,-32768,  2569,-32768,  2569,   513,   968,-32768,   695,  2569,
  -32768,  4859,-32768,   635,   635,  4859,  4634,-32768,  4859,-32768,
     635,   675,-32768,   868,  1124,   523,-32768,   546,  2569,   163,
  -32768,-32768,   163,   696,-32768,  3313,  2569,  2585,   750,-32768,
    2569,  2569,   986,   972,   973,   975,   976,   978,   979,   984,
  -32768,   138,   370,   370,-32768,-32768,  1128,   239,  2569,-32768,
    2083,-32768,   713,-32768,-32768,-32768,   699,-32768,   990,  1137,
  -32768,-32768,  2569,    70,   985,    77,-32768,  1150,-32768,  1340,
    2569,  2569,  2569,   995,   206,    35,   997,-32768,-32768,  1153,
    1422,  2094,   999,  2569,  2569,  2260,  2569,-32768,   167,  1751,
     989,   327,-32768,-32768,-32768,-32768,-32768,  2569,  2569,  2375,
    2460,   707,   163,   718,-32768,  1157,  1158,  1003,   729,   635,
     436,  5431,-32768,  1006,  1007,  1008,  1024,  1011,-32768,  1027,
  -32768,  1019,-32768,-32768,  1029,  1030,  1031,  1034,  1035,  1036,
     548,  4779,   393,  2569,-32768,  2569,    99,-32768,  3031,   727,
     747,   757,  2569,   762,   868,  1033,-32768,  1041,-32768,   551,
     774,   794,-32768,  1039,  4799,  2569,  4859,-32768,  4859,  1058,
    4859,  1062,  2569,   234,  2569,   234,   234,   234,  2569,   234,
    1045,   398,   420,  1048,  1091,  1091,-32768,   553,  1068,   554,
     842,   842,   842,-32768,-32768,   713,   167,-32768,-32768,  1065,
     559,-32768,  2569,-32768,  1175,-32768,-32768,  3333,  3353,  3373,
  -32768,-32768,  1069,   566,  1066,  2569,  1162,-32768,  1072,  3393,
  -32768,  3542,  3562,  1073,  3582,  1074,-32768,  1075,  2569,  2569,
    1077,   243,-32768,  4859,-32768,   568,  2815,  1085,   171,  2569,
    2569,  4100,  1089,  2569,  2569,  4120,-32768,   799,-32768,  1082,
  -32768,   800,  1082,   807,   810,-32768,-32768,   811,-32768,-32768,
  -32768,   436,-32768,  2557,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,  2569,-32768,  2569,-32768,  2840,  4859,-32768,-32768,   842,
      44,   842,-32768,-32768,  1080,-32768,-32768,  3054,-32768,  1980,
  -32768,-32768,-32768,-32768,  1092,  1093,  2569,  3124,  2569,  2569,
    4859,  1097,  3104,  1098,  1099,  1100,  1109,  3144,  1110,-32768,
     172,   174,   180,   184,   138,   586,   594,  2569,-32768,-32768,
  -32768,   635,   635,   635,-32768,  1184,  1245,  1111,  4140,-32768,
     447,  1783,  1812,  1917,  1614,  2569,  4287,-32768,   447,   167,
     167,  1614,   167,  1614,  1614,-32768,-32768,  4859,  4859,   167,
    2541,  2541,-32768,  1112,  2569,-32768,  2569,  1855,  4307,  4327,
  -32768,  2569,  4347,  4367,-32768,-32768,  2569,  1157,-32768,-32768,
  -32768,-32768,  1113,-32768,  3602,   597,  4819,-32768,   635,   842,
     635,   635,-32768,  1219,-32768,  1114,-32768,  3622,-32768,-32768,
    4839,  2569,  4859,  4859,  2569,-32768,   991,  1117,  1122,  2569,
    2569,  2569,  2569,  1115,  2569,  1119,   138,  1123,   138,  1125,
     138,  1126,   138,  1127,-32768,-32768,-32768,   814,   815,   818,
  -32768,  1129,-32768,-32768,-32768,   447,  1614,    64,  1243,-32768,
     577,   -46,    86,  1678,   183,  1721,-32768,  4387,-32768,  1614,
    1242,  1244,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
    3077,  3642,-32768,-32768,  3791,-32768,-32768,  4859,-32768,-32768,
  -32768,  2569,-32768,  2569,   819,   635,   823,   831,-32768,-32768,
  -32768,  2569,  3811,  3104,   989,  1133,   991,  3104,  1138,  3104,
    3104,-32768,  1139,-32768,   197,-32768,   197,-32768,   197,-32768,
     197,-32768,-32768,-32768,-32768,  1140,-32768,  1495,-32768,   167,
     167,-32768,  1154,-32768,-32768,   167,-32768,-32768,-32768,-32768,
    2271,  1541,   167,   167,-32768,  1141,  1141,  1144,  4859,-32768,
     836,-32768,-32768,  4859,-32768,  1145,-32768,   989,  1147,  2569,
    1148,  1149,  2569,-32768,-32768,-32768,  2569,  2569,  2569,  2569,
  -32768,-32768,-32768,-32768,-32768,  1152,  4534,-32768,-32768,-32768,
    2569,  2569,-32768,-32768,  1155,  1159,   199,  1160,  1161,   199,
    3831,  3851,  3871,  3891,   118,   126,  4554,  4574,-32768,-32768,
    1156,-32768,-32768,  1163,  1164,  1165,  1166,  1167,  1171,  1172,
    1174,-32768,-32768,  1176,  1178,-32768,-32768,-32768,-32768,  2569,
    1614,  2569,-32768,-32768,  4040,-32768,  4060,  1614,  1614,-32768,
  -32768,  1313,  1316,-32768
};

static const short yypgoto[] =
{
  -32768,-32768,  1060,   266,-32768,  1301,  -641,-32768,   405,  -550,
    -135,-32768,-32768,  -176,-32768,   195,  -359,   817,  -579,  -152,
    1334,  -364,-32768,  -305,  -301,  -680,-32768,   -23,   -19,    -5,
      -7,   478,-32768,   874,  -164,-32768,   339,  -201,-32768,-32768,
    1169,   736,-32768,   216,    65,  -652,   620,-32768,-32768,-32768,
  -32768,-32768,  -360,-32768,-32768,  1094,-32768,-32768,-32768,-32768,
     401,   654,  -110,   241,  -107,-32768,-32768,-32768,  -380,  -258,
  -32768,-32768,   641,-32768,-32768,  1170,   212,  1083,   617,-32768,
  -32768,  -157,  -269,-32768,   899,  -234,  1015,   719,-32768,   613,
  -32768,  -512,   621,-32768,  -486,   742,-32768,   364,  -454,  -706,
  -32768,  -275,-32768,-32768,  -525,  -541,   588,-32768,-32768,-32768,
  -32768,-32768,   955,-32768,-32768,   958,-32768,-32768,   595,  -401,
     525,  1214,-32768,-32768,-32768
};


#define	YYLAST		5620


static const short yytable[] =
{
      44,   773,   685,   720,   419,   420,   421,   422,   670,   820,
     824,   473,   562,   794,   550,   632,   633,   347,   477,   549,
      63,    13,   553,    67,    54,    55,    56,    57,    59,    60,
      61,    62,    25,    64,    66,   289,   792,    68,    25,   624,
     625,   626,   627,   628,   629,   322,   324,   103,   181,   464,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   207,   134,   546,   288,   157,
     295,   320,   157,   547,   307,   689,   314,   144,   862,   133,
     603,   309,   485,   180,   280,   315,   -72,   805,  1085,   265,
     143,    58,    25,    25,    26,    27,   456,    28,    29,    30,
      14,   106,   449,  1078,   457,   427,    65,   300,    25,    26,
      27,    25,    28,    29,    30,   783,   169,  1159,   170,    25,
     157,   423,   315,    20,   773,  1086,   315,   806,   424,   180,
      19,    25,    26,    27,   379,    28,    29,    30,   959,   383,
       1,   106,   303,   467,   304,    25,   579,   562,   472,   562,
     706,   580,   581,   107,   315,   209,   315,   579,   520,   484,
      25,   521,   580,   581,    25,   985,   579,   987,   325,   886,
    -200,   580,   581,   989,   593,   594,   264,   991,   695,    23,
       9,   692,   696,   579,   310,   284,   600,   522,   580,   581,
     523,   524,   975,   541,   599,   525,   526,   527,  1079,   528,
     284,    10,   529,    22,   551,   290,   106,  -328,  1113,  1114,
     206,   734,    -2,   862,   530,   531,   532,   533,   168,   781,
    1079,   534,  1088,   690,    24,   905,   650,    25,    26,    27,
      96,    28,    29,    30,   158,     1,   159,   270,   415,   658,
     613,   764,   548,   281,   442,   -72,   284,   667,   658,   765,
      39,   -72,     9,   535,   536,   445,    21,   584,    42,  1004,
     590,   715,   955,   719,   714,    39,   717,  1004,   895,   896,
     284,   272,   291,    42,  -305,   699,   541,    48,  -305,   426,
     425,   442,   537,   316,   541,   158,   538,   159,    39,   881,
     706,   883,   884,   885,   678,   888,    42,   292,   293,     2,
     620,   474,   679,   582,   474,   646,   471,   284,   996,   327,
     621,   319,   284,   426,   638,   930,   682,  1079,   294,   539,
     540,   937,   986,   640,   988,   541,    -1,    75,    76,    77,
     990,  1115,   513,  -200,   992,   514,   210,  1105,   230,   448,
     676,  1108,   698,  1110,  1111,   639,   976,   641,   211,     1,
     210,    96,   602,     3,  1076,   791,    69,    70,     1,   433,
     185,   328,   564,    75,    76,    77,    96,   515,  1020,  1021,
     284,  1023,   329,    50,   786,   691,   693,   677,  1026,   435,
      52,   437,   730,   731,    39,   786,   175,   432,   149,   733,
     931,  1058,    42,    53,   932,  1062,   160,   281,   439,   574,
     890,   149,    99,   284,   150,  1130,  1131,   434,   891,   436,
     570,   474,   269,     2,   271,   330,   151,   150,   724,   591,
    1150,   100,   892,  1153,   188,   189,   438,   595,   596,   151,
     893,   274,   598,   275,   276,   740,    97,   499,   741,   500,
      98,   190,   331,   332,   333,   334,   335,   336,   337,   160,
    1084,   254,  1097,  1084,   326,  1084,   605,  1081,   615,   137,
      89,    90,    91,    92,    93,   382,   135,     3,   338,   297,
     298,  1077,   185,   308,   106,   622,   622,   622,   622,   622,
     622,   622,  1158,  1160,  1091,   833,   814,   136,   837,   430,
     431,   901,   902,   903,    87,    88,    89,    90,    91,    92,
      93,   716,   106,   453,   454,   240,   142,    16,   241,    17,
     634,   635,   501,   254,   465,   432,   433,   636,   827,   718,
    1136,   243,   502,  1139,    28,    29,   681,   433,  1122,  1123,
     246,   247,   248,   474,  1124,   434,   435,   436,   437,   106,
     106,  1128,  1129,   146,  1117,   432,  1118,   435,  1119,   437,
     138,   251,   854,   721,   438,   439,   139,   182,   191,   193,
     728,   183,   192,   194,   287,   434,   439,   436,  1082,   722,
      25,    26,    27,   140,    28,    29,    30,   702,   654,   963,
     958,   960,   961,   663,   438,   552,   666,   744,   746,   554,
     672,   748,   750,   739,    31,    32,    33,   838,   839,   840,
     154,   686,   687,   749,   751,    91,    92,    93,   688,   721,
    1008,   721,    96,   198,   760,   199,  1083,   287,    69,    70,
     167,    71,    72,    73,    74,    75,    76,    77,   171,  1016,
     106,   787,   788,   789,   610,    80,  1022,   780,  1024,  1025,
     226,   155,   799,    94,   801,   802,   804,   721,   106,    95,
     813,   173,   710,    96,   711,   213,   214,   174,   712,   816,
     821,   825,   179,   215,   216,   217,   218,   219,   180,   723,
     106,   220,   815,    96,   736,   221,   222,   299,   184,   300,
    1045,   544,    89,    90,    91,    92,    93,   544,   428,   429,
     997,   998,   999,   737,   855,   851,   856,   738,   106,   852,
     897,   897,   871,   867,   898,   900,   106,   321,   157,   171,
     907,  -130,    34,    35,    36,   106,   877,    37,    38,   933,
     196,   658,   688,   880,  -130,   882,    96,    39,   323,   887,
     171,    40,    41,   636,   658,    42,   622,   994,   622,   622,
     622,   636,   622,   488,  1041,   995,   489,  1044,  1042,  1046,
    1047,   187,   346,   908,   300,    86,    87,    88,    89,    90,
      91,    92,    93,   446,   200,   447,   916,  1013,  1015,   770,
     771,   240,   201,   585,   241,   287,   198,   202,   450,   927,
     928,   651,   652,   859,   860,   240,   772,   243,   241,   203,
     938,   939,   786,   205,   942,   943,   246,   247,   248,   212,
     861,   243,   458,   462,   459,   463,   786,   475,   263,   476,
     246,   247,   248,   475,   954,   478,   198,   251,   480,   198,
     260,   482,   721,   458,   956,   512,   518,   475,   519,   573,
     458,   251,   614,   261,  1100,   618,   475,   619,   694,   697,
     967,   171,   726,   458,   727,   742,   198,   970,   777,   972,
     973,   653,   585,   544,   475,   266,   826,   585,   664,   272,
     585,   544,   544,   277,   585,   475,   226,   828,   721,   680,
     345,   544,   544,   272,   819,   823,   198,   993,   836,   213,
     214,   655,   278,    25,   521,  1175,  1017,   215,   216,   217,
     218,   219,  1179,  1180,   198,   220,   865,  1027,  1028,   221,
     222,   813,   813,   348,   198,  1030,   866,  1031,   813,   198,
     522,   868,  1034,   523,   524,   225,   227,  1037,   525,   526,
     527,   475,   528,   872,   381,   529,    69,    70,   656,    71,
      72,    73,    74,    75,    76,    77,   351,   530,   531,   532,
     533,   475,  1052,   873,   534,  1053,   475,   947,   945,   948,
    1057,   721,  1059,  1060,   198,   721,   949,   947,   198,   950,
     951,   198,   198,  1072,  1073,   198,   198,  1074,  1099,  1064,
     198,  1066,  1101,  1068,   442,  1070,   535,   536,   198,   417,
    1102,   213,   214,   198,    25,  1133,   761,   762,   544,   215,
     216,   217,   218,   219,   793,   558,   560,   220,   284,   544,
     451,   221,   222,   452,   455,   537,   460,   544,   461,   538,
     304,   466,   721,   468,  1098,   469,   479,   486,   481,   483,
     507,   487,  1103,   492,   657,   493,   494,   556,   495,   496,
     497,   506,    69,    70,   498,    71,    72,    73,    74,    75,
      76,    77,   539,   540,   508,   509,   565,   510,   541,    80,
      81,    82,   511,   555,   566,   585,   567,   568,   575,   576,
     577,  1126,   578,   592,    87,    88,    89,    90,    91,    92,
      93,   520,   300,    25,   521,   608,   631,   616,   609,   612,
     721,   617,   630,   721,   500,   643,   644,  1140,  1141,  1142,
    1143,   645,   648,   649,   650,   708,   709,   660,   661,   662,
     522,  1146,  1147,   523,   524,   544,   665,   671,   525,   526,
     527,   673,   528,   674,   675,   529,   725,   735,   752,   753,
     754,   763,   755,   756,   504,   757,   758,   530,   531,   532,
     533,   759,   779,   782,   534,   329,   859,   860,   240,   778,
    1174,   241,  1176,   784,   790,   795,   796,   680,   800,    96,
     829,   832,   835,   861,   243,  -499,  -500,  -501,   841,   520,
     842,    25,   521,   246,   247,   248,   535,   536,    85,    86,
      87,    88,    89,    90,    91,    92,    93,   843,   330,   569,
     845,   846,   847,   869,   251,   848,   849,   850,   522,   338,
     878,   523,   524,   544,   879,   537,   525,   526,   527,   538,
     528,   604,   889,   529,   894,   331,   332,   333,   334,   335,
     336,   337,   906,   909,   915,   530,   531,   532,   533,   899,
     914,   917,   534,   919,   923,   925,   926,   668,   929,  1000,
     946,   338,   539,   540,   520,   936,    25,   521,   541,   941,
    1001,   968,   969,   544,   974,   979,   980,   981,   544,   544,
     544,   544,   544,   544,   535,   536,   982,   984,   544,  1048,
    1002,  1029,  1039,   522,  1061,  1049,   523,   524,  1055,  1056,
    1063,   525,   526,   527,  1065,   528,  1067,  1069,   529,  1092,
    1075,  1093,  1106,   537,  1071,  1109,  1112,   538,  1079,  1120,
     530,   531,   532,   533,   818,  1132,  1134,   534,  1135,  1137,
    1138,  1144,   669,  1182,  1148,  1054,  1183,  1163,  1149,  1151,
    1152,   384,    47,  1019,  1164,  1165,  1166,  1167,  1168,  1169,
     539,   540,  1171,  1170,   647,  1172,   541,  1173,    15,   535,
     536,   520,   962,    25,   521,   544,   857,   544,  1038,   611,
     834,   380,   844,   416,   870,   505,   874,   597,   544,   769,
     747,   229,   231,   904,   875,   557,   952,   178,   537,   559,
     522,     0,   538,   523,   524,     0,     0,     0,   525,   526,
     527,     0,   528,     0,     0,   529,     0,  1080,   785,     0,
       0,     0,     0,     0,     0,  1107,     0,   530,   531,   532,
     533,     0,   682,     0,   534,   539,   540,     0,     0,     0,
       0,   541,     0,     0,     0,     0,   544,     0,   544,   544,
       0,     0,     0,   520,   544,    25,   521,     0,     0,     0,
     544,   544,   544,     0,     0,     0,   535,   536,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   522,     0,     0,   523,   524,     0,     0,     0,
     525,   526,   527,     0,   528,   537,     0,   529,     0,   538,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   530,
     531,   532,   533,   793,   793,     0,   534,     0,     0,     0,
       0,   797,     0,     0,     0,     0,   520,     0,    25,   521,
       0,     0,   539,   540,     0,     0,     0,     0,   541,   544,
       0,     0,     0,     0,     0,     0,   544,   544,   535,   536,
       0,     0,     0,     0,     0,   522,     0,     0,   523,   524,
       0,     0,     0,   525,   526,   527,     0,   528,     0,     0,
     529,     0,   520,  1121,    25,   521,     0,   537,     0,     0,
       0,   538,   530,   531,   532,   533,     0,     0,     0,   534,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   522,     0,     0,   523,   524,     0,     0,     0,   525,
     526,   527,     0,   528,   539,   540,   529,     0,     0,     0,
     541,   535,   536,     0,     0,     0,     0,     0,   530,   531,
     532,   533,     0,     0,     0,   534,     0,     0,     0,     0,
    1127,     0,     0,     0,     0,   520,     0,    25,   521,     0,
     537,     0,     0,     0,   538,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   535,   536,     0,
       0,     0,     0,     0,   522,     0,     0,   523,   524,     0,
       0,     0,   525,   526,   527,     0,   528,   539,   540,   529,
       0,     0,     0,   541,     0,     0,   537,     0,     0,     0,
     538,   530,   531,   532,   533,     0,     0,     0,   534,  1082,
       0,    25,    26,    27,     0,    28,    29,    30,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   539,   540,    31,    32,    33,     0,   541,
     535,   536,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1008,  1082,     0,    25,    26,    27,  1087,    28,    29,
      30,     0,     0,     0,     0,     0,     0,     0,     0,   537,
       0,     0,     0,   538,     0,     0,     0,     0,    31,    32,
      33,     0,   807,     0,    25,    26,    27,     0,    28,    29,
      30,     0,     0,     0,  1008,     0,     0,     0,     0,     0,
    1089,     0,     0,     0,     0,     0,   539,   540,    31,    32,
      33,     0,   541,     0,  1007,     0,    25,    26,    27,     0,
      28,    29,    30,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      31,    32,    33,  1012,     0,    25,    26,    27,     0,    28,
      29,    30,  -130,    34,    35,    36,  1008,   808,    37,    38,
       0,     0,     0,     0,     0,  -130,     0,   809,    39,    31,
      32,    33,    40,    41,     0,     0,    42,     0,     0,     0,
       0,     0,     0,     0,     0,  1008,   807,     0,    25,    26,
      27,     0,    28,    29,    30,  -130,    34,    35,    36,     0,
       0,    37,    38,     0,     0,     0,     0,     0,  -130,     0,
       0,    39,    31,    32,    33,    40,    41,     0,     0,    42,
       0,     0,     0,     0,     0,     0,    34,    35,    36,     0,
       0,    37,    38,   810,     0,     0,     0,     0,     0,     0,
       0,    39,     0,     0,     0,    40,    41,     0,  1014,    42,
      25,    26,    27,     0,    28,    29,    30,  -130,    34,    35,
      36,   808,     0,    37,    38,     0,     0,     0,     0,     0,
    -130,   809,     0,    39,    31,    32,    33,    40,    41,     0,
       0,    42,     0,     0,     0,     0,  -130,    34,    35,    36,
    1008,     0,    37,    38,     0,     0,     0,     0,     0,  -130,
       0,     0,    39,     0,     0,     0,    40,    41,     0,     0,
      42,   965,     0,    25,    26,    27,     0,    28,    29,    30,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      34,    35,    36,     0,     0,    37,    38,    31,    32,    33,
     208,     0,    25,    26,    27,    39,    28,    29,    30,    40,
      41,     0,     0,    42,    25,    26,    27,     0,    28,    29,
      30,     0,     0,     0,     0,     0,    31,    32,    33,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    31,    32,
      33,     0,     0,     0,    25,    26,    27,     0,    28,    29,
      30,  -130,    34,    35,    36,     0,     0,    37,    38,     0,
       0,     0,     0,     0,  -130,     0,     0,    39,    31,    32,
      33,    40,    41,     0,   768,    42,    25,    26,    27,     0,
      28,    29,    30,     0,     0,   798,     0,    25,    26,    27,
       0,    28,    29,    30,     0,     0,     0,     0,     0,     0,
      31,    32,    33,     0,     0,     0,     0,     0,     0,     0,
       0,    31,    32,    33,     0,    34,    35,    36,     0,     0,
      37,    38,     0,     0,     0,     0,     0,     0,     0,     0,
      39,   966,     0,     0,    40,    41,     0,     0,    42,     0,
       0,     0,     0,     0,    34,    35,    36,     0,     0,    37,
      38,     0,     0,     0,     0,     0,    34,    35,    36,    39,
       0,    37,    38,    40,    41,     0,     0,    42,     0,     0,
       0,    39,     0,     0,     0,    40,    41,     0,     0,    42,
       0,   569,     0,     0,     0,     0,    34,    35,    36,     0,
       0,    37,    38,     0,     0,     0,     0,     0,     0,     0,
       0,    39,     0,     0,     0,    40,    41,     0,     0,    42,
       0,   604,     0,     0,     0,     0,     0,     0,    34,    35,
      36,     0,     0,    37,    38,     0,     0,     0,     0,    34,
      35,    36,     0,    39,    37,    38,     0,    40,    41,     0,
       0,    42,     0,     0,    39,     0,     0,     0,    40,    41,
       0,   803,    42,    25,    26,    27,     0,    28,    29,    30,
       0,     0,  1125,     0,    25,    26,    27,     0,    28,    29,
      30,     0,     0,     0,     0,     0,     0,    31,    32,    33,
      25,    26,    27,     0,    28,    29,    30,     0,    31,    32,
      33,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    31,    32,    33,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    25,    26,
      27,     0,    28,    29,    30,     0,     0,     0,     0,   430,
     431,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    31,    32,    33,     0,     0,    25,    26,    27,
       0,    28,    29,    30,     0,   432,   433,     0,    25,    26,
      27,     0,    28,    29,    30,     0,     0,     0,     0,     0,
       0,    31,    32,    33,     0,   434,   435,   436,   437,     0,
       0,     0,    31,    32,    33,    34,    35,    36,     0,     0,
      37,    38,     0,     0,   438,   439,    34,    35,    36,     0,
      39,    37,    38,   432,    40,    41,     0,     0,    42,     0,
       0,    39,    34,    35,    36,    40,    41,    37,    38,    42,
       0,     0,     0,   434,     0,   436,     0,    39,     0,     0,
       0,    40,    41,   433,     0,    42,     0,     0,     0,     0,
       0,     0,   438,    25,    26,    27,     0,    28,    29,    30,
       0,     0,   817,   435,     0,   437,     0,     0,     0,     0,
      34,    35,    36,     0,     0,    37,    38,    31,    32,    33,
       0,     0,   439,     0,     0,    39,     0,     0,     0,    40,
      41,     0,     0,    42,     0,     0,     0,     0,     0,    34,
      35,    36,     0,     0,    37,    38,     0,     0,     0,     0,
      34,    35,    36,     0,    39,    37,    38,     0,    40,    41,
       0,     0,    42,     0,     0,    39,     0,   539,   818,    40,
      41,     0,     0,    42,    25,    26,    27,     0,    28,    29,
      30,     0,     0,     0,     0,     0,     0,   822,     0,     0,
      25,    26,    27,     0,    28,    29,    30,     0,    31,    32,
      33,     0,    25,    26,    27,     0,    28,    29,    30,     0,
       0,     0,     0,     0,    31,    32,    33,     0,    25,    26,
      27,     0,    28,    29,    30,     0,    31,    32,    33,     0,
       0,     0,     0,     0,     0,    34,    35,    36,     0,     0,
      37,    38,    31,    32,    33,     0,     0,   808,     0,     0,
      39,     0,   539,   818,    40,    41,     0,   809,    42,    69,
      70,     0,    71,    72,    73,    74,    75,    76,    77,     0,
       0,     0,     0,     0,    78,    79,    80,    81,    82,     0,
       0,     0,     0,     0,     0,    69,    70,     0,    71,    72,
      73,    74,    75,    76,    77,     0,     0,     0,     0,     0,
      78,    79,    80,    81,    82,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    34,    35,    36,     0,
       0,    37,    38,     0,     0,     0,     0,     0,     0,     0,
       0,    39,    34,    35,    36,    40,    41,    37,    38,    42,
       0,     0,     0,     0,    34,    35,    36,    39,   953,    37,
      38,    40,    41,     0,     0,    42,     0,     0,     0,    39,
      34,    35,    36,    40,    41,    37,    38,    42,     0,     0,
       0,     0,     0,     0,     0,   745,     0,     0,     0,    40,
      41,     0,     0,    42,     0,     0,     0,     0,     0,     0,
       0,     0,    83,     0,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   105,    83,   147,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      69,    70,     0,    71,    72,    73,    74,    75,    76,    77,
       0,     0,   148,     0,     0,    78,    79,    80,    81,    82,
       0,     0,     0,     0,     0,    69,    70,     0,    71,    72,
      73,    74,    75,    76,    77,     0,     0,     0,     0,     0,
      78,    79,    80,    81,    82,     0,     0,     0,     0,     0,
      69,    70,     0,    71,    72,    73,    74,    75,    76,    77,
       0,     0,     0,     0,     0,    78,    79,    80,    81,    82,
       0,     0,     0,    69,    70,     0,    71,    72,    73,    74,
      75,    76,    77,     0,     0,     0,     0,     0,    78,    79,
      80,    81,    82,     0,     0,     0,    69,    70,     0,    71,
      72,    73,    74,    75,    76,    77,     0,     0,     0,     0,
       0,    78,    79,    80,    81,    82,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    83,   267,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   268,    83,   934,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   935,    83,   589,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    83,   957,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    83,
     197,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    69,    70,     0,    71,    72,    73,    74,    75,    76,
      77,     0,     0,   563,     0,     0,    78,    79,    80,    81,
      82,     0,     0,     0,    69,    70,     0,    71,    72,    73,
      74,    75,    76,    77,     0,     0,     0,     0,     0,    78,
      79,    80,    81,    82,     0,     0,     0,    69,    70,     0,
      71,    72,    73,    74,    75,    76,    77,     0,     0,     0,
       0,     0,    78,    79,    80,    81,    82,   975,     0,     0,
       0,     0,     0,     0,    69,    70,     0,    71,    72,    73,
      74,    75,    76,    77,     0,     0,     0,     0,     0,    78,
      79,    80,    81,    82,    69,    70,     0,    71,    72,    73,
      74,    75,    76,    77,     0,     0,     0,     0,     0,    78,
      79,    80,    81,    82,    69,    70,     0,    71,    72,    73,
      74,    75,    76,    77,     0,     0,     0,     0,     0,    78,
      79,    80,    81,    82,    83,     0,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    83,   858,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      83,   964,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1094,     0,     0,    83,     0,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,     0,
       0,   976,     0,     0,     0,     0,     0,    83,     0,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,     0,
       0,   971,     0,     0,     0,   104,     0,    83,   853,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,     0,
       0,     0,     0,    69,    70,   983,    71,    72,    73,    74,
      75,    76,    77,     0,     0,     0,     0,     0,    78,    79,
      80,    81,    82,    69,    70,     0,    71,    72,    73,    74,
      75,    76,    77,     0,     0,     0,     0,     0,    78,    79,
      80,    81,    82,    69,    70,     0,    71,    72,    73,    74,
      75,    76,    77,     0,     0,     0,     0,     0,    78,    79,
      80,    81,    82,    69,    70,     0,    71,    72,    73,    74,
      75,    76,    77,     0,     0,     0,     0,     0,    78,    79,
      80,    81,    82,    69,    70,     0,    71,    72,    73,    74,
      75,    76,    77,     0,     0,     0,     0,     0,    78,    79,
      80,    81,    82,    69,    70,     0,    71,    72,    73,    74,
      75,    76,    77,     0,     0,     0,     0,     0,    78,    79,
      80,    81,    82,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    83,     0,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,     0,     0,
       0,     0,     0,     0,   104,     0,    83,     0,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,     0,     0,
       0,     0,     0,     0,   743,     0,    83,     0,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,     0,     0,
       0,     0,     0,     0,   911,     0,    83,     0,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,     0,     0,
       0,     0,     0,     0,   912,     0,    83,     0,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,     0,     0,
       0,     0,     0,     0,   913,     0,    83,     0,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,     0,     0,
       0,     0,    69,    70,   920,    71,    72,    73,    74,    75,
      76,    77,     0,     0,     0,     0,     0,    78,    79,    80,
      81,    82,    69,    70,     0,    71,    72,    73,    74,    75,
      76,    77,     0,     0,     0,     0,     0,    78,    79,    80,
      81,    82,    69,    70,     0,    71,    72,    73,    74,    75,
      76,    77,     0,     0,     0,     0,     0,    78,    79,    80,
      81,    82,    69,    70,     0,    71,    72,    73,    74,    75,
      76,    77,     0,     0,     0,     0,     0,    78,    79,    80,
      81,    82,    69,    70,     0,    71,    72,    73,    74,    75,
      76,    77,     0,     0,     0,     0,     0,    78,    79,    80,
      81,    82,    69,    70,     0,    71,    72,    73,    74,    75,
      76,    77,     0,     0,     0,     0,     0,    78,    79,    80,
      81,    82,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    83,     0,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,     0,     0,     0,
       0,     0,     0,   921,     0,    83,     0,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,     0,     0,     0,
       0,     0,     0,   922,     0,    83,     0,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,     0,     0,     0,
       0,     0,     0,   924,     0,    83,     0,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,     0,     0,     0,
       0,     0,     0,  1040,     0,    83,     0,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,     0,     0,     0,
       0,     0,     0,  1050,     0,    83,     0,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,     0,     0,     0,
       0,    69,    70,  1095,    71,    72,    73,    74,    75,    76,
      77,     0,     0,     0,     0,     0,    78,    79,    80,    81,
      82,    69,    70,     0,    71,    72,    73,    74,    75,    76,
      77,     0,     0,     0,     0,     0,    78,    79,    80,    81,
      82,    69,    70,     0,    71,    72,    73,    74,    75,    76,
      77,     0,     0,     0,     0,     0,    78,    79,    80,    81,
      82,    69,    70,     0,    71,    72,    73,    74,    75,    76,
      77,     0,     0,     0,     0,     0,    78,    79,    80,    81,
      82,    69,    70,     0,    71,    72,    73,    74,    75,    76,
      77,     0,     0,     0,     0,     0,    78,    79,    80,    81,
      82,    69,    70,     0,    71,    72,    73,    74,    75,    76,
      77,     0,     0,     0,     0,     0,    78,    79,    80,    81,
      82,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    83,     0,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,     0,     0,     0,     0,
       0,     0,  1096,     0,    83,     0,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,     0,     0,     0,     0,
       0,     0,  1104,     0,    83,     0,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,     0,     0,     0,     0,
       0,     0,  1154,     0,    83,     0,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,     0,     0,     0,     0,
       0,     0,  1155,     0,    83,     0,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,     0,     0,     0,     0,
       0,     0,  1156,     0,    83,     0,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,     0,     0,     0,     0,
      69,    70,  1157,    71,    72,    73,    74,    75,    76,    77,
       0,     0,     0,     0,     0,    78,    79,    80,    81,    82,
      69,    70,     0,    71,    72,    73,    74,    75,    76,    77,
       0,     0,     0,     0,     0,    78,    79,    80,    81,    82,
      69,    70,     0,    71,    72,    73,    74,    75,    76,    77,
       0,     0,     0,     0,     0,    78,    79,    80,    81,    82,
      69,    70,     0,    71,    72,    73,    74,    75,    76,    77,
       0,     0,     0,     0,     0,    78,    79,    80,    81,    82,
      69,    70,     0,    71,    72,    73,    74,    75,    76,    77,
       0,     0,     0,     0,     0,    78,    79,    80,    81,    82,
      69,    70,     0,    71,    72,    73,    74,    75,    76,    77,
       0,     0,     0,     0,     0,    78,    79,    80,    81,    82,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    83,     0,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,     0,     0,     0,     0,     0,
       0,  1177,     0,    83,     0,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,     0,     0,     0,     0,     0,
       0,  1178,     0,    83,     0,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,     0,     0,     0,     0,   713,
       0,     0,     0,    83,     0,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,     0,     0,     0,     0,   940,
       0,     0,     0,    83,     0,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,     0,     0,     0,     0,   944,
       0,     0,     0,    83,     0,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,     0,     0,    69,    70,  1003,
      71,    72,    73,    74,    75,    76,    77,     0,     0,     0,
       0,     0,    78,    79,    80,    81,    82,    69,    70,     0,
      71,    72,    73,    74,    75,    76,    77,     0,     0,     0,
       0,     0,    78,    79,    80,    81,    82,    69,    70,     0,
      71,    72,    73,    74,    75,    76,    77,     0,     0,     0,
       0,     0,    78,    79,    80,    81,    82,    69,    70,     0,
      71,    72,    73,    74,    75,    76,    77,     0,     0,     0,
       0,     0,    78,    79,    80,    81,    82,    69,    70,     0,
      71,    72,    73,    74,    75,    76,    77,     0,     0,     0,
       0,     0,    78,    79,    80,    81,    82,    69,    70,     0,
      71,    72,    73,    74,    75,    76,    77,     0,     0,     0,
       0,     0,    78,    79,    80,    81,    82,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      83,     0,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,     0,     0,     0,     0,  1018,     0,     0,     0,
      83,     0,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,     0,     0,     0,     0,  1032,     0,     0,     0,
      83,     0,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,     0,     0,     0,     0,  1033,     0,     0,     0,
      83,     0,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,     0,     0,     0,     0,  1035,     0,     0,     0,
      83,     0,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,     0,     0,     0,     0,  1036,     0,     0,     0,
      83,     0,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,     0,     0,    69,    70,  1090,    71,    72,    73,
      74,    75,    76,    77,     0,     0,     0,     0,     0,    78,
      79,    80,    81,    82,    69,    70,     0,    71,    72,    73,
      74,    75,    76,    77,     0,     0,     0,     0,     0,    78,
      79,    80,    81,    82,    69,    70,     0,    71,    72,    73,
      74,    75,    76,    77,     0,     0,     0,     0,     0,    78,
      79,    80,    81,    82,    69,    70,     0,    71,    72,    73,
      74,    75,    76,    77,     0,     0,     0,     0,     0,    78,
      79,    80,    81,    82,    69,    70,     0,    71,    72,    73,
      74,    75,    76,    77,     0,     0,     0,     0,     0,    78,
      79,    80,    81,    82,    69,    70,     0,    71,    72,    73,
      74,    75,    76,    77,     0,     0,     0,     0,     0,    78,
      79,    80,    81,    82,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    83,     0,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,     0,
       0,     0,     0,  1145,     0,     0,     0,    83,     0,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,     0,
       0,     0,     0,  1161,     0,     0,     0,    83,     0,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,     0,
       0,     0,     0,  1162,     0,     0,     0,    83,   145,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,     0,
       0,     0,     0,     0,     0,     0,     0,    83,   589,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,     0,
       0,     0,     0,     0,     0,     0,     0,    83,   732,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    69,
      70,     0,    71,    72,    73,    74,    75,    76,    77,     0,
       0,     0,     0,     0,    78,    79,    80,    81,    82,    69,
      70,     0,    71,    72,    73,    74,    75,    76,    77,     0,
       0,     0,     0,     0,    78,    79,    80,    81,    82,    69,
      70,     0,    71,    72,    73,    74,    75,    76,    77,     0,
       0,     0,     0,     0,    78,    79,    80,    81,    82,    69,
      70,     0,    71,    72,    73,    74,    75,    76,    77,     0,
       0,     0,     0,     0,    78,    79,    80,    81,    82,    69,
      70,     0,    71,    72,    73,    74,    75,    76,    77,     0,
       0,     0,     0,     0,    78,    79,    80,    81,    82,    69,
      70,     0,    71,    72,    73,    74,    75,    76,    77,     0,
       0,     0,     0,     0,     0,    79,    80,    81,    82,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    83,   853,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,     0,     0,     0,     0,     0,     0,
       0,     0,    83,   876,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,     0,     0,     0,     0,     0,     0,
       0,     0,    83,  1043,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,     0,     0,     0,     0,     0,     0,
       0,     0,    83,  1051,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,     0,     0,     0,     0,     0,     0,
       0,     0,    83,     0,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,   232,     0,   233,     0,     0,     0,
       0,     0,     0,     0,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    69,    70,     1,    71,    72,    73,
      74,    75,    76,    77,   -10,   -10,   234,     0,   -10,   -10,
     -10,    80,    81,    82,   -10,     0,     0,   235,     0,     0,
       0,     0,     0,     0,  -264,     0,     0,     0,     0,   236,
       0,     0,     0,     0,   237,     0,     0,     0,   -10,   238,
     239,   240,     0,     0,   241,     0,     0,     0,   -10,     0,
     -10,   -10,   -10,   -10,   -10,   -10,   242,   243,   -10,     0,
       0,     0,     0,   244,   245,   -10,   246,   247,   248,     0,
       0,   -10,   -10,   -10,   -10,   -10,     0,     0,     0,   249,
       0,     0,     0,   -10,   -10,     0,   250,   251,   -10,   -10,
     -10,   -10,   -10,   -10,   -10,   -10,   252,     0,     0,   -10,
       0,     0,     0,   -10,   -10,   -10,   -10,     0,     0,     0,
       0,     0,     0,     0,   253,     0,     0,   232,     0,   233,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,     1,
       0,     0,     0,     0,     0,     0,     0,   -10,   -10,   234,
       0,   -10,   -10,   -10,     0,     0,     0,   -10,     0,     0,
     235,     0,     0,     0,     0,     0,     0,  -263,     0,     0,
       0,     0,   236,     0,     0,     0,     0,   237,     0,     0,
       0,   -10,   238,   239,   240,     0,     0,   241,     0,     0,
       0,   -10,     0,   -10,   -10,   -10,   -10,   -10,   -10,   242,
     243,   -10,     0,     0,     0,     0,   244,   245,   -10,   246,
     247,   248,     0,     0,   -10,   -10,   -10,   -10,   -10,     0,
       0,     0,   249,     0,     0,     0,   -10,   -10,     0,   250,
     251,   -10,   -10,   -10,   -10,   -10,   -10,   -10,   -10,   252,
       0,     0,   -10,     0,     0,     0,   -10,   -10,   -10,   -10,
     352,   353,     0,     0,   354,   355,   356,   253,     0,     0,
     357,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   358,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   359,     0,   360,   361,   362,   363,
     364,   365,     0,     0,   366,     0,     0,     0,     0,     0,
       0,   367,     0,     0,     0,     0,     0,   368,   369,   370,
     371,   372,     0,     0,     0,     0,     0,     0,     0,   213,
     214,     0,     0,     0,   373,   374,   375,   215,   216,   217,
     218,   219,     0,     0,     0,   220,   385,   561,     0,   221,
     222,   376,   377,   386,     0,   387,     0,   388,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   389,   390,   391,   392,   393,   394,
     395,   396,   397,   398,   399,   400,   401,   402,   403,   404,
     405,   406,   407,   385,   700,     0,     0,     0,     0,     0,
     386,     0,   387,     0,   388,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   389,   390,   391,   392,   393,   394,   395,   396,   397,
     398,   399,   400,   401,   402,   403,   404,   405,   406,   407,
     385,   701,     0,     0,     0,     0,     0,   386,     0,   387,
       0,   388,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   389,   390,
     391,   392,   393,   394,   395,   396,   397,   398,   399,   400,
     401,   402,   403,   404,   405,   406,   407,   385,     0,     0,
       0,     0,     0,     0,   386,   702,   387,     0,   388,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   703,   704,   705,   392,   393,
     394,   395,   396,   397,   398,   399,   400,   401,   402,   403,
     404,   405,   406,   407,   385,     0,     0,     0,     0,     0,
       0,   386,     0,   387,     0,   388,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   389,   390,   391,   392,   393,   394,   395,   396,
     397,   398,   399,   400,   401,   402,   403,   404,   405,   406,
     407
};

static const short yycheck[] =
{
      19,   642,   543,   582,   273,   274,   275,   276,   533,   689,
     690,   316,   413,   665,   378,   501,   502,   251,   319,   378,
      39,     3,   382,    42,    31,    32,    33,    34,    35,    36,
      37,    38,     3,    40,    41,   236,     1,    42,     3,   493,
     494,   495,   496,   497,   498,   246,   247,    52,   155,   307,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,   182,    95,   352,   235,     3,
     237,   245,     3,   358,   241,    10,   243,   106,   729,    94,
     470,     1,   326,     3,     1,     3,     3,   676,   144,   206,
     105,     1,     3,     3,     4,     5,   158,     7,     8,     9,
     160,   157,   288,    49,   166,   279,     1,     3,     3,     4,
       5,     3,     7,     8,     9,    48,   145,     1,   147,     3,
       3,     1,     3,    24,   775,    49,     3,   678,     8,     3,
     158,     3,     4,     5,   254,     7,     8,     9,   104,   259,
      23,   157,     1,   310,     3,     3,     3,   558,   315,   560,
     561,     8,     9,   169,     3,   184,     3,     3,     1,   326,
       3,     4,     8,     9,     3,     3,     3,     3,     1,   758,
       3,     8,     9,     3,   453,   454,   205,     3,   552,     3,
       3,   550,   552,     3,   104,   166,   465,    30,     8,     9,
      33,    34,     3,   168,   462,    38,    39,    40,   144,    42,
     166,    24,    45,   160,   378,     1,   157,     3,    21,    22,
      84,   601,     0,   864,    57,    58,    59,    60,   169,   159,
     144,    64,    49,   158,     3,   776,   159,     3,     4,     5,
     170,     7,     8,     9,   168,    23,   170,   168,   267,   524,
     484,    12,   160,   160,   162,   162,   166,   532,   533,    20,
     160,   168,     3,    96,    97,   284,   157,   168,   168,   910,
     446,   576,   851,   578,   575,   160,   577,   918,   764,   765,
     166,   104,    68,   168,   157,   554,   168,     3,   161,   160,
     160,   162,   125,   160,   168,   168,   129,   170,   160,   753,
     701,   755,   756,   757,   152,   759,   168,    93,    94,    87,
      76,   316,   160,   160,   319,   516,   160,   166,   897,     1,
      86,   160,   166,   160,   160,    82,   159,   144,   114,   162,
     163,   160,   160,   160,   160,   168,     0,    17,    18,    19,
     160,   144,    70,   166,   160,    73,   157,  1053,     3,   158,
     160,  1057,   553,  1059,  1060,   507,   157,   509,   169,    23,
     157,   170,   469,   141,  1005,   159,    10,    11,    23,    89,
     158,    53,   169,    17,    18,    19,   170,   105,   919,   920,
     166,   922,    64,   157,   659,   549,   550,   539,   929,   109,
     160,   111,   593,   594,   160,   670,    65,    88,    67,   600,
     157,   980,   168,     7,   161,   984,   140,   160,   128,   428,
      12,    67,     5,   166,    83,  1095,  1096,   108,    20,   110,
     425,   426,   210,    87,   212,   107,    95,    83,   585,   448,
    1136,   162,    12,  1139,    66,    67,   127,   456,   457,    95,
      20,   225,   461,   227,   228,   609,   157,     1,   612,     3,
     161,    83,   134,   135,   136,   137,   138,   139,   140,   193,
    1010,   195,  1041,  1013,   248,  1015,   471,  1008,   487,   159,
     150,   151,   152,   153,   154,   259,     3,   141,   160,   238,
     239,  1006,   270,   242,   157,   492,   493,   494,   495,   496,
     497,   498,  1144,  1145,  1019,   696,   169,     3,   699,    62,
      63,   770,   771,   772,   148,   149,   150,   151,   152,   153,
     154,    63,   157,   297,   298,    68,   161,    72,    71,    74,
     150,   151,    76,   257,   308,    88,    89,   157,   692,    62,
    1109,    84,    86,  1112,     7,     8,   541,    89,  1079,  1080,
      93,    94,    95,   548,  1085,   108,   109,   110,   111,   157,
     157,  1092,  1093,   161,  1066,    88,  1068,   109,  1070,   111,
     157,   114,   169,   582,   127,   128,   160,   157,   157,   157,
     589,   161,   161,   161,   235,   108,   128,   110,     1,   584,
       3,     4,     5,   160,     7,     8,     9,   151,   523,   864,
     859,   860,   861,   528,   127,   379,   531,   616,   617,   383,
     535,   620,   621,   608,    27,    28,    29,   171,   172,   173,
       5,   159,   160,   620,   621,   152,   153,   154,   166,   638,
      43,   640,   170,   157,   631,   159,    49,   288,    10,    11,
     159,    13,    14,    15,    16,    17,    18,    19,     3,   914,
     157,   660,   661,   662,   161,    27,   921,   652,   923,   924,
      95,    84,   671,   160,   673,   674,   675,   676,   157,   166,
     679,     3,   161,   170,   157,   110,   111,     3,   161,   688,
     689,   690,   161,   118,   119,   120,   121,   122,     3,   166,
     157,   126,   687,   170,   161,   130,   131,     1,   166,     3,
     959,   352,   150,   151,   152,   153,   154,   358,   158,   159,
     901,   902,   903,   157,   723,   157,   725,   161,   157,   161,
     157,   157,   161,   732,   161,   161,   157,     1,     3,     3,
     161,   144,   145,   146,   147,   157,   745,   150,   151,   161,
     169,  1006,   166,   752,   157,   754,   170,   160,     1,   758,
       3,   164,   165,   157,  1019,   168,   753,   161,   755,   756,
     757,   157,   759,     3,   157,   161,     6,   958,   161,   960,
     961,     3,     1,   782,     3,   147,   148,   149,   150,   151,
     152,   153,   154,   157,   159,   159,   795,   912,   913,    66,
      67,    68,   159,   444,    71,   446,   157,     3,   159,   808,
     809,   159,   160,    66,    67,    68,    83,    84,    71,   112,
     819,   820,  1077,   158,   823,   824,    93,    94,    95,   160,
      83,    84,   157,   157,   159,   159,  1091,   157,    52,   159,
      93,    94,    95,   157,   843,   159,   157,   114,   159,   157,
       3,   159,   851,   157,   853,   159,     1,   157,     3,   159,
     157,   114,   159,   158,  1045,   157,   157,   159,   159,     1,
     869,     3,   157,   157,   159,   159,   157,   876,   159,   878,
     879,   522,   523,   524,   157,   167,   159,   528,   529,   104,
     531,   532,   533,   159,   535,   157,    95,   159,   897,   540,
       3,   542,   543,   104,   689,   690,   157,   894,   159,   110,
     111,     1,   162,     3,     4,  1170,   915,   118,   119,   120,
     121,   122,  1177,  1178,   157,   126,   159,   930,   931,   130,
     131,   930,   931,   160,   157,   934,   159,   936,   937,   157,
      30,   159,   941,    33,    34,   189,   190,   946,    38,    39,
      40,   157,    42,   159,    51,    45,    10,    11,    48,    13,
      14,    15,    16,    17,    18,    19,   160,    57,    58,    59,
      60,   157,   971,   159,    64,   974,   157,   157,   159,   159,
     979,   980,   981,   982,   157,   984,   159,   157,   157,   159,
     159,   157,   157,   159,   159,   157,   157,   159,   159,   986,
     157,   988,   159,   990,   162,   992,    96,    97,   157,   161,
     159,   110,   111,   157,     3,   159,   632,   633,   659,   118,
     119,   120,   121,   122,   665,   410,   411,   126,   166,   670,
      50,   130,   131,     3,   159,   125,   159,   678,   158,   129,
       3,   159,  1041,   159,  1043,   157,   159,    53,   159,   159,
     158,   160,  1051,   160,   144,   160,   160,    54,   160,   160,
     160,   159,    10,    11,   160,    13,    14,    15,    16,    17,
      18,    19,   162,   163,   159,   158,     3,   159,   168,    27,
      28,    29,   159,   159,     3,   726,     3,     3,   157,   157,
     157,  1090,   157,   159,   148,   149,   150,   151,   152,   153,
     154,     1,     3,     3,     4,   160,   166,   158,   161,   161,
    1109,   158,   161,  1112,     3,   161,   161,  1116,  1117,  1118,
    1119,   161,   161,   157,   159,   161,     3,   160,   160,   160,
      30,  1130,  1131,    33,    34,   776,   160,   160,    38,    39,
      40,   160,    42,   160,   160,    45,   158,     3,   142,   157,
     157,     3,   157,   157,    53,   157,   157,    57,    58,    59,
      60,   157,     5,   158,    64,    64,    66,    67,    68,   159,
    1169,    71,  1171,     3,   159,   158,     3,   818,   159,   170,
       3,     3,   159,    83,    84,   159,   159,   159,   144,     1,
     159,     3,     4,    93,    94,    95,    96,    97,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   160,   107,   170,
     161,   161,   161,   160,   114,   161,   161,   161,    30,   160,
     142,    33,    34,   864,   142,   125,    38,    39,    40,   129,
      42,   170,   167,    45,   166,   134,   135,   136,   137,   138,
     139,   140,   157,    48,   158,    57,    58,    59,    60,   161,
     161,    69,    64,   161,   161,   161,   161,    69,   161,    55,
     158,   160,   162,   163,     1,   160,     3,     4,   168,   160,
       5,   159,   159,   914,   157,   157,   157,   157,   919,   920,
     921,   922,   923,   924,    96,    97,   157,   157,   929,    50,
     159,   159,   159,    30,   159,   161,    33,    34,   161,   157,
     161,    38,    39,    40,   161,    42,   161,   161,    45,    47,
     161,    47,   159,   125,   167,   157,   157,   129,   144,   159,
      57,    58,    59,    60,   163,   161,   161,    64,   161,   161,
     161,   159,   144,     0,   159,   976,     0,   161,   159,   159,
     159,   261,    21,   918,   161,   161,   161,   161,   161,   158,
     162,   163,   158,   161,   517,   159,   168,   159,     4,    96,
      97,     1,   864,     3,     4,  1006,   726,  1008,   947,   475,
     696,   257,   711,   270,   737,   340,   743,   458,  1019,   640,
     618,   191,   193,   775,   743,   410,   841,   153,   125,   411,
      30,    -1,   129,    33,    34,    -1,    -1,    -1,    38,    39,
      40,    -1,    42,    -1,    -1,    45,    -1,   144,    48,    -1,
      -1,    -1,    -1,    -1,    -1,  1056,    -1,    57,    58,    59,
      60,    -1,   159,    -1,    64,   162,   163,    -1,    -1,    -1,
      -1,   168,    -1,    -1,    -1,    -1,  1077,    -1,  1079,  1080,
      -1,    -1,    -1,     1,  1085,     3,     4,    -1,    -1,    -1,
    1091,  1092,  1093,    -1,    -1,    -1,    96,    97,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    33,    34,    -1,    -1,    -1,
      38,    39,    40,    -1,    42,   125,    -1,    45,    -1,   129,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    57,
      58,    59,    60,  1144,  1145,    -1,    64,    -1,    -1,    -1,
      -1,    69,    -1,    -1,    -1,    -1,     1,    -1,     3,     4,
      -1,    -1,   162,   163,    -1,    -1,    -1,    -1,   168,  1170,
      -1,    -1,    -1,    -1,    -1,    -1,  1177,  1178,    96,    97,
      -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    33,    34,
      -1,    -1,    -1,    38,    39,    40,    -1,    42,    -1,    -1,
      45,    -1,     1,    48,     3,     4,    -1,   125,    -1,    -1,
      -1,   129,    57,    58,    59,    60,    -1,    -1,    -1,    64,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    33,    34,    -1,    -1,    -1,    38,
      39,    40,    -1,    42,   162,   163,    45,    -1,    -1,    -1,
     168,    96,    97,    -1,    -1,    -1,    -1,    -1,    57,    58,
      59,    60,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,
      69,    -1,    -1,    -1,    -1,     1,    -1,     3,     4,    -1,
     125,    -1,    -1,    -1,   129,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    -1,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    33,    34,    -1,
      -1,    -1,    38,    39,    40,    -1,    42,   162,   163,    45,
      -1,    -1,    -1,   168,    -1,    -1,   125,    -1,    -1,    -1,
     129,    57,    58,    59,    60,    -1,    -1,    -1,    64,     1,
      -1,     3,     4,     5,    -1,     7,     8,     9,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   162,   163,    27,    28,    29,    -1,   168,
      96,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    43,     1,    -1,     3,     4,     5,    49,     7,     8,
       9,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,
      -1,    -1,    -1,   129,    -1,    -1,    -1,    -1,    27,    28,
      29,    -1,     1,    -1,     3,     4,     5,    -1,     7,     8,
       9,    -1,    -1,    -1,    43,    -1,    -1,    -1,    -1,    -1,
      49,    -1,    -1,    -1,    -1,    -1,   162,   163,    27,    28,
      29,    -1,   168,    -1,     1,    -1,     3,     4,     5,    -1,
       7,     8,     9,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      27,    28,    29,     1,    -1,     3,     4,     5,    -1,     7,
       8,     9,   144,   145,   146,   147,    43,    76,   150,   151,
      -1,    -1,    -1,    -1,    -1,   157,    -1,    86,   160,    27,
      28,    29,   164,   165,    -1,    -1,   168,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    43,     1,    -1,     3,     4,
       5,    -1,     7,     8,     9,   144,   145,   146,   147,    -1,
      -1,   150,   151,    -1,    -1,    -1,    -1,    -1,   157,    -1,
      -1,   160,    27,    28,    29,   164,   165,    -1,    -1,   168,
      -1,    -1,    -1,    -1,    -1,    -1,   145,   146,   147,    -1,
      -1,   150,   151,   152,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   160,    -1,    -1,    -1,   164,   165,    -1,     1,   168,
       3,     4,     5,    -1,     7,     8,     9,   144,   145,   146,
     147,    76,    -1,   150,   151,    -1,    -1,    -1,    -1,    -1,
     157,    86,    -1,   160,    27,    28,    29,   164,   165,    -1,
      -1,   168,    -1,    -1,    -1,    -1,   144,   145,   146,   147,
      43,    -1,   150,   151,    -1,    -1,    -1,    -1,    -1,   157,
      -1,    -1,   160,    -1,    -1,    -1,   164,   165,    -1,    -1,
     168,     1,    -1,     3,     4,     5,    -1,     7,     8,     9,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     145,   146,   147,    -1,    -1,   150,   151,    27,    28,    29,
       1,    -1,     3,     4,     5,   160,     7,     8,     9,   164,
     165,    -1,    -1,   168,     3,     4,     5,    -1,     7,     8,
       9,    -1,    -1,    -1,    -1,    -1,    27,    28,    29,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    28,
      29,    -1,    -1,    -1,     3,     4,     5,    -1,     7,     8,
       9,   144,   145,   146,   147,    -1,    -1,   150,   151,    -1,
      -1,    -1,    -1,    -1,   157,    -1,    -1,   160,    27,    28,
      29,   164,   165,    -1,     1,   168,     3,     4,     5,    -1,
       7,     8,     9,    -1,    -1,     1,    -1,     3,     4,     5,
      -1,     7,     8,     9,    -1,    -1,    -1,    -1,    -1,    -1,
      27,    28,    29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    28,    29,    -1,   145,   146,   147,    -1,    -1,
     150,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     160,   161,    -1,    -1,   164,   165,    -1,    -1,   168,    -1,
      -1,    -1,    -1,    -1,   145,   146,   147,    -1,    -1,   150,
     151,    -1,    -1,    -1,    -1,    -1,   145,   146,   147,   160,
      -1,   150,   151,   164,   165,    -1,    -1,   168,    -1,    -1,
      -1,   160,    -1,    -1,    -1,   164,   165,    -1,    -1,   168,
      -1,   170,    -1,    -1,    -1,    -1,   145,   146,   147,    -1,
      -1,   150,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   160,    -1,    -1,    -1,   164,   165,    -1,    -1,   168,
      -1,   170,    -1,    -1,    -1,    -1,    -1,    -1,   145,   146,
     147,    -1,    -1,   150,   151,    -1,    -1,    -1,    -1,   145,
     146,   147,    -1,   160,   150,   151,    -1,   164,   165,    -1,
      -1,   168,    -1,    -1,   160,    -1,    -1,    -1,   164,   165,
      -1,     1,   168,     3,     4,     5,    -1,     7,     8,     9,
      -1,    -1,     1,    -1,     3,     4,     5,    -1,     7,     8,
       9,    -1,    -1,    -1,    -1,    -1,    -1,    27,    28,    29,
       3,     4,     5,    -1,     7,     8,     9,    -1,    27,    28,
      29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    27,    28,    29,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,    -1,     7,     8,     9,    -1,    -1,    -1,    -1,    62,
      63,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    27,    28,    29,    -1,    -1,     3,     4,     5,
      -1,     7,     8,     9,    -1,    88,    89,    -1,     3,     4,
       5,    -1,     7,     8,     9,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    28,    29,    -1,   108,   109,   110,   111,    -1,
      -1,    -1,    27,    28,    29,   145,   146,   147,    -1,    -1,
     150,   151,    -1,    -1,   127,   128,   145,   146,   147,    -1,
     160,   150,   151,    88,   164,   165,    -1,    -1,   168,    -1,
      -1,   160,   145,   146,   147,   164,   165,   150,   151,   168,
      -1,    -1,    -1,   108,    -1,   110,    -1,   160,    -1,    -1,
      -1,   164,   165,    89,    -1,   168,    -1,    -1,    -1,    -1,
      -1,    -1,   127,     3,     4,     5,    -1,     7,     8,     9,
      -1,    -1,    97,   109,    -1,   111,    -1,    -1,    -1,    -1,
     145,   146,   147,    -1,    -1,   150,   151,    27,    28,    29,
      -1,    -1,   128,    -1,    -1,   160,    -1,    -1,    -1,   164,
     165,    -1,    -1,   168,    -1,    -1,    -1,    -1,    -1,   145,
     146,   147,    -1,    -1,   150,   151,    -1,    -1,    -1,    -1,
     145,   146,   147,    -1,   160,   150,   151,    -1,   164,   165,
      -1,    -1,   168,    -1,    -1,   160,    -1,   162,   163,   164,
     165,    -1,    -1,   168,     3,     4,     5,    -1,     7,     8,
       9,    -1,    -1,    -1,    -1,    -1,    -1,    97,    -1,    -1,
       3,     4,     5,    -1,     7,     8,     9,    -1,    27,    28,
      29,    -1,     3,     4,     5,    -1,     7,     8,     9,    -1,
      -1,    -1,    -1,    -1,    27,    28,    29,    -1,     3,     4,
       5,    -1,     7,     8,     9,    -1,    27,    28,    29,    -1,
      -1,    -1,    -1,    -1,    -1,   145,   146,   147,    -1,    -1,
     150,   151,    27,    28,    29,    -1,    -1,    76,    -1,    -1,
     160,    -1,   162,   163,   164,   165,    -1,    86,   168,    10,
      11,    -1,    13,    14,    15,    16,    17,    18,    19,    -1,
      -1,    -1,    -1,    -1,    25,    26,    27,    28,    29,    -1,
      -1,    -1,    -1,    -1,    -1,    10,    11,    -1,    13,    14,
      15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,
      25,    26,    27,    28,    29,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   145,   146,   147,    -1,
      -1,   150,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   160,   145,   146,   147,   164,   165,   150,   151,   168,
      -1,    -1,    -1,    -1,   145,   146,   147,   160,   161,   150,
     151,   164,   165,    -1,    -1,   168,    -1,    -1,    -1,   160,
     145,   146,   147,   164,   165,   150,   151,   168,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,    -1,   164,
     165,    -1,    -1,   168,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   143,    -1,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   168,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
      10,    11,    -1,    13,    14,    15,    16,    17,    18,    19,
      -1,    -1,   167,    -1,    -1,    25,    26,    27,    28,    29,
      -1,    -1,    -1,    -1,    -1,    10,    11,    -1,    13,    14,
      15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,
      25,    26,    27,    28,    29,    -1,    -1,    -1,    -1,    -1,
      10,    11,    -1,    13,    14,    15,    16,    17,    18,    19,
      -1,    -1,    -1,    -1,    -1,    25,    26,    27,    28,    29,
      -1,    -1,    -1,    10,    11,    -1,    13,    14,    15,    16,
      17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,    26,
      27,    28,    29,    -1,    -1,    -1,    10,    11,    -1,    13,
      14,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
      -1,    25,    26,    27,    28,    29,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   167,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   167,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   143,   167,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,
     167,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,    10,    11,    -1,    13,    14,    15,    16,    17,    18,
      19,    -1,    -1,   167,    -1,    -1,    25,    26,    27,    28,
      29,    -1,    -1,    -1,    10,    11,    -1,    13,    14,    15,
      16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,
      26,    27,    28,    29,    -1,    -1,    -1,    10,    11,    -1,
      13,    14,    15,    16,    17,    18,    19,    -1,    -1,    -1,
      -1,    -1,    25,    26,    27,    28,    29,     3,    -1,    -1,
      -1,    -1,    -1,    -1,    10,    11,    -1,    13,    14,    15,
      16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,
      26,    27,    28,    29,    10,    11,    -1,    13,    14,    15,
      16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,
      26,    27,    28,    29,    10,    11,    -1,    13,    14,    15,
      16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,
      26,    27,    28,    29,   143,    -1,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,   167,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     143,   167,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   167,    -1,    -1,   143,    -1,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,    -1,
      -1,   157,    -1,    -1,    -1,    -1,    -1,   143,    -1,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,    -1,
      -1,   157,    -1,    -1,    -1,   161,    -1,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,    -1,
      -1,    -1,    -1,    10,    11,   161,    13,    14,    15,    16,
      17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,    26,
      27,    28,    29,    10,    11,    -1,    13,    14,    15,    16,
      17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,    26,
      27,    28,    29,    10,    11,    -1,    13,    14,    15,    16,
      17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,    26,
      27,    28,    29,    10,    11,    -1,    13,    14,    15,    16,
      17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,    26,
      27,    28,    29,    10,    11,    -1,    13,    14,    15,    16,
      17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,    26,
      27,    28,    29,    10,    11,    -1,    13,    14,    15,    16,
      17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,    26,
      27,    28,    29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,    -1,    -1,
      -1,    -1,    -1,    -1,   161,    -1,   143,    -1,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,    -1,    -1,
      -1,    -1,    -1,    -1,   161,    -1,   143,    -1,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,    -1,    -1,
      -1,    -1,    -1,    -1,   161,    -1,   143,    -1,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,    -1,    -1,
      -1,    -1,    -1,    -1,   161,    -1,   143,    -1,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,    -1,    -1,
      -1,    -1,    -1,    -1,   161,    -1,   143,    -1,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,    -1,    -1,
      -1,    -1,    10,    11,   161,    13,    14,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    25,    26,    27,
      28,    29,    10,    11,    -1,    13,    14,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    25,    26,    27,
      28,    29,    10,    11,    -1,    13,    14,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    25,    26,    27,
      28,    29,    10,    11,    -1,    13,    14,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    25,    26,    27,
      28,    29,    10,    11,    -1,    13,    14,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    25,    26,    27,
      28,    29,    10,    11,    -1,    13,    14,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    25,    26,    27,
      28,    29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   143,    -1,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,    -1,    -1,    -1,
      -1,    -1,    -1,   161,    -1,   143,    -1,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,    -1,    -1,    -1,
      -1,    -1,    -1,   161,    -1,   143,    -1,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,    -1,    -1,    -1,
      -1,    -1,    -1,   161,    -1,   143,    -1,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,    -1,    -1,    -1,
      -1,    -1,    -1,   161,    -1,   143,    -1,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,    -1,    -1,    -1,
      -1,    -1,    -1,   161,    -1,   143,    -1,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,    -1,    -1,    -1,
      -1,    10,    11,   161,    13,    14,    15,    16,    17,    18,
      19,    -1,    -1,    -1,    -1,    -1,    25,    26,    27,    28,
      29,    10,    11,    -1,    13,    14,    15,    16,    17,    18,
      19,    -1,    -1,    -1,    -1,    -1,    25,    26,    27,    28,
      29,    10,    11,    -1,    13,    14,    15,    16,    17,    18,
      19,    -1,    -1,    -1,    -1,    -1,    25,    26,    27,    28,
      29,    10,    11,    -1,    13,    14,    15,    16,    17,    18,
      19,    -1,    -1,    -1,    -1,    -1,    25,    26,    27,    28,
      29,    10,    11,    -1,    13,    14,    15,    16,    17,    18,
      19,    -1,    -1,    -1,    -1,    -1,    25,    26,    27,    28,
      29,    10,    11,    -1,    13,    14,    15,    16,    17,    18,
      19,    -1,    -1,    -1,    -1,    -1,    25,    26,    27,    28,
      29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   143,    -1,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,    -1,    -1,    -1,    -1,
      -1,    -1,   161,    -1,   143,    -1,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,    -1,    -1,    -1,    -1,
      -1,    -1,   161,    -1,   143,    -1,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,    -1,    -1,    -1,    -1,
      -1,    -1,   161,    -1,   143,    -1,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,    -1,    -1,    -1,    -1,
      -1,    -1,   161,    -1,   143,    -1,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,    -1,    -1,    -1,    -1,
      -1,    -1,   161,    -1,   143,    -1,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,    -1,    -1,    -1,    -1,
      10,    11,   161,    13,    14,    15,    16,    17,    18,    19,
      -1,    -1,    -1,    -1,    -1,    25,    26,    27,    28,    29,
      10,    11,    -1,    13,    14,    15,    16,    17,    18,    19,
      -1,    -1,    -1,    -1,    -1,    25,    26,    27,    28,    29,
      10,    11,    -1,    13,    14,    15,    16,    17,    18,    19,
      -1,    -1,    -1,    -1,    -1,    25,    26,    27,    28,    29,
      10,    11,    -1,    13,    14,    15,    16,    17,    18,    19,
      -1,    -1,    -1,    -1,    -1,    25,    26,    27,    28,    29,
      10,    11,    -1,    13,    14,    15,    16,    17,    18,    19,
      -1,    -1,    -1,    -1,    -1,    25,    26,    27,    28,    29,
      10,    11,    -1,    13,    14,    15,    16,    17,    18,    19,
      -1,    -1,    -1,    -1,    -1,    25,    26,    27,    28,    29,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   143,    -1,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,    -1,    -1,    -1,    -1,    -1,
      -1,   161,    -1,   143,    -1,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,    -1,    -1,    -1,    -1,    -1,
      -1,   161,    -1,   143,    -1,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,    -1,    -1,    -1,    -1,   159,
      -1,    -1,    -1,   143,    -1,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,    -1,    -1,    -1,    -1,   159,
      -1,    -1,    -1,   143,    -1,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,    -1,    -1,    -1,    -1,   159,
      -1,    -1,    -1,   143,    -1,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,    -1,    -1,    10,    11,   159,
      13,    14,    15,    16,    17,    18,    19,    -1,    -1,    -1,
      -1,    -1,    25,    26,    27,    28,    29,    10,    11,    -1,
      13,    14,    15,    16,    17,    18,    19,    -1,    -1,    -1,
      -1,    -1,    25,    26,    27,    28,    29,    10,    11,    -1,
      13,    14,    15,    16,    17,    18,    19,    -1,    -1,    -1,
      -1,    -1,    25,    26,    27,    28,    29,    10,    11,    -1,
      13,    14,    15,    16,    17,    18,    19,    -1,    -1,    -1,
      -1,    -1,    25,    26,    27,    28,    29,    10,    11,    -1,
      13,    14,    15,    16,    17,    18,    19,    -1,    -1,    -1,
      -1,    -1,    25,    26,    27,    28,    29,    10,    11,    -1,
      13,    14,    15,    16,    17,    18,    19,    -1,    -1,    -1,
      -1,    -1,    25,    26,    27,    28,    29,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     143,    -1,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,    -1,    -1,    -1,    -1,   159,    -1,    -1,    -1,
     143,    -1,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,    -1,    -1,    -1,    -1,   159,    -1,    -1,    -1,
     143,    -1,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,    -1,    -1,    -1,    -1,   159,    -1,    -1,    -1,
     143,    -1,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,    -1,    -1,    -1,    -1,   159,    -1,    -1,    -1,
     143,    -1,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,    -1,    -1,    -1,    -1,   159,    -1,    -1,    -1,
     143,    -1,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,    -1,    -1,    10,    11,   159,    13,    14,    15,
      16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,
      26,    27,    28,    29,    10,    11,    -1,    13,    14,    15,
      16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,
      26,    27,    28,    29,    10,    11,    -1,    13,    14,    15,
      16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,
      26,    27,    28,    29,    10,    11,    -1,    13,    14,    15,
      16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,
      26,    27,    28,    29,    10,    11,    -1,    13,    14,    15,
      16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,
      26,    27,    28,    29,    10,    11,    -1,    13,    14,    15,
      16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,
      26,    27,    28,    29,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,    -1,
      -1,    -1,    -1,   159,    -1,    -1,    -1,   143,    -1,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,    -1,
      -1,    -1,    -1,   159,    -1,    -1,    -1,   143,    -1,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,    -1,
      -1,    -1,    -1,   159,    -1,    -1,    -1,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,    10,
      11,    -1,    13,    14,    15,    16,    17,    18,    19,    -1,
      -1,    -1,    -1,    -1,    25,    26,    27,    28,    29,    10,
      11,    -1,    13,    14,    15,    16,    17,    18,    19,    -1,
      -1,    -1,    -1,    -1,    25,    26,    27,    28,    29,    10,
      11,    -1,    13,    14,    15,    16,    17,    18,    19,    -1,
      -1,    -1,    -1,    -1,    25,    26,    27,    28,    29,    10,
      11,    -1,    13,    14,    15,    16,    17,    18,    19,    -1,
      -1,    -1,    -1,    -1,    25,    26,    27,    28,    29,    10,
      11,    -1,    13,    14,    15,    16,    17,    18,    19,    -1,
      -1,    -1,    -1,    -1,    25,    26,    27,    28,    29,    10,
      11,    -1,    13,    14,    15,    16,    17,    18,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    26,    27,    28,    29,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   143,    -1,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,     1,    -1,     3,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,    10,    11,    23,    13,    14,    15,
      16,    17,    18,    19,    31,    32,    33,    -1,    35,    36,
      37,    27,    28,    29,    41,    -1,    -1,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,    56,
      -1,    -1,    -1,    -1,    61,    -1,    -1,    -1,    65,    66,
      67,    68,    -1,    -1,    71,    -1,    -1,    -1,    75,    -1,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    -1,
      -1,    -1,    -1,    90,    91,    92,    93,    94,    95,    -1,
      -1,    98,    99,   100,   101,   102,    -1,    -1,    -1,   106,
      -1,    -1,    -1,   110,   111,    -1,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,    -1,    -1,   126,
      -1,    -1,    -1,   130,   131,   132,   133,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,    -1,    -1,     1,    -1,     3,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,    23,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    33,
      -1,    35,    36,    37,    -1,    -1,    -1,    41,    -1,    -1,
      44,    -1,    -1,    -1,    -1,    -1,    -1,    51,    -1,    -1,
      -1,    -1,    56,    -1,    -1,    -1,    -1,    61,    -1,    -1,
      -1,    65,    66,    67,    68,    -1,    -1,    71,    -1,    -1,
      -1,    75,    -1,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    -1,    -1,    -1,    -1,    90,    91,    92,    93,
      94,    95,    -1,    -1,    98,    99,   100,   101,   102,    -1,
      -1,    -1,   106,    -1,    -1,    -1,   110,   111,    -1,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
      -1,    -1,   126,    -1,    -1,    -1,   130,   131,   132,   133,
      31,    32,    -1,    -1,    35,    36,    37,   141,    -1,    -1,
      41,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    65,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    75,    -1,    77,    78,    79,    80,
      81,    82,    -1,    -1,    85,    -1,    -1,    -1,    -1,    -1,
      -1,    92,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,
     101,   102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,
     111,    -1,    -1,    -1,   115,   116,   117,   118,   119,   120,
     121,   122,    -1,    -1,    -1,   126,   143,   144,    -1,   130,
     131,   132,   133,   150,    -1,   152,    -1,   154,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   143,   144,    -1,    -1,    -1,    -1,    -1,
     150,    -1,   152,    -1,   154,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     143,   144,    -1,    -1,    -1,    -1,    -1,   150,    -1,   152,
      -1,   154,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   143,    -1,    -1,
      -1,    -1,    -1,    -1,   150,   151,   152,    -1,   154,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,   187,   188,   189,   143,    -1,    -1,    -1,    -1,    -1,
      -1,   150,    -1,   152,    -1,   154,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */
#line 3 "d:\\\300\356\352\325\\My Projects\\vp/share/bison/bison.simple"

/* Skeleton output parser for bison,

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002 Free Software
   Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* This is the parser code that is written into each bison parser when
   the %semantic_parser declaration is not specified in the grammar.
   It was written by Richard Stallman by simplifying the hairy parser
   used when %semantic_parser is specified.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

#if ! defined (yyoverflow) || defined (YYERROR_VERBOSE)

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# if YYSTACK_USE_ALLOCA
#  define YYSTACK_ALLOC alloca
# else
#  ifndef YYSTACK_USE_ALLOCA
#   if defined (alloca) || defined (_ALLOCA_H)
#    define YYSTACK_ALLOC alloca
#   else
#    ifdef __GNUC__
#     define YYSTACK_ALLOC __builtin_alloca
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
# else
#  if defined (__STDC__) || defined (__cplusplus)
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   define YYSIZE_T size_t
#  endif
#  define YYSTACK_ALLOC malloc
#  define YYSTACK_FREE free
# endif
#endif /* ! defined (yyoverflow) || defined (YYERROR_VERBOSE) */


#if (! defined (yyoverflow) \
     && (! defined (__cplusplus) \
	 || (YYLTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short yyss;
  YYSTYPE yyvs;
# if YYLSP_NEEDED
  YYLTYPE yyls;
# endif
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAX (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# if YYLSP_NEEDED
#  define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (YYSTYPE) + sizeof (YYLTYPE))	\
      + 2 * YYSTACK_GAP_MAX)
# else
#  define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (YYSTYPE))				\
      + YYSTACK_GAP_MAX)
# endif

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  register YYSIZE_T yyi;		\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (0)
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAX;	\
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (0)

#endif


#if ! defined (YYSIZE_T) && defined (__SIZE_TYPE__)
# define YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (YYSIZE_T) && defined (size_t)
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T)
# if defined (__STDC__) || defined (__cplusplus)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# endif
#endif
#if ! defined (YYSIZE_T)
# define YYSIZE_T unsigned int
#endif

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	goto yyacceptlab
#define YYABORT 	goto yyabortlab
#define YYERROR		goto yyerrlab1
/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL		goto yyerrlab
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { 								\
      yyerror ("syntax error: cannot back up");			\
      YYERROR;							\
    }								\
while (0)

#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Compute the default location (before the actions
   are run).

   When YYLLOC_DEFAULT is run, CURRENT is set the location of the
   first token.  By default, to implement support for ranges, extend
   its range to the last symbol.  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)       	\
   Current.last_line   = Rhs[N].last_line;	\
   Current.last_column = Rhs[N].last_column;
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#if YYPURE
# if YYLSP_NEEDED
#  ifdef YYLEX_PARAM
#   define YYLEX		yylex (&yylval, &yylloc, YYLEX_PARAM)
#  else
#   define YYLEX		yylex (&yylval, &yylloc)
#  endif
# else /* !YYLSP_NEEDED */
#  ifdef YYLEX_PARAM
#   define YYLEX		yylex (&yylval, YYLEX_PARAM)
#  else
#   define YYLEX		yylex (&yylval)
#  endif
# endif /* !YYLSP_NEEDED */
#else /* !YYPURE */
# define YYLEX			yylex ()
#endif /* !YYPURE */


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (0)
/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
#endif /* !YYDEBUG */

/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   SIZE_MAX < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#if YYMAXDEPTH == 0
# undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif

#ifdef YYERROR_VERBOSE

# ifndef yystrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
yystrlen (const char *yystr)
#   else
yystrlen (yystr)
     const char *yystr;
#   endif
{
  register const char *yys = yystr;

  while (*yys++ != '\0')
    continue;

  return yys - yystr - 1;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
yystpcpy (char *yydest, const char *yysrc)
#   else
yystpcpy (yydest, yysrc)
     char *yydest;
     const char *yysrc;
#   endif
{
  register char *yyd = yydest;
  register const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif
#endif

#line 315 "d:\\\300\356\352\325\\My Projects\\vp/share/bison/bison.simple"


/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
#  define YYPARSE_PARAM_ARG void *YYPARSE_PARAM
#  define YYPARSE_PARAM_DECL
# else
#  define YYPARSE_PARAM_ARG YYPARSE_PARAM
#  define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
# endif
#else /* !YYPARSE_PARAM */
# define YYPARSE_PARAM_ARG
# define YYPARSE_PARAM_DECL
#endif /* !YYPARSE_PARAM */

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
# ifdef YYPARSE_PARAM
int yyparse (void *);
# else
int yyparse (void);
# endif
#endif

/* YY_DECL_VARIABLES -- depending whether we use a pure parser,
   variables are global, or local to YYPARSE.  */

#define YY_DECL_NON_LSP_VARIABLES			\
/* The lookahead symbol.  */				\
int yychar;						\
							\
/* The semantic value of the lookahead symbol. */	\
YYSTYPE yylval;						\
							\
/* Number of parse errors so far.  */			\
int yynerrs;

#if YYLSP_NEEDED
# define YY_DECL_VARIABLES			\
YY_DECL_NON_LSP_VARIABLES			\
						\
/* Location data for the lookahead symbol.  */	\
YYLTYPE yylloc;
#else
# define YY_DECL_VARIABLES			\
YY_DECL_NON_LSP_VARIABLES
#endif


/* If nonreentrant, generate the variables here. */

#if !YYPURE
YY_DECL_VARIABLES
#endif  /* !YYPURE */

int
yyparse (YYPARSE_PARAM_ARG)
     YYPARSE_PARAM_DECL
{
  /* If reentrant, generate the variables here. */
#if YYPURE
  YY_DECL_VARIABLES
#endif  /* !YYPURE */

  register int yystate;
  register int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Lookahead token as an internal (translated) token number.  */
  int yychar1 = 0;

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack. */
  short	yyssa[YYINITDEPTH];
  short *yyss = yyssa;
  register short *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  register YYSTYPE *yyvsp;

#if YYLSP_NEEDED
  /* The location stack.  */
  YYLTYPE yylsa[YYINITDEPTH];
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;
#endif

#if YYLSP_NEEDED
# define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
# define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  YYSIZE_T yystacksize = YYINITDEPTH;


  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
#if YYLSP_NEEDED
  YYLTYPE yyloc;
#endif

  /* When reducing, the number of symbols on the RHS of the reduced
     rule. */
  int yylen;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;
#if YYLSP_NEEDED
  yylsp = yyls;
#endif
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed. so pushing a state here evens the stacks.
     */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	short *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  */
# if YYLSP_NEEDED
	YYLTYPE *yyls1 = yyls;
	/* This used to be a conditional around just the two extra args,
	   but that might be undefined if yyoverflow is a macro.  */
	yyoverflow ("parser stack overflow",
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yyls1, yysize * sizeof (*yylsp),
		    &yystacksize);
	yyls = yyls1;
# else
	yyoverflow ("parser stack overflow",
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);
# endif
	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyoverflowlab;
# else
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	goto yyoverflowlab;
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;

      {
	short *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyoverflowlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);
# if YYLSP_NEEDED
	YYSTACK_RELOCATE (yyls);
# endif
# undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
#if YYLSP_NEEDED
      yylsp = yyls + yysize - 1;
#endif

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (yychar <= 0)		/* This means end of input. */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more */

      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yychar1 = YYTRANSLATE (yychar);

#if YYDEBUG
     /* We have to keep this `#if YYDEBUG', since we use variables
	which are defined only if `YYDEBUG' is set.  */
      if (yydebug)
	{
	  YYFPRINTF (stderr, "Next token is %d (%s",
		     yychar, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise
	     meaning of a token, for further debugging info.  */
# ifdef YYPRINT
	  YYPRINT (stderr, yychar, yylval);
# endif
	  YYFPRINTF (stderr, ")\n");
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    goto yydefault;

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrlab;

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */
  YYDPRINTF ((stderr, "Shifting token %d (%s), ",
	      yychar, yytname[yychar1]));

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
#if YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  yystate = yyn;
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
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to the semantic value of
     the lookahead token.  This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

#if YYLSP_NEEDED
  /* Similarly for the default location.  Let the user run additional
     commands if for instance locations are ranges.  */
  yyloc = yylsp[1-yylen];
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
#endif

#if YYDEBUG
  /* We have to keep this `#if YYDEBUG', since we use variables which
     are defined only if `YYDEBUG' is set.  */
  if (yydebug)
    {
      int yyi;

      YYFPRINTF (stderr, "Reducing via rule %d (line %d), ",
		 yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (yyi = yyprhs[yyn]; yyrhs[yyi] > 0; yyi++)
	YYFPRINTF (stderr, "%s ", yytname[yyrhs[yyi]]);
      YYFPRINTF (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif

  switch (yyn) {

case 5:
#line 231 "parse.y"
{ yyval.number = yyvsp[0].number; ;
    break;}
case 6:
#line 233 "parse.y"
{ yyval.number = yyvsp[0].number; ;
    break;}
case 7:
#line 235 "parse.y"
{ yyval.number = pform_verinum_with_size(yyvsp[-1].number,yyvsp[0].number, yylsp[0].text, yylsp[0].first_line); ;
    break;}
case 8:
#line 242 "parse.y"
{ yyval.named_pexprs = yyvsp[-1].named_pexprs; ;
    break;}
case 9:
#line 243 "parse.y"
{ yyval.named_pexprs = 0; ;
    break;}
case 10:
#line 244 "parse.y"
{ yyval.named_pexprs = 0; ;
    break;}
case 11:
#line 249 "parse.y"
{ svector<named_pexpr_t*>*tmp =
			new svector<named_pexpr_t*>(*yyvsp[-2].named_pexprs,yyvsp[0].named_pexpr);
		  delete yyvsp[-2].named_pexprs;
		  yyval.named_pexprs = tmp;
		;
    break;}
case 12:
#line 255 "parse.y"
{ svector<named_pexpr_t*>*tmp = new svector<named_pexpr_t*>(1);
		  (*tmp)[0] = yyvsp[0].named_pexpr;
		  yyval.named_pexprs = tmp;
		;
    break;}
case 13:
#line 264 "parse.y"
{ named_pexpr_t*tmp = new named_pexpr_t;
		  tmp->name = string(yyvsp[0].text);
		  tmp->parm = 0;
		  delete yyvsp[0].text;
		  yyval.named_pexpr = tmp;
		;
    break;}
case 14:
#line 271 "parse.y"
{ PExpr*tmp = yyvsp[0].expr;
		if (!pform_expression_is_constant(tmp)) {
			yyerror(yylsp[0], "error: attribute value "
			            "expression must be constant.");
			delete tmp;
			tmp = 0;
		  }
		  named_pexpr_t*tmp2 = new named_pexpr_t;
		  tmp2->name = string(yyvsp[-2].text);
		  tmp2->parm = tmp;
		  delete yyvsp[-2].text;
		  yyval.named_pexpr = tmp2;
		;
    break;}
case 15:
#line 294 "parse.y"
{ pform_set_net_range(yyvsp[-1].texts, yyvsp[-2].exprs, yyvsp[-3].flag);
		;
    break;}
case 16:
#line 297 "parse.y"
{ pform_set_net_range(yyvsp[-1].texts, 0, yyvsp[-2].flag);
		;
    break;}
case 17:
#line 300 "parse.y"
{ pform_set_reg_integer(yyvsp[-1].texts);
		;
    break;}
case 18:
#line 303 "parse.y"
{ pform_set_reg_time(yyvsp[-1].texts);
		;
    break;}
case 19:
#line 306 "parse.y"
{ pform_make_reals(yyvsp[-1].texts, yylsp[-2].text, yylsp[-2].first_line);
		;
    break;}
case 20:
#line 309 "parse.y"
{ pform_make_reals(yyvsp[-1].texts, yylsp[-2].text, yylsp[-2].first_line);
		;
    break;}
case 23:
#line 318 "parse.y"
{ yyerror(yylsp[-2], "error: syntax error in reg variable list.");
		  yyerrok;
		;
    break;}
case 24:
#line 322 "parse.y"
{ yyerror(yylsp[-2], "error: syntax error in integer variable list.");
		  yyerrok;
		;
    break;}
case 25:
#line 326 "parse.y"
{ yyerror(yylsp[-2], "error: syntax error in time variable list.");
		  yyerrok;
		;
    break;}
case 26:
#line 330 "parse.y"
{ yyerror(yylsp[-2], "error: syntax error in real variable list.");
		  yyerrok;
		;
    break;}
case 27:
#line 334 "parse.y"
{ yyerror(yylsp[-2], "error: syntax error in realtime variable list.");
		  yyerrok;
		;
    break;}
case 28:
#line 338 "parse.y"
{ yyerror(yylsp[-2], "error: syntax error in parameter list.");
		  yyerrok;
		;
    break;}
case 29:
#line 342 "parse.y"
{ yyerror(yylsp[-2], "error: syntax error localparam list.");
		  yyerrok;
		;
    break;}
case 34:
#line 359 "parse.y"
{ PCase::Item*tmp = new PCase::Item;
		  tmp->expr = *yyvsp[-2].exprs;
		  tmp->stat = yyvsp[0].statement;
		  delete yyvsp[-2].exprs;
		  yyval.citem = tmp;
		;
    break;}
case 35:
#line 366 "parse.y"
{ PCase::Item*tmp = new PCase::Item;
		  tmp->stat = yyvsp[0].statement;
		  yyval.citem = tmp;
		;
    break;}
case 36:
#line 371 "parse.y"
{ PCase::Item*tmp = new PCase::Item;
		  tmp->stat = yyvsp[0].statement;
		  yyval.citem = tmp;
		;
    break;}
case 37:
#line 376 "parse.y"
{ yyerror(yylsp[-2], "error: Incomprehensible case expression.");
		  yyerrok;
		;
    break;}
case 38:
#line 383 "parse.y"
{ svector<PCase::Item*>*tmp;
		  tmp = new svector<PCase::Item*>(*yyvsp[-1].citems, yyvsp[0].citem);
		  delete yyvsp[-1].citems;
		  yyval.citems = tmp;
		;
    break;}
case 39:
#line 389 "parse.y"
{ svector<PCase::Item*>*tmp = new svector<PCase::Item*>(1);
		  (*tmp)[0] = yyvsp[0].citem;
		  yyval.citems = tmp;
		;
    break;}
case 45:
#line 408 "parse.y"
{ PExpr*tmp = yyvsp[0].expr;
		  if (!pform_expression_is_constant(tmp)) {
			yyerror(yylsp[0], "error: parameter value "
			            "must be constant.");
			delete tmp;
			tmp = 0;
		  }
		  pform_set_defparam(*yyvsp[-2].hier, yyvsp[0].expr);
		  delete yyvsp[-2].hier;
		;
    break;}
case 47:
#line 423 "parse.y"
{ yyerror(yylsp[-1], "error: defparam may not include a range.");
		  delete yyvsp[-1].exprs;
		;
    break;}
case 49:
#line 431 "parse.y"
{ svector<PExpr*>*tmp = new svector<PExpr*>(1);
		  (*tmp)[0] = yyvsp[0].expr;
		  yyval.exprs = tmp;
		;
    break;}
case 50:
#line 436 "parse.y"
{ svector<PExpr*>*tmp = new svector<PExpr*>(1);
		  (*tmp)[0] = yyvsp[-1].expr;
		  yyval.exprs = tmp;
		;
    break;}
case 51:
#line 444 "parse.y"
{ svector<PExpr*>*tmp = new svector<PExpr*>(1);
		  (*tmp)[0] = yyvsp[0].expr;
		  yyval.exprs = tmp;
		;
    break;}
case 52:
#line 449 "parse.y"
{ svector<PExpr*>*tmp = new svector<PExpr*>(1);
		  (*tmp)[0] = yyvsp[-1].expr;
		  yyval.exprs = tmp;
		;
    break;}
case 53:
#line 454 "parse.y"
{ svector<PExpr*>*tmp = new svector<PExpr*>(2);
		  (*tmp)[0] = yyvsp[-3].expr;
		  (*tmp)[1] = yyvsp[-1].expr;
		  yyval.exprs = tmp;
		;
    break;}
case 54:
#line 460 "parse.y"
{ svector<PExpr*>*tmp = new svector<PExpr*>(3);
		  (*tmp)[0] = yyvsp[-5].expr;
		  (*tmp)[1] = yyvsp[-3].expr;
		  (*tmp)[2] = yyvsp[-1].expr;
		  yyval.exprs = tmp;
		;
    break;}
case 55:
#line 469 "parse.y"
{ yyval.exprs = yyvsp[0].exprs; ;
    break;}
case 56:
#line 470 "parse.y"
{ yyval.exprs = 0; ;
    break;}
case 57:
#line 475 "parse.y"
{ PExpr*tmp = yyvsp[0].expr;
		  yyval.expr = tmp;
		;
    break;}
case 58:
#line 479 "parse.y"
{ yyval.expr = pform_select_mtm_expr(yyvsp[-4].expr, yyvsp[-2].expr, yyvsp[0].expr); ;
    break;}
case 59:
#line 485 "parse.y"
{ verinum*tmp = yyvsp[0].number;
		  if (tmp == 0) {
			yyerror(yylsp[0], "internal error: delay.");
			yyval.expr = 0;
		  } else {
			yyval.expr = new PENumber(tmp);
			yyval.expr->set_file(yylsp[0].text);
			yyval.expr->set_lineno(yylsp[0].first_line);
		  }
		;
    break;}
case 60:
#line 496 "parse.y"
{ verireal*tmp = yyvsp[0].realtime;
		  if (tmp == 0) {
			yyerror(yylsp[0], "internal error: delay.");
			yyval.expr = 0;
		  } else {
			yyval.expr = new PEFNumber(tmp);
			yyval.expr->set_file(yylsp[0].text);
			yyval.expr->set_lineno(yylsp[0].first_line);
		  }
		;
    break;}
case 61:
#line 507 "parse.y"
{ PEIdent*tmp = new PEIdent(hname_t(yyvsp[0].text));
		  tmp->set_file(yylsp[0].text);
		  tmp->set_lineno(yylsp[0].first_line);
		  yyval.expr = tmp;
		  delete yyvsp[0].text;
		;
    break;}
case 64:
#line 519 "parse.y"
{ pform_set_type_attrib(yyvsp[-5].text, yyvsp[-3].text, yyvsp[-1].text);
		  delete yyvsp[-5].text;
		  delete yyvsp[-3].text;
		;
    break;}
case 65:
#line 527 "parse.y"
{ yyval.drive.str0 = yyvsp[-3].drive.str0;
		  yyval.drive.str1 = yyvsp[-1].drive.str1;
		;
    break;}
case 66:
#line 531 "parse.y"
{ yyval.drive.str0 = yyvsp[-1].drive.str0;
		  yyval.drive.str1 = yyvsp[-3].drive.str1;
		;
    break;}
case 67:
#line 535 "parse.y"
{ yyval.drive.str0 = yyvsp[-3].drive.str0;
		  yyval.drive.str1 = PGate::HIGHZ;
		;
    break;}
case 68:
#line 539 "parse.y"
{ yyval.drive.str0 = PGate::HIGHZ;
		  yyval.drive.str1 = yyvsp[-3].drive.str1;
		;
    break;}
case 69:
#line 543 "parse.y"
{ yyval.drive.str0 = yyvsp[-1].drive.str0;
		  yyval.drive.str1 = PGate::HIGHZ;
		;
    break;}
case 70:
#line 547 "parse.y"
{ yyval.drive.str0 = PGate::HIGHZ;
		  yyval.drive.str1 = yyvsp[-1].drive.str1;
		;
    break;}
case 71:
#line 553 "parse.y"
{ yyval.drive = yyvsp[0].drive; ;
    break;}
case 72:
#line 554 "parse.y"
{ yyval.drive.str0 = PGate::STRONG; yyval.drive.str1 = PGate::STRONG; ;
    break;}
case 73:
#line 558 "parse.y"
{ yyval.drive.str0 = PGate::SUPPLY; ;
    break;}
case 74:
#line 559 "parse.y"
{ yyval.drive.str0 = PGate::STRONG; ;
    break;}
case 75:
#line 560 "parse.y"
{ yyval.drive.str0 = PGate::PULL; ;
    break;}
case 76:
#line 561 "parse.y"
{ yyval.drive.str0 = PGate::WEAK; ;
    break;}
case 77:
#line 565 "parse.y"
{ yyval.drive.str1 = PGate::SUPPLY; ;
    break;}
case 78:
#line 566 "parse.y"
{ yyval.drive.str1 = PGate::STRONG; ;
    break;}
case 79:
#line 567 "parse.y"
{ yyval.drive.str1 = PGate::PULL; ;
    break;}
case 80:
#line 568 "parse.y"
{ yyval.drive.str1 = PGate::WEAK; ;
    break;}
case 81:
#line 573 "parse.y"
{ PEIdent*tmpi = new PEIdent(*yyvsp[0].hier);
		  tmpi->set_file(yylsp[0].text);
		  tmpi->set_lineno(yylsp[0].first_line);
		  delete yyvsp[0].hier;
		  PEEvent*tmpe = new PEEvent(PEEvent::ANYEDGE, tmpi);
		  PEventStatement*tmps = new PEventStatement(tmpe);
		  tmps->set_file(yylsp[-1].text);
		  tmps->set_lineno(yylsp[-1].first_line);
		  yyval.event_statement = tmps;
		;
    break;}
case 82:
#line 584 "parse.y"
{ PEventStatement*tmp = new PEventStatement(*yyvsp[-1].event_expr);
		  tmp->set_file(yylsp[-3].text);
		  tmp->set_lineno(yylsp[-3].first_line);
		  delete yyvsp[-1].event_expr;
		  yyval.event_statement = tmp;
		;
    break;}
case 83:
#line 591 "parse.y"
{ yyerror(yylsp[-3], "error: Malformed event control expression.");
		  yyval.event_statement = 0;
		;
    break;}
case 84:
#line 598 "parse.y"
{ yyval.event_expr = yyvsp[0].event_expr; ;
    break;}
case 85:
#line 600 "parse.y"
{ svector<PEEvent*>*tmp = new svector<PEEvent*>(*yyvsp[-2].event_expr, *yyvsp[0].event_expr);
		  delete yyvsp[-2].event_expr;
		  delete yyvsp[0].event_expr;
		  yyval.event_expr = tmp;
		;
    break;}
case 86:
#line 606 "parse.y"
{ svector<PEEvent*>*tmp = new svector<PEEvent*>(*yyvsp[-2].event_expr, *yyvsp[0].event_expr);
		  delete yyvsp[-2].event_expr;
		  delete yyvsp[0].event_expr;
		  yyval.event_expr = tmp;
		;
    break;}
case 87:
#line 615 "parse.y"
{ PEEvent*tmp = new PEEvent(PEEvent::POSEDGE, yyvsp[0].expr);
		  tmp->set_file(yylsp[-1].text);
		  tmp->set_lineno(yylsp[-1].first_line);
		  svector<PEEvent*>*tl = new svector<PEEvent*>(1);
		  (*tl)[0] = tmp;
		  yyval.event_expr = tl;
		;
    break;}
case 88:
#line 623 "parse.y"
{ PEEvent*tmp = new PEEvent(PEEvent::NEGEDGE, yyvsp[0].expr);
		  tmp->set_file(yylsp[-1].text);
		  tmp->set_lineno(yylsp[-1].first_line);
		  svector<PEEvent*>*tl = new svector<PEEvent*>(1);
		  (*tl)[0] = tmp;
		  yyval.event_expr = tl;
		;
    break;}
case 89:
#line 631 "parse.y"
{ PEEvent*tmp = new PEEvent(PEEvent::ANYEDGE, yyvsp[0].expr);
		  tmp->set_file(yylsp[0].text);
		  tmp->set_lineno(yylsp[0].first_line);
		  svector<PEEvent*>*tl = new svector<PEEvent*>(1);
		  (*tl)[0] = tmp;
		  yyval.event_expr = tl;
		;
    break;}
case 90:
#line 642 "parse.y"
{ yyval.expr = yyvsp[0].expr; ;
    break;}
case 91:
#line 644 "parse.y"
{ yyval.expr = yyvsp[0].expr; ;
    break;}
case 92:
#line 646 "parse.y"
{ PEUnary*tmp = new PEUnary('-', yyvsp[0].expr);
		  tmp->set_file(yylsp[0].text);
		  tmp->set_lineno(yylsp[0].first_line);
		  yyval.expr = tmp;
		;
    break;}
case 93:
#line 652 "parse.y"
{ PEUnary*tmp = new PEUnary('~', yyvsp[0].expr);
		  tmp->set_file(yylsp[0].text);
		  tmp->set_lineno(yylsp[0].first_line);
		  yyval.expr = tmp;
		;
    break;}
case 94:
#line 658 "parse.y"
{ PEUnary*tmp = new PEUnary('&', yyvsp[0].expr);
		  tmp->set_file(yylsp[0].text);
		  tmp->set_lineno(yylsp[0].first_line);
		  yyval.expr = tmp;
		;
    break;}
case 95:
#line 664 "parse.y"
{ PEUnary*tmp = new PEUnary('!', yyvsp[0].expr);
		  tmp->set_file(yylsp[0].text);
		  tmp->set_lineno(yylsp[0].first_line);
		  yyval.expr = tmp;
		;
    break;}
case 96:
#line 670 "parse.y"
{ PEUnary*tmp = new PEUnary('|', yyvsp[0].expr);
		  tmp->set_file(yylsp[0].text);
		  tmp->set_lineno(yylsp[0].first_line);
		  yyval.expr = tmp;
		;
    break;}
case 97:
#line 676 "parse.y"
{ PEUnary*tmp = new PEUnary('^', yyvsp[0].expr);
		  tmp->set_file(yylsp[0].text);
		  tmp->set_lineno(yylsp[0].first_line);
		  yyval.expr = tmp;
		;
    break;}
case 98:
#line 682 "parse.y"
{ PEUnary*tmp = new PEUnary('A', yyvsp[0].expr);
		  tmp->set_file(yylsp[0].text);
		  tmp->set_lineno(yylsp[0].first_line);
		  yyval.expr = tmp;
		;
    break;}
case 99:
#line 688 "parse.y"
{ PEUnary*tmp = new PEUnary('N', yyvsp[0].expr);
		  tmp->set_file(yylsp[0].text);
		  tmp->set_lineno(yylsp[0].first_line);
		  yyval.expr = tmp;
		;
    break;}
case 100:
#line 694 "parse.y"
{ PEUnary*tmp = new PEUnary('X', yyvsp[0].expr);
		  tmp->set_file(yylsp[0].text);
		  tmp->set_lineno(yylsp[0].first_line);
		  yyval.expr = tmp;
		;
    break;}
case 101:
#line 700 "parse.y"
{ yyerror(yylsp[-1], "error: Operand of unary ! "
			  "is not a primary expression.");
		  yyval.expr = 0;
		;
    break;}
case 102:
#line 705 "parse.y"
{ yyerror(yylsp[-1], "error: Operand of reduction ^ "
			  "is not a primary expression.");
		  yyval.expr = 0;
		;
    break;}
case 103:
#line 710 "parse.y"
{ PEBinary*tmp = new PEBinary('^', yyvsp[-2].expr, yyvsp[0].expr);
		  tmp->set_file(yylsp[-1].text);
		  tmp->set_lineno(yylsp[-1].first_line);
		  yyval.expr = tmp;
		;
    break;}
case 104:
#line 716 "parse.y"
{ PEBinary*tmp = new PEBinary('*', yyvsp[-2].expr, yyvsp[0].expr);
		  tmp->set_file(yylsp[-1].text);
		  tmp->set_lineno(yylsp[-1].first_line);
		  yyval.expr = tmp;
		;
    break;}
case 105:
#line 722 "parse.y"
{ PEBinary*tmp = new PEBinary('/', yyvsp[-2].expr, yyvsp[0].expr);
		  tmp->set_file(yylsp[-1].text);
		  tmp->set_lineno(yylsp[-1].first_line);
		  yyval.expr = tmp;
		;
    break;}
case 106:
#line 728 "parse.y"
{ PEBinary*tmp = new PEBinary('%', yyvsp[-2].expr, yyvsp[0].expr);
		  tmp->set_file(yylsp[-1].text);
		  tmp->set_lineno(yylsp[-1].first_line);
		  yyval.expr = tmp;
		;
    break;}
case 107:
#line 734 "parse.y"
{ PEBinary*tmp = new PEBinary('+', yyvsp[-2].expr, yyvsp[0].expr);
		  tmp->set_file(yylsp[-1].text);
		  tmp->set_lineno(yylsp[-1].first_line);
		  yyval.expr = tmp;
		;
    break;}
case 108:
#line 740 "parse.y"
{ PEBinary*tmp = new PEBinary('-', yyvsp[-2].expr, yyvsp[0].expr);
		  tmp->set_file(yylsp[-1].text);
		  tmp->set_lineno(yylsp[-1].first_line);
		  yyval.expr = tmp;
		;
    break;}
case 109:
#line 746 "parse.y"
{ PEBinary*tmp = new PEBinary('&', yyvsp[-2].expr, yyvsp[0].expr);
		  tmp->set_file(yylsp[-1].text);
		  tmp->set_lineno(yylsp[-1].first_line);
		  yyval.expr = tmp;
		;
    break;}
case 110:
#line 752 "parse.y"
{ PEBinary*tmp = new PEBinary('|', yyvsp[-2].expr, yyvsp[0].expr);
		  tmp->set_file(yylsp[-1].text);
		  tmp->set_lineno(yylsp[-1].first_line);
		  yyval.expr = tmp;
		;
    break;}
case 111:
#line 758 "parse.y"
{ PEBinary*tmp = new PEBinary('A', yyvsp[-2].expr, yyvsp[0].expr);
		  tmp->set_file(yylsp[-1].text);
		  tmp->set_lineno(yylsp[-1].first_line);
		  yyval.expr = tmp;
		;
    break;}
case 112:
#line 764 "parse.y"
{ PEBinary*tmp = new PEBinary('O', yyvsp[-2].expr, yyvsp[0].expr);
		  tmp->set_file(yylsp[-1].text);
		  tmp->set_lineno(yylsp[-1].first_line);
		  yyval.expr = tmp;
		;
    break;}
case 113:
#line 770 "parse.y"
{ PEBinary*tmp = new PEBinary('X', yyvsp[-2].expr, yyvsp[0].expr);
		  tmp->set_file(yylsp[-1].text);
		  tmp->set_lineno(yylsp[-1].first_line);
		  yyval.expr = tmp;
		;
    break;}
case 114:
#line 776 "parse.y"
{ PEBinary*tmp = new PEBinary('<', yyvsp[-2].expr, yyvsp[0].expr);
		  tmp->set_file(yylsp[-1].text);
		  tmp->set_lineno(yylsp[-1].first_line);
		  yyval.expr = tmp;
		;
    break;}
case 115:
#line 782 "parse.y"
{ PEBinary*tmp = new PEBinary('>', yyvsp[-2].expr, yyvsp[0].expr);
		  tmp->set_file(yylsp[-1].text);
		  tmp->set_lineno(yylsp[-1].first_line);
		  yyval.expr = tmp;
		;
    break;}
case 116:
#line 788 "parse.y"
{ PEBinary*tmp = new PEBinary('l', yyvsp[-2].expr, yyvsp[0].expr);
		  tmp->set_file(yylsp[-1].text);
		  tmp->set_lineno(yylsp[-1].first_line);
		  yyval.expr = tmp;
		;
    break;}
case 117:
#line 794 "parse.y"
{ PEBinary*tmp = new PEBinary('r', yyvsp[-2].expr, yyvsp[0].expr);
		  tmp->set_file(yylsp[-1].text);
		  tmp->set_lineno(yylsp[-1].first_line);
		  yyval.expr = tmp;
		;
    break;}
case 118:
#line 800 "parse.y"
{ PEBinary*tmp = new PEBinary('R', yyvsp[-2].expr, yyvsp[0].expr);
		  tmp->set_file(yylsp[-1].text);
		  tmp->set_lineno(yylsp[-1].first_line);
		  yyval.expr = tmp;
		;
    break;}
case 119:
#line 806 "parse.y"
{ PEBinary*tmp = new PEBinary('e', yyvsp[-2].expr, yyvsp[0].expr);
		  tmp->set_file(yylsp[-1].text);
		  tmp->set_lineno(yylsp[-1].first_line);
		  yyval.expr = tmp;
		;
    break;}
case 120:
#line 812 "parse.y"
{ PEBinary*tmp = new PEBinary('E', yyvsp[-2].expr, yyvsp[0].expr);
		  tmp->set_file(yylsp[-1].text);
		  tmp->set_lineno(yylsp[-1].first_line);
		  yyval.expr = tmp;
		;
    break;}
case 121:
#line 818 "parse.y"
{ PEBinary*tmp = new PEBinary('L', yyvsp[-2].expr, yyvsp[0].expr);
		  tmp->set_file(yylsp[-1].text);
		  tmp->set_lineno(yylsp[-1].first_line);
		  yyval.expr = tmp;
		;
    break;}
case 122:
#line 824 "parse.y"
{ PEBinary*tmp = new PEBinary('G', yyvsp[-2].expr, yyvsp[0].expr);
		  tmp->set_file(yylsp[-1].text);
		  tmp->set_lineno(yylsp[-1].first_line);
		  yyval.expr = tmp;
		;
    break;}
case 123:
#line 830 "parse.y"
{ PEBinary*tmp = new PEBinary('n', yyvsp[-2].expr, yyvsp[0].expr);
		  tmp->set_file(yylsp[-1].text);
		  tmp->set_lineno(yylsp[-1].first_line);
		  yyval.expr = tmp;
		;
    break;}
case 124:
#line 836 "parse.y"
{ PEBinary*tmp = new PEBinary('N', yyvsp[-2].expr, yyvsp[0].expr);
		  tmp->set_file(yylsp[-1].text);
		  tmp->set_lineno(yylsp[-1].first_line);
		  yyval.expr = tmp;
		;
    break;}
case 125:
#line 842 "parse.y"
{ PEBinary*tmp = new PEBinary('o', yyvsp[-2].expr, yyvsp[0].expr);
		  tmp->set_file(yylsp[-1].text);
		  tmp->set_lineno(yylsp[-1].first_line);
		  yyval.expr = tmp;
		;
    break;}
case 126:
#line 848 "parse.y"
{ PEBinary*tmp = new PEBinary('a', yyvsp[-2].expr, yyvsp[0].expr);
		  tmp->set_file(yylsp[-1].text);
		  tmp->set_lineno(yylsp[-1].first_line);
		  yyval.expr = tmp;
		;
    break;}
case 127:
#line 854 "parse.y"
{ PETernary*tmp = new PETernary(yyvsp[-4].expr, yyvsp[-2].expr, yyvsp[0].expr);
		  tmp->set_file(yylsp[-3].text);
		  tmp->set_lineno(yylsp[-3].first_line);
		  yyval.expr = tmp;
		;
    break;}
case 128:
#line 867 "parse.y"
{ svector<PExpr*>*tmp = new svector<PExpr*>(*yyvsp[-2].exprs, yyvsp[0].expr);
		  delete yyvsp[-2].exprs;
		  yyval.exprs = tmp;
		;
    break;}
case 129:
#line 872 "parse.y"
{ svector<PExpr*>*tmp = new svector<PExpr*>(1);
		  (*tmp)[0] = yyvsp[0].expr;
		  yyval.exprs = tmp;
		;
    break;}
case 130:
#line 877 "parse.y"
{ svector<PExpr*>*tmp = new svector<PExpr*>(1);
		  (*tmp)[0] = 0;
		  yyval.exprs = tmp;
		;
    break;}
case 131:
#line 883 "parse.y"
{ svector<PExpr*>*tmp = new svector<PExpr*>(*yyvsp[-1].exprs, 0);
		  delete yyvsp[-1].exprs;
		  yyval.exprs = tmp;
		;
    break;}
case 132:
#line 892 "parse.y"
{ assert(yyvsp[0].number);
		  PENumber*tmp = new PENumber(yyvsp[0].number);
		  tmp->set_file(yylsp[0].text);
		  tmp->set_lineno(yylsp[0].first_line);
		  yyval.expr = tmp;
		;
    break;}
case 133:
#line 899 "parse.y"
{ PEFNumber*tmp = new PEFNumber(yyvsp[0].realtime);
		  tmp->set_file(yylsp[0].text);
		  tmp->set_lineno(yylsp[0].first_line);
		  yyval.expr = tmp;
		;
    break;}
case 134:
#line 905 "parse.y"
{ PEString*tmp = new PEString(yyvsp[0].text);
		  tmp->set_file(yylsp[0].text);
		  tmp->set_lineno(yylsp[0].first_line);
		  yyval.expr = tmp;
		;
    break;}
case 135:
#line 911 "parse.y"
{ PEIdent*tmp = new PEIdent(*yyvsp[0].hier);
		  tmp->set_file(yylsp[0].text);
		  tmp->set_lineno(yylsp[0].first_line);
		  yyval.expr = tmp;
		  delete yyvsp[0].hier;
		;
    break;}
case 136:
#line 918 "parse.y"
{ PECallFunction*tmp = new PECallFunction(hname_t(yyvsp[0].text));
		  tmp->set_file(yylsp[0].text);
		  tmp->set_lineno(yylsp[0].first_line);
		  yyval.expr = tmp;
		  delete yyvsp[0].text;
		;
    break;}
case 137:
#line 925 "parse.y"
{ PEIdent*tmp = new PEIdent(*yyvsp[-3].hier);
		  tmp->set_file(yylsp[-3].text);
		  tmp->set_lineno(yylsp[-3].first_line);
		  tmp->msb_ = yyvsp[-1].expr;
		  delete yyvsp[-3].hier;
		  yyval.expr = tmp;
		;
    break;}
case 138:
#line 933 "parse.y"
{ PEIdent*tmp = new PEIdent(*yyvsp[-5].hier);
		  tmp->set_file(yylsp[-5].text);
		  tmp->set_lineno(yylsp[-5].first_line);
		  tmp->msb_ = yyvsp[-3].expr;
		  tmp->lsb_ = yyvsp[-1].expr;
		  delete yyvsp[-5].hier;
		  yyval.expr = tmp;
		;
    break;}
case 139:
#line 942 "parse.y"
{ PECallFunction*tmp = new PECallFunction(*yyvsp[-3].hier, *yyvsp[-1].exprs);
		  tmp->set_file(yylsp[-3].text);
		  tmp->set_lineno(yylsp[-3].first_line);
		  delete yyvsp[-3].hier;
		  yyval.expr = tmp;
		;
    break;}
case 140:
#line 949 "parse.y"
{ PECallFunction*tmp = new PECallFunction(hname_t(yyvsp[-3].text), *yyvsp[-1].exprs);
		  tmp->set_file(yylsp[-3].text);
		  tmp->set_lineno(yylsp[-3].first_line);
		  yyval.expr = tmp;
		;
    break;}
case 141:
#line 955 "parse.y"
{ yyval.expr = yyvsp[-1].expr; ;
    break;}
case 142:
#line 957 "parse.y"
{ PEConcat*tmp = new PEConcat(*yyvsp[-1].exprs);
		  tmp->set_file(yylsp[-2].text);
		  tmp->set_lineno(yylsp[-2].first_line);
		  delete yyvsp[-1].exprs;
		  yyval.expr = tmp;
		;
    break;}
case 143:
#line 964 "parse.y"
{ PExpr*rep = yyvsp[-4].expr;
		  if (!pform_expression_is_constant(yyvsp[-4].expr)) {
			yyerror(yylsp[-4], "error: Repeat expression "
			            "must be constant.");
			delete rep;
			delete yyvsp[-4].expr;
			rep = 0;
		  }
		  PEConcat*tmp = new PEConcat(*yyvsp[-2].exprs, rep);
		  tmp->set_file(yylsp[-5].text);
		  tmp->set_lineno(yylsp[-5].first_line);
		  delete yyvsp[-2].exprs;
		  yyval.expr = tmp;
		;
    break;}
case 144:
#line 986 "parse.y"
{ svector<PWire*>*tmp
			= pform_make_task_ports(NetNet::PINPUT, false,
						yyvsp[-2].exprs, yyvsp[-1].texts,
						yylsp[-3].text, yylsp[-3].first_line);
		  yyval.wires = tmp;
		;
    break;}
case 145:
#line 993 "parse.y"
{ svector<PWire*>*tmp
			= pform_make_task_ports(NetNet::PINPUT, true,
						yyvsp[-2].exprs, yyvsp[-1].texts,
						yylsp[-4].text, yylsp[-4].first_line);
		  yyval.wires = tmp;
		;
    break;}
case 146:
#line 1000 "parse.y"
{ svector<PWire*>*tmp
			= pform_make_task_ports(NetNet::PINPUT, false,
						yyvsp[-2].exprs, yyvsp[-1].texts,
						yylsp[-3].text, yylsp[-3].first_line);
		  yyval.wires = tmp;
		  yyerror(yylsp[-3], "Functions may not have output ports.");
		;
    break;}
case 147:
#line 1008 "parse.y"
{ svector<PWire*>*tmp
			= pform_make_task_ports(NetNet::PINPUT, false,
						yyvsp[-2].exprs, yyvsp[-1].texts,
						yylsp[-3].text, yylsp[-3].first_line);
		  yyval.wires = tmp;
		  yyerror(yylsp[-3], "Functions may not have inout ports.");
		;
    break;}
case 148:
#line 1016 "parse.y"
{ yyval.wires = 0; ;
    break;}
case 149:
#line 1025 "parse.y"
{ yyval.wires = yyvsp[0].wires; ;
    break;}
case 150:
#line 1027 "parse.y"
{ if (yyvsp[-1].wires && yyvsp[0].wires) {
		        svector<PWire*>*tmp = new svector<PWire*>(*yyvsp[-1].wires, *yyvsp[0].wires);
			delete yyvsp[-1].wires;
			delete yyvsp[0].wires;
			yyval.wires = tmp;
		  } else if (yyvsp[-1].wires) {
			yyval.wires = yyvsp[-1].wires;
		  } else {
			yyval.wires = yyvsp[0].wires;
		  }
		;
    break;}
case 151:
#line 1044 "parse.y"
{ lgate*tmp = new lgate;
		  tmp->name = yyvsp[-3].text;
		  tmp->parms = yyvsp[-1].exprs;
		  tmp->file  = yylsp[-3].text;
		  tmp->lineno = yylsp[-3].first_line;
		  delete yyvsp[-3].text;
		  yyval.gate = tmp;
		;
    break;}
case 152:
#line 1054 "parse.y"
{ lgate*tmp = new lgate;
		  svector<PExpr*>*rng = yyvsp[-3].exprs;
		  tmp->name = yyvsp[-4].text;
		  tmp->parms = yyvsp[-1].exprs;
		  tmp->range[0] = (*rng)[0];
		  tmp->range[1] = (*rng)[1];
		  tmp->file  = yylsp[-4].text;
		  tmp->lineno = yylsp[-4].first_line;
		  delete yyvsp[-4].text;
		  delete rng;
		  yyval.gate = tmp;
		;
    break;}
case 153:
#line 1067 "parse.y"
{ lgate*tmp = new lgate;
		  tmp->name = "";
		  tmp->parms = yyvsp[-1].exprs;
		  tmp->file  = yylsp[-2].text;
		  tmp->lineno = yylsp[-2].first_line;
		  yyval.gate = tmp;
		;
    break;}
case 154:
#line 1078 "parse.y"
{ lgate*tmp = new lgate;
		  tmp->name = yyvsp[-3].text;
		  tmp->parms_by_name = yyvsp[-1].named_pexprs;
		  tmp->file  = yylsp[-3].text;
		  tmp->lineno = yylsp[-3].first_line;
		  delete yyvsp[-3].text;
		  yyval.gate = tmp;
		;
    break;}
case 155:
#line 1090 "parse.y"
{ svector<lgate>*tmp1 = yyvsp[-2].gates;
		  lgate*tmp2 = yyvsp[0].gate;
		  svector<lgate>*out = new svector<lgate> (*tmp1, *tmp2);
		  delete tmp1;
		  delete tmp2;
		  yyval.gates = out;
		;
    break;}
case 156:
#line 1098 "parse.y"
{ svector<lgate>*tmp = new svector<lgate>(1);
		  (*tmp)[0] = *yyvsp[0].gate;
		  delete yyvsp[0].gate;
		  yyval.gates = tmp;
		;
    break;}
case 157:
#line 1106 "parse.y"
{ yyval.gatetype = PGBuiltin::AND; ;
    break;}
case 158:
#line 1107 "parse.y"
{ yyval.gatetype = PGBuiltin::NAND; ;
    break;}
case 159:
#line 1108 "parse.y"
{ yyval.gatetype = PGBuiltin::OR; ;
    break;}
case 160:
#line 1109 "parse.y"
{ yyval.gatetype = PGBuiltin::NOR; ;
    break;}
case 161:
#line 1110 "parse.y"
{ yyval.gatetype = PGBuiltin::XOR; ;
    break;}
case 162:
#line 1111 "parse.y"
{ yyval.gatetype = PGBuiltin::XNOR; ;
    break;}
case 163:
#line 1112 "parse.y"
{ yyval.gatetype = PGBuiltin::BUF; ;
    break;}
case 164:
#line 1113 "parse.y"
{ yyval.gatetype = PGBuiltin::BUFIF0; ;
    break;}
case 165:
#line 1114 "parse.y"
{ yyval.gatetype = PGBuiltin::BUFIF1; ;
    break;}
case 166:
#line 1115 "parse.y"
{ yyval.gatetype = PGBuiltin::NOT; ;
    break;}
case 167:
#line 1116 "parse.y"
{ yyval.gatetype = PGBuiltin::NOTIF0; ;
    break;}
case 168:
#line 1117 "parse.y"
{ yyval.gatetype = PGBuiltin::NOTIF1; ;
    break;}
case 169:
#line 1118 "parse.y"
{ yyval.gatetype = PGBuiltin::NMOS; ;
    break;}
case 170:
#line 1119 "parse.y"
{ yyval.gatetype = PGBuiltin::RNMOS; ;
    break;}
case 171:
#line 1120 "parse.y"
{ yyval.gatetype = PGBuiltin::PMOS; ;
    break;}
case 172:
#line 1121 "parse.y"
{ yyval.gatetype = PGBuiltin::RPMOS; ;
    break;}
case 173:
#line 1122 "parse.y"
{ yyval.gatetype = PGBuiltin::CMOS; ;
    break;}
case 174:
#line 1123 "parse.y"
{ yyval.gatetype = PGBuiltin::RCMOS; ;
    break;}
case 175:
#line 1124 "parse.y"
{ yyval.gatetype = PGBuiltin::TRAN; ;
    break;}
case 176:
#line 1125 "parse.y"
{ yyval.gatetype = PGBuiltin::RTRAN; ;
    break;}
case 177:
#line 1126 "parse.y"
{ yyval.gatetype = PGBuiltin::TRANIF0; ;
    break;}
case 178:
#line 1127 "parse.y"
{ yyval.gatetype = PGBuiltin::TRANIF1; ;
    break;}
case 179:
#line 1128 "parse.y"
{ yyval.gatetype = PGBuiltin::RTRANIF0; ;
    break;}
case 180:
#line 1129 "parse.y"
{ yyval.gatetype = PGBuiltin::RTRANIF1; ;
    break;}
case 181:
#line 1138 "parse.y"
{ yyval.hier = new hname_t(yyvsp[0].text);
		  delete yyvsp[0].text;
		;
    break;}
case 182:
#line 1142 "parse.y"
{ hname_t * tmp = yyvsp[-2].hier;
		  tmp->append(yyvsp[0].text);
		  delete yyvsp[0].text;
		  yyval.hier = tmp;
		;
    break;}
case 183:
#line 1154 "parse.y"
{ list<char*>*tmp = new list<char*>;
		  tmp->push_back(yyvsp[0].text);
		  yyval.texts = tmp;
		;
    break;}
case 184:
#line 1159 "parse.y"
{ list<char*>*tmp = yyvsp[-2].texts;
		  tmp->push_back(yyvsp[0].text);
		  yyval.texts = tmp;
		;
    break;}
case 185:
#line 1184 "parse.y"
{ svector<Module::port_t*>*tmp
			  = new svector<Module::port_t*>(1);
		  (*tmp)[0] = yyvsp[0].mport;
		  yyval.mports = tmp;
		;
    break;}
case 186:
#line 1190 "parse.y"
{ svector<Module::port_t*>*tmp
			= new svector<Module::port_t*>(*yyvsp[-2].mports, yyvsp[0].mport);
		  delete yyvsp[-2].mports;
		  yyval.mports = tmp;
		;
    break;}
case 187:
#line 1199 "parse.y"
{ svector<Module::port_t*>*tmp
			  = new svector<Module::port_t*>(1);
		  (*tmp)[0] = yyvsp[0].mport;
		  yyval.mports = tmp;
		;
    break;}
case 188:
#line 1205 "parse.y"
{ svector<Module::port_t*>*tmp
			= new svector<Module::port_t*>(*yyvsp[-2].mports, yyvsp[0].mport);
		  delete yyvsp[-2].mports;
		  yyval.mports = tmp;
		;
    break;}
case 189:
#line 1211 "parse.y"
{ Module::port_t*ptmp;
		  ptmp = pform_module_port_reference(yyvsp[0].text, yylsp[0].text,
						     yylsp[0].first_line);
		  svector<Module::port_t*>*tmp
			= new svector<Module::port_t*>(*yyvsp[-2].mports, ptmp);

		    /* Get the port declaration details, the port type
		       and what not, from context data stored by the
		       last port_declaration rule. */
		  pform_module_define_port(yylsp[0], yyvsp[0].text,
					port_declaration_context.port_type,
					port_declaration_context.port_net_type,
					port_declaration_context.sign_flag,
					port_declaration_context.range, 0);
		  delete yyvsp[-2].mports;
		  yyval.mports = tmp;
		;
    break;}
case 190:
#line 1233 "parse.y"
{ Module::port_t*ptmp;
		  ptmp = pform_module_port_reference(yyvsp[0].text, yylsp[-4].text,
						     yylsp[-4].first_line);
		  pform_module_define_port(yylsp[-4], yyvsp[0].text, NetNet::PINPUT,
					   yyvsp[-3].nettype, yyvsp[-2].flag, yyvsp[-1].exprs, yyvsp[-5].named_pexprs);
		  port_declaration_context.port_type = NetNet::PINPUT;
		  port_declaration_context.port_net_type = yyvsp[-3].nettype;
		  port_declaration_context.sign_flag = yyvsp[-2].flag;
		  port_declaration_context.range = yyvsp[-1].exprs;
		  delete yyvsp[-5].named_pexprs;
		  delete yyvsp[0].text;
		  yyval.mport = ptmp;
		;
    break;}
case 191:
#line 1248 "parse.y"
{ Module::port_t*ptmp;
		  ptmp = pform_module_port_reference(yyvsp[0].text, yylsp[-4].text,
						     yylsp[-4].first_line);
		  pform_module_define_port(yylsp[-4], yyvsp[0].text, NetNet::PINOUT,
					   yyvsp[-3].nettype, yyvsp[-2].flag, yyvsp[-1].exprs, yyvsp[-5].named_pexprs);
		  port_declaration_context.port_type = NetNet::PINOUT;
		  port_declaration_context.port_net_type = yyvsp[-3].nettype;
		  port_declaration_context.sign_flag = yyvsp[-2].flag;
		  port_declaration_context.range = yyvsp[-1].exprs;
		  delete yyvsp[-5].named_pexprs;
		  delete yyvsp[0].text;
		  yyval.mport = ptmp;
		;
    break;}
case 192:
#line 1263 "parse.y"
{ Module::port_t*ptmp;
		  ptmp = pform_module_port_reference(yyvsp[0].text, yylsp[-4].text,
						     yylsp[-4].first_line);
		  pform_module_define_port(yylsp[-4], yyvsp[0].text, NetNet::POUTPUT,
					   yyvsp[-3].nettype, yyvsp[-2].flag, yyvsp[-1].exprs, yyvsp[-5].named_pexprs);
		  port_declaration_context.port_type = NetNet::POUTPUT;
		  port_declaration_context.port_net_type = yyvsp[-3].nettype;
		  port_declaration_context.sign_flag = yyvsp[-2].flag;
		  port_declaration_context.range = yyvsp[-1].exprs;
		  delete yyvsp[-5].named_pexprs;
		  delete yyvsp[0].text;
		  yyval.mport = ptmp;
		;
    break;}
case 193:
#line 1278 "parse.y"
{ Module::port_t*ptmp;
		  ptmp = pform_module_port_reference(yyvsp[0].text, yylsp[-4].text,
						     yylsp[-4].first_line);
		  pform_module_define_port(yylsp[-4], yyvsp[0].text, NetNet::POUTPUT,
					   yyvsp[-3].nettype, yyvsp[-2].flag, yyvsp[-1].exprs, yyvsp[-5].named_pexprs);
		  port_declaration_context.port_type = NetNet::POUTPUT;
		  port_declaration_context.port_net_type = yyvsp[-3].nettype;
		  port_declaration_context.sign_flag = yyvsp[-2].flag;
		  port_declaration_context.range = yyvsp[-1].exprs;
		  delete yyvsp[-5].named_pexprs;
		  delete yyvsp[0].text;
		  yyval.mport = ptmp;
		;
    break;}
case 194:
#line 1294 "parse.y"
{ yyval.nettype = yyvsp[0].nettype; ;
    break;}
case 195:
#line 1295 "parse.y"
{ yyval.nettype = NetNet::IMPLICIT; ;
    break;}
case 196:
#line 1299 "parse.y"
{ yyval.mports = yyvsp[-1].mports; ;
    break;}
case 197:
#line 1300 "parse.y"
{ yyval.mports = yyvsp[-1].mports; ;
    break;}
case 198:
#line 1301 "parse.y"
{ yyval.mports = 0; ;
    break;}
case 199:
#line 1304 "parse.y"
{ yyval.flag = true; ;
    break;}
case 200:
#line 1304 "parse.y"
{yyval.flag = false; ;
    break;}
case 201:
#line 1311 "parse.y"
{ PEIdent*tmp = new PEIdent(*yyvsp[0].hier);
		  tmp->set_file(yylsp[0].text);
		  tmp->set_lineno(yylsp[0].first_line);
		  delete yyvsp[0].hier;
		  yyval.expr = tmp;
		;
    break;}
case 202:
#line 1318 "parse.y"
{ PEIdent*tmp = new PEIdent(*yyvsp[-3].hier);
		  PExpr*sel = yyvsp[-1].expr;
    		  if (! pform_expression_is_constant(sel)) {
			yyerror(yylsp[-2], "error: Bit select in lvalue must "
			        "contain a constant expression.");
			delete sel;
		  } else {
			tmp->msb_ = sel;
		  }
		  tmp->set_file(yylsp[-3].text);
		  tmp->set_lineno(yylsp[-3].first_line);
		  delete yyvsp[-3].hier;
		  yyval.expr = tmp;
		;
    break;}
case 203:
#line 1333 "parse.y"
{ PEIdent*tmp = new PEIdent(*yyvsp[-1].hier);
		  assert(yyvsp[0].exprs->count() == 2);
		  tmp->msb_ = (*yyvsp[0].exprs)[0];
		  tmp->lsb_ = (*yyvsp[0].exprs)[1];
		  tmp->set_file(yylsp[-1].text);
		  tmp->set_lineno(yylsp[-1].first_line);
		  delete yyvsp[-1].hier;
		  delete yyvsp[0].exprs;
		  yyval.expr = tmp;
		;
    break;}
case 204:
#line 1344 "parse.y"
{ PEConcat*tmp = new PEConcat(*yyvsp[-1].exprs);
		  tmp->set_file(yylsp[-2].text);
		  tmp->set_lineno(yylsp[-2].first_line);
		  delete yyvsp[-1].exprs;
		  yyval.expr = tmp;
		;
    break;}
case 205:
#line 1356 "parse.y"
{ PEIdent*tmp = new PEIdent(*yyvsp[0].hier);
		  tmp->set_file(yylsp[0].text);
		  tmp->set_lineno(yylsp[0].first_line);
		  delete yyvsp[0].hier;
		  yyval.expr = tmp;
		;
    break;}
case 206:
#line 1363 "parse.y"
{ PEIdent*tmp = new PEIdent(*yyvsp[-3].hier);
		  tmp->msb_ = yyvsp[-1].expr;
		  tmp->set_file(yylsp[-3].text);
		  tmp->set_lineno(yylsp[-3].first_line);
		  delete yyvsp[-3].hier;

		  yyval.expr = tmp;
		;
    break;}
case 207:
#line 1372 "parse.y"
{ PEIdent*tmp = new PEIdent(*yyvsp[-5].hier);
		  tmp->msb_ = yyvsp[-3].expr;
		  tmp->lsb_ = yyvsp[-1].expr;
		  tmp->set_file(yylsp[-5].text);
		  tmp->set_lineno(yylsp[-5].first_line);
		  delete yyvsp[-5].hier;
		  yyval.expr = tmp;
		;
    break;}
case 208:
#line 1381 "parse.y"
{ PEConcat*tmp = new PEConcat(*yyvsp[-1].exprs);
		  tmp->set_file(yylsp[-2].text);
		  tmp->set_lineno(yylsp[-2].first_line);
		  delete yyvsp[-1].exprs;
		  yyval.expr = tmp;
		;
    break;}
case 209:
#line 1391 "parse.y"
{ svector<PExpr*>*tmp = new svector<PExpr*>(2);
		  (*tmp)[0] = yyvsp[-2].expr;
		  (*tmp)[1] = yyvsp[0].expr;
		  yyval.exprs = tmp;
		;
    break;}
case 210:
#line 1400 "parse.y"
{ svector<PExpr*>*tmp = new svector<PExpr*>(*yyvsp[-2].exprs, *yyvsp[0].exprs);
		  delete yyvsp[-2].exprs;
		  delete yyvsp[0].exprs;
		  yyval.exprs = tmp;
		;
    break;}
case 211:
#line 1406 "parse.y"
{ yyval.exprs = yyvsp[0].exprs; ;
    break;}
case 212:
#line 1415 "parse.y"
{ pform_startmodule(yyvsp[0].text, yylsp[-1].text, yylsp[-1].first_line, yyvsp[-2].named_pexprs); ;
    break;}
case 213:
#line 1418 "parse.y"
{ pform_module_set_ports(yyvsp[-1].mports); ;
    break;}
case 214:
#line 1421 "parse.y"
{ pform_endmodule(yyvsp[-7].text);
		  delete yyvsp[-7].text;
		;
    break;}
case 217:
#line 1430 "parse.y"
{ yyval.range_delay.range = yyvsp[-1].exprs; yyval.range_delay.delay = yyvsp[0].exprs; ;
    break;}
case 223:
#line 1450 "parse.y"
{ pform_makewire(yylsp[-4], yyvsp[-2].range_delay.range, yyvsp[-3].flag, yyvsp[-1].texts, yyvsp[-4].nettype,
				 NetNet::NOT_A_PORT, yyvsp[-5].named_pexprs);
		  if (yyvsp[-2].range_delay.delay != 0) {
			yyerror(yylsp[-2], "sorry: net delays not supported.");
			delete yyvsp[-2].range_delay.delay;
		  }
		  if (yyvsp[-5].named_pexprs) delete yyvsp[-5].named_pexprs;
		;
    break;}
case 224:
#line 1459 "parse.y"
{ pform_makewire(yylsp[-4], yyvsp[-2].range_delay.range, yyvsp[-3].flag, yyvsp[-2].range_delay.delay,
				 str_strength, yyvsp[-1].net_decl_assign, yyvsp[-4].nettype);
		  if (yyvsp[-5].named_pexprs) {
			yyerror(yylsp[-3], "sorry: Attributes not supported "
				"on net declaration assignments.");
			delete yyvsp[-5].named_pexprs;
		  }
		;
    break;}
case 225:
#line 1468 "parse.y"
{ pform_makewire(yylsp[-4], 0, yyvsp[-3].flag, 0, yyvsp[-2].drive, yyvsp[-1].net_decl_assign, yyvsp[-4].nettype);
		  if (yyvsp[-5].named_pexprs) {
			yyerror(yylsp[-2], "sorry: Attributes not supported "
				"on net declaration assignments.");
			delete yyvsp[-5].named_pexprs;
		  }
		;
    break;}
case 226:
#line 1476 "parse.y"
{ yyerror(yylsp[-4], "sorry: trireg nets not supported.");
		  delete yyvsp[-2].range_delay.range;
		  delete yyvsp[-2].range_delay.delay;
		;
    break;}
case 227:
#line 1482 "parse.y"
{ pform_set_port_type(yylsp[-4], yyvsp[-1].texts, yyvsp[-2].range_delay.range, yyvsp[-3].flag, yyvsp[-4].porttype);
		;
    break;}
case 228:
#line 1490 "parse.y"
{ pform_makewire(yylsp[-5], yyvsp[-2].exprs, yyvsp[-3].flag, yyvsp[-1].texts, yyvsp[-4].nettype, yyvsp[-5].porttype, 0);
		;
    break;}
case 229:
#line 1494 "parse.y"
{ pform_makewire(yylsp[-5], yyvsp[-2].exprs, yyvsp[-3].flag, yyvsp[-1].texts, yyvsp[-4].nettype, NetNet::POUTPUT, 0);
		;
    break;}
case 230:
#line 1502 "parse.y"
{ pform_makewire(yylsp[-5], yyvsp[-2].exprs, yyvsp[-3].flag, yyvsp[-1].texts, yyvsp[-4].nettype, NetNet::PINPUT, 0);
		  yyerror(yylsp[-4], "error: reg variables cannot be inputs.");
		;
    break;}
case 231:
#line 1507 "parse.y"
{ pform_makewire(yylsp[-5], yyvsp[-2].exprs, yyvsp[-3].flag, yyvsp[-1].texts, yyvsp[-4].nettype, NetNet::PINOUT, 0);
		  yyerror(yylsp[-4], "error: reg variables cannot be inouts.");
		;
    break;}
case 232:
#line 1512 "parse.y"
{ yyerror(yylsp[-2], "error: Invalid variable list"
			  " in port declaration.");
		  if (yyvsp[-2].range_delay.range) delete yyvsp[-2].range_delay.range;
		  if (yyvsp[-2].range_delay.delay) delete yyvsp[-2].range_delay.delay;
		  yyerrok;
		;
    break;}
case 235:
#line 1528 "parse.y"
{ pform_make_events(yyvsp[-1].texts, yylsp[-2].text, yylsp[-2].first_line);
		;
    break;}
case 236:
#line 1535 "parse.y"
{ pform_makegates(yyvsp[-2].gatetype, str_strength, 0, yyvsp[-1].gates, yyvsp[-3].named_pexprs);
		;
    break;}
case 237:
#line 1539 "parse.y"
{ pform_makegates(yyvsp[-3].gatetype, str_strength, yyvsp[-2].exprs, yyvsp[-1].gates, yyvsp[-4].named_pexprs);
		;
    break;}
case 238:
#line 1543 "parse.y"
{ pform_makegates(yyvsp[-3].gatetype, yyvsp[-2].drive, 0, yyvsp[-1].gates, yyvsp[-4].named_pexprs);
		;
    break;}
case 239:
#line 1547 "parse.y"
{ pform_makegates(yyvsp[-4].gatetype, yyvsp[-3].drive, yyvsp[-2].exprs, yyvsp[-1].gates, yyvsp[-5].named_pexprs);
		;
    break;}
case 240:
#line 1554 "parse.y"
{ pform_makegates(PGBuiltin::PULLUP, pull_strength, 0,
				  yyvsp[-1].gates, 0);
		;
    break;}
case 241:
#line 1558 "parse.y"
{ pform_makegates(PGBuiltin::PULLDOWN, pull_strength,
				  0, yyvsp[-1].gates, 0);
		;
    break;}
case 242:
#line 1563 "parse.y"
{ pform_makegates(PGBuiltin::PULLUP, yyvsp[-3].drive, 0, yyvsp[-1].gates, 0);
		;
    break;}
case 243:
#line 1567 "parse.y"
{ pform_makegates(PGBuiltin::PULLDOWN, yyvsp[-3].drive, 0, yyvsp[-1].gates, 0);
		;
    break;}
case 244:
#line 1575 "parse.y"
{ pform_make_modgates(yyvsp[-3].text, yyvsp[-2].parmvalue, yyvsp[-1].gates);
		  delete yyvsp[-3].text;
		;
    break;}
case 245:
#line 1584 "parse.y"
{ pform_make_pgassign_list(yyvsp[-1].exprs, yyvsp[-2].exprs, yyvsp[-3].drive, yylsp[-4].text, yylsp[-4].first_line); ;
    break;}
case 246:
#line 1589 "parse.y"
{ PProcess*tmp = pform_make_behavior(PProcess::PR_ALWAYS,
						     yyvsp[0].statement, yyvsp[-2].named_pexprs);
		  tmp->set_file(yylsp[-1].text);
		  tmp->set_lineno(yylsp[-1].first_line);
		;
    break;}
case 247:
#line 1595 "parse.y"
{ PProcess*tmp = pform_make_behavior(PProcess::PR_INITIAL,
						     yyvsp[0].statement, yyvsp[-2].named_pexprs);
		  tmp->set_file(yylsp[-1].text);
		  tmp->set_lineno(yylsp[-1].first_line);
		;
    break;}
case 248:
#line 1607 "parse.y"
{ pform_push_scope(yyvsp[-1].text); ;
    break;}
case 249:
#line 1610 "parse.y"
{ PTask*tmp = new PTask;
		  tmp->set_file(yylsp[-6].text);
		  tmp->set_lineno(yylsp[-6].first_line);
		  tmp->set_ports(yyvsp[-2].wires);
		  tmp->set_statement(yyvsp[-1].statement);
		  pform_set_task(yyvsp[-5].text, tmp);
		  pform_pop_scope();
		  delete yyvsp[-5].text;
		;
    break;}
case 250:
#line 1626 "parse.y"
{ pform_push_scope(yyvsp[-1].text); ;
    break;}
case 251:
#line 1629 "parse.y"
{ PFunction *tmp = new PFunction;
		  tmp->set_file(yylsp[-7].text);
		  tmp->set_lineno(yylsp[-7].first_line);
		  tmp->set_ports(yyvsp[-2].wires);
		  tmp->set_statement(yyvsp[-1].statement);
		  pform_set_function(yyvsp[-5].text, yyvsp[-6].range_type.ntype, yyvsp[-6].range_type.range, tmp);
		  pform_pop_scope();
		  delete yyvsp[-5].text;
		;
    break;}
case 252:
#line 1642 "parse.y"
{ /* empty lists are legal syntax. */ ;
    break;}
case 253:
#line 1645 "parse.y"
{
		;
    break;}
case 254:
#line 1649 "parse.y"
{ yyerror(yylsp[-2], "error: syntax error in specify block");
		  yyerrok;
		;
    break;}
case 255:
#line 1658 "parse.y"
{ yyerror(yylsp[-1], "error: invalid module item. "
			  "Did you forget an initial or always?");
		  yyerrok;
		;
    break;}
case 256:
#line 1664 "parse.y"
{ yyerror(yylsp[-4], "error: syntax error in left side "
			  "of continuous assignment.");
		  yyerrok;
		;
    break;}
case 257:
#line 1670 "parse.y"
{ yyerror(yylsp[-2], "error: syntax error in "
			  "continuous assignment");
		  yyerrok;
		;
    break;}
case 258:
#line 1676 "parse.y"
{ yyerror(yylsp[-2], "error: I give up on this "
			  "function definition.");
		  yyerrok;
		;
    break;}
case 259:
#line 1685 "parse.y"
{ pform_set_attrib(yyvsp[-6].text, yyvsp[-4].text, yyvsp[-2].text);
		  delete yyvsp[-6].text;
		  delete yyvsp[-4].text;
		;
    break;}
case 260:
#line 1690 "parse.y"
{ yyerror(yylsp[-4], "error: Malformed $attribute parameter list."); ;
    break;}
case 265:
#line 1714 "parse.y"
{ net_decl_assign_t*tmp = new net_decl_assign_t;
		  tmp->next = tmp;
		  tmp->name = yyvsp[-2].text;
		  tmp->expr = yyvsp[0].expr;
		  yyval.net_decl_assign = tmp;
		;
    break;}
case 266:
#line 1724 "parse.y"
{ net_decl_assign_t*tmp = yyvsp[-2].net_decl_assign;
		  yyvsp[0].net_decl_assign->next = tmp->next;
		  tmp->next = yyvsp[0].net_decl_assign;
		  yyval.net_decl_assign = tmp;
		;
    break;}
case 267:
#line 1730 "parse.y"
{ yyval.net_decl_assign = yyvsp[0].net_decl_assign;
		;
    break;}
case 268:
#line 1735 "parse.y"
{ yyval.nettype = NetNet::WIRE; ;
    break;}
case 269:
#line 1736 "parse.y"
{ yyval.nettype = NetNet::TRI; ;
    break;}
case 270:
#line 1737 "parse.y"
{ yyval.nettype = NetNet::TRI1; ;
    break;}
case 271:
#line 1738 "parse.y"
{ yyval.nettype = NetNet::SUPPLY0; ;
    break;}
case 272:
#line 1739 "parse.y"
{ yyval.nettype = NetNet::WAND; ;
    break;}
case 273:
#line 1740 "parse.y"
{ yyval.nettype = NetNet::TRIAND; ;
    break;}
case 274:
#line 1741 "parse.y"
{ yyval.nettype = NetNet::TRI0; ;
    break;}
case 275:
#line 1742 "parse.y"
{ yyval.nettype = NetNet::SUPPLY1; ;
    break;}
case 276:
#line 1743 "parse.y"
{ yyval.nettype = NetNet::WOR; ;
    break;}
case 277:
#line 1744 "parse.y"
{ yyval.nettype = NetNet::TRIOR; ;
    break;}
case 278:
#line 1748 "parse.y"
{ yyval.nettype = NetNet::REG; ;
    break;}
case 279:
#line 1753 "parse.y"
{ PExpr*tmp = yyvsp[0].expr;
  		  if (!pform_expression_is_constant(tmp)) {
			yyerror(yylsp[0], "error: parameter value "
			            "must be a constant expression.");
			delete tmp;
			tmp = 0;
		  } else {
			pform_set_parameter(yyvsp[-2].text, active_signed,
					    active_range, tmp);
		  }
		  delete yyvsp[-2].text;
		;
    break;}
case 281:
#line 1769 "parse.y"
{ active_range = yyvsp[0].exprs; active_signed = false; ;
    break;}
case 282:
#line 1771 "parse.y"
{ active_range = 0;
		  active_signed = false;
		;
    break;}
case 283:
#line 1774 "parse.y"
{ active_range = yyvsp[0].exprs; active_signed = true; ;
    break;}
case 284:
#line 1776 "parse.y"
{ active_range = 0;
		  active_signed = false;
		;
    break;}
case 287:
#line 1794 "parse.y"
{ PExpr*tmp = yyvsp[0].expr;
		  if (!pform_expression_is_constant(tmp)) {
			yyerror(yylsp[0], "error: parameter value "
			            "must be constant.");
			delete tmp;
			tmp = 0;
		  }
		  pform_set_localparam(yyvsp[-2].text, tmp);
		  delete yyvsp[-2].text;
		;
    break;}
case 289:
#line 1809 "parse.y"
{ yywarn(yylsp[-1], "Ranges in localparam definition "
		          "are not supported.");
		  delete yyvsp[-1].exprs;
		;
    break;}
case 291:
#line 1833 "parse.y"
{ struct parmvalue_t*tmp = new struct parmvalue_t;
		  tmp->by_order = yyvsp[-1].exprs;
		  tmp->by_name = 0;
		  yyval.parmvalue = tmp;
		;
    break;}
case 292:
#line 1839 "parse.y"
{ struct parmvalue_t*tmp = new struct parmvalue_t;
		  tmp->by_order = 0;
		  tmp->by_name = yyvsp[-1].named_pexprs;
		  yyval.parmvalue = tmp;
		;
    break;}
case 293:
#line 1845 "parse.y"
{ assert(yyvsp[0].number);
		  PENumber*tmp = new PENumber(yyvsp[0].number);
		  tmp->set_file(yylsp[-1].text);
		  tmp->set_lineno(yylsp[-1].first_line);

		  struct parmvalue_t*lst = new struct parmvalue_t;
		  lst->by_order = new svector<PExpr*>(1);
		  (*lst->by_order)[0] = tmp;
		  lst->by_name = 0;
		  yyval.parmvalue = lst;
		;
    break;}
case 294:
#line 1857 "parse.y"
{ yyerror(yylsp[-1], "error: syntax error in parameter value "
			  "assignment list.");
		  yyval.parmvalue = 0;
		;
    break;}
case 295:
#line 1862 "parse.y"
{ yyval.parmvalue = 0; ;
    break;}
case 296:
#line 1867 "parse.y"
{ named_pexpr_t*tmp = new named_pexpr_t;
		  tmp->name = yyvsp[-3].text;
		  tmp->parm = yyvsp[-1].expr;
		  free(yyvsp[-3].text);
		  yyval.named_pexpr = tmp;
		;
    break;}
case 297:
#line 1874 "parse.y"
{ named_pexpr_t*tmp = new named_pexpr_t;
		  tmp->name = yyvsp[-2].text;
		  tmp->parm = 0;
		  free(yyvsp[-2].text);
		  yyval.named_pexpr = tmp;
		;
    break;}
case 298:
#line 1884 "parse.y"
{ svector<named_pexpr_t*>*tmp = new svector<named_pexpr_t*>(1);
		  (*tmp)[0] = yyvsp[0].named_pexpr;
		  yyval.named_pexprs = tmp;
		;
    break;}
case 299:
#line 1889 "parse.y"
{ svector<named_pexpr_t*>*tmp =
			new svector<named_pexpr_t*>(*yyvsp[-2].named_pexprs,yyvsp[0].named_pexpr);
		  delete yyvsp[-2].named_pexprs;
		  yyval.named_pexprs = tmp;
		;
    break;}
case 300:
#line 1912 "parse.y"
{ yyval.mport = yyvsp[0].mport; ;
    break;}
case 301:
#line 1920 "parse.y"
{ Module::port_t*tmp = yyvsp[-1].mport;
		  tmp->name = yyvsp[-3].text;
		  delete yyvsp[-3].text;
		  yyval.mport = tmp;
		;
    break;}
case 302:
#line 1931 "parse.y"
{ Module::port_t*tmp = yyvsp[-1].mport;
		  tmp->name = "";
		  yyval.mport = tmp;
		;
    break;}
case 303:
#line 1940 "parse.y"
{ Module::port_t*tmp = yyvsp[-2].mport;
		  tmp->name = yyvsp[-5].text;
		  delete yyvsp[-5].text;
		  yyval.mport = tmp;
		;
    break;}
case 304:
#line 1948 "parse.y"
{ yyval.mport = yyvsp[0].mport; ;
    break;}
case 305:
#line 1949 "parse.y"
{ yyval.mport = 0; ;
    break;}
case 306:
#line 1966 "parse.y"
{ Module::port_t*ptmp;
		  ptmp = pform_module_port_reference(yyvsp[0].text, yylsp[0].text,
						     yylsp[0].first_line);
		  delete yyvsp[0].text;
		  yyval.mport = ptmp;
		;
    break;}
case 307:
#line 1974 "parse.y"
{ PEIdent*wtmp = new PEIdent(hname_t(yyvsp[-5].text));
		  wtmp->set_file(yylsp[-5].text);
		  wtmp->set_lineno(yylsp[-5].first_line);
		  if (!pform_expression_is_constant(yyvsp[-3].expr)) {
			yyerror(yylsp[-3], "error: msb expression of "
				"port part select must be constant.");
		  }
		  if (!pform_expression_is_constant(yyvsp[-1].expr)) {
			yyerror(yylsp[-1], "error: lsb expression of "
				"port part select must be constant.");
		  }
		  wtmp->msb_ = yyvsp[-3].expr;
		  wtmp->lsb_ = yyvsp[-1].expr;
		  Module::port_t*ptmp = new Module::port_t;
		  ptmp->name = "";
		  ptmp->expr = svector<PEIdent*>(1);
		  ptmp->expr[0] = wtmp;
		  delete yyvsp[-5].text;
		  yyval.mport = ptmp;
		;
    break;}
case 308:
#line 1996 "parse.y"
{ PEIdent*tmp = new PEIdent(hname_t(yyvsp[-3].text));
		  tmp->set_file(yylsp[-3].text);
		  tmp->set_lineno(yylsp[-3].first_line);
		  if (!pform_expression_is_constant(yyvsp[-1].expr)) {
			yyerror(yylsp[-1], "error: port bit select "
				"must be constant.");
		  }
		  tmp->msb_ = yyvsp[-1].expr;
		  Module::port_t*ptmp = new Module::port_t;
		  ptmp->name = "";
		  ptmp->expr = svector<PEIdent*>(1);
		  ptmp->expr[0] = tmp;
		  delete yyvsp[-3].text;
		  yyval.mport = ptmp;
		;
    break;}
case 309:
#line 2013 "parse.y"
{ yyerror(yylsp[-3], "error: invalid port bit select");
		  Module::port_t*ptmp = new Module::port_t;
		  PEIdent*wtmp = new PEIdent(hname_t(yyvsp[-3].text));
		  wtmp->set_file(yylsp[-3].text);
		  wtmp->set_lineno(yylsp[-3].first_line);
		  ptmp->name = yyvsp[-3].text;
		  ptmp->expr = svector<PEIdent*>(1);
		  ptmp->expr[0] = wtmp;
		  delete yyvsp[-3].text;
		  yyval.mport = ptmp;
		;
    break;}
case 310:
#line 2029 "parse.y"
{ yyval.mport = yyvsp[0].mport; ;
    break;}
case 311:
#line 2031 "parse.y"
{ Module::port_t*tmp = yyvsp[-2].mport;
		  tmp->expr = svector<PEIdent*>(tmp->expr, yyvsp[0].mport->expr);
		  delete yyvsp[0].mport;
		  yyval.mport = tmp;
		;
    break;}
case 312:
#line 2044 "parse.y"
{ named_pexpr_t*tmp = new named_pexpr_t;
		  tmp->name = yyvsp[-3].text;
		  tmp->parm = yyvsp[-1].expr;
		  delete yyvsp[-3].text;
		  yyval.named_pexpr = tmp;
		;
    break;}
case 313:
#line 2051 "parse.y"
{ yyerror(yylsp[-1], "error: invalid port connection expression.");
		  named_pexpr_t*tmp = new named_pexpr_t;
		  tmp->name = yyvsp[-3].text;
		  tmp->parm = 0;
		  delete yyvsp[-3].text;
		  yyval.named_pexpr = tmp;
		;
    break;}
case 314:
#line 2059 "parse.y"
{ named_pexpr_t*tmp = new named_pexpr_t;
		  tmp->name = yyvsp[-2].text;
		  tmp->parm = 0;
		  delete yyvsp[-2].text;
		  yyval.named_pexpr = tmp;
		;
    break;}
case 315:
#line 2069 "parse.y"
{ svector<named_pexpr_t*>*tmp;
		  tmp = new svector<named_pexpr_t*>(*yyvsp[-2].named_pexprs, yyvsp[0].named_pexpr);
		  delete yyvsp[-2].named_pexprs;
		  yyval.named_pexprs = tmp;
		;
    break;}
case 316:
#line 2075 "parse.y"
{ svector<named_pexpr_t*>*tmp = new svector<named_pexpr_t*>(1);
		  (*tmp)[0] = yyvsp[0].named_pexpr;
		  yyval.named_pexprs = tmp;
		;
    break;}
case 317:
#line 2082 "parse.y"
{ yyval.porttype = NetNet::PINPUT; ;
    break;}
case 318:
#line 2083 "parse.y"
{ yyval.porttype = NetNet::POUTPUT; ;
    break;}
case 319:
#line 2084 "parse.y"
{ yyval.porttype = NetNet::PINOUT; ;
    break;}
case 320:
#line 2089 "parse.y"
{ svector<PExpr*>*tmp = new svector<PExpr*> (2);
		  if (!pform_expression_is_constant(yyvsp[-3].expr))
			yyerror(yylsp[-3], "error: msb of range must be constant.");

		  (*tmp)[0] = yyvsp[-3].expr;
		  
		  if (!pform_expression_is_constant(yyvsp[-1].expr))
			yyerror(yylsp[-1], "error: lsb of range must be constant.");

		  (*tmp)[1] = yyvsp[-1].expr;
		  
		  yyval.exprs = tmp;
		;
    break;}
case 322:
#line 2106 "parse.y"
{ yyval.exprs = 0; ;
    break;}
case 323:
#line 2111 "parse.y"
{ yyval.range_type.range = yyvsp[0].exprs; yyval.range_type.ntype = NetNet::REG; ;
    break;}
case 324:
#line 2112 "parse.y"
{ yyval.range_type.range = 0;  yyval.range_type.ntype = NetNet::INTEGER; ;
    break;}
case 325:
#line 2113 "parse.y"
{ yyval.range_type.range = 0;  yyval.range_type.ntype = NetNet::IMPLICIT; ;
    break;}
case 326:
#line 2114 "parse.y"
{ yyval.range_type.range = 0;  yyval.range_type.ntype = NetNet::IMPLICIT; ;
    break;}
case 327:
#line 2115 "parse.y"
{ yyval.range_type.range = 0;  yyval.range_type.ntype = NetNet::IMPLICIT; ;
    break;}
case 328:
#line 2116 "parse.y"
{ yyval.range_type.range = 0;  yyval.range_type.ntype = NetNet::IMPLICIT; ;
    break;}
case 329:
#line 2126 "parse.y"
{ pform_makewire(yylsp[0], yyvsp[0].text, NetNet::REG,
				 NetNet::NOT_A_PORT, 0);
		  yyval.text = yyvsp[0].text;
		;
    break;}
case 330:
#line 2131 "parse.y"
{ pform_makewire(yylsp[-2], yyvsp[-2].text, NetNet::REG,
				 NetNet::NOT_A_PORT, 0);
		  if (! pform_expression_is_constant(yyvsp[0].expr))
			yyerror(yylsp[0], "error: register declaration assignment"
				" value must be a constant expression.");
		  pform_make_reginit(yylsp[-2], yyvsp[-2].text, yyvsp[0].expr);
		  yyval.text = yyvsp[-2].text;
		;
    break;}
case 331:
#line 2140 "parse.y"
{ pform_makewire(yylsp[-5], yyvsp[-5].text, NetNet::REG,
				 NetNet::NOT_A_PORT, 0);
		  if (! pform_expression_is_constant(yyvsp[-3].expr))
			yyerror(yylsp[-3], "error: msb of register range must be constant.");
		  if (! pform_expression_is_constant(yyvsp[-1].expr))
			yyerror(yylsp[-3], "error: lsb of register range must be constant.");
		  pform_set_reg_idx(yyvsp[-5].text, yyvsp[-3].expr, yyvsp[-1].expr);
		  yyval.text = yyvsp[-5].text;
		;
    break;}
case 332:
#line 2153 "parse.y"
{ list<char*>*tmp = new list<char*>;
		  tmp->push_back(yyvsp[0].text);
		  yyval.texts = tmp;
		;
    break;}
case 333:
#line 2158 "parse.y"
{ list<char*>*tmp = yyvsp[-2].texts;
		  tmp->push_back(yyvsp[0].text);
		  yyval.texts = tmp;
		;
    break;}
case 335:
#line 2167 "parse.y"
{
		;
    break;}
case 336:
#line 2170 "parse.y"
{
		;
    break;}
case 337:
#line 2173 "parse.y"
{
		;
    break;}
case 338:
#line 2176 "parse.y"
{
		;
    break;}
case 339:
#line 2180 "parse.y"
{ delete yyvsp[-3].expr;
		;
    break;}
case 340:
#line 2184 "parse.y"
{ delete yyvsp[-3].expr;
		;
    break;}
case 341:
#line 2188 "parse.y"
{ delete yyvsp[-3].expr;
		;
    break;}
case 342:
#line 2192 "parse.y"
{ delete yyvsp[-3].expr;
		;
    break;}
case 343:
#line 2196 "parse.y"
{ delete yyvsp[-5].expr;
		  delete yyvsp[-3].expr;
		;
    break;}
case 344:
#line 2201 "parse.y"
{ delete yyvsp[-5].expr;
		  delete yyvsp[-3].expr;
		;
    break;}
case 345:
#line 2206 "parse.y"
{ delete yyvsp[-5].expr;
		  delete yyvsp[-3].expr;
		;
    break;}
case 346:
#line 2210 "parse.y"
{ delete yyvsp[-2].expr;
		;
    break;}
case 347:
#line 2215 "parse.y"
{ ;
    break;}
case 348:
#line 2216 "parse.y"
{ ;
    break;}
case 366:
#line 2250 "parse.y"
{ yyerror(yylsp[-3], "Syntax error in delay value list.");
		  yyerrok;
		;
    break;}
case 367:
#line 2258 "parse.y"
{ pform_make_specify_path(yyvsp[-4].texts, yyvsp[-3].letter, false, yyvsp[-1].texts); ;
    break;}
case 368:
#line 2261 "parse.y"
{ pform_make_specify_path(yyvsp[-4].texts, yyvsp[-3].letter, true, yyvsp[-1].texts); ;
    break;}
case 369:
#line 2263 "parse.y"
{ yyerror(yylsp[-1], "Invalid simple path");
		  yyerrok;
		;
    break;}
case 370:
#line 2270 "parse.y"
{ list<char*>*tmp = new list<char*>;
		  tmp->push_back(yyvsp[0].text);
		  yyval.texts = tmp;
		;
    break;}
case 371:
#line 2275 "parse.y"
{ list<char*>*tmp = new list<char*>;
		  tmp->push_back(yyvsp[-3].text);
		  yyval.texts = tmp;
		;
    break;}
case 372:
#line 2280 "parse.y"
{ list<char*>*tmp = yyvsp[-2].texts;
		  tmp->push_back(yyvsp[0].text);
		  yyval.texts = tmp;
		;
    break;}
case 373:
#line 2285 "parse.y"
{ list<char*>*tmp = yyvsp[-5].texts;
		  tmp->push_back(yyvsp[-3].text);
		  yyval.texts = tmp;
		;
    break;}
case 374:
#line 2293 "parse.y"
{ PExpr*tmp = yyvsp[0].expr;
		  if (!pform_expression_is_constant(tmp)) {
			yyerror(yylsp[0], "error: specparam value "
			            "must be a constant expression.");
			delete tmp;
			tmp = 0;
		  } else {
			pform_set_specparam(yyvsp[-2].text, tmp);
		  }
		  delete yyvsp[-2].text;
		;
    break;}
case 375:
#line 2305 "parse.y"
{ delete yyvsp[-6].text;
		  delete yyvsp[-4].expr;
		  delete yyvsp[-2].expr;
		  delete yyvsp[0].expr;
		;
    break;}
case 376:
#line 2311 "parse.y"
{ delete yyvsp[-2].text;
		  delete yyvsp[0].expr;
		;
    break;}
case 377:
#line 2315 "parse.y"
{ delete yyvsp[-6].text;
		  delete yyvsp[-3].expr;
		  delete yyvsp[-1].expr;
		;
    break;}
case 380:
#line 2327 "parse.y"
{ yyval.letter = '+'; ;
    break;}
case 381:
#line 2328 "parse.y"
{ yyval.letter = '-'; ;
    break;}
case 382:
#line 2329 "parse.y"
{ yyval.letter = 0;   ;
    break;}
case 383:
#line 2334 "parse.y"
{ delete yyvsp[0].expr; ;
    break;}
case 384:
#line 2336 "parse.y"
{ delete yyvsp[0].expr; ;
    break;}
case 385:
#line 2338 "parse.y"
{ delete yyvsp[-2].expr;
		  delete yyvsp[0].expr;
		;
    break;}
case 386:
#line 2342 "parse.y"
{ delete yyvsp[-2].expr;
		  delete yyvsp[0].expr;
		;
    break;}
case 387:
#line 2346 "parse.y"
{ delete yyvsp[-2].expr;
		  delete yyvsp[0].expr;
		;
    break;}
case 388:
#line 2350 "parse.y"
{ delete yyvsp[0].expr; ;
    break;}
case 389:
#line 2355 "parse.y"
{  ;
    break;}
case 390:
#line 2357 "parse.y"
{  ;
    break;}
case 391:
#line 2361 "parse.y"
{  ;
    break;}
case 392:
#line 2363 "parse.y"
{ delete yyvsp[0].hier; ;
    break;}
case 393:
#line 2365 "parse.y"
{  ;
    break;}
case 394:
#line 2367 "parse.y"
{ delete yyvsp[0].hier;
		;
    break;}
case 395:
#line 2370 "parse.y"
{ delete yyvsp[0].text; ;
    break;}
case 396:
#line 2382 "parse.y"
{ PCAssign*tmp = new PCAssign(yyvsp[-3].expr, yyvsp[-1].expr);
		  tmp->set_file(yylsp[-4].text);
		  tmp->set_lineno(yylsp[-4].first_line);
		  yyval.statement = tmp;
		;
    break;}
case 397:
#line 2389 "parse.y"
{ PDeassign*tmp = new PDeassign(yyvsp[-1].expr);
		  tmp->set_file(yylsp[-2].text);
		  tmp->set_lineno(yylsp[-2].first_line);
		  yyval.statement = tmp;
		;
    break;}
case 398:
#line 2400 "parse.y"
{ PForce*tmp = new PForce(yyvsp[-3].expr, yyvsp[-1].expr);
		  tmp->set_file(yylsp[-4].text);
		  tmp->set_lineno(yylsp[-4].first_line);
		  yyval.statement = tmp;
		;
    break;}
case 399:
#line 2406 "parse.y"
{ PRelease*tmp = new PRelease(yyvsp[-1].expr);
		  tmp->set_file(yylsp[-2].text);
		  tmp->set_lineno(yylsp[-2].first_line);
		  yyval.statement = tmp;
		;
    break;}
case 400:
#line 2419 "parse.y"
{ PBlock*tmp = new PBlock(PBlock::BL_SEQ, *yyvsp[-1].statement_list);
		  tmp->set_file(yylsp[-2].text);
		  tmp->set_lineno(yylsp[-2].first_line);
		  delete yyvsp[-1].statement_list;
		  yyval.statement = tmp;
		;
    break;}
case 401:
#line 2426 "parse.y"
{ pform_push_scope(yyvsp[0].text); ;
    break;}
case 402:
#line 2429 "parse.y"
{ pform_pop_scope();
		  PBlock*tmp = new PBlock(yyvsp[-4].text, PBlock::BL_SEQ, *yyvsp[-1].statement_list);
		  tmp->set_file(yylsp[-6].text);
		  tmp->set_lineno(yylsp[-6].first_line);
		  delete yyvsp[-4].text;
		  delete yyvsp[-1].statement_list;
		  yyval.statement = tmp;
		;
    break;}
case 403:
#line 2438 "parse.y"
{ PBlock*tmp = new PBlock(PBlock::BL_SEQ);
		  tmp->set_file(yylsp[-1].text);
		  tmp->set_lineno(yylsp[-1].first_line);
		  yyval.statement = tmp;
		;
    break;}
case 404:
#line 2444 "parse.y"
{ PBlock*tmp = new PBlock(PBlock::BL_SEQ);
		  tmp->set_file(yylsp[-3].text);
		  tmp->set_lineno(yylsp[-3].first_line);
		  yyval.statement = tmp;
		;
    break;}
case 405:
#line 2450 "parse.y"
{ yyerrok; ;
    break;}
case 406:
#line 2458 "parse.y"
{ pform_push_scope(yyvsp[0].text); ;
    break;}
case 407:
#line 2461 "parse.y"
{ pform_pop_scope();
		  PBlock*tmp = new PBlock(yyvsp[-4].text, PBlock::BL_PAR, *yyvsp[-1].statement_list);
		  tmp->set_file(yylsp[-6].text);
		  tmp->set_lineno(yylsp[-6].first_line);
		  delete yyvsp[-4].text;
		  delete yyvsp[-1].statement_list;
		  yyval.statement = tmp;
		;
    break;}
case 408:
#line 2470 "parse.y"
{ PBlock*tmp = new PBlock(PBlock::BL_PAR);
		  tmp->set_file(yylsp[-1].text);
		  tmp->set_lineno(yylsp[-1].first_line);
		  yyval.statement = tmp;
		;
    break;}
case 409:
#line 2476 "parse.y"
{ PBlock*tmp = new PBlock(PBlock::BL_PAR);
		  tmp->set_file(yylsp[-3].text);
		  tmp->set_lineno(yylsp[-3].first_line);
		  delete yyvsp[-1].text;
		  yyval.statement = tmp;
		;
    break;}
case 410:
#line 2484 "parse.y"
{ PDisable*tmp = new PDisable(*yyvsp[-1].hier);
		  tmp->set_file(yylsp[-2].text);
		  tmp->set_lineno(yylsp[-2].first_line);
		  delete yyvsp[-1].hier;
		  yyval.statement = tmp;
		;
    break;}
case 411:
#line 2491 "parse.y"
{ PTrigger*tmp = new PTrigger(*yyvsp[-1].hier);
		  tmp->set_file(yylsp[-1].text);
		  tmp->set_lineno(yylsp[-1].first_line);
		  delete yyvsp[-1].hier;
		  yyval.statement = tmp;
		;
    break;}
case 412:
#line 2498 "parse.y"
{ PForever*tmp = new PForever(yyvsp[0].statement);
		  tmp->set_file(yylsp[-1].text);
		  tmp->set_lineno(yylsp[-1].first_line);
		  yyval.statement = tmp;
		;
    break;}
case 413:
#line 2504 "parse.y"
{ PBlock*tmp = new PBlock(PBlock::BL_PAR, *yyvsp[-1].statement_list);
		  tmp->set_file(yylsp[-2].text);
		  tmp->set_lineno(yylsp[-2].first_line);
		  delete yyvsp[-1].statement_list;
		  yyval.statement = tmp;
		;
    break;}
case 414:
#line 2511 "parse.y"
{ PRepeat*tmp = new PRepeat(yyvsp[-2].expr, yyvsp[0].statement);
		  tmp->set_file(yylsp[-4].text);
		  tmp->set_lineno(yylsp[-4].first_line);
		  yyval.statement = tmp;
		;
    break;}
case 415:
#line 2517 "parse.y"
{ PCase*tmp = new PCase(NetCase::EQ, yyvsp[-3].expr, yyvsp[-1].citems);
		  tmp->set_file(yylsp[-5].text);
		  tmp->set_lineno(yylsp[-5].first_line);
		  yyval.statement = tmp;
		;
    break;}
case 416:
#line 2523 "parse.y"
{ PCase*tmp = new PCase(NetCase::EQX, yyvsp[-3].expr, yyvsp[-1].citems);
		  tmp->set_file(yylsp[-5].text);
		  tmp->set_lineno(yylsp[-5].first_line);
		  yyval.statement = tmp;
		;
    break;}
case 417:
#line 2529 "parse.y"
{ PCase*tmp = new PCase(NetCase::EQZ, yyvsp[-3].expr, yyvsp[-1].citems);
		  tmp->set_file(yylsp[-5].text);
		  tmp->set_lineno(yylsp[-5].first_line);
		  yyval.statement = tmp;
		;
    break;}
case 418:
#line 2535 "parse.y"
{ yyerrok; ;
    break;}
case 419:
#line 2537 "parse.y"
{ yyerrok; ;
    break;}
case 420:
#line 2539 "parse.y"
{ yyerrok; ;
    break;}
case 421:
#line 2541 "parse.y"
{ PCondit*tmp = new PCondit(yyvsp[-2].expr, yyvsp[0].statement, 0);
		  tmp->set_file(yylsp[-4].text);
		  tmp->set_lineno(yylsp[-4].first_line);
		  yyval.statement = tmp;
		;
    break;}
case 422:
#line 2547 "parse.y"
{ PCondit*tmp = new PCondit(yyvsp[-4].expr, yyvsp[-2].statement, yyvsp[0].statement);
		  tmp->set_file(yylsp[-6].text);
		  tmp->set_lineno(yylsp[-6].first_line);
		  yyval.statement = tmp;
		;
    break;}
case 423:
#line 2553 "parse.y"
{ yyerror(yylsp[-4], "error: Malformed conditional expression.");
		  yyval.statement = yyvsp[0].statement;
		;
    break;}
case 424:
#line 2557 "parse.y"
{ yyerror(yylsp[-6], "error: Malformed conditional expression.");
		  yyval.statement = yyvsp[-2].statement;
		;
    break;}
case 425:
#line 2562 "parse.y"
{ PForStatement*tmp = new PForStatement(yyvsp[-10].expr, yyvsp[-8].expr, yyvsp[-6].expr, yyvsp[-4].expr, yyvsp[-2].expr, yyvsp[0].statement);
		  tmp->set_file(yylsp[-12].text);
		  tmp->set_lineno(yylsp[-12].first_line);
		  yyval.statement = tmp;
		;
    break;}
case 426:
#line 2569 "parse.y"
{ yyval.statement = 0;
		  yyerror(yylsp[-2], "error: Error in for loop step assignment.");
		;
    break;}
case 427:
#line 2574 "parse.y"
{ yyval.statement = 0;
		  yyerror(yylsp[-6], "error: Error in for loop condition expression.");
		;
    break;}
case 428:
#line 2578 "parse.y"
{ yyval.statement = 0;
		  yyerror(yylsp[-2], "error: Incomprehensible for loop.");
		;
    break;}
case 429:
#line 2582 "parse.y"
{ PWhile*tmp = new PWhile(yyvsp[-2].expr, yyvsp[0].statement);
		  tmp->set_file(yylsp[-4].text);
		  tmp->set_lineno(yylsp[-4].first_line);
		  yyval.statement = tmp;
		;
    break;}
case 430:
#line 2588 "parse.y"
{ yyval.statement = 0;
		  yyerror(yylsp[-2], "error: Error in while loop condition.");
		;
    break;}
case 431:
#line 2592 "parse.y"
{ PExpr*del = (*yyvsp[-1].exprs)[0];
		  assert(yyvsp[-1].exprs->count() == 1);
		  PDelayStatement*tmp = new PDelayStatement(del, yyvsp[0].statement);
		  tmp->set_file(yylsp[-1].text);
		  tmp->set_lineno(yylsp[-1].first_line);
		  yyval.statement = tmp;
		;
    break;}
case 432:
#line 2600 "parse.y"
{ PEventStatement*tmp = yyvsp[-1].event_statement;
		  if (tmp == 0) {
			yyerror(yylsp[-1], "error: Invalid event control.");
			yyval.statement = 0;
		  } else {
			tmp->set_statement(yyvsp[0].statement);
			yyval.statement = tmp;
		  }
		;
    break;}
case 433:
#line 2610 "parse.y"
{ PEventStatement*tmp = new PEventStatement;
		  tmp->set_file(yylsp[-2].text);
		  tmp->set_lineno(yylsp[-2].first_line);
		  tmp->set_statement(yyvsp[0].statement);
		  yyval.statement = tmp;
		;
    break;}
case 434:
#line 2617 "parse.y"
{ PEventStatement*tmp = new PEventStatement;
		  tmp->set_file(yylsp[-4].text);
		  tmp->set_lineno(yylsp[-4].first_line);
		  tmp->set_statement(yyvsp[0].statement);
		  yyval.statement = tmp;
		;
    break;}
case 435:
#line 2624 "parse.y"
{ PAssign*tmp = new PAssign(yyvsp[-3].expr,yyvsp[-1].expr);
		  tmp->set_file(yylsp[-3].text);
		  tmp->set_lineno(yylsp[-3].first_line);
		  yyval.statement = tmp;
		;
    break;}
case 436:
#line 2630 "parse.y"
{ PAssignNB*tmp = new PAssignNB(yyvsp[-3].expr,yyvsp[-1].expr);
		  tmp->set_file(yylsp[-3].text);
		  tmp->set_lineno(yylsp[-3].first_line);
		  yyval.statement = tmp;
		;
    break;}
case 437:
#line 2636 "parse.y"
{ assert(yyvsp[-2].exprs->count() == 1);
		  PExpr*del = (*yyvsp[-2].exprs)[0];
		  PAssign*tmp = new PAssign(yyvsp[-4].expr,del,yyvsp[-1].expr);
		  tmp->set_file(yylsp[-4].text);
		  tmp->set_lineno(yylsp[-4].first_line);
		  yyval.statement = tmp;
		;
    break;}
case 438:
#line 2644 "parse.y"
{ assert(yyvsp[-2].exprs->count() == 1);
		  PExpr*del = (*yyvsp[-2].exprs)[0];
		  PAssignNB*tmp = new PAssignNB(yyvsp[-4].expr,del,yyvsp[-1].expr);
		  tmp->set_file(yylsp[-4].text);
		  tmp->set_lineno(yylsp[-4].first_line);
		  yyval.statement = tmp;
		;
    break;}
case 439:
#line 2652 "parse.y"
{ PAssign*tmp = new PAssign(yyvsp[-4].expr,yyvsp[-2].event_statement,yyvsp[-1].expr);
		  tmp->set_file(yylsp[-4].text);
		  tmp->set_lineno(yylsp[-4].first_line);
		  yyval.statement = tmp;
		;
    break;}
case 440:
#line 2658 "parse.y"
{ PAssign*tmp = new PAssign(yyvsp[-8].expr,yyvsp[-2].event_statement,yyvsp[-1].expr);
		  tmp->set_file(yylsp[-8].text);
		  tmp->set_lineno(yylsp[-8].first_line);
		  yyerror(yylsp[-6], "sorry: repeat event control not supported.");
		  delete yyvsp[-4].expr;
		  yyval.statement = tmp;
		;
    break;}
case 441:
#line 2666 "parse.y"
{ yyerror(yylsp[-4], "sorry: Event controls not supported here.");
		  PAssignNB*tmp = new PAssignNB(yyvsp[-4].expr,yyvsp[-1].expr);
		  tmp->set_file(yylsp[-4].text);
		  tmp->set_lineno(yylsp[-4].first_line);
		  yyval.statement = tmp;
		;
    break;}
case 442:
#line 2673 "parse.y"
{ yyerror(yylsp[-8], "sorry: Event controls not supported here.");
		  delete yyvsp[-4].expr;
		  PAssignNB*tmp = new PAssignNB(yyvsp[-8].expr,yyvsp[-1].expr);
		  tmp->set_file(yylsp[-8].text);
		  tmp->set_lineno(yylsp[-8].first_line);
		  yyval.statement = tmp;
		;
    break;}
case 443:
#line 2681 "parse.y"
{ PEventStatement*tmp;
		  PEEvent*etmp = new PEEvent(PEEvent::POSITIVE, yyvsp[-2].expr);
		  tmp = new PEventStatement(etmp);
		  tmp->set_statement(yyvsp[0].statement);
		  yyval.statement = tmp;
		;
    break;}
case 444:
#line 2688 "parse.y"
{ PCallTask*tmp = new PCallTask(hname_t(yyvsp[-4].text), *yyvsp[-2].exprs);
		  tmp->set_file(yylsp[-4].text);
		  tmp->set_lineno(yylsp[-4].first_line);
		  delete yyvsp[-4].text;
		  delete yyvsp[-2].exprs;
		  yyval.statement = tmp;
		;
    break;}
case 445:
#line 2696 "parse.y"
{ svector<PExpr*>pt (0);
		  PCallTask*tmp = new PCallTask(hname_t(yyvsp[-1].text), pt);
		  tmp->set_file(yylsp[-1].text);
		  tmp->set_lineno(yylsp[-1].first_line);
		  delete yyvsp[-1].text;
		  yyval.statement = tmp;
		;
    break;}
case 446:
#line 2704 "parse.y"
{ PCallTask*tmp = new PCallTask(*yyvsp[-4].hier, *yyvsp[-2].exprs);
		  tmp->set_file(yylsp[-4].text);
		  tmp->set_lineno(yylsp[-4].first_line);
		  delete yyvsp[-4].hier;
		  delete yyvsp[-2].exprs;
		  yyval.statement = tmp;
		;
    break;}
case 447:
#line 2712 "parse.y"
{ svector<PExpr*>pt (0);
		  PCallTask*tmp = new PCallTask(*yyvsp[-1].hier, pt);
		  tmp->set_file(yylsp[-1].text);
		  tmp->set_lineno(yylsp[-1].first_line);
		  delete yyvsp[-1].hier;
		  yyval.statement = tmp;
		;
    break;}
case 448:
#line 2720 "parse.y"
{ yyerror(yylsp[-1], "error: malformed statement");
		  yyerrok;
		  yyval.statement = new PNoop;
		;
    break;}
case 449:
#line 2728 "parse.y"
{ svector<Statement*>*tmp = new svector<Statement*>(*yyvsp[-1].statement_list, yyvsp[0].statement);
		  delete yyvsp[-1].statement_list;
		  yyval.statement_list = tmp;
		;
    break;}
case 450:
#line 2733 "parse.y"
{ svector<Statement*>*tmp = new svector<Statement*>(1);
		  (*tmp)[0] = yyvsp[0].statement;
		  yyval.statement_list = tmp;
		;
    break;}
case 452:
#line 2741 "parse.y"
{ yyval.statement = 0; ;
    break;}
case 453:
#line 2747 "parse.y"
{ yyval.wires = new svector<PWire*>(0); ;
    break;}
case 454:
#line 2749 "parse.y"
{ svector<PWire*>*tmp
			= pform_make_task_ports(NetNet::PINPUT, false,
						yyvsp[-2].exprs, yyvsp[-1].texts,
						yylsp[-3].text, yylsp[-3].first_line);
		  yyval.wires = tmp;
		;
    break;}
case 455:
#line 2756 "parse.y"
{ svector<PWire*>*tmp
			= pform_make_task_ports(NetNet::POUTPUT, false,
						yyvsp[-2].exprs, yyvsp[-1].texts,
						yylsp[-3].text, yylsp[-3].first_line);
		  yyval.wires = tmp;
		;
    break;}
case 456:
#line 2763 "parse.y"
{ svector<PWire*>*tmp
			= pform_make_task_ports(NetNet::PINOUT, false,
						yyvsp[-2].exprs, yyvsp[-1].texts,
						yylsp[-3].text, yylsp[-3].first_line);
		  yyval.wires = tmp;
		;
    break;}
case 457:
#line 2773 "parse.y"
{ svector<PWire*>*tmp = new svector<PWire*>(*yyvsp[-1].wires, *yyvsp[0].wires);
		  delete yyvsp[-1].wires;
		  delete yyvsp[0].wires;
		  yyval.wires = tmp;
		;
    break;}
case 458:
#line 2779 "parse.y"
{ yyval.wires = yyvsp[0].wires; ;
    break;}
case 459:
#line 2784 "parse.y"
{ yyval.wires = yyvsp[0].wires; ;
    break;}
case 460:
#line 2786 "parse.y"
{ yyval.wires = 0; ;
    break;}
case 461:
#line 2790 "parse.y"
{ lex_start_table(); ;
    break;}
case 462:
#line 2792 "parse.y"
{ lex_end_table(); yyval.strings = yyvsp[-1].strings; ;
    break;}
case 465:
#line 2802 "parse.y"
{ char*tmp = new char[strlen(yyvsp[-3].text)+3];
		  strcpy(tmp, yyvsp[-3].text);
		  char*tp = tmp+strlen(tmp);
		  *tp++ = ':';
		  *tp++ = yyvsp[-1].letter;
		  *tp++ = 0;
		  delete[]yyvsp[-3].text;
		  yyval.text = tmp;
		;
    break;}
case 466:
#line 2815 "parse.y"
{ list<string>*tmp = new list<string>;
		  tmp->push_back(yyvsp[0].text);
		  delete yyvsp[0].text;
		  yyval.strings = tmp;
		;
    break;}
case 467:
#line 2821 "parse.y"
{ list<string>*tmp = yyvsp[-1].strings;
		  tmp->push_back(yyvsp[0].text);
		  delete yyvsp[0].text;
		  yyval.strings = tmp;
		;
    break;}
case 468:
#line 2830 "parse.y"
{ list<string>*tmp = new list<string>;
		  tmp->push_back(yyvsp[0].text);
		  delete yyvsp[0].text;
		  yyval.strings = tmp;
		;
    break;}
case 469:
#line 2836 "parse.y"
{ list<string>*tmp = yyvsp[-1].strings;
		  tmp->push_back(yyvsp[0].text);
		  delete yyvsp[0].text;
		  yyval.strings = tmp;
		;
    break;}
case 470:
#line 2845 "parse.y"
{ char*tmp = new char[strlen(yyvsp[-5].text)+5];
		  strcpy(tmp, yyvsp[-5].text);
		  char*tp = tmp+strlen(tmp);
		  *tp++ = ':';
		  *tp++ = yyvsp[-3].letter;
		  *tp++ = ':';
		  *tp++ = yyvsp[-1].letter;
		  *tp++ = 0;
		  yyval.text = tmp;
		;
    break;}
case 471:
#line 2859 "parse.y"
{ PExpr*etmp = new PENumber(yyvsp[-1].number);
		  PEIdent*itmp = new PEIdent(hname_t(yyvsp[-3].text));
		  PAssign*atmp = new PAssign(itmp, etmp);
		  atmp->set_file(yylsp[-3].text);
		  atmp->set_lineno(yylsp[-3].first_line);
		  delete yyvsp[-3].text;
		  yyval.statement = atmp;
		;
    break;}
case 472:
#line 2870 "parse.y"
{ yyval.statement = yyvsp[0].statement; ;
    break;}
case 473:
#line 2871 "parse.y"
{ yyval.statement = 0; ;
    break;}
case 474:
#line 2876 "parse.y"
{ char*tmp = new char[2];
		  tmp[0] = yyvsp[0].letter;
		  tmp[1] = 0;
		  yyval.text = tmp;
		;
    break;}
case 475:
#line 2882 "parse.y"
{ char*tmp = new char[strlen(yyvsp[-1].text)+2];
		  strcpy(tmp, yyvsp[-1].text);
		  char*tp = tmp+strlen(tmp);
		  *tp++ = yyvsp[0].letter;
		  *tp++ = 0;
		  delete[]yyvsp[-1].text;
		  yyval.text = tmp;
		;
    break;}
case 476:
#line 2893 "parse.y"
{ yyval.letter = '0'; ;
    break;}
case 477:
#line 2894 "parse.y"
{ yyval.letter = '1'; ;
    break;}
case 478:
#line 2895 "parse.y"
{ yyval.letter = 'x'; ;
    break;}
case 479:
#line 2896 "parse.y"
{ yyval.letter = '?'; ;
    break;}
case 480:
#line 2897 "parse.y"
{ yyval.letter = 'b'; ;
    break;}
case 481:
#line 2898 "parse.y"
{ yyval.letter = '*'; ;
    break;}
case 482:
#line 2899 "parse.y"
{ yyval.letter = '%'; ;
    break;}
case 483:
#line 2900 "parse.y"
{ yyval.letter = 'f'; ;
    break;}
case 484:
#line 2901 "parse.y"
{ yyval.letter = 'F'; ;
    break;}
case 485:
#line 2902 "parse.y"
{ yyval.letter = 'l'; ;
    break;}
case 486:
#line 2903 "parse.y"
{ yyval.letter = 'H'; ;
    break;}
case 487:
#line 2904 "parse.y"
{ yyval.letter = 'B'; ;
    break;}
case 488:
#line 2905 "parse.y"
{ yyval.letter = 'r'; ;
    break;}
case 489:
#line 2906 "parse.y"
{ yyval.letter = 'R'; ;
    break;}
case 490:
#line 2907 "parse.y"
{ yyval.letter = 'M'; ;
    break;}
case 491:
#line 2908 "parse.y"
{ yyval.letter = 'n'; ;
    break;}
case 492:
#line 2909 "parse.y"
{ yyval.letter = 'N'; ;
    break;}
case 493:
#line 2910 "parse.y"
{ yyval.letter = 'p'; ;
    break;}
case 494:
#line 2911 "parse.y"
{ yyval.letter = 'P'; ;
    break;}
case 495:
#line 2912 "parse.y"
{ yyval.letter = 'Q'; ;
    break;}
case 496:
#line 2913 "parse.y"
{ yyval.letter = 'q'; ;
    break;}
case 497:
#line 2914 "parse.y"
{ yyval.letter = '_'; ;
    break;}
case 498:
#line 2915 "parse.y"
{ yyval.letter = '+'; ;
    break;}
case 499:
#line 2919 "parse.y"
{ yyval.letter = '0'; ;
    break;}
case 500:
#line 2920 "parse.y"
{ yyval.letter = '1'; ;
    break;}
case 501:
#line 2921 "parse.y"
{ yyval.letter = 'x'; ;
    break;}
case 502:
#line 2922 "parse.y"
{ yyval.letter = '-'; ;
    break;}
case 503:
#line 2927 "parse.y"
{ yyval.wires = pform_make_udp_input_ports(yyvsp[-1].texts); ;
    break;}
case 504:
#line 2929 "parse.y"
{ PWire*pp = new PWire(yyvsp[-1].text, NetNet::IMPLICIT, NetNet::POUTPUT);
		  svector<PWire*>*tmp = new svector<PWire*>(1);
		  (*tmp)[0] = pp;
		  yyval.wires = tmp;
		;
    break;}
case 505:
#line 2935 "parse.y"
{ PWire*pp = new PWire(yyvsp[-1].text, NetNet::REG, NetNet::PIMPLICIT);
		  svector<PWire*>*tmp = new svector<PWire*>(1);
		  (*tmp)[0] = pp;
		  yyval.wires = tmp;
		;
    break;}
case 506:
#line 2944 "parse.y"
{ yyval.wires = yyvsp[0].wires; ;
    break;}
case 507:
#line 2946 "parse.y"
{ svector<PWire*>*tmp = new svector<PWire*>(*yyvsp[-1].wires, *yyvsp[0].wires);
		  delete yyvsp[-1].wires;
		  delete yyvsp[0].wires;
		  yyval.wires = tmp;
		;
    break;}
case 508:
#line 2955 "parse.y"
{ list<string>*tmp = new list<string>;
		  tmp->push_back(yyvsp[0].text);
		  delete yyvsp[0].text;
		  yyval.strings = tmp;
		;
    break;}
case 509:
#line 2961 "parse.y"
{ list<string>*tmp = yyvsp[-2].strings;
		  tmp->push_back(yyvsp[0].text);
		  delete yyvsp[0].text;
		  yyval.strings = tmp;
		;
    break;}
case 510:
#line 2974 "parse.y"
{ pform_make_udp(yyvsp[-8].text, yyvsp[-6].strings, yyvsp[-3].wires, yyvsp[-1].strings, yyvsp[-2].statement,
				 yylsp[-8].text, yylsp[-8].first_line);
		  delete[]yyvsp[-8].text;
		;
    break;}
}

#line 705 "d:\\\300\356\352\325\\My Projects\\vp/share/bison/bison.simple"


  yyvsp -= yylen;
  yyssp -= yylen;
#if YYLSP_NEEDED
  yylsp -= yylen;
#endif

#if YYDEBUG
  if (yydebug)
    {
      short *yyssp1 = yyss - 1;
      YYFPRINTF (stderr, "state stack now");
      while (yyssp1 != yyssp)
	YYFPRINTF (stderr, " %d", *++yyssp1);
      YYFPRINTF (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;
#if YYLSP_NEEDED
  *++yylsp = yyloc;
#endif

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;

#ifdef YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  YYSIZE_T yysize = 0;
	  char *yymsg;
	  int yyx, yycount;

	  yycount = 0;
	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  for (yyx = yyn < 0 ? -yyn : 0;
	       yyx < (int) (sizeof (yytname) / sizeof (char *)); yyx++)
	    if (yycheck[yyx + yyn] == yyx)
	      yysize += yystrlen (yytname[yyx]) + 15, yycount++;
	  yysize += yystrlen ("parse error, unexpected ") + 1;
	  yysize += yystrlen (yytname[YYTRANSLATE (yychar)]);
	  yymsg = (char *) YYSTACK_ALLOC (yysize);
	  if (yymsg != 0)
	    {
	      char *yyp = yystpcpy (yymsg, "parse error, unexpected ");
	      yyp = yystpcpy (yyp, yytname[YYTRANSLATE (yychar)]);

	      if (yycount < 5)
		{
		  yycount = 0;
		  for (yyx = yyn < 0 ? -yyn : 0;
		       yyx < (int) (sizeof (yytname) / sizeof (char *));
		       yyx++)
		    if (yycheck[yyx + yyn] == yyx)
		      {
			const char *yyq = ! yycount ? ", expecting " : " or ";
			yyp = yystpcpy (yyp, yyq);
			yyp = yystpcpy (yyp, yytname[yyx]);
			yycount++;
		      }
		}
	      yyerror (yymsg);
	      YYSTACK_FREE (yymsg);
	    }
	  else
	    yyerror ("parse error; also virtual memory exhausted");
	}
      else
#endif /* defined (YYERROR_VERBOSE) */
	yyerror ("parse error");
    }
  goto yyerrlab1;


/*--------------------------------------------------.
| yyerrlab1 -- error raised explicitly by an action |
`--------------------------------------------------*/
yyerrlab1:
  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      /* return failure if at end of input */
      if (yychar == YYEOF)
	YYABORT;
      YYDPRINTF ((stderr, "Discarding token %d (%s).\n",
		  yychar, yytname[yychar1]));
      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */

  yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto yyerrhandle;


/*-------------------------------------------------------------------.
| yyerrdefault -- current state does not do anything special for the |
| error token.                                                       |
`-------------------------------------------------------------------*/
yyerrdefault:
#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */

  /* If its default is to accept any token, ok.  Otherwise pop it.  */
  yyn = yydefact[yystate];
  if (yyn)
    goto yydefault;
#endif


/*---------------------------------------------------------------.
| yyerrpop -- pop the current state because it cannot handle the |
| error token                                                    |
`---------------------------------------------------------------*/
yyerrpop:
  if (yyssp == yyss)
    YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#if YYLSP_NEEDED
  yylsp--;
#endif

#if YYDEBUG
  if (yydebug)
    {
      short *yyssp1 = yyss - 1;
      YYFPRINTF (stderr, "Error: state stack now");
      while (yyssp1 != yyssp)
	YYFPRINTF (stderr, " %d", *++yyssp1);
      YYFPRINTF (stderr, "\n");
    }
#endif

/*--------------.
| yyerrhandle.  |
`--------------*/
yyerrhandle:
  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yyerrdefault;

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    goto yyerrdefault;

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrpop;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrpop;

  if (yyn == YYFINAL)
    YYACCEPT;

  YYDPRINTF ((stderr, "Shifting error token, "));

  *++yyvsp = yylval;
#if YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

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

/*---------------------------------------------.
| yyoverflowab -- parser overflow comes here.  |
`---------------------------------------------*/
yyoverflowlab:
  yyerror ("parser stack overflow");
  yyresult = 2;
  /* Fall through.  */

yyreturn:
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  return yyresult;
}
#line 2979 "parse.y"

