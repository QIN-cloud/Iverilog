#include "config.h"
#include <iostream>

#include "netlist.h"
#include <sstream>
#include <string>
#include <typeinfo>
#ifdef HAVE_MALLOC_H
#include <malloc.h>
#endif

void connect(Nexus*l, Link&r)
{
      assert(l);
      assert(r.nexus_);

      if (l == r.nexus_)
	    return;


      Nexus*tmp = r.nexus_;
      while (Link*cur = tmp->list_) {
	    tmp->list_ = cur->next_;
	    cur->nexus_ = 0;
	    cur->next_  = 0;
	    l->relink(cur);
      }

      l->driven_ = Nexus::NO_GUESS;

      assert(tmp->list_ == 0);
      delete tmp;
}

void connect(Link&l, Link&r)
{
      assert(&l != &r);
      connect(l.nexus_, r);
}

Link::Link()
: dir_(PASSIVE), drive0_(STRONG), drive1_(STRONG), init_(verinum::Vx),
  inst_(0), next_(0), nexus_(0)
{
      (new Nexus()) -> relink(this);
}

Link::~Link()
{
      assert(nexus_);
      Nexus*tmp = nexus_;
      nexus_->unlink(this);
      if (tmp->list_ == 0)
	    delete tmp;
}

Nexus* Link::nexus()
{
      return nexus_;
}

const Nexus* Link::nexus() const
{
      return nexus_;
}

void Link::set_dir(DIR d)
{
      dir_ = d;
}

Link::DIR Link::get_dir() const
{
      return dir_;
}

void Link::drive0(Link::strength_t str)
{
      drive0_ = str;
}

void Link::drive1(Link::strength_t str)
{
      drive1_ = str;
}

Link::strength_t Link::drive0() const
{
      return drive0_;
}

Link::strength_t Link::drive1() const
{
      return drive1_;
}

void Link::set_init(verinum::V val)
{
      init_ = val;
}

verinum::V Link::get_init() const
{
      return init_;
}


void Link::cur_link(NetObj*&net, unsigned &pin)
{
      net = node_;
      pin = pin_;
}

void Link::cur_link(const NetObj*&net, unsigned &pin) const
{
      net = node_;
      pin = pin_;
}

void Link::unlink()
{
      assert(nexus_);
      if (! is_linked())
	    return;

      nexus_->unlink(this);
      (new Nexus()) -> relink(this);
}

bool Link::is_equal(const Link&that) const
{
      return (node_ == that.node_) && (pin_ == that.pin_);
}

bool Link::is_linked() const
{
      if (next_)
	    return true;
      if (nexus_->first_nlink() != this)
	    return true;

      return false;
}

bool Link::is_linked(const Link&that) const
{
      return nexus_ == that.nexus_;
}

Link* Link::next_nlink()
{
      return next_;
}

const Link* Link::next_nlink() const
{
      return next_;
}

const NetObj*Link::get_obj() const
{
      return node_;
}

NetObj*Link::get_obj()
{
      return node_;
}

unsigned Link::get_pin() const
{
      return pin_;
}

void Link::set_name(const string&n, unsigned i)
{
      name_ = n;
      inst_ = i;
}

const string& Link::get_name() const
{
      return name_;
}

unsigned Link::get_inst() const
{
      return inst_;
}

Nexus::Nexus()
{
      name_ = 0;
      list_ = 0;
      driven_ = NO_GUESS;
      t_cookie_ = 0;
}

Nexus::~Nexus()
{
      assert(list_ == 0);
      if (name_)
	    delete[]name_;
}

verinum::V Nexus::get_init() const
{
      assert(list_);
      for (Link*cur = list_ ;  cur ;  cur = cur->next_) {
	    if (cur->get_dir() == Link::OUTPUT)
		  return verinum::Vx;

	    if ((cur->get_dir() == Link::PASSIVE)
		&& (cur->get_init() != verinum::Vz))
		  return cur->get_init();
      }

      return verinum::Vz;
}

void Nexus::unlink(Link*that)
{
      if (name_) {
	    delete[] name_;
	    name_ = 0;
      }

	/* If the link I'm removing was a driver for this nexus, then
	   cancel my guess of the driven value. */
      if (that->get_dir() != Link::INPUT)
	    driven_ = NO_GUESS;

      assert(that);
      if (list_ == that) {
	    list_ = that->next_;
	    that->next_ = 0;
	    that->nexus_ = 0;
	    return;
      }

      Link*cur = list_;
      while (cur->next_ != that) {
	    assert(cur->next_);
	    cur = cur->next_;
      }

      cur->next_ = that->next_;
      that->nexus_ = 0;
      that->next_ = 0;
}

void Nexus::relink(Link*that)
{
      if (name_) {
	    delete[] name_;
	    name_ = 0;
      }

	/* If the link I'm adding is a driver for this nexus, then
	   cancel my guess of the driven value. */
      if (that->get_dir() != Link::INPUT)
	    driven_ = NO_GUESS;

      assert(that->nexus_ == 0);
      assert(that->next_ == 0);
      that->next_ = list_;
      that->nexus_ = this;
      list_ = that;
}

