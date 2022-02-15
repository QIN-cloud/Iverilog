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

void NetProcTop::dump_cond_clp(ofstream& o, unsigned lineno, set<RefVar*>& refs, set<clp_var>& used, ostringstream& expr, bool result, int caseitemidx, unsigned temp_idx) const
{
	statement_->dump_cond_clp(o, lineno, refs, used, expr, result, caseitemidx, temp_idx);
}

void NetProcTop::dump_expr_clp(ofstream& o, unsigned lineno, set<RefVar*>& refs, set<RefVar*>& lrefs, set<clp_var>& used, ostringstream& expr, bool result, int caseitemidx, unsigned temp_idx) const
{
	statement_->dump_expr_clp(o, lineno, refs, lrefs, used, expr, result, caseitemidx, temp_idx);
}


/* Dump a block statement */
void NetBlock::dump_cond_clp(ofstream& o, unsigned lineno, set<RefVar*>& refs, set<clp_var>& used, ostringstream& expr, bool result, int caseitemidx, unsigned temp_idx) const
{
	if (last_) {
		const NetProc*cur = last_;
		do {
			cur = cur->next_;
			cur->dump_cond_clp(o, lineno, refs, used, expr, result, caseitemidx, temp_idx);
			
		}while (cur != last_);
	}
}

void NetBlock::dump_expr_clp(ofstream& o, unsigned lineno, set<RefVar*>& refs, set<RefVar*>& lrefs, set<clp_var>& used, ostringstream& expr, bool result, int caseitemidx, unsigned temp_idx) const
{
	if (last_) {
		const NetProc*cur = last_;
		do {
			cur = cur->next_;
			cur->dump_expr_clp(o, lineno, refs, lrefs, used, expr, result, caseitemidx, temp_idx);
			
		}while (cur != last_);
	}
}

void NetCase::dump_cond_clp(ofstream& o, unsigned lineno, set<RefVar*>& refs, set<clp_var>& used, ostringstream& expr, bool result, int caseitemidx, unsigned temp_idx) const
{
	if(get_lineno() == lineno)
	{
		if(items_[caseitemidx].guard)
		{
			clp_var tmp = expr_->dump_cond_clp(o,lineno, refs, used, expr, temp_idx);
			o << expr.str();
			clp_var guard;
			
			guard = items_[caseitemidx].guard->dump_cond_clp(o, lineno, refs, used, expr, temp_idx);
			if(items_[caseitemidx].statement)
				items_[caseitemidx].statement->dump_cond_clp(o, lineno, refs, used, expr,result,caseitemidx, temp_idx);
			o << expr.str();
			
			clp_var var;
			ostringstream os;
			os << temp_idx;
			var.basename = "";
			var.clpname = "Temp_" + os.str();
			var.parentname = "";
			var.index = -1;
			var.lsb = 0;
			var.msb = 0;
			var.width = 1;
			var.type = "OUTPUT";
			
			used.insert(var);
			used.insert(tmp);
			used.insert(guard);
			
			o << "Temp_" << temp_idx << " #<=> " << tmp.clpname << " #= " << guard.clpname << "," << endl;
			temp_idx = temp_idx + 1;
		}
	}
}

void NetCase::dump_expr_clp(ofstream& o, unsigned lineno, set<RefVar*>& refs, set<RefVar*>& lrefs, set<clp_var>& used, ostringstream& expr, bool result, int caseitemidx, unsigned temp_idx) const
{
	for (unsigned idx = 0 ;  idx < nitems_ ;  idx += 1) 
	{
		if (items_[idx].statement) 
		{
			items_[idx].statement->dump_expr_clp(o, lineno, refs, lrefs, used, expr, result, caseitemidx, temp_idx);
		} 
    }
}

void NetCAssign::dump_cond_clp(ofstream& o, unsigned lineno, set<RefVar*>& refs, set<clp_var>& used, ostringstream& expr, bool result, int caseitemidx, unsigned temp_idx) const
{
}

void NetCAssign::dump_expr_clp(ofstream& o, unsigned lineno, set<RefVar*>& refs, set<RefVar*>& lrefs, set<clp_var>& used, ostringstream& expr, bool result, int caseitemidx, unsigned temp_idx) const
{
}

