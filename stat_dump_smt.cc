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
#include "Module.h"
#include "smt_generator.h"
#include "Statement.h"

void NetProcTop::set_id(unsigned id)
{
	id_ = id;
}

unsigned NetProcTop::get_id()
{
	return id_;
}

void NetProcTop::set_sync(bool sync)
{
	sync_ = sync;
}

bool NetProcTop::get_sync()
{
	return sync_;
}

/*
* Traverse the netlist and record the statements which can be generated to SMT-LIB2 format. 
*/
void NetProc::gen_stats(NetStats *proc_stats)
{
}

void NetProcTop::gen_stats(NetStats *proc_stats)
{
	statement_->gen_stats(proc_stats);
}

void NetBlock::gen_stats(NetStats *proc_stats)
{
	if (last_)
	{
		NetProc *cur = last_;
		do
		{
			cur = cur->next_;
			cur->gen_stats(proc_stats);

		} while (cur != last_);
	}
}

void NetCase::gen_stats(NetStats *proc_stats)
{
	(*proc_stats)[get_lineno()].insert(this);
	for (unsigned idx = 0; idx < items_.size(); idx += 1)
	{
		if (items_[idx].statement)
		{
			items_[idx].statement->gen_stats(proc_stats);
		}
	}
}

void NetCondit::gen_stats(NetStats *proc_stats)
{
	(*proc_stats)[get_lineno()].insert(this);
	if (if_)
		if_->gen_stats(proc_stats);
	if (else_)
		else_->gen_stats(proc_stats);
}

void NetEvWait::gen_stats(NetStats *proc_stats)
{
	if (statement_)
		statement_->gen_stats(proc_stats);
}

void NetRepeat::gen_stats(NetStats *proc_stats)
{
	(*proc_stats)[get_lineno()].insert(this);
	if (statement_)
		statement_->gen_stats(proc_stats);
}

void NetWhile::gen_stats(NetStats *proc_stats)
{
	(*proc_stats)[get_lineno()].insert(this);
	if (proc_)
		proc_->gen_stats(proc_stats);
}

void NetAssignBase::gen_stats(NetStats *proc_stats)
{
	(*proc_stats)[get_lineno()].insert(this);
}

void NetCAssign::gen_stats(NetStats *proc_stats)
{
}

void NetDeassign::gen_stats(NetStats *proc_stats)
{
}

void NetDisable::gen_stats(NetStats *proc_stats)
{
}

void NetEvTrig::gen_stats(NetStats *proc_stats)
{
}

void NetForce::gen_stats(NetStats *proc_stats)
{
}

void NetForever::gen_stats(NetStats *proc_stats)
{
}

void NetPDelay::gen_stats(NetStats *proc_stats)
{
}

void NetRelease::gen_stats(NetStats *proc_stats)
{
}

void NetSTask::gen_stats(NetStats *proc_stats)
{
}

void NetUTask::gen_stats(NetStats *proc_stats)
{
}

/*
* Generate SMT-LIB2 statement for control statement type.
*/
void NetProc::dump_smt(ostream &o, map<string, RefVar *> &vars, set<SmtVar *> &used, bool result, int caseitemidx, Module *md, set<string> &refs) const
{
}

/*
* Generate SMT-LIB2 statement for a assign statement type in always.
*/
void NetProc::dump_smt(ostream &o, map<string, RefVar *> &vars, set<SmtVar *> &used, Module *md, set<string> &refs, unsigned cur_time) const
{
}

/*
* Generate SMT-LIB2 statement for a base assign statement in always.
* For example, "a[1] = 0" or "a[1:0] = 2'b00" or "a = 0".
*/
void NetAssignBase::dump_smt(ostream &o, map<string, RefVar *> &vars, set<SmtVar *> &used, Module *md, set<string> &refs, unsigned cur_time) const
{
	//We can't generate the expression like "{a[1:0], b[1:0]} = 4'b0000" temporarily.
	if (l_val_count() > 1)
	{
		cerr << get_fileline() << " : Concatation expression isn't supported now" << endl;
		exit(0);
	}

	//Generate the respective SMT formats for left and right expressions.
	//Then verify the formats of expressions are correctly.
	ostringstream r_expr;
	ostringstream l_expr;
	int r_width = rval_->dump_smt(vars, used, r_expr, md);
	int l_width = lval_->dump_smt(vars, used, l_expr, md, true, refs, cur_time);
	if (l_width == SMT_WRONG)
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
	if (l_width == r_width)
		r_equal << r_expr.str();
	else
	{
		//Right expression is a int number, then convert it to a bitvec.
		//For example, "a = 3" or "a = 2'b11(Convert to Int)".
		if (r_width == SMT_INT)
			int_to_bv(r_expr, l_width, r_equal);
		//The width of right is unequal to left, convert to fit the left width.
		//For example, "a = b" with a.width = 3 and b.width = 4.
		else
			bv_int_bv(r_expr, l_width, r_equal);
	}
	o << "(assert"
	  << "(= " << l_expr.str() << " " << r_equal.str() << "))" << endl;
}

