#include "config.h"

#include "verireal.h"
#include "verinum.h"
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <assert.h>

verireal::verireal()
{
      value_ = 0.0;
}

verireal::verireal(const char*txt)
{
      char*tmp = new char[strlen(txt)+1];
      char*cp = tmp;
      for (unsigned idx = 0 ;  txt[idx] ;  idx += 1) {
	    if (txt[idx] == '_')
		  continue;

	    *cp++ = txt[idx];
      }
      cp[0] = 0;

      value_ = strtod(tmp, 0);
      delete[]tmp;
}

verireal::verireal(long val)
{
      value_ = (double)val;
}

verireal::~verireal()
{
}

long verireal::as_long(int shift) const
{
      double out = value_ * pow(10.0,shift);
      double outf;

      if (out >= 0.0) {
	    outf = floor(out);
	    if (out >= (outf + 0.5))
		  outf += 1.0;
      } else {
	    outf = ceil(out);
	    if (out <= (outf - 0.5))
		  outf -= 1.0;
      }
      return (long) outf;
}

double verireal::as_double() const
{
      return value_;
}

verireal operator* (const verireal&l, const verireal&r)
{
      verireal res;
      res.value_ = l.value_ * r.value_;
      return res;
}

verireal operator/ (const verireal&l, const verireal&r)
{
      verireal res;
      res.value_ = l.value_ / r.value_;
      return res;
}

verireal operator/ (const verireal&l, const verinum&r)
{
      verireal res;
      res.value_ = l.value_ / (double)r.as_long();
      return res;
}

verireal operator% (const verireal&l, const verireal&r)
{
      verireal res;
      assert(0);
      return res;
}

verireal operator% (const verireal&l, const verinum&r)
{
      verireal res;
      assert(0);
      return res;
}

ostream& operator<< (ostream&out, const verireal&v)
{
      out << v.as_double();
      return out;
}