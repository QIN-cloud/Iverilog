#include "config.h"
#include "PWire.h"
#include <assert.h>

PWire::PWire(const hname_t&n, NetNet::Type t, NetNet::PortType pt)
: hname_(n), type_(t), port_type_(pt), signed_(false), isint_(false),
lidx_(0), ridx_(0)
{
      if (t == NetNet::INTEGER) {
	    type_ = NetNet::REG;
	    signed_ = true;
	    isint_ = true;
      }
}

PWire::PWire(char*n, NetNet::Type t, NetNet::PortType pt)
: hname_(n), type_(t), port_type_(pt), signed_(false), isint_(false),
lidx_(0), ridx_(0)
{
      if (t == NetNet::INTEGER) {
	    type_ = NetNet::REG;
	    signed_ = true;
	    isint_ = true;
      }
}

NetNet::Type PWire::get_wire_type() const
{
      return type_;
}

const hname_t& PWire::path() const
{
      return hname_;
}

bool PWire::set_wire_type(NetNet::Type t)
{
      assert(t != NetNet::IMPLICIT);

      switch (type_) {
	  case NetNet::IMPLICIT:
	    type_ = t;
	    return true;
	  case NetNet::IMPLICIT_REG:
	    if (t == NetNet::REG) { type_ = t; return true; }
	    return false;
	  case NetNet::REG:
	    if (t == NetNet::INTEGER) {
		  isint_ = true;
		  return true;
	    }
	    if (t == NetNet::REG) return true;
	    return false;
	  default:
	    if (type_ != t)
		  return false;
	    else
		  return true;
      }
}

NetNet::PortType PWire::get_port_type() const
{
      return port_type_;
}

bool PWire::set_port_type(NetNet::PortType pt)
{
      assert(pt != NetNet::NOT_A_PORT);
      assert(pt != NetNet::PIMPLICIT);

      switch (port_type_) {
	  case NetNet::PIMPLICIT:
	    port_type_ = pt;
	    return true;

	  case NetNet::NOT_A_PORT:
	    return false;

	  default:
	    if (port_type_ != pt)
		  return false;
	    else
		  return true;
      }
}

void PWire::set_signed(bool flag)
{
      signed_ = flag;
}

bool PWire::get_signed() const
{
      return signed_;
}

bool PWire::get_isint() const
{
      return isint_;
}

void PWire::set_range(PExpr*m, PExpr*l)
{
      msb_ = svector<PExpr*>(msb_,m);
      lsb_ = svector<PExpr*>(lsb_,l);
}

void PWire::set_memory_idx(PExpr*ldx, PExpr*rdx)
{
      assert(lidx_ == 0);
      assert(ridx_ == 0);
      assert(type_ == NetNet::REG);
      lidx_ = ldx;
      ridx_ = rdx;
}

unsigned PWire::getwidth()
{
	return iwid_;
}

long PWire::getlsb() 
{
	return ilsb_;
}

long PWire::getmsb() 
{
	return imsb_;
}