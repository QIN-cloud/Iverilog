/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

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

#ifndef YY_VL_PARSE_H_INCLUDED
# define YY_VL_PARSE_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int VLdebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    IDENTIFIER = 258,
    SYSTEM_IDENTIFIER = 259,
    STRING = 260,
    TIME_LITERAL = 261,
    TYPE_IDENTIFIER = 262,
    PACKAGE_IDENTIFIER = 263,
    DISCIPLINE_IDENTIFIER = 264,
    PATHPULSE_IDENTIFIER = 265,
    BASED_NUMBER = 266,
    DEC_NUMBER = 267,
    UNBASED_NUMBER = 268,
    REALTIME = 269,
    K_PLUS_EQ = 270,
    K_MINUS_EQ = 271,
    K_INCR = 272,
    K_DECR = 273,
    K_LE = 274,
    K_GE = 275,
    K_EG = 276,
    K_EQ = 277,
    K_NE = 278,
    K_CEQ = 279,
    K_CNE = 280,
    K_WEQ = 281,
    K_WNE = 282,
    K_LP = 283,
    K_LS = 284,
    K_RS = 285,
    K_RSS = 286,
    K_SG = 287,
    K_CONTRIBUTE = 288,
    K_PO_POS = 289,
    K_PO_NEG = 290,
    K_POW = 291,
    K_PSTAR = 292,
    K_STARP = 293,
    K_DOTSTAR = 294,
    K_LOR = 295,
    K_LAND = 296,
    K_NAND = 297,
    K_NOR = 298,
    K_NXOR = 299,
    K_TRIGGER = 300,
    K_NB_TRIGGER = 301,
    K_LEQUIV = 302,
    K_SCOPE_RES = 303,
    K_edge_descriptor = 304,
    K_always = 305,
    K_and = 306,
    K_assign = 307,
    K_begin = 308,
    K_buf = 309,
    K_bufif0 = 310,
    K_bufif1 = 311,
    K_case = 312,
    K_casex = 313,
    K_casez = 314,
    K_cmos = 315,
    K_deassign = 316,
    K_default = 317,
    K_defparam = 318,
    K_disable = 319,
    K_edge = 320,
    K_else = 321,
    K_end = 322,
    K_endcase = 323,
    K_endfunction = 324,
    K_endmodule = 325,
    K_endprimitive = 326,
    K_endspecify = 327,
    K_endtable = 328,
    K_endtask = 329,
    K_event = 330,
    K_for = 331,
    K_force = 332,
    K_forever = 333,
    K_fork = 334,
    K_function = 335,
    K_highz0 = 336,
    K_highz1 = 337,
    K_if = 338,
    K_ifnone = 339,
    K_initial = 340,
    K_inout = 341,
    K_input = 342,
    K_integer = 343,
    K_join = 344,
    K_large = 345,
    K_macromodule = 346,
    K_medium = 347,
    K_module = 348,
    K_nand = 349,
    K_negedge = 350,
    K_nmos = 351,
    K_nor = 352,
    K_not = 353,
    K_notif0 = 354,
    K_notif1 = 355,
    K_or = 356,
    K_output = 357,
    K_parameter = 358,
    K_pmos = 359,
    K_posedge = 360,
    K_primitive = 361,
    K_pull0 = 362,
    K_pull1 = 363,
    K_pulldown = 364,
    K_pullup = 365,
    K_rcmos = 366,
    K_real = 367,
    K_realtime = 368,
    K_reg = 369,
    K_release = 370,
    K_repeat = 371,
    K_rnmos = 372,
    K_rpmos = 373,
    K_rtran = 374,
    K_rtranif0 = 375,
    K_rtranif1 = 376,
    K_scalared = 377,
    K_small = 378,
    K_specify = 379,
    K_specparam = 380,
    K_strong0 = 381,
    K_strong1 = 382,
    K_supply0 = 383,
    K_supply1 = 384,
    K_table = 385,
    K_task = 386,
    K_time = 387,
    K_tran = 388,
    K_tranif0 = 389,
    K_tranif1 = 390,
    K_tri = 391,
    K_tri0 = 392,
    K_tri1 = 393,
    K_triand = 394,
    K_trior = 395,
    K_trireg = 396,
    K_vectored = 397,
    K_wait = 398,
    K_wand = 399,
    K_weak0 = 400,
    K_weak1 = 401,
    K_while = 402,
    K_wire = 403,
    K_wor = 404,
    K_xnor = 405,
    K_xor = 406,
    K_Shold = 407,
    K_Snochange = 408,
    K_Speriod = 409,
    K_Srecovery = 410,
    K_Ssetup = 411,
    K_Ssetuphold = 412,
    K_Sskew = 413,
    K_Swidth = 414,
    KK_attribute = 415,
    K_bool = 416,
    K_logic = 417,
    K_automatic = 418,
    K_endgenerate = 419,
    K_generate = 420,
    K_genvar = 421,
    K_localparam = 422,
    K_noshowcancelled = 423,
    K_pulsestyle_onevent = 424,
    K_pulsestyle_ondetect = 425,
    K_showcancelled = 426,
    K_signed = 427,
    K_unsigned = 428,
    K_Sfullskew = 429,
    K_Srecrem = 430,
    K_Sremoval = 431,
    K_Stimeskew = 432,
    K_cell = 433,
    K_config = 434,
    K_design = 435,
    K_endconfig = 436,
    K_incdir = 437,
    K_include = 438,
    K_instance = 439,
    K_liblist = 440,
    K_library = 441,
    K_use = 442,
    K_wone = 443,
    K_uwire = 444,
    K_alias = 445,
    K_always_comb = 446,
    K_always_ff = 447,
    K_always_latch = 448,
    K_assert = 449,
    K_assume = 450,
    K_before = 451,
    K_bind = 452,
    K_bins = 453,
    K_binsof = 454,
    K_bit = 455,
    K_break = 456,
    K_byte = 457,
    K_chandle = 458,
    K_class = 459,
    K_clocking = 460,
    K_const = 461,
    K_constraint = 462,
    K_context = 463,
    K_continue = 464,
    K_cover = 465,
    K_covergroup = 466,
    K_coverpoint = 467,
    K_cross = 468,
    K_dist = 469,
    K_do = 470,
    K_endclass = 471,
    K_endclocking = 472,
    K_endgroup = 473,
    K_endinterface = 474,
    K_endpackage = 475,
    K_endprogram = 476,
    K_endproperty = 477,
    K_endsequence = 478,
    K_enum = 479,
    K_expect = 480,
    K_export = 481,
    K_extends = 482,
    K_extern = 483,
    K_final = 484,
    K_first_match = 485,
    K_foreach = 486,
    K_forkjoin = 487,
    K_iff = 488,
    K_ignore_bins = 489,
    K_illegal_bins = 490,
    K_import = 491,
    K_inside = 492,
    K_int = 493,
    K_interface = 494,
    K_intersect = 495,
    K_join_any = 496,
    K_join_none = 497,
    K_local = 498,
    K_longint = 499,
    K_matches = 500,
    K_modport = 501,
    K_new = 502,
    K_null = 503,
    K_package = 504,
    K_packed = 505,
    K_priority = 506,
    K_program = 507,
    K_property = 508,
    K_protected = 509,
    K_pure = 510,
    K_rand = 511,
    K_randc = 512,
    K_randcase = 513,
    K_randsequence = 514,
    K_ref = 515,
    K_return = 516,
    K_sequence = 517,
    K_shortint = 518,
    K_shortreal = 519,
    K_solve = 520,
    K_static = 521,
    K_string = 522,
    K_struct = 523,
    K_super = 524,
    K_tagged = 525,
    K_this = 526,
    K_throughout = 527,
    K_timeprecision = 528,
    K_timeunit = 529,
    K_type = 530,
    K_typedef = 531,
    K_union = 532,
    K_unique = 533,
    K_var = 534,
    K_virtual = 535,
    K_void = 536,
    K_wait_order = 537,
    K_wildcard = 538,
    K_with = 539,
    K_within = 540,
    K_accept_on = 541,
    K_checker = 542,
    K_endchecker = 543,
    K_eventually = 544,
    K_global = 545,
    K_implies = 546,
    K_let = 547,
    K_nexttime = 548,
    K_reject_on = 549,
    K_restrict = 550,
    K_s_always = 551,
    K_s_eventually = 552,
    K_s_nexttime = 553,
    K_s_until = 554,
    K_s_until_with = 555,
    K_strong = 556,
    K_sync_accept_on = 557,
    K_sync_reject_on = 558,
    K_unique0 = 559,
    K_until = 560,
    K_until_with = 561,
    K_untyped = 562,
    K_weak = 563,
    K_implements = 564,
    K_interconnect = 565,
    K_nettype = 566,
    K_soft = 567,
    K_above = 568,
    K_abs = 569,
    K_absdelay = 570,
    K_abstol = 571,
    K_access = 572,
    K_acos = 573,
    K_acosh = 574,
    K_ac_stim = 575,
    K_aliasparam = 576,
    K_analog = 577,
    K_analysis = 578,
    K_asin = 579,
    K_asinh = 580,
    K_atan = 581,
    K_atan2 = 582,
    K_atanh = 583,
    K_branch = 584,
    K_ceil = 585,
    K_connect = 586,
    K_connectmodule = 587,
    K_connectrules = 588,
    K_continuous = 589,
    K_cos = 590,
    K_cosh = 591,
    K_ddt = 592,
    K_ddt_nature = 593,
    K_ddx = 594,
    K_discipline = 595,
    K_discrete = 596,
    K_domain = 597,
    K_driver_update = 598,
    K_endconnectrules = 599,
    K_enddiscipline = 600,
    K_endnature = 601,
    K_endparamset = 602,
    K_exclude = 603,
    K_exp = 604,
    K_final_step = 605,
    K_flicker_noise = 606,
    K_floor = 607,
    K_flow = 608,
    K_from = 609,
    K_ground = 610,
    K_hypot = 611,
    K_idt = 612,
    K_idtmod = 613,
    K_idt_nature = 614,
    K_inf = 615,
    K_initial_step = 616,
    K_laplace_nd = 617,
    K_laplace_np = 618,
    K_laplace_zd = 619,
    K_laplace_zp = 620,
    K_last_crossing = 621,
    K_limexp = 622,
    K_ln = 623,
    K_log = 624,
    K_max = 625,
    K_merged = 626,
    K_min = 627,
    K_nature = 628,
    K_net_resolution = 629,
    K_noise_table = 630,
    K_paramset = 631,
    K_potential = 632,
    K_pow = 633,
    K_resolveto = 634,
    K_sin = 635,
    K_sinh = 636,
    K_slew = 637,
    K_split = 638,
    K_sqrt = 639,
    K_tan = 640,
    K_tanh = 641,
    K_timer = 642,
    K_transition = 643,
    K_units = 644,
    K_white_noise = 645,
    K_wreal = 646,
    K_zi_nd = 647,
    K_zi_np = 648,
    K_zi_zd = 649,
    K_zi_zp = 650,
    K_TAND = 651,
    K_MUL_EQ = 652,
    K_DIV_EQ = 653,
    K_MOD_EQ = 654,
    K_AND_EQ = 655,
    K_OR_EQ = 656,
    K_XOR_EQ = 657,
    K_LS_EQ = 658,
    K_RS_EQ = 659,
    K_RSS_EQ = 660,
    UNARY_PREC = 661,
    less_than_K_else = 662,
    no_timeunits_declaration = 663,
    one_timeunits_declaration = 664
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 362 "parse.y" /* yacc.c:1909  */

      bool flag;

      char letter;
      int  int_val;

	/* text items are C strings allocated by the lexor using
	   strdup. They can be put into lists with the texts type. */
      char*text;
      list<perm_string>*perm_strings;

      list<pform_port_t>*port_list;

      vector<pform_tf_port_t>* tf_ports;

      pform_name_t*pform_name;

      ivl_discipline_t discipline;

      hname_t*hier;

      list<string>*strings;

      struct str_pair_t drive;

      PCase::Item*citem;
      svector<PCase::Item*>*citems;

      lgate*gate;
      svector<lgate>*gates;

      Module::port_t *mport;
      LexicalScope::range_t* value_range;
      vector<Module::port_t*>*mports;

      list<PLet::let_port_t*>*let_port_lst;
      PLet::let_port_t*let_port_itm;

      named_number_t* named_number;
      list<named_number_t>* named_numbers;

      named_pexpr_t*named_pexpr;
      list<named_pexpr_t>*named_pexprs;
      struct parmvalue_t*parmvalue;
      list<pform_range_t>*ranges;

      PExpr*expr;
      list<PExpr*>*exprs;

      svector<PEEvent*>*event_expr;

      ivl_case_quality_t case_quality;
      NetNet::Type nettype;
      PGBuiltin::Type gatetype;
      NetNet::PortType porttype;
      ivl_variable_type_t vartype;
      PBlock::BL_TYPE join_keyword;

      PWire*wire;
      vector<PWire*>*wires;

      PEventStatement*event_statement;
      Statement*statement;
      vector<Statement*>*statement_list;

      net_decl_assign_t*net_decl_assign;
      enum_type_t*enum_type;

      decl_assignment_t*decl_assignment;
      list<decl_assignment_t*>*decl_assignments;

      struct_member_t*struct_member;
      list<struct_member_t*>*struct_members;
      struct_type_t*struct_type;

      data_type_t*data_type;
      class_type_t*class_type;
      real_type_t::type_t real_type;
      property_qualifier_t property_qualifier;
      PPackage*package;

      struct {
	    char*text;
	    data_type_t*type;
      } type_identifier;

      struct {
	    data_type_t*type;
	    list<PExpr*>*exprs;
      } class_declaration_extends;

      struct {
	    char*text;
	    PExpr*expr;
      } genvar_iter;

      verinum* number;

      verireal* realtime;

      PSpecPath* specpath;
      list<index_component_t> *dimensions;

      LexicalScope::lifetime_t lifetime;

#line 570 "parse.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


extern YYSTYPE VLlval;
extern YYLTYPE VLlloc;
int VLparse (void);

#endif /* !YY_VL_PARSE_H_INCLUDED  */
