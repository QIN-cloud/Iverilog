#include "config.h"
#include "netlist.h"
#include "compiler.h"
#include "PExpr.h"
#include <iostream>

static unsigned count_def_pins(const NetFuncDef*def)
{
      unsigned sum = 0;
      for (unsigned idx = 0 ;  idx < def->port_count() ;  idx += 1)
	    sum += def->port(idx)->pin_count();

      return sum;
}

NetUserFunc::NetUserFunc(NetScope*s, const char*n, NetScope*d)
: NetNode(s, lex_strings.add(n), count_def_pins(d->func_def())),
  def_(d)
{
      NetFuncDef*def = def_->func_def();

      unsigned port_wid = port_width(0);
      for (unsigned idx = 0 ;  idx < port_wid ;  idx += 1) {
	    pin(idx).set_dir(Link::OUTPUT);
	    pin(idx).set_name(def_->basename(), idx);
      }

      unsigned pin_base = port_wid;
      for (idx = 1 ;  idx < port_count() ;  idx += 1) {

	    const NetNet*port_sig = def->port(idx);
	    unsigned bits = port_width(idx);
	    for (unsigned bit = 0; bit < bits; bit += 1) {
		  pin(pin_base+bit).set_dir(Link::INPUT);
		  pin(pin_base+bit).set_name(port_sig->name(), bit);
		  pin(pin_base+bit).drive0(Link::HIGHZ);
		  pin(pin_base+bit).drive1(Link::HIGHZ);
	    }

	    pin_base += bits;
      }
}

NetUserFunc::~NetUserFunc()
{
}

unsigned NetUserFunc::port_count() const
{
      return def_->func_def()->port_count();
}

unsigned NetUserFunc::port_width(unsigned port) const
{
      NetFuncDef*def = def_->func_def();

      assert(port < def->port_count());
      const NetNet*port_sig = def->port(port);

      return port_sig->pin_count();
}

Link& NetUserFunc::port_pin(unsigned port, unsigned idx)
{
      NetFuncDef*def = def_->func_def();
      unsigned pin_base = 0;
      const NetNet*port_sig;

      assert(port < def->port_count());

      for (unsigned port_idx = 0 ;  port_idx < port ;  port_idx += 1) {
	    port_sig = def->port(port_idx);
	    pin_base += port_sig->pin_count();
      }

      port_sig = def->port(port);
      assert(idx < port_sig->pin_count());
      assert((pin_base+idx) < pin_count());

      return pin(pin_base+idx);
}


const NetScope* NetUserFunc::def() const
{
      return def_;
}

/*
 * This method of the PECallFunction class checks that the parameters
 * of the PECallFunction match the function definition. This is used
 * during elaboration to validate the parameters before using them.
 */
bool PECallFunction::check_call_matches_definition_(Design*des, NetScope*dscope) const
{
      assert(dscope);

	/* How many parameters have I got? Normally the size of the
	   list is correct, but there is the special case of a list of
	   1 nil pointer. This is how the parser tells me of no
	   parameter. In other words, ``func()'' is 1 nil parameter. */

      unsigned parms_count = parms_.count();
      if ((parms_count == 1) && (parms_[0] == 0))
	    parms_count = 0;

      if (dscope->type() != NetScope::FUNC) {
	    cerr << get_line() << ": error: Attempt to call scope "
		 << dscope->name() << " as a function." << endl;
	    des->errors += 1;
	    return false;
      }

      if ((parms_count+1) != dscope->func_def()->port_count()) {
	    cerr << get_line() << ": error: Function " << dscope->name()
		 << " expects " << (dscope->func_def()->port_count()-1)
		 << " parameters, you passed " << parms_count << "."
		 << endl;
	    des->errors += 1;
	    return false;
      }

      return true;
}