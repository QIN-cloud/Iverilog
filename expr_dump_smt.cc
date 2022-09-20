#include "config.h"

/*
* This file contains all the dump methods of the netlist classes.
*/
#include <typeinfo>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <math.h>
#include "Module.h"
#include "netlist.h"
#include "cfg.h"
#include "testpath.h"
#include "PExpr.h"
#include "smt_generator.h"

/*
* Generate the SMT-LIB2 for a netlist expression.
* Return the type of expression, including Int, Bool, BitVec or Wrong number.
* If the expression is a BitVec, return the width.
*/
int NetExpr::dump_smt(map<string, RefVar *> &vars, set<SmtVar *> &used, ostringstream &expr, Module *md) const
{
	cout << get_fileline() << "(?" << typeid(*this).name() << "?)";
	return SMT_NULL;
}

int NetExpr::dump_design_smt(ostringstream &out, InstanModule *instan, unordered_map<SmtDefine*, bool>& change, unsigned &tempid) const
{
	cout << get_fileline() << "(?" << typeid(*this).name() << "?)";
	return SMT_NULL;
}

/*
* Generate a const variable, usually a exact number or parameter.
* Return Int type.
*/
int NetEConst::dump_smt(map<string, RefVar *> &vars, set<SmtVar *> &used, ostringstream &expr, Module *md) const
{
	if (value_.is_string())
	{
		cerr << get_fileline() << " String in expression is unpported for test generation!" << endl;
		exit(1);
	}

	expr << value_.as_unsigned();
	return SMT_INT;
}

int NetEConst::dump_design_smt(ostringstream &out, InstanModule *instan, unordered_map<SmtDefine*, bool>& change, unsigned &tempid) const
{
	if (value_.is_string())
	{
		cerr << get_fileline() << " String in expression is unpported for test generation!" << endl;
		exit(1);
	}
	
	out << "#b" << value_.dumpstring();

	return value_.len();
}

/*
* Generate a SMT-LIB2 name for a BitVec.
* The format is modname_varname_time_space.
* Return the width of selection part.
*/
int NetESelect::dump_smt(map<string, RefVar *> &vars, set<SmtVar *> &used, ostringstream &expr, Module *md) const
{
	int width = expr_width();

	if (const NetESignal *signal = dynamic_cast<const NetESignal *>(expr_))
	{

		if (vars.find(signal->name().str()) != vars.end())
		{

			//Search the variable and make a SMT name
			RefVar *var = vars[signal->name().str()];
			ostringstream name;

			name << var->name << "_" << var->time << "_" << var->space;

			if (!var->record)
			{
				SmtVar *sv = new SmtVar;
				sv->smtname = name.str();
				sv->basename = var->name;
				sv->lsb = var->lsb;
				sv->msb = var->msb;
				sv->width = var->width;
				sv->type = var->ptype;
				sv->temp_flag = false;
				var->record = true;
				used.insert(sv);
			}

			//Bit or part selection of variable.
			if (base_)
			{

				if (const NetEConst *lsi_ = dynamic_cast<const NetEConst *>(base_))
				{
					unsigned long lsi = lsi_->value().as_unsigned();
					extract(name, lsi + width - 1, lsi, expr);
				}
				else
				{
					cerr << get_fileline() << " Memory unsupported!" << endl;
					exit(1);
				}
			}

			//Whole selection of variable.
			else
				expr << name.str();
		}

		else
		{
			cerr << get_fileline() << " Can't find expression in reference set!" << endl;
			exit(1);
		}
	}

	return width;
}

