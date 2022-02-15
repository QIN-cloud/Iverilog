#include "config.h"

#include "netlist.h"

/*
 * NetAssign
 */

unsigned count_lval_width(const NetAssign_*idx)
{
      unsigned wid = 0;
      while (idx) {
	    wid += idx->lwidth();
	    idx = idx->more;
      }
      return wid;
}

NetAssign_::NetAssign_(NetNet*s)
: sig_(s), mem_(0), var_(0), bmux_(0)
{
      loff_ = 0;
      lwid_ = sig_->pin_count();
      sig_->incr_lref();
      more = 0;
}

NetAssign_::NetAssign_(NetMemory*s)
: sig_(0), mem_(s), var_(0), bmux_(0)
{
      loff_ = 0;
      lwid_ = mem_->width();
      more = 0;
}

NetAssign_::NetAssign_(NetVariable*s)
: sig_(0), mem_(0), var_(s), bmux_(0)
{
      loff_ = 0;
      lwid_ = 0;
      more = 0;
}

NetAssign_::~NetAssign_()
{
      if (sig_) {
	    sig_->decr_lref();
	    if (sig_->peek_lref() == 0)
		  sig_->type(NetNet::WIRE);
      }

      assert( more == 0 );
      if (bmux_) delete bmux_;
}

void NetAssign_::set_bmux(NetExpr*r)
{
      assert(bmux_ == 0);
      bmux_ = r;
}

NetExpr* NetAssign_::bmux()
{
      return bmux_;
}

const NetExpr* NetAssign_::bmux() const
{
      return bmux_;
}

unsigned NetAssign_::lwidth() const
{
      if (mem_)  return lwid_;
      else if (bmux_) return 1;
      else return lwid_;
}

const char*NetAssign_::name() const
{
      if (sig_) {
	    return sig_->name();
      } else if (mem_) {
	    return mem_->name();
      } else {
	    return "";
      }
}

NetNet* NetAssign_::sig() const
{
      return sig_;
}

NetMemory* NetAssign_::mem() const
{
      return mem_;
}

NetVariable* NetAssign_::var() const
{
      return var_;
}


void NetAssign_::set_part(unsigned lo, unsigned lw)
{
      loff_ = lo;
      lwid_ = lw;
      if (sig_) {
	    assert(sig_->pin_count() >= (lo + lw));
      } else {
	    assert(mem_);
	    assert(lwid_ == mem_->width());
      }
}

unsigned NetAssign_::get_loff() const
{
      return loff_;
}

NetAssignBase::NetAssignBase(NetAssign_*lv, NetExpr*rv)
: lval_(lv), rval_(rv), delay_(0)
{
}

NetAssignBase::~NetAssignBase()
{
      if (rval_) delete rval_;
      while (lval_) {
	    NetAssign_*tmp = lval_;
	    lval_ = tmp->more;
	    tmp->more = 0;
	    delete tmp;
      }
}

NetExpr* NetAssignBase::rval()
{
      return rval_;
}

const NetExpr* NetAssignBase::rval() const
{
      return rval_;
}

void NetAssignBase::set_rval(NetExpr*r)
{
      if (rval_) delete rval_;
      rval_ = r;
}

NetAssign_* NetAssignBase::l_val(unsigned idx)
{
      NetAssign_*cur = lval_;
      while (idx > 0) {
	    if (cur == 0)
		  return cur;

	    cur = cur->more;
	    idx -= 1;
      }

      assert(idx == 0);
      return cur;
}

const NetAssign_* NetAssignBase::l_val(unsigned idx) const
{
      const NetAssign_*cur = lval_;
      while (idx > 0) {
	    if (cur == 0)
		  return cur;

	    cur = cur->more;
	    idx -= 1;
      }

      assert(idx == 0);
      return cur;
}

unsigned NetAssignBase::l_val_count() const
{
      const NetAssign_*cur = lval_;
      unsigned cnt = 0;
      while (cur) {
	    cnt += 1;
	    cur = cur->more;
      }

      return cnt;
}

unsigned NetAssignBase::lwidth() const
{
      unsigned sum = 0;
      for (NetAssign_*cur = lval_ ;  cur ;  cur = cur->more)
	    sum += cur->lwidth();
      return sum;
}

void NetAssignBase::set_delay(NetExpr*expr)
{
      delay_ = expr;
}

const NetExpr* NetAssignBase::get_delay() const
{
      return delay_;
}

NetAssign::NetAssign(NetAssign_*lv, NetExpr*rv)
: NetAssignBase(lv, rv)
{
}

NetAssign::~NetAssign()
{
}

NetAssignNB::NetAssignNB(NetAssign_*lv, NetExpr*rv)
: NetAssignBase(lv, rv)
{
}

NetAssignNB::~NetAssignNB()
{
}