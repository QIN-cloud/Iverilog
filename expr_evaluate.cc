
#include "PExpr.h"
#include  "ivl_assert.h"

verinum* PExpr::evaluate(Design*des, NetScope*scope, vcd_vars& vv, bool combine, Cfg_Node* cfgnode, map<PExpr*, verinum>& items)
{
	return 0;
}

verinum* PEConcat::evaluate(Design*des, NetScope*scope, vcd_vars& vv, bool combine, Cfg_Node* cfgnode, map<PExpr*, verinum>& items)
{
	long rep = 1;
	svector<verinum*> vn;
	if(repeat_)
	{
		verinum* vn = repeat_->evaluate(des, scope, vv, combine, cfgnode, items);
		rep = vn->as_ulong();
	}
	unsigned width = 0;
	for(unsigned idx = 0; idx < parms_.size(); ++idx)
	{
		verinum* v = parms_[idx]->evaluate(des, scope, vv, combine, cfgnode, items);
		width = width + v->len();
		vn = svector<verinum*>(vn, v);
	}
	width = width * rep;
	verinum* v = new verinum(verinum::Vx, width);
	unsigned count = 0;
	for(unsigned re = 0; re < rep; ++re)
	{
		for(unsigned idx = vn.count() - 1; idx >= 0; --idx)
		{
			for(unsigned index = vn[idx]->len() - 1; index >= 0; --index)
			{
				v->set(count, (*(vn[idx]))[index]);
				count = count + 1;
			}
		}
	}
	if(combine)
	{
		items.insert(make_pair(this, verinum(*v)));
	}
	return v;
}

verinum* PEEvent::evaluate(Design*des, NetScope*scope, vcd_vars& vv, bool combine, Cfg_Node* cfgnode, map<PExpr*, verinum>& items)
{
	return 0;
}

verinum* PEFNumber::evaluate(Design*des, NetScope*scope, vcd_vars& vv, bool combine, Cfg_Node* cfgnode, map<PExpr*, verinum>& items)
{
	long val = value_->as_long();
	verinum* v = new verinum(val);
	if(combine)
	{
		items.insert(make_pair(this, verinum(*v)));
	}
	return v;
}