int NetESelect::dump_design_smt(ostringstream &out, InstanModule *instan, unordered_map<SmtDefine*, bool>& change, unsigned &tempid) const
{
	int width = expr_width();

	if (const NetESignal *signal = dynamic_cast<const NetESignal *>(expr_))
	{
		if (instan->define.find(signal->name().str()) != instan->define.end())
		{
			SmtDefine* var = instan->define[signal->name().str()];
            if(width > var->getWidth())
            {
                cerr << get_lineno() << " : " << var->symbol->get_name() << " may has a wrong size, must fix it." << endl;
                width = var->getWidth();
            }
			ostringstream name;
			if(change.find(var) != change.end())
				name << var->getLastName();
			else
				name << var->getName();
			//Bit or part selection of variable.
			if (base_)
			{

				if (const NetEConst *lsi_ = dynamic_cast<const NetEConst *>(base_))
				{
					unsigned long lsi = lsi_->value().as_unsigned();
					extract(name, lsi + width - 1 - var->symbol->get_lsb(), lsi - var->symbol->get_lsb(), out);
				}
				else
				{
					cerr << get_fileline() << " Memory unsupported!" << endl;
					exit(1);
				}
			}

			//Whole selection of variable.
			else
				out << name.str();
		}

		else
		{
			cerr << get_fileline() << " Can't find expression in reference set!" << endl;
			exit(1);
		}
	}
	return width;
}

/*
* Generate a SMT-LIB2 name for a BitVec.
* The format is modname_varname_time_space.
* Return the width of variable.
*/
int NetESignal::dump_smt(map<string, RefVar *> &vars, set<SmtVar *> &used, ostringstream &expr, Module *md) const
{
	bool find = false;
	int width;

	if (vars.find(name().str()) != vars.end())
	{

		RefVar *var = vars[name().str()];
		ostringstream name;

		name << var->name << "_" << var->time << "_" << var->space;

		if (!var->record)
		{
			SmtVar *sv = new SmtVar;
			sv->smtname = name.str();
			sv->basename = var->name;
			sv->lsb = var->lsb;
			sv->msb = var->msb;
			sv->width = var->width;
			sv->type = var->ptype;
			sv->temp_flag = false;
			var->record = true;
			used.insert(sv);
		}

		width = var->width;
		expr << name.str();
	}

	else
	{
		cerr << get_fileline() << " Can't find " << name() << " in reference set!" << endl;
		exit(1);
	}

	return width;
}

int NetESignal::dump_design_smt(ostringstream &out, InstanModule *instan, unordered_map<SmtDefine*, bool>& change, unsigned &tempid) const
{
	bool find = false;
	int width;

	if (instan->define.find(name().str()) != instan->define.end())
	{
		SmtDefine* var = instan->define[name().str()];
		width = var->getWidth();
		if(change.find(var) != change.end() && !change[var])
			out << var->getLastName();
		else
			out << var->getName();
	}

	else
	{
		cerr << get_fileline() << " Can't find " << name() << " in reference set!" << endl;
		exit(1);
	}

	return width;
}

/*
* Generate a SMT-LIB2 statement for a ternary expression (if ? true_expr : expr_expr).
*/
int NetETernary::dump_smt(map<string, RefVar *> &vars, set<SmtVar *> &used, ostringstream &expr, Module *md) const
{
	//Using ite(if true_expr else_expr) form to generate the smt
	ostringstream i_expr, t_expr, e_expr;
	int i_width, t_width, e_width;
	int width;

	i_width = cond_->dump_smt(vars, used, i_expr, md);
	t_width = true_val_->dump_smt(vars, used, t_expr, md);
	e_width = false_val_->dump_smt(vars, used, e_expr, md);

	assert(i_width != SMT_NULL);
	assert(t_width != SMT_NULL);
	assert(e_width != SMT_NULL);

	if (t_width == SMT_BOOL || e_width == SMT_BOOL)
	{
		cerr << get_fileline() << " True and else expressions in ternary are not allowed using bool expressions!" << endl;
		exit(1);
	}

	//Generate SMT for if expression
	ostringstream i_expr_bool;
	switch (i_width)
	{
	//The format is like "a > 0 ? t_expr : e_expr"
	case SMT_BOOL:
		i_expr_bool << i_expr.str();
		break;
	//The format is like "2 - 1 > t_expr : e_expr", usually the format like this is not allowed
	case SMT_INT:
		i_expr_bool << "(ite "
					<< "(distinct " << i_expr.str() << " 0) true false)";
		break;
	//The format is like "a[2:0] ? t_expr : e_expr", usually is not allowed too.
	default:
		i_expr_bool << "(ite ";
		bv_compare_zero(i_expr, "distinct", i_width, i_expr_bool);
		i_expr_bool << " 0) true false)";
		break;
	}

	expr << "(ite"
		 << " " << i_expr_bool.str() << " ";

	// If t_expr and e_expr have different widths,
	// convert the BitVec to the Int and return Int type.

	if (t_width == e_width)
	{
		width = t_width;
		expr << t_expr.str() << " " << e_expr.str();
	}

	else
	{

		width = SMT_INT;
		ostringstream t_expr_int;
		ostringstream e_expr_int;

		if (t_width != SMT_INT)
			bv_to_int(t_expr, t_expr_int);
		else
			t_expr_int << t_expr.str();

		if (e_width != SMT_INT)
			bv_to_int(e_expr, e_expr_int);
		else
			e_expr_int << e_expr.str();

		expr << t_expr_int.str() << " " << e_expr_int.str();
	}

	expr << ")";
	return width;
}

