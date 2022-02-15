#ifndef __parse_misc_H
#define __parse_misc_H

#include <list>
#include "pform.h"

/*
 * The vlltype supports the passing of detailed source file location
 * information between the lexical analyzer and the parser. Defining
 * YYLTYPE compels the lexor to use this type and not something other.
 */
struct vlltype {
      unsigned first_line;
      unsigned first_column;
      unsigned last_line;
      unsigned last_column;
      const char*text;
};
# define YYLTYPE vlltype

  /* This for compatibility with new and older bison versions. */
#ifndef yylloc
# define yylloc VLlloc
#endif
extern YYLTYPE yylloc;

/*
 * Interface into the lexical analyzer. ...
 */
extern int  VLlex();
extern void VLerror(const char*msg);
extern void VLerror(const YYLTYPE&loc, const char*msg);
#define yywarn VLwarn
extern void VLwarn(const YYLTYPE&loc, const char*msg);

extern unsigned error_count, warn_count;

#endif
