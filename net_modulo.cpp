#include "config.h"

#include <typeinfo>
#include <iostream>
#include <iomanip>
#include <cassert>

#include "netlist.h"
#include "compiler.h"


NetModulo::NetModulo(NetScope*s, const string&n, unsigned wr,
		     unsigned wa, unsigned wb)
: NetNode(s, lex_strings.add(n.c_str()), wr+wa+wb),
  width_r_(wr), width_a_(wa), width_b_(wb)
{
      unsigned p = 0;
      for (unsigned idx = 0 ;  idx < width_r_ ;  idx += 1, p += 1) {
	    pin(p).set_dir(Link::OUTPUT);
	    pin(p).set_name("Result", idx);
      }
      for (idx = 0 ;  idx < width_a_ ;  idx += 1, p += 1) {
	    pin(p).set_dir(Link::INPUT);
	    pin(p).set_name("DataA", idx);
      }
      for (idx = 0 ;  idx < width_b_ ;  idx += 1, p += 1) {
	    pin(p).set_dir(Link::INPUT);
	    pin(p).set_name("DataB", idx);
      }
}

NetModulo::~NetModulo()
{
}

unsigned NetModulo::width_r() const
{
      return width_r_;
}

unsigned NetModulo::width_a() const
{
      return width_a_;
}

unsigned NetModulo::width_b() const
{
      return width_b_;
}

Link& NetModulo::pin_Result(unsigned idx)
{
      assert(idx < width_r_);
      return pin(idx);
}

const Link& NetModulo::pin_Result(unsigned idx) const
{
      assert(idx < width_r_);
      return pin(idx);
}

Link& NetModulo::pin_DataA(unsigned idx)
{
      assert(idx < width_a_);
      return pin(idx+width_r_);
}

const Link& NetModulo::pin_DataA(unsigned idx) const
{
      assert(idx < width_a_);
      return pin(idx+width_r_);
}

Link& NetModulo::pin_DataB(unsigned idx)
{
      assert(idx < width_b_);
      return pin(idx+width_r_+width_a_);
}

const Link& NetModulo::pin_DataB(unsigned idx) const
{
      assert(idx < width_b_);
      return pin(idx+width_r_+width_a_);
}