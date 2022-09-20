
#include "PExpr.h"
#include "PGate.h"
#include "ivl_assert.h"
#include "smt_generator.h"
#include <sstream>

vector<ConcatItem*> PExpr::parse_concat_expr(Design*des, NetScope*scope, map<string, RefVar*>& vars)
{
	dump(cerr);
	cerr << " is unsupported in line " << get_fileline() << endl;
	exit(1);
	return vector<ConcatItem*>();
}

vector<ConcatItem*> PEConcat::parse_concat_expr(Design*des, NetScope*scope, map<string, RefVar*>& vars)
{
	if(basevec.empty()) {
		for(PExpr* expr : parms_) {
			vector<ConcatItem*> temp = expr->parse_concat_expr(des, scope, vars);
			basevec.insert(basevec.end(), temp.begin(), temp.end());
		}
		if(repeat_) {
			PENumber* num = dynamic_cast<PENumber*>(repeat_);
			assert(num);
			unsigned repeat_n = num->value().as_unsigned();
			vector<ConcatItem*> vec_temp = basevec;
			for(unsigned i = 0; i < repeat_n-1; i++) {
				basevec.insert(basevec.end(), vec_temp.begin(), vec_temp.end());
			}
		}
	}
	return basevec;
}

vector<ConcatItem*> PEIdent::parse_concat_expr(Design*des, NetScope*scope, map<string, RefVar*>& vars)
{
	vector<ConcatItem*> res;
	assert(scope);
	name_component_t var_expr =  path_.back();
	perm_string var_str = var_expr.name;
	list<index_component_t> bits_width = var_expr.index;

	//variable expression is a memory
	if(bits_width.size() > 1){
		cerr << "Line : " << get_lineno() << " error : "
		" Memory values can not reported in vcd file , please use reg or wire in this condition expression" << endl;
		exit(1);
	}

	NetScope::param_ref_t expr_pos;
	
	//The expr is a parameter.
	if(scope->find_parameter(var_str, expr_pos)){
		verinum* v =0;
		NetExpr* expr = expr_pos->second.val;
		//Expr has lsb or msb , such as using "x[1:0]",  "x[1]".
		if(bits_width.size()) {
			PExpr* lsb_ = bits_width.back().lsb;
			PExpr* msb_ = bits_width.back().msb;
			//Lsb and msb both exist, then we copy the part from lsb to msb of value.
			if(lsb_ && msb_) {
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
					"invaild part select : " << var_str.str() << 
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
				map<PExpr*, map<PExpr*, bool> > values;
				PENumber* expr = dynamic_cast<PENumber*>(msb_);
				assert(expr);
				map<unsigned, vector<unsigned> > bvalues;
				verinum* vn = msb_->evaluate(des, scope, nullptr, false, false, values, bvalues);
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
		ConcatItem* item = new ConcatItem;
		item->type_ = ConcatItem::STABLE;
		item->baseitem_.stable_ = new verinum(*v);
		delete v;
		res.push_back(item);
	} else {
		ConcatItem* item = new ConcatItem();
		item->baseitem_.variable_ = new ConcatItem::ConcatOfVcdVar;
		item->type_ = ConcatItem::VARIABLE;
		item->baseitem_.variable_->name_ = var_str.str();
		assert(vars.find(var_str.str()) != vars.end());
		RefVar* var = vars[var_str.str()];
		//Lsb or msb exit.
		if(bits_width.size()){   
			PExpr* lsb_ = bits_width.back().lsb;
			PExpr* msb_ = bits_width.back().msb;
			verinum* msn_ ;
			NetNet* net = scope->find_signal(var_str);

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
				assert(msv >= lsv);
				item->baseitem_.variable_->lwidth_ = msv;
				item->baseitem_.variable_->rwidth_ = lsv;
			}
			//Only msb exits and msb is a const type, we can get this bit directly, like "x[1]".
			else if (msb_ && (msn_ = msb_->eval_const(des, scope))) {
				unsigned long msv = msn_->as_ulong();
				unsigned idx = msv - var->lsb;
				item->baseitem_.variable_->lwidth_ = idx;
				item->baseitem_.variable_->rwidth_ = idx;
			}
			//Msb is a expr, we need to evaluate this expr.
			else if (msb_) {
				map<PExpr*, map<PExpr*, bool> > values;
				PENumber* expr = dynamic_cast<PENumber*>(msb_);
				assert(expr);
				map<unsigned, vector<unsigned> > bvalues;
				verinum* t = msb_->evaluate(des, scope, nullptr, false, false, values, bvalues);
				unsigned idx = t->as_ulong() - var->lsb;
				item->baseitem_.variable_->lwidth_ = idx;
				item->baseitem_.variable_->rwidth_ = idx;
				delete t;
			}
		}

		//Use the current value directly.
		else {
			item->baseitem_.variable_->lwidth_ = var->msb;
			item->baseitem_.variable_->rwidth_ = var->lsb;
		}
		res.push_back(item);
	}
	return res;
}

vector<ConcatItem*> PENumber::parse_concat_expr(Design*des, NetScope*scope, map<string, RefVar*>& vars)
{
	vector<ConcatItem*> res;
	long val = value_->as_long();
	verinum* v = new verinum(val);
	ConcatItem* item = new ConcatItem;
	item->type_ = ConcatItem::STABLE;
	item->baseitem_.stable_ = v;
	res.push_back(item);
	return res;
}

void ConcatItem::dump(ostream& out) {
	if(type_ == STABLE) {
		out << baseitem_.stable_->get_nbits() << "b'";
		baseitem_.stable_->dump(out);
	} else {
		out << baseitem_.variable_->name_ << "[" << baseitem_.variable_->lwidth_ << ":" << baseitem_.variable_->rwidth_ << "]";
	}
}

unsigned ConcatItem::dump_smt(ostringstream& out, map<string, RefVar*>& vars)
{
	if(type_ == ConcatItem::STABLE) {
		out << "#b";
		out << baseitem_.stable_->dumpstring();
		return baseitem_.stable_->get_nbits();
	} else {
		assert(vars.find(baseitem_.variable_->name_) != vars.end());
		RefVar* var = vars[baseitem_.variable_->name_];
		out << "((_ extract " << baseitem_.variable_->lwidth_ << " " << baseitem_.variable_->rwidth_ << ")";
		out << var->name << "_" << var->time << "_" << var->space << ")";
		return (baseitem_.variable_->lwidth_ - baseitem_.variable_->rwidth_ + 1);
	}
}

unsigned ConcatItem::dump_smt(ostringstream& out, unordered_map<string, SmtDefine*>& symbols)
{
	if(type_ == ConcatItem::STABLE) {
		out << "#b";
		out << baseitem_.stable_->dumpstring();
		return baseitem_.stable_->get_nbits();
	} else {
		assert(symbols.find(baseitem_.variable_->name_) != symbols.end());
		SmtDefine* var = symbols[baseitem_.variable_->name_];
		out << "((_ extract " << baseitem_.variable_->lwidth_ << " " << baseitem_.variable_->rwidth_ << ")";
		out << var->symbol->get_name() << "_" << var->state.first << "_" << var->state.second << ")";
		return (baseitem_.variable_->lwidth_ - baseitem_.variable_->rwidth_ + 1);
	}
}

vector<pair<VcdVar*, unsigned> > ConcatItem::get_asleft(VcdScope* instan)
{
	assert(type_ == VARIABLE && baseitem_.variable_);
	assert(instan->defines_.find(baseitem_.variable_->name_) != instan->defines_.end());
	VcdVar* var = instan->defines_[baseitem_.variable_->name_];
	vector<pair<VcdVar*, unsigned> > res;
	unsigned l = baseitem_.variable_->lwidth_;
	unsigned r = baseitem_.variable_->rwidth_;
	for(unsigned i = r; i <= l; i++) {
		res.push_back(make_pair(var, i));
	}
	return res;
}

verinum* ConcatItem::get_asright(VcdScope* instan)
{
	if(type_ == STABLE) {
		return new verinum(*(baseitem_.stable_));
	} else {
		unsigned l = baseitem_.variable_->lwidth_;
		unsigned r = baseitem_.variable_->rwidth_;
		unsigned width = l - r + 1;
		verinum* res = new verinum(uint64_t(0), width);
		assert(instan->defines_.find(baseitem_.variable_->name_) != instan->defines_.end());
		verinum value = instan->defines_[baseitem_.variable_->name_]->sim_val;
		for(unsigned i = 0; i < width; i++) {
			res->set(i, value.get(i + r));
		}
		return res;
	}
}

vector<pair<VcdVar*, unsigned> > PEConcat::get_asleft(VcdScope* instan)
{
	assert(!basevec.empty());
	vector<pair<VcdVar*, unsigned> > res;
	for(ConcatItem* item : basevec) {
		vector<pair<VcdVar*, unsigned> > temp = item->get_asleft(instan);
		res.insert(res.end(), temp.begin(), temp.end());
	}
	return res;
}

void verinum_equal(vector<pair<VcdVar*, unsigned> >& v1, verinum* v2)
{
	unsigned lpos = v1.size()-1;
	unsigned rpos = v2->get_nbits()-1;
	while(lpos || rpos) {
		if(lpos && rpos) {
			v1[lpos].first->sim_val.set(v1[lpos].second, v2->get(rpos));
			lpos--;
			rpos--;
		} else if (lpos) {
			v1[lpos].first->sim_val.set(v1[lpos].second, verinum::V0);
			lpos--;
		} else {
			break;
		}
	}
}

void verinum_equal(verinum* v1, verinum* v2)
{
	unsigned lpos = v1->get_nbits()-1;
	unsigned rpos = v2->get_nbits()-1;
	while(lpos || rpos) {
		if(lpos && rpos) {
			v1->set(lpos, v2->get(rpos));
			lpos--;
			rpos--;
		} else if (lpos) {
			v1->set(lpos, verinum::V0);
			lpos--;
		} else {
			break;
		}
	}
}

void PGAssign::evaluate(Design* des, NetScope* scope, VcdScope* instan, bool combine, bool branch)
{
	map<PExpr*, map<PExpr*, bool> > values;
	map<unsigned, vector<unsigned> > bvalues;
	PExpr* left = get_pins()[0];
	if(PEConcat* left_concat = dynamic_cast<PEConcat*>(left)) {
		if(left_concat->basevec.empty())
			left_concat->basevec = left_concat->parse_concat_expr(des, scope, instan->module_->vartab_);
		verinum* value = get_pins()[1]->evaluate(des, scope, instan, combine, branch, values, bvalues);
		vector<pair<VcdVar*, unsigned> > lvalue = left_concat->get_asleft(instan);
		verinum_equal(lvalue, value);
		delete value;
	} else if(PEIdent* left_ident = dynamic_cast<PEIdent*>(left)) {
		set<string> nameset = left_ident->get_var_names();
		assert(nameset.size() == 1);
		string name = *(nameset.begin());
		assert(instan->defines_.find(name) != instan->defines_.end());
		VcdVar* var = instan->defines_[name];
		verinum* value = get_pins()[1]->evaluate(des, scope, instan, combine, branch, values, bvalues);
		verinum_equal(&(var->sim_val), value);
		delete value;
	} else {
		cerr << "Wrong when evaluate the assign statement" << endl;
		dump(cerr);
		exit(1);
	}
	if(combine && !values.empty())
		instan->report_->add_cond_report(values);
	if(branch && !bvalues.empty())
		instan->report_->add_branch_report(bvalues.begin()->first, bvalues.begin()->second);
}

verinum* PExpr::evaluate(Design*des, NetScope*scope, VcdScope* instan, bool combine, bool branch, map<PExpr*, map<PExpr*, bool> >& values, map<unsigned, vector<unsigned> >& bvalues )
{
	return 0;
}

verinum* PEConcat::evaluate(Design*des, NetScope*scope, VcdScope* instan, bool combine, bool branch, map<PExpr*, map<PExpr*, bool> >& values, map<unsigned, vector<unsigned> >& bvalues )
{
	verinum v;

	/* Parse and record the bits when evaluate this expression first.
       For example, a concat expression{a[3:0], 4'b0011, c[3:0]}.
	   Convert it to a vector to evaluate easily.*/
	if(basevec.empty())
		basevec = parse_concat_expr(des, scope, instan->module_->vartab_);

	// Get the base vector, like mutiple items{a, b, c, d, e}.
	for(size_t i = 0; i < basevec.size(); i++) {
		verinum* temp = basevec[i]->get_asright(instan);
		v = concat(v, *temp);
		delete temp;
	}

	return new verinum(v);
}

verinum* PEEvent::evaluate(Design*des, NetScope*scope, VcdScope* instan, bool combine, bool branch, map<PExpr*, map<PExpr*, bool> >& values, map<unsigned, vector<unsigned> >& bvalues )
{
	return 0;
}

verinum* PEFNumber::evaluate(Design*des, NetScope*scope, VcdScope* instan, bool combine, bool branch, map<PExpr*, map<PExpr*, bool> >& values, map<unsigned, vector<unsigned> >& bvalues )
{
	long val = value_->as_long();
	verinum* v = new verinum(val);
	return v;
}

verinum* PEIdent::evaluate(Design*des, NetScope*scope, VcdScope* instan, bool combine, bool branch, map<PExpr*, map<PExpr*, bool> >& values, map<unsigned, vector<unsigned> >& bvalues )
{
	assert(scope);
	name_component_t varexpr =  path_.back();
	perm_string varname = varexpr.name;
	list<index_component_t> bits_width = varexpr.index;

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
	if(scope->find_parameter(varname, expr_pos)){
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
					"invaild part select : " << varname.str() << 
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
				verinum* vn = msb_->evaluate(des, scope, instan, combine, branch, values, bvalues);
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

		if(instan->defines_.find(varname.str()) == instan->defines_.end()){
			cerr << "Line : " << get_lineno() << " error : "
			"The signal used in condition expression are not dumped : " << path_ << endl;
			exit(1);
		}
		var = instan->defines_[varname.str()];
		vn = var->sim_val;

		//Lsb or msb exit.
		if(bits_width.size()){   
			PExpr* lsb_ = bits_width.back().lsb;
			PExpr* msb_ = bits_width.back().msb;
			verinum* msn_ ;
			NetNet* net = scope->find_signal(varname);

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
				verinum* t = msb_->evaluate(des, scope, instan, combine, branch, values, bvalues);
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

verinum* PENumber::evaluate(Design*des, NetScope*scope, VcdScope* instan, bool combine, bool branch, map<PExpr*, map<PExpr*, bool> >& values, map<unsigned, vector<unsigned> >& bvalues )
{
	return new verinum(value());
}

verinum* PEString::evaluate(Design*des, NetScope*scope, VcdScope* instan, bool combine, bool branch, map<PExpr*, map<PExpr*, bool> >& values, map<unsigned, vector<unsigned> >& bvalues )
{
	verinum* v = new verinum(string(text_));
	return v;
}

verinum* PEUnary::evaluate(Design*des, NetScope*scope, VcdScope* instan, bool combine, bool branch, map<PExpr*, map<PExpr*, bool> >& values, map<unsigned, vector<unsigned> >& bvalues )
{
	verinum* val = expr_->evaluate(des, scope, instan, combine, branch, values, bvalues);
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

verinum* PEBinary::evaluate(Design*des, NetScope*scope, VcdScope* instan, bool combine, bool branch, map<PExpr*, map<PExpr*, bool> >& values, map<unsigned, vector<unsigned> >& bvalues )
{
	verinum*l = left_->evaluate(des, scope, instan, combine, branch, values, bvalues);
	verinum*r = right_->evaluate(des, scope, instan, combine, branch, values, bvalues);
	
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

verinum* PETernary::evaluate(Design*des, NetScope*scope, VcdScope* instan, bool combine, bool branch, map<PExpr*, map<PExpr*, bool> >& values, map<unsigned, vector<unsigned> >& bvalues )
{
	verinum* vn = expr_->evaluate(des, scope, instan, combine, branch, values, bvalues);
	verinum* v;
	if(combine)
		values[this][expr_] = (*vn)[0] == verinum::V1;
	if(branch)
		bvalues[get_lineno()].push_back((*vn)[0] == verinum::V1 ? BRANCH_TRUE_VALUE : BRANCH_FALSE_VALUE);
	if((*vn)[0] == verinum::V1)
		v = tru_->evaluate(des, scope, instan, combine, branch, values, bvalues);
	else
		v = fal_->evaluate(des, scope, instan, combine, branch, values, bvalues);
	delete vn;
	return v;
}

verinum* PECallFunction::evaluate(Design*des, NetScope*scope, VcdScope* instan, bool combine, bool branch, map<PExpr*, map<PExpr*, bool> >& values, map<unsigned, vector<unsigned> >& bvalues )
{
	cerr << "Unsupported function call in expression!" << endl;
	exit(1);
}