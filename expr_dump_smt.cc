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

/*
* Generate the SMT-LIB2 for a netlist expression.
* Return the type of expression, including Int, Bool, BitVec or Wrong number.
* If the expression is a BitVec, return the width.
*/
int NetExpr::dump_smt(map<string, RefVar*>& vars, set<SmtVar*>& used, ostringstream& expr, Module* md) const
{
	expr << get_fileline() << "(?" << typeid(*this).name() << "?)";
	return SMT_NULL;
}

/*
* Generate a const variable, usually a exact number or parameter.
* Return Int type.
*/
int NetEConst::dump_smt(map<string, RefVar*>& vars, set<SmtVar*>& used, ostringstream& expr, Module* md) const
{
	if (value_.is_string()){
		cerr << get_fileline() << " String in expression is unpported for test generation!" << endl;
		exit(1);
	}

	expr << value_.as_unsigned();
	return SMT_INT;
}

/*
* Generate a SMT-LIB2 name for a BitVec.
* The format is modname_varname_time_space.
* Return the width of selection part.
*/
int NetESelect::dump_smt(map<string, RefVar*>& vars, set<SmtVar*>& used, ostringstream& expr, Module* md) const
{
	int width = expr_width();

	if(const NetESignal* signal = dynamic_cast<const NetESignal*>(expr_)){
		
		if(vars.find(signal->name().str()) != vars.end()){

			//Search the variable and make a SMT name
			RefVar* var = vars[signal->name().str()];
			ostringstream name;
			SmtVar* sv = new SmtVar;
			name << md->pscope_name() << "_" << var->name << "_" << var->time << "_" << var->space;
			sv->smtname = name.str();
			sv->basename = var->name;
			sv->lsb = var->lsb;
			sv->msb = var->msb;
			sv->width = var->width;
			sv->type = var->ptype;
			sv->temp_flag = false;
			used.insert(sv);

			//Bit or part selection of variable.
			if(base_){

				if(const NetEConst* lsi_ = dynamic_cast<const NetEConst*>(base_)){
					unsigned long lsi = lsi_->value().as_unsigned();
					extract(name, lsi+width-1, lsi, expr);
				}
				else{
					cerr << get_fileline() << " Memory unsupported!" << endl;
					exit(1);
				}
			}

			//Whole selection of variable.
			else
				expr << name.str();

			}

			else{
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
int NetESignal::dump_smt(map<string, RefVar*>& vars, set<SmtVar*>& used, ostringstream& expr, Module* md) const
{
	bool find = false;
	int width;

	if(vars.find(name().str()) != vars.end()){

		RefVar* var = vars[name().str()];
		ostringstream name;
		SmtVar* sv = new SmtVar;
		name << md->pscope_name() << "_" << var->name << "_" << var->time << "_" << var->space;
		sv->smtname = name.str();
		sv->basename = var->name;
		sv->lsb = var->lsb;
		sv->msb = var->msb;
		sv->width = var->width;
		sv->type = var->ptype;
		sv->temp_flag = false;
		used.insert(sv);
		expr << name.str();
		width = sv->width;

		}

	else{
		cerr << get_fileline() << " Can't find " << name() << " in reference set!" << endl;
		exit(1);
	}

	return width;
}
/*
* Generate a SMT-LIB2 statement for a ternary expression (if ? true_expr : expr_expr).
*/
int NetETernary::dump_smt(map<string, RefVar*>& vars, set<SmtVar*>& used, ostringstream& expr, Module* md) const
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
		//The format is like "2 - 1 > t_expr : e_expr", usually the format like this is not allowed
		case SMT_INT  : 
			i_expr_bool << "(ite " << "(distinct " << i_expr.str() << " 0) true false)"; 
			break;
		//The format is like "a[2:0] ? t_expr : e_expr", usually is not allowed too.
		default : 
			i_expr_bool << "(ite ";
			bv_compare_zero(i_expr, "distinct", i_width, i_expr_bool);
			i_expr_bool << " 0) true false)";
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
/*
* Generate a SMT-LIB2 statement for a unary expression.
*/
int NetEUnary::dump_smt(map<string, RefVar*>& vars, set<SmtVar*>& used, ostringstream& expr, Module* md) const
{
	ostringstream u_expr;

	int width = expr_->dump_smt(vars, used, u_expr, md);

	assert(width != SMT_NULL);

	//Form as !V, the type of V is uncertain, convert V to a Bool expression and do negation.
	if(op_ == '!'){
		ostringstream u_bool;

		if(width == SMT_BOOL) 
			expr << u_expr.str();
		else if(width == SMT_INT) 
			expr << "(distinct " << u_expr.str() << " 0)"; 
		else 
			bv_compare_zero(u_expr, "distinct", width, u_bool);

		expr << "(not " << u_bool.str() << ")";  
		return SMT_BOOL;
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
	else if(bv_logic[op_]){
		expr << "(" << bv_logic[op_];
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
/*
  Generate a SMT-LIB2 statement for a binary expression.
*/
int NetEBinary::dump_smt(map<string, RefVar*>& vars, set<SmtVar*>& used, ostringstream& expr, Module* md) const
{
	ostringstream l_expr, r_expr;
	int l_width, r_width;

	l_width = left_->dump_smt(vars, used, l_expr, md);
	r_width = right_->dump_smt(vars, used, r_expr, md);

	assert(l_width != SMT_NULL);
	assert(r_width != SMT_NULL);
	
	//Arithmetic oprator, exchange the BitVec into int type, then calculate and return Int num.
	if(bv_arith[op_]){

		assert(l_width != SMT_BOOL);
		assert(r_width != SMT_BOOL);

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
		return SMT_INT;
	}

	//Bitwise operator, exchange the int into BitVec type, then calculate and return BitVec or bool.
	else if(bv_logic[op_])
	{
		if(l_width == SMT_BOOL || r_width == SMT_BOOL)
		{
			ostringstream l_bool, r_bool;
			bool not_flag = op_ == 'O' || op_ == 'X' || op_ == 'A';

			if(l_width != SMT_BOOL){

				if(l_width == SMT_INT) 
					l_bool << "(distinct " << l_expr.str() << " 0)";
				else 
					bv_compare_zero(l_expr, "distinct", l_width, l_bool);

			}
			else 
				l_bool << l_expr.str();

			if(r_width != SMT_BOOL){

				if(r_width == SMT_INT) 
					r_bool << "(distinct " << r_expr.str() << " 0)";
				else 
					bv_compare_zero(r_expr, "distinct", r_width, r_bool);

			}
			else 
				r_bool << r_expr.str();

			if(not_flag) expr << "(not";
			expr << "(" << bv_logic[op_] << " " << l_bool.str() << " " << r_bool.str() << ")";
			if(not_flag) expr << ")";
			return SMT_BOOL;
		}
		else
		{
			int width = !(l_width == SMT_INT && r_width == SMT_INT) ? max(l_width, r_width) : SMT_MAX;
			ostringstream l_bv, r_bv;

			if(width != SMT_MAX){

				if(l_width == SMT_INT) 
					int_to_bv(l_expr, width, l_bv);
				else 
					l_bv << l_expr.str();

				if(r_width == SMT_INT) 
					int_to_bv(r_expr, width, r_bv);
				else 
					r_bv << r_expr.str();

			}
			else{
				int_to_bv(l_expr, width, l_bv);
				int_to_bv(r_expr, width, r_bv);
			}
			expr << "(" << bv_logic[op_] << " " << l_bv.str() << " " << r_bv.str() << ")";
			return width;
		}
	}

	//Logic operator, exchange the int or BitVec into bool type, then calculate and return bool.
	else if(logic_binary[op_])
	{
		ostringstream l_bool, r_bool;
		if(l_width != SMT_BOOL)
		{
			if(l_width == SMT_INT) l_bool << "(distinct " << l_expr.str() << " 0)";
			else bv_compare_zero(l_expr, "distinct", l_width, l_bool);
		}
		else l_bool << l_expr.str();
		if(r_width != SMT_BOOL)
		{
			if(r_width == SMT_INT) r_bool << "(distinct " << r_expr.str() << " 0)";
			else bv_compare_zero(r_expr, "distinct", r_width, r_bool);
		}
		else r_bool << r_expr.str();
		expr << "(" << logic_binary[op_] << " " << l_bool.str() << " " << r_bool.str() << ")";
		return SMT_BOOL; 
	}

	//Num compare opetator, exchange the BitVec into int type, then calculate and return bool.
	else if(bv_compare[op_])
	{
		assert(l_width != SMT_BOOL);
		assert(r_width != SMT_BOOL);

		if(l_width == r_width){
			const char* option;
			option = l_width == SMT_INT ? int_compare[op_] : bv_compare[op_];
			expr << "(" << option << " " << l_expr.str() << " " << r_expr.str() << ")";
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

			expr << "(" << int_compare[op_] << " " << l_int.str() << " " << r_int.str() << ")";
		}

		return SMT_BOOL;
	}

	//Num and logic compare, calculate by types and return bool.
	else if(int_compare[op_])
	{
		if(op_ == 'E' || op_ == 'N'){
			cerr << get_fileline() << " : case equality unspported!" << endl;
			exit(1);
		}

		assert(!(l_width == SMT_BOOL ^ r_width == SMT_BOOL));

		if(l_width == r_width){
			expr << "(" << int_compare[op_] << " " << l_expr.str() << " " << r_expr.str() << ")";
		}
		else{
			ostringstream l_int, r_int;

			if(l_width != SMT_INT) 
				bv_to_int(l_expr, l_int);
			else 
				l_int << l_expr.str();

			if(r_width != SMT_INT) 
				bv_to_int(r_expr, r_int);
			else 
				r_int << r_expr.str();

			expr << "(" << int_compare[op_] << " " << l_int.str() << " " << r_int.str() << ")";
		}
		return SMT_BOOL;
	}
	
	else
	{
		cerr << get_fileline() << " : NetEBinary operator " << op_ << " unspported!" << endl;
		exit(1);
	}
}

int NetEArrayPattern::dump_smt(map<string, RefVar*>& vars, set<SmtVar*>& used, ostringstream& expr, Module* md) const
{
	cerr << get_fileline() << " : Array pattern unspported!" << endl;
	exit(1);
	return SMT_NULL;
}

int NetELast::dump_smt(map<string, RefVar*>& vars, set<SmtVar*>& used, ostringstream& expr, Module* md) const
{
	cerr << get_fileline() << " : NetELast unspported!" << endl;
	exit(1);
	return SMT_NULL;
}

int NetEAccess::dump_smt(map<string, RefVar*>& vars, set<SmtVar*>& used, ostringstream& expr, Module* md) const
{
	cerr << get_fileline() << " : NetEAccess unspported!" << endl;
	exit(1);
	return SMT_NULL;
}

int NetEEvent::dump_smt(map<string, RefVar*>& vars, set<SmtVar*>& used, ostringstream& expr, Module* md) const
{
	cerr << get_fileline() << " : NetEvent unspported!" << endl;
	exit(1);
	return SMT_NULL;
}

int NetENetenum::dump_smt(map<string, RefVar*>& vars, set<SmtVar*>& used, ostringstream& expr, Module* md) const
{
	cerr << get_fileline() << " : NetENetenum unspported!" << endl;
	exit(1);
	return SMT_NULL;
}

int NetENew::dump_smt(map<string, RefVar*>& vars, set<SmtVar*>& used, ostringstream& expr, Module* md) const
{
	cerr << get_fileline() << " : NetENew unspported!" << endl;
	exit(1);
	return SMT_NULL;
}

int NetENull::dump_smt(map<string, RefVar*>& vars, set<SmtVar*>& used, ostringstream& expr, Module* md) const
{
	cerr << get_fileline() << " : NetENull unspported!" << endl;
	exit(1);
	return SMT_NULL;
}

int NetEProperty::dump_smt(map<string, RefVar*>& vars, set<SmtVar*>& used, ostringstream& expr, Module* md) const
{
	cerr << get_fileline() << " : NetEProperty unspported!" << endl;
	exit(1);
	return SMT_NULL;
}

int NetEShallowCopy::dump_smt(map<string, RefVar*>& vars, set<SmtVar*>& used, ostringstream& expr, Module* md) const
{
	cerr << get_fileline() << " : NetEShallowCopy unspported!" << endl;
	exit(1);
	return SMT_NULL;
}

int NetECString::dump_smt(map<string, RefVar*>& vars, set<SmtVar*>& used, ostringstream& expr, Module* md) const
{
	cerr << "Const string is unsupported now!" << endl;
	exit(1);
	return SMT_NULL;
}

int NetECReal::dump_smt(map<string, RefVar*>& vars, set<SmtVar*>& used, ostringstream& expr, Module* md) const
{
	cerr << get_fileline() << " : Real number is unsupported now!" << endl;
	exit(1);
	return SMT_NULL;
}

int NetEScope::dump_smt(map<string, RefVar*>& vars, set<SmtVar*>& used, ostringstream& expr, Module* md) const
{
	cerr << get_fileline() << " : Scope name unspported!" << endl;
	exit(1);
	return SMT_NULL;
}

int NetESFunc::dump_smt(map<string, RefVar*>& vars, set<SmtVar*>& used, ostringstream& expr, Module* md) const
{
	cerr << get_fileline() << " : System function call unspported!" << endl;
	exit(1);
	return SMT_NULL;
}

int NetEConcat::dump_smt(map<string, RefVar*>& vars, std::set<SmtVar*>& used, ostringstream& expr, Module* md) const
{
	cerr << "Concatation operator is unsupported now!" << endl;
	exit(1);
	return SMT_NULL;
}

int NetEUFunc::dump_smt(map<string, RefVar*>& vars, set<SmtVar*>& used, ostringstream& expr, Module* md) const
{
	cerr << "Function call is unsupported now!" << endl;
	exit(1);
	return SMT_NULL;
}

