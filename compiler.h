#ifndef __compiler_H
#define __compiler_H

#include <list>
#include  <map>
#include  <string>
#include "StringHeap.h"

/*
 * This defines constants and defaults for the compiler in general.
 */


/* The INTEGER_WIDTH is the default width of integer variables, and
   the presumed width of unsigned literal numbers. */
#ifndef INTEGER_WIDTH
# define INTEGER_WIDTH 32
#endif

/* The TIME_WIDTH is the width of time variables. */
#ifndef TIME_WIDTH
# define TIME_WIDTH 64
#endif

/*
 * When doing dynamic linking, we need a uniform way to identify the
 * symbol. Some compilers put leading _, some trailing _. The
 * configure script figures out which is the local convention and
 * defines NEED_LU and NEED_TU as required.
 */
#ifdef NEED_LU
#define LU "_"
#else
#define LU ""
#endif

#ifdef NEED_TU
#define TU "_"
#else
#define TU ""
#endif

extern StringHeapLex lex_strings;

/* This is true if verbose output is requested. */
extern bool verbose_flag;

/* Warn about legal but questionable module port bindings. */
extern bool warn_portbinding;
extern bool error_implicit;
extern bool warn_implicit;

extern char*ivlpp_string;

extern std::list<const char*>library_suff;
extern std::map<std::string,unsigned> missing_modules;

extern int yydebug;

#endif
