#include "config.h"

#include  "parse_misc.h"
#include  <iostream>

extern const char*vl_file;
unsigned error_count = 0;
unsigned warn_count = 0;

void VLerror(const char*msg)
{
      error_count += 1;
      cerr << yylloc.text << ":" << yylloc.first_line << ": " << msg << endl;
}

void VLerror(const YYLTYPE&loc, const char*msg)
{
      error_count += 1;
      if (loc.text)
	    cerr << loc.text << ":";

      cerr << loc.first_line << ": " << msg << endl;
}

void yywarn(const YYLTYPE&loc, const char*msg)
{
      warn_count += 1;
      if (loc.text)
	    cerr << loc.text << ":";

      cerr << loc.first_line << ": warning: " << msg << endl;
}

int VLwrap()
{
      return -1;
}
