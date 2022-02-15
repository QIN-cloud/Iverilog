#include "config.h"
/*
* This file contains all the dump methods of the netlist classes.
*/
#include <typeinfo>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <math.h>
#include "netlist.h"
#include "cfg.h"
#include "testpath.h"

/* 
* Convert BitVec type to Int type.
* The format is ( (_ bv2int) BitVec ).
*/
void NetProc::bv_to_int(ostringstream& expr, ostringstream& target) const
{
	target << "((_ bv2int)" << expr.str() << ")";
}
/* 
* Convert Int type to BitVec type.
* Request a width of BitVec.
* The format is ( (_ int2bv width) Int ).
*/
void NetProc::int_to_bv(ostringstream& expr, int width, ostringstream& target) const
{
	target << "((_ int2bv " << width << ")" << expr.str() << ")";
}
/*
* Convert a BitVec to a BitVec for changing the width of the BitVec.
* The format is ( (_ int2bv width ) ( (_ bv2int ) BitVec ) ).
*/
void NetProc::bv_int_bv(ostringstream& expr, int width, ostringstream& target) const
{
	target << "((_ int2bv " << width << ")" << "((_ bv2int)" << expr.str() << "))";
}
/*
* Compare a BitVec with #b0 which need to expand the number of bits.
*/
void NetProc::bv_compare_zero(ostringstream& expr, string op_, int width, ostringstream& target) const
{
	target << "(" << op_ << " " << expr.str() << " " << "#b";
	for(int i = 0; i < width; i++){target << "0";}
	target << ")";
}

void NetProcTop::set_id(unsigned id)
{
	id_ = id;
}

unsigned NetProcTop::get_id()
{
	return id_;
}

/*
* Traverse the netlist and record the statements which can be generated to SMT-LIB2 format. 
*/
void NetProc::gen_stats(smt_stats* proc_stats)
{
	cout << get_fileline() << "Unsupport now!" << endl;
}


void NetProcTop::gen_stats(smt_stats* proc_stats)
{
	statement_->gen_stats(proc_stats);
}

void NetBlock::gen_stats(smt_stats* proc_stats)
{
	if(last_){
		NetProc*cur = last_;
		do{
			cur = cur->next_;
			cur->gen_stats(proc_stats);
			
		}while(cur != last_);
	}
}

void NetCase::gen_stats(smt_stats* proc_stats)
{
	(*proc_stats)[get_lineno()].insert(this); 
	for(unsigned idx = 0; idx < items_.size(); idx += 1) 
	{
		if(items_[idx].statement) 
		{
			items_[idx].statement->gen_stats(proc_stats);
		} 
    }
}

void NetCondit::gen_stats(smt_stats* proc_stats)
{
	(*proc_stats)[get_lineno()].insert(this); 
	if(if_) if_->gen_stats(proc_stats);
    if(else_) else_->gen_stats(proc_stats);
}

void NetEvWait::gen_stats(smt_stats* proc_stats)
{
	if(statement_) statement_->gen_stats(proc_stats);
}

void NetRepeat::gen_stats(smt_stats* proc_stats)
{
	(*proc_stats)[get_lineno()].insert(this); 
	if(statement_) statement_->gen_stats(proc_stats);
}

void NetWhile::gen_stats(smt_stats* proc_stats)
{
	(*proc_stats)[get_lineno()].insert(this); 
	if(proc_) proc_->gen_stats(proc_stats);
}

void NetAssignBase::gen_stats(smt_stats* proc_stats)
{
	(*proc_stats)[get_lineno()].insert(this); 
}

void NetCAssign::gen_stats(smt_stats* proc_stats)
{

}

void NetDeassign::gen_stats(smt_stats* proc_stats)
{

}

void NetDisable::gen_stats(smt_stats* proc_stats)
{

}

void NetEvTrig::gen_stats(smt_stats* proc_stats)
{

}

void NetForce::gen_stats(smt_stats* proc_stats)
{

}

void NetForever::gen_stats(smt_stats* proc_stats)
{

}

void NetPDelay::gen_stats(smt_stats* proc_stats)
{

}

void NetRelease::gen_stats(smt_stats* proc_stats)
{

}

void NetSTask::gen_stats(smt_stats* proc_stats)
{

}

