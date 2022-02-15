#include "Module.h"
#include "PExpr.h"

verinum* PExpr::evaluate(Design*des, NetScope*scope, const vcd_vars vv)
{
	return 0;
}

verinum* PEConcat::evaluate(Design*des, NetScope*scope, const vcd_vars vv)
{
	long rep = 1;
	svector<verinum*> vn;
	if(repeat_)
	{
		verinum* vn = repeat_->evaluate(des, scope, vv);
		rep = vn->as_ulong();
	}
	unsigned width = 0;
	for(unsigned idx = 0; idx < parms_.count(); ++idx)
	{
		verinum* v = parms_[idx]->evaluate(des, scope, vv);
		width = width + v->len();
		vn = svector<verinum*>(vn, v);
	}
	width = width * rep;
	verinum* res = new verinum(verinum::Vx, width);
	unsigned count = 0;
	for(unsigned re = 0; re < rep; ++re)
	{
		for(idx = vn.count() - 1; idx >= 0; --idx)
		{
			for(unsigned index = vn[idx]->len() - 1; index >= 0; --index)
			{
				res->set(count, (*(vn[idx]))[index]);
				count = count + 1;
			}
		}
	}

	return res;
}

verinum* PEEvent::evaluate(Design*des, NetScope*scope, const vcd_vars vv)
{
	return 0;
}

verinum* PEFNumber::evaluate(Design*des, NetScope*scope, const vcd_vars vv)
{
	long val = value_->as_long();
	return new verinum(val);
}

verinum* PEIdent::evaluate(Design*des, NetScope*scope, const vcd_vars vv)
{
	//NetScope*found_in;
	assert(scope);
	const NetExpr*expr = des->find_parameter(scope, path_);
	if(expr != 0) //is parameter
	{
		NetExpr*tmp;
		
		assert(expr);
		tmp = expr? expr->dup_expr() : new NetEParam(des, scope, path_);
		if (msb_ && lsb_) 
		{
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
				exit(1);
			}
			
			long lsb = lsn->as_long();
			long msb = msn->as_long();
			if ((lsb < 0) || (msb < lsb)) {
				cerr << get_line() << ": error: invalid part "
					<< "select: " << path_
					<< "["<<msb<<":"<<lsb<<"]" << endl;
				exit(1);
			}
			
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
			for (long idx = lsb ;  idx <= msb ;  idx += 1) 
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
			
			/* If the input is a string, and the part select
			is working on byte boundaries, then the result
			can be made into a string. */
			if (exl.is_string()	&& (lsb%8 == 0)	&& (result.len()%8 == 0))
				result = verinum(result.as_string());
			return new verinum(result);
		} 
		else if (msb_) 
		{
		/* Handle the case where a parameter has a bit
		select attached to it. Generate a NetESelect
			object to select the bit as desired. */
			verinum* vn = msb_->evaluate(des, scope, vv);
			
			/* Let's first try to get constant values for both
			the parameter and the bit select. If they are
			both constant, then evaluate the bit select and
			return instead a single-bit constant. */
			
			NetEConst*le = dynamic_cast<NetEConst*>(tmp);
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
				return new verinum(rb, 1);
			} 
			else 
			{
				cerr << get_line() << ": internal error: Unable to evaluate "
					<< "constant expression (parameter=" << path_
					<< "): " << *le << endl;
				exit(1);
			}			
		} 
		else
		{
			/* No bit or part select. Make the constant into a
			NetEConstParam if possible. */
			NetEConst*ctmp = dynamic_cast<NetEConst*>(tmp);
			if (ctmp != 0) {
				cerr << get_line() << ": internal error: Unable to evaluate "
					<< "constant expression (parameter=" << path_
					<< "): " << *ctmp << endl;
				exit(1);
			}

			return new verinum(ctmp->value());
		}
	}
	else //not parameter, must search in vcd_vars_
	{
		bool found = false;
		verinum vn;
		for(map<string, vcd_var>::const_iterator pos = vv.begin(); pos != vv.end(); ++pos)
		{
			if(path_.peek_name(path_.component_count() - 1) == pos->second.name_)
			{
				found = true;
				vn = pos->second.cur_val_;
				break;
			}
		}
		if(!found)
		{
			cerr << "The signal used in condition expression are not dumped : " << path_ << endl;
			exit(1);
		}
		else
		{
			if (NetNet*net = des->find_signal(scope, path_)) 
			{
				if (lsb_)
				{
					assert(msb_);
					verinum*lsn = lsb_->eval_const(des, scope);
					verinum*msn = msb_->eval_const(des, scope);
					
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
					
					verinum* v = new verinum(verinum::Vx, wid); 
					for(unsigned idx = 0; idx < wid; ++idx)
					{
						v->set(idx, vn[lsv + idx]);
					}
					return v;
				}
				
				verinum*msn;
				if (msb_ && (msn = msb_->eval_const(des, scope))) 
				{
					unsigned long msv = msn->as_ulong();
					unsigned idx = net->sb_to_idx(msv);
					return new verinum(vn[idx], 1);
				}

				if (msb_) 
				{
					verinum* t = msb_->evaluate(des, scope, vv);
					return new verinum(vn[t->as_ulong()], 1);
				}

				assert(msb_ == 0);
				return new verinum(vn);

				if (NetMemory*mem = des->find_memory(scope, path_)) 
				{
					cerr << "Memory type is unsupported by dump and our system!" << endl;
					exit(1);
				}
			}
		}
	}
}

verinum* PENumber::evaluate(Design*des, NetScope*scope, const vcd_vars vv)
{
	return new verinum(value());
}

