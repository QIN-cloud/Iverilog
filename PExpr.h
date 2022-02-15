#ifndef __PExpr_H
#define __PExpr_H

#include <set>
#include <string>
#include <map>
#include "LineInfo.h"
#include "verinum.h"
#include "verireal.h"
#include "HName.h"
#include "svector.h"
#include "netlist.h"
#include "Module.h"

class Module;
class PDesign;
class NetScope;
class Design;
class NetExpr;
class NetNet;

/*
 * The PExpr class hierarchy supports the description of
 * expressions. The parser can generate expression objects from the
 * source, possibly reducing things that it knows how to reduce.
 *
 * The elaborate_net method is used by structural elaboration to build
 * up a netlist interpretation of the expression.
 */

class PExpr : public LineInfo {

    public:
      PExpr();
      virtual ~PExpr();

	  // Procedural elaboration of the expression. Set the
	  // bare_memory_ok flag if the result is allowed to be a
	  // NetEMemory without an index.
      virtual NetExpr*elaborate_expr(Design*des, NetScope*scope,
		  bool sys_task_arg =false) const;
	  
	  // Elaborate expressions that are the r-value of parameter
	  // assignments. This elaboration follows the restrictions of
	  // constant expressions and supports later overriding and
	  // evaluation of parameters.
      virtual NetExpr*elaborate_pexpr(Design*des, NetScope*sc) const;
	  
	  // This method elaborate the expression as gates, for use in a
	  // continuous assign or other wholly structural context.
      virtual NetNet* elaborate_net(Design*des, NetScope*scope,
		  unsigned lwidth,
		  unsigned long rise,
		  unsigned long fall,
		  unsigned long decay,
		  Link::strength_t drive0 =Link::STRONG,
		  Link::strength_t drive1 =Link::STRONG)
		  const;
	  
	  // This method elaborates the expression as NetNet objects. It
	  // only allows regs suitable for procedural continuous assignments.
      virtual NetNet* elaborate_anet(Design*des, NetScope*scope) const;
	  
	  // This method elaborates the expression as gates, but
	  // restricted for use as l-values of continuous assignments.
      virtual NetNet* elaborate_lnet(Design*des, NetScope*scope,
		  bool implicit_net_ok =false) const;
	  
	  // Expressions that can be in the l-value of procedural
	  // assignments can be elaborated with this method.
      virtual NetAssign_* elaborate_lval(Design*des, NetScope*scope) const;

      virtual void dump(std::ostream&) const;

	  // This attempts to evaluate a constant expression, and return
	  // a verinum as a result. If the expression cannot be
	  // evaluated, return 0.
      virtual verinum* eval_const(const Design*des, const NetScope*sc) const;

	  virtual verinum* evaluate(Design*des, NetScope*scope, const vcd_vars);

	// This method returns true if that expression is the same as
	// this expression. This method is used for comparing
	// expressions that must be structurally "identical".
      virtual bool is_the_same(const PExpr*that) const;
      virtual string& get_var(unsigned idx);
      virtual unsigned vars_count();
      virtual svector<string>& get_vars();
	  virtual set<string>& get_funcname()
	  {
		  set<string>* tmp = new set<string>;
		  return *tmp;
	  };

	  // Return true if this expression is a valid constant
	// expression. the Module pointer is needed to find parameter
	// identifiers and any other module specific interpretations
	// of expressions.
      virtual bool is_constant(Module*) const;
	
    protected:
      svector<string>vars_;
    private: // not implemented
      PExpr(const PExpr&);
      PExpr& operator= (const PExpr&);
};

std::ostream& operator << (std::ostream&, const PExpr&);

class PEConcat : public PExpr {

    public:
      PEConcat(const svector<PExpr*>&p, PExpr*r =0);
      ~PEConcat();

      virtual void dump(std::ostream&) const;

	  // Concatenated Regs can be on the left of procedural
	  // continuous assignments.
      virtual NetNet* elaborate_anet(Design*des, NetScope*scope) const;
	  
