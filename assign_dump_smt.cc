# include "PGate.h"
# include "testpath.h"
# include "PExpr.h"
# include <fstream>
# include <sstream>
# include "Module.h"
# include "smt_generator.h"

# define RIGHT true
# define LEFT false

bool left_expression = false;

void PGAssign::dump_smt(Design* design, ostream& o, map<string, RefVar*>& vars, set<SmtVar*>& used, Module* md, unsigned cur_time) const
{
    //We can't generate the expression like " assign {a[1:0], b[1:0]} = 4'b0000" temporarily.
	if(pin(0)->get_var_names().size() > 1)
	{
		cerr << get_fileline() << " : Concatation expression isn't supported now" << endl;
		exit(0);
	}

	//Generate the respective SMT formats for left and right expressions.
	//Then verify the formats of expressions are correctly.
	ostringstream l_expr, r_expr;
	hname_t name(md->pscope_name());
	NetScope* scope = design->find_scope(name);
	assert(scope);

    int r_width = pin(1)->dump_smt(design, scope, vars, used, r_expr, md, RIGHT, cur_time);

	int l_width = pin(0)->dump_smt(design, scope, vars, used, l_expr, md, LEFT, cur_time);

	if(l_width == SMT_WRONG)
	{
		cerr << get_fileline() << " Can't find left variable in lrefences." << endl;
		exit(0);
	}

	assert(r_width != SMT_NULL);
	assert(l_width != SMT_INT && l_width != SMT_NULL && l_width != SMT_BOOL);

	//Establish an equality relationship between the left and right expressions.
	//If the bit widths of the left and right expressions are different,
	//convert the right expression according to the width of left expression.
	ostringstream r_equal;
	if(l_width == r_width) r_equal << r_expr.str();
	else
	{
		//Right expression is a int number, then convert it to a bitvec.
		//For example, "assign a = 3" or "assign a = 2'b11 (Convert to Int)".
		if(r_width == SMT_INT) int_to_bv(r_expr, l_width, r_equal);
		//Right expression is one bit but bool type
		//For example "assign a = b == c "
		else if(r_width == SMT_BOOL) bool_to_bv(r_expr, r_equal); 
		//The width of right is unequal to left, convert to fit the left width.
		//For example, "assign a = b" with a.width = 3 and b.width = 4.
		else bv_int_bv(r_expr, l_width, r_equal);
	}
	o << "(assert" << "(= " << l_expr.str() << " " << r_equal.str() << "))" << endl;

}

int PExpr::dump_smt(Design* design, NetScope* scope, map<string, RefVar*>& vars, set<SmtVar*>& used, ostringstream& expr, Module* md, bool type, unsigned cur_time) const
{
    return SMT_NULL;
}

int PENumber::dump_smt(Design* design, NetScope* scope, map<string, RefVar*>& vars, set<SmtVar*>& used, ostringstream& expr, Module* md, bool type, unsigned cur_time) const
{
    expr << value_->as_unsigned();
	return SMT_INT;
}