verinum* PEString::evaluate(Design*des, NetScope*scope, const vcd_vars vv)
{
	return new verinum(string(text_));
}

verinum* PEUnary::evaluate(Design*des, NetScope*scope, const vcd_vars vv)
{
	verinum*val = expr_->evaluate(des, scope, vv);

	switch(op_)
	{
	case '~': //  -- Bit-wise negation
		return new verinum(v_not(*val));
	case '!': //  -- Logical negation
		return new verinum(lnot(*val), 1);
	case '&': //  -- Reduction AND
		return new verinum(unary_and(*val), 1);
	case '|': //  -- Reduction OR
		return new verinum(unary_or(*val), 1);
	case '^': //  -- Reduction XOR
		return new verinum(unary_xor(*val), 1);
	case '+': //  --
		return val;
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
			return val;
		}
	case 'A': //  -- Reduction NAND (~&)
		return new verinum(unary_nand(*val), 1);
	case 'N': //  -- Reduction NOR (~|)
		return new verinum(unary_nor(*val), 1);
	case 'X': //  -- Reduction NXOR (~^ or ^~)
		return new verinum(unary_xnor(*val), 1);
	}
}

verinum* PEBinary::evaluate(Design*des, NetScope*scope, const vcd_vars vv)
{
	verinum*l = left_->evaluate(des, scope, vv);
	verinum*r = right_->evaluate(des, scope, vv);
	
	verinum*res;

	switch(op_)
	{
	case '^': //  -- Bit-wise exclusive OR
		res = new verinum(bitwise_xor(*l, *r));
		break;
	case '+': //  -- Arithmetic add
		{
			if (l->is_defined() && r->is_defined()) {
				res = new verinum(*l + *r);
			} else {
				res = new verinum(verinum::Vx, l->len());
			}
			break;
		}
	case '-': //  -- Arithmetic minus
		{
			if (l->is_defined() && r->is_defined()) {
				res = new verinum(*l - *r);
			} else {
				res = new verinum(verinum::Vx, l->len());
			}
			break;
		}
	case '*': //  -- Arithmetic multiply
		{
			if (l->is_defined() && r->is_defined()) {
				res = new verinum(*l * *r);
			} else {
				res = new verinum(verinum::Vx, l->len());
			}
			break;
		}
	case '/': //  -- Arithmetic divide
		{
			if (l->is_defined() && r->is_defined()) {
				long lv = l->as_long();
				long rv = r->as_long();
				res = new verinum(lv / rv, l->len());
			} else {
				res = new verinum(verinum::Vx, l->len());
			}
			break;
		}
	case '%': //  -- Arithmetic modulus
		{
			if (l->is_defined() && r->is_defined()) {
				long lv = l->as_long();
				long rv = r->as_long();
				res = new verinum(lv % rv, l->len());
			} else {
				res = new verinum(verinum::Vx, l->len());
			}
			break;
		}
	case '&': //  -- Bit-wise AND
		res = new verinum(bitwise_and(*l, *r));
		break;
	case '|': //  -- Bit-wise OR
		res = new verinum(bitwise_or(*l, *r));
		break;
	case '<': //  -- Less then
		{
			if (l->is_defined() && r->is_defined()) {
				res = new verinum(l < r, l->len());
			} else {
				res = new verinum(verinum::Vx, l->len());
			}
			break;
		}
	case '>': //  -- Greater then
		{
			if (l->is_defined() && r->is_defined()) {
				res = new verinum(l > r, l->len());
			} else {
				res = new verinum(verinum::Vx, l->len());
			}
			break;
		}
	case 'e': //  -- Logical equality (==)
		res = new verinum(*l == *r, 1);
		break;
	case 'E': //  -- Case equality (===)
		res = new verinum(ceq(*l, *r), 1);
		break;
	case 'L': //  -- Less or equal
		res = new verinum(*l <= *r, 1);
		break;
	case 'G': //  -- Greater or equal
		res = new verinum(*l >= *r, 1);
		break;
	case 'n': //  -- Logical inequality (!=)
		res = new verinum(*l != *r, 1);
		break;
	case 'N': //  -- Case inequality (!==)
		res = new verinum(cneq(*l, *r), 1);
		break;
	case 'a': //  -- Logical AND (&&)
		res = new verinum(land(*l, *r), 1);
		break;
	case 'A': //  -- Bitwise NAND (~&)
		res = new verinum(v_not(bitwise_and(*l, *r)));
		break;
	case 'o': //  -- Logical OR (||)
		res = new verinum(lor(*l, *r), 1);
		break;
	case 'O': //  -- Bit-wise NOR (~|)
		res = new verinum(v_not(bitwise_or(*l, *r)));
		break;
	case 'l': //  -- Left shift (<<)
		res = new verinum(lshift(*l, *r));
		break;
	case 'r': //  -- Right shift (>>)
		res = new verinum(rshift(*l, *r));
		break;
	case 'X': //  -- Bitwise exclusive NOR (~^)
		res = new verinum(bitwise_xnor(*l, *r), 1);
		break;
	}

	return res;
}

verinum* PETernary::evaluate(Design*des, NetScope*scope, const vcd_vars vv)
{
	verinum* vn = expr_->evaluate(des, scope, vv);
	if(vn[0] == verinum::V1)
		return tru_->evaluate(des, scope, vv);
	else
		return fal_->evaluate(des, scope, vv);
}

verinum* PECallFunction::evaluate(Design*des, NetScope*scope, const vcd_vars vv)
{
	cerr << "Unsupported function call in expression!" << endl;
	exit(1);
}