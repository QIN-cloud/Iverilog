#include "config.h"

/*
 * This functor scans the design looking for dangling objects and
 * excess local signals. These deletions are not necessarily required
 * for proper functioning of anything, but they can clean up the
 * appearance of design files that are generated.
 */
#include "functor.h"
#include "netlist.h"
#include "compiler.h"

class nodangle_f  : public functor_t {
    public:
      void event(Design*des, NetEvent*ev);
      void signal(Design*des, NetNet*sig);

      unsigned count_;
      unsigned stotal, etotal;
};

void nodangle_f::event(Design*des, NetEvent*ev)
{
	/* If there are no references to this event, then go right
	   ahead and delete in. There is no use looking further at
	   it. */
      if ((ev->nwait() + ev->ntrig() + ev->nexpr()) == 0) {
	    delete ev;
	    etotal += 1;
	    return;
      }

	/* Try to remove duplicate probes from the event. */
      for (unsigned idx = 0 ;  idx < ev->nprobe() ;  idx += 1) {
	    unsigned jdx = idx + 1;
	    while (jdx < ev->nprobe()) {
		  NetEvProbe*ip = ev->probe(idx);
		  NetEvProbe*jp = ev->probe(jdx);

		  if (ip->edge() != jp->edge()) {
			jdx += 1;
			continue;
		  }

		  bool fully_connected = true;
		  for (unsigned jpin = 0; jpin < jp->pin_count(); jpin += 1) {
			unsigned ipin = 0;
			bool connected_flag = false;
			for (ipin = 0 ; ipin < ip->pin_count(); ipin += 1)
			      if (connected(ip->pin(ipin), jp->pin(jpin))) {
				    connected_flag = true;
				    break;
			      }

			if (!connected_flag) {
			      fully_connected = false;
			      break;
			}
		  }

		  if (fully_connected) {
			delete jp;
		  } else {
			jdx += 1;
		  }
	    }
      }

	/* Try to find all the events that are similar to me, and
	   replace their references with references to me. */
      list<NetEvent*> match;
      ev->find_similar_event(match);
      for (list<NetEvent*>::iterator idx = match.begin()
		 ; idx != match.end() ;  idx ++) {

	    NetEvent*tmp = *idx;
	    assert(tmp != ev);
	    tmp ->replace_event(ev);
      }
}

void nodangle_f::signal(Design*des, NetNet*sig)
{
	/* Cannot delete signals referenced in an expression
	   or an l-value. */
      if (sig->get_refs() > 0)
	    return;

	/* Cannot delete the ports of tasks or functions. There are
	   too many places where they are referenced. */
      if ((sig->port_type() != NetNet::NOT_A_PORT)
	  && (sig->scope()->type() == NetScope::TASK))
	    return;

      if ((sig->port_type() != NetNet::NOT_A_PORT)
	  && (sig->scope()->type() == NetScope::FUNC))
	    return;

	  /* Can't delete ports of cells. */
      if ((sig->port_type() != NetNet::NOT_A_PORT)
		  && (sig->scope()->attribute("ivl_synthesis_cell") != verinum()))
		  return;

	/* Check to see if the signal is completely unconnected. If
	   all the bits are unlinked, then delete it. */
      bool linked_flag = false;
      for (unsigned idx =  0 ;  idx < sig->pin_count() ;  idx += 1)
	    if (sig->pin(idx).is_linked()) {
		  linked_flag = true;
		  break;
	    }

      if (! linked_flag) {
	    delete sig;
	    stotal += 1;
	    return;
      }

	/* The remaining things can only be done to synthesized
	   signals, not ones that appear in the original Verilog. */
      if (! sig->local_flag())
	    return;

	/* Check to see if there is some significant signal connected
	   to every pin of this signal. */
      unsigned significant_flags = 0;
      for (idx = 0 ;  idx < sig->pin_count() ;  idx += 1) {
	    Nexus*nex = sig->pin(idx).nexus();

	    for (Link*cur = nex->first_nlink()
		       ; cur ;  cur = cur->next_nlink()) {

		  if (cur == &sig->pin(idx))
			continue;

		  NetNet*cursig = dynamic_cast<NetNet*>(cur->get_obj());
		  if (cursig == 0)
			continue;

		  if (cursig->local_flag())
			continue;

		  significant_flags += 1;
		  break;
	    }

	    if (significant_flags <= idx)
		  break;
      }

	/* If every pin is connected to another significant signal,
	   then I can delete this one. */
      if (significant_flags == sig->pin_count()) {
	    count_ += 1;
	    delete sig;
	    stotal += 1;
      }
}

void nodangle(Design*des)
{
      nodangle_f fun;
      unsigned count_iterations = 0;
      fun.stotal = 0;
      fun.etotal = 0;

      do {
	    fun.count_ = 0;
	    des->functor(&fun);
	    count_iterations += 1;

	    if (verbose_flag) {
		  cout << " ... " << count_iterations << " iterations"
		       << " deleted " << fun.stotal << " dangling signals"
		       << " and " << fun.etotal << " events."
		       << " (count=" << fun.count_ << ")" << endl;
	    }

      } while (fun.count_ > 0);

}