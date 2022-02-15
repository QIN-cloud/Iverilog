#include "config.h"

#include "Attrib.h"
#include <assert.h>

Attrib::Attrib()
{
      nlist_ = 0;
      list_ = 0;
}

Attrib::~Attrib()
{
      delete[] list_;
}

#if 0
void Attrib::copy_attributes(const map<string,verinum>&attr)
{
      assert(list_ == 0);

      nlist_ = attr.size();
      list_ = new cell_[nlist_];

      map<string,verinum>::const_iterator idx;
      unsigned jdx;
      for (idx = attr.begin(), jdx = 0 ;  idx != attr.end() ;  idx ++, jdx++) {
	    struct cell_*tmp = list_ + jdx;
	    tmp->key = (*idx).first;
	    tmp->val = (*idx).second;
      }
}
#endif

const verinum& Attrib::attribute(const string&key) const
{
      for (unsigned idx = 0 ;  idx < nlist_ ;  idx += 1) {

	    if (key == list_[idx].key)
		  return list_[idx].val;
      }

      static const verinum null;
      return null;
}

void Attrib::attribute(const string&key, const verinum&value)
{
      unsigned idx;

      for (idx = 0 ; idx < nlist_ ;  idx += 1) {
	    if (key == list_[idx].key) {
		  list_[idx].val = value;
		  return;
	    }
      }

      struct cell_*tmp = new struct cell_[nlist_+1];
      for (idx = 0 ;  idx < nlist_ ;  idx += 1)
	    tmp[idx] = list_[idx];

      tmp[nlist_].key = key;
      tmp[nlist_].val = value;

      nlist_ += 1;
      delete[]list_;
      list_ = tmp;
}

bool Attrib::has_compat_attributes(const Attrib&that) const
{
      unsigned idx;

      for (idx = 0 ;  idx < that.nlist_ ;  idx += 1) {

	    verinum tmp = attribute(that.list_[idx].key);
	    if (tmp != that.list_[idx].val)
		  return false;
      }

      return true;
}

unsigned Attrib::attr_cnt() const
{
      return nlist_;
}

const char* Attrib::attr_key(unsigned idx) const
{
      assert(idx < nlist_);
      return list_[idx].key.c_str();
}

const verinum& Attrib::attr_value(unsigned idx) const
{
      assert(idx < nlist_);
      return list_[idx].val;
}