#ifndef __PData_H
#define __PData_H

#include "HName.h"
#include "netlist.h"
#include "LineInfo.h"
#include <string>
#include <map>

class PExpr;

/*
 * The PData object represents declaration of atomic datum such as
 * real and realtime variables. These are variables that cannot be bit
 * or part selected, but can be used in expressions.
 */

class PData : public LineInfo {

    public:
      PData(const hname_t&hname);
      ~PData();

	// Return a hierarchical name.
      const hname_t&name() const;

      void elaborate_scope(Design*des, NetScope*scope) const;

      map<string,PExpr*> attributes;

    private:
      hname_t hname_;

    private:
      PData(const PData&);
      PData& operator= (const PData&);
};

#endif