int PEIdent::dump_smt(Design* design, NetScope* scope, map<string, RefVar*>& vars, set<SmtVar*>& used, ostringstream& expr, Module* md, bool type, unsigned cur_time) const
{
	int width;
	string vn =  path_.back().name.str();
	list<index_component_t> bits_width = path_.back().index;

	//variable expression is a memory
	if(bits_width.size() > 1)
	{
		cerr << "Line : " << get_lineno() << " error : "
		" Memory is unsupported!" << endl;
		exit(1);
	}

	NetScope::param_ref_t expr_pos;
	
	//The expr is a parameter.
	if(scope->find_parameter(perm_string(vn.c_str()), expr_pos))
	{
		verinum* v =0;
		//No lsb and msb means that we can use the value of parameter from where it defined.
		NetExpr* tmp = expr_pos->second.val;
		NetEConst*ctmp = dynamic_cast<NetEConst*>(tmp);
		if (ctmp == 0) {
			cerr << get_lineno() << ": internal error: Unable to evaluate "
				<< "unconstant expression (parameter=" << path_
				<< "): " << *ctmp << endl;
			exit(1);
		}
		v = new verinum(ctmp->value());
		unsigned value = v->as_unsigned();
		expr << value; 
		width = SMT_INT;
	}

	else if(vars.find(vn) != vars.end())
	{

		RefVar* var = vars[vn];

		if(!type)
		{
			var->space = var->time == cur_time ? var->space + 1 : 0;
			var->time = cur_time;
		}

		ostringstream name;

		name << var->name << "_" << var->time << "_" << var->space;

		if(!var->record || !type)
		{
			SmtVar* sv = new SmtVar;
			sv->smtname = name.str();
			sv->basename = var->name;
			sv->lsb = var->lsb;
			sv->msb = var->msb;
			sv->width = var->width;
			sv->type = var->ptype;
			var->record = true;
			sv->temp_flag = false;
			used.insert(sv);
		}

		if(!bits_width.empty())
		{

			PExpr* lsb_ = bits_width.back().lsb;
			PExpr* msb_ = bits_width.back().msb;
			
			if(lsb_ && msb_)
			{
				verinum* lsn = lsb_->eval_const(design, scope);
				verinum* msn = msb_->eval_const(design, scope);

				long lsb = lsn->as_long();
				long msb = msn->as_long();

				extract(name, msb, lsb, expr);
				width = msb - lsb + 1;
			}

			else if(msb_)
			{
				long msb = msb_->eval_const(design, scope)->as_long();
				extract(name, msb, msb, expr);
				width = 1;
			}

			else
			{
				expr << name.str();
				width = var->width;
			}
		}

		else
		{
			expr << name.str();
			width = var->width;
		}
			
	}

	else
	{
		cerr << get_fileline() << " Can't find " << vn << " in reference set!" << endl;
		exit(1);
	}

	return width;
}

int PEUnary::dump_smt(Design* design, NetScope* scope, map<string, RefVar*>& vars, set<SmtVar*>& used, ostringstream& expr, Module* md, bool type, unsigned cur_time) const
{
	ostringstream u_expr;

	int width = expr_->dump_smt(design, scope, vars, used, u_expr, md, type, cur_time);

	assert(width != SMT_NULL);

	//Form as !V, the type of V is uncertain, convert V to a Bool expression and do negation.
	if(op_ == '!'){
		ostringstream u_bool;

		if(width == SMT_BOOL) 
			u_bool << "(= " << u_expr.str() << " " << "false" << ")";

		else if(width == SMT_INT) 
			u_bool << "(= " << u_expr.str() << " " << "0" << ")";

		else 
			bv_compare_zero(u_expr, "=", width, u_bool);

		expr << "(ite " << u_bool.str() << " " << "1" << " " << "0" << ")";  

		return SMT_INT;
	}

	//Form as +V, -V, ~V, the type is BitVec, others is unsupported now.
	else if(op_ == '~' || op_ == '+' || op_ == '-'){
		switch(op_){
			case '~' : 
				expr << "((_ bvnot " << width << ")" << u_expr.str() << ")";
				break;
			case '-' : 
				expr << "((_ bvneg " << width << ")" << u_expr.str() << ")";
				break;
			case '+' : 
				expr << u_expr.str();
		}
		return width;
	}

	//Form as |V, ~|V and so on, the type of V is BitVec, and it returns only one bit.
	else if(SMT_Vec_Bits[op_]){
		expr << "(" << SMT_Vec_Bits[op_];
		for(int i = width-1; i >= 0; i--){
			expr << " " << "((_ extract " << i << " " << i << ")" << u_expr.str() << ")";
		}
		expr << ")";
		return 1;
	}

	else{
		cerr << get_fileline() << " : This NetEUnary is unpported for smt generation!" << endl;
		exit(1);
	}
}

