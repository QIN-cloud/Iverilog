#ifndef __verireal_H
#define __verireal_H

#include "config.h"
#include <iostream>

class verinum;

/*
 * This class holds a floating point decimal number. The number is
 * stored as an integer mantissa and a power of 10. The mantissa is an
 * integer so that decimal numbers in the source (which are decimal)
 * can be stored exactly.
 */

class verireal {

	  friend std::ostream& operator<< (std::ostream&, const verireal&);
      friend verireal operator* (const verireal&, const verireal&);
      friend verireal operator/ (const verireal&, const verireal&);
      friend verireal operator/ (const verireal&, const verinum&);
      friend verireal operator% (const verireal&, const verireal&);
      friend verireal operator% (const verireal&, const verinum&);

    public:
      explicit verireal();
      explicit verireal(const char*text);
      explicit verireal(long val);
      ~verireal();

	/* Return the value of the floating point number as an
	   integer, rounded as needed. The shift is the power of 10 to
	   multiply the value before calculating the result. So for
	   example if the value is 2.5 and shift == 1, the result
	   is 25. */
      long as_long(int shift =0) const;

      double as_double() const;

    private:
      double value_;
};

extern std::ostream& operator<< (std::ostream&, const verireal&);
extern verireal operator* (const verireal&, const verireal&);
extern verireal operator/ (const verireal&, const verireal&);
extern verireal operator/ (const verireal&, const verinum&);
extern verireal operator% (const verireal&, const verireal&);
extern verireal operator% (const verireal&, const verinum&);

#endif
