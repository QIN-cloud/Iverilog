#ifndef __verinum_H
#define __verinum_H

#include <string>

#include "config.h"
#include <iostream>

using namespace std;
/*
 * Numbers in Verilog are multibit strings, where each bit has 4
 * possible values: 0, 1, x or z. The verinum number is store in
 * little-endian format. This means that if the long value is 2b'10,
 * get(0) is 0 and get(1) is 1.
 */
class verinum {

    public:
      enum V { V0 = 0, V1, Vx, Vz };

      verinum();
      verinum(const string&str);
      verinum(const V*v, unsigned nbits, bool has_len =true);
      verinum(V, unsigned nbits =1, bool has_len =true);
      verinum(unsigned long val, unsigned bits);
      verinum(const verinum&);

	// Create a signed number, with an unspecified number of bits.
      explicit verinum(long val);

	// Copy only the specified number of bits from the
	// source. Also mark this number as has_len.
      verinum(const verinum&, unsigned bits);

      ~verinum();
      verinum& operator= (const verinum&);

	// Number of significant bits in this number.
      unsigned len() const { return nbits_; }

	// A number "has a length" if the length was specified fixed
	// in some way.
      bool has_len() const { return has_len_; }

      bool has_sign(bool flag) { has_sign_ = flag; return has_sign_; }
      bool has_sign() const { return has_sign_; }

	// A number is "defined" if there are no x or z bits in its value.
      bool is_defined() const;
      bool is_zero() const;

	// A number is "a string" if its value came directly from
	// an ASCII description instead of a number value.
      bool is_string() const { return string_flag_; }

	// Comparison for use in sorting algorithms.
      bool is_before(const verinum&that) const;

	// Individual bits can be accessed with the get and set
	// methods.
      V get(unsigned idx) const;
      V set(unsigned idx, V val);

      V operator[] (unsigned idx) const { return get(idx); }


      unsigned long as_ulong() const;
      signed long   as_long() const;
      string as_string() const;

    private:
      V* bits_;
      unsigned nbits_;
      bool has_len_;
      bool has_sign_;

	// These are some convenience flags that help us do a better
	// job of pretty-printing values.
      bool string_flag_;
};

/* Return a verinum that is minimal. That is, it has only the length
   needed to accurately represent the contained value, signed or not. */
extern verinum trim_vnum(const verinum&);

extern ostream& operator<< (ostream&, const verinum&);
extern ostream& operator<< (ostream&, verinum::V);

extern verinum::V operator | (verinum::V l, verinum::V r);
extern verinum::V operator & (verinum::V l, verinum::V r);
extern verinum::V operator ^ (verinum::V l, verinum::V r);
extern verinum::V bit_nor(verinum::V l, verinum::V r);
extern verinum::V bit_nand(verinum::V l, verinum::V r);
extern verinum::V bit_xnor(verinum::V l, verinum::V r);
extern verinum::V bit_not(verinum::V l);

extern verinum::V operator == (const verinum&left, const verinum&right);
extern verinum::V operator <= (const verinum&left, const verinum&right);
extern verinum::V operator <  (const verinum&left, const verinum&right);

/* These are arithmetic operators. These generally work to produce
   results that do not overflow. That means the result may expand or
   contract to hold the bits needed to hold the operation results
   accurately. It is up to the caller to truncate or pad if a specific
   width is expected. */
extern verinum operator + (const verinum&left, const verinum&right);
extern verinum operator - (const verinum&left, const verinum&right);
extern verinum operator * (const verinum&left, const verinum&right);
extern verinum operator / (const verinum&left, const verinum&right);
extern verinum operator % (const verinum&left, const verinum&right);
extern verinum operator - (verinum&left);

inline verinum::V operator > (const verinum&left, const verinum&right)
{ return right < left; }

inline verinum::V operator >= (const verinum&left, const verinum&right)
{ return right <= left; }

inline verinum::V operator != (const verinum&left, const verinum&right)
{ return (left == right)? verinum::V0 : verinum::V1; }

extern verinum v_not(const verinum&left); //bitwise not;
extern verinum bitwise_and(const verinum&left, const verinum&right);
extern verinum bitwise_or(const verinum&left, const verinum&right);
extern verinum bitwise_xor(const verinum&left, const verinum&right);
extern verinum bitwise_xnor(const verinum&left, const verinum&right);

extern verinum::V unary_and(const verinum&l);
extern verinum::V unary_nand(const verinum&l);
extern verinum::V unary_or(const verinum&l);
extern verinum::V unary_nor(const verinum&l);
extern verinum::V unary_xor(const verinum&l);
extern verinum::V unary_xnor(const verinum&l);

extern verinum::V land(const verinum&left, const verinum&right);//&&
extern verinum::V lor(const verinum&left, const verinum&right);//||
extern verinum::V lnot(const verinum&left);
extern verinum::V ceq(const verinum&left, const verinum&right);//===
extern verinum::V cneq(const verinum&left, const verinum&right);//!==

extern verinum lshift(const verinum& l, const verinum& r);
extern verinum rshift(const verinum& l, const verinum& r);

#endif