int PEBinary::dump_smt(Design* design, NetScope* scope, map<string, RefVar*>& vars, set<SmtVar*>& used, ostringstream& expr, Module* md, bool type, unsigned cur_time) const
{
	ostringstream l_expr, r_expr;
	int l_width, r_width;
	int width;

	l_width = left_->dump_smt(design, scope, vars, used, l_expr, md, type, cur_time);
	r_width = right_->dump_smt(design, scope, vars, used, r_expr, md, type, cur_time);

	if(SMT_Vec_Add[op_])
	{
		assert(l_width >= SMT_INT);
		assert(r_width >= SMT_INT);

		if(l_width == r_width && l_width != SMT_INT)
		{
			expr << "(" << SMT_Vec_Add[op_] << " " << l_expr.str() << " " << r_expr.str() << ")";

			width = l_width;
		}
		
		else
		{
			ostringstream l_int, r_int;

			if(l_width != SMT_INT) 
				bv_to_int(l_expr, l_int);
			else 
				l_int << l_expr.str();

			if(r_width != SMT_INT) 
				bv_to_int(r_expr, r_int);
			else 
				r_int << r_expr.str();

			expr << "(" << op_ << " " << l_int.str() << " " << r_int.str() << ")";

			width = SMT_INT;
		}

		return width;
	}

	if(SMT_Vec_Bits[op_])
	{
		ostringstream l_expr, r_expr;
		int l_width, r_width;
		int width;

		l_width = left_->dump_smt(design, scope, vars, used, r_expr, md, type, cur_time);
		r_width = right_->dump_smt(design, scope, vars, used, r_expr, md, type, cur_time);

		assert(l_width >= SMT_INT);
		assert(r_width >= SMT_INT);

		if(l_width == r_width && l_width != SMT_INT)
		{
			expr << "(" << SMT_Vec_Bits[op_] << " " << l_expr.str() << " " << r_expr.str() << ")";

			width = l_width;
		}

		else
		{
			ostringstream l_max, r_max;

			assert(l_width < SMT_MAX && r_width < SMT_MAX);
			
			if(l_width == SMT_INT)
				int_to_bv(l_expr, SMT_MAX, l_max);
			else
				bv_int_bv(l_expr, SMT_MAX, l_max);

			if(r_width == SMT_INT)
				int_to_bv(r_expr, SMT_MAX, r_max);
			else
				bv_int_bv(r_expr, SMT_MAX, r_max);

			expr << "(" << SMT_Vec_Bits[op_] << " " << l_max.str() << " " << r_max.str() << ")";
			
			width = SMT_MAX;
		}

		return width;
	}

	if(SMT_Vec_Comp[op_])
	{
		assert(l_width >= SMT_INT);
		assert(r_width >= SMT_INT);

		if(l_width == r_width && l_width != SMT_INT)
		{
			expr << "(" << SMT_Vec_Comp[op_] << " " << l_expr.str() << " " << r_expr.str() << ")";
		}

		else
		{
			ostringstream l_int, r_int;

			if(l_width != SMT_INT)
				bv_to_int(l_expr, l_int);
			else
				l_int << l_expr.str();

			if(r_width != SMT_INT)
				bv_to_int(r_expr, r_int);
			else
				r_int << r_expr.str();
			
			expr << "(" << SMT_Int_Comp[op_] << " " << l_int.str() << " " << r_int.str() << ")";
		}

		width = SMT_BOOL;

		return width;
	}

	if(SMT_Vec_Div[op_])
	{
		assert(l_width >= SMT_INT);
		assert(r_width >= SMT_INT);

		if(l_width == r_width && l_width != SMT_INT)
		{
			expr << "(" << SMT_Vec_Div[op_] << " " << l_expr.str() << " " << r_expr.str() << ")";

			width = l_width;
		}
		
		else
		{
			ostringstream l_int, r_int;

			if(l_width != SMT_INT) 
				bv_to_int(l_expr, l_int);
			else 
				l_int << l_expr.str();

			if(r_width != SMT_INT) 
				bv_to_int(r_expr, r_int);
			else 
				r_int << r_expr.str();

			expr << "(" << op_ << " " << l_int.str() << " " << r_int.str() << ")";

			width = SMT_INT;
		}

		return width;
	}

	if(SMT_Bool_Logic[op_])
	{
		if(l_width == r_width && l_width == SMT_BOOL)
		{
			expr << "(" << SMT_Bool_Logic[op_] << " " << l_expr.str() << " " << r_expr.str() << ")";
		}

		else
		{
			ostringstream l_bool, r_bool;

			if(l_width != SMT_BOOL) {
				if(l_width == SMT_INT)
					int_compare_zero(l_expr, "distinct", l_bool);
				else
					bv_compare_zero(l_expr, "distinct", l_width, l_bool);
			}

			if(r_width != SMT_BOOL) {
				if(r_width == SMT_INT)
					int_compare_zero(r_expr, "distinct", r_bool);
				else
					bv_compare_zero(r_expr, "distinct", r_width, r_bool);
			}
			
			expr << "(" << SMT_Bool_Logic[op_] << " " << l_bool.str() << " " << r_bool.str() << ")";
		}

		width = SMT_BOOL;

		return width;
	}

	if(SMT_Vec_Mult[op_])
	{
		assert(l_width >= SMT_INT);
		assert(r_width >= SMT_INT);

		if(l_width == r_width && l_width != SMT_INT)
		{
			expr << "(" << SMT_Vec_Mult[op_] << " " << l_expr.str() << " " << r_expr.str() << ")";

			width = l_width;
		}
		
		else
		{
			ostringstream l_int, r_int;

			if(l_width != SMT_INT) 
				bv_to_int(l_expr, l_int);
			else 
				l_int << l_expr.str();

			if(r_width != SMT_INT) 
				bv_to_int(r_expr, r_int);
			else 
				r_int << r_expr.str();

			expr << "(" << op_ << " " << l_int.str() << " " << r_int.str() << ")";

			width = SMT_INT;
		}

		return width;
	}

	cerr << get_fileline() << " : Pow operator is unspported!" << endl;
	exit(1);
	return SMT_NULL;

}

