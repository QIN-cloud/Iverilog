#ifndef __PGate_H
#define __PGate_H

#include "svector.h"
#include "named.h"
#include "LineInfo.h"
#include "PDelays.h"
#include <map>
#include <set>
#include <string>
class PExpr;
class PUdp;
class Module;
class CfgNode;
class PDesign;

/*
 * A PGate represents a Verilog gate. The gate has a name and other
 * properties, and a set of pins that connect to wires. It is known at
 * the time a gate is constructed how many pins the gate has.
 *
 * This pins of a gate are connected to expressions. The elaboration
 * step will need to convert expressions to a network of gates in
 * order to elaborate expression inputs, but that can easily be done.
 *
 * The PGate base class also carries the strength0 and strength1
 * strengths for those gates where the driver[s] can be described by a
 * single strength pair. There is a strength of the 0 drive, and a
 * strength of the 1 drive.
 */
class PGate : public LineInfo {
      
    public:
      enum strength_t { HIGHZ, WEAK, PULL, STRONG, SUPPLY, NO_STRENGTH};
	  enum GType {BUILDIN, ASSIGN, MODULE};

      explicit PGate(const string&name, svector<PExpr*>*pins,
		     const svector<PExpr*>*del);

      explicit PGate(const string&name, svector<PExpr*>*pins,
		     PExpr*del);

      explicit PGate(const string&name, svector<PExpr*>*pins);

      virtual ~PGate();

      const string& get_name() const { return name_; }
	  virtual const char* get_type(){return "";};

	  void eval_delays(Design*des, NetScope*scope,
		       unsigned long&rise_time,
		       unsigned long&fall_time,
		       unsigned long&decay_time) const;

      unsigned pin_count() const { return pins_? pins_->count() : 0; }
      const PExpr*pin(unsigned idx) const { return (*pins_)[idx]; }

      strength_t strength0() const;
      strength_t strength1() const;

      void strength0(strength_t);
      void strength1(strength_t);

      map<string,PExpr*> attributes;

      virtual CfgNode* build_node(PDesign& de){return 0;};
      virtual void dump(std::ostream&out) const;
	  virtual set<string>& get_modname()
	  {
		  set<string>* tmp = new set<string>;
		  return *tmp;
	  };
	  virtual set<string>& get_funcname()
	  {
		  set<string>* tmp = new set<string>;
		  return *tmp;
	  };
	  
	  GType ty;

	  virtual void elaborate(Design*des, NetScope*scope) const;
      virtual void elaborate_scope(Design*des, NetScope*sc) const;
      virtual bool elaborate_sig(Design*des, NetScope*scope) const;

    protected:
      const svector<PExpr*>* get_pins() const { return pins_; }

      void dump_pins(std::ostream&out) const;
      void dump_delays(std::ostream&out) const;

      const string name_;
      PDelays delay_;
      svector<PExpr*>*pins_;

      strength_t str0_, str1_;

    private: // not implemented
      PGate(const PGate&);
      PGate& operator= (const PGate&);
};

/* A continuous assignment has a single output and a single input. The
   input is passed directly to the output. This is different from a
   BUF because elaboration may need to turn this into a vector of
   gates. */
class PGAssign  : public PGate {

    public:
      explicit PGAssign(svector<PExpr*>*pins);
      explicit PGAssign(svector<PExpr*>*pins, svector<PExpr*>*dels);
      ~PGAssign();

      virtual CfgNode* build_node(PDesign& de);
      void dump(std::ostream&out) const;
	  virtual set<string>& get_modname()
	  {
		  set<string>* tmp = new set<string>;
		  return *tmp;
	  };
	  virtual set<string>& get_funcname()
	  {
		  set<string>* tmp = new set<string>;
		  set<string>::const_iterator pos;
		  set<string> tmp1;

		  tmp1 = (*pins_)[1]->get_funcname();
		  for(pos = tmp1.begin();
		      pos != tmp1.end();
			  ++pos)
				  tmp->insert(*pos);

		  return *tmp;
	  };

	  virtual void elaborate(Design*des, NetScope*scope) const;

	  virtual const char* get_type(){return "";};

    private:		
};