void NetCondit::dump_cond_clp(ofstream& o, unsigned lineno, set<RefVar*>& refs, set<clp_var>& used, ostringstream& expr, bool result, int caseitemidx, unsigned temp_idx) const
{
	if(get_lineno() == lineno)
	{
		clp_var tmp = expr_->dump_cond_clp(o,lineno, refs, used, expr, temp_idx);

		clp_var var;
		ostringstream os;
		os << temp_idx;
		var.basename = "";
		var.clpname = "Temp_" + os.str();
		var.parentname = "";
		var.index = -1;
		var.lsb = 0;
		var.msb = 0;
		var.width = 1;
		var.type = "OUTPUT";

		used.insert(var);
		used.insert(tmp);

		
		o << expr.str() << endl;
		if(result)
			o << "Temp_" << temp_idx << " #<=> " << tmp.clpname << " #= 1," << endl;
		else
			o << "Temp_" << temp_idx << " #<=> " << tmp.clpname << " #= 0," << endl;
	}
	temp_idx = temp_idx + 1;
	if (if_) if_->dump_cond_clp(o, lineno, refs, used, expr, result,caseitemidx, temp_idx);
    if (else_)else_->dump_cond_clp(o, lineno, refs, used, expr, result,caseitemidx, temp_idx);
}

void NetCondit::dump_expr_clp(ofstream& o, unsigned lineno, set<RefVar*>& refs, set<RefVar*>& lrefs, set<clp_var>& used, ostringstream& expr, bool result, int caseitemidx, unsigned temp_idx) const
{
	if (if_) if_->dump_expr_clp(o, lineno, refs, lrefs, used, expr, result,caseitemidx, temp_idx);
    if (else_)else_->dump_expr_clp(o, lineno, refs, lrefs, used, expr, result,caseitemidx, temp_idx);
}

void NetDeassign::dump_cond_clp(ofstream& o, unsigned lineno, set<RefVar*>& refs, set<clp_var>& used, ostringstream& expr, bool result, int caseitemidx, unsigned temp_idx) const
{
}

void NetDeassign::dump_expr_clp(ofstream& o, unsigned lineno, set<RefVar*>& refs, set<RefVar*>& lrefs, set<clp_var>& used, ostringstream& expr, bool result, int caseitemidx, unsigned temp_idx) const
{
}

void NetDisable::dump_cond_clp(ofstream& o, unsigned lineno, set<RefVar*>& refs, set<clp_var>& used, ostringstream& expr, bool result, int caseitemidx, unsigned temp_idx) const
{
}

void NetDisable::dump_expr_clp(ofstream& o, unsigned lineno, set<RefVar*>& refs, set<RefVar*>& lrefs, set<clp_var>& used, ostringstream& expr, bool result, int caseitemidx, unsigned temp_idx) const
{
}

void NetEvTrig::dump_cond_clp(ofstream& o, unsigned lineno, set<RefVar*>& refs, set<clp_var>& used, ostringstream& expr, bool result, int caseitemidx, unsigned temp_idx) const
{
}

void NetEvTrig::dump_expr_clp(ofstream& o, unsigned lineno, set<RefVar*>& refs, set<RefVar*>& lrefs, set<clp_var>& used, ostringstream& expr, bool result, int caseitemidx, unsigned temp_idx) const
{
}

void NetEvWait::dump_cond_clp(ofstream& o, unsigned lineno, set<RefVar*>& refs, set<clp_var>& used, ostringstream& expr, bool result, int caseitemidx, unsigned temp_idx) const
{
	if (statement_)
		statement_->dump_cond_clp(o, lineno, refs, used, expr, result, caseitemidx, temp_idx);
}

void NetEvWait::dump_expr_clp(ofstream& o, unsigned lineno, set<RefVar*>& refs, set<RefVar*>& lrefs, set<clp_var>& used, ostringstream& expr, bool result, int caseitemidx, unsigned temp_idx) const
{
	if (statement_)
		statement_->dump_expr_clp(o, lineno, refs, lrefs, used, expr, result, caseitemidx, temp_idx);
}

void NetForce::dump_cond_clp(ofstream& o, unsigned lineno, set<RefVar*>& refs, set<clp_var>& used, ostringstream& expr, bool result, int caseitemidx, unsigned temp_idx) const
{
}