int PETernary::dump_smt(Design* design, NetScope* scope, map<string, RefVar*>& vars, set<SmtVar*>& used, ostringstream& expr, Module* md, bool type, unsigned cur_time) const
{
	int i_width, t_width, e_width;
	int width;

	ostringstream i_expr, t_expr, e_expr;

	i_width = expr_->dump_smt(design, scope, vars, used, i_expr, md, type, cur_time);
	t_width = tru_->dump_smt(design, scope, vars, used, t_expr, md, type, cur_time);
	e_width = fal_->dump_smt(design, scope, vars, used, e_expr, md, type, cur_time);

	assert(i_width != SMT_NULL);
	assert(t_width != SMT_NULL);
	assert(e_width != SMT_NULL);

	if(t_width == SMT_BOOL || e_width == SMT_BOOL){
		cerr << get_fileline() << " True and else expressions in ternary are not allowed using bool expressions!" << endl;
		exit(1);
	}

	//Generate SMT for if expression
	ostringstream i_expr_bool;
	switch(i_width){
		//The format is like "a > 0 ? t_expr : e_expr"
		case SMT_BOOL : 
			i_expr_bool << i_expr.str(); 
			break;
		//The format is like "2 - 1 ? t_expr : e_expr", usually the format like this is not allowed
		case SMT_INT  : 
			i_expr_bool << "(ite " << "(distinct " << i_expr.str() << " 0) true false)"; 
			break;
		//The format is like "a[2:0] ? t_expr : e_expr", usually is not allowed too.
		default : 
			i_expr_bool << "(ite ";
			bv_compare_zero(i_expr, "distinct", i_width, i_expr_bool);
			i_expr_bool << " true false)";
			break;
	}

	expr << "(ite" << " " << i_expr_bool.str() << " ";

	// If t_expr and e_expr have different widths,
    // convert the BitVec to the Int and return Int type.

	if(t_width == e_width){
		width = t_width;
		expr << t_expr.str() << " " << e_expr.str();
	}

	else{

		width = SMT_INT;
		ostringstream t_expr_int;
		ostringstream e_expr_int;

		if(t_width != SMT_INT) 
			bv_to_int(t_expr, t_expr_int);
		else 
			t_expr_int << t_expr.str();

		if(e_width != SMT_INT) 
			bv_to_int(e_expr, e_expr_int);
		else 
			e_expr_int << e_expr.str();

		expr << t_expr_int.str() << " " << e_expr_int.str();
	}

	expr << ")";
	return width;
}

