#include "config.h"
#include "compiler.h"

#include "pform.h"
#include "netlist.h"
#include "netmisc.h"
#include "util.h"

/*
 * This table describes all the return values of various system
 * functions. This table is used to elaborate expressions that are
 * system function calls.
 */
struct sfunc_return_type {
      const char*   name;
      NetExpr::TYPE type;
      unsigned      wid;
      int           signed_flag;
};

static const struct sfunc_return_type sfunc_table[] = {
      { "$realtime",   NetExpr::ET_REAL,    0, 0 },
      { "$bitstoreal", NetExpr::ET_REAL,    0, 0 },
      { "$itor",       NetExpr::ET_REAL,    0, 0 },
      { "$realtobits", NetExpr::ET_VECTOR, 64, 0 },
      { "$time",       NetExpr::ET_VECTOR, 64, 0 },
      { "$stime",      NetExpr::ET_VECTOR, 32, 0 },
      { "$simtime",    NetExpr::ET_VECTOR, 64, 0 },
      { 0,             NetExpr::ET_VECTOR, 32, 0 }
};


NetExpr* PExpr::elaborate_expr(Design*des, NetScope*, bool) const
{
      cerr << get_line() << ": internal error: I do not know how to elaborate"
	   << " expression. " << endl;
      cerr << get_line() << ":               : Expression is: " << *this
	   << endl;
      des->errors += 1;
      return 0;
}

/*
 * Elaborate binary expressions. This involves elaborating the left
 * and right sides, and creating one of a variety of different NetExpr
 * types. 
 */
NetExpr* PEBinary::elaborate_expr(Design*des, NetScope*scope, bool) const
{
      assert(left_);
      assert(right_);

      NetExpr*lp = left_->elaborate_expr(des, scope);
      NetExpr*rp = right_->elaborate_expr(des, scope);
      if ((lp == 0) || (rp == 0)) {
	    delete lp;
	    delete rp;
	    return 0;
      }


	/* If either expression can be evaluated ahead of time, then
	   do so. This can prove helpful later. */
      { NetExpr*tmp;
        tmp = lp->eval_tree();
	if (tmp) {
	      delete lp;
	      lp = tmp;
	}

	tmp = rp->eval_tree();
	if (tmp) {
	      delete rp;
	      rp = tmp;
	}
      }

      NetEBinary*tmp = elaborate_expr_base_(des, lp, rp);
      return tmp;
}

/*
 * This is common elaboration of the operator. It presumes that the
 * operands are elaborated as necessary, and all I need to do is make
 * the correct NetEBinary object and connect the parameters.
 */
NetEBinary* PEBinary::elaborate_expr_base_(Design*des,
					   NetExpr*lp, NetExpr*rp) const
{
      bool flag;
      NetEBinary*tmp;

      switch (op_) {
	  default:
	    tmp = new NetEBinary(op_, lp, rp);
	    tmp->set_line(*this);
	    break;

	  case 'a':
	  case 'o':
	    tmp = new NetEBLogic(op_, lp, rp);
	    tmp->set_line(*this);
	    break;

	  case '*':
	    tmp = new NetEBMult(op_, lp, rp);
	    tmp->set_line(*this);
	    break;

	  case '/':
	  case '%':
	    tmp = new NetEBDiv(op_, lp, rp);
	    tmp->set_line(*this);
	    break;

	  case 'l': // <<
	  case 'r': // >>
	  case 'R': // >>>
	    tmp = new NetEBShift(op_, lp, rp);
	    tmp->set_line(*this);
	    break;

	  case '^':
	  case '&':
	  case '|':
	  case 'O': // NOR (~|)
	  case 'A': // NAND (~&)
	  case 'X':
	    tmp = new NetEBBits(op_, lp, rp);
	    tmp->set_line(*this);
	    break;

	  case '+':
	  case '-':
	    tmp = new NetEBAdd(op_, lp, rp);
	    tmp->set_line(*this);
	    break;

	  case 'e': /* == */
	  case 'E': /* === */
	  case 'n': /* != */
	  case 'N': /* !== */
	    if (dynamic_cast<NetEConst*>(rp)
		&& (lp->expr_width() > rp->expr_width()))
		  rp->set_width(lp->expr_width());

	    if (dynamic_cast<NetEConst*>(lp)
		&& (lp->expr_width() < rp->expr_width()))
		  lp->set_width(rp->expr_width());

	      /* from here, handle this like other compares. */
	  case 'L': /* <= */
	  case 'G': /* >= */
	  case '<':
	  case '>':
	    tmp = new NetEBComp(op_, lp, rp);
	    tmp->set_line(*this);
	    flag = tmp->set_width(1);
	    if (flag == false) {
		  cerr << get_line() << ": internal error: "
			"expression bit width of comparison != 1." << endl;
		  des->errors += 1;
	    }
	    break;
      }

      return tmp;
}