      virtual NetNet* elaborate_lnet(Design*des, NetScope*scope,
		  bool implicit_net_ok =false) const;
      virtual NetNet* elaborate_net(Design*des, NetScope*scope,
		  unsigned width,
		  unsigned long rise,
		  unsigned long fall,
		  unsigned long decay,
		  Link::strength_t drive0,
		  Link::strength_t drive1) const;
      virtual NetExpr*elaborate_expr(Design*des, NetScope*,
		  bool sys_task_arg =false) const;
      virtual NetExpr*elaborate_pexpr(Design*des, NetScope*) const;
      virtual NetAssign_* elaborate_lval(Design*des, NetScope*scope) const;

	  virtual set<string>& get_funcname()
	  {
		  set<string>* tmp = new set<string>;
		  set<string>::const_iterator pos;
		  set<string> tmp1;

		  for(unsigned idx = 0; idx < parms_.count(); ++idx)
		  {
			  tmp1 = parms_[idx]->get_funcname();
			  for(pos = tmp1.begin();
					pos != tmp1.end(); ++pos)
						tmp->insert(*pos);
		  }

		  if(repeat_)
		  {
			  tmp1 = repeat_->get_funcname();
			  for(pos = tmp1.begin();
			  pos != tmp1.end(); ++pos)
				  tmp->insert(*pos);	
		  }

		  return *tmp;
	  };

	  virtual bool is_constant(Module*) const;

	  virtual verinum* evaluate(Design*des, NetScope*scope, const vcd_vars);

    private:
      svector<PExpr*>parms_;
      PExpr*repeat_;
};

/*
 * Event expressions are expressions that can be combined with the
 * event "or" operator. These include "posedge foo" and similar, and
 * also include named events. "edge" events are associated with an
 * expression, whereas named events simply have a name, which
 * represents an event variable.
 */
class PEEvent : public PExpr {

    public:
      enum edge_t {ANYEDGE, POSEDGE, NEGEDGE, POSITIVE};

	// Use this constructor to create events based on edges or levels.
      PEEvent(edge_t t, PExpr*e);

      ~PEEvent();

      edge_t type() const;
      PExpr* expr() const;
 
      virtual void dump(std::ostream&) const;
	  virtual set<string>& get_funcname()
	  {
		  set<string>* tmp = new set<string>;
		  set<string>::const_iterator pos;
		  set<string> tmp1;
		  tmp1 = expr_->get_funcname();
		  for(pos = tmp1.begin();
					pos != tmp1.end(); ++pos)
						tmp->insert(*pos);	

		  return *tmp;
	  };

	  virtual verinum* evaluate(Design*des, NetScope*scope, const vcd_vars);

    private:
      edge_t type_;
      PExpr *expr_;
};

/*
 * This holds a floating point constant in the source.
 */
class PEFNumber : public PExpr {

    public:
      explicit PEFNumber(verireal*vp);
      ~PEFNumber();

      const verireal& value() const;

      virtual void dump(std::ostream&) const;
	  
	  /* The eval_const method as applied to a floating point number
	   gets the *integer* value of the number. This accounts for
	   any rounding that is needed to get the value. */
      virtual verinum* eval_const(const Design*des, const NetScope*sc) const;

	  virtual set<string>& get_funcname()
	  {
		  set<string>* tmp = new set<string>;
		  return *tmp;
	  };

	  /* A PEFNumber is a constant, so this returns true. */
      virtual bool is_constant(Module*) const;

	  virtual NetExpr*elaborate_expr(Design*des, NetScope*,
				     bool sys_task_arg =false) const;
      virtual NetExpr*elaborate_pexpr(Design*des, NetScope*sc) const;

	  virtual verinum* evaluate(Design*des, NetScope*scope, const vcd_vars);
    private:
      verireal*value_;
};

class PEIdent : public PExpr {

    public:
      explicit PEIdent(const hname_t&s);
      ~PEIdent();

      virtual void dump(std::ostream&) const;

	  verinum* eval_const(const Design*des, const NetScope*sc) const;

	  virtual set<string>& get_funcname()
	  {
		  set<string>* tmp = new set<string>;
		  set<string>::const_iterator pos;
		  set<string> tmp1;
		  if(msb_)
		  {
			  tmp1 = msb_->get_funcname();
			  for(pos = tmp1.begin();
			  pos != tmp1.end(); ++pos)
				  tmp->insert(*pos);
		  }

		  if(lsb_)
		  {
			  tmp1 = lsb_->get_funcname();
			  for(pos = tmp1.begin();
			  pos != tmp1.end(); ++pos)
				  tmp->insert(*pos);	
		  }

		  return *tmp;
	  };

