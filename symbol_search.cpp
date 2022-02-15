
#include  "netlist.h"
#include  <assert.h>


/*
 * Search for the hierarchical name.
 */
NetScope*symbol_search(Design*des, NetScope*scope, hname_t path,
		       NetNet*&net,
		       NetMemory*&mem,
		       NetVariable*&var,
		       const NetExpr*&par,
		       NetEvent*&eve)
{
      assert(scope);

	/* Get the tail name of the object we are looking for. */
      char*key = path.remove_tail_name();

	/* Initialize output argument to cleared. */
      net = 0;
      mem = 0;
      var = 0;
      par = 0;
      eve = 0;

	/* If the path has a scope part, then search for the specified
	   scope that we are supposed to search. */
      if (path.peek_name(0))
	    scope = des->find_scope(scope, path);

      while (scope) {
	    if ( (net = scope->find_signal(key)) ) {
		  delete key;
		  return scope;
	    }

	    if ( (mem = scope->find_memory(key)) ) {
		  delete key;
		  return scope;
	    }

	    if ( (var = scope->find_variable(key)) ) {
		  delete key;
		  return scope;
	    }

	    if ( (eve = scope->find_event(key)) ) {
		  delete key;
		  return scope;
	    }

	    if ( (par = scope->get_parameter(key)) ) {
		  delete key;
		  return scope;
	    }

	    if (scope->type() == NetScope::MODULE)
		  scope = 0;
	    else
		  scope = scope->parent();
      }

      delete key;
      return 0;
}