void NetAssignNB::dump_smt(ostream &o, map<string, RefVar *> &vars, set<SmtVar *> &used, Module *md, set<string> &refs, unsigned cur_time) const
{
	//We can't generate the expression like "{a[1:0], b[1:0]} = 4'b0000" temporarily.
	if (l_val_count() > 1)
	{
		cerr << get_fileline() << " : Concatation expression isn't supported now" << endl;
		exit(0);
	}

	//Generate the respective SMT formats for left and right expressions.
	//Then verify the formats of expressions are correctly.
	ostringstream r_expr;
	ostringstream l_expr;
	int r_width = rval()->dump_smt(vars, used, r_expr, md);
	int l_width = lval()->dump_smt(vars, used, l_expr, md, false, refs, cur_time);
	if (l_width == SMT_WRONG)
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
	if (l_width == r_width)
		r_equal << r_expr.str();
	else
	{
		//Right expression is a int number, then convert it to a bitvec.
		//For example, "a = 3" or "a = 2'b11(Convert to Int)".
		if (r_width == SMT_INT)
			int_to_bv(r_expr, l_width, r_equal);
		//The width of right is unequal to left, convert to fit the left width.
		//For example, "a = b" with a.width = 3 and b.width = 4.
		else
			bv_int_bv(r_expr, l_width, r_equal);
	}
	o << "(assert"
	  << "(= " << l_expr.str() << " " << r_equal.str() << "))" << endl;
}

