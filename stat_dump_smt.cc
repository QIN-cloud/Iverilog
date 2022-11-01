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