/*
 * Given a call to a system function, generate the proper expression
 * nodes to represent the call in the netlist. Since we don't support
 * size_tf functions, make assumptions about widths based on some
 * known function names.
 */
NetExpr* PECallFunction::elaborate_sfunc_(Design*des, NetScope*scope) const
{

	/* Catch the special case that the system function is the
	   $signed function. This function is special, in that it does
	   not lead to executable code but takes the single parameter
	   and makes it into a signed expression. No bits are changed,
	   it just changes the interpretation. */
      if (strcmp(path_.peek_name(0), "$signed") == 0) {
	    if ((parms_.count() != 1) || (parms_[0] == 0)) {
		  cerr << get_line() << ": error: The $signed() function "
		       << "takes exactly one(1) argument." << endl;
		  des->errors += 1;
		  return 0;
	    }

	    PExpr*expr = parms_[0];
	    NetExpr*sub = expr->elaborate_expr(des, scope, true);
	    sub->cast_signed(true);
	    return sub;
      }

	/* Interpret the internal $sizeof system function to return
	   the bit width of the sub-expression. The value of the
	   sub-expression is not used, so the expression itself can be
	   deleted. */
      if ((strcmp(path_.peek_name(0), "$sizeof") == 0)
	  || (strcmp(path_.peek_name(0), "$bits") == 0)) {
	    if ((parms_.count() != 1) || (parms_[0] == 0)) {
		  cerr << get_line() << ": error: The $bits() function "
		       << "takes exactly one(1) argument." << endl;
		  des->errors += 1;
		  return 0;
	    }

	    if (strcmp(path_.peek_name(0), "$sizeof") == 0)
		  cerr << get_line() << ": warning: $sizeof is deprecated."
		       << " Use $bits() instead." << endl;

	    PExpr*expr = parms_[0];
	    NetExpr*sub = expr->elaborate_expr(des, scope, true);
	    verinum val (sub->expr_width(), 8*sizeof(unsigned));
	    delete sub;

	    sub = new NetEConst(val);
	    sub->set_line(*this);

	    return sub;
      }

	/* Get the return type of the system function by looking it up
	   in the sfunc_table. */
      unsigned sfunc_idx;
      for (sfunc_idx = 0 ;  sfunc_table[sfunc_idx].name ;  sfunc_idx += 1)
	    if (strcmp(path_.peek_name(0), sfunc_table[sfunc_idx].name) == 0)
		  break;

      NetExpr::TYPE sfunc_type = sfunc_table[sfunc_idx].type;
      unsigned wid = sfunc_table[sfunc_idx].wid;


	/* How many parameters are there? The Verilog language allows
	   empty parameters in certain contexts, so the parser will
	   allow things like func(1,,3). It will also cause func() to
	   be interpreted as a single empty parameter.

	   Functions cannot really take empty parameters, but the
	   case ``func()'' is the same as no parameters at all. So
	   catch that special case here. */
      unsigned nparms = parms_.count();
      if ((nparms == 1) && (parms_[0] == 0))
	    nparms = 0;

      NetESFunc*fun = new NetESFunc(path_.peek_name(0), sfunc_type,
				    wid, nparms);

	/* Now run through the expected parameters. If we find that
	   there are missing parameters, print an error message.

	   While we're at it, try to evaluate the function parameter
	   expression as much as possible, and use the reduced
	   expression if one is created. */

      unsigned missing_parms = 0;
      for (unsigned idx = 0 ;  idx < nparms ;  idx += 1) {
	    PExpr*expr = parms_[idx];
	    if (expr) {
		  NetExpr*tmp1 = expr->elaborate_expr(des, scope, true);
		  if (NetExpr*tmp2 = tmp1->eval_tree()) {
			delete tmp1;
			fun->parm(idx, tmp2);
		  } else {
			fun->parm(idx, tmp1);
		  }

	    } else {
		  missing_parms += 1;
		  fun->parm(idx, 0);
	    }
      }

      if (missing_parms > 0) {
	    cerr << get_line() << ": error: The function "
		 << path_.peek_name(0)
		 << " has been called with empty parameters." << endl;
	    cerr << get_line() << ":      : Verilog doesn't allow "
		 << "passing empty parameters to functions." << endl;
	    des->errors += 1;
      }

      return fun;
}

