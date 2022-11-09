
#include "PExpr.h"
#include "PGate.h"
#include "ivl_assert.h"
#include "smt_generator.h"
#include <sstream>

vector<ConcatItem*> PExpr::parse_concat_expr(NetInstan* idp)
{
	dump(cerr);
	cerr << " is unsupported in line " << get_fileline() << endl;
	exit(1);
	return vector<ConcatItem*>();
}

vector<ConcatItem*> PEConcat::parse_concat_expr(NetInstan* idp)
{
	if(ccvec.empty()) {
		for(PExpr* expr : parms_) {
			vector<ConcatItem*> temp = expr->parse_concat_expr(idp);
			ccvec.insert(ccvec.end(), temp.begin(), temp.end());
		}
		if(repeat_) {
			PENumber* num = dynamic_cast<PENumber*>(repeat_);
			assert(num);
			unsigned repeat_n = num->value().as_unsigned();
			vector<ConcatItem*> vec_temp = ccvec;
			for(unsigned i = 0; i < repeat_n-1; i++) {
				ccvec.insert(ccvec.end(), vec_temp.begin(), vec_temp.end());
			}
		}
	}
	return ccvec;
}

vector<ConcatItem*> PEIdent::parse_concat_expr(NetInstan* idp)
{
    name_component_t expr =  path_.back();
    string name = expr.name.str();
    list<index_component_t> width = expr.index;

    ConcatItem* ccitem = nullptr;

    //variable expression is a memory
    if(width.size() > 1){
        cerr << "Line : " << get_lineno() << " error :  Memory values can not reported in vcd file , please use reg or wire in this condition expression" << endl;
        exit(1);
    }

    //verinum result of this expr.
    verinum* v = 0;

    //The expr is a parameter.
    if(idp->find_parameter(name)){
        //Expr has lsb or msb , such as using "x[1:0]",  "x[1]".
        if(width.size()){
            PExpr* lsb_ = width.back().lsb;
            PExpr* msb_ = width.back().msb;
            //Lsb and msb both exist, then we copy the part from lsb to msb of value.
            if(lsb_ && msb_){
                verinum* lsn = lsb_->eval_const(idp->get_module()->get_design(), idp->get_scope());
                verinum* msn = msb_->eval_const(idp->get_module()->get_design(), idp->get_scope());

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
                        "invaild part select : " << name <<
                        "[" << msb << ":" << lsb << "]" << endl;
                    exit(1);
                }

                verinum result(verinum::V0, msb-lsb+1, true);
                verinum exl = idp->get_parameter(name);

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
                verinum* vn = msb_->evaluate(idp, nullptr);
                NetEConst* le = dynamic_cast<NetEConst*>(msb_);

                if (le){
                    verinum lv = idp->get_parameter(name);
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
            v = new verinum(idp->get_parameter(name));
        }
        ccitem = new ConcatItem;
        ccitem->type_ = ConcatItem::STABLE;
        ccitem->baseitem_.stable_ = v;
    }
    //The expr is a variable, we need to get the value from vcd var.
    else {
        verinum vn;
        NetSymbol* var = 0;
        if(!idp->find_symbol(name)){
            cerr << "Line : " << get_lineno() << " error : "
                                                 "The signal used in condition expression are not dumped : " << path_ << endl;
            exit(1);
        }

        var = idp->get_symbol(name);
        vn = *(var->get_sim_val());

        ccitem = new ConcatItem;
        ccitem->type_ = ConcatItem::VARIABLE;
        ccitem->baseitem_.variable_ = new ConcatItem::ConcatOfVar;
        ccitem->baseitem_.variable_->name = var->get_name();

        //Lsb or msb exit.
        if(width.size()){
            PExpr* lsb_ = width.back().lsb;
            PExpr* msb_ = width.back().msb;
            verinum* msn_ ;
            //Lsb and msb exit, we select the part form lsb to msb of current value.
            if (lsb_){
                assert(msb_);
                verinum* lsn = lsb_->eval_const(idp->get_module()->get_design(), idp->get_scope());
                verinum* msn = msb_->eval_const(idp->get_module()->get_design(), idp->get_scope());
                assert(lsn);
                assert(msn);
                long lsv = lsn->as_long();
                long msv = msn->as_long();
                assert(msv >= lsv);
                ccitem->baseitem_.variable_->msi = msv;
                ccitem->baseitem_.variable_->lsi = lsv;
            }
            //Only msb exits and msb is a const type, we can get this bit directly, like "x[1]".
            else if (msb_ && (msn_ = msb_->eval_const(idp->get_module()->get_design(), idp->get_scope()))) {
                unsigned long msv = msn_->as_ulong();
                ccitem->baseitem_.variable_->msi = msv;
                ccitem->baseitem_.variable_->lsi = msv;
            }
            //Msb is a expr, we need to evaluate this expr.
            else if (msb_) {
                verinum* t = msb_->evaluate(idp, nullptr);
                unsigned long msv = t->as_ulong();
                ccitem->baseitem_.variable_->msi = msv;
                ccitem->baseitem_.variable_->lsi = msv;
                delete t;
            }
        }
        //Use the current value directly.
        else {
            ccitem->baseitem_.variable_->msi = var->get_msb();
            ccitem->baseitem_.variable_->lsi = var->get_lsb();
        }
    }
    vector<ConcatItem*> result;
    result.push_back(ccitem);
    return result;
}

vector<ConcatItem*> PENumber::parse_concat_expr(NetInstan* idp)
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

vector<pair<NetSymbol*, unsigned> > ConcatItem::get_asleft(NetInstan* idp)
{
	assert(type_ == VARIABLE && baseitem_.variable_);
	assert(idp->find_symbol(baseitem_.variable_->name));
	NetSymbol* var = idp->get_symbol(baseitem_.variable_->name);
	vector<pair<NetSymbol*, unsigned> > res;
	unsigned l = baseitem_.variable_->msi;
	unsigned r = baseitem_.variable_->lsi;
	for(unsigned i = r; i <= l; i++) {
		res.push_back(make_pair(var, i - var->get_lsb()));
	}
	return res;
}

verinum* ConcatItem::get_asright(NetInstan* idp)
{
	if(type_ == STABLE) {
		return new verinum(*(baseitem_.stable_));
	} else {
		unsigned l = baseitem_.variable_->msi;
		unsigned r = baseitem_.variable_->lsi;
		unsigned width = l - r + 1;
		verinum* res = new verinum(uint64_t(0), width);
		assert(idp->find_symbol(baseitem_.variable_->name));
        NetSymbol* var = idp->get_symbol(baseitem_.variable_->name);
		verinum value = *(var->get_sim_val());
		for(unsigned i = 0; i < width; i++) {
			res->set(i, value.get(i + r - var->get_lsb()));
		}
		return res;
	}
}

vector<pair<NetSymbol*, unsigned> > PEConcat::evaluate_as_left(NetInstan* idp)
{
	assert(!ccvec.empty());
	vector<pair<NetSymbol*, unsigned> > res;
	for(ConcatItem* item : ccvec) {
		vector<pair<NetSymbol*, unsigned> > temp = item->get_asleft(idp);
		res.insert(res.end(), temp.begin(), temp.end());
	}
	return res;
}

void verinum_equal(vector<pair<NetSymbol*, unsigned> >& v1, verinum* v2)
{
	unsigned lpos = v1.size()-1;
	unsigned rpos = v2->get_nbits()-1;
	while(lpos || rpos) {
		if(lpos && rpos) {
			v1[lpos].first->get_sim_val()->set(v1[lpos].second, v2->get(rpos));
			lpos--;
			rpos--;
		} else if (lpos) {
			v1[lpos].first->get_sim_val()->set(v1[lpos].second, verinum::V0);
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

void PGAssign::evaluate(NetInstan* idp, DesignCoverage* dcov)
{
	PExpr* left = get_pins()[0];
	if(PEConcat* left_concat = dynamic_cast<PEConcat*>(left)) {
		if(left_concat->ccvec.empty())
			left_concat->parse_concat_expr(idp);
		verinum* value = get_pins()[1]->evaluate(idp, dcov);
		vector<pair<NetSymbol*, unsigned> > lvalue = left_concat->evaluate_as_left(idp);
		verinum_equal(lvalue, value);
		delete value;
	} else if(PEIdent* left_ident = dynamic_cast<PEIdent*>(left)) {
		set<string> nameset = left_ident->get_var_names();
		assert(nameset.size() == 1);
		string name = *(nameset.begin());
		assert(idp->find_symbol(name));
		NetSymbol* var = idp->get_symbol(name);
		verinum* value = get_pins()[1]->evaluate(idp, dcov);
		verinum_equal(var->get_sim_val(), value);
		delete value;
	} else {
		cerr << "Wrong when evaluate the assign statement" << endl;
		dump(cerr);
		exit(1);
	}
}

verinum* PExpr::evaluate(NetInstan* idp, DesignCoverage* dcov)
{
	return 0;
}

verinum* PEConcat::evaluate(NetInstan* idp, DesignCoverage* dcov)
{
	verinum v;

	/* Parse and record the bits when evaluate this expression first.
       For example, a concat expression{a[3:0], 4'b0011, c[3:0]}.
	   Convert it to a vector to evaluate easily.*/
	if(ccvec.empty())
		parse_concat_expr(idp);

	// Get the base vector, like mutiple items{a, b, c, d, e}.
	for(size_t i = 0; i < ccvec.size(); i++) {
		verinum* temp = ccvec[i]->get_asright(idp);
		v = concat(v, *temp);
		delete temp;
	}

	return new verinum(v);
}

verinum* PEEvent::evaluate(NetInstan* idp, DesignCoverage* dcov)
{
	return 0;
}

verinum* PEFNumber::evaluate(NetInstan* idp, DesignCoverage* dcov)
{
	long val = value_->as_long();
	verinum* v = new verinum(val);
	return v;
}

verinum* PEIdent::evaluate(NetInstan* idp, DesignCoverage* dcov)
{
	name_component_t expr =  path_.back();
	perm_string name = expr.name;
	list<index_component_t> width = expr.index;

	//variable expression is a memory
	if(width.size() > 1){
		cerr << "Line : " << get_lineno() << " error : "
		" Memory values can not reported in vcd file , please use reg or wire in this condition expression" << endl;
		exit(1);
	}

	//verinum result of this expr.
	verinum* v = 0;
	
	//The expr is a parameter.
	if(idp->find_parameter(name.str())){
		//Expr has lsb or msb , such as using "x[1:0]",  "x[1]".
		if(width.size()){
			PExpr* lsb_ = width.back().lsb;
			PExpr* msb_ = width.back().msb;
			//Lsb and msb both exist, then we copy the part from lsb to msb of value.
			if(lsb_ && msb_){
				verinum* lsn = lsb_->eval_const(idp->get_module()->get_design(), idp->get_scope());
				verinum* msn = msb_->eval_const(idp->get_module()->get_design(), idp->get_scope());

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
					"invaild part select : " << name.str() << 
					"[" << msb << ":" << lsb << "]" << endl;
					exit(1);
				}

				verinum result(verinum::V0, msb-lsb+1, true);
				verinum exl = idp->get_parameter(name.str());

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
				verinum* vn = msb_->evaluate(idp, dcov);
				NetEConst* le = dynamic_cast<NetEConst*>(msb_);

				if (le){
					verinum lv = idp->get_parameter(name.str());
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
			v = new verinum(idp->get_parameter(name.str()));
		}
	}
	//The expr is a variable, we need to get the value from vcd var.
	else {
		verinum vn;
		NetSymbol* var = 0;

		if(!idp->find_symbol(name.str())){
			cerr << "Line : " << get_lineno() << " error : "
			"The signal used in condition expression are not dumped : " << path_ << endl;
			exit(1);
		}

		var = idp->get_symbol(name.str());
		vn = *(var->get_sim_val());

		//Lsb or msb exit.
		if(width.size()){   
			PExpr* lsb_ = width.back().lsb;
			PExpr* msb_ = width.back().msb;
			verinum* msn_ ;
			//Lsb and msb exit, we select the part form lsb to msb of current value.
			if (lsb_){
				assert(msb_);

				verinum* lsn = lsb_->eval_const(idp->get_module()->get_design(), idp->get_scope());
				verinum* msn = msb_->eval_const(idp->get_module()->get_design(), idp->get_scope());

				assert(lsn);
				assert(msn);
				
				long lsv = lsn->as_long();
				long msv = msn->as_long();

                assert(msv >= lsv);

				unsigned long wid = msv - lsv + 1;

				v = new verinum(verinum::Vx, wid); 

				for(unsigned idx = 0; idx < wid; ++idx){
					if((lsv + idx) >= vn.get_nbits())
						v->set(idx, verinum::V0);
					else
						v->set(idx, vn[lsv + idx]);
				}
			}

			//Only msb exits and msb is a const type, we can get this bit directly, like "x[1]".
			else if (msb_ && (msn_ = msb_->eval_const(idp->get_module()->get_design(), idp->get_scope()))) {
				unsigned long msv = msn_->as_ulong();
				unsigned idx = msv - var->get_lsb();
				v = new verinum(vn[idx], 1);
			}
			//Msb is a expr, we need to evaluate this expr.
			else if (msb_) {
				verinum* t = msb_->evaluate(idp, dcov);
				unsigned idx = t->as_ulong() - var->get_lsb();
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

verinum* PENumber::evaluate(NetInstan* idp, DesignCoverage* dcov)
{
	return new verinum(value());
}

verinum* PEString::evaluate(NetInstan* idp, DesignCoverage* dcov)
{
	verinum* v = new verinum(string(text_));
	return v;
}

verinum* PEUnary::evaluate(NetInstan* idp, DesignCoverage* dcov)
{
	verinum* val = expr_->evaluate(idp, dcov);
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
	//delete val;
	return v;
}

verinum* PEBinary::evaluate(NetInstan* idp, DesignCoverage* dcov)
{
	verinum*l = left_->evaluate(idp, dcov);
	verinum*r = right_->evaluate(idp, dcov);

	if(op_ == 'a' || op_ == '0') {
		if(CHECK_COVERAGE(EXPRESSION_COVERAGE, dcov->get_coverage())) {
			if(idp->get_module()->pp_reverse_expr.find(left_) != idp->get_module()->pp_reverse_expr.end()) {
                ExpressionNode *node = idp->get_module()->pp_reverse_expr[left_];
                if (dcov->sim_expr.find(node) == dcov->sim_expr.end()) {
                    dcov->sim_expr[node] = new CoverBitVecArray(node->item.size());
                }
                if (l->as_unsigned())
                    dcov->sim_expr[node]->set_high(node->item[left_]);
            }
            if(idp->get_module()->pp_reverse_expr.find(right_) != idp->get_module()->pp_reverse_expr.end()) {
                ExpressionNode *node = idp->get_module()->pp_reverse_expr[right_];
                if (dcov->sim_expr.find(node) == dcov->sim_expr.end()) {
                    dcov->sim_expr[node] = new CoverBitVecArray(node->item.size());
                }
                if (l->as_unsigned())
                    dcov->sim_expr[node]->set_high(node->item[right_]);
            }
		}
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

verinum* PETernary::evaluate(NetInstan* idp, DesignCoverage* dcov)
{
	verinum* vn = expr_->evaluate(idp, dcov);
	verinum* v;

	if(CHECK_COVERAGE(EXPRESSION_COVERAGE, dcov->get_coverage())) {
		ExpressionNode* node = idp->get_module()->pp_expr[this];
		assert(dcov->sim_expr[node] == nullptr);
		dcov->sim_expr[node] = new CoverBitVecArray(node->item.size());
		if((*vn)[0] == verinum::V1) {
			dcov->sim_expr[node]->set_high(node->item[this]);
		}
	}

    if(CHECK_COVERAGE(BRANCH_COVERAGE, dcov->get_coverage())) {
        BranchNode* node = idp->get_module()->pp_branch[expr_->get_lineno()];
        dcov->sim_branch[node].push_back((*vn)[0] == verinum::V1 ? TRUE_BRANCH : FALSE_BRANCH);
    }

	if((*vn)[0] == verinum::V1)
		v = tru_->evaluate(idp, dcov);
	else
		v = fal_->evaluate(idp, dcov);

	delete vn;
	return v;
}

verinum* PECallFunction::evaluate(NetInstan* idp, DesignCoverage* dcov)
{
	cerr << "Unsupported function call in expression!" << endl;
	exit(1);
}