#ifndef BISON_PARSE_HPP
# define BISON_PARSE_HPP

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


extern YYSTYPE VLlval;

#endif /* not BISON_PARSE_HPP */