void NetUTask::gen_stats(smt_stats* proc_stats)
{

}
/*
* Generate SMT-LIB2 statement for control statement type.
*/
void NetProc::dump_smt(ofstream& o, set<RefVar*>& refs, set<smt_var>& used, bool result, int caseitemidx, const char* modname) const
{

}
/*
* Generate SMT-LIB2 statement for a assign statement type in always.
*/
void NetProc::dump_smt(ofstream& o, set<RefVar*>& refs, set<RefVar*>& lrefs, set<smt_var>& used, const char* modname) const
{
	
}
/*
* Generate SMT-LIB2 statement for a base assign statement in always.
* For example, "a[1] = 0" or "a[1:0] = 2'b00" or "a = 0".
*/
void NetAssignBase::dump_smt(ofstream& o, set<RefVar*>& refs, set<RefVar*>& lrefs, set<smt_var>& used, const char* modname) const
{
	//We can't generate the expression like "{a[1:0], b[1:0]} = 4'b0000" temporarily.
	if(l_val_count() > 1)
	{
		cerr << get_fileline() << " : Concatation expression isn't supported now" << endl;
		exit(0);
	}

	//Generate the respective SMT formats for left and right expressions.
	//Then verify the formats of expressions are correctly.
	ostringstream r_expr;
	ostringstream l_expr;
	int r_width = rval_->dump_smt(refs, used, r_expr, modname);
	int l_width = lval_->dump_smt(lrefs, used, l_expr, modname);
	if(l_width == SMT_WRONG)
	{
		cerr << get_fileline() << " Can't find left variable in lrefences." << endl;
		exit(0);
	}
	assert(r_width != SMT_NULL && r_width != SMT_BOOL);
	assert(l_width != SMT_INT && l_width != SMT_NULL && l_width != SMT_BOOL);

	//Establish an equality relationship between the left and right expressions.
	//If the bit widths of the left and right expressions are different,
	//convert the right expression according to the width of left expression.
	ostringstream r_equal;
	if(l_width == r_width) r_equal << r_expr.str();
	else
	{
		//Right expression is a int number, then convert it to a bitvec.
		//For example, "a = 3" or "a = 2'b11(Convert to Int)".
		if(r_width == SMT_INT) int_to_bv(r_expr, l_width, r_equal);
		//The width of right is unequal to left, convert to fit the left width.
		//For example, "a = b" with a.width = 3 and b.width = 4.
		else bv_int_bv(r_expr, l_width, r_equal);
	}
	o << "(assert" << "(= " << l_expr.str() << " " << r_equal.str() << "))" << endl;;
}
/*
* Generate SMT-LIB2 statement for left expression of assign statement.
*/
int NetAssign_::dump_smt(set<RefVar*>& lrefs, set<smt_var>& used, ostringstream& expr, const char* modname) const
{
	if(sig_)
	{
		bool find = false;
		int width;
		//Find the variable from the left references set.
		for(set<RefVar*>::iterator rvpos = lrefs.begin(); rvpos != lrefs.end(); rvpos++)
		{
			//Get the time and space number and generate the smt name.
			if(sig_->name() == perm_string((*rvpos)->name.c_str()))
			{
				ostringstream name;
				smt_var sv;
				find = true;
				width = lwid_;
				name << modname << "_" << (*rvpos)->name << "_" << (*rvpos)->time << "_" << (*rvpos)->space;
				sv.smtname = name.str();
				sv.basename = (*rvpos)->name;
				sv.lsb = (*rvpos)->lsb;
				sv.msb = (*rvpos)->msb;
				sv.width = (*rvpos)->width;
				sv.type = (*rvpos)->ptype;
				sv.temp_flag = false;
				used.insert(sv);
				//
				if(width != sv.width)
				{
					if(const NetEConst* lsi_ = dynamic_cast<const NetEConst*>(base_))
					{
						long lsi = lsi_->value().as_ulong();
						expr << "((_ extract " << lsi+lwid_-1 << " " << lsi << ")" << name.str() << ")";
					}
					else
					{
						cerr << get_fileline() << " : Non-const part select is unsupported now!" << endl;
						exit(0);
					}
				}
				//
				else
				{
					expr << name.str();
				}
			}
		}
		if(!find)
		{
			return SMT_WRONG;
		}
		return width;
	}
	//We can't generate memory temporarily.
	else
	{
		cerr << get_fileline() << " Memory is unsopport now!" << endl;
		exit(0);
	}
}
/*
  Generate SMT-LIB2 statement for a if-else cond expression.
*/
void NetCondit::dump_smt(ofstream& o, set<RefVar*>& refs, set<smt_var>& used, bool result, int caseitemidx, const char* modname) const
{
	ostringstream expr;
	int width = expr_->dump_smt(refs, used, expr, modname);
	assert(width != SMT_NULL);
	//condit expression is a bool
	string res;
	if(width == SMT_BOOL)
	{
		res = result ? "true" : "false";
		o << "(assert(= " << expr.str() << " " << res << "))" << endl;
	}
	else
	{
		ostringstream expr_bool;
		res = result ? "distinct" : "=";
		if(width == SMT_INT) expr_bool << "(" << res << " " << expr.str() << " 0)";
		else bv_compare_zero(expr, res, width, expr_bool);
		o << "(assert" << expr_bool.str() << ")" << endl;
	}
}
/*
  Generate SMT-LIB2 statement for a while cond expression.
*/
void NetWhile::dump_smt(ofstream& o, set<RefVar*>& refs, set<smt_var>& used, bool result, int caseitemidx, const char* modname) const
{
	ostringstream expr;
	int width = cond_->dump_smt(refs, used, expr, modname);
	assert(width != SMT_NULL);
	//condit expression is a bool
	string res;
	if(width == SMT_BOOL)
	{
		res = result ? "true" : "false";
		o << "(assert(= " << expr.str() << " " << res << "))" << endl;
	}
	else
	{
		ostringstream expr_bool;
		res = result ? "distinct" : "=";
		if(width == SMT_INT) expr_bool << "(" << res << " " << expr.str() << " 0)";
		else bv_compare_zero(expr, res, width, expr_bool);
		o << "(assert" << expr_bool.str() << ")" << endl;
	}
}
/*
  Generate SMT-LIB2 statement for a repeat cond expression.
*/
void NetRepeat::dump_smt(ofstream& o, set<RefVar*>& refs, set<smt_var>& used, bool result, int caseitemidx, const char* modname) const
{
	ostringstream expr;
	int width = expr_->dump_smt(refs, used, expr, modname);
	assert(width != SMT_NULL);
	//condit expression is a bool
	string res;
	if(width == SMT_BOOL)
	{
		res = result ? "true" : "false";
		o << "(assert(= " << expr.str() << " " << res << "))" << endl;
	}
	else
	{
		ostringstream expr_bool;
		res = result ? "distinct" : "=";
		if(width == SMT_INT) expr_bool << "(" << res << " " << expr.str() << " 0)";
		else bv_compare_zero(expr, res, width, expr_bool);
		o << "(assert" << expr_bool.str() << ")" << endl;
	}
}
/*
  Generate SMT-LIB2 statement for a case cond expression.
*/
void NetCase::dump_smt(ofstream& o, set<RefVar*>& refs, set<smt_var>& used, bool result, int caseitemidx, const char* modname) const
{
	ostringstream  c_expr;
	int c_width;
	c_width = expr_->dump_smt(refs, used, c_expr, modname);
	assert(c_width != SMT_NULL && c_width != SMT_BOOL);
	//Line of case item, build a equal smt assert for this case item with cond expression.
	if(caseitemidx >= 0)
	{
		ostringstream i_expr;
		int i_width;
		i_width = items_[caseitemidx].guard->dump_smt(refs, used, i_expr, modname);
		ostringstream c_equal, i_equal;
		if(c_width == i_width && c_width != SMT_INT)
		{
			c_equal << c_expr.str();
			i_equal << i_expr.str();
		}
		else
		{
			if(c_width != SMT_INT) bv_to_int(c_expr, c_equal);
			else c_equal << c_expr.str();
			if(i_width != SMT_INT) bv_to_int(i_expr, i_equal);
			else i_equal << i_expr.str();
		}
		o << "(assert(= " << c_equal.str() << " " << i_equal.str() << "))" << endl;
	}
	//Default line, build not equal SMT asserts for every case item with cond expression.
	else
	{
		smt_var sv;
		sv.smtname = "Temp_" + to_string(used.size());
		sv.type = "None";
		sv.temp_flag = true;
		used.insert(sv);
		ostringstream c_expr_int;
		if(c_width == SMT_INT) c_expr_int << c_expr.str();
		else bv_to_int(c_expr, c_expr_int);
		o << "(assert(= " << sv.smtname << " " << c_expr_int.str() << "))" << endl;

		for(int idx = 0; idx < items_.size(); idx++)
		{
			if(items_[idx].guard)
			{
				ostringstream i_expr;
				int i_width;
				i_width = items_[idx].guard->dump_smt(refs, used, i_expr, modname);
				ostringstream i_expr_int;
				if(i_width != SMT_INT) bv_to_int(i_expr, i_expr_int);
				else i_expr_int << i_expr.str();
				o << "(assert(distinct " << sv.smtname << " " << i_expr_int.str() << "))" << endl;
			}
		}
	}
}

