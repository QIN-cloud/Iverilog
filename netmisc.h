#ifndef __netmisc_H
#define __netmisc_H

#include "netlist.h"

/*
 * Search for a symbol using the "start" scope as the starting
 * point. If the path includes a scope part, then locate the
 * scope first.
 *
 * The return value is the scope where the symbol was found.
 * If the symbol was not found, return 0. The output arguments
 * get 0 except for the pointer to the object that represents
 * the located symbol.
 */
extern NetScope* symbol_search(Design*des, NetScope*start, hname_t path,
			       NetNet*&net,       /* net/reg */
			       NetMemory*&mem,    /* memory */
			       NetVariable*&var,  /* real/realtime */
			       const NetExpr*&par,/* parameter */
			       NetEvent*&eve       /* named event */);

/*
 * This function transforms an expression by padding the high bits
 * with V0 until the expression has the desired width. This may mean
 * not transforming the expression at all, if it is already wide
 * enough.
 */
extern NetExpr*pad_to_width(NetExpr*expr, unsigned wid);
extern NetNet*pad_to_width(Design*des, NetNet*n, unsigned w);

/*
 * This function takes as input a NetNet signal and adds a constant
 * value to it. If the val is 0, then simply return sig. Otherwise,
 * return a new NetNet value that is the output of an addition.
 */
extern NetNet*add_to_net(Design*des, NetNet*sig, long val);

/*
 * In some cases the lval is accessible as a pointer to the head of
 * a list of NetAssign_ objects. This function returns the width of
 * the l-value represented by this list.
 */
extern unsigned count_lval_width(const class NetAssign_*first);

/*
 * This function elaborates an expression, and tries to evaluate it
 * right away. It is useful for those places where the user is
 * supposed to supply a constant expression.
 */
class PExpr;
extern NetExpr* elab_and_eval(Design*des, NetScope*scope, const PExpr*pe);

#endif
