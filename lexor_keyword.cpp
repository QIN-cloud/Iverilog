/* ANSI-C code produced by gperf version 2.7.2 */
/* Command-line: gperf -o -i 7 -C -k '1-3,$' -L ANSI-C -H keyword_hash -N check_identifier -t ./lexor_keyword.gperf  */
/* Command-line: gperf -o -i 1 -C -k 1-3,$ -L C -H keyword_hash -N check_identifier -tT lexor_keyword.gperf */

#include "config.h"
#include "parse_misc.h"
#include "parse.h"
#include <string.h>
#include "lexor_keyword.h"

struct lexor_keyword { const char*name; int tokenType; };

#define TOTAL_KEYWORDS 103
#define MIN_WORD_LENGTH 2
#define MAX_WORD_LENGTH 12
#define MIN_HASH_VALUE 33
#define MAX_HASH_VALUE 265
/* maximum key range = 233, duplicates = 0 */

#ifdef __GNUC__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif
static unsigned int
keyword_hash (register const char *str, register unsigned int len)
{
  static const unsigned short asso_values[] =
    {
      266, 266, 266, 266, 266, 266, 266, 266, 266, 266,
      266, 266, 266, 266, 266, 266, 266, 266, 266, 266,
      266, 266, 266, 266, 266, 266, 266, 266, 266, 266,
      266, 266, 266, 266, 266, 266, 266, 266, 266, 266,
      266, 266, 266, 266, 266, 266, 266, 266,  57,  77,
      266, 266, 266, 266, 266, 266, 266, 266, 266, 266,
      266, 266, 266, 266, 266, 266, 266, 266, 266, 266,
      266, 266, 266, 266, 266, 266, 266, 266, 266, 266,
      266, 266, 266, 266, 266, 266, 266, 266, 266, 266,
      266, 266, 266, 266, 266, 266, 266,  17,  17,  47,
       27,   7,  27,  77,   7,  92,   7, 102,  67, 104,
        7,  17,  37, 266,   7,   7,  12,  27,  37,  52,
      102,  27,   7, 266, 266, 266, 266, 266, 266, 266,
      266, 266, 266, 266, 266, 266, 266, 266, 266, 266,
      266, 266, 266, 266, 266, 266, 266, 266, 266, 266,
      266, 266, 266, 266, 266, 266, 266, 266, 266, 266,
      266, 266, 266, 266, 266, 266, 266, 266, 266, 266,
      266, 266, 266, 266, 266, 266, 266, 266, 266, 266,
      266, 266, 266, 266, 266, 266, 266, 266, 266, 266,
      266, 266, 266, 266, 266, 266, 266, 266, 266, 266,
      266, 266, 266, 266, 266, 266, 266, 266, 266, 266,
      266, 266, 266, 266, 266, 266, 266, 266, 266, 266,
      266, 266, 266, 266, 266, 266, 266, 266, 266, 266,
      266, 266, 266, 266, 266, 266, 266, 266, 266, 266,
      266, 266, 266, 266, 266, 266, 266, 266, 266, 266,
      266, 266, 266, 266, 266, 266
    };
  register int hval = len;

  switch (hval)
    {
      default:
      case 3:
        hval += asso_values[(unsigned char)str[2]];
      case 2:
        hval += asso_values[(unsigned char)str[1]];
      case 1:
        hval += asso_values[(unsigned char)str[0]];
        break;
    }
  return hval + asso_values[(unsigned char)str[len - 1]];
}

