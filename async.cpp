#include "config.h"

#include "functor.h"
#include "netlist.h"
#include <assert.h>

bool NetAssign::is_asynchronous()
{
      return true;
}

bool NetCondit::is_asynchronous()
{
      return false;
}

/*
 * NetEvWait statements come from statements of the form @(...) in the
 * Verilog source. These event waits are considered asynchronous if
 * all of the events in the list are ANYEDGE, and all the inputs to
 * the statement are included in the sensitivity list. If any of the
 * events are posedge or negedge, the statement is synchronous
 * (i.e. an edge-triggered flip-flop) and if any of the inputs are
 * unaccounted for in the sensitivity list then the statement is a
 * latch.
 */
bool NetEvWait::is_asynchronous()
{
	/* The "sense" set contains the set of Nexa that are in the
	   sensitivity list. We also require that the events are all
	   level sensitive, but the nex_async_ method takes care of
	   that test. */
      NexusSet*sense = new NexusSet;
      for (unsigned idx = 0 ;  idx < nevents_ ;  idx += 1) {
	    NexusSet*tmp = event(idx)->nex_async_();
	    if (tmp == 0) {
		  delete sense;
		  return false;
	    }

	    sense->add(*tmp);
	    delete tmp;
      }

      NexusSet*inputs = statement_->nex_input();

      if (! sense->contains(*inputs)) {
	    delete sense;
	    delete inputs;
	    return false;
      }

      delete sense;
      delete inputs;

	  /* If it passes all the other tests, then this statement is
	   asynchronous. */
      return true;
}

bool NetProc::is_asynchronous()
{
      return false;
}

bool NetProcTop::is_asynchronous()
{
      if (type_ == NetProcTop::KINITIAL)
	    return false;

      return statement_->is_asynchronous();
}