int PEConcat::dump_smt(Design* design, NetScope* scope, map<string, RefVar*>& vars, set<SmtVar*>& used, ostringstream& expr, Module* md, bool type, unsigned cur_time) const
{
	int res = 0;
	if(basevec.empty()) {
		PEConcat* p = const_cast<PEConcat*>(this);
		p->parse_concat_expr(md->get_design(), md->get_scope(), vars);
	}
	expr << "(concat";
	for(ConcatItem* item : basevec) {
		ostringstream cc;
		res += item->dump_smt(cc, vars);
		expr << " " << cc.str();
	}
	expr << ")";
	return res;
}

void PGAssign::dump_design_smt(ostream &out, InstanModule *instan) const
{
	//Generate the respective SMT formats for left and right expressions.
	//Then verify the formats of expressions are correctly.
	ostringstream l_expr, r_expr;
	hname_t name(instan->instan->get_module()->pscope_name());
	NetScope* scope = instan->instan->get_module()->get_design()->find_scope(name);
	assert(scope);

    int r_width = pin(1)->dump_design_smt(out, r_expr, instan);

//	set<string> change = pin(0)->get_var_names();
//	for(string s : change)
//	{
//		assert(instan->define.find(s) != instan->define.end());
//		SmtDefine* var = instan->define[s];
//		var->update(out, SmtDefine::UpdateType::SPACEUPDATE, 0, nullptr);
//	}

    left_expression = true;
	int l_width = pin(0)->dump_design_smt(out, l_expr, instan);
    left_expression = false;

	if(l_width == SMT_WRONG)
	{
		cerr << get_fileline() << " Can't find left variable in lrefences." << endl;
		exit(0);
	}

	assert(r_width != SMT_INT);

	//Establish an equality relationship between the left and right expressions.
	//If the bit widths of the left and right expressions are different,
	//convert the right expression according to the width of left expression.
	ostringstream r_bv;
	if(r_width == SMT_BOOL){
		bool_to_bv(r_expr, r_bv);
		r_width = 1;
	}
	else
		r_bv << r_expr.str();

	ostringstream r_equal;
	if(r_width != l_width)
		bv_to_bv(r_bv, r_width, l_width, r_equal);
	else
		r_equal << r_bv.str();

	out << "(assert" << "(= " << l_expr.str() << " " << r_equal.str() << ") )" << endl;
}

int PExpr::dump_design_smt(ostream& o, ostringstream &out, InstanModule *instan) const
{
	cout << get_fileline() << "(?" << typeid(*this).name() << "?)";
	return SMT_NULL;
}

int PEConcat::dump_design_smt(ostream& o, ostringstream &out, InstanModule *instan) const
{
	list<string> items;
	unsigned width = 0;

	for(PExpr* expr : parms_) 
	{
		ostringstream item;
		int i = expr->dump_design_smt(o, item, instan);
        ostringstream bv_item;
        if(i == SMT_BOOL) {
            bool_to_bv(item, bv_item);
            i = 1;
        }
        else
            bv_item << item.str();
		width += i;
		items.push_back(bv_item.str());
	}
	assert(!items.empty());

	unsigned rnum = 1;
	if(repeat_)
	{
		PENumber* num = dynamic_cast<PENumber*>(repeat_);
		if(!num)
            rnum = get_param_value(instan->instan->get_module(), repeat_);
		else
            rnum = num->value().as_unsigned();
	}


	if(items.size() == 1 && rnum == 1)
	{
		out << items.front();
	}
	else
	{
		width *= rnum;
		out << "(concat";
		for(size_t i = 0; i < rnum; i++)
		{
			for(string item : items)
			{
				out << " " << item;
			}
			out << " ";
		}
		out << ")";
	}
	return width;
}