NetExpr* PECallFunction::elaborate_expr(Design*des, NetScope*scope, bool) const
{
      if (path_.peek_name(0)[0] == '$')
	    return elaborate_sfunc_(des, scope);

      NetFuncDef*def = des->find_function(scope, path_);
      if (def == 0) {
	    cerr << get_line() << ": error: No function " << path_ <<
		  " in this context (" << scope->name() << ")." << endl;
	    des->errors += 1;
	    return 0;
      }
      assert(def);

      NetScope*dscope = def->scope();
      assert(dscope);

      if (! check_call_matches_definition_(des, dscope))
	    return 0;

      unsigned parms_count = parms_.count();
      if ((parms_count == 1) && (parms_[0] == 0))
	    parms_count = 0;

      

      svector<NetExpr*> parms (parms_count);

	/* Elaborate the input expressions for the function. This is
	   done in the scope of the function call, and not the scope
	   of the function being called. The scope of the called
	   function is elaborated when the definition is elaborated. */

      unsigned missing_parms = 0;
      for (unsigned idx = 0 ;  idx < parms.count() ;  idx += 1) {
	    PExpr*tmp = parms_[idx];
	    if (tmp) {
		  parms[idx] = tmp->elaborate_expr(des, scope);

	    } else {
		  missing_parms += 1;
		  parms[idx] = 0;
	    }
      }

      if (missing_parms > 0) {
	    cerr << get_line() << ": error: The function " << path_
		 << " has been called with empty parameters." << endl;
	    cerr << get_line() << ":      : Verilog doesn't allow "
		 << "passing empty parameters to functions." << endl;
	    des->errors += 1;
      }


	/* Look for the return value signal for the called
	   function. This return value is a magic signal in the scope
	   of the function, that has the name of the function. The
	   function code assigns to this signal to return a value.

	   dscope, in this case, is the scope of the function, so the
	   return value is the name within that scope. */

      NetNet*res = dscope->find_signal(dscope->basename());
      if (res == 0) {
	    cerr << get_line() << ": internal error: Unable to locate "
		  "function return value for " << path_ << " in " <<
		  def->name() << "." << endl;
	    des->errors += 1;
	    return 0;
      }

      assert(res);
      NetESignal*eres = new NetESignal(res);
      assert(eres);
      NetEUFunc*func = new NetEUFunc(dscope, eres, parms);
      return func;
}


NetExpr* PEConcat::elaborate_expr(Design*des, NetScope*scope, bool) const
{
      NetExpr* repeat = 0;

	/* If there is a repeat expression, then evaluate the constant
	   value and set the repeat count. */
      if (repeat_) {
	    NetExpr*tmp = elab_and_eval(des, scope, repeat_);
	    assert(tmp);
	    NetEConst*rep = dynamic_cast<NetEConst*>(tmp);

	    if (rep == 0) {
		  cerr << get_line() << ": error: "
			"concatenation repeat expression cannot be evaluated."
		       << endl;
		  cerr << get_line() << ":      : The expression is: "
		       << *tmp << endl;
		  des->errors += 1;
	    }

	    repeat = rep;
      }

	/* Make the empty concat expression. */
      NetEConcat*tmp = new NetEConcat(parms_.count(), repeat);
      tmp->set_line(*this);

      unsigned wid_sum = 0;

	/* Elaborate all the parameters and attach them to the concat node. */
      for (unsigned idx = 0 ;  idx < parms_.count() ;  idx += 1) {
	    assert(parms_[idx]);
	    NetExpr*ex = elab_and_eval(des, scope, parms_[idx]);
	    if (ex == 0) continue;

	    ex->set_line(*parms_[idx]);

	    if (! ex->has_width()) {
		  cerr << ex->get_line() << ": error: operand of "
		       << "concatenation has indefinite width: "
		       << *ex << endl;
		  des->errors += 1;
	    }

	    wid_sum += ex->expr_width();
	    tmp->seti(idx, ex);
      }

      tmp->set_width(wid_sum * tmp->repeat());

      return tmp;
}

