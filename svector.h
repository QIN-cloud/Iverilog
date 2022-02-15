#ifndef __svector_H
#define __svector_H

#include "config.h"
#include <string>
#include <assert.h>

using namespace std;

/*
 * This is a way simplified vector class that cannot grow or shrink,
 * and is really only able to handle values. It is intended to be
 * lighter weight then the STL list class.
 */

template <class TYPE> class svector {

    public:
      explicit svector() : nitems_(0), items_(0) { }

      explicit svector(unsigned size) : nitems_(size), items_(new TYPE[size])
	    { for (unsigned idx = 0 ;  idx < size ;  idx += 1);
		  //items_[idx] = 0;
	    }

      svector(const svector<TYPE>&that)
            : nitems_(that.nitems_), items_(new TYPE[nitems_])
	    { for (unsigned idx = 0 ;  idx < that.nitems_ ;  idx += 1)
		    items_[idx] = that[idx];
	    }

      svector(const svector<TYPE>&l, const svector<TYPE>&r)
            : nitems_(l.nitems_ + r.nitems_), items_(new TYPE[nitems_])
	    { unsigned idx;
		  for (idx = 0 ;  idx < l.nitems_ ;  idx += 1)
		    items_[idx] = l[idx];

	      for (idx = 0 ;  idx < r.nitems_ ;  idx += 1)
		    items_[l.nitems_+idx] = r[idx];
	    }

      svector(const svector<TYPE>&l, TYPE r)
            : nitems_(l.nitems_ + 1), items_(new TYPE[nitems_])
	    { for (unsigned idx = 0 ;  idx < l.nitems_ ;  idx += 1)
		    items_[idx] = l[idx];
	      items_[nitems_-1] = r;
	    }

      ~svector() { delete[]items_; }

      svector<TYPE>& operator= (const svector<TYPE>&that)
	    { if (&that == this) return *this;
	      delete[]items_;
	      nitems_ = that.nitems_;
	      items_ = new TYPE[nitems_];
	      for (unsigned idx = 0 ;  idx < nitems_ ;  idx += 1) {
		    items_[idx] = that.items_[idx];
	      }
	      return *this;
	    }

      unsigned count() const { return nitems_; }

      TYPE&operator[] (unsigned idx)
	    { assert(idx < nitems_);
	      return items_[idx];
	    }

      TYPE operator[] (unsigned idx) const
	    { assert(idx < nitems_);
	      return items_[idx];
	    }

    private:
      unsigned nitems_;
      TYPE* items_;

};

/*
 * Override the implementation of the above template for the string
 * type parameter. The initialization to nil works different here.
 */
inline svector<string>::svector<string>(unsigned size)
: nitems_(size), items_(new string[size])
{
}

#endif
