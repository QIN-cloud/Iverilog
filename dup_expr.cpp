#include "config.h"

#include "netlist.h"
#include <cassert>

NetExpr* NetEBComp::dup_expr() const
{
      NetEBComp*result = new NetEBComp(op_, left_->dup_expr(),
				       right_->dup_expr());
      return result;
}

NetExpr* NetEConst::dup_expr() const
{
      NetEConst*tmp = new NetEConst(value_);
      tmp->set_line(*this);
      return tmp;
}

NetExpr* NetEConstParam::dup_expr() const
{
      NetEConstParam*tmp = new NetEConstParam(scope_, name_, value());
      tmp->set_line(*this);
      return tmp;
}

NetExpr* NetECRealParam::dup_expr() const
{
      NetECRealParam*tmp = new NetECRealParam(scope_, name_, value());
      tmp->set_line(*this);
      return tmp;
}

NetExpr* NetEEvent::dup_expr() const
{
      assert(0);
      return 0;
}

NetExpr* NetEScope::dup_expr() const
{
      assert(0);
      return 0;
}

NetExpr* NetESelect::dup_expr() const
{
      return new NetESelect(expr_->dup_expr(), base_->dup_expr(),
			    expr_width());
}

NetExpr* NetESFunc::dup_expr() const
{
      NetESFunc*tmp = new NetESFunc(name_, type_, expr_width(), nparms());
      assert(tmp);
      for (unsigned idx = 0 ;  idx < nparms() ;  idx += 1) {
	    assert(tmp->parm(idx));
	    tmp->parm(idx, tmp->parm(idx)->dup_expr());
      }

      return tmp;
}

NetExpr* NetESignal::dup_expr() const
{
      NetESignal*tmp = new NetESignal(net_, msi_, lsi_);
      assert(tmp);
      tmp->expr_width(expr_width());
      return tmp;
}

NetExpr* NetETernary::dup_expr() const
{
      NetETernary*tmp = new NetETernary(cond_->dup_expr(),
					true_val_->dup_expr(),
					false_val_->dup_expr());
      return tmp;
}

NetExpr* NetEUFunc::dup_expr() const
{
      NetEUFunc*tmp;
      svector<NetExpr*> tmp_parms (parms_.count());

      for (unsigned idx = 0 ;  idx < tmp_parms.count() ;  idx += 1) {
	    assert(parms_[idx]);
	    tmp_parms[idx] = parms_[idx]->dup_expr();
      }

      tmp = new NetEUFunc(func_, (NetESignal*)(result_->dup_expr()), tmp_parms);

      return tmp;
}

NetExpr* NetEUnary::dup_expr() const
{
      NetEUnary*tmp = new NetEUnary(op_, expr_->dup_expr());
      assert(tmp);
      return tmp;
}

NetExpr* NetEUReduce::dup_expr() const
{
      NetEUReduce*tmp = new NetEUReduce(op_, expr_->dup_expr());
      assert(tmp);
      return tmp;
}

NetExpr* NetEVariable::dup_expr() const
{
      NetEVariable*tmp = new NetEVariable(var_);
      return tmp;
}