NetExpr* PEFNumber::elaborate_expr(Design*des, NetScope*scope, bool) const
{
      NetECReal*tmp = new NetECReal(*value_);
      tmp->set_line(*this);
      return tmp;
}

/*
 * Elaborate an identifier in an expression. The identifier can be a
 * parameter name, a signal name or a memory name. It can also be a
 * scope name (Return a NetEScope) but only certain callers can use
 * scope names. However, we still support it here.
 *
 * Function names are not handled here, they are detected by the
 * parser and are elaborated by PECallFunction.
 *
 * The signal name may be escaped, but that affects nothing here.
 */
NetExpr* PEIdent::elaborate_expr(Design*des, NetScope*scope,
				 bool sys_task_arg) const
{
      assert(scope);

      NetNet*       net = 0;
      NetMemory*    mem = 0;
      NetVariable*  var = 0;
      const NetExpr*par = 0;
      NetEvent*     eve = 0;

      NetScope*found_in = symbol_search(des, scope, path_,
					net, mem, var, par, eve);

	// If the identifier name is a parameter name, then return
	// a reference to the parameter expression.
      if (par != 0) {
	    NetExpr*tmp;

	    tmp = par->dup_expr();

	    if (msb_ && lsb_) {
		    /* If the parameter has a part select, we support
		       it by pulling the right bits out and making a
		       sized unsigned constant. This code assumes the
		       lsb of a parameter is 0 and the msb is the
		       width of the parameter. */

		  verinum*lsn = lsb_->eval_const(des, scope);
		  verinum*msn = msb_->eval_const(des, scope);
		  if ((lsn == 0) || (msn == 0)) {
			cerr << get_line() << ": error: "
			      "Part select expressions must be "
			      "constant expressions." << endl;
			des->errors += 1;
			return 0;
		  }

		  long lsb = lsn->as_long();
		  long msb = msn->as_long();
		  if ((lsb < 0) || (msb < lsb)) {
			cerr << get_line() << ": error: invalid part "
			     << "select: " << path_
			     << "["<<msb<<":"<<lsb<<"]" << endl;
			des->errors += 1;
			return 0;
		  }
		  unsigned long ulsb=lsb;
		  unsigned long umsb=msb;

		  NetEConst*le = dynamic_cast<NetEConst*>(tmp);
		  assert(le);

		  verinum result (verinum::V0, msb-lsb+1, true);
		  verinum exl = le->value();

		    /* Pull the bits from the parameter, one at a
		       time. If the bit is within the range, simply
		       copy it to the result. If the bit is outside
		       the range, we sign extend signed unsized
		       numbers, zero extend unsigned unsigned numbers,
		       and X extend sized numbers. */
		  for (unsigned long idx = ulsb ;  idx <= umsb ;  idx += 1) {
			if (idx < exl.len())
			      result.set(idx-lsb, exl.get(idx));
		        else if (exl.is_string())
			      result.set(idx-lsb, verinum::V0);
		        else if (exl.has_len())
			      result.set(idx-lsb, verinum::Vx);
		        else if (exl.has_sign())
			      result.set(idx-lsb, exl.get(exl.len()-1));
		        else
			      result.set(idx-lsb, verinum::V0);
		  }

		    /* If the input is a string, and the part select
		       is working on byte boundaries, then the result
		       can be made into a string. */
		  if (exl.is_string()
		      && (lsb%8 == 0)
		      && (result.len()%8 == 0))
		      result = verinum(result.as_string());

		  delete tmp;
		  tmp = new NetEConst(result);

	    } else if (msb_) {
		    /* Handle the case where a parameter has a bit
		       select attached to it. Generate a NetESelect
		       object to select the bit as desired. */
		  NetExpr*mtmp = msb_->elaborate_expr(des, scope);
		  if (! dynamic_cast<NetEConst*>(mtmp)) {
			NetExpr*re = mtmp->eval_tree();
			if (re) {
			      delete mtmp;
			      mtmp = re;
			}
		  }

		    /* Let's first try to get constant values for both
		       the parameter and the bit select. If they are
		       both constant, then evaluate the bit select and
		       return instead a single-bit constant. */

		  NetEConst*le = dynamic_cast<NetEConst*>(tmp);
		  NetEConst*re = dynamic_cast<NetEConst*>(mtmp);
		  if (le && re) {

			verinum lv = le->value();
			verinum rv = re->value();
			verinum::V rb = verinum::Vx;

			long ridx = rv.as_long();
			if ((ridx >= 0) && ((unsigned long) ridx < lv.len())) {
			      rb = lv[ridx];

			} else if ((ridx >= 0) && (!lv.has_len())) {
			      if (lv.has_sign())
				    rb = lv[lv.len()-1];
			      else
				    rb = verinum::V0;
			}

			NetEConst*re = new NetEConst(verinum(rb, 1));
			delete tmp;
			delete mtmp;
			tmp = re;

		  } else {

			NetESelect*stmp = new NetESelect(tmp, mtmp, 1);
			tmp->set_line(*this);
			tmp = stmp;
		  }

	    } else {
		    /* No bit or part select. Make the constant into a
		       NetEConstParam if possible. */
		  NetEConst*ctmp = dynamic_cast<NetEConst*>(tmp);
		  if (ctmp != 0) {
			const char*name
			     = lex_strings.add(path_.peek_tail_name());
			NetEConstParam*ptmp
			     = new NetEConstParam(found_in, name, ctmp->value());
			delete tmp;
			tmp = ptmp;
		  }
	    }

	    tmp->set_line(*this);
	    return tmp;
      }

	// If the identifier names a signal (a register or wire)
	// then create a NetESignal node to handle it.
      if (net != 0) {

	      // If this is a part select of a signal, then make a new
	      // temporary signal that is connected to just the
	      // selected bits. The lsb_ and msb_ expressions are from
	      // the foo[msb:lsb] expression in the original.
	    if (lsb_) {
		  assert(msb_);
		  verinum*lsn = lsb_->eval_const(des, scope);
		  verinum*msn = msb_->eval_const(des, scope);
		  if ((lsn == 0) || (msn == 0)) {
			cerr << get_line() << ": error: "
			      "Part select expressions must be "
			      "constant expressions." << endl;
			des->errors += 1;
			return 0;
		  }

		  assert(lsn);
		  assert(msn);

		    /* The indices of part selects are signed
		       integers, so allow negative values. However,
		       the width that they represent is
		       unsigned. Remember that any order is possible,
		       i.e., [1:0], [-4,6], etc. */

		  long lsv = lsn->as_long();
		  long msv = msn->as_long();
		  unsigned long wid = 1 + ((msv>lsv)? (msv-lsv) : (lsv-msv));
		  if (wid > net->pin_count()) {
			cerr << get_line() << ": error: part select ["
			     << msv << ":" << lsv << "] out of range."
			     << endl;
			des->errors += 1;
			delete lsn;
			delete msn;
			return 0;
		  }
		  assert(wid <= net->pin_count());

		  if (net->sb_to_idx(msv) < net->sb_to_idx(lsv)) {
			cerr << get_line() << ": error: part select ["
			     << msv << ":" << lsv << "] out of order."
			     << endl;
			des->errors += 1;
			delete lsn;
			delete msn;
			return 0;
		  }


		  if (net->sb_to_idx(msv) >= net->pin_count()) {
			cerr << get_line() << ": error: part select ["
			     << msv << ":" << lsv << "] out of range."
			     << endl;
			des->errors += 1;
			delete lsn;
			delete msn;
			return 0;
		  }

		  NetESignal*tmp = new NetESignal(net,
						  net->sb_to_idx(msv),
						  net->sb_to_idx(lsv));
		  tmp->set_line(*this);

		  return tmp;
	    }

	      // If the bit select is constant, then treat it similar
	      // to the part select, so that I save the effort of
	      // making a mux part in the netlist.
	    verinum*msn;
	    if (msb_ && (msn = msb_->eval_const(des, scope))) {
		  assert(idx_ == 0);
		  long msv = msn->as_long();//unsigned long msv = msn->as_ulong();
		  unsigned idx = net->sb_to_idx(msv);

		  if (idx >= net->pin_count()) {
			  /* The bit select is out of range of the
			     vector. This is legal, but returns a
			     constant 1'bx value. */
			verinum x (verinum::Vx);
			NetEConst*tmp = new NetEConst(x);
			tmp->set_line(*this);

			cerr << get_line() << ": warning: Bit select ["
			     << msv << "] out of range of vector "
			     << net->name() << "[" << net->msb()
			     << ":" << net->lsb() << "]." << endl;
			cerr << get_line() << ":        : Replacing "
			     << "expression with a constant 1'bx." << endl;
			delete msn;
			return tmp;
		  }

		  NetESignal*tmp = new NetESignal(net, idx, idx);
		  tmp->set_line(*this);

		  return tmp;
	    }

	    NetESignal*node = new NetESignal(net);
	    assert(idx_ == 0);

	      // Non-constant bit select? punt and make a subsignal
	      // device to mux the bit in the net.
	    if (msb_) {
		  NetExpr*ex = msb_->elaborate_expr(des, scope);
		  NetEBitSel*ss = new NetEBitSel(node, ex);
		  ss->set_line(*this);
		  return ss;
	    }

	      // All else fails, return the signal itself as the
	      // expression.
	    assert(msb_ == 0);
	    return node;
      }

	// If the identifier names a memory, then this is a
	// memory reference and I must generate a NetEMemory
	// object to handle it.
      if (mem != 0) {
	    if (msb_ == 0) {

		    // If this memory is an argument to a system task,
		    // then it is OK for it to not have an index.
		  if (sys_task_arg) {
			NetEMemory*node = new NetEMemory(mem);
			node->set_line(*this);
			return node;
		  }

		    // If it is not a simple system task argument,
		    // this a missing index is an error.
		  cerr << get_line() << ": error: memory " << mem->name()
		       << " needs an index in this context." << endl;
		  des->errors += 1;
		  return 0;
	    }

	    assert(msb_ != 0);
	    if (lsb_) {
		  cerr << get_line() << ": error: part select of a memory: "
		       << mem->name() << endl;
		  des->errors += 1;
		  return 0;
	    }

	    assert(lsb_ == 0);
	    assert(idx_ == 0);
	    NetExpr*i = msb_->elaborate_expr(des, scope);
	    if (msb_ && i == 0) {
		  cerr << get_line() << ": error: Unable to elaborate "
			"index expression `" << *msb_ << "'" << endl;
		  des->errors += 1;
		  return 0;
	    }

	    NetEMemory*node = new NetEMemory(mem, i);
	    node->set_line(*this);
	    return node;
      }

	// If the identifier names a variable of some sort, then this
	// is a variable reference.
      if (var != 0) {

	    NetEVariable*node = new NetEVariable(var);
	    node->set_line(*this);
	    return node;
      }

	  // If the identifier is a named event.
	  // is a variable reference.
      if (eve != 0) {
		  NetEEvent*tmp = new NetEEvent(eve);
		  tmp->set_line(*this);
		  return tmp;
      }

	// Finally, if this is a scope name, then return that. Look
	// first to see if this is a name of a local scope. Failing
	// that, search globally for a hierarchical name.
      if ((path_.peek_name(1) == 0))
	    if (NetScope*nsc = scope->child(path_.peek_name(0))) {
		  NetEScope*tmp = new NetEScope(nsc);
		  tmp->set_line(*this);
		  return tmp;
	    }

	// Try full hierarchical scope name.
      if (NetScope*nsc = des->find_scope(path_)) {
	    NetEScope*tmp = new NetEScope(nsc);
	    tmp->set_line(*this);
	    return tmp;
      }

	// Try relative scope name.
      if (NetScope*nsc = des->find_scope(scope, path_)) {
	    NetEScope*tmp = new NetEScope(nsc);
	    tmp->set_line(*this);
	    return tmp;
      }

	// I cannot interpret this identifier. Error message.
      cerr << get_line() << ": error: Unable to bind wire/reg/memory "
	    "`" << path_ << "' in `" << scope->name() << "'" << endl;
      des->errors += 1;
      return 0;
}

