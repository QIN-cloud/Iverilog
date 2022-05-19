
#include "PExpr.h"
#include "PGate.h"
#include  "ivl_assert.h"

void link_wires(vector<Wires>& l, vector<Wires>& r);

void PGAssign::evaluate(Design* des, NetScope* scope, VcdScope* instan, bool combine, bool branch)
{
	if(instan->bits_.find(this) == instan->bits_.end()){
		parse_bits(des, scope, instan);
	}

	vector<Wires>* bits = &(instan->bits_[this]);

	map<PExpr*, map<PExpr*, bool> > values;
	verinum* value = pin(1)->evaluate(des, scope, instan, combine, values);
	if(combine) instan->report_->add_cond_report(values);

	unsigned width = value->get_nbits();
	int index = width - 1; 
	for(Wires w : *bits){
		VcdVar* var = w.first;
		for(unsigned bit : w.second){
			if(index < 0){
				cerr << get_fileline() << " width of ports should be equal." << endl;
			}
			var->sim_val.set(bit, value->get(index));
			index--;
		}
	}
	delete value;
}

void link_wires(vector<Wires>& l, vector<Wires>& r)
{
	for(Wires w : r){
		l.push_back(w);
	}
}

void PGAssign::parse_bits(Design* des, NetScope* scope, VcdScope* instan)
{
	instan->bits_[this] = get_pins()[0]->parse_bits(des, scope, instan);
	assert(!instan->bits_[this].empty());
}

vector<Wires> PExpr::parse_bits(Design* des, NetScope*scope, VcdScope* instan)
{
	vector<Wires> res = vector<Wires>(0);
	return res;
}

vector<Wires> PEConcat::parse_bits(Design* des, NetScope*scope, VcdScope* instan)
{
	vector<Wires> res = vector<Wires>(0);

	if(parms_.empty()){
		cerr << "Line " << get_fileline() << " concat {null} is not allowed." << endl;
	}

	for(unsigned i = 0; i < parms_.size(); i++){
		vector<Wires> ws = parms_[i]->parse_bits(des, scope, instan);
		link_wires(res, ws);
		ws.clear();
	}

	return res;
}

vector<Wires> PEIdent::parse_bits(Design* des, NetScope*scope, VcdScope* instan)
{
	vector<Wires> res = vector<Wires>(0);
	vector<unsigned> bits;
	//NetScope*found_in;
	assert(scope);
	name_component_t variable =  path_.back();
	perm_string name = variable.name;
	list<index_component_t> bits_width = variable.index;

	//variable expression is a memory
	if(bits_width.size() > 1){
		cerr << get_fileline() << " error : "
		" Memory values can not reported in vcd file , please use reg or wire in this condition expression" << endl;
		exit(1);
	}

	VcdVar* var = 0;

	assert(instan->defines_.find(name.str()) != instan->defines_.end());
	var = instan->defines_[name.str()];

	//Lsb or msb exit.
	if(bits_width.size()){   
		PExpr* lsb_ = bits_width.back().lsb;
		PExpr* msb_ = bits_width.back().msb;
		NetNet* net = scope->find_signal(name);

		assert(net);
		//Lsb and msb exit, we select the part form lsb to msb of current value.
		if (lsb_){
			assert(msb_);
			verinum* lsn = lsb_->eval_const(des, scope);
			verinum* msn = msb_->eval_const(des, scope);
			assert(lsn);
			assert(msn);
			unsigned lsv = lsn->as_unsigned();
			unsigned msv = msn->as_unsigned();
			for(unsigned i = lsv; i <= msv; i++){
				bits.insert(bits.begin(), (i - unsigned(var->lsb)));
			}
			delete lsn;
			delete msn;
		}
		//Only msb exits and msb is a const type, we can get this bit directly, like "x[1]".
		else if (msb_) {
			verinum* msn = msb_->eval_const(des, scope);
			assert(msn);
			long msv = msn->as_long();
			bits.push_back(msv - var->lsb);
			delete msn;
		}
	}
	//Using variable name, like x.
	else{
		for(unsigned i = 0; i < var->width; i++)
			bits.insert(bits.begin(), i);
	}

	res.push_back(make_pair(var, bits));
	return res;
}

verinum* PExpr::evaluate(Design*des, NetScope*scope, VcdScope* instan, bool combine, map<PExpr*, map<PExpr*, bool> >& values)
{
	return 0;
}