      const hname_t& path() const;

	  virtual bool is_constant(Module*) const;

	  // Regs can be on the left of procedural continuous assignments
      virtual NetNet* elaborate_anet(Design*des, NetScope*scope) const;
	  
	  // Identifiers are allowed (with restrictions) is assign l-values.
      virtual NetNet* elaborate_lnet(Design*des, NetScope*scope,
		  bool implicit_net_ok =false) const;
	  
	  // Identifiers are also allowed as procedural assignment l-values.
      virtual NetAssign_* elaborate_lval(Design*des, NetScope*scope) const;
	  
	  // Structural r-values are OK.
      virtual NetNet* elaborate_net(Design*des, NetScope*scope,
		  unsigned lwidth,
		  unsigned long rise,
		  unsigned long fall,
		  unsigned long decay,
		  Link::strength_t drive0,
		  Link::strength_t drive1) const;
	  
      virtual NetExpr*elaborate_expr(Design*des, NetScope*,
		  bool sys_task_arg =false) const;
      virtual NetExpr*elaborate_pexpr(Design*des, NetScope*sc) const;
	  
	  // Elaborate the PEIdent as a port to a module. This method
	  // only applies to Ident expressions.
      NetNet* elaborate_port(Design*des, NetScope*sc) const;

	  virtual verinum* evaluate(Design*des, NetScope*scope, const vcd_vars);

    private:
      hname_t path_;

    public:
		// Use these to support bit- and part-select operators.
		PExpr*msb_;
		PExpr*lsb_;
		
		// If this is a reference to a memory, this is the index
		// expression.
		PExpr*idx_;
		
		NetNet* elaborate_net_ram_(Design*des, NetScope*scope,
			NetMemory*mem, unsigned lwidth,
			unsigned long rise,
			unsigned long fall,
			unsigned long decay) const;
		
		NetNet* elaborate_net_bitmux_(Design*des, NetScope*scope,
			NetNet*sig,
			unsigned long rise,
			unsigned long fall,
			unsigned long decay,
			Link::strength_t drive0,
			Link::strength_t drive1) const;
		
    private:
		NetAssign_* elaborate_mem_lval_(Design*des, NetScope*scope,
			NetMemory*mem) const;
};

class PENumber : public PExpr {

    public:
      explicit PENumber(verinum*vp);
      ~PENumber();

      const verinum& value() const;

      virtual void dump(std::ostream&) const;

	  verinum* eval_const(const Design*des, const NetScope*sc) const;

	  virtual set<string>& get_funcname()
	  {
		  set<string>* tmp = new set<string>;
		  return *tmp;
	  };

	  //virtual verinum* eval_const(const Design*des, const NetScope*sc) const;

      virtual bool is_the_same(const PExpr*that) const;
      virtual bool is_constant(Module*) const;

	  virtual NetNet* elaborate_net(Design*des, NetScope*scope,
				    unsigned lwidth,
				    unsigned long rise,
				    unsigned long fall,
				    unsigned long decay,
				    Link::strength_t drive0,
				    Link::strength_t drive1) const;
      virtual NetExpr*elaborate_expr(Design*des, NetScope*,
				     bool sys_task_arg =false) const;
      virtual NetExpr*elaborate_pexpr(Design*des, NetScope*sc) const;
      virtual NetAssign_* elaborate_lval(Design*des, NetScope*scope) const;

	  virtual verinum* evaluate(Design*des, NetScope*scope, const vcd_vars);

    private:
      verinum*const value_;

};

/*
 * This represents a string constant in an expression.
 *
 * The s parameter to the PEString constructor is a C string that this
 * class instance will take for its own. The caller should not delete
 * the string, the destructor will do it.
 */
class PEString : public PExpr {

    public:
      explicit PEString(char*s);
      ~PEString();

      string value() const;
      virtual void dump(std::ostream&) const;
	  virtual set<string>& get_funcname()
	  {
		  set<string>* tmp = new set<string>;
		  return *tmp;
	  };

