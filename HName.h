#ifndef __HName_H
#define __HName_H

#include <iostream>

using namespace std;

/*
 * This class represents a Verilog hierarchical name. A hierarchical
 * name is an ordered list of simple names.
 */

class hname_t {

    public:
      hname_t ();
      explicit hname_t (const char*text);
      hname_t (const hname_t&that);
      ~hname_t();

	// This method adds a name to the end of the hierarchical
	// path. This becomes a new base name.
      void append(const char*text);

	// This method adds a name to the *front* of the hierarchical
	// path. The base name remains the same, unless this is the
	// only component.
      void prepend(const char*text);

	// This method removes the tail name from the hierarchy, and
	// returns a pointer to that tail name. That tail name now
	// must be removed by the caller.
      char* remove_tail_name();

	// Return the given component in the hierarchical name. If the
	// idx is too large, return 0.
      const char*peek_name(unsigned idx) const;
      const char*peek_tail_name() const;

	// Return the number of components in the hierarchical
	// name. If this is a simple name, this will return 1.
      unsigned component_count() const;

      //friend ostream& operator<< (ostream&out, const hname_t&that);

    //private:
      union {
	    char**array_;
	    char* item_;
      };
      unsigned count_;

    private: // not implemented
      hname_t& operator= (const hname_t&);
};

extern bool operator <  (const hname_t&, const hname_t&);
extern bool operator == (const hname_t&, const hname_t&);
ostream& operator<< (ostream&out, const hname_t&that);

#endif