int NetETernary::dump_design_smt(ostringstream &out, InstanModule *instan, unordered_map<SmtDefine*, bool>& change, unsigned &tempid) const
{
	//Using ite(if true_expr else_expr) form to generate the smt
	ostringstream i_expr, t_expr, e_expr;
	int i_width, t_width, e_width;

	i_width = cond_->dump_design_smt(i_expr, instan,change, tempid);
	t_width = true_val_->dump_design_smt(t_expr, instan, change, tempid);
	e_width = false_val_->dump_design_smt(e_expr, instan, change, tempid);

	assert(i_width == SMT_BOOL);
	assert(t_width != SMT_INT);
	assert(e_width != SMT_INT);

	if (t_width == SMT_BOOL || e_width == SMT_BOOL)
	{
		cerr << get_fileline() << " True and else expressions in ternary are not allowed using bool expressions!" << endl;
		exit(1);
	}

	out << "(ite " << i_expr.str() << " ";

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
	
	return width;
}

/*
* Generate a SMT-LIB2 statement for a unary expression.
*/
int NetEUnary::dump_smt(map<string, RefVar *> &vars, set<SmtVar *> &used, ostringstream &expr, Module *md) const
{
	ostringstream u_expr;

	int width = expr_->dump_smt(vars, used, u_expr, md);

	assert(width != SMT_NULL);

	//Form as !V, the type of V is uncertain, convert V to a Bool expression and do negation.
	if (op_ == '!')
	{
		ostringstream u_bool;

		if (width == SMT_BOOL)
			u_bool << "(= " << u_expr.str() << " "
				   << "false"
				   << ")";

		else if (width == SMT_INT)
			u_bool << "(= " << u_expr.str() << " "
				   << "0"
				   << ")";

		else
			bv_compare_zero(u_expr, "=", width, u_bool);

		expr << "(ite " << u_bool.str() << " "
			 << "1"
			 << " "
			 << "0"
			 << ")";

		return SMT_INT;
	}

	//Form as +V, -V, ~V, the type is BitVec, others is unsupported now.
	else if (op_ == '~' || op_ == '+' || op_ == '-')
	{
		switch (op_)
		{
		case '~':
			expr << "((_ bvnot " << width << ")" << u_expr.str() << ")";
			break;
		case '-':
			expr << "((_ bvneg " << width << ")" << u_expr.str() << ")";
			break;
		case '+':
			expr << u_expr.str();
		}
		return width;
	}

	//Form as |V, ~|V and so on, the type of V is BitVec, and it returns only one bit.
	else if (SMT_Vec_Bits[op_])
	{
		expr << "(" << SMT_Vec_Bits[op_];
		for (int i = width - 1; i >= 0; i--)
		{
			expr << " "
				 << "((_ extract " << i << " " << i << ")" << u_expr.str() << ")";
		}
		expr << ")";
		return 1;
	}

	else
	{
		cerr << get_fileline() << " : This NetEUnary is unpported for smt generation!" << endl;
		exit(1);
	}
}