void NetForce::dump_expr_clp(ofstream& o, unsigned lineno, set<RefVar*>& refs, set<RefVar*>& lrefs, set<clp_var>& used, ostringstream& expr, bool result, int caseitemidx, unsigned temp_idx) const
{
}

void NetForever::dump_cond_clp(ofstream& o, unsigned lineno, set<RefVar*>& refs, set<clp_var>& used, ostringstream& expr, bool result, int caseitemidx, unsigned temp_idx) const
{
}

void NetForever::dump_expr_clp(ofstream& o, unsigned lineno, set<RefVar*>& refs, set<RefVar*>& lrefs, set<clp_var>& used, ostringstream& expr, bool result, int caseitemidx, unsigned temp_idx) const
{
}

void NetPDelay::dump_cond_clp(ofstream& o, unsigned lineno, set<RefVar*>& refs, set<clp_var>& used, ostringstream& expr, bool result, int caseitemidx, unsigned temp_idx) const
{
}

void NetPDelay::dump_expr_clp(ofstream& o, unsigned lineno, set<RefVar*>& refs, set<RefVar*>& lrefs, set<clp_var>& used, ostringstream& expr, bool result, int caseitemidx, unsigned temp_idx) const
{
}

void NetRelease::dump_cond_clp(ofstream& o, unsigned lineno, set<RefVar*>& refs, set<clp_var>& used, ostringstream& expr, bool result, int caseitemidx, unsigned temp_idx) const
{
}

void NetRelease::dump_expr_clp(ofstream& o, unsigned lineno, set<RefVar*>& refs, set<RefVar*>& lrefs, set<clp_var>& used, ostringstream& expr, bool result, int caseitemidx, unsigned temp_idx) const
{
}

void NetRepeat::dump_cond_clp(ofstream& o, unsigned lineno, set<RefVar*>& refs, set<clp_var>& used, ostringstream& expr, bool result, int caseitemidx, unsigned temp_idx) const
{
	if(get_lineno() == lineno)
	{
		clp_var tmp = expr_->dump_cond_clp(o, lineno, refs, used, expr, temp_idx);

		clp_var var;
		ostringstream os;
		os << temp_idx;
		var.basename = "";
		var.clpname = "Temp_" + os.str();
		var.parentname = "";
		var.index = -1;
		var.lsb = 0;
		var.msb = 0;
		var.width = 1;
		var.type = "OUTPUT";

		used.insert(var);
		used.insert(tmp);

		o << expr.str() << endl;
		if(result)
			o << "Temp_" << temp_idx << " #<=> " << tmp.clpname << " #= 1," << endl;
		else
			o << "Temp_" << temp_idx << " #<=> " << tmp.clpname << " #= 0," << endl;
	}
	temp_idx = temp_idx + 1;
	if(statement_) statement_->dump_cond_clp(o, lineno, refs, used, expr, result,caseitemidx, temp_idx);
}

void NetRepeat::dump_expr_clp(ofstream& o, unsigned lineno, set<RefVar*>& refs, set<RefVar*>& lrefs, set<clp_var>& used, ostringstream& expr, bool result, int caseitemidx, unsigned temp_idx) const
{
	if(statement_) statement_->dump_expr_clp(o, lineno, refs, lrefs, used, expr, result,caseitemidx, temp_idx);
}

void NetSTask::dump_cond_clp(ofstream& o, unsigned lineno, set<RefVar*>& refs, set<clp_var>& used, ostringstream& expr, bool result, int caseitemidx, unsigned temp_idx) const
{
}

void NetSTask::dump_expr_clp(ofstream& o, unsigned lineno, set<RefVar*>& refs, set<RefVar*>& lrefs, set<clp_var>& used, ostringstream& expr, bool result, int caseitemidx, unsigned temp_idx) const
{
}

void NetUTask::dump_cond_clp(ofstream& o, unsigned lineno, set<RefVar*>& refs, set<clp_var>& used, ostringstream& expr, bool result, int caseitemidx, unsigned temp_idx) const
{
}

void NetUTask::dump_expr_clp(ofstream& o, unsigned lineno, set<RefVar*>& refs, set<RefVar*>& lrefs, set<clp_var>& used, ostringstream& expr, bool result, int caseitemidx, unsigned temp_idx) const
{
}