	  virtual bool is_constant(Module*) const;

	  verinum* eval_const(const Design*, const NetScope*) const;

	  virtual NetNet* elaborate_net(Design*des, NetScope*scope,
				    unsigned width,
				    unsigned long rise,
				    unsigned long fall,
				    unsigned long decay,
				    Link::strength_t drive0,
				    Link::strength_t drive1) const;
      virtual NetExpr*elaborate_expr(Design*des, NetScope*,
				     bool sys_task_arg =false) const;
      virtual NetExpr*elaborate_pexpr(Design*des, NetScope*sc) const;

	  virtual verinum* evaluate(Design*des, NetScope*scope, const vcd_vars);

    private:
      char*text_;
};

class PEUnary : public PExpr {

    public:
      explicit PEUnary(char op, PExpr*ex);
      ~PEUnary();

      virtual void dump(std::ostream&out) const;

	  virtual verinum* eval_const(const Design*des, const NetScope*sc) const;

	  virtual set<string>& get_funcname()
	  {
		  set<string>* tmp = new set<string>;
		  set<string>::const_iterator pos;
		  set<string> tmp1;
		  tmp1 = expr_->get_funcname();
		  for(pos = tmp1.begin();
					pos != tmp1.end(); ++pos)
						tmp->insert(*pos);	

		  return *tmp;
	  };

	  virtual bool is_constant(Module*) const;

	  virtual NetNet* elaborate_net(Design*des, NetScope*scope,
				    unsigned width,
				    unsigned long rise,
				    unsigned long fall,
				    unsigned long decay,
				    Link::strength_t drive0,
				    Link::strength_t drive1) const;
      virtual NetExpr*elaborate_expr(Design*des, NetScope*,
				     bool sys_task_arg =false) const;
      virtual NetExpr*elaborate_pexpr(Design*des, NetScope*sc) const;

	  virtual verinum* evaluate(Design*des, NetScope*scope, const vcd_vars);

    private:
      char op_;
      PExpr*expr_;
};

class PEBinary : public PExpr {

    public:
      explicit PEBinary(char op, PExpr*l, PExpr*r);
      ~PEBinary();

      virtual void dump(std::ostream&out) const;

	  virtual verinum* eval_const(const Design*des, const NetScope*sc) const;

	  virtual bool is_constant(Module*) const;

	  virtual set<string>& get_funcname()
	  {
		  set<string>* tmp = new set<string>;
		  set<string>::const_iterator pos;
		  set<string> tmp1;

		  tmp1 = left_->get_funcname();
		  for(pos = tmp1.begin();
		  pos != tmp1.end(); ++pos)
			  tmp->insert(*pos);

		  tmp1 = right_->get_funcname();
		  for(pos = tmp1.begin();
					pos != tmp1.end(); ++pos)
						tmp->insert(*pos);	

		  return *tmp;
	  };

	  virtual NetNet* elaborate_net(Design*des, NetScope*scope,
				    unsigned width,
				    unsigned long rise,
				    unsigned long fall,
				    unsigned long decay,
				    Link::strength_t drive0,
				    Link::strength_t drive1) const;
      virtual NetExpr*elaborate_expr(Design*des, NetScope*,
					bool sys_task_arg =false) const;
      virtual NetExpr*elaborate_pexpr(Design*des, NetScope*sc) const;

	  virtual verinum* evaluate(Design*des, NetScope*scope, const vcd_vars);

    private:
      char op_;
      PExpr*left_;
      PExpr*right_;

	  NetEBinary*elaborate_expr_base_(Design*, NetExpr*lp, NetExpr*rp) const;

