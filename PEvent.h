#ifndef __PEvent_H
#define __PEvent_H

#include "LineInfo.h"
#include <string>

class Design;
class NetScope;

/*
 * The PEvent class represents event objects. These are things that
 * are declared in Verilog as ``event foo;'' The name passed to the
 * constructure is the "foo" part of the declaration.
 */
class PEvent : public LineInfo {

    public:
      explicit PEvent(const string&name);
      ~PEvent();

      string name() const;

	  void elaborate_scope(Design*des, NetScope*scope) const;

    private:
      string name_;

    private: // not implemented
      PEvent(const PEvent&);
      PEvent& operator= (const PEvent&);
};

#endif
