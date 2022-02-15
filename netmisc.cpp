#include "config.h"

#include "netlist.h"
#include "netmisc.h"
#include "PExpr.h"

NetNet* add_to_net(Design*des, NetNet*sig, long val)
{
      if (val == 0)
	    return sig;

      NetScope*scope = sig->scope();
      unsigned long abs_val = (val >= 0)? val : (-val);
      unsigned width = sig->pin_count();

      verinum val_v (abs_val, width);

      NetConst*val_c = new NetConst(scope, scope->local_hsymbol(), val_v);

      NetNet*val_s = new NetNet(scope, scope->local_symbol(),
			      NetNet::IMPLICIT, width);
      val_s->local_flag(true);

      NetNet*res = new NetNet(scope, scope->local_symbol(),
			      NetNet::IMPLICIT, width);
      res->local_flag(true);

      NetAddSub*add = new NetAddSub(scope, scope->local_symbol(), width);

      for (unsigned idx = 0 ;  idx < width ;  idx += 1)
	    connect(sig->pin(idx), add->pin_DataA(idx));

      for (idx = 0 ;  idx < width ;  idx += 1)
	    connect(val_c->pin(idx), add->pin_DataB(idx));

      for (idx = 0 ;  idx < width ;  idx += 1)
	    connect(val_s->pin(idx), add->pin_DataB(idx));

      for (idx = 0 ;  idx < width ;  idx += 1)
	    connect(res->pin(idx), add->pin_Result(idx));

      if (val < 0)
	    add->attribute("LPM_Direction", verinum("SUB"));
      else
	    add->attribute("LPM_Direction", verinum("ADD"));

      des->add_node(add);
      des->add_node(val_c);

      return res;
}


NetExpr* elab_and_eval(Design*des, NetScope*scope, const PExpr*pe)
{
      NetExpr*tmp = pe->elaborate_expr(des, scope);
      if (tmp == 0)
	    return 0;

      if (NetExpr*tmp2 = tmp->eval_tree()) {
	    delete tmp;
	    tmp = tmp2;
      }

      return tmp;
}