void NetWhile::dump_cond_clp(ofstream& o, unsigned lineno, set<RefVar*>& refs, set<clp_var>& used, ostringstream& expr, bool result, int caseitemidx, unsigned temp_idx) const
{
	if(get_lineno() == lineno)
	{
		clp_var tmp = cond_->dump_cond_clp(o, lineno, refs, used, expr, temp_idx);

		clp_var var;
		ostringstream os;
		os << temp_idx;
		var.basename = "";
		var.clpname = "Temp_" + os.str();
		var.parentname = "";
		var.index = -1;
		var.lsb = 0;
		var.msb = 0;
		var.width = 1;
		var.type = "OUTPUT";

		used.insert(var);
		used.insert(tmp);

		o << expr.str() << endl;
		if(result)
			o << "Temp_" << temp_idx << " #<=> " << tmp.clpname << " #= 1," << endl;
		else
			o << "Temp_" << temp_idx << " #<=> " << tmp.clpname << " #= 0," << endl;
	}
	temp_idx = temp_idx + 1;
	if(proc_) proc_->dump_cond_clp(o, lineno, refs, used, expr, result,caseitemidx, temp_idx);
}

void NetWhile::dump_expr_clp(ofstream& o, unsigned lineno, set<RefVar*>& refs, set<RefVar*>& lrefs, set<clp_var>& used, ostringstream& expr, bool result, int caseitemidx, unsigned temp_idx) const
{
	if(proc_) proc_->dump_expr_clp(o, lineno, refs, lrefs, used, expr, result,caseitemidx, temp_idx);
}

/* Dump a statement type that someone didn't write a dump for. */
void NetProc::dump_cond_clp(ofstream& o, unsigned lineno, set<RefVar*>& refs, set<clp_var>& used, ostringstream& expr, bool result, int caseitemidx, unsigned temp_idx) const
{
	//o << lineno << "" << "// " << typeid(*this).name() << endl;
}

void NetProc::dump_expr_clp(ofstream& o, unsigned lineno, set<RefVar*>& refs, set<RefVar*>& lrefs, set<clp_var>& used, ostringstream& expr, bool result, int caseitemidx, unsigned temp_idx) const
{
	//o << lineno << "" << "// " << typeid(*this).name() << endl;
}

