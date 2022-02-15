#include "config.h"

#include <iostream>

#include "PExpr.h"
#include "Module.h"
#include <typeinfo>

PExpr::PExpr()
{
}

PExpr::~PExpr()
{
}

bool PExpr::is_the_same(const PExpr*that) const
{
      return typeid(this) == typeid(that);
}

string& PExpr::get_var(unsigned idx)
{
	assert(idx < vars_.count());
	return vars_[idx];
}

unsigned PExpr::vars_count()
{
	return vars_.count();
}

svector<string>& PExpr::get_vars()
{
	return vars_;
}

NetNet* PExpr::elaborate_lnet(Design*des, NetScope*, bool) const
{
      cerr << get_line() << ": error: expression not valid in assign l-value: "
	   << *this << endl;
      return 0;
}
		  
PEBinary::PEBinary(char op, PExpr*l, PExpr*r)
: op_(op), left_(l), right_(r)
{
	if(l != 0)
	  vars_ = svector<string>(vars_, l->get_vars());
	if(r != 0)
	  vars_ = svector<string>(vars_, r->get_vars());
}

PEBinary::~PEBinary()
{
}

PECallFunction::PECallFunction(const hname_t&n, const svector<PExpr *> &parms) 
: path_(n), parms_(parms)
{
	for(unsigned idx = 0; idx < parms.count(); ++idx)
  	  vars_ = svector<string>(vars_, parms[idx]->get_vars());
}

PECallFunction::PECallFunction(const hname_t&n) 
: path_(n)
{
}

PECallFunction::~PECallFunction()
{
}

PEConcat::PEConcat(const svector<PExpr*>&p, PExpr*r)
: parms_(p), repeat_(r)
{
	for(unsigned idx = 0; idx < p.count(); ++idx)
	  vars_ = svector<string>(vars_, p[idx]->get_vars());
	if(r)
	  vars_ = svector<string>(vars_, r->get_vars());
}

PEConcat::~PEConcat()
{
      delete repeat_;
}

PEEvent::PEEvent(PEEvent::edge_t t, PExpr*e)
: type_(t), expr_(e)
{
	if(e)
	  vars_ = svector<string>(vars_, e->get_vars());
}

PEEvent::~PEEvent()
{
}

PEEvent::edge_t PEEvent::type() const
{
      return type_;
}

PExpr* PEEvent::expr() const
{
      return expr_;
}

PEFNumber::PEFNumber(verireal*v)
: value_(v)
{
}

PEFNumber::~PEFNumber()
{
      delete value_;
}

const verireal& PEFNumber::value() const
{
      return *value_;
}

bool PEFNumber::is_constant(Module*) const
{
      return true;
}

PEIdent::PEIdent(const hname_t&s)
: path_(s), msb_(0), lsb_(0), idx_(0)
{
	string tmps(s.peek_name(s.component_count()-1));
	vars_ = svector<string>(vars_, tmps);
}

PEIdent::~PEIdent()
{
}

const hname_t& PEIdent::path() const
{
      return path_;
}

PENumber::PENumber(verinum*vp)
: value_(vp)
{
      assert(vp);
}

PENumber::~PENumber()
{
      delete value_;
}

const verinum& PENumber::value() const
{
      return *value_;
}

bool PENumber::is_the_same(const PExpr*that) const
{
      const PENumber*obj = dynamic_cast<const PENumber*>(that);
      if (obj == 0)
	    return false;

      return *value_ == *obj->value_;
}

bool PENumber::is_constant(Module*) const
{
      return true;
}

PEString::PEString(char*s)
: text_(s)
{
}

PEString::~PEString()
{
      delete[]text_;
}

string PEString::value() const
{
      return text_;
}

PETernary::PETernary(PExpr*e, PExpr*t, PExpr*f)
: expr_(e), tru_(t), fal_(f)
{
	if(e)
	  vars_ = svector<string>(vars_, e->get_vars());
	if(t)
	  vars_ = svector<string>(vars_, t->get_vars());
	if(f)
	  vars_ = svector<string>(vars_, f->get_vars());
}

PETernary::~PETernary()
{
}

PEUnary::PEUnary(char op, PExpr*ex)
: op_(op), expr_(ex)
{
	if(ex)
	  vars_ = svector<string>(vars_, ex->get_vars());
}

PEUnary::~PEUnary()
{
}

bool PExpr::is_constant(Module*) const
{
      return false;
}

bool PEBinary::is_constant(Module*mod) const
{
      return left_->is_constant(mod) && right_->is_constant(mod);
}

bool PEConcat::is_constant(Module *mod) const
{
      bool constant = repeat_? repeat_->is_constant(mod) : true;
      for (unsigned i = 0; constant && i < parms_.count(); ++i) {
	    constant = constant && parms_[i]->is_constant(mod);
      }
      return constant;
}

/*
 * An identifier can be in a constant expression if (and only if) it is
 * a parameter.
 */
bool PEIdent::is_constant(Module*mod) const
{
      if (mod == 0) return false;

      { map<string,Module::param_expr_t>::const_iterator cur;
        cur = mod->parameters.find(path_.peek_name(0));
	if (cur != mod->parameters.end()) return true;
      }

      { map<string,Module::param_expr_t>::const_iterator cur;
        cur = mod->localparams.find(path_.peek_name(0));
	if (cur != mod->localparams.end()) return true;
      }

      return false;
}

bool PEString::is_constant(Module*) const
{
      return true;
}

bool PETernary::is_constant(Module*m) const
{
      return expr_->is_constant(m)
	    && tru_->is_constant(m)
	    && fal_->is_constant(m);
}

bool PEUnary::is_constant(Module*m) const
{
      return expr_->is_constant(m);
}