Link* Nexus::first_nlink()
{
      return list_;
}

const Link* Nexus::first_nlink() const
{
      return list_;
}

void* Nexus::t_cookie() const
{
      return t_cookie_;
}

void* Nexus::t_cookie(void*val)const
{
      void*tmp = t_cookie_;
      t_cookie_ = val;
      return tmp;
}

const char* Nexus::name() const
{
      if (name_)
	    return name_;

      const NetNet*sig = 0;
      unsigned pin = 0;
      for (const Link*cur = first_nlink()
		 ;  cur  ;  cur = cur->next_nlink()) {

	    const NetNet*cursig = dynamic_cast<const NetNet*>(cur->get_obj());
	    if (cursig == 0)
		  continue;

	    if (sig == 0) {
		  sig = cursig;
		  pin = cur->get_pin();
		  continue;
	    }

	    if ((cursig->pin_count() == 1) && (sig->pin_count() > 1))
		  continue;

	    if ((cursig->pin_count() > 1) && (sig->pin_count() == 1)) {
		  sig = cursig;
		  pin = cur->get_pin();
		  continue;
	    }

	    if (cursig->local_flag() && !sig->local_flag())
		  continue;

	    if (cursig->name() < sig->name())
		  continue;

	    sig = cursig;
	    pin = cur->get_pin();
      }

      if (sig == 0) {
	    const Link*lnk = first_nlink();
	    const NetObj*obj = lnk->get_obj();
	    pin = lnk->get_pin();
	    cerr << "internal error: No signal for nexus of " <<
		  obj->name() << " pin " << pin << "(" <<
		  lnk->get_name() << "<" << lnk->get_inst() << ">)"
		  " type=" << typeid(*obj).name() << "?" << endl;

      }
      assert(sig);
      ostringstream tmp;
      tmp << sig->name();
      if (sig->pin_count() > 1)
	    tmp << "<" << pin << ">";

      const string tmps = tmp.str();
      name_ = new char[strlen(tmps.c_str()) + 1];
      strcpy(name_, tmps.c_str());
      return name_;
}


NexusSet::NexusSet()
{
      items_ = 0;
      nitems_ = 0;
}

NexusSet::~NexusSet()
{
      if (nitems_ > 0) {
	    assert(items_ != 0);
	    delete[] items_;
      } else {
	    assert(items_ == 0);
      }
}

unsigned NexusSet::count() const
{
      return nitems_;
}

void NexusSet::add(Nexus*that)
{
      if (nitems_ == 0) {
	    assert(items_ == 0);
	    items_ = (Nexus**)malloc(sizeof(Nexus*));
	    items_[0] = that;
	    nitems_ = 1;
	    return;
      }

      unsigned ptr = bsearch_(that);
      if ((ptr < nitems_) && (items_[ptr] == that))
	    return;

      items_ = (Nexus**)realloc(items_, (nitems_+1) * sizeof(Nexus*));
      for (unsigned idx = nitems_ ;  idx > ptr ;  idx -= 1)
	    items_[idx] = items_[idx-1];

      items_[ptr] = that;
      nitems_ += 1;
}

void NexusSet::add(const NexusSet&that)
{
      for (unsigned idx = 0 ;  idx < that.nitems_ ;  idx += 1)
	    add(that.items_[idx]);
}

void NexusSet::rem(Nexus*that)
{
      if (nitems_ == 0)
	    return;

      unsigned ptr = bsearch_(that);
      if ((ptr >= nitems_) || (items_[ptr] != that))
	    return;

      if (nitems_ == 1) {
	    free(items_);
	    items_ = 0;
	    nitems_ = 0;
	    return;
      }

      for (unsigned idx = ptr ;  idx < (nitems_-1) ;  idx += 1)
	    items_[idx] = items_[idx+1];

      items_ = (Nexus**)realloc(items_, (nitems_-1) * sizeof(Nexus*));
      nitems_ -= 1;
}

void NexusSet::rem(const NexusSet&that)
{
      for (unsigned idx = 0 ;  idx < that.nitems_ ;  idx += 1)
	    rem(that.items_[idx]);
}

Nexus* NexusSet::operator[] (unsigned idx) const
{
      assert(idx < nitems_);
      return items_[idx];
}

unsigned NexusSet::bsearch_(Nexus*that) const
{
      for (unsigned idx = 0 ;  idx < nitems_ ;  idx += 1) {
	    if (items_[idx] < that)
		  continue;

	    return idx;
      }

      return nitems_;
}

bool NexusSet::contains(const NexusSet&that) const
{
      for (unsigned idx = 0 ;  idx < that.nitems_ ;  idx += 1) {
	    unsigned where = bsearch_(that[idx]);
	    if (where == nitems_)
		  return false;
	    if (items_[where] != that[idx])
		  return false;
      }

      return true;
}

bool NexusSet::intersect(const NexusSet&that) const
{
      for (unsigned idx = 0 ;  idx < that.nitems_ ;  idx += 1) {
	    unsigned where = bsearch_(that[idx]);
	    if (where == nitems_)
		  continue;
	    if (items_[where] == that[idx])
		  return true;
      }

      return false;
}