NetExpr* PENumber::elaborate_expr(Design*des, NetScope*, bool) const
{
      assert(value_);
      NetEConst*tmp = new NetEConst(*value_);
      tmp->set_line(*this);
      return tmp;
}

NetExpr* PEString::elaborate_expr(Design*des, NetScope*, bool) const
{
      NetEConst*tmp = new NetEConst(value());
      tmp->set_line(*this);
      return tmp;
}

/*
 * Elaborate the Ternary operator. I know that the expressions were
 * parsed so I can presume that they exist, and call elaboration
 * methods. If any elaboration fails, then give up and return 0.
 */
NetExpr*PETernary::elaborate_expr(Design*des, NetScope*scope, bool) const
{
      assert(expr_);
      assert(tru_);
      assert(fal_);

      NetExpr*con = expr_->elaborate_expr(des, scope);
      if (con == 0)
	    return 0;

      NetExpr*tru = tru_->elaborate_expr(des, scope);
      if (tru == 0) {
	    delete con;
	    return 0;
      }

      NetExpr*fal = fal_->elaborate_expr(des, scope);
      if (fal == 0) {
	    delete con;
	    delete tru;
	    return 0;
      }

      NetETernary*res = new NetETernary(con, tru, fal);
	  res->set_line(*this);
      return res;
}