      NetNet* elaborate_net_add_(Design*des, NetScope*scope,
				 unsigned lwidth,
				 unsigned long rise,
				 unsigned long fall,
				 unsigned long decay) const;
      NetNet* elaborate_net_bit_(Design*des, NetScope*scope,
				 unsigned lwidth,
				 unsigned long rise,
				 unsigned long fall,
				 unsigned long decay) const;
      NetNet* elaborate_net_cmp_(Design*des, NetScope*scope,
				 unsigned lwidth,
				 unsigned long rise,
				 unsigned long fall,
				 unsigned long decay) const;
      NetNet* elaborate_net_div_(Design*des, NetScope*scope,
				 unsigned lwidth,
				 unsigned long rise,
				 unsigned long fall,
				 unsigned long decay) const;
      NetNet* elaborate_net_mod_(Design*des, NetScope*scope,
				 unsigned lwidth,
				 unsigned long rise,
				 unsigned long fall,
				 unsigned long decay) const;
      NetNet* elaborate_net_log_(Design*des, NetScope*scope,
				 unsigned lwidth,
				 unsigned long rise,
				 unsigned long fall,
				 unsigned long decay) const;
      NetNet* elaborate_net_mul_(Design*des, NetScope*scope,
				 unsigned lwidth,
				 unsigned long rise,
				 unsigned long fall,
				 unsigned long decay) const;
      NetNet* elaborate_net_shift_(Design*des, NetScope*scope,
				   unsigned lwidth,
				   unsigned long rise,
				   unsigned long fall,
				   unsigned long decay) const;
};

/*
 * This class supports the ternary (?:) operator. The operator takes
 * three expressions, the test, the true result and the false result.
 */
class PETernary : public PExpr {

    public:
      explicit PETernary(PExpr*e, PExpr*t, PExpr*f);
      ~PETernary();

      virtual void dump(std::ostream&out) const;
	  virtual bool is_constant(Module*) const;
	  virtual verinum* eval_const(const Design*des, const NetScope*sc) const;

	  virtual set<string>& get_funcname()
	  {
		  set<string>* tmp = new set<string>;
		  set<string>::const_iterator pos;
		  set<string> tmp1;
		  
		  tmp1 = expr_->get_funcname();
		  for(pos = tmp1.begin();
		  pos != tmp1.end(); ++pos)
			  tmp->insert(*pos);
		  
		  tmp1 = tru_->get_funcname();
		  for(pos = tmp1.begin();
		  pos != tmp1.end(); ++pos)
			  tmp->insert(*pos);
		  
		  tmp1 = fal_->get_funcname();
		  for(pos = tmp1.begin();
		  pos != tmp1.end(); ++pos)
			  tmp->insert(*pos);

		  return *tmp;
	  };

	  virtual NetNet* elaborate_net(Design*des, NetScope*scope,
				    unsigned width,
				    unsigned long rise,
				    unsigned long fall,
				    unsigned long decay,
				    Link::strength_t drive0,
				    Link::strength_t drive1) const;
      virtual NetExpr*elaborate_expr(Design*des, NetScope*,
					 bool sys_task_arg =false) const;
      virtual NetExpr*elaborate_pexpr(Design*des, NetScope*sc) const;

	  virtual verinum* evaluate(Design*des, NetScope*scope, const vcd_vars);

    private:
      PExpr*expr_;
      PExpr*tru_;
      PExpr*fal_;
};

/*
 * This class represents a parsed call to a function, including calls
 * to system functions. The parameters in the parms list are the
 * expressions that are passed as input to the ports of the function.
 */
class PECallFunction : public PExpr {
    public:
      explicit PECallFunction(const hname_t&n, const svector<PExpr *> &parms);
      explicit PECallFunction(const hname_t&n);
      ~PECallFunction();

      virtual void dump(std::ostream &) const;
	  virtual set<string>& get_funcname()
	  {
		  set<string>* tmp = new set<string>;
		  tmp->insert(path_.peek_name(path_.component_count()-1));

		  return *tmp;
	  };

	  virtual NetNet* elaborate_net(Design*des, NetScope*scope,
				    unsigned width,
				    unsigned long rise,
				    unsigned long fall,
				    unsigned long decay,
				    Link::strength_t drive0,
				    Link::strength_t drive1) const;
      virtual NetExpr*elaborate_expr(Design*des, NetScope*scope,
				     bool sys_task_arg =false) const;

	  virtual verinum* evaluate(Design*des, NetScope*scope, const vcd_vars);

    private:
      hname_t path_;
      svector<PExpr *> parms_;

	  bool check_call_matches_definition_(Design*des, NetScope*dscope) const;

	  NetExpr* elaborate_sfunc_(Design*des, NetScope*scope) const;
};

#endif