/*
* Generate SMT-LIB2 statement for left expression of assign statement.
*/
int NetAssign_::dump_smt(map<string, RefVar *> &vars, set<SmtVar *> &used, ostringstream &expr, Module *md, bool base_type, set<string> &refs, unsigned cur_time) const
{
	if (sig_)
	{
		int width;
		//Find the variable from the references set.
		if (vars.find(sig_->name().str()) != vars.end())
		{
			ostringstream name;
			unsigned temp_space, temp_time;

			RefVar *var = vars[sig_->name().str()];
			SmtVar *sv = new SmtVar;

			width = lwid_;

			temp_space = var->time == cur_time ? temp_space + 1 : 1;
			temp_time = cur_time;

			name << var->name << "_" << temp_time << "_" << temp_space;
			sv->smtname = name.str();
			sv->basename = var->name;
			sv->lsb = var->lsb;
			sv->msb = var->msb;
			sv->width = var->width;
			sv->type = var->ptype;
			sv->temp_flag = false;
			var->record = true;

			if (base_type)
			{
				var->space = temp_space;
				var->time = temp_time;
			}

			used.insert(sv);
			refs.insert(sv->basename);
			//
			if (width != sv->width)
			{
				if (const NetEConst *lsi_ = dynamic_cast<const NetEConst *>(base_))
				{
					long lsi = lsi_->value().as_ulong();
					expr << "((_ extract " << lsi + lwid_ - 1 << " " << lsi << ")" << name.str() << ")";
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
		else
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
void NetCondit::dump_smt(ostream &o, map<string, RefVar *> &vars, set<SmtVar *> &used, bool result, int caseitemidx, Module *md, set<string> &refs) const
{
	ostringstream expr;
	int width = expr_->dump_smt(vars, used, expr, md);
	assert(width != SMT_NULL);
	//condit expression is a bool
	string res;
	if (width == SMT_BOOL)
	{
		res = result ? "true" : "false";
		o << "(assert(= " << expr.str() << " " << res << "))" << endl;
	}
	else
	{
		ostringstream expr_bool;
		res = result ? "distinct" : "=";
		if (width == SMT_INT)
			expr_bool << "(" << res << " " << expr.str() << " 0)";
		else
			bv_compare_zero(expr, res, width, expr_bool);
		o << "(assert" << expr_bool.str() << ")" << endl;
	}
}

/*
  Generate SMT-LIB2 statement for a while cond expression.
*/
void NetWhile::dump_smt(ostream &o, map<string, RefVar *> &vars, set<SmtVar *> &used, bool result, int caseitemidx, Module *md, set<string> &refs) const
{
	ostringstream expr;
	int width = cond_->dump_smt(vars, used, expr, md);
	assert(width != SMT_NULL);
	//condit expression is a bool
	string res;
	if (width == SMT_BOOL)
	{
		res = result ? "true" : "false";
		o << "(assert(= " << expr.str() << " " << res << "))" << endl;
	}
	else
	{
		ostringstream expr_bool;
		res = result ? "distinct" : "=";
		if (width == SMT_INT)
			expr_bool << "(" << res << " " << expr.str() << " 0)";
		else
			bv_compare_zero(expr, res, width, expr_bool);
		o << "(assert" << expr_bool.str() << ")" << endl;
	}
}

/*
  Generate SMT-LIB2 statement for a repeat cond expression.
*/
void NetRepeat::dump_smt(ostream &o, map<string, RefVar *> &vars, set<SmtVar *> &used, bool result, int caseitemidx, Module *md, set<string> &refs) const
{
	ostringstream expr;
	int width = expr_->dump_smt(vars, used, expr, md);
	assert(width != SMT_NULL);
	//condit expression is a bool
	string res;
	if (width == SMT_BOOL)
	{
		res = result ? "true" : "false";
		o << "(assert(= " << expr.str() << " " << res << "))" << endl;
	}
	else
	{
		ostringstream expr_bool;
		res = result ? "distinct" : "=";
		if (width == SMT_INT)
			expr_bool << "(" << res << " " << expr.str() << " 0)";
		else
			bv_compare_zero(expr, res, width, expr_bool);
		o << "(assert" << expr_bool.str() << ")" << endl;
	}
}

/*
  Generate SMT-LIB2 statement for a case cond expression.
*/
void NetCase::dump_smt(ostream &o, map<string, RefVar *> &vars, set<SmtVar *> &used, bool result, int caseitemidx, Module *md, set<string> &refs) const
{
	ostringstream c_expr;
	int c_width;
	c_width = expr_->dump_smt(vars, used, c_expr, md);
	assert(c_width != SMT_NULL && c_width != SMT_BOOL);
	//Line of case item, build a equal smt assert for this case item with cond expression.
	if (caseitemidx >= 0)
	{
		ostringstream i_expr;
		int i_width;
		i_width = items_[caseitemidx].guard->dump_smt(vars, used, i_expr, md);
		ostringstream c_equal, i_equal;
		if (c_width == i_width && c_width != SMT_INT)
		{
			c_equal << c_expr.str();
			i_equal << i_expr.str();
		}
		else
		{
			if (c_width != SMT_INT)
				bv_to_int(c_expr, c_equal);
			else
				c_equal << c_expr.str();
			if (i_width != SMT_INT)
				bv_to_int(i_expr, i_equal);
			else
				i_equal << i_expr.str();
		}
		o << "(assert(= " << c_equal.str() << " " << i_equal.str() << "))" << endl;
	}
	//Default line, build not equal SMT asserts for every case item with cond expression.
	else
	{
		SmtVar *sv = new SmtVar;
		sv->smtname = "Temp_" + to_string(used.size());
		sv->type = "None";
		sv->temp_flag = true;
		used.insert(sv);
		ostringstream c_expr_int;
		if (c_width == SMT_INT)
			c_expr_int << c_expr.str();
		else
			bv_to_int(c_expr, c_expr_int);
		o << "(assert(= " << sv->smtname << " " << c_expr_int.str() << "))" << endl;

		for (int idx = 0; idx < items_.size(); idx++)
		{
			if (items_[idx].guard)
			{
				ostringstream i_expr;
				int i_width;
				i_width = items_[idx].guard->dump_smt(vars, used, i_expr, md);
				ostringstream i_expr_int;
				if (i_width != SMT_INT)
					bv_to_int(i_expr, i_expr_int);
				else
					i_expr_int << i_expr.str();
				o << "(assert(distinct " << sv->smtname << " " << i_expr_int.str() << "))" << endl;
			}
		}
	}
}

void NetProc::dump_design_smt(ostream &o, InstanModule *instan, unordered_map<SmtDefine*, bool>& change, list<pair<string, bool>> &condit, unsigned &tempid) const
{
	cout << get_fileline() << "(?" << typeid(*this).name() << "?)";
}

void NetProcTop::dump_design_smt(ostream &o, InstanModule *instan, unordered_map<SmtDefine*, bool>& change, unsigned &tempid) const
{
	list<pair<string, bool>> condit;
	const PEventStatement* es = dynamic_cast<const PEventStatement*>(event);
	es->dump_design_smt(o, instan, cfg, condit, tempid);
	statement_->dump_design_smt(o, instan, change, condit, tempid);
	string name = condit.front().first;
	for(string s : cfg->defs)
	{
		if(instan->define.find(s) != instan->define.end())
		{
			SmtDefine* var = instan->define[s];
			o << "(assert (=> (not " << name << ") (= "  << var->getName() << " " << var->getLastName() << ")))" << endl;
		}
	}
}

void NetEvWait::dump_design_smt(ostream &o, InstanModule *instan, unordered_map<SmtDefine*, bool>& change, list<pair<string, bool>> &condit, unsigned &tempid) const
{
	if(statement_)
		statement_->dump_design_smt(o, instan, change, condit, tempid);
}

void PEventStatement::dump_design_smt(ostream &o, InstanModule *instan, Cfg *cfg, list<pair<string, bool>> &condit, unsigned& tempid) const
{
	string name = "Temp" + to_string(tempid++);
	declareConstBool(o, name);
	list<string> conds;
	if (cfg->sync)
	{
		for (unsigned i = 0; i < expr_.count(); i++)
		{
			PEEvent *event = dynamic_cast<PEEvent *>(expr_[i]);
			perm_string name = dynamic_cast<PEIdent *>(event->expr())->path().front().name;
			PEEvent::edge_t tp = event->type();

			assert(instan->define.find(name.str()) != instan->define.end());
			SmtDefine *var = instan->define[name.str()];
			ostringstream s;

			if (tp == PEEvent::edge_t::POSEDGE)
			{
				s << "(and "
					<< "(= " << var->getLastName() << " #b0)"
					<< "(= " << var->getName() << " #b1) )";
			}
			else if (tp == PEEvent::edge_t::NEGEDGE)
			{
				s << "(and "
					<< "(= " << var->getLastName() << " #b1)"
					<< "(= " << var->getName() << " #b0) )";
			}
			else
			{
				cerr << event->get_fileline() << " ";
				event->dump(cerr);
				cerr << " is unsupported now!" << endl;
				exit(1);
			}
			conds.push_back(s.str());
		}
	}
	//Always like always@(a or b) or always@(*)
	else
	{
		//Get refs in cfg, if this process need to replay, one signal or port changed at least.
		for (string ref : cfg->refs)
		{
			if(instan->define.find(ref) != instan->define.end())
			{
				SmtDefine *var = instan->define[ref];
				ostringstream s;
				s << "(distinct " << var->getName() << " " << var->getLastName() << ")";
				conds.push_back(s.str());
			}
		}
	}
	if (conds.size() == 1)
	{
		assertSingelStatement(o, name, conds.front(), "=");
	}
	else
	{
		o << "(assert (= " << name << " (or";
		for(string c : conds)
		{
			o << " " << c;
		}
		o << ") ) )" << endl;
	}
	condit.push_back(make_pair(name, true));
}

void NetBlock::dump_design_smt(ostream &o, InstanModule *instan, unordered_map<SmtDefine*, bool>& change, list<pair<string, bool>> &condit, unsigned &tempid) const
{
	if (last_)
	{
		NetProc *cur = last_;
		do
		{
			cur = cur->next_;
			cur->dump_design_smt(o, instan, change, condit, tempid);

		} while (cur != last_);
	}
}

void NetCondit::dump_design_smt(ostream &o, InstanModule *instan, unordered_map<SmtDefine*, bool>& change, list<pair<string, bool>> &condit, unsigned &tempid) const
{
	ostringstream expr;
	int width = expr_->dump_design_smt(expr, instan, change, tempid);
	string name = "Temp" + to_string(tempid++);
	declareConstBool(o, name);
	assertConditStatement(o, expr, width, name);
	condit.push_back(make_pair(name, true));
	if_->dump_design_smt(o, instan, change, condit, tempid);
	condit.back().second = false;
	if(else_)
		else_->dump_design_smt(o, instan, change, condit, tempid);
	condit.pop_back();
}

void NetCase::dump_design_smt(ostream &o, InstanModule *instan, unordered_map<SmtDefine*, bool>& change, list<pair<string, bool>> &condit, unsigned &tempid) const
{
	unsigned n = 0;
	ostringstream c_expr;
	int c_width = expr_->dump_design_smt(c_expr, instan, change, tempid);
	assert(c_width != SMT_INT);
	for (int idx = 0; idx < items_.size(); idx++)
	{
		if (items_[idx].guard)
		{
			ostringstream i_expr;
			int i_width = items_[idx].guard->dump_design_smt(i_expr, instan, change, tempid);
			assert(i_width != SMT_INT);
			ostringstream compare;
			assertEqual(compare, c_expr, c_width, i_expr, i_width, "=");
			string name = "Temp" + to_string(tempid++);
			declareConstBool(o, name);
			condit.push_back(make_pair(name, true));
			assertSingelStatement(o, name, compare.str(), "=");
			n++;
		}
		if (items_[idx].statement)
			items_[idx].statement->dump_design_smt(o, instan, change, condit, tempid);
		condit.back().second = false;
	}
	while (n)
	{
		condit.pop_back();
		n--;
	}
}

int NetAssign_::dump_design_smt(ostream& out, ostringstream& expr, InstanModule* instan, unordered_map<SmtDefine*, bool>& change, list<pair<string, bool> >& condit, unsigned& tempid, bool base) const
{
	if (sig_)
	{
		int width;
		//Find the variable from the references set.
		if (instan->define.find(sig_->name().str()) != instan->define.end())
		{
			ostringstream name;
			SmtDefine* var = instan->define[sig_->name().str()];

			width = lwid_ > var->getWidth() ? var->getWidth() : lwid_;
            bool part_select;
            int left, right;

			if (width < var->getWidth())
			{
				if (const NetEConst *lsi_ = dynamic_cast<const NetEConst *>(base_))
				{
					long lsi = lsi_->value().as_ulong();
                    left = lsi + lwid_ - 1;
                    right = lsi;
				}
				else
				{
                    left = lwid_ - 1;
                    right = 0;
				}
                part_select = true;
			}
			//
			else
			{
                part_select = false;
                left = var->symbol->get_msb();
                right = var->symbol->get_lsb();
			}

            if(base || change.find(var) == change.end())
                var->update(out, SmtDefine::UpdateType::SPACEUPDATE, 0, nullptr, left, right);
            if(!base)
                var->bit_vec->full_initial();

            change[var] = base;
            name << var->getName();

            if(part_select)
                expr << "((_ extract " << left - var->symbol->get_lsb() << " " << right - var->symbol->get_lsb() << ")" << name.str() << ")";
            else
                expr << name.str();

		}
		else
		{
			cerr << sig_->name().str() << " can't find!" << endl;
			exit(1);
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

void NetAssignBase::dump_design_smt(ostream& out, InstanModule* instan, unordered_map<SmtDefine*, bool>& change, list<pair<string, bool> >& condit, unsigned& tempid) const
{
	ostringstream l_expr;
	int l_width = 0;
	list<string> items;
	NetAssign_* cur = lval_;

    //Generate the respective SMT formats for left and right expressions.
    //Then verify the formats of expressions are correctly.
    ostringstream r_expr;

    int r_width = rval_->dump_design_smt(r_expr, instan, change, tempid);

    assert(r_width != SMT_INT);

	while(cur) {
		ostringstream itemexpr;
		int itemwidth = cur->dump_design_smt(out, itemexpr, instan, change, condit, tempid, true);
		items.push_front(itemexpr.str());
		l_width += itemwidth;
		cur = cur->more;
	}

	assert(!items.empty());

	if(items.size() == 1)
		l_expr << items.front();
	else
	{
		l_expr << "(concat";
		for(string item : items)
		{
			l_expr << " " << item;
		}
		l_expr << ")";
	}

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
	
	ostringstream res;
	res << "(= " << l_expr.str() << " " << r_equal.str() << ")";
	assertStatement(out, condit, res);
}

void NetAssignNB::dump_design_smt(ostream& out, InstanModule* instan, unordered_map<SmtDefine*, bool>& change, list<pair<string, bool> >& condit, unsigned& tempid) const
{
	ostringstream l_expr;
	int l_width = 0;
	list<string> items;
	const NetAssign_* cur = lval();

    //Generate the respective SMT formats for left and right expressions.
    //Then verify the formats of expressions are correctly.
    ostringstream r_expr;

    int r_width = rval()->dump_design_smt(r_expr, instan, change, tempid);

    assert(r_width != SMT_INT);

	while(cur) {
		ostringstream itemexpr;
		int itemwidth = cur->dump_design_smt(out, itemexpr, instan, change, condit, tempid, false);
		items.push_front(itemexpr.str());
		l_width += itemwidth;
		cur = cur->more;
	}

	assert(!items.empty());

	if(items.size() == 1)
		l_expr << items.front();
	else
	{
		l_expr << "(concat";
		for(string item : items)
		{
			l_expr << " " << item;
		}
		l_expr << ")";
	}

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

	ostringstream res;
	res << "(= " << l_expr.str() << " " << r_equal.str() << ")";
	assertStatement(out, condit, res);
}