clp_var NetAssign_::dump_lval_clp(ofstream&o, unsigned lineno, set<RefVar*>& refs, set<clp_var>& used, ostringstream& expr, unsigned temp_idx) const
{
	if (sig_) 
	{
		if (bmux_) 
		{
			cerr << "At line : " << lineno << " : Non-const bit selection unsupported now for test generation!" << endl;
			exit(0);
		} 
		for(set<RefVar*>::const_iterator rvpos = refs.begin(); rvpos != refs.end(); ++rvpos)
		{
			if(name() == (*rvpos)->name) //find the variables
			{
				if(lwid_ == (*rvpos)->width) //bit selection
				{
					clp_var cv;
					ostringstream name;
					unsigned time = (*rvpos)->time;
					name << "S_" << (*rvpos)->name << "_" << time << "_" << (*rvpos)->space;
					cv.basename = (*rvpos)->name;
					cv.clpname = name.str();
					cv.index = -1;
					cv.parentname = "";
					cv.width = (*rvpos)->width;
					cv.lsb = (*rvpos)->lsb;
					cv.msb = (*rvpos)->msb;
					cv.type = (*rvpos)->ptype;
					used.insert(cv);
					return cv;
				}
				else //section selection
				{
					//generate a variable for the whole variable
					ostringstream name, name1, name2, name3;
					clp_var var;
					unsigned time = (*rvpos)->time;
					name << "S_" << (*rvpos)->name << "_" << time << "_" << (*rvpos)->space;
					var.basename = (*rvpos)->name;
					var.clpname = name.str();
					var.index = -1;
					var.parentname = "";
					var.width = (*rvpos)->width;
					var.lsb = (*rvpos)->lsb;
					var.msb = (*rvpos)->msb;
					var.type = (*rvpos)->ptype;
					used.insert(var);
					
					expr << var.clpname << " #= ";
					
					//first new a temp variable for the bit(s) selection
					clp_var p3;
					name1 << "S_" << (*rvpos)->name << "_" << (*rvpos)->time << "_" << (*rvpos)->space << "_" << lwid_ + loff_ - 1;
					p3.basename = (*rvpos)->name;
					p3.clpname = name1.str();
					p3.index = loff_;
					p3.parentname = (*rvpos)->name;
					p3.width = lwid_;
					p3.lsb = loff_;
					p3.msb = lwid_ + loff_ - 1;
					p3.type = "NOINPUT";
					used.insert(p3);
					
					int i = pow(2, lwid_ + loff_ - 1);
					
					expr << i << " * " << p3.clpname;
					
					//the selection divide the variable_time_space into 2 or 3 parts
					//genearte temp variables for the other two parts
					if(p3.msb != (*rvpos)->msb) //3 parts, and p1 is the first part, from msb to msi_ - 1;
					{
						clp_var p1;
						name2 << "S_" << (*rvpos)->name << "_" << (*rvpos)->time << "_" << (*rvpos)->space << "_" << (*rvpos)->msb;
						p1.basename = (*rvpos)->name;
						p1.clpname = name2.str();
						p1.index = (*rvpos)->msb;
						p1.parentname = (*rvpos)->name;
						p1.width = (*rvpos)->msb - p3.msb;
						p1.lsb = p3.msb + 1;
						p1.msb = (*rvpos)->msb;
						p1.type = "NOINPUT";
						used.insert(p1);
						
						i = pow(2, p1.msb);
						expr << " + " << i << " * " << p1.clpname;
					}
					//generate the least part, p2 is the part from lsi_+1 to lsb
					if(loff_ != (*rvpos)->lsb)
					{
						clp_var p2;
						name3 << "S_" << (*rvpos)->name << "_" << (*rvpos)->time << "_" << (*rvpos)->space << "_" << loff_ - 1;
						p2.basename = (*rvpos)->name;
						p2.clpname = name3.str();
						p2.index = loff_ - 1;
						p2.parentname = (*rvpos)->name;
						p2.width = loff_ - (*rvpos)->lsb;
						p2.lsb = (*rvpos)->lsb;
						p2.msb = loff_ - 1;
						p2.type = "NOINPUT";
						used.insert(p2);
						
						i = pow(2, (p2.width - 1));
						expr << " + " << i << " * " << p2.clpname;
					}
					expr << "," << endl;
					return p3;
				}
			}
		}
	} 
	else if (mem_) 
	{
		cerr << "At line : " << lineno << " : Memory type is unsupported now for test generation!" << endl;
		exit(0);
	} 
	else if (var_) 
	{
		cerr << "At line : " << lineno << " : Real type variable is unsupported now for test generation!" << endl;
		exit(0);
	} 
	else 
	{
		cerr << "At line : " << lineno << " : Unrecognized left value!" << endl;
		exit(0);
	}
}

void NetAssignBase::dump_expr_clp(ofstream& o, unsigned lineno, set<RefVar*>& refs, set<RefVar*>& lrefs, set<clp_var>& used, ostringstream& expr, bool result, int caseitemidx, unsigned temp_idx) const
{
	if(get_lineno() == lineno)
	{
		if(l_val_count() > 1)
		{
			cerr << "At line:" << lineno << " : Concatation expression isn't supported now" << endl;
			exit(0);
		}
		clp_var rcv = rval_->dump_cond_clp(o, lineno, refs, used, expr, temp_idx);
		clp_var lcv = lval_->dump_lval_clp(o, lineno, lrefs, used, expr, temp_idx);
		o<< expr.str() << endl;
		o << lcv.clpname << " #= " << rcv.clpname << "," << endl;
	}
}

void NetAssignBase::dump_cond_clp(ofstream&o, unsigned lineno, set<RefVar*>& refs, set<clp_var>& used, ostringstream& expr, unsigned temp_idx) const
{
}

void NetAssign::dump_cond_clp(ofstream&o, unsigned lineno, set<RefVar*>& refs, set<clp_var>& used, ostringstream& expr, unsigned temp_idx) const
{
}

void NetAssignNB::dump_cond_clp(ofstream&o, unsigned lineno, set<RefVar*>& refs, set<clp_var>& used, ostringstream& expr, unsigned temp_idx) const
{
}