int PETernary::dump_design_smt(ostream& o, ostringstream &out, InstanModule *instan) const
{
	//Using ite(if true_expr else_expr) form to generate the smt
	ostringstream i_expr, t_expr, e_expr;
	int i_width, t_width, e_width;

	i_width = expr_->dump_design_smt(o, i_expr, instan);
	t_width = tru_->dump_design_smt(o, t_expr, instan);
	e_width = fal_->dump_design_smt(o, e_expr, instan);

	assert(t_width > SMT_INT);
	assert(e_width > SMT_INT);

	ostringstream i_bool;
	if(i_width != SMT_BOOL)
		bv_or_int_to_bool(i_expr, i_width, i_bool);
	else
		i_bool << i_expr.str();

	out << "(ite " << i_bool.str() << " ";

	// If t_expr and e_expr have different widths,
	// convert the BitVec to max width.

	int width = max(t_width, e_width);
	ostringstream t_bv, e_bv;

	if (t_width < width)
		bv_to_bv(t_expr, t_width, width, t_bv);
	else
		t_bv << t_expr.str();

	if (e_width < width)
		bv_to_bv(e_expr, e_width, width, e_bv);
	else
		e_bv << e_expr.str();
	
	out << t_bv.str() << " " << e_bv.str() << ")";
	
	return width;
}

int PENumber::dump_design_smt(ostream& o, ostringstream &out, InstanModule *instan) const
{
	if (value_->is_string())
	{
		cerr << get_fileline() << " String in expression is unpported for test generation!" << endl;
		exit(1);
	}
	
	out << "#b" << value_->dumpstring();

	return value_->len();
}

int PEIdent::dump_design_smt(ostream& o, ostringstream &out, InstanModule *instan) const
{
	int width;
	string vn =  path_.back().name.str();
	list<index_component_t> bits_width = path_.back().index;

	//variable expression is a memory
	if(bits_width.size() > 1)
	{
		cerr << "Line : " << get_lineno() << " error : "
		" Memory is unsupported!" << endl;
		exit(1);
	}

	NetScope::param_ref_t expr_pos;
	
	//The expr is a parameter.
	if(instan->instan->find_parameter(vn))
	{
        verinum v = instan->instan->get_parameter(vn);
		out << "#b" << v.dumpstring();
		width = v.len();
	}

	else if(instan->define.find(vn) != instan->define.end())
	{
		SmtDefine* var = instan->define[vn];

		ostringstream name;

		if(!bits_width.empty())
		{
            index_component_t& ic = bits_width.front();

            verinum* lsn = nullptr;
            verinum* msn = nullptr;

            if(ic.lsb)
                lsn = ic.lsb->eval_const(instan->instan->get_module()->get_design(), instan->instan->get_module()->get_scope());
            if(ic.msb)
                msn = ic.msb->eval_const(instan->instan->get_module()->get_design(), instan->instan->get_module()->get_scope());

            long lsb = lsn? lsn->as_long() : var->symbol->get_lsb();
            long msb = msn? msn->as_long() : var->symbol->get_msb();

            unsigned l = msb - var->symbol->get_lsb();
            unsigned r = lsb - var->symbol->get_lsb();

            switch (ic.sel) {
                case index_component_t::SEL_BIT:
                    if(left_expression)
                        var->update(o, SmtDefine::SPACEUPDATE, 0, nullptr, msb, msb);
                    name << var->getName();
                    extract(name, l, l, out);
                    width = 1;
                    break;
                case index_component_t::SEL_BIT_LAST:
                    if(left_expression)
                        var->update(o, SmtDefine::SPACEUPDATE, 0, nullptr, msb, msb);
                    name << var->getName();
                    out << get_lineno() << " : SEL_BIT_LAST is unsupported in smt generating" << endl;
                    break;
                case index_component_t::SEL_PART:
                    if(left_expression)
                        var->update(o, SmtDefine::SPACEUPDATE, 0, nullptr, msb, lsb);
                    name << var->getName();
                    extract(name, l, r, out);
                    width = msb - lsb + 1;
                    break;
                case index_component_t::SEL_IDX_UP:
                    if(left_expression)
                        var->update(o, SmtDefine::SPACEUPDATE, 0, nullptr, msb + lsb - 1, msb);
                    name << var->getName();
                    extract(name, l + lsb - 1, l, out);
                    width = lsb;
                    break;
                case index_component_t::SEL_IDX_DO:
                    if(left_expression)
                        var->update(o, SmtDefine::SPACEUPDATE, 0, nullptr, msb - lsb + 1, msb);
                    name << var->getName();
                    extract(name, l - lsb + 1, l, out);
                    width = lsb;
                    break;
                default:
                    out << "???";
                    break;
            }
		}

		else
		{
            if(left_expression)
                var->update(o, SmtDefine::SPACEUPDATE, 0, nullptr, var->symbol->get_msb(), var->symbol->get_lsb());
            name << var->getName();
            out << name.str();
			width = var->getWidth();
		}
			
	}

	else
	{
		cerr << get_fileline() << " Can't find " << vn << " in reference set!" << endl;
		exit(1);
	}

	return width;
}