NetExpr* PEUnary::elaborate_expr(Design*des, NetScope*scope, bool) const
{
      NetExpr*ip = expr_->elaborate_expr(des, scope);
      if (ip == 0) return 0;

      /* Should we evaluate expressions ahead of time,
       * just like in PEBinary::elaborate_expr() ?
       */

      NetExpr*tmp;
      switch (op_) {
	  default:
	    tmp = new NetEUnary(op_, ip);
	    tmp->set_line(*this);
	    break;

	  case '-':
	    if (NetEConst*ipc = dynamic_cast<NetEConst*>(ip)) {
		    /* When taking the - of a number, turn it into a
		       signed expression and extend it one bit to
		       accommodate a possible sign bit. */
		  verinum val = ipc->value();
		  verinum zero (verinum::V0, val.len()+1, val.has_len());
		  val = zero - val;
		  val.has_sign(true);
		  tmp = new NetEConst(val);
		  delete ip;
	    } else {
		  tmp = new NetEUnary(op_, ip);
		  tmp->set_line(*this);
	    }
	    break;

	  case '+':
	    tmp = ip;
	    break;

	  case '!': // Logical NOT
	      /* If the operand to unary ! is a constant, then I can
		 evaluate this expression here and return a logical
		 constant in its place. */
	    if (NetEConst*ipc = dynamic_cast<NetEConst*>(ip)) {
		  verinum val = ipc->value();
		  unsigned v1 = 0;
		  unsigned vx = 0;
		  for (unsigned idx = 0 ;  idx < val.len() ;  idx += 1)
			switch (val[idx]) {
			    case verinum::V0:
			      break;
			    case verinum::V1:
			      v1 += 1;
			      break;
			    default:
			      vx += 1;
			      break;
			}

		  verinum::V res;
		  if (v1 > 0)
			res = verinum::V0;
		  else if (vx > 0)
			res = verinum::Vx;
		  else
			res = verinum::V1;

		  verinum vres (res, 1, true);
		  tmp = new NetEConst(vres);
		  tmp->set_line(*this);
		  delete ip;
	    } else {
		  tmp = new NetEUReduce(op_, ip);
		  tmp->set_line(*this);
	    }
	    break;

	  case '&': // Reduction AND
	  case '|': // Reduction OR
	  case '^': // Reduction XOR
	  case 'A': // Reduction NAND (~&)
	  case 'N': // Reduction NOR (~|)
	  case 'X': // Reduction NXOR (~^)
	    tmp = new NetEUReduce(op_, ip);
	    tmp->set_line(*this);
	    break;

	  case '~':
	    tmp = new NetEUBits(op_, ip);
	    tmp->set_line(*this);
	    break;
      }

      return tmp;
}