/*
 * The Builtin class is specifically a gate with one of the builtin
 * types. The parser recognizes these types during parse. These types
 * have special properties that allow them to be treated specially.
 *
 * A PGBuiltin can be grouped into an array of devices. If this is
 * done, the msb_ and lsb_ are set to the indices of the array
 * range. Elaboration causes a gate to be created for each element of
 * the array, and a name will be generated for each gate.
 */
class PGBuiltin  : public PGate {

    public:
      enum Type { AND, NAND, OR, NOR, XOR, XNOR, BUF, BUFIF0, BUFIF1,
		  NOT, NOTIF0, NOTIF1, PULLDOWN, PULLUP, NMOS, RNMOS,
		  PMOS, RPMOS, CMOS, RCMOS, TRAN, RTRAN, TRANIF0,
		  TRANIF1, RTRANIF0, RTRANIF1 };

    public:
      explicit PGBuiltin(Type t, const string&name,
			 svector<PExpr*>*pns,
			 svector<PExpr*>*del);
      explicit PGBuiltin(Type t, const string&name,
			 svector<PExpr*>*pins,
			 PExpr*del);
      ~PGBuiltin();

      Type type() const { return type_; }
      void set_range(PExpr*msb, PExpr*lsb);

      virtual CfgNode* build_node(PDesign& de);
      virtual void dump(std::ostream&out) const;
	  virtual set<string>& get_modname()
	  {
		  set<string>* tmp = new set<string>;
		  return *tmp;
	  };
	  virtual set<string>& get_funcname()
	  {
		  set<string>* tmp = new set<string>;
		  return *tmp;
	  };

	  virtual void elaborate(Design*, NetScope*scope) const;

	  virtual const char* get_type(){return "";};

    private:
      Type type_;

      PExpr*msb_;
      PExpr*lsb_;
};

/*
 * This kind of gate is an instantiation of a module. The stored type
 * is the name of a module definition somewhere in the pform. This
 * type also handles UDP devices, because it is generally not known at
 * parse time whether a name belongs to a module or a UDP.
 */
class PGModule  : public PGate {

    public:
	// If the binding of ports is by position, this constructor
	// builds everything all at once.
      explicit PGModule(const char*type, const string&name,
			svector<PExpr*>*pins);

	// If the binding of ports is by name, this constructor takes
	// the bindings and stores them for later elaboration.
      explicit PGModule(const char*type, const string&name,
			named<PExpr*>*pns, unsigned npins);


      ~PGModule();

	// Parameter overrides can come as an ordered list, or a set
	// of named expressions.
      void set_parameters(svector<PExpr*>*o);
      void set_parameters(named<PExpr*>*pa, unsigned npa);

	// Modules can be instantiated in ranges. The parser uses this
	// method to pass the range to the pform.
      void set_range(PExpr*msb, PExpr*lsb);

      virtual CfgNode* build_node(PDesign& de);
      virtual void dump(std::ostream&out) const;
      virtual const char* get_type();

	  virtual set<string>& get_modname()
	  {
		  set<string>* tmp = new set<string>;
		  tmp->insert(type_);
		  return *tmp;
	  };
	  virtual set<string>& get_funcname()
	  {
		  set<string>* tmp = new set<string>;
		  return *tmp;
	  };

	  virtual void elaborate(Design*, NetScope*scope) const;
      virtual void elaborate_scope(Design*des, NetScope*sc) const;
      virtual bool elaborate_sig(Design*des, NetScope*scope) const;

    private:
      char* type_;
      svector<PExpr*>*overrides_;
      named<PExpr*>*pins;
      unsigned npins_;

	// These members support parameter override by name
      named<PExpr*>*parms_;
      unsigned nparms_;

	// Arrays of modules are give if these are set.
      PExpr*msb_;
      PExpr*lsb_;

	  void elaborate_mod_(Design*, Module*mod, NetScope*scope) const;
      void elaborate_udp_(Design*, PUdp  *udp, NetScope*scope) const;
      void elaborate_scope_mod_(Design*des, Module*mod, NetScope*sc) const;
      bool elaborate_sig_mod_(Design*des, NetScope*scope, Module*mod) const;
};

#endif