int PEUnary::dump_design_smt(ostream& o, ostringstream &out, InstanModule *instan) const
{
	ostringstream u_expr;

	int width = expr_->dump_design_smt(o, u_expr, instan);

	assert(width != SMT_INT);

	//Form as !V, the type of V is uncertain, convert V to a Bool expression and do negation.
	if (op_ == '!')
	{
		if (width == SMT_BOOL)
		{
			ostringstream u_bool;
			u_bool << "(not " << u_expr.str() << ")";
			out << u_bool.str();
			return SMT_BOOL;
		}

		else
		{
			assert(width == 1);
			out << "(bvnot " << u_expr.str() << ")";
			return 1;
		}
	}

	//Form as +V, -V, ~V, the type is BitVec, others is unsupported now.
	else if (op_ == '~' || op_ == '+' || op_ == '-')
	{
		assert(width > SMT_INT);
		switch (op_)
		{
		case '~':
			out << "((_ bvnot " << width << ")" << u_expr.str() << ")";
			break;
		case '-':
			out << "((_ bvneg " << width << ")" << u_expr.str() << ")";
			break;
		case '+':
			out << u_expr.str();
		}
		return width;
	}

	//Form as |V, ~|V and so on, the type of V is BitVec, and it returns only one bit.
	else if (SMT_Vec_Bits[op_])
	{
		out << "(" << SMT_Vec_Bits[op_];
		for (int i = 0; i < width; i++)
		{
			out << " ((_ extract " << i << " " << i << ")" << u_expr.str() << ")";
		}
		out << ")";
		return 1;
	}

	else
	{
		cerr << get_fileline() << " : This NetEUnary is unpported for smt generation!" << endl;
		exit(1);
	}
}