int NetEUnary::dump_design_smt(ostringstream &out, InstanModule *instan, unordered_map<SmtDefine*, bool>& change, unsigned &tempid) const
{
	ostringstream u_expr;

	int width = expr_->dump_design_smt(u_expr, instan, change, tempid);

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

/*
  Generate a SMT-LIB2 statement for a binary expression.
*/
int NetEBinary::dump_smt(map<string, RefVar *> &vars, set<SmtVar *> &used, ostringstream &expr, Module *md) const
{
	return SMT_NULL;
}

int NetEBinary::dump_design_smt(ostringstream &out, InstanModule *instan, unordered_map<SmtDefine*, bool>& change, unsigned &tempid) const
{
	return SMT_NULL;
}

int NetEBAdd::dump_smt(map<string, RefVar *> &vars, set<SmtVar *> &used, ostringstream &expr, Module *md) const
{
	ostringstream l_expr, r_expr;
	int l_width, r_width;
	int width;

	l_width = left_->dump_smt(vars, used, l_expr, md);
	r_width = right_->dump_smt(vars, used, r_expr, md);

	assert(l_width >= SMT_INT);
	assert(r_width >= SMT_INT);

	if (l_width == r_width && l_width != SMT_INT)
	{
		expr << "(" << SMT_Vec_Add[op()] << " " << l_expr.str() << " " << r_expr.str() << ")";

		width = l_width;
	}

	else
	{
		ostringstream l_int, r_int;

		if (l_width != SMT_INT)
			bv_to_int(l_expr, l_int);
		else
			l_int << l_expr.str();

		if (r_width != SMT_INT)
			bv_to_int(r_expr, r_int);
		else
			r_int << r_expr.str();

		expr << "(" << op() << " " << l_int.str() << " " << r_int.str() << ")";

		width = SMT_INT;
	}

	return width;
}

int NetEBAdd::dump_design_smt(ostringstream &out, InstanModule *instan, unordered_map<SmtDefine*, bool>& change, unsigned &tempid) const
{
	ostringstream l_expr, r_expr;
	int l_width, r_width;

	l_width = left_->dump_design_smt(l_expr, instan, change, tempid);
	r_width = right_->dump_design_smt(r_expr, instan, change, tempid);

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
	
	out << "(" << SMT_Vec_Add[op()] << " " << l_bv.str() << " " << r_bv.str() << ")";

	return width;
}

int NetEBDiv::dump_smt(map<string, RefVar *> &vars, set<SmtVar *> &used, ostringstream &expr, Module *md) const
{
	ostringstream l_expr, r_expr;
	int l_width, r_width;
	int width;

	l_width = left_->dump_smt(vars, used, l_expr, md);
	r_width = right_->dump_smt(vars, used, r_expr, md);

	assert(l_width >= SMT_INT);
	assert(r_width >= SMT_INT);

	if (l_width == r_width && l_width != SMT_INT)
	{
		expr << "(" << SMT_Vec_Div[op()] << " " << l_expr.str() << " " << r_expr.str() << ")";

		width = l_width;
	}

	else
	{
		ostringstream l_int, r_int;

		if (l_width != SMT_INT)
			bv_to_int(l_expr, l_int);
		else
			l_int << l_expr.str();

		if (r_width != SMT_INT)
			bv_to_int(r_expr, r_int);
		else
			r_int << r_expr.str();

		expr << "(" << op() << " " << l_int.str() << " " << r_int.str() << ")";

		width = SMT_INT;
	}

	return width;
}

int NetEBDiv::dump_design_smt(ostringstream &out, InstanModule *instan, unordered_map<SmtDefine*, bool>& change, unsigned &tempid) const
{
	ostringstream l_expr, r_expr;
	int l_width, r_width;

	l_width = left_->dump_design_smt(l_expr, instan, change, tempid);
	r_width = right_->dump_design_smt(r_expr, instan, change, tempid);

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

	out << "(" << SMT_Vec_Div[op()] << " " << l_bv.str() << " " << r_bv.str() << ")";

	return width;
}

int NetEBBits::dump_smt(map<string, RefVar *> &vars, set<SmtVar *> &used, ostringstream &expr, Module *md) const
{
	ostringstream l_expr, r_expr;
	int l_width, r_width;
	int width;

	l_width = left_->dump_smt(vars, used, l_expr, md);
	r_width = right_->dump_smt(vars, used, r_expr, md);

	assert(l_width != SMT_INT);
	assert(r_width != SMT_INT);

	if (l_width == r_width && l_width != SMT_INT)
	{
		expr << "(" << SMT_Vec_Bits[op()] << " " << l_expr.str() << " " << r_expr.str() << ")";

		width = l_width;
	}

	else
	{
		ostringstream l_max, r_max;

		assert(l_width < SMT_MAX && r_width < SMT_MAX);

		if (l_width == SMT_INT)
			int_to_bv(l_expr, SMT_MAX, l_max);
		else
			bv_int_bv(l_expr, SMT_MAX, l_max);

		if (r_width == SMT_INT)
			int_to_bv(r_expr, SMT_MAX, r_max);
		else
			bv_int_bv(r_expr, SMT_MAX, r_max);

		expr << "(" << SMT_Vec_Bits[op()] << " " << l_max.str() << " " << r_max.str() << ")";

		width = SMT_MAX;
	}

	return width;
}

int NetEBBits::dump_design_smt(ostringstream &out, InstanModule *instan, unordered_map<SmtDefine*, bool>& change, unsigned &tempid) const
{
	ostringstream l_expr, r_expr;
	int l_width, r_width;

	l_width = left_->dump_design_smt(l_expr, instan, change, tempid);
	r_width = right_->dump_design_smt(r_expr, instan, change, tempid);

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

	out << "(" << SMT_Vec_Bits[op()] << " " << l_equal.str() << " " << r_equal.str() << ")";

	return width;
}

int NetEBComp::dump_smt(map<string, RefVar *> &vars, set<SmtVar *> &used, ostringstream &expr, Module *md) const
{
	ostringstream l_expr, r_expr;
	int l_width, r_width;
	int width;

	l_width = left_->dump_smt(vars, used, l_expr, md);
	r_width = right_->dump_smt(vars, used, r_expr, md);

	assert(l_width >= SMT_INT);
	assert(r_width >= SMT_INT);

	if (l_width == r_width && l_width != SMT_INT)
	{
		expr << "(" << SMT_Vec_Comp[op()] << " " << l_expr.str() << " " << r_expr.str() << ")";
	}

	else
	{
		ostringstream l_int, r_int;

		if (l_width != SMT_INT)
			bv_to_int(l_expr, l_int);
		else
			l_int << l_expr.str();

		if (r_width != SMT_INT)
			bv_to_int(r_expr, r_int);
		else
			r_int << r_expr.str();

		expr << "(" << SMT_Int_Comp[op()] << " " << l_int.str() << " " << r_int.str() << ")";
	}

	width = SMT_BOOL;

	return width;
}

int NetEBComp::dump_design_smt(ostringstream &out, InstanModule *instan, unordered_map<SmtDefine*, bool>& change, unsigned &tempid) const
{
	ostringstream l_expr, r_expr;
	int l_width, r_width;
	int width;

	l_width = left_->dump_design_smt(l_expr, instan, change, tempid);
	r_width = right_->dump_design_smt(r_expr, instan, change, tempid);

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

	out << "(" << SMT_Vec_Comp[op()] << " " << l_bv.str() << " " << r_bv.str() << ")";

	width = SMT_BOOL;

	return width;
}

int NetEBLogic::dump_smt(map<string, RefVar *> &vars, set<SmtVar *> &used, ostringstream &expr, Module *md) const
{
	ostringstream l_expr, r_expr;
	int l_width, r_width;
	int width;

	l_width = left_->dump_smt(vars, used, l_expr, md);
	r_width = right_->dump_smt(vars, used, r_expr, md);

	if (l_width == r_width && l_width == SMT_BOOL)
	{
		expr << "(" << SMT_Bool_Logic[op()] << " " << l_expr.str() << " " << r_expr.str() << ")";
	}

	else
	{
		ostringstream l_bool, r_bool;

		if (l_width != SMT_INT)
			bv_compare_zero(l_expr, "distinct", l_width, l_bool);
		else
			int_compare_zero(l_expr, "distinct", l_bool);

		if (r_width != SMT_INT)
			bv_compare_zero(r_expr, "distinct", r_width, r_bool);
		else
			int_compare_zero(r_expr, "distinct", r_bool);

		expr << "(" << SMT_Bool_Logic[op()] << " " << l_bool.str() << " " << r_bool.str() << ")";
	}

	width = SMT_BOOL;

	return width;
}

int NetEBLogic::dump_design_smt(ostringstream &out, InstanModule *instan, unordered_map<SmtDefine*, bool>& change, unsigned &tempid) const
{
	ostringstream l_expr, r_expr;
	int l_width, r_width;
	int width;

	l_width = left_->dump_design_smt(l_expr, instan, change, tempid);
	r_width = right_->dump_design_smt(r_expr, instan, change, tempid);

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

	out << "(" << SMT_Bool_Logic[op()] << " " << l_bool.str() << " " << r_bool.str() << ")";

	width = SMT_BOOL;

	return width;
}

int NetEBMinMax::dump_smt(map<string, RefVar *> &vars, set<SmtVar *> &used, ostringstream &expr, Module *md) const
{
	cerr << get_fileline() << " : Max and Min operator is unspported!" << endl;
	exit(1);
	return SMT_NULL;
}

int NetEBMinMax::dump_design_smt(ostringstream &out, InstanModule *instan, unordered_map<SmtDefine*, bool>& change, unsigned &tempid) const
{
	cerr << get_fileline() << " : Max and Min operator is unspported!" << endl;
	exit(1);
	return SMT_NULL;
}

int NetEBMult::dump_smt(map<string, RefVar *> &vars, set<SmtVar *> &used, ostringstream &expr, Module *md) const
{
	ostringstream l_expr, r_expr;
	int l_width, r_width;
	int width;

	l_width = left_->dump_smt(vars, used, l_expr, md);
	r_width = right_->dump_smt(vars, used, r_expr, md);

	assert(l_width >= SMT_INT);
	assert(r_width >= SMT_INT);

	if (l_width == r_width && l_width != SMT_INT)
	{
		expr << "(" << SMT_Vec_Mult[op()] << " " << l_expr.str() << " " << r_expr.str() << ")";

		width = l_width;
	}

	else
	{
		ostringstream l_int, r_int;

		if (l_width != SMT_INT)
			bv_to_int(l_expr, l_int);
		else
			l_int << l_expr.str();

		if (r_width != SMT_INT)
			bv_to_int(r_expr, r_int);
		else
			r_int << r_expr.str();

		expr << "(" << op() << " " << l_int.str() << " " << r_int.str() << ")";

		width = SMT_INT;
	}

	return width;
}

int NetEBMult::dump_design_smt(ostringstream &out, InstanModule *instan, unordered_map<SmtDefine*, bool>& change, unsigned &tempid) const
{
	ostringstream l_expr, r_expr;
	int l_width, r_width;

	l_width = left_->dump_design_smt(l_expr, instan, change, tempid);
	r_width = right_->dump_design_smt(r_expr, instan, change, tempid);

	assert(l_width > SMT_INT);
	assert(r_width > SMT_INT);

	int width = 2 * (l_width + r_width);

	ostringstream l_expand, r_expand;

	bv_to_bv(l_expr, l_width, width, l_expand);
	bv_to_bv(r_expr, r_width, width, r_expand);

	out << "(" << SMT_Vec_Mult[op()] << " " << l_expand.str() << " " << r_expand.str() << ")";

	return width;
}

int NetEBPow::dump_smt(map<string, RefVar *> &vars, set<SmtVar *> &used, ostringstream &expr, Module *md) const
{
	cerr << get_fileline() << " : Pow operator is unspported!" << endl;
	exit(1);
	return SMT_NULL;
}

int NetEBPow::dump_design_smt(ostringstream &out, InstanModule *instan, unordered_map<SmtDefine*, bool>& change, unsigned &tempid) const
{
	cerr << get_fileline() << " : Pow operator is unspported!" << endl;
	exit(1);
	return SMT_NULL;
}

int NetEBShift::dump_smt(map<string, RefVar *> &vars, set<SmtVar *> &used, ostringstream &expr, Module *md) const
{
	cerr << get_fileline() << " : Shift operator is unspported!" << endl;
	exit(1);
	return SMT_NULL;
}

int NetEBShift::dump_design_smt(ostringstream &out, InstanModule *instan, unordered_map<SmtDefine*, bool>& change, unsigned &tempid) const
{
	cerr << get_fileline() << " : Shift operator is unspported!" << endl;
	exit(1);
	return SMT_NULL;
}

int NetEArrayPattern::dump_smt(map<string, RefVar *> &vars, set<SmtVar *> &used, ostringstream &expr, Module *md) const
{
	cerr << get_fileline() << " : Array pattern unspported!" << endl;
	exit(1);
	return SMT_NULL;
}

int NetEArrayPattern::dump_design_smt(ostringstream &out, InstanModule *instan, unordered_map<SmtDefine*, bool>& change, unsigned &tempid) const
{
	cerr << get_fileline() << " : Array pattern unspported!" << endl;
	exit(1);
	return SMT_NULL;
}

int NetELast::dump_smt(map<string, RefVar *> &vars, set<SmtVar *> &used, ostringstream &expr, Module *md) const
{
	cerr << get_fileline() << " : NetELast unspported!" << endl;
	exit(1);
	return SMT_NULL;
}

int NetELast::dump_design_smt(ostringstream &out, InstanModule *instan, unordered_map<SmtDefine*, bool>& change, unsigned &tempid) const
{
	cerr << get_fileline() << " : NetELast unspported!" << endl;
	exit(1);
	return SMT_NULL;
}

int NetEAccess::dump_smt(map<string, RefVar *> &vars, set<SmtVar *> &used, ostringstream &expr, Module *md) const
{
	cerr << get_fileline() << " : NetEAccess unspported!" << endl;
	exit(1);
	return SMT_NULL;
}

int NetEAccess::dump_design_smt(ostringstream &out, InstanModule *instan, unordered_map<SmtDefine*, bool>& change, unsigned &tempid) const
{
	cerr << get_fileline() << " : NetEAccess unspported!" << endl;
	exit(1);
	return SMT_NULL;
}

int NetEEvent::dump_smt(map<string, RefVar *> &vars, set<SmtVar *> &used, ostringstream &expr, Module *md) const
{
	cerr << get_fileline() << " : NetEvent unspported!" << endl;
	exit(1);
	return SMT_NULL;
}

int NetEEvent::dump_design_smt(ostringstream &out, InstanModule *instan, unordered_map<SmtDefine*, bool>& change, unsigned &tempid) const
{
	cerr << get_fileline() << " : NetEvent unspported!" << endl;
	exit(1);
	return SMT_NULL;
}

int NetENetenum::dump_smt(map<string, RefVar *> &vars, set<SmtVar *> &used, ostringstream &expr, Module *md) const
{
	cerr << get_fileline() << " : NetENetenum unspported!" << endl;
	exit(1);
	return SMT_NULL;
}

int NetENetenum::dump_design_smt(ostringstream &out, InstanModule *instan, unordered_map<SmtDefine*, bool>& change, unsigned &tempid) const
{
	cerr << get_fileline() << " : NetENetenum unspported!" << endl;
	exit(1);
	return SMT_NULL;
}

int NetENew::dump_smt(map<string, RefVar *> &vars, set<SmtVar *> &used, ostringstream &expr, Module *md) const
{
	cerr << get_fileline() << " : NetENew unspported!" << endl;
	exit(1);
	return SMT_NULL;
}

int NetENew::dump_design_smt(ostringstream &out, InstanModule *instan, unordered_map<SmtDefine*, bool>& change, unsigned &tempid) const
{
	cerr << get_fileline() << " : NetENew unspported!" << endl;
	exit(1);
	return SMT_NULL;
}

int NetENull::dump_smt(map<string, RefVar *> &vars, set<SmtVar *> &used, ostringstream &expr, Module *md) const
{
	cerr << get_fileline() << " : NetENull unspported!" << endl;
	exit(1);
	return SMT_NULL;
}

int NetENull::dump_design_smt(ostringstream &out, InstanModule *instan, unordered_map<SmtDefine*, bool>& change, unsigned &tempid) const
{
	cerr << get_fileline() << " : NetENull unspported!" << endl;
	exit(1);
	return SMT_NULL;
}

int NetEProperty::dump_smt(map<string, RefVar *> &vars, set<SmtVar *> &used, ostringstream &expr, Module *md) const
{
	cerr << get_fileline() << " : NetEProperty unspported!" << endl;
	exit(1);
	return SMT_NULL;
}

int NetEProperty::dump_design_smt(ostringstream &out, InstanModule *instan, unordered_map<SmtDefine*, bool>& change, unsigned &tempid) const
{
	cerr << get_fileline() << " : NetEProperty unspported!" << endl;
	exit(1);
	return SMT_NULL;
}

int NetEShallowCopy::dump_smt(map<string, RefVar *> &vars, set<SmtVar *> &used, ostringstream &expr, Module *md) const
{
	cerr << get_fileline() << " : NetEShallowCopy unspported!" << endl;
	exit(1);
	return SMT_NULL;
}

int NetEShallowCopy::dump_design_smt(ostringstream &out, InstanModule *instan, unordered_map<SmtDefine*, bool>& change, unsigned &tempid) const
{
	cerr << get_fileline() << " : NetEShallowCopy unspported!" << endl;
	exit(1);
	return SMT_NULL;
}

int NetECString::dump_smt(map<string, RefVar *> &vars, set<SmtVar *> &used, ostringstream &expr, Module *md) const
{
	cerr << "Const string is unsupported now!" << endl;
	exit(1);
	return SMT_NULL;
}

int NetECString::dump_design_smt(ostringstream &out, InstanModule *instan, unordered_map<SmtDefine*, bool>& change, unsigned &tempid) const
{
	cerr << "Const string is unsupported now!" << endl;
	exit(1);
	return SMT_NULL;
}

int NetECReal::dump_smt(map<string, RefVar *> &vars, set<SmtVar *> &used, ostringstream &expr, Module *md) const
{
	cerr << get_fileline() << " : Real number is unsupported now!" << endl;
	exit(1);
	return SMT_NULL;
}

int NetECReal::dump_design_smt(ostringstream &out, InstanModule *instan, unordered_map<SmtDefine*, bool>& change, unsigned &tempid) const
{
	cerr << get_fileline() << " : Real number is unsupported now!" << endl;
	exit(1);
	return SMT_NULL;
}

int NetEScope::dump_smt(map<string, RefVar *> &vars, set<SmtVar *> &used, ostringstream &expr, Module *md) const
{
	cerr << get_fileline() << " : Scope name unspported!" << endl;
	exit(1);
	return SMT_NULL;
}

int NetEScope::dump_design_smt(ostringstream &out, InstanModule *instan, unordered_map<SmtDefine*, bool>& change, unsigned &tempid) const
{
	cerr << get_fileline() << " : Scope name unspported!" << endl;
	exit(1);
	return SMT_NULL;
}

int NetESFunc::dump_smt(map<string, RefVar *> &vars, set<SmtVar *> &used, ostringstream &expr, Module *md) const
{
	cerr << get_fileline() << " : System function call unspported!" << endl;
	exit(1);
	return SMT_NULL;
}

int NetESFunc::dump_design_smt(ostringstream &out, InstanModule *instan, unordered_map<SmtDefine*, bool>& change, unsigned &tempid) const
{
	cerr << get_fileline() << " : System function call unspported!" << endl;
	exit(1);
	return SMT_NULL;
}

int NetEConcat::dump_smt(map<string, RefVar*>& vars, std::set<SmtVar*>& used, ostringstream& expr, Module* md) const
{
	return 0;
}

int NetEConcat::dump_design_smt(ostringstream &out, InstanModule *instan, std::unordered_map<SmtDefine*, bool>& change, unsigned &tempid) const
{
	list<string> items;
	unsigned width = 0;
	for(NetExpr* expr : parms_) 
	{
		ostringstream item;
		unsigned i = expr->dump_design_smt(item, instan, change, tempid);
		width += i;
		items.push_back(item.str());
	}
	assert(!items.empty());
	if(items.size() == 1 && repeat_ == 1)
	{
		out << items.front();
	}
	else
	{
		width *= repeat_;
		out << "(concat";
		for(size_t i = 0; i < repeat_; i++)
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

int NetEUFunc::dump_smt(map<string, RefVar *> &vars, set<SmtVar *> &used, ostringstream &expr, Module *md) const
{
	cerr << get_fileline() << " : System function call unspported!" << endl;
	exit(1);
	return SMT_NULL;
}

int NetEUFunc::dump_design_smt(ostringstream &out, InstanModule *instan, unordered_map<SmtDefine*, bool>& change, unsigned &tempid) const
{
	cerr << get_fileline() << " : System function call unspported!" << endl;
	exit(1);
	return SMT_NULL;
}
