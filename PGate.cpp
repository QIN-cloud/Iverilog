#include "config.h"

#include "PGate.h"
#include "PExpr.h"
#include <assert.h>

PGate::PGate(const string&name,
	     svector<PExpr*>*pins,
	     const svector<PExpr*>*del)
: name_(name), pins_(pins)
{
      if (del) delay_.set_delays(del);
      str0_ = NO_STRENGTH;
      str1_ = NO_STRENGTH;
}

PGate::PGate(const string&name,
	     svector<PExpr*>*pins,
	     PExpr*del)
: name_(name), pins_(pins)
{
      if (del) delay_.set_delay(del);
      str0_ = NO_STRENGTH;
      str1_ = NO_STRENGTH;
}

PGate::PGate(const string&name, svector<PExpr*>*pins)
: name_(name), pins_(pins)
{
      str0_ = NO_STRENGTH;
      str1_ = NO_STRENGTH;
}

PGate::~PGate()
{
}

PGate::strength_t PGate::strength0() const
{
      return str0_;
}

void PGate::strength0(PGate::strength_t s)
{
      str0_ = s;
}

PGate::strength_t PGate::strength1() const
{
      return str1_;
}

void PGate::strength1(PGate::strength_t s)
{
      str1_ = s;
}

PGAssign::PGAssign(svector<PExpr*>*pins)
: PGate("", pins)
{
      assert(pins->count() == 2);
	  ty = ASSIGN;
}

PGAssign::PGAssign(svector<PExpr*>*pins, svector<PExpr*>*dels)
: PGate("", pins, dels)
{
      assert(pins->count() == 2);
	  ty = ASSIGN;
}

PGAssign::~PGAssign()
{
}

PGBuiltin::PGBuiltin(Type t, const string&name,
		     svector<PExpr*>*pins,
		     svector<PExpr*>*del)
: PGate(name, pins, del), type_(t), msb_(0), lsb_(0)
{
	ty = BUILDIN;
}

PGBuiltin::PGBuiltin(Type t, const string&name,
		     svector<PExpr*>*pins,
		     PExpr*del)
: PGate(name, pins, del), type_(t), msb_(0), lsb_(0)
{
	ty = BUILDIN;
}

PGBuiltin::~PGBuiltin()
{
}

void PGBuiltin::set_range(PExpr*msb, PExpr*lsb)
{
      assert(msb_ == 0);
      assert(lsb_ == 0);

      msb_ = msb;
      lsb_ = lsb;
}

PGModule::PGModule(const char*type, const string&name, svector<PExpr*>*pns)
: PGate(name, pns), overrides_(0), pins(0),
  npins_(0), parms_(0), nparms_(0), msb_(0), lsb_(0)
{
      type_ = strdup(type);
	  ty = MODULE;
}

PGModule::PGModule(const char*type, const string&name,
		   named<PExpr*>*pns, unsigned npins)
: PGate(name, 0), overrides_(0), pins(pns),
  npins_(npins), parms_(0), nparms_(0), msb_(0), lsb_(0)
{
      type_ = strdup(type);
	  ty = MODULE;
}

PGModule::~PGModule()
{
      free(type_);
}

void PGModule::set_parameters(svector<PExpr*>*o)
{
      assert(overrides_ == 0);
      overrides_ = o;
}

void PGModule::set_parameters(named<PExpr*>*pa, unsigned npa)
{
      assert(parms_ == 0);
      assert(overrides_ == 0);
      parms_ = pa;
      nparms_ = npa;
}

void PGModule::set_range(PExpr*msb, PExpr*lsb)
{
      assert(msb_ == 0);
      assert(lsb_ == 0);

      msb_ = msb;
      lsb_ = lsb;
}

const char* PGModule::get_type()
{
      return type_;
}

/*
 * This method is used during elaboration to calculate the
 * rise/fall/decay times for the gate. These values were set in pform
 * by the constructor, so here I evaluate the expression in the given
 * design context and save the calculated delays into the output
 * parameters. This method understands how to handle the different
 * numbers of expressions.
 */
void PGate::eval_delays(Design*des, NetScope*scope,
			unsigned long&rise_time,
			unsigned long&fall_time,
			unsigned long&decay_time) const
{
      delay_.eval_delays(des, scope, rise_time, fall_time, decay_time);
}

void PGate::elaborate_scope(Design*, NetScope*) const
{
}