int PEBinary::dump_design_smt(ostream& o, ostringstream &out, InstanModule *instan) const
{
	ostringstream l_expr, r_expr;
	int l_width, r_width;
	int width;

	l_width = left_->dump_design_smt(o, l_expr, instan);
	r_width = right_->dump_design_smt(o, r_expr, instan);

	if(SMT_Vec_Add[op_])
	{
		assert(l_width > SMT_INT);
		assert(r_width > SMT_INT);

		int width = max(l_width, r_width);
		ostringstream l_bv, r_bv;

		if (l_width < width)
			bv_to_bv(l_expr, l_width, width, l_bv);
		else
			l_bv << l_expr.str();

		if (r_width < width)
			bv_to_bv(r_expr, r_width, width, r_bv);
		else
			r_bv << r_expr.str();
		
		out << "(" << SMT_Vec_Add[op_] << " " << l_bv.str() << " " << r_bv.str() << ")";

		return width;
	}

	if(SMT_Vec_Bits[op_])
	{
		ostringstream l_expr, r_expr;
		int l_width, r_width;

		l_width = left_->dump_design_smt(o, l_expr, instan);
		r_width = right_->dump_design_smt(o, r_expr, instan);

		assert(l_width != SMT_INT);
		assert(r_width != SMT_INT);

		ostringstream l_bv, r_bv;

		if (l_width == SMT_BOOL){
			bool_to_bv(l_expr, l_bv);
			l_width = 1;
		}
		else
			l_bv << l_expr.str();

		if (r_width == SMT_BOOL){
			bool_to_bv(r_expr, r_bv);
			r_width = 1;
		}
		else
			r_bv << r_expr.str();

		ostringstream l_equal, r_equal;
		int width = max(l_width, r_width);

		if(l_width != width)
			bv_to_bv(l_bv, l_width, width, l_equal);
		else
			l_equal << l_bv.str();

		if(r_width != width)
			bv_to_bv(r_bv, r_width, width, r_equal);
		else
			r_equal << r_bv.str();

		out << "(" << SMT_Vec_Bits[op_] << " " << l_equal.str() << " " << r_equal.str() << ")";

		return width;
	}

	if(SMT_Vec_Comp[op_])
	{
		ostringstream l_expr, r_expr;
		int l_width, r_width;
		int width;

		l_width = left_->dump_design_smt(o, l_expr, instan);
		r_width = right_->dump_design_smt(o, r_expr, instan);

		assert(l_width > SMT_INT);
		assert(r_width > SMT_INT);

		int bvwidth = max(l_width, r_width);
		ostringstream l_bv, r_bv;

		if (l_width < bvwidth)
			bv_to_bv(l_expr, l_width, bvwidth, l_bv);
		else
			l_bv << l_expr.str();

		if (r_width < bvwidth)
			bv_to_bv(r_expr, r_width, bvwidth, r_bv);
		else
			r_bv << r_expr.str();

		out << "(" << SMT_Vec_Comp[op_] << " " << l_bv.str() << " " << r_bv.str() << ")";

		width = SMT_BOOL;

		return width;
	}

	if(SMT_Vec_Div[op_])
	{
		ostringstream l_expr, r_expr;
		int l_width, r_width;

		l_width = left_->dump_design_smt(o, l_expr, instan);
		r_width = right_->dump_design_smt(o, r_expr, instan);

		assert(l_width > SMT_INT);
		assert(r_width > SMT_INT);

		int width = max(l_width, r_width);
		ostringstream l_bv, r_bv;

		if (l_width < width)
			bv_to_bv(l_expr, l_width, width, l_bv);
		else
			l_bv << l_expr.str();

		if (r_width < width)
			bv_to_bv(r_expr, r_width, width, r_bv);
		else
			r_bv << r_expr.str();

		out << "(" << SMT_Vec_Div[op_] << " " << l_bv.str() << " " << r_bv.str() << ")";

		return width;
	}

	if(SMT_Bool_Logic[op_])
	{
		ostringstream l_expr, r_expr;
		int l_width, r_width;
		int width;

		l_width = left_->dump_design_smt(o, l_expr, instan);
		r_width = right_->dump_design_smt(o, r_expr, instan);

		assert(l_width != SMT_INT);
		assert(r_width != SMT_INT);
		ostringstream l_bool, r_bool;

		if (l_width != SMT_BOOL)
			bv_compare_zero(l_expr, "distinct", l_width, l_bool);
		else
			l_bool << l_expr.str();

		if (r_width != SMT_BOOL)
			bv_compare_zero(r_expr, "distinct", r_width, r_bool);
		else
			r_bool << r_expr.str();

		out << "(" << SMT_Bool_Logic[op_] << " " << l_bool.str() << " " << r_bool.str() << ")";

		width = SMT_BOOL;

		return width;
	}

	if(SMT_Vec_Mult[op_])
	{
		ostringstream l_expr, r_expr;
		int l_width, r_width;

		l_width = left_->dump_design_smt(o, l_expr, instan);
		r_width = right_->dump_design_smt(o, r_expr, instan);

		assert(l_width > SMT_INT);
		assert(r_width > SMT_INT);

		int width = 2 * (l_width + r_width);

		ostringstream l_expand, r_expand;

		bv_to_bv(l_expr, l_width, width, l_expand);
		bv_to_bv(r_expr, r_width, width, r_expand);

		out << "(" << SMT_Vec_Mult[op_] << " " << l_expand.str() << " " << r_expand.str() << ")";

		return width;
	}

	cerr << get_fileline() << " : Pow operator is unspported!" << endl;
	exit(1);
	return SMT_NULL;

}