verinum* PEIdent::evaluate(Design*des, NetScope*scope, vcd_vars& vv, bool combine, Cfg_Node* cfgnode, map<PExpr*, verinum>& items)
{
	//NetScope*found_in;
	assert(scope);
	name_component_t cond_expr =  path_.back();
	perm_string cond_str = cond_expr.name;
	list<index_component_t> bits_width = cond_expr.index;

	//variable expression is a memory
	if(bits_width.size() > 1)
	{
		cerr << "Line : " << get_lineno() << " error : "
		" Memory values can not reported in vcd file , please use reg or wire in this condition expression" << endl;
		exit(1);
	}

	//verinum result of this expr.
	verinum* v =0;

	NetScope::param_ref_t expr_pos;
	
	//The expr is a parameter.
	if(scope->find_parameter(cond_str, expr_pos))
	{
		NetExpr* expr = expr_pos->second.val;
		//Expr has lsb or msb , such as using "x[1:0]",  "x[1]".
		if(bits_width.size())
		{
			PExpr* lsb_ = bits_width.back().lsb;
			PExpr* msb_ = bits_width.back().msb;
			//Lsb and msb both exist, then we copy the part from lsb to msb of value.
			if(lsb_ && msb_)
			{
				verinum* lsn = lsb_->eval_const(des, scope);
				verinum* msn = msb_->eval_const(des, scope);
				if(lsn == 0 || msn == 0)
				{
					cerr << "Line : " << get_lineno() << " error : "
					"Part select expressions must be constant expressions."
					<< endl;
					exit(1);
				}

				long lsb = lsn->as_long();
				long msb = lsn->as_long();
				if((lsb < 0) || (msb < lsb))
				{
					cerr<<"Line : " << get_lineno() <<
					"invaild part select : " << cond_str.str() << 
					"[" << msb << ":" << lsb << "]" << endl;
					exit(1);
				}

				NetEConst* le = dynamic_cast<NetEConst*>(expr);
				assert(le);

				verinum result(verinum::V0, msb-lsb+1, true);
				verinum exl = le->value();

				for(long idx = lsb ; idx <= msb; idx += 1)
				{
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

				if(exl.is_string() && (lsb%8 == 0) && (result.len()%8 == 0))
					result = verinum(result.as_string());

				v = new verinum(result);
			}
			//Msb only, we select the bit in msb.
			else if(msb_)
			{
				verinum* vn = msb_->evaluate(des, scope, vv, combine, cfgnode, items);
				NetEConst* le = dynamic_cast<NetEConst*>(expr);

				if (le) 
				{
					verinum lv = le->value();
					verinum::V rb = verinum::Vx;
					
					long ridx = vn->as_long();
					if ((ridx >= 0) && (ridx < lv.len())) 
					{
						rb = lv[ridx];
					} 
					else if ((ridx >= 0) && (!lv.has_len())) 
					{
						if (lv.has_sign())
							rb = lv[lv.len()-1];
						else
							rb = verinum::V0;
					}
					v = new verinum(rb, 1);
				} 
				else 
				{
					cerr << get_lineno() << ": internal error: Unable to evaluate "
						<< "constant expression (parameter=" << path_
						<< "): " << *le << endl;
					exit(1);
				}
			}

		}
		//No lsb and msb means that we can use the value of parameter from where it defined.
		else
		{
			NetExpr* tmp = expr_pos->second.val;
			NetEConst*ctmp = dynamic_cast<NetEConst*>(tmp);
			if (ctmp == 0) {
				cerr << get_lineno() << ": internal error: Unable to evaluate "
					<< "unconstant expression (parameter=" << path_
					<< "): " << *ctmp << endl;
				exit(1);
			}

			v = new verinum(ctmp->value());
		}
	}
	//The expr is a variable, we need to get the value from vcd var.
	else
	{
		bool found = false;
		verinum vn;
		map<string, VcdVar*>::const_iterator pos;
		for(pos = vv.begin(); pos != vv.end(); pos++)
		{
			if(pos->second->name.compare(cond_str.str()) == 0)
			{
				vn = pos->second->cur_val;
				found = true;
				break;
			}
		}

		if(!found)
		{
			cerr << "Line : " << get_lineno() << " error : "
			"The signal used in condition expression are not dumped : " << path_ << endl;
			exit(1);
		}
		//Lsb or msb exit.
		if(bits_width.size())
		{
			PExpr* lsb_ = bits_width.back().lsb;
			PExpr* msb_ = bits_width.back().msb;
			verinum* msn_ ;
			NetNet* net = scope->find_signal(cond_str);

			assert(net);
			//Lsb and msb exit, we select the part form lsb to msb of current value.
			if (lsb_)
			{
				assert(msb_);

				verinum* lsn = lsb_->eval_const(des, scope);
				verinum* msn = msb_->eval_const(des, scope);
				assert(lsn);
				assert(msn);
				
				long lsv = lsn->as_long();
				long msv = msn->as_long();
				unsigned long wid = 1 + ((msv>lsv)? (msv-lsv) : (lsv-msv));
				
				v = new verinum(verinum::Vx, wid); 
				for(unsigned idx = 0; idx < wid; ++idx)
				{
					v->set(idx, vn[lsv + idx]);
				}
			}

			//Only msb exits and msb is a const type, we can get this bit directly, like "x[1]".
			else if (msb_ && (msn_ = msb_->eval_const(des, scope))) 
			{
				unsigned long msv = msn_->as_ulong();
				unsigned idx = msv - pos->second->lsb;
				v = new verinum(vn[idx], 1);
			}
			//Msb is a expr, we need to evaluate this expr.
			else if (msb_) 
			{
				verinum* t = msb_->evaluate(des, scope, vv, combine, cfgnode, items);
				unsigned idx = t->as_ulong() - pos->second->lsb;
				v = new verinum(vn[idx], 1);
				delete t;
			}

		}
		//Use the current value directly.
		else
		{
			v = new verinum(pos->second->cur_val);
		}

	}
	if(combine)
	{
		items.insert(make_pair(this, verinum(*v)));
	}
	return v;
}

verinum* PENumber::evaluate(Design*des, NetScope*scope, vcd_vars& vv, bool combine, Cfg_Node* cfgnode, map<PExpr*, verinum>& items)
{
	return new verinum(value());
}

verinum* PEString::evaluate(Design*des, NetScope*scope, vcd_vars& vv, bool combine, Cfg_Node* cfgnode, map<PExpr*, verinum>& items)
{
	verinum* v = new verinum(string(text_));
	if(combine)
	{
		items.insert(make_pair(this, verinum(*v)));
	}
	return v;
}

verinum* PEUnary::evaluate(Design*des, NetScope*scope, vcd_vars& vv, bool combine, Cfg_Node* cfgnode, map<PExpr*, verinum>& items)
{
	verinum* val = expr_->evaluate(des, scope, vv, combine, cfgnode, items);
	verinum* v; 
	switch(op_)
	{
	case '~': //  -- Bit-wise negation
		v = new verinum(v_not(*val));
		break;
	case '!': //  -- Logical negation
		v = new verinum(lnot(*val), 1);
		break;
	case '&': //  -- Reduction AND
		v = new verinum(unary_and(*val), 1);
		break;
	case '|': //  -- Reduction OR
		v = new verinum(unary_or(*val), 1);
		break;
	case '^': //  -- Reduction XOR
		v = new verinum(unary_xor(*val), 1);
		break;
	case '+': //  --
		v = new verinum(*val);
		break;
	case '-': //  --
		{
			/* We need to expand the value a bit if we are
			taking the 2's complement so that we are
			guaranteed to not overflow. */
			verinum tmp (0UL, val->len()+1);
			for (unsigned idx = 0 ;  idx < val->len() ;  idx += 1)
				tmp.set(idx, val->get(idx));
			
			*val = v_not(tmp) + verinum(verinum::V1, 1);
			val->has_sign(true);
			v = new verinum(*val);
			break;
		}
	case 'A': //  -- Reduction NAND (~&)
		v = new verinum(unary_nand(*val), 1);
		break;
	case 'N': //  -- Reduction NOR (~|)
		v = new verinum(unary_nor(*val), 1);
		break;
	case 'X': //  -- Reduction NXOR (~^ or ^~)
		v = new verinum(unary_xnor(*val), 1);
		break;
	}
	delete val;
	return v;
}

verinum* PEBinary::evaluate(Design*des, NetScope*scope, vcd_vars& vv, bool combine, Cfg_Node* cfgnode, map<PExpr*, verinum>& items)
{
	verinum*l = left_->evaluate(des, scope, vv, combine, cfgnode, items);
	verinum*r = right_->evaluate(des, scope, vv, combine, cfgnode, items);

	verinum* v;

	switch(op_)
	{
	case '^': //  -- Bit-wise exclusive OR
		v = new verinum(bitwise_xor(*l, *r));
		break;
	case '+': //  -- Arithmetic add
		{
			if (l->is_defined() && r->is_defined()) {
				long lv = l->as_long();
				long rv = r->as_long();
				v = new verinum(lv + rv, l->len() + r->len());
			} else {
				v = new verinum(verinum::Vx, l->len());
			}
			break;
		}
	case '-': //  -- Arithmetic minus
		{
			if (l->is_defined() && r->is_defined()) {
				long lv = l->as_long();
				long rv = r->as_long();
				v = new verinum((lv-rv)>0?lv-rv:0, l->len());
			} else {
				v = new verinum(verinum::Vx, l->len());
			}
			break;
		}
	case '*': //  -- Arithmetic multiply
		{
			if (l->is_defined() && r->is_defined()) {
				v = new verinum(*l * *r);
			} else {
				v = new verinum(verinum::Vx, l->len());
			}
			break;
		}
	case '/': //  -- Arithmetic divide
		{
			if (l->is_defined() && r->is_defined()) {
				long lv = l->as_long();
				long rv = r->as_long();
				v = new verinum(lv / rv, l->len());
			} else {
				v = new verinum(verinum::Vx, l->len());
			}
			break;
		}
	case '%': //  -- Arithmetic modulus
		{
			if (l->is_defined() && r->is_defined()) {
				long lv = l->as_long();
				long rv = r->as_long();
				v = new verinum(lv % rv, l->len());
			} else {
				v = new verinum(verinum::Vx, l->len());
			}
			break;
		}
	case '&': //  -- Bit-wise AND
		v = new verinum(bitwise_and(*l, *r));
		break;
	case '|': //  -- Bit-wise OR
		v = new verinum(bitwise_or(*l, *r));
		break;
	case '<': //  -- Less then
		{
			/*if (l->is_defined() && r->is_defined()) {
				res = new verinum(l < r, l->len());
			} else {
				res = new verinum(verinum::Vx, l->len());
			}*/
			v = new verinum((*l)<(*r) ? verinum::V1 : verinum::V0, 1);
			break;
		}
	case '>': //  -- Greater then
		{
			/*if (l->is_defined() && r->is_defined()) {
				res = new verinum(l > r, l->len());
			} else {
				res = new verinum(verinum::Vx, l->len());
			}*/
			v = new verinum((*l)>(*r) ? verinum::V1 : verinum::V0, 1);
			break;
		}
	case 'e': //  -- Logical equality (==)
		v = new verinum(*l == *r, 1);
		break;
	case 'E': //  -- Case equality (===)
		v = new verinum(ceq(*l, *r), 1);
		break;
	case 'L': //  -- Less or equal
		v = new verinum(*l <= *r, 1);
		break;
	case 'G': //  -- Greater or equal
		v = new verinum(*l >= *r, 1);
		break;
	case 'n': //  -- Logical inequality (!=)
		v = new verinum(*l != *r, 1);
		break;
	case 'N': //  -- Case inequality (!==)
		v = new verinum(cneq(*l, *r), 1);
		break;
	case 'a': //  -- Logical AND (&&)
		v = new verinum(land(*l, *r), 1);
		break;
	case 'A': //  -- Bitwise NAND (~&)
		v = new verinum(v_not(bitwise_and(*l, *r)));
		break;
	case 'o': //  -- Logical OR (||)
		v = new verinum(lor(*l, *r), 1);
		break;
	case 'O': //  -- Bit-wise NOR (~|)
		v = new verinum(v_not(bitwise_or(*l, *r)));
		break;
	case 'l': //  -- Left shift (<<)
		v = new verinum(lshift(*l, *r));
		break;
	case 'r': //  -- Right shift (>>)
		v = new verinum(rshift(*l, *r));
		break;
	case 'X': //  -- Bitwise exclusive NOR (~^)
		v = new verinum(bitwise_xnor(*l, *r), 1);
		break;
	}
	delete l;
	delete r;
	return v;
}

verinum* PETernary::evaluate(Design*des, NetScope*scope, vcd_vars& vv, bool combine, Cfg_Node* cfgnode, map<PExpr*, verinum>& items)
{
	verinum* vn = expr_->evaluate(des, scope, vv, combine, cfgnode, items);
	verinum* v;
	if((*vn)[0] == verinum::V1)
		v = tru_->evaluate(des, scope, vv, combine, cfgnode, items);
	else
		v = fal_->evaluate(des, scope, vv, combine, cfgnode, items);
	if(combine)
	{
		items.insert(make_pair(this, verinum(*v)));
	}
	delete vn;
	return v;
}

verinum* PECallFunction::evaluate(Design*des, NetScope*scope, vcd_vars& vv, bool combine, Cfg_Node* cfgnode, map<PExpr*, verinum>& items)
{
	cerr << "Unsupported function call in expression!" << endl;
	exit(1);
}