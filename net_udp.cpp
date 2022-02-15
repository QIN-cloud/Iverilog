#include "config.h"
#include "compiler.h"
#include "netlist.h"

NetUDP::NetUDP(NetScope*s, const string&n, unsigned pins, PUdp *u)
  : NetNode(s, lex_strings.add(n.c_str()), pins), udp(u)
{
      pin(0).set_dir(Link::OUTPUT);
      pin(0).set_name("O", 0);
      for (unsigned idx = 1 ;  idx < pins ;  idx += 1) {
	    pin(idx).set_dir(Link::INPUT);
	    pin(idx).set_name("I", idx-1);
      }
}

bool NetUDP::first(string&inp, char&out) const
{
  table_idx = (unsigned) -1;
  return next(inp, out);
}
  
bool NetUDP::next(string&inp, char&out) const
{
  table_idx++;

  if (table_idx >= udp->tinput.count())
    return false;

  if (is_sequential())
    {
      inp = string("") + udp->tcurrent[table_idx] + udp->tinput[table_idx];
      assert(inp.length() == pin_count());
    }
  else
    {
      inp = udp->tinput[table_idx];
      assert(inp.length() == (pin_count()-1));
    }

  out = udp->toutput[table_idx];
  assert( (out == '0') 
       || (out == '1') 
       || (out == 'x') 
       || (is_sequential() && (out == '-')));

  return true;
}

char NetUDP::get_initial() const
{
  assert (is_sequential());

  switch (udp->initial)
    {
    case verinum::V0:
      return '0';
    case verinum::V1:
      return '1';
    case verinum::Vx:
    case verinum::Vz:
      return 'x';
    }

  assert(0);
  return 'x';
}