verinum* PEConcat::evaluate(Design*des, NetScope*scope, VcdScope* instan, bool combine, map<PExpr*, map<PExpr*, bool> >& values)
{
	verinum* v = nullptr;
	verinum vec(verinum::V0,0,false);
	for(unsigned i = 0; i < parms_.size(); i++){
		verinum* vn = parms_[i]->evaluate(des, scope, instan, combine, values);
		vec = concat(vec, *vn);
		delete vn;
	}
	if(repeat_){
		PENumber* num = dynamic_cast<PENumber*>(repeat_);
		assert(num);
		unsigned n = num->value().as_unsigned();
		unsigned l = vec.get_nbits();
		v = new verinum(verinum::Vx, l*n, true);
		for(unsigned i = 0; i < n; i++){
			for(unsigned j = 0; j < l; j++){
				v->set(i*l+j, vec.get(j));
			}
		}
	}
	else
		v = new verinum(vec);
	return v;
}

verinum* PEEvent::evaluate(Design*des, NetScope*scope, VcdScope* instan, bool combine, map<PExpr*, map<PExpr*, bool> >& values)
{
	return 0;
}

verinum* PEFNumber::evaluate(Design*des, NetScope*scope, VcdScope* instan, bool combine, map<PExpr*, map<PExpr*, bool> >& values)
{
	long val = value_->as_long();
	verinum* v = new verinum(val);
	return v;
}

