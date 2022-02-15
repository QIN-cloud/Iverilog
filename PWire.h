#ifndef __PWire_H
#define __PWire_H

#include "LineInfo.h"
#include "netlist.h"
#include <map>
#include "svector.h"
#include "HName.h"

#ifdef HAVE_IOSFWD
# include  <iosfwd>
#else
class ostream;
#endif
using namespace std;

class PExpr;
class Design;

/*
 * Wires include nets, registers and ports. A net or register becomes
 * a port by declaration, so ports are not seperate. The module
 * identifies a port by keeping it in its port list.
 *
 * The hname parameter to the constructor is a hierarchical name. It
 * is an array of strings starting with the root, running towards
 * the base name, and terminated by a null pointer. The environment
 * allocates the memory for me.
 */
class PWire : public LineInfo {

    public:
      //enum Type { IMPLICIT, IMPLICIT_REG, INTEGER, WIRE, TRI, TRI1, SUPPLY0,
	  //	  SUPPLY1, WAND, TRIAND, TRI0, WOR, TRIOR, REG };

      //enum PortType { NOT_A_PORT, PIMPLICIT, PINPUT, POUTPUT, PINOUT };
      
	  PWire(const hname_t&hname, NetNet::Type t, NetNet::PortType pt);
      PWire(char*name, NetNet::Type t, NetNet::PortType pt);

	// Return a hierarchical name.
	//const string name() const;
      const hname_t&path() const;

      NetNet::Type get_wire_type() const;
      bool set_wire_type(NetNet::Type);

      NetNet::PortType get_port_type() const;
      bool set_port_type(NetNet::PortType);

      void set_signed(bool flag);
      bool get_signed() const;
      bool get_isint() const;

      void set_range(PExpr*msb, PExpr*lsb);

      void set_memory_idx(PExpr*ldx, PExpr*rdx);

      map<string,PExpr*> attributes;

	// Write myself to the specified stream.
      void dump(std::ostream&out) const;

	  void elaborate_sig(Design*, NetScope*scope);
	  
	  unsigned getwidth();
	  long getlsb();
	  long getmsb();

    private:
      hname_t hname_;
      NetNet::Type type_;
      NetNet::PortType port_type_;
      bool signed_;
      bool isint_;		// original type of integer

	// These members hold expressions for the bit width of the
	// wire. If they do not exist, the wire is 1 bit wide.
      svector<PExpr*>msb_;
      svector<PExpr*>lsb_;

	// If this wire is actually a memory, these indices will give
	// me the size and address range of the memory.
      PExpr*lidx_;
      PExpr*ridx_;

  	  unsigned iwid_;
	  long ilsb_, imsb_;

    private: // not implemented
      PWire(const PWire&);
      PWire& operator= (const PWire&);
};

#endif
