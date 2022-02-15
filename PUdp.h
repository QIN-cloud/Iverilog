#ifndef __PUdp_H
#define __PUdp_H

#include <map>
#include "svector.h"
#include <string>
#include "verinum.h"

class PExpr;

/*
 * This class represents a parsed UDP. This is a much simpler object
 * then a module or macromodule.
 *
 *   - all ports are scaler,
 *   - pin 0 (the first port) is always output,
 *     and the remaining pins are input.
 *
 * Thus, the ports can be represented as an ordered list of pin names.
 * If the output port is declared as a register in the Verilog source,
 * then this is a sequential UDP and the sequential flag is set to true.
 *
 * STATE TABLE
 * Each entry in the state table is given as a string with the same
 * number of characters as inputs. If the UDP is sequential, a
 * character is also included at the end of the string to represent
 * the current output.
 *
 * If the UDP is sequential, the "initial" member is taken to be the
 * intial value assigned in the source, or 'x' if none is given.
 */
class PUdp {

    public:
      explicit PUdp(const string&n, unsigned nports);

      svector<string>ports;
      bool sequential;

      svector<string>tinput;
      svector<char>  tcurrent;
      svector<char>  toutput;

	  verinum::V initial;

      map<string,PExpr*> attributes;

      void dump(std::ostream&out) const;

      const string name_;
    private:

    private: // Not implemented
      PUdp(const PUdp&);
      PUdp& operator= (const PUdp&);
};

#endif
