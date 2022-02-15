#include "config.h"

#include "functor.h"
#include "netlist.h"
#include <assert.h>

/*
 * Most process statements are not roots of synchronous logic.
 */
bool NetProc::is_synchronous()
{
      return false;
}

bool NetEvWait::is_synchronous()
{
      for (unsigned idx = 0 ;  idx < nevents_ ;  idx += 1) {
	    NetEvent*ev = events_[idx];

	    if (ev->nprobe() == 0)
		  return false;

	    for (unsigned pdx = 0 ;  pdx < ev->nprobe() ;  pdx += 1) {
		  NetEvProbe*pr = ev->probe(pdx);

		    /* No level sensitive clocks. */
		  if (pr->edge() == NetEvProbe::ANYEDGE)
			return false;
	    }

      }

	/* So we know that there is a clock source. Check that the
	   input to the storage is asynchronous. */
      return true; //statement_->is_asynchronous();
}

bool NetProcTop::is_synchronous()
{
      if (type_ == NetProcTop::KINITIAL)
	    return false;

      return statement_->is_synchronous();
}