verinum* PEIdent::evaluate(Design*des, NetScope*scope, VcdScope* instan, bool combine, map<PExpr*, map<PExpr*, bool> >& values)
{
	assert(scope);
	name_component_t cond_expr =  path_.back();
	perm_string cond_str = cond_expr.name;
	list<index_component_t> bits_width = cond_expr.index;

	//variable expression is a memory
	if(bits_width.size() > 1){
		cerr << "Line : " << get_lineno() << " error : "
		" Memory values can not reported in vcd file , please use reg or wire in this condition expression" << endl;
		exit(1);
	}

	//verinum result of this expr.
	verinum* v =0;

	NetScope::param_ref_t expr_pos;
	
	//The expr is a parameter.
	if(scope->find_parameter(cond_str, expr_pos)){
		NetExpr* expr = expr_pos->second.val;
		//Expr has lsb or msb , such as using "x[1:0]",  "x[1]".
		if(bits_width.size()){
			PExpr* lsb_ = bits_width.back().lsb;
			PExpr* msb_ = bits_width.back().msb;
			//Lsb and msb both exist, then we copy the part from lsb to msb of value.
			if(lsb_ && msb_){
				verinum* lsn = lsb_->eval_const(des, scope);
				verinum* msn = msb_->eval_const(des, scope);
				if(lsn == 0 || msn == 0){
					cerr << "Line : " << get_lineno() << " error : "
					"Part select expressions must be constant expressions."
					<< endl;
					exit(1);
				}

				long lsb = lsn->as_long();
				long msb = lsn->as_long();
				if((lsb < 0) || (msb < lsb)){
					cerr<<"Line : " << get_lineno() <<
					"invaild part select : " << cond_str.str() << 
					"[" << msb << ":" << lsb << "]" << endl;
					exit(1);
				}

				NetEConst* le = dynamic_cast<NetEConst*>(expr);
				assert(le);

				verinum result(verinum::V0, msb-lsb+1, true);
				verinum exl = le->value();

				for(long idx = lsb ; idx <= msb; idx += 1){
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
			else if(msb_){
				verinum* vn = msb_->evaluate(des, scope, instan, combine, values);
				NetEConst* le = dynamic_cast<NetEConst*>(expr);

				if (le){
					verinum lv = le->value();
					verinum::V rb = verinum::Vx;
					
					long ridx = vn->as_long();
					if ((ridx >= 0) && (ridx < lv.len())) {
						rb = lv[ridx];
					} 
					else if ((ridx >= 0) && (!lv.has_len())) {
						if (lv.has_sign())
							rb = lv[lv.len()-1];
						else
							rb = verinum::V0;
					}
					v = new verinum(rb, 1);
				} 
				else {
					cerr << get_lineno() << ": internal error: Unable to evaluate "
						<< "constant expression (parameter=" << path_
						<< "): " << *le << endl;
					exit(1);
				}
			}

		}
		//No lsb and msb means that we can use the value of parameter from where it defined.
		else{
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
	else{
		verinum vn;
		VcdVar* var = 0;

		if(instan->defines_.find(cond_str.str()) == instan->defines_.end()){
			cerr << "Line : " << get_lineno() << " error : "
			"The signal used in condition expression are not dumped : " << path_ << endl;
			exit(1);
		}
		var = instan->defines_[cond_str.str()];
		vn = var->sim_val;

		//Lsb or msb exit.
		if(bits_width.size()){   
			PExpr* lsb_ = bits_width.back().lsb;
			PExpr* msb_ = bits_width.back().msb;
			verinum* msn_ ;
			NetNet* net = scope->find_signal(cond_str);

			assert(net);
			//Lsb and msb exit, we select the part form lsb to msb of current value.
			if (lsb_){
				assert(msb_);

				verinum* lsn = lsb_->eval_const(des, scope);
				verinum* msn = msb_->eval_const(des, scope);
				assert(lsn);
				assert(msn);
				
				long lsv = lsn->as_long();
				long msv = msn->as_long();
				unsigned long wid = 1 + ((msv>lsv)? (msv-lsv) : (lsv-msv));
				v = new verinum(verinum::Vx, wid); 

				for(unsigned idx = 0; idx < wid; ++idx){
					if((lsv + idx) >= vn.get_nbits())
						v->set(idx, verinum::V0);
					else
						v->set(idx, vn[lsv + idx]);
				}
			}

			//Only msb exits and msb is a const type, we can get this bit directly, like "x[1]".
			else if (msb_ && (msn_ = msb_->eval_const(des, scope))) {
				unsigned long msv = msn_->as_ulong();
				unsigned idx = msv - var->lsb;
				v = new verinum(vn[idx], 1);
			}
			//Msb is a expr, we need to evaluate this expr.
			else if (msb_) {
				verinum* t = msb_->evaluate(des, scope, instan, combine, values);
				unsigned idx = t->as_ulong() - var->lsb;
				v = new verinum(vn[idx], 1);
				delete t;
			}

		}
		//Use the current value directly.
		else
			v = new verinum(vn);
	}
	return v;
}

verinum* PENumber::evaluate(Design*des, NetScope*scope, VcdScope* instan, bool combine, map<PExpr*, map<PExpr*, bool> >& values)
{
	return new verinum(value());
}

verinum* PEString::evaluate(Design*des, NetScope*scope, VcdScope* instan, bool combine, map<PExpr*, map<PExpr*, bool> >& values)
{
	verinum* v = new verinum(string(text_));
	return v;
}

verinum* PEUnary::evaluate(Design*des, NetScope*scope, VcdScope* instan, bool combine, map<PExpr*, map<PExpr*, bool> >& values)
{
	verinum* val = expr_->evaluate(des, scope, instan, combine, values);
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

verinum* PEBinary::evaluate(Design*des, NetScope*scope, VcdScope* instan, bool combine, map<PExpr*, map<PExpr*, bool> >& values)
{
	verinum*l = left_->evaluate(des, scope, instan, combine, values);
	verinum*r = right_->evaluate(des, scope, instan, combine, values);
	
	if(combine){
		if(instan->module_->reverse_exprs_.find(left_)
		!= instan->module_->reverse_exprs_.end())
			values[instan->module_->reverse_exprs_[left_]][left_] = l->as_unsigned();
		if(instan->module_->reverse_exprs_.find(right_)
		!= instan->module_->reverse_exprs_.end())
			values[instan->module_->reverse_exprs_[right_]][right_] = r->as_unsigned();
	}
	
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

verinum* PETernary::evaluate(Design*des, NetScope*scope, VcdScope* instan, bool combine, map<PExpr*, map<PExpr*, bool> >& values)
{
	verinum* vn = expr_->evaluate(des, scope, instan, combine, values);
	verinum* v;
	if(combine)
		values[this][expr_] = (*vn)[0] == verinum::V1;
	if((*vn)[0] == verinum::V1)
		v = tru_->evaluate(des, scope, instan, combine, values);
	else
		v = fal_->evaluate(des, scope, instan, combine, values);
	delete vn;
	return v;
}

verinum* PECallFunction::evaluate(Design*des, NetScope*scope, VcdScope* instan, bool combine, map<PExpr*, map<PExpr*, bool> >& values)
{
	cerr << "Unsupported function call in expression!" << endl;
	exit(1);
}