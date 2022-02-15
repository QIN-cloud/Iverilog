#include "config.h"

#include <iostream>
#include <cassert>
#include <typeinfo>
#include "netlist.h"
#include "netmisc.h"

NexusSet* NetExpr::nex_input()
{
      cerr << get_line()
	   << ": internal error: nex_input not implemented: "
	   << *this << endl;
      return 0;
}

NexusSet* NetProc::nex_input()
{
      cerr << get_line()
	   << ": internal error: NetProc::nex_input not implemented"
	   << endl;
      return 0;
}

NexusSet* NetEBinary::nex_input()
{
      NexusSet*result = left_->nex_input();
      NexusSet*tmp = right_->nex_input();
      result->add(*tmp);
      delete tmp;
      return result;
}

NexusSet* NetEBitSel::nex_input()
{
      NexusSet*result = sig_->nex_input();
      NexusSet*tmp = idx_->nex_input();
      result->add(*tmp);
      delete tmp;
      return result;
}

NexusSet* NetEConcat::nex_input()
{
      NexusSet*result = parms_[0]->nex_input();
      for (unsigned idx = 1 ;  idx < parms_.count() ;  idx += 1) {
	    NexusSet*tmp = parms_[idx]->nex_input();
	    result->add(*tmp);
	    delete tmp;
      }
      return result;
}

/*
 * A constant has not inputs, so always return an empty set.
 */
NexusSet* NetEConst::nex_input()
{
      return new NexusSet;
}

NexusSet* NetECReal::nex_input()
{
      return new NexusSet;
}

NexusSet* NetEMemory::nex_input()
{
      NexusSet*result = idx_->nex_input();
      return result;
}

/*
 * A parameter by definition has no inputs. It represents a constant
 * value, even if that value is a constant expression.
 */
NexusSet* NetEParam::nex_input()
{
      return new NexusSet;
}

NexusSet* NetEEvent::nex_input()
{
      return new NexusSet;
}

NexusSet* NetEScope::nex_input()
{
      return new NexusSet;
}

NexusSet* NetESelect::nex_input()
{
      NexusSet*result = base_? base_->nex_input() : new NexusSet();
      NexusSet*tmp = expr_->nex_input();
      result->add(*tmp);
      delete tmp;
      return result;
}

NexusSet* NetESFunc::nex_input()
{
      if (nparms_ == 0)
	    return new NexusSet;

      NexusSet*result = parms_[0]->nex_input();
      for (unsigned idx = 1 ;  idx < nparms_ ;  idx += 1) {
	    NexusSet*tmp = parms_[idx]->nex_input();
	    result->add(*tmp);
	    delete tmp;
      }
      return result;
}

NexusSet* NetESignal::nex_input()
{
      NexusSet*result = new NexusSet;
      for (unsigned idx = 0 ;  idx < net_->pin_count() ;  idx += 1)
	    result->add(net_->pin(idx).nexus());

      return result;
}

NexusSet* NetETernary::nex_input()
{
      NexusSet*tmp;
      NexusSet*result = cond_->nex_input();

      tmp = true_val_->nex_input();
      result->add(*tmp);
      delete tmp;

      tmp = false_val_->nex_input();
      result->add(*tmp);
      delete tmp;

      return result;
}

NexusSet* NetEUFunc::nex_input()
{
      NexusSet*result = new NexusSet;
      for (unsigned idx = 0 ;  idx < parms_.count() ;  idx += 1) {
	    NexusSet*tmp = parms_[idx]->nex_input();
	    result->add(*tmp);
	    delete tmp;
      }

      return result;
}

NexusSet* NetEUnary::nex_input()
{
      return expr_->nex_input();
}

NexusSet* NetEVariable::nex_input()
{
      return new NexusSet;
}

NexusSet* NetAssignBase::nex_input()
{
      NexusSet*result = rval_->nex_input();
      return result;
}

/*
 * The nex_input of a begin/end block is the NexusSet of bits that the
 * block reads from outside the block. That means it is the union of
 * the nex_input for all the substatements.
 *
 * The input set for a sequential set is not exactly the union of the
 * input sets because there is the possibility of intermediate values,
 * that don't deserve to be in the input set. To wit:
 *
 *      begin
 *         t = a + b;
 *         c = ~t;
 *      end
 *
 * In this example, "t" should not be in the input set because it is
 * used by the sequence as a temporary value.
 */