#ifdef __GNUC__
__inline
#endif
const struct lexor_keyword *
check_identifier (register const char *str, register unsigned int len)
{
  static const struct lexor_keyword wordlist[] =
    {
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
      {"or",		K_or},
      {""}, {""}, {""}, {""},
      {"rtran",		K_rtran},
      {""}, {""},
      {"nor",		K_nor},
      {""}, {""},
      {"assign",		K_assign},
      {""},
      {"realtime",	K_realtime},
      {"tran",		K_tran},
      {""}, {""}, {""},
      {"not",		K_not},
      {""}, {""}, {""},
      {"endcase",	K_endcase},
      {"endtable",	K_endtable},
      {"endmodule",	K_endmodule},
      {"table",		K_table},
      {"endfunction",	K_endfunction},
      {"endprimitive",	K_endprimitive},
      {"for",		K_for},
      {"nand",		K_nand},
      {"force",		K_force},
      {""},
      {"forever",	K_forever},
      {"deassign",	K_deassign},
      {""},
      {"event",		K_event},
      {"repeat",		K_repeat},
      {""},
      {"end",		K_end},
      {""}, {""},
      {"output",		K_output},
      {"posedge",	K_posedge},
      {"function",	K_function},
      {"parameter",	K_parameter},
      {"endspecify",	K_endspecify},
      {""},
      {"default",	K_default},
      {"and",		K_and},
      {"case",		K_case},
      {"casez",		K_casez},
      {""},
      {"specify",	K_specify},
      {"wor",		K_wor},
      {""}, {""}, {""},
      {"strong0",	K_strong0},
      {"rtranif0",	K_rtranif0},
      {"else",		K_else},
      {""}, {""},
      {"release",	K_release},
      {""}, {""}, {""},
      {"notif0",		K_notif0},
      {"tranif0",	K_tranif0},
      {"buf",		K_buf},
      {"real",		K_real},
      {"large",		K_large},
      {""},
      {"negedge",	K_negedge},
      {"scalared",	K_scalared},
      {"wand",		K_wand},
      {""}, {""},
      {"strong1",	K_strong1},
      {"rtranif1",	K_rtranif1},
      {""},
      {"begin",		K_begin},
      {""}, {""}, {""}, {""}, {""},
      {"notif1",		K_notif1},
      {"tranif1",	K_tranif1},
      {""},
      {"edge",		K_edge},
      {"trior",		K_trior},
      {""},
      {"integer",	K_integer},
      {"vectored",	K_vectored},
      {"join",		K_join},
      {""}, {""},
      {"rnmos",		K_rnmos},
      {""}, {""},
      {"inout",		K_inout},
      {"bufif0",		K_bufif0},
      {"supply0",	K_supply0},
      {"xor",		K_xor},
      {"xnor",		K_xnor},
      {"weak0",		K_weak0},
      {"nmos",		K_nmos},
      {"disable",	K_disable},
      {""},
      {"task",		K_task},
      {""},
      {"triand",		K_triand},
      {""},
      {"pulldown",	K_pulldown},
      {""},
      {"if",		K_if},
      {"always",		K_always},
      {"endtask",	K_endtask},
      {""},
      {"primitive",	K_primitive},
      {"input",		K_input},
      {"bufif1",		K_bufif1},
      {"supply1",	K_supply1},
      {""},
      {"fork",		K_fork},
      {"weak1",		K_weak1},
      {""},
      {"rpmos",		K_rpmos},
      {"module",		K_module},
      {"wire",		K_wire},
      {"while",		K_while},
      {"specparam",	K_specparam},
      {""}, {""}, {""}, {""},
      {"pmos",		K_pmos},
      {"rcmos",		K_rcmos},
      {"reg",		K_reg},
      {"tri0",		K_tri0},
      {"defparam",	K_defparam},
      {"pullup",		K_pullup},
      {""}, {""},
      {"wait",		K_wait},
      {"casex",		K_casex},
      {"cmos",		K_cmos},
      {""}, {""}, {""}, {""}, {""}, {""},
      {"macromodule",	K_macromodule},
      {""}, {""}, {""}, {""}, {""},
      {"tri1",		K_tri1},
      {"pull0",		K_pull0},
      {"trireg",		K_trireg},
      {""}, {""}, {""}, {""}, {""},
      {"small",		K_small},
      {""}, {""}, {""}, {""}, {""},
      {"tri",		K_tri},
      {""}, {""},
      {"signed",		K_signed},
      {""}, {""}, {""},
      {"pull1",		K_pull1},
      {""}, {""}, {""}, {""}, {""},
      {"time",		K_time},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
      {"highz0",		K_highz0},
      {""}, {""}, {""}, {""}, {""},
      {"localparam",	K_localparam},
      {""}, {""},
      {"medium",		K_medium},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
      {"highz1",		K_highz1},
      {""}, {""}, {""}, {""}, {""},
      {"initial",	K_initial}
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register int key = keyword_hash (str, len);

      if (key <= MAX_HASH_VALUE && key >= 0)
        {
          register const char *s = wordlist[key].name;

          if (*str == *s && !strcmp (str + 1, s + 1))
            return &wordlist[key];
        }
    }
  return 0;
}

int lexor_keyword_code(const char*str, unsigned nstr)
{
      const struct lexor_keyword*rc = check_identifier(str, nstr);
      if (rc == 0)
	  return IDENTIFIER;
      else
	  return rc->tokenType;
}
