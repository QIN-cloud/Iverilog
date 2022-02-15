#include "config.h"

#include <iostream>

#include <cassert>
#include <typeinfo>
#include "netlist.h"
#include "netmisc.h"

void NetProc::nex_output(NexusSet&out)
{
      cerr << get_line()
	   << ": internal error: NetProc::nex_output not implemented"
	   << endl;
}

void NetAssignBase::nex_output(NexusSet&out)
{
      if (NetNet*lsig = lval_->sig()) {
	    assert(lval_->more == 0);
	    for (unsigned idx = 0 ;  idx < lval_->lwidth() ;  idx += 1) {
		  unsigned off = lval_->get_loff() + idx;
		  out.add(lsig->pin(off).nexus());
	    }
      } else {
	    /* Quoting from netlist.h comments for class NetMemory:
	     * "This is not a node because memory objects can only be
	     * accessed by behavioral code."
	     */
	    cerr << get_line() << ": internal error: "
		 << "NetAssignBase::nex_output on unsupported lval ";
	    dump_lval(cerr);
	    cerr << endl;
      }
}

void NetBlock::nex_output(NexusSet&out)
{
      if (last_ == 0)
	    return;

      NetProc*cur = last_;
      do {
	    cur = cur->next_;
	    cur->nex_output(out);
      } while (cur != last_);
}

void NetCase::nex_output(NexusSet&out)
{
      for (unsigned idx = 0 ;  idx < nitems_ ;  idx += 1) {

	    assert(items_[idx].statement);
	    items_[idx].statement->nex_output(out);
      }

}

void NetCondit::nex_output(NexusSet&out)
{
      if (if_ != 0)
	    if_->nex_output(out);
      if (else_ != 0)
	    else_->nex_output(out);
}

void NetEvWait::nex_output(NexusSet&out)
{
      assert(statement_);
      statement_->nex_output(out);
}

void NetWhile::nex_output(NexusSet&out)
{
      if (proc_ != 0)
	    proc_->nex_output(out);
}