NexusSet* NetBlock::nex_input()
{
      if (last_ == 0)
	    return new NexusSet;

      if (type_ == PARA) {
	    cerr << get_line() << ": internal error: Sorry, "
		 << "I don't know how to synthesize fork/join blocks."
		 << endl;
	    return 0;
      }

      NetProc*cur = last_->next_;
	/* This is the accumulated input set. */
      NexusSet*result = new NexusSet;
	/* This is an accumulated output set. */
      NexusSet*prev = new NexusSet;

      do {
	      /* Get the inputs for the current statement. */
	    NexusSet*tmp = cur->nex_input();

	      /* Remove from the input set those bits that are outputs
		 from previous statements. They aren't really inputs
		 to the block, just internal intermediate values. */
	    tmp->rem(*prev);

	      /* Add the corrected set to the accumulated input set. */
	    result->add(*tmp);
	    delete tmp;

	      /* Add the current outputs to the accumulated output
		 set, for processing of later statements. */
	    cur->nex_output(*prev);

	    cur = cur->next_;
      } while (cur != last_->next_);

      return result;
}

/*
 * The inputs to a case statement are the inputs to the expression,
 * the inputs to all the guards, and the inputs to all the guarded
 * statements.
 */
NexusSet* NetCase::nex_input()
{
      NexusSet*result = expr_->nex_input();
      if (result == 0)
	    return 0;

      for (unsigned idx = 0 ;  idx < nitems_ ;  idx += 1) {

	    //assert(items_[idx].statement);
		
		  /* Skip cases that have empty statements. */
	    if (items_[idx].statement == 0)
		  continue;

	    NexusSet*tmp = items_[idx].statement->nex_input();
	    assert(tmp);
	    result->add(*tmp);
	    delete tmp;

	      /* Usually, this is the guard expression. The default
		 case is special and is identified by a null
		 guard. The default guard obviously has no input. */
	    if (items_[idx].guard) {
		  tmp = items_[idx].guard->nex_input();
		  assert(tmp);
		  result->add(*tmp);
		  delete tmp;
	    }
      }

      return result;
}

NexusSet* NetCAssign::nex_input()
{
      cerr << get_line() << ": internal warning: NetCAssign::nex_input()"
	   << " not implemented." << endl;
      return new NexusSet;
}

NexusSet* NetCondit::nex_input()
{
      NexusSet*result = expr_->nex_input();
      if (if_ != 0) {
	    NexusSet*tmp = if_->nex_input();
	    result->add(*tmp);
	    delete tmp;
      }

      if (else_ != 0) {
	    NexusSet*tmp = else_->nex_input();
	    result->add(*tmp);
	    delete tmp;
      }

      return result;
}

NexusSet* NetForce::nex_input()
{
      cerr << get_line() << ": internal warning: NetForce::nex_input()"
	   << " not implemented." << endl;
      return new NexusSet;
}

NexusSet* NetForever::nex_input()
{
      NexusSet*result = statement_->nex_input();
      return result;
}

/*
 * The NetPDelay statement is a statement of the form
 *
 *   #<expr> <statement>
 *
 * The nex_input set is the input set of the <statement>. Do *not*
 * include the input set of the <expr> because it does not affect the
 * result.
 */
NexusSet* NetPDelay::nex_input()
{
      NexusSet*result = statement_->nex_input();
      return result;
}

NexusSet* NetRepeat::nex_input()
{
      NexusSet*result = statement_->nex_input();
      NexusSet*tmp = expr_->nex_input();
      result->add(*tmp);
      delete tmp;
      return result;
}

NexusSet* NetSTask::nex_input()
{
      if (parms_.count() == 0)
	    return new NexusSet;

      NexusSet*result = parms_[0]->nex_input();
      for (unsigned idx = 1 ;  idx < parms_.count() ;  idx += 1) {
	    NexusSet*tmp = parms_[idx]->nex_input();
	    result->add(*tmp);
	    delete tmp;
      }

      return result;
}

/*
 * The NetUTask represents a call to a user defined task. There are no
 * parameters to consider, because the compiler already removed them
 * and converted them to blocking assignments.
 */
NexusSet* NetUTask::nex_input()
{
      return new NexusSet;
}

NexusSet* NetWhile::nex_input()
{
      NexusSet*result = proc_->nex_input();
      NexusSet*tmp = cond_->nex_input();
      result->add(*tmp);
      delete tmp;
      return result;
}