#include "config.h"
#include "util.h"
#include "PExpr.h"
#include "netlist.h"
#include <iostream>
#include <assert.h>

/*
 * The evaluate_attributes function evaluates the attribute
 * expressions from the map, and returns a table in a form suitable
 * for passing to netlist devices.
 */

attrib_list_t* evaluate_attributes(const map<string,PExpr*>&att,
				   unsigned&natt,
				   const Design*des,
				   const NetScope*scope)
{
      natt = att.size();
      if (natt == 0)
	    return 0;

      attrib_list_t*table = new attrib_list_t [natt];

      unsigned idx = 0;

      typedef map<string,PExpr*>::const_iterator iter_t;
      for (iter_t cur = att.begin() ;  cur != att.end() ;  cur ++, idx++) {
	    table[idx].key = (*cur).first;
	    PExpr*exp = (*cur).second;

	      /* If the attribute value is given in the source, then
		 evaluate it as a constant. If the value is not
		 given, then assume the value is 1. */
	    verinum*tmp;
	    if (exp)
		  tmp = exp->eval_const(des, scope);
	    else
		  tmp = new verinum(1);

	    if (tmp == 0)
		  cerr << "internal error: no result for " << *exp << endl;
	    assert(tmp);

	    table[idx].val = *tmp;
	    delete tmp;
      }

      assert(idx == natt);
      return table;
}