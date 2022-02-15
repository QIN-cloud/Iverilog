/*
 * Copyright (c) 1998-2019 Stephen Williams (steve@icarus.com)
 *
 *    This source code is free software; you can redistribute it
 *    and/or modify it in source code form under the terms of the GNU
 *    General Public License as published by the Free Software
 *    Foundation; either version 2 of the License, or (at your option)
 *    any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

# include "config.h"

# include  "Statement.h"
# include  "PExpr.h"
# include  "ivl_assert.h"

svector<unsigned>& PProcess::get_linenos()
{
	svector<unsigned>*tmp = new svector<unsigned>();
	svector<unsigned> tmp1;

	//tmp = new svector<unsigned>(*tmp, get_lineno());
	if(statement_)
	{
		tmp1 = statement_->get_linenos();
		for(unsigned i = 0; i < tmp1.count(); ++i)
			tmp = new svector<unsigned>(*tmp, tmp1[i]);
			//tmp->insert(tmp1[i]);
	}
	return *tmp;
}

svector<unsigned>& Statement::get_linenos()
{
	svector<unsigned>*tmp = new svector<unsigned>();
	tmp = new svector<unsigned>(*tmp, get_lineno());
	return *tmp;
}

set<string>& Statement::get_funcs()
{
	set<string>* tmp = new set<string>;
	return *tmp;
}

svector<string>& Statement::get_vars()
{
      svector<string>*tmp = new svector<string>;
      return *tmp;
}

set<string>& Statement::get_funcname()
{
      set<string>* tmp = new set<string>;
      return *tmp;
}

svector<string>& PAssign::get_vars()
{
      svector<string>*tmp = new svector<string>();
      tmp = new svector<string>(*tmp, lval()->get_vars());
      tmp = new svector<string>(*tmp, rval()->get_vars());
      return *tmp;
}

set<string>& PAssign::get_funcname()
{
      set<string>* tmp = new set<string>;
      set<string> lval_set = lval()->get_funcname();
      set<string>::iterator lval_iter;
      set<string> rval_set = rval()->get_funcname();
      set<string>::iterator rval_iter;
      if(lval())
      {
      	for(lval_iter = lval_set.begin(); lval_iter != lval_set.end(); lval_iter++)
      		tmp->insert(*lval_iter);
      }
      if(rval()){
      	for(rval_iter = rval_set.begin(); rval_iter != rval_set.end(); rval_iter++)
      		tmp->insert(*rval_iter);
      }
      return *tmp;
}

set<string>& PAssign::get_funcs()
{
	set<string>* tmp = new set<string>;
	set<string>::const_iterator pos;
	set<string> tmp1;
	if(lval())
	{
		tmp1 = lval()->get_funcname();
		for(pos = tmp1.begin(); pos != tmp1.end(); ++pos)
			tmp->insert(*pos);
	}
	if(rval())
	{
		tmp1 = rval()->get_funcname();
		for(pos = tmp1.begin(); pos != tmp1.end(); ++pos)
			tmp->insert(*pos);
	}
	if(event_)
	{
		tmp1 = event_->get_funcs();
		for(pos = tmp1.begin(); pos != tmp1.end(); ++pos)
			tmp->insert(*pos);
	}
	if(delay_)
	{
		tmp1 = delay_->get_funcname();
		for(pos = tmp1.begin(); pos != tmp1.end(); ++pos)
			tmp->insert(*pos);
	}
	return *tmp;
}

svector<string>& PAssignNB::get_vars()
{
      svector<string>*tmp = new svector<string>();
      tmp = new svector<string>(*tmp, lval()->get_vars());
      tmp = new svector<string>(*tmp, rval()->get_vars());
      return *tmp;
}

set<string>& PAssignNB::get_funcname()
{
      set<string>* tmp = new set<string>;
      tmp->insert(lval()->get_funcname().begin(), lval()->get_funcname().end());
      tmp->insert(rval()->get_funcname().begin(), rval()->get_funcname().end());
      return *tmp;
}

set<string>& PAssignNB::get_funcs()
{
	set<string>* tmp = new set<string>;
	set<string>::const_iterator pos;
	set<string> tmp1;
	if(lval())
	{
		tmp1 = lval()->get_funcname();
		for(pos = tmp1.begin(); pos != tmp1.end(); ++pos)
			tmp->insert(*pos);
	}
	if(rval())
	{
		tmp1 = rval()->get_funcname();
		for(pos = tmp1.begin(); pos != tmp1.end(); ++pos)
			tmp->insert(*pos);
	}
	if(event_)
	{
		tmp1 = event_->get_funcs();
		for(pos = tmp1.begin(); pos != tmp1.end(); ++pos)
			tmp->insert(*pos);
	}
	if(delay_)
	{
		tmp1 = delay_->get_funcname();
		for(pos = tmp1.begin(); pos != tmp1.end(); ++pos)
			tmp->insert(*pos);
	}
	return *tmp;
}

svector<unsigned>& PBlock::get_linenos()
{
	svector<unsigned>* tmp = new svector<unsigned>();
	for(unsigned idx = 0; idx < list_.size(); ++idx)
	{
		svector<unsigned>*tmps = new svector<unsigned>(*tmp, list_[idx]->get_linenos());
		delete tmp;
		tmp = tmps;
	}
	return *tmp;
}

set<string>& PBlock::get_funcs()
{
	set<string>* tmp = new set<string>;
	set<string>::const_iterator pos;
	set<string> tmp1;
	for(unsigned idx = 0; idx < list_.size(); ++idx)
	{
		tmp1 = list_[idx]->get_funcs();
		for(pos = tmp1.begin(); pos != tmp1.end(); ++pos)
			tmp->insert(*pos);
	}
	return *tmp;
}

set<string>& PCallTask::get_funcs()
{
	set<string>* tmp = new set<string>;
	tmp->insert(peek_tail_name(path_).str());
	return *tmp;
}

set<string>& PCase::get_funcs()
{
	set<string>* tmp = new set<string>;
	set<string>::const_iterator pos;
	set<string> tmp1;
	tmp1 = expr_->get_funcname();
	for(pos = tmp1.begin(); pos != tmp1.end(); ++pos)
		tmp->insert(*pos);
	for(unsigned i=0; i < items_->count(); ++i)
	{
		tmp1 = (*items_)[i]->stat->get_funcs();
		for(pos = tmp1.begin(); pos != tmp1.end(); ++pos)
			tmp->insert(*pos);
		for(list<PExpr*>::iterator it = (*items_)[i]->expr.begin();
             it != (*items_)[i]->expr.end(); ++it)
		{
			tmp1 = (*it)->get_funcname();
			for(pos = tmp1.begin(); pos != tmp1.end(); ++pos)
				tmp->insert(*pos);
		}
	}
	return *tmp;
}

svector<unsigned>& PCase::get_linenos()
{
	svector<unsigned>* tmp = new svector<unsigned>();
	tmp = new svector<unsigned>(*tmp, get_lineno());
	for(unsigned idx = 0; idx < items_->count(); ++idx)
	{
	    tmp = new svector<unsigned>(*tmp, (*items_)[idx]->stat->get_linenos());
	}
	return *tmp;
}

set<string>& PCAssign::get_funcs()
{
	set<string>* tmp = new set<string>;
	set<string>::const_iterator pos;
	set<string> tmp1;
	tmp1 = lval_->get_funcname();
	for(pos = tmp1.begin(); pos != tmp1.end(); ++pos)
		tmp->insert(*pos);
	tmp1 = expr_->get_funcname();
	for(pos = tmp1.begin(); pos != tmp1.end(); ++pos)
		tmp->insert(*pos);
	return *tmp;
}

set<string>& PCondit::get_funcs()
{
	set<string>* tmp = new set<string>;
	set<string>::const_iterator pos;
	set<string> tmp1;
	tmp1 = expr_->get_funcname();
	for(pos = tmp1.begin(); pos != tmp1.end(); ++pos)
		tmp->insert(*pos);
	if(if_)
	{
		tmp1 = if_->get_funcs();
		for(pos = tmp1.begin(); pos != tmp1.end(); ++pos)
			tmp->insert(*pos);
	}
	if(else_)
	{
		tmp1 = else_->get_funcs();
		for(pos = tmp1.begin(); pos != tmp1.end(); ++pos)
			tmp->insert(*pos);
	}
	return *tmp;
}

svector<unsigned>& PCondit::get_linenos()
{
	svector<unsigned>* tmp = new svector<unsigned>();
        tmp = new svector<unsigned>(*tmp, get_lineno());
	if(if_)
		tmp = new svector<unsigned>(*tmp, if_->get_linenos());
	if(else_)
		tmp = new svector<unsigned>(*tmp, else_->get_linenos());
	return *tmp;
}

set<string>& PDeassign::get_funcs()
{
	set<string>* tmp = new set<string>;
	set<string>::const_iterator pos;
	set<string> tmp1;
	tmp1 = lval_->get_funcname();
	for(pos = tmp1.begin(); pos != tmp1.end(); ++pos)
		tmp->insert(*pos);
	return *tmp;
}

set<string>& PDelayStatement::get_funcs()
{
	set<string>* tmp = new set<string>;
	set<string>::const_iterator pos;
	set<string> tmp1;
	tmp1 = delay_->get_funcname();
	for(pos = tmp1.begin(); pos != tmp1.end(); ++pos)
		tmp->insert(*pos);
	if(statement_)
	{
		tmp1 = statement_->get_funcs();
		for(pos = tmp1.begin(); pos != tmp1.end(); ++pos)
			tmp->insert(*pos);
	}
	return *tmp;
}

svector<unsigned>& PDelayStatement::get_linenos()
{
	svector<unsigned>* tmp = new svector<unsigned>(statement_->get_linenos());
	return *tmp;
}

set<string>& PEventStatement::get_funcs()
{
	set<string>* tmp = new set<string>;
	set<string>::const_iterator pos;
	set<string> tmp1;
	for(unsigned idx = 0; idx < expr_.count(); ++idx)
	{
		tmp1 = expr_[idx]->get_funcname();
		for(pos = tmp1.begin(); pos != tmp1.end(); ++pos)
			tmp->insert(*pos);
	}
	if(statement_)
	{
		tmp1 = statement_->get_funcs();
		for(pos = tmp1.begin(); pos != tmp1.end(); ++pos)
			tmp->insert(*pos);
	}
	return *tmp;
}

set<string>& PForce::get_funcs()
{
	set<string>* tmp = new set<string>;
	set<string>::const_iterator pos;
	set<string> tmp1;
	tmp1 = lval_->get_funcname();
	for(pos = tmp1.begin(); pos != tmp1.end(); ++pos)
		tmp->insert(*pos);
	tmp1 = expr_->get_funcname();
	for(pos = tmp1.begin(); pos != tmp1.end(); ++pos)
		tmp->insert(*pos);
	return *tmp;
}

set<string>& PForever::get_funcs()
{
	set<string>* tmp = new set<string>;
	set<string>::const_iterator pos;
	set<string> tmp1;
	if(statement_)
	{
		tmp1 = statement_->get_funcs();
		for(pos = tmp1.begin(); pos != tmp1.end(); ++pos)
			tmp->insert(*pos);
	}
	return *tmp;
}

svector<unsigned>& PForever::get_linenos()
{
	svector<unsigned>* tmp = new svector<unsigned>(statement_->get_linenos());
	return *tmp;
}

set<string>& PForStatement::get_funcs()
{
	set<string>* tmp = new set<string>;
	set<string>::const_iterator pos;
	set<string> tmp1;
	if(name1_)
	{
		tmp1 = name1_->get_funcname();
		for(pos = tmp1.begin(); pos != tmp1.end(); ++pos)
			tmp->insert(*pos);
	}
	if(expr1_)
	{
		tmp1 = expr1_->get_funcname();
		for(pos = tmp1.begin(); pos != tmp1.end(); ++pos)
			tmp->insert(*pos);
	}
	if(step_)
	{
		tmp1 = step_->get_funcname();
		for(pos = tmp1.begin(); pos != tmp1.end(); ++pos)
			tmp->insert(*pos);
	}
	if(cond_)
	{
		tmp1 = cond_->get_funcname();
		for(pos = tmp1.begin(); pos != tmp1.end(); ++pos)
			tmp->insert(*pos);
	}
	if(statement_)
	{
		tmp1 = statement_->get_funcs();
		for(pos = tmp1.begin(); pos != tmp1.end(); ++pos)
			tmp->insert(*pos);
	}
	return *tmp;
}

svector<unsigned>& PForStatement::get_linenos()
{
	svector<unsigned>* tmp = new svector<unsigned>();
	tmp = new svector<unsigned>(*tmp, get_lineno());
	tmp = new svector<unsigned>(*tmp, statement_->get_linenos());
	return *tmp;
}

set<string>& PProcess::get_funcs()
{
	set<string>* tmp = new set<string>;
	set<string>::const_iterator pos;
	set<string> tmp1;
	if(statement_)
	{
		tmp1 = statement_->get_funcs();
		for(pos = tmp1.begin(); pos != tmp1.end(); ++pos)
			tmp->insert(*pos);
	}
	return *tmp;
}

set<string>& PRelease::get_funcs()
{
	set<string>* tmp = new set<string>;
	set<string>::const_iterator pos;
	set<string> tmp1;
	tmp1 = lval_->get_funcname();
	for(pos = tmp1.begin(); pos != tmp1.end(); ++pos)
		tmp->insert(*pos);
	return *tmp;
}

set<string>& PRepeat::get_funcs()
{
	set<string>* tmp = new set<string>;
	set<string>::const_iterator pos;
	set<string> tmp1;
	tmp1 = expr_->get_funcname();
	for(pos = tmp1.begin(); pos != tmp1.end(); ++pos)
		tmp->insert(*pos);
	if(statement_)
	{
		tmp1 = statement_->get_funcs();
		for(pos = tmp1.begin(); pos != tmp1.end(); ++pos)
			tmp->insert(*pos);
	}
	return *tmp;
}

set<string>& PWhile::get_funcs()
{
	set<string>* tmp = new set<string>;
	set<string>::const_iterator pos;
	set<string> tmp1;
	tmp1 = cond_->get_funcname();
		for(pos = tmp1.begin(); pos != tmp1.end(); ++pos)
			tmp->insert(*pos);
	if(statement_)
	{
		tmp1 = statement_->get_funcs();
		for(pos = tmp1.begin(); pos != tmp1.end(); ++pos)
			tmp->insert(*pos);
	}
	return *tmp;
}

svector<unsigned>& PWhile::get_linenos()
{
	svector<unsigned>* tmp = new svector<unsigned>();
	tmp = new svector<unsigned>(*tmp, get_lineno());
	tmp = new svector<unsigned>(*tmp, statement_->get_linenos());
	return *tmp;
}

PNBTrigger::PNBTrigger(const pform_name_t&ev, PExpr*dly)
: event_(ev), dly_(dly)
{
}

PNBTrigger::~PNBTrigger()
{
}

Statement::~Statement()
{
}

PAssign_::PAssign_(PExpr*lval__, PExpr*ex, bool is_constant)
: event_(0), count_(0), lval_(lval__), rval_(ex), is_constant_(is_constant)
{
      delay_ = 0;
}

PAssign_::PAssign_(PExpr*lval__, PExpr*de, PExpr*ex)
: event_(0), count_(0), lval_(lval__), rval_(ex), is_constant_(false)
{
      delay_ = de;
}

PAssign_::PAssign_(PExpr*lval__, PExpr*cnt, PEventStatement*ev, PExpr*ex)
: event_(ev), count_(cnt), lval_(lval__), rval_(ex), is_constant_(false)
{
      delay_ = 0;
}

PAssign_::~PAssign_()
{
      delete lval_;
      delete rval_;
}

PAssign::PAssign(PExpr*lval__, PExpr*ex)
: PAssign_(lval__, ex, false), op_(0)
{
}

PAssign::PAssign(PExpr*lval__, char op, PExpr*ex)
: PAssign_(lval__, ex, false), op_(op)
{
}

PAssign::PAssign(PExpr*lval__, PExpr*d, PExpr*ex)
: PAssign_(lval__, d, ex), op_(0)
{
}

PAssign::PAssign(PExpr*lval__, PExpr*cnt, PEventStatement*d, PExpr*ex)
: PAssign_(lval__, cnt, d, ex), op_(0)
{
}

PAssign::PAssign(PExpr*lval__, PExpr*ex, bool is_constant)
: PAssign_(lval__, ex, is_constant), op_(0)
{
}

PAssign::~PAssign()
{
}

PAssignNB::PAssignNB(PExpr*lval__, PExpr*ex)
: PAssign_(lval__, ex, false)
{
}

PAssignNB::PAssignNB(PExpr*lval__, PExpr*d, PExpr*ex)
: PAssign_(lval__, d, ex)
{
}

PAssignNB::PAssignNB(PExpr*lval__, PExpr*cnt, PEventStatement*d, PExpr*ex)
: PAssign_(lval__, cnt, d, ex)
{
}

PAssignNB::~PAssignNB()
{
}

PBlock::PBlock(perm_string n, LexicalScope*parent, BL_TYPE t)
: PScope(n, parent), bl_type_(t)
{
}

PBlock::PBlock(BL_TYPE t)
: PScope(perm_string()), bl_type_(t)
{
}

PBlock::~PBlock()
{
      for (unsigned idx = 0 ;  idx < list_.size() ;  idx += 1)
	    delete list_[idx];
}

bool PBlock::var_init_needs_explicit_lifetime() const
{
      return default_lifetime == STATIC;
}

PChainConstructor* PBlock::extract_chain_constructor()
{
      if (list_.empty())
	    return 0;

      if (PChainConstructor*res = dynamic_cast<PChainConstructor*> (list_[0])) {
	    for (size_t idx = 0 ; idx < list_.size()-1 ; idx += 1)
		  list_[idx] = list_[idx+1];
	    list_.resize(list_.size()-1);
	    return res;
      }

      return 0;
}

void PBlock::set_join_type(PBlock::BL_TYPE type)
{
      assert(bl_type_ == BL_PAR);
      assert(type==BL_PAR || type==BL_JOIN_NONE || type==BL_JOIN_ANY);
      bl_type_ = type;
}

void PBlock::set_statement(const vector<Statement*>&st)
{
      list_ = st;
}

void PBlock::push_statement_front(Statement*that)
{
      ivl_assert(*this, bl_type_==BL_SEQ);

      list_.resize(list_.size()+1);
      for (size_t idx = list_.size()-1 ; idx > 0 ; idx -= 1)
	    list_[idx] = list_[idx-1];

      list_[0] = that;
}

PNamedItem::SymbolType PBlock::symbol_type() const
{
      return BLOCK;
}

PCallTask::PCallTask(const pform_name_t&n, const list<PExpr*>&p)
: package_(0), path_(n), parms_(p.size())
{
      list<PExpr*>::const_iterator cur = p.begin();
      for (size_t idx = 0 ; idx < parms_.size() ; idx += 1) {
	    parms_[idx] = *cur;
	    ++cur;
      }
      assert(cur == p.end());
}

PCallTask::PCallTask(PPackage*pkg, const pform_name_t&n, const list<PExpr*>&p)
: package_(pkg), path_(n), parms_(p.size())
{
      list<PExpr*>::const_iterator cur = p.begin();
      for (size_t idx = 0 ; idx < parms_.size() ; idx += 1) {
	    parms_[idx] = *cur;
	    ++cur;
      }
      assert(cur == p.end());
}

PCallTask::PCallTask(perm_string n, const list<PExpr*>&p)
: package_(0), parms_(p.size())
{
      list<PExpr*>::const_iterator cur = p.begin();
      for (size_t idx = 0 ; idx < parms_.size() ; idx += 1) {
	    parms_[idx] = *cur;
	    ++cur;
      }
      assert(cur == p.end());
      path_.push_back(name_component_t(n));
}

PCallTask::~PCallTask()
{
}

const pform_name_t& PCallTask::path() const
{
      return path_;
}

PCase::PCase(ivl_case_quality_t q, NetCase::TYPE t, PExpr*ex, svector<PCase::Item*>*l)
: quality_(q), type_(t), expr_(ex), items_(l)
{
}

PCase::~PCase()
{
      delete expr_;
      for (unsigned idx = 0 ;  idx < items_->count() ;  idx += 1)
	    if ((*items_)[idx]->stat) delete (*items_)[idx]->stat;

      delete[]items_;
}

PCAssign::PCAssign(PExpr*l, PExpr*r)
: lval_(l), expr_(r)
{
}

PCAssign::~PCAssign()
{
      delete lval_;
      delete expr_;
}

PChainConstructor::PChainConstructor(const list<PExpr*>&parms)
: parms_(parms.size())
{
      list<PExpr*>::const_iterator cur = parms.begin();
      for (size_t idx = 0 ; idx < parms_.size() ; idx += 1) {
	    parms_[idx] = *cur;
	    ++cur;
      }
      assert(cur == parms.end());
}

PChainConstructor::~PChainConstructor()
{
}

PCondit::PCondit(PExpr*ex, Statement*i, Statement*e)
: expr_(ex), if_(i), else_(e)
{
}

PCondit::~PCondit()
{
      delete expr_;
      delete if_;
      delete else_;
}

PDeassign::PDeassign(PExpr*l)
: lval_(l)
{
}

PDeassign::~PDeassign()
{
      delete lval_;
}


PDelayStatement::PDelayStatement(PExpr*d, Statement*st)
: delay_(d), statement_(st)
{
}

PDelayStatement::~PDelayStatement()
{
}

PDisable::PDisable(const pform_name_t&sc)
: scope_(sc)
{
}

PDisable::~PDisable()
{
}

PDoWhile::PDoWhile(PExpr*ex, Statement*st)
: cond_(ex), statement_(st)
{
}

PDoWhile::~PDoWhile()
{
      delete cond_;
      delete statement_;
}

PEventStatement::PEventStatement(const svector<PEEvent*>&ee)
: expr_(ee), statement_(0), always_sens_(false)
{
      assert(expr_.count() > 0);
}


PEventStatement::PEventStatement(PEEvent*ee)
: expr_(1), statement_(0), always_sens_(false)
{
      expr_[0] = ee;
}

PEventStatement::PEventStatement(bool always_sens)
: statement_(0), always_sens_(always_sens)
{
}

PEventStatement::~PEventStatement()
{
	// delete the events and the statement?
}

void PEventStatement::set_statement(Statement*st)
{
      statement_ = st;
}

bool PEventStatement::has_aa_term(Design*des, NetScope*scope)
{
      bool flag = false;
      for (unsigned idx = 0 ; idx < expr_.count() ; idx += 1) {
	    flag = expr_[idx]->has_aa_term(des, scope) || flag;
      }
      return flag;
}

PForce::PForce(PExpr*l, PExpr*r)
: lval_(l), expr_(r)
{
}

PForce::~PForce()
{
      delete lval_;
      delete expr_;
}

PForeach::PForeach(perm_string av, const list<perm_string>&ix, Statement*s)
: array_var_(av), index_vars_(ix.size()), statement_(s)
{
      size_t idx = 0;
      for (list<perm_string>::const_iterator cur = ix.begin()
		 ; cur != ix.end() ; ++cur)
	    index_vars_[idx++] = *cur;
}

PForeach::~PForeach()
{
      delete statement_;
}

PForever::PForever(Statement*s)
: statement_(s)
{
}

PForever::~PForever()
{
      delete statement_;
}

PForStatement::PForStatement(PExpr*n1, PExpr*e1, PExpr*cond,
			     Statement*step, Statement*st)
: name1_(n1), expr1_(e1), cond_(cond), step_(step), statement_(st)
{
}

PForStatement::~PForStatement()
{
}

PProcess::~PProcess()
{
      delete statement_;
}

PRelease::PRelease(PExpr*l)
: lval_(l)
{
}

PRelease::~PRelease()
{
      delete lval_;
}

PRepeat::PRepeat(PExpr*e, Statement*s)
: expr_(e), statement_(s)
{
}

PRepeat::~PRepeat()
{
      delete expr_;
      delete statement_;
}

PReturn::PReturn(PExpr*e)
: expr_(e)
{
}

PReturn::~PReturn()
{
      delete expr_;
}

PTrigger::PTrigger(PPackage*pkg, const pform_name_t&e)
: package_(pkg), event_(e)
{
}

PTrigger::~PTrigger()
{
}

PWhile::PWhile(PExpr*ex, Statement*st)
: cond_(ex), statement_(st)
{
}

PWhile::~PWhile()
{
      delete cond_;